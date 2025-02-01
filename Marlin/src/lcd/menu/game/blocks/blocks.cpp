#if 1

#include "../game.h"

// size of one block, squared
constexpr game_dim_t BLOCK_SIZE = 3;

// where on the board the shape will spawn, board coordinates
constexpr game_dim_t SPAWN_POINT_X = (BLOCKS_BOARD_WIDTH / 2);
constexpr game_dim_t SPAWN_POINT_Y = 0;

// how many milliseconds between each gravity update, ms
// lower causes pieces to fall faster
#define FALL_SPEED(level) (_MAX(50, 250 - (level * 50)))

// location of the board (top left corner)
constexpr game_dim_t BOARD_OFFSET_X = ((GAME_WIDTH - (BLOCKS_BOARD_WIDTH * BLOCK_SIZE)) / 2);
constexpr game_dim_t BOARD_OFFSET_Y = ((GAME_HEIGHT - (BLOCKS_BOARD_HEIGHT * BLOCK_SIZE)) / 2);

// location of the next shape preview (top left corner)
constexpr game_dim_t NEXT_SHAPE_PREVIEW_X = (BOARD_OFFSET_X - 3 - (4 * BLOCK_SIZE));
constexpr game_dim_t NEXT_SHAPE_PREVIEW_Y = BOARD_OFFSET_Y;

// location of the score display (top left corner)
constexpr game_dim_t SCORE_X = (BOARD_OFFSET_X + (BLOCKS_BOARD_WIDTH * BLOCK_SIZE) + 3);
constexpr game_dim_t SCORE_Y = BOARD_OFFSET_Y;

#define BOARD_X_TO_SCREEN(x) (BOARD_OFFSET_X + (x * BLOCK_SIZE))
#define BOARD_Y_TO_SCREEN(y) (BOARD_OFFSET_Y + (y * BLOCK_SIZE))

#define GAME_STATE_GAME_OVER 0
#define GAME_STATE_FALLING_ACTIVE 1
#define GAME_STATE_LINE_CLEAR 2

void BlocksGame::enter_game()
{
  init_game(GAME_STATE_LINE_CLEAR, game_screen);

  // reset state
  marlin_game_data.blocks.board.clear();
  marlin_game_data.blocks.lines_cleared = 0;
  marlin_game_data.blocks.old_encoder = 0;

  // ensure no falling block is active
  marlin_game_data.blocks.falling.shape = blockshape::NONE;
}

void BlocksGame::game_screen()
{
  if (game_frame()) do {
    if (game_state == GAME_STATE_GAME_OVER)
    {
      if (ui.use_click())
        exit_game();
    }
    else if (game_state == GAME_STATE_FALLING_ACTIVE)
    {
      const millis_t now = millis();

      handle_player_input(marlin_game_data.blocks.board, marlin_game_data.blocks.falling);

      if (handle_falling_gravity(marlin_game_data.blocks.board, marlin_game_data.blocks.falling, now, FALL_SPEED(marlin_game_data.blocks.level())))
      {
        // landed on something, commit the falling block
        commit_falling(marlin_game_data.blocks.board, marlin_game_data.blocks.falling);

        // to not continue drawing the falling block, set it to empty
        // not doing this could cause weirdness
        marlin_game_data.blocks.falling.shape = blockshape::NONE;

        // check if we need to clear lines
        game_state = GAME_STATE_LINE_CLEAR;
      }
    }
    else if (game_state == GAME_STATE_LINE_CLEAR)
    {
      // clear lines until no more lines to clear
      uint8_t cleared = 0;
      while (handle_line_clear(marlin_game_data.blocks.board))
      {
        cleared++;
      }

      // call handler for lines cleared
      on_lines_cleared(cleared);

      // no more lines to clear, spawn new falling block
      game_state = GAME_STATE_FALLING_ACTIVE;

      if (!spawn_falling(marlin_game_data.blocks.board, marlin_game_data.blocks.falling, marlin_game_data.blocks.next_shape))
      {
        // cannot spawn new shape, game over
        game_state = GAME_STATE_GAME_OVER;
      }
      else
      {
        // determine next block shape
        marlin_game_data.blocks.next_shape = static_cast<blockshape>(random(0, 7));
      }
    }
  } while (0);

  frame_start();
  draw_board(marlin_game_data.blocks.board);

  // draw falling shape
  draw_blockshape(BOARD_X_TO_SCREEN(marlin_game_data.blocks.falling.x),
                  BOARD_Y_TO_SCREEN(marlin_game_data.blocks.falling.y),
                  marlin_game_data.blocks.falling.shape,
                  marlin_game_data.blocks.falling.rotation);

  // draw next shape preview
  set_color(color::WHITE);
  draw_frame(NEXT_SHAPE_PREVIEW_X - 1,
             NEXT_SHAPE_PREVIEW_Y - 1,
             (4 * BLOCK_SIZE) + 2,
             (4 * BLOCK_SIZE) + 2);
  draw_blockshape(NEXT_SHAPE_PREVIEW_X,
                  NEXT_SHAPE_PREVIEW_Y,
                  marlin_game_data.blocks.next_shape,
                  0);

  // draw score
  set_color(color::WHITE);
  draw_string(SCORE_X, SCORE_Y, "Score:");
  draw_int(SCORE_X, SCORE_Y + GAME_FONT_ASCENT, score);

  // draw level
  draw_string(SCORE_X, SCORE_Y + (2 * GAME_FONT_ASCENT), "Level:");
  draw_int(SCORE_X, SCORE_Y + (3 * GAME_FONT_ASCENT), marlin_game_data.blocks.level());

  if (game_state == GAME_STATE_GAME_OVER)
    draw_game_over();

  frame_end();
}

void BlocksGame::on_falling_committed(const falling_t &falling)
{
  // score based on how many lines the piece fell
  // this is conveniently just the y coordinate of the falling piece
  score += falling.y;
}

void BlocksGame::on_lines_cleared(const uint8_t count)
{
  // score based on how many lines were cleared
  const uint8_t level = marlin_game_data.blocks.level();
  if (count == 1)
    score += level * 100; // Single
  if (count == 2)
    score += level * 300; // Double
  if (count == 3)
    score += level * 500; // Triple
  if (count >= 4)
    score += level * 800; // Big One

  // update lines cleared in state
  marlin_game_data.blocks.lines_cleared += count;
}

void BlocksGame::handle_player_input(const board_t &board, falling_t &falling)
{
  // record position before update
  falling_t old = falling;
  bool dirty = false;

  // update position when clicking
  if (ui.use_click())
  {
    falling.rotation = (falling.rotation + 1) % 4;
    dirty = true;
  }

  // update left/right movement by encoder
  const int8_t enc = int8_t(ui.encoderPosition);
  const int8_t diff = enc - marlin_game_data.blocks.old_encoder;
  if (diff != 0)
  {
    marlin_game_data.blocks.old_encoder = enc;
    falling.x += diff;
    dirty = true;
  }

  if (!dirty)
    return;

  // block movement if it would collide
  if (collision_check_falling(board, falling))
  {
    falling = old;
  }
}

bool BlocksGame::handle_falling_gravity(const board_t &board, falling_t &falling, const millis_t now, const millis_t fall_speed)
{
  // record position before update
  const uint8_t oldY = falling.y;

  // make the block fall
  const bool should_fall = (now - falling.last_update_millis) > fall_speed;
  if (should_fall)
  {
    falling.y++;
    falling.last_update_millis = now;
  }

  // undo falling and commit if collision detected
  if (collision_check_falling(board, falling))
  {
    falling.y = oldY;
    return true;
  }

  return false;
}

bool BlocksGame::handle_line_clear(board_t &board)
{
  // check bottom-to-top for full lines
  for (uint8_t y = BLOCKS_BOARD_HEIGHT - 1; y > 0; y--)
  {
    bool full = true;
    for (uint8_t x = 0; x < BLOCKS_BOARD_WIDTH; x++)
    {
      if (board.get(x, y) == blockshape::NONE)
      {
        full = false;
        break;
      }
    }

    if (full)
    {
      // clear the line
      // for (uint8_t x = 0; x < BLOCKS_BOARD_WIDTH; x++)
      //{
      //  board.set(x, y, blockshape::NONE);
      //}

      // move all lines above one down
      for (uint8_t y2 = y; y2 > 0; y2--)
      {
        for (uint8_t x = 0; x < BLOCKS_BOARD_WIDTH; x++)
        {
          board.set(x, y2, board.get(x, y2 - 1));
        }
      }

      // clear the top line
      for (uint8_t x = 0; x < BLOCKS_BOARD_WIDTH; x++)
      {
        board.set(x, 0, blockshape::NONE);
      }

      return true;
    }
  }

  return false;
}

void BlocksGame::commit_falling(board_t &board, const falling_t &falling)
{
  if (falling.shape == blockshape::NONE)
  {
    return;
  }

  const uint8_t *shape = get_blockshape_shape(falling.shape, falling.rotation);
  for (uint8_t x = 0; x < 4; x++)
  {
    for (uint8_t y = 0; y < 4; y++)
    {
      if (shape[y] & (1 << (3 - x)))
      {
        board.set(falling.x + x, falling.y + y, falling.shape);
      }
    }
  }

  on_falling_committed(falling);
}

bool BlocksGame::spawn_falling(const board_t &board, falling_t &falling, const blockshape shape)
{
  falling.shape = shape;
  falling.x = SPAWN_POINT_X;
  falling.y = SPAWN_POINT_Y;
  falling.rotation = 0;

  if (collision_check_falling(board, falling))
  {
    // spawning not possible!
    falling.shape = blockshape::NONE;
    return false;
  }

  return true;
}

bool BlocksGame::collision_check_falling(const board_t &board, const falling_t &falling)
{
  if (falling.shape == blockshape::NONE)
  {
    // invalid state
    return true;
  }

  const uint8_t *shape = get_blockshape_shape(falling.shape, falling.rotation);
  for (uint8_t x = 0; x < 4; x++)
  {
    for (uint8_t y = 0; y < 4; y++)
    {
      if (shape[y] & (1 << (3 - x)))
      {
        const uint8_t collision = board.check_collision(falling.x + x, falling.y + y);
        if (collision != 0)
        {
          SERIAL_ECHOPGM("collision detected: ");
          SERIAL_ECHOLN(static_cast<int>(collision));
          return true;
        }
      }
    }
  }

  return false;
}

void BlocksGame::draw_board(const board_t &board)
{
  // draw the blocks of the board
  for (uint8_t x = 0; x < BLOCKS_BOARD_WIDTH; x++)
  {
    for (uint8_t y = 0; y < BLOCKS_BOARD_HEIGHT; y++)
    {
      const blockshape type = board.get(x, y);
      if (type != blockshape::NONE)
      {
        draw_block(BOARD_X_TO_SCREEN(x), BOARD_Y_TO_SCREEN(y), type);
      }
    }
  }

  // draw outline of the board
  set_color(color::WHITE);
  draw_frame(BOARD_OFFSET_X - 1,
             BOARD_OFFSET_Y - 1,
             (BLOCKS_BOARD_WIDTH * BLOCK_SIZE) + 2,
             (BLOCKS_BOARD_HEIGHT * BLOCK_SIZE) + 2);
}

void BlocksGame::draw_blockshape(const game_dim_t screen_x, const game_dim_t screen_y, const blockshape shape, const uint8_t rotation)
{
  if (shape == blockshape::NONE)
  {
    return;
  }

  const uint8_t *s = get_blockshape_shape(shape, rotation);
  for (uint8_t x = 0; x < 4; x++)
  {
    for (uint8_t y = 0; y < 4; y++)
    {
      if (s[y] & (1 << (3 - x)))
      {
        draw_block(screen_x + (x * BLOCK_SIZE), screen_y + (y * BLOCK_SIZE), shape);
      }
    }
  }
}

void BlocksGame::draw_block(const game_dim_t screen_x, const game_dim_t screen_y, const blockshape type)
{
  const size_t c = static_cast<size_t>(type);
  assert(c < sizeof(BLOCK_COLORS) / sizeof(BLOCK_COLORS[0]));

  set_color(BLOCK_COLORS[c]);
  draw_box(screen_x,
           screen_y,
           BLOCK_SIZE,
           BLOCK_SIZE);
}

const uint8_t *BlocksGame::get_blockshape_shape(const blockshape shape, const uint8_t rotation)
{
  return BLOCK_SHAPES[static_cast<uint8_t>(shape)][rotation];
}

#endif // MARLIN_BLOCKS
