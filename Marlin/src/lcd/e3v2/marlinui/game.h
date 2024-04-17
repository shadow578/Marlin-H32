#pragma once
#include <stdint.h>
#include "../marlinui/marlinui_dwin.h"

typedef uint8_t game_dim_t;
typedef uint16_t screen_dim_t;
typedef const uint8_t* pgm_bitmap_t;

namespace dwin_game {
  /**
   * @brief Target the renderer at 128x64 pixels to match UG8 screens
   */
  constexpr float TARGET_WIDTH = 128;
  constexpr float TARGET_HEIGHT = 64;

  /**
   * @brief Is the screen in portrait mode?
   * If yes, scale the game to fit the width of the screen and offset on Y.
   * If no, scale the game to fit the height of the screen and offset on X.
   */
  constexpr bool IS_PORTRAIT = DWIN_HEIGHT > DWIN_WIDTH;

  /**
   * @brief Game render scale.
   */
  constexpr float scale = IS_PORTRAIT ? (DWIN_WIDTH / TARGET_WIDTH) : (DWIN_HEIGHT / TARGET_HEIGHT);

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
   * @brief Offset of the game window on the screen. Applied after scaling.
   */
  constexpr screen_dim_t x_offset =  IS_PORTRAIT ? 0 : (DWIN_WIDTH - game_to_screen(TARGET_WIDTH)) / 2;
  constexpr screen_dim_t y_offset = !IS_PORTRAIT ? 0 : (DWIN_HEIGHT - game_to_screen(TARGET_HEIGHT)) / 2;
} // namespace dwin_game

constexpr game_dim_t GAME_WIDTH = dwin_game::screen_to_game(DWIN_WIDTH - (dwin_game::x_offset * 2));
constexpr game_dim_t GAME_HEIGHT = dwin_game::screen_to_game(DWIN_HEIGHT - (dwin_game::y_offset * 2));
constexpr game_dim_t GAME_FONT_WIDTH = dwin_game::screen_to_game(MENU_FONT_WIDTH);
constexpr game_dim_t GAME_FONT_ASCENT = dwin_game::screen_to_game(MENU_FONT_ASCENT);

// not needed on DWIN
#define PAGE_OVER(ya) true
#define PAGE_UNDER(yb) true
#define PAGE_CONTAINS(ya, yb) true
