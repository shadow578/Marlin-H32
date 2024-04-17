#include "../../../inc/MarlinConfigPre.h"

#if IS_DWIN_MARLINUI && HAS_GAMES

#include "../../menu/game/types.h" // includes e3v2/marlinui/game.h
#include "../../lcdprint.h"
#include "lcdprint_dwin.h"
#include "marlinui_dwin.h"

void MarlinGame::frame_start() {
  // clear the screen before each frame
  //dwinFrameClear(CLEAR_COLOR);

  // filling the play area should be faster than clearing the whole screen
  const uint16_t fg = dwin_font.fg;
  dwin_font.fg = COLOR_BG_BLACK;
  draw_box(0, 0, GAME_WIDTH, GAME_HEIGHT);
  dwin_font.fg = fg;
}

void MarlinGame::frame_end() {}

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
}

void MarlinGame::draw_vline(const game_dim_t x, const game_dim_t y, const game_dim_t h) {
  // draw lines as boxes, since DWIN lines are always 1px wide but we want to scale them
  draw_box(x, y, 1, h);
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
}

void MarlinGame::draw_pixel(const game_dim_t x, const game_dim_t y) {
  // draw pixels as boxes, since DWIN pixels are always 1px wide but we want to scale them
  draw_box(x, y, 1, 1);
}

void MarlinGame::draw_bitmap(const game_dim_t x, const game_dim_t y, const game_dim_t bytes_per_row, const game_dim_t rows, const pgm_bitmap_t bitmap) {
  // DWIN theorethically supports bitmaps since kernel 2.1, but most screens don't support it
  // (either because they use an older kernel version, or because they just (badly) emulate the DWIN protocol).
  // So instead, we draw the bitmap as a series of pixels, effectively emulating the draw call.
  // This will totally suck for performance, but it's the best we can do.
  for (game_dim_t row = 0; row < rows; row++) {
    for (game_dim_t col = 0; col < bytes_per_row; col++) {
      const uint8_t byte = bitmap[(row * bytes_per_row) + col];
      for (uint8_t bit = 0; bit < 8; bit++) {
        // assume that the screen area is cleared before drawing
        if (byte & (1 << bit)) {
          draw_pixel(x + (col * 8) + (7 - bit + 1), y + row);
        }
      }
    }
  }
}

int MarlinGame::draw_string(const game_dim_t x, const game_dim_t y, const char* str) {
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
  // TODO: lcd_put_int doesn't seem to work ?!
  char str[12];
  itoa(value, str, 10);
  draw_string(x, y, str);

  //lcd_put_int(
  //  dwin_game::game_to_screen(x) + dwin_game::x_offset,
  //  dwin_game::game_to_screen(y) + dwin_game::y_offset,
  //  value
  //);
}
#endif // IS_DWIN_MARLINUI && HAS_GAMES
