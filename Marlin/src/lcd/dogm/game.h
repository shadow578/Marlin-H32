#pragma once
#include "marlinui_DOGM.h"
#include "../lcdprint.h"

typedef uint8_t game_dim_t;
typedef const u8g_pgm_uint8_t* pgm_bitmap_t;

constexpr game_dim_t GAME_WIDTH = LCD_PIXEL_WIDTH;
constexpr game_dim_t GAME_HEIGHT = LCD_PIXEL_HEIGHT;
constexpr game_dim_t GAME_FONT_WIDTH = MENU_FONT_WIDTH;
constexpr game_dim_t GAME_FONT_ASCENT = MENU_FONT_ASCENT;
