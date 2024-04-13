#include "../../inc/MarlinConfig.h"

#if HAS_MARLINUI_U8GLIB

#include "../menu/game/renderer.h"

void GameRenderer::frame_start() {}

void GameRenderer::frame_end() {}

void GameRenderer::set_color(const uint8_t color) {
  u8g.setColorIndex(color);
}

void GameRenderer::draw_hline(const game_dim_t x, const game_dim_t y, const game_dim_t w) {
  u8g.drawHLine(x, y, w);
}

void GameRenderer::draw_vline(const game_dim_t x, const game_dim_t y, const game_dim_t h) {
  u8g.drawVLine(x, y, h);
}

void GameRenderer::draw_frame(const game_dim_t x, const game_dim_t y, const game_dim_t w, const game_dim_t h) {
  u8g.drawFrame(x, y, w, h);
}

void GameRenderer::draw_box(const game_dim_t x, const game_dim_t y, const game_dim_t w, const game_dim_t h) {
  u8g.drawBox(x, y, w, h);
}

void GameRenderer::draw_pixel(const game_dim_t x, const game_dim_t y) {
  u8g.drawPixel(x, y);
}

int GameRenderer::draw_string(const game_dim_t x, const game_dim_t y, const char* str) {
  lcd_moveto(x, y);
  return lcd_put_u8str_P(str);
}

int GameRenderer::draw_string(const game_dim_t x, const game_dim_t y, FSTR_P const fstr) {
  lcd_moveto(x, y);
  return lcd_put_u8str(fstr);
}

void GameRenderer::draw_int(const game_dim_t x, const game_dim_t y, const int value) {
  lcd_put_int(x, y, value);
}
#endif // HAS_MARLINUI_U8GLIB
