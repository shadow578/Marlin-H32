#include "../../../inc/MarlinConfig.h"

#if IS_DWIN_MARLINUI

#include "../../menu/game/renderer.h" // includes game_renderer.h
#include "../../lcdprint.h"
#include "lcdprint_dwin.h"
#include "marlinui_dwin.h"

// ensure game screen size doesn't exceed maximum size of a int8_t
static_assert(GAME_WIDTH <= 0x7f, "GAME_WIDTH must fit int8_t. Consider increasing the scaling factor.");
static_assert(GAME_HEIGHT <= 0x7f, "GAME_HEIGHT must fit int8_t. Consider increasing the scaling factor.");

void GameRenderer::frame_start() {
  // clear the screen before each frame
  dwinFrameClear(RGB(0, 0, 0));
}

void GameRenderer::frame_end() {}

void GameRenderer::set_color(const uint8_t color) {
  dwin_font.fg = dwin_game::color_to_dwin(color);
}

void GameRenderer::draw_hline(const game_dim_t x, const game_dim_t y, const game_dim_t w) {
  // draw lines as boxes, since DWIN lines are always 1px wide but we want to scale them
  draw_box(x, y, w, 1);
}

void GameRenderer::draw_vline(const game_dim_t x, const game_dim_t y, const game_dim_t h) {
  // draw lines as boxes, since DWIN lines are always 1px wide but we want to scale them
  draw_box(x, y, 1, h);
}

void GameRenderer::draw_frame(const game_dim_t x, const game_dim_t y, const game_dim_t w, const game_dim_t h) {
  dwinDrawBox(
    0, // mode = frame
    dwin_font.fg, // color
    dwin_game::game_to_screen(x) + dwin_game::x_offset,
    dwin_game::game_to_screen(y) + dwin_game::y_offset,
    dwin_game::game_to_screen(w),
    dwin_game::game_to_screen(h)
  );
}

void GameRenderer::draw_box(const game_dim_t x, const game_dim_t y, const game_dim_t w, const game_dim_t h) {
  dwinDrawBox(
    1, // mode = fill
    dwin_font.fg, // color
    dwin_game::game_to_screen(x) + dwin_game::x_offset,
    dwin_game::game_to_screen(y) + dwin_game::y_offset,
    dwin_game::game_to_screen(w),
    dwin_game::game_to_screen(h)
  );
}

void GameRenderer::draw_pixel(const game_dim_t x, const game_dim_t y) {
  // draw pixels as boxes, since DWIN pixels are always 1px wide but we want to scale them
  draw_box(x, y, 1, 1);
}

void GameRenderer::draw_bitmapP(const game_dim_t x, const game_dim_t y, const game_dim_t bytes_per_row, const game_dim_t rows, const pgm_bitmap_t bitmap) {
  // DWIN theorethically supports bitmaps, but most screens don't support it 
  // (either because they use an older kernel version, or because they just (badly) emulate the DWIN protocol).
  // So instead, we draw the bitmap as a series of pixels, effectively emulating the draw call.
  // This will totally suck for performance, but it's the best we can do.
  for (game_dim_t row = 0; row < rows; row++) {
    for (game_dim_t col = 0; col < bytes_per_row; col++) {
      const uint8_t byte = bitmap[(row * bytes_per_row) + col];
      for (uint8_t bit = 0; bit < 7; bit++) {
        // assume that the screen area is cleared before drawing
        if (byte & (1 << bit)) {
          draw_pixel(x + (col * 8) + (7 - bit + 1), y + row);
        }
      }
    }
  }
}

int GameRenderer::draw_string(const game_dim_t x, const game_dim_t y, const char* str) {
  lcd_moveto_xy(
    dwin_game::game_to_screen(x) + dwin_game::x_offset,
    dwin_game::game_to_screen(y) + dwin_game::y_offset
  );
  
  return lcd_put_u8str_max_P(
    str,
    PIXEL_LEN_NOLIMIT
  );
}

int GameRenderer::draw_string(const game_dim_t x, const game_dim_t y, FSTR_P const fstr) {
  return draw_string(x, y, FTOP(fstr));
}

void GameRenderer::draw_int(const game_dim_t x, const game_dim_t y, const int value) {
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
#endif // IS_DWIN_MARLINUI
