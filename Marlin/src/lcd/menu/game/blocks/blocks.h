#pragma once

#include "../types.h"
#include "assert.h"

// size of the game board, in cells
constexpr size_t BLOCKS_BOARD_WIDTH = 10;
constexpr size_t BLOCKS_BOARD_HEIGHT = 20;

class BlocksGame : MarlinGame
{
public:
  static void enter_game();
  static void game_screen();

private:
  enum class blockshape : uint8_t
  {
    I = 0,
    J = 1,
    L = 2,
    O = 3,
    S = 4,
    T = 5,
    Z = 6,
    NONE = 7
  };

  struct board_t
  {
    struct
    {
      uint8_t left : 4;
      uint8_t right : 4;
    } board[BLOCKS_BOARD_WIDTH / 2][BLOCKS_BOARD_HEIGHT];

    void clear()
    {
      for (size_t x = 0; x < BLOCKS_BOARD_WIDTH; x++)
      {
        for (size_t y = 0; y < BLOCKS_BOARD_HEIGHT; y++)
        {
          set(x, y, blockshape::NONE);
        }
      }
    }

    void set(const uint8_t x, const uint8_t y, const blockshape value)
    {
      assert(x < BLOCKS_BOARD_WIDTH);
      assert(y < BLOCKS_BOARD_HEIGHT);

      if (x % 2 == 0)
      {
        board[x / 2][y].left = static_cast<uint8_t>(value);
      }
      else
      {
        board[x / 2][y].right = static_cast<uint8_t>(value);
      }
    }

    blockshape get(const uint8_t x, const uint8_t y) const
    {
      assert(x < BLOCKS_BOARD_WIDTH);
      assert(y < BLOCKS_BOARD_HEIGHT);

      uint8_t value;
      if (x % 2 == 0)
      {
        value = board[x / 2][y].left;
      }
      else
      {
        value = board[x / 2][y].right;
      }

      return static_cast<blockshape>(value);
    }

    /**
     * Check if the given coordinates are within the bounds of the board.
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @return
     * - 0 - in bounds
     * - 1 - x is out of bounds
     * - 2 - y is out of bounds
     * - 3 - collision with block
     */
    uint8_t check_collision(const uint8_t x, const uint8_t y) const
    {
      if (x >= BLOCKS_BOARD_WIDTH)
      {
        return 1;
      }
      if (y >= BLOCKS_BOARD_HEIGHT)
      {
        return 2;
      }
      if (get(x, y) != blockshape::NONE)
      {
        return 3;
      }
      return 0;
    }
  };

  struct falling_t
  {
    blockshape shape;
    uint8_t x;
    uint8_t y;

    /**
     * 0 = 0 degrees
     * 1 = 90 degrees
     * 2 = 180 degrees
     * 3 = 270 degrees
     */
    uint8_t rotation;

    millis_t last_update_millis;
  };

public:
  struct state_t
  {
    board_t board;
    falling_t falling;
    blockshape next_shape;
    uint8_t lines_cleared;

    int8_t old_encoder;

    uint8_t level() const
    {
      return (lines_cleared / 10) + 1;
    }
  };

private:
  static void on_falling_committed(const falling_t &falling);
  static void on_lines_cleared(const uint8_t count);

  static void handle_player_input(const board_t &board, falling_t &falling);
  static bool handle_falling_gravity(const board_t &board, falling_t &falling, const millis_t now, const millis_t fall_speed);
  static bool handle_line_clear(board_t &board);

  static void commit_falling(board_t &board, const falling_t &falling);
  static bool spawn_falling(const board_t &board, falling_t &falling, const blockshape shape);

  static bool collision_check_falling(const board_t &board, const falling_t &falling);

  static void draw_board(const board_t &board);
  static void draw_blockshape(const game_dim_t screen_x, const game_dim_t screen_y, const blockshape shape, const uint8_t rotation);
  static void draw_block(const game_dim_t screen_x, const game_dim_t screen_y, const blockshape type);

  static const uint8_t *get_blockshape_shape(const blockshape shape, const uint8_t rotation);
  static const uint8_t BLOCK_SHAPES[/*id*/ 7][/*rotation*/ 4][/*row*/ 4];
  static const color BLOCK_COLORS[/*id*/ 7];
};

extern BlocksGame blocks_game;

static_assert(BLOCKS_BOARD_WIDTH % 2 == 0, "BLOCKS_BOARD_WIDTH must be divisible by 2");
