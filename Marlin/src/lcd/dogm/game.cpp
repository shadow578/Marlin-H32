#include "../../inc/MarlinConfigPre.h"

#if HAS_MARLINUI_U8GLIB && HAS_GAMES

#include "../menu/game/types.h" // includes dogm/game.h

void MarlinGame::frame_start() {}

void MarlinGame::frame_end() {}

void MarlinGame::set_color(const color color) {
  switch(color)
  {
    case color::BLACK:
      u8g.setColorIndex(0);
      break;
    case color::WHITE:
    default:
      u8g.setColorIndex(1);
      break;
  }
}

void MarlinGame::draw_hline(const game_dim_t x, const game_dim_t y, const game_dim_t w) {
  u8g.drawHLine(x, y, w);
}

void MarlinGame::draw_vline(const game_dim_t x, const game_dim_t y, const game_dim_t h) {
  u8g.drawVLine(x, y, h);
}

void MarlinGame::draw_frame(const game_dim_t x, const game_dim_t y, const game_dim_t w, const game_dim_t h) {
  u8g.drawFrame(x, y, w, h);
}

void MarlinGame::draw_box(const game_dim_t x, const game_dim_t y, const game_dim_t w, const game_dim_t h) {
  u8g.drawBox(x, y, w, h);
}

void MarlinGame::draw_pixel(const game_dim_t x, const game_dim_t y) {
  u8g.drawPixel(x, y);
}

void MarlinGame::draw_bitmap(const game_dim_t x, const game_dim_t y, const game_dim_t bytes_per_row, const game_dim_t rows, const pgm_bitmap_t bitmap) {
  u8g.drawBitmapP(x, y, bytes_per_row, rows, bitmap);
}

int MarlinGame::draw_string(const game_dim_t x, const game_dim_t y, const char* str) {
  lcd_moveto(x, y);
  return lcd_put_u8str_P(str);
}

int MarlinGame::draw_string(const game_dim_t x, const game_dim_t y, FSTR_P const fstr) {
  lcd_moveto(x, y);
  return lcd_put_u8str(fstr);
}

void MarlinGame::draw_int(const game_dim_t x, const game_dim_t y, const int value) {
  lcd_put_int(x, y, value);
}
#endif // HAS_MARLINUI_U8GLIB && HAS_GAMES
