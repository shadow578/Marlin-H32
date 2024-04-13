#pragma once
#include <stdint.h>
#include "../marlinui/marlinui_dwin.h"

typedef uint8_t game_dim_t;
typedef uint16_t screen_dim_t;
typedef const uint8_t* pgm_bitmap_t;

namespace dwin_game {
  /**
   * @brief Main game scaling factor 
   */
  constexpr float scale = 4.0f;

  /**
   * @brief Offset of the game window on the screen
   */
  constexpr screen_dim_t x_offset = 0;
  constexpr screen_dim_t y_offset = 0;

  /**
   * @brief scale a game dimension to screen dimensions 
   */
  constexpr game_dim_t screen_to_game(const screen_dim_t x) {
    return x / scale;
  }

  /**
   * @brief scale a screen dimension to game dimensions
   */
  constexpr screen_dim_t game_to_screen(const game_dim_t x) {
    return x * scale;
  }

  /**
   * @brief convert a color (from set_color) to dwin 
   */
  constexpr uint16_t color_to_dwin(const uint8_t color) {
    return color == 0 ? RGB(0, 0, 0) : RGB(255, 255, 255);
  }
} // namespace dwin_game

constexpr game_dim_t GAME_WIDTH = dwin_game::screen_to_game(DWIN_WIDTH);
constexpr game_dim_t GAME_HEIGHT = dwin_game::screen_to_game(DWIN_HEIGHT);
constexpr game_dim_t GAME_FONT_WIDTH = dwin_game::screen_to_game(MENU_FONT_WIDTH);
constexpr game_dim_t GAME_FONT_ASCENT = dwin_game::screen_to_game(MENU_FONT_ASCENT);
  
// not needed on DWIN
#define PAGE_OVER(ya) true
#define PAGE_UNDER(yb) true
#define PAGE_CONTAINS(ya, yb) true
