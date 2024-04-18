#include "../../../inc/MarlinConfigPre.h"

#if IS_DWIN_MARLINUI && HAS_GAMES

#define PERFORMANCE_COUNTERS 1

#include "../../menu/game/types.h" // includes e3v2/marlinui/game.h
#include "../../lcdprint.h"
#include "lcdprint_dwin.h"
#include "marlinui_dwin.h"

#if PERFORMANCE_COUNTERS == 1
  static uint32_t draw_call_cnt = 0;     // total number of draw calls in the current frame
  static millis_t frame_draw_millis = 0, // time spent drawing the frame 
                  frame_wait_millis = 0; // time spent waiting for the next frame

  #define DRAW_CALL_INC(subcall_cnt) draw_call_cnt++
#else
  #define DRAW_CALL_INC(subcall_cnt)
#endif

void MarlinGame::frame_start() {
  // clear the screen before each frame
  //dwinFrameClear(CLEAR_COLOR);

  // filling the play area should be faster than clearing the whole screen
  const uint16_t fg = dwin_font.fg;
  dwin_font.fg = COLOR_BG_BLACK;
  draw_box(0, 0, GAME_WIDTH, GAME_HEIGHT);
  dwin_font.fg = fg;

  dwin_font.index = DWIN_FONT_MENU;

  // reset performance counters
  #if PERFORMANCE_COUNTERS == 1
    draw_call_cnt = 0;
    frame_draw_millis = millis();
    frame_wait_millis = frame_draw_millis - frame_wait_millis; 
  #endif
}

void MarlinGame::frame_end() {
  #if PERFORMANCE_COUNTERS == 1
    const millis_t frame_wait = frame_wait_millis;
    frame_wait_millis = millis();
    frame_draw_millis = frame_wait_millis - frame_draw_millis;

    // calculate frames per deci-seconds (100 milliseconds)
    const uint32_t fpds = 100 / (frame_draw_millis + frame_wait);

    // format the performance counters as a string
    char perf_str[64];
    sprintf_P(
      perf_str,
      PSTR("d%04lu w%04lu c%04lu f%02lu "),
      frame_draw_millis,
      frame_wait,
      draw_call_cnt,
      fpds
    );

    // draw the performance counters at the (physical) origin of the screen
    const uint16_t fg = dwin_font.fg;
    const bool solid = dwin_font.solid;
    dwin_font.fg = RGB(0x1F, 0x3F, 0x00);
    dwin_font.solid = true;

    lcd_moveto_xy(0, 0);
    lcd_put_u8str(perf_str);

    dwin_font.fg = fg;
    dwin_font.solid = solid;
  #endif
}

void MarlinGame::set_color(const color color) {
  switch(color)
  {
    case color::BLACK:
      dwin_font.fg = COLOR_BG_BLACK;
      break;
    case color::WHITE:
    default:
      dwin_font.fg = COLOR_WHITE;
      break;

    // https://rgbcolorpicker.com/565/table
    case color::RED:
      dwin_font.fg = RGB(0x1F, 0x00, 0x00);
      break;
    case color::GREEN:
      dwin_font.fg = RGB(0x00, 0x3F, 0x00);
      break;
    case color::BLUE:
      dwin_font.fg = RGB(0x00, 0x00, 0x1F);
      break;
    case color::YELLOW:
      dwin_font.fg = RGB(0x1F, 0x3F, 0x00);
      break;
    case color::CYAN:
      dwin_font.fg = RGB(0x00, 0x3F, 0x1F);
      break;
    case color::MAGENTA:
      dwin_font.fg = RGB(0x1F, 0x00, 0x1F);
      break;
  }
}

void MarlinGame::draw_hline(const game_dim_t x, const game_dim_t y, const game_dim_t w) {
  // draw lines as boxes, since DWIN lines are always 1px wide but we want to scale them
  draw_box(x, y, w, 1);

  DRAW_CALL_INC(1);
}

void MarlinGame::draw_vline(const game_dim_t x, const game_dim_t y, const game_dim_t h) {
  // draw lines as boxes, since DWIN lines are always 1px wide but we want to scale them
  draw_box(x, y, 1, h);

  DRAW_CALL_INC(1);
}

void MarlinGame::draw_frame(const game_dim_t x, const game_dim_t y, const game_dim_t w, const game_dim_t h) {
  dwinDrawBox(
    0, // mode = frame
    dwin_font.fg, // color
    dwin_game::game_to_screen(x) + dwin_game::x_offset,
    dwin_game::game_to_screen(y) + dwin_game::y_offset,
    dwin_game::game_to_screen(w),
    dwin_game::game_to_screen(h)
  );

  DRAW_CALL_INC(0);
}

void MarlinGame::draw_box(const game_dim_t x, const game_dim_t y, const game_dim_t w, const game_dim_t h) {
  dwinDrawBox(
    1, // mode = fill
    dwin_font.fg, // color
    dwin_game::game_to_screen(x) + dwin_game::x_offset,
    dwin_game::game_to_screen(y) + dwin_game::y_offset,
    dwin_game::game_to_screen(w),
    dwin_game::game_to_screen(h)
  );

  DRAW_CALL_INC(0);
}

void MarlinGame::draw_pixel(const game_dim_t x, const game_dim_t y) {
  // draw pixels using boxes.
  // while DWIN protocol supports drawing points with different sizes, the
  // 0x02 'draw point' command is way slower per pixel than 0x05 'fill rectangle'
  // (0.4 us vs 0.14 us per pixel)
  draw_box(x, y, 1, 1);

  DRAW_CALL_INC(1);
}

void MarlinGame::draw_bitmap(const game_dim_t x, const game_dim_t y, const game_dim_t bytes_per_row, const game_dim_t rows, const pgm_bitmap_t bitmap) {
  // DWIN theorethically supports bitmaps since kernel 2.1, but most screens don't support it
  // (either because they use an older kernel version, or because they just (badly) emulate the DWIN protocol).
  // So instead, we have to fall back to drawing points manually.

  #if DISABLED(TJC_DISPLAY)
    // DWIN T5UI actually supports drawing multiple points in one go using the 0x02 'draw point' command, ever since kernel 1.2.
    // So we use that to draw the bitmap as a series of points, which should be faster than drawing rectangles using draw_pixel.
    // This will be somewhat slow, but way faster than drawing rectangles one by one.
    dwinDrawPointMap(
      dwin_font.fg, // color
      dwin_game::game_to_screen(1), // point size
      dwin_game::game_to_screen(1),
      dwin_game::game_to_screen(x) + dwin_game::x_offset, // x / y
      dwin_game::game_to_screen(y) + dwin_game::y_offset,
      bytes_per_row, // bitmap dimensions
      rows,
      bitmap // U8G bitmap format is compatible to DrawPointMap format
    );

    DRAW_CALL_INC(0);
  #else
    // TJC displays don't seem to support the 0x02 'draw point' command, so instead we have to draw the bitmap
    // as a series of rectangles using draw_pixel.
    // This will absolutely suck for performance, but it's the best we can do on these screens.
    for (game_dim_t row = 0; row < rows; row++) {
      for (game_dim_t col = 0; col < bytes_per_row; col++) {
        const uint8_t byte = bitmap[(row * bytes_per_row) + col];
        for (uint8_t bit = 0; bit < 8; bit++) {
          // assume that the screen area is cleared before drawing
          if (byte & (1 << bit)) {
            draw_pixel(x + (col * 8) + (7 - bit + 1), y + row);
            DRAW_CALL_INC(1);
          }
        }
      }
    }
  #endif
}

int MarlinGame::draw_string(const game_dim_t x, const game_dim_t y, const char* str) {
  DRAW_CALL_INC(0);

  lcd_moveto_xy(
    dwin_game::game_to_screen(x) + dwin_game::x_offset,
    dwin_game::game_to_screen(y) + dwin_game::y_offset
  );

  return lcd_put_u8str_max_P(
    str,
    PIXEL_LEN_NOLIMIT
  );
}

int MarlinGame::draw_string(const game_dim_t x, const game_dim_t y, FSTR_P const str) {
  return draw_string(x, y, FTOP(str));
}

void MarlinGame::draw_int(const game_dim_t x, const game_dim_t y, const int value) {
  COUNT_DRAW_CALL(0);

  lcd_moveto_xy(
    dwin_game::game_to_screen(x) + dwin_game::x_offset,
    dwin_game::game_to_screen(y) + dwin_game::y_offset
  );

  lcd_put_int(value);
}
#endif // IS_DWIN_MARLINUI && HAS_GAMES
