#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(MARLIN_FLAPPY)

#include "game.h"
static flappy_data_t &state = marlin_game_data.flappy;

// Bird:
//
// |--------//--
// |
// |    _   -----
// |   |_|>     | <- bird_size (square)
// |   |    -----
// |   |
// |--------//--
// |   |
// |<->|
// bird_x
constexpr int8_t bird_x = 5;
constexpr int8_t bird_size = 3;


// Pipe:
//
// pipe_width |     | distance_between_pipes
//          |<>|<------->|
//          |  |         |
// |----//----------------------//--
// |        |  |         |  |      | <- pipe_gap_min_y
// |       |____| ------ |  | ------
// |                     |  |
// |        ____         |  |
// |       |    |        |  |
// |        |  |         |  |
// |        |  |        |____| _____
// |        |  |                   | <- pipe_gap
// |        |  |         ____  -----
// |        |  |        |    |     | <- pipe_gap_max_y
// | _______|__|_________|__|___   |
// |----//----------------------//--
constexpr int8_t pipe_width = 2;

constexpr int8_t pipe_gap = 2 * bird_size;
constexpr int8_t pipe_gap_min_y = (pipe_gap) + (pipe_gap / 2);
constexpr int8_t pipe_gap_max_y = GAME_HEIGHT - pipe_gap_min_y;

constexpr int8_t distance_between_pipes = GAME_WIDTH / COUNT(state.pipes);
static_assert(distance_between_pipes >= bird_size * 6, "distance_between_pipes is too small - pipes will be very difficult to dodge, making the game frustrating.");


// Pipe Flare:
//
// pipe_flare_widht
// |<---------->|
//  ____________  --
// |            |  | <- pipe_flare_height
// |__        __| --
//    |      |
//    |      |
//
constexpr int8_t pipe_flare_width = pipe_width + 1;
constexpr int8_t pipe_flare_height = 2;


// Ground:
//
// |---------------//----
// |                    |
// ~                    ~
// |       ____         | <- ground_y
// |      |    |        |
// |_______|__|____ -----
// |
// |---------------//----
constexpr int8_t ground_y = GAME_HEIGHT - 1;

// score display (visible after game over only)
constexpr int8_t score_x = (GAME_WIDTH / 2) - 2; // kinda centered
constexpr int8_t score_y = GAME_HEIGHT - GAME_FONT_ASCENT - 1;



// Bird Physics:
// - every frame, the birds Y position is increased by its current velocity
// - the gravity increases the velocity by 1 px/frame, accelerating the bird downwards
// - on flap, the velocity is instantly set to -flap_strength, giving the bird upwards momentum
// - the bird's velocity is clamped to a maximum and minimum value
//
// |-//---------------//--
// |
// |     ^
// |     | <- flap_strength
// |     _
// |    |_|>
// |     |
// |     v <- gravity
// |
// |-//---------------//--

constexpr int8_t gravity = 1;                  // px / frame^2
constexpr int8_t flap_strength = gravity * 3;  // px / frame
constexpr int8_t max_velocity = flap_strength; // px / frame
constexpr int8_t min_velocity = -15;  // px / frame

// Pipe Physics:
// - pipes move to the right-to-left by pipe_speed px/frame
//
// |----//-------------------//--
// |                   |  |
// |                  |____|
// |
// |                   ____
// |                  |    |
// |                   |  |
// |                   |  |
// | <---------------- |  |
// |   | pipe_speed    |  |
// |                   |  |
// | __________________|__|_
// |----//-------------------//--
constexpr int8_t pipe_speed = 1; // px / frame

// draw a frame around the game area
constexpr bool frame_game_area = true;


void FlappyGame::reset() {
  state.bird_y = GAME_HEIGHT / 2;
  state.bird_velocity = 0;

  // deactivate all pipes
  for (auto &pipe : state.pipes) {
    pipe.active = false;
  }
}

void FlappyGame::enter_game() {
  init_game(1, game_screen); // 1 = Game running
  reset();
}

void FlappyGame::game_screen() {
  // run game logic once per screen
  if (game_frame()) do {
    if (game_state) move_bird(ui.use_click());
    move_pipes();
    spawn_pipes();

    // game over ?
    if (is_bird_out_of_bounds() || is_bird_colliding()) {
      on_game_over();
    }
  } while(0);

  // let the dead bird fall to the ground
  if (!game_state) move_bird(false);

  // draw the game
  frame_start();

  // draw game elements
  draw_bird();
  for (auto &pipe : state.pipes) {
    draw_pipe(pipe);
  }

  // draw the frame around the game area
  set_color(color::WHITE);
  if (frame_game_area) {
    draw_frame(
      0,
      0,
      GAME_WIDTH,
      GAME_HEIGHT
    );
  }

  // draw game over screen and exit on click
  if (!game_state) {
    draw_game_over();
    draw_int(score_x, score_y, score);
    if (ui.use_click()) exit_game();
  }

  frame_end();
}

void FlappyGame::move_bird(bool did_flap) {
  // make the bird fall down, constrained to max_velocity
  state.bird_velocity = constrain(state.bird_velocity + gravity, min_velocity, max_velocity);

  // only allow flapping if the bird is falling
  bool can_flap = state.bird_velocity >= 0;
  if (did_flap && can_flap) {
    // flap!
    state.bird_velocity = -flap_strength;
    on_flap();
  }

  // apply velocity
  state.bird_y += state.bird_velocity;

  // constrain bird to screen top and ground
  state.bird_y = constrain(state.bird_y, 0, ground_y);
}

void FlappyGame::move_pipes() {
  for (auto &pipe : state.pipes) {
    // move pipe
    pipe.gap.x -= pipe_speed;

    // deactivate the pipe if reached screen edge
    if (pipe.gap.x < -3 && pipe.active) {
      pipe.active = false;

      // on pipe reset, increment score
      // it's not quite at the right time, but close enough and simple
      on_score();
    }
  }
}

void FlappyGame::spawn_pipes() {
  // find the coordinates of the last active pipe (with highest x coordinate)
  int8_t last_pipe_x = -1;
  for (auto &pipe : state.pipes) {
    if (pipe.active) {
      last_pipe_x = max(last_pipe_x, pipe.gap.x);
    }
  }

  // if the last pipe is far enough away, try to spawn a new one in the next slot
  if (last_pipe_x < GAME_WIDTH - distance_between_pipes) {
    for (auto &pipe : state.pipes) {
      if (!pipe.active) {
        pipe.active = true;
        pipe.gap = get_next_pipe();
        break;
      }
    }
  }
}

bool FlappyGame::is_bird_out_of_bounds() {
  return state.bird_y >= ground_y;
}

bool FlappyGame::is_bird_colliding() {
  for (auto &pipe : state.pipes) {
    if (!pipe.active) continue;

    // check if bird is within pipe x range
    if (bird_x >= pipe.gap.x - pipe_width && bird_x <= pipe.gap.x + pipe_width) {
      // check if bird is outside of the pipe gap
      if (state.bird_y < pipe.gap.y - pipe_gap || state.bird_y > pipe.gap.y + pipe_gap) {
        return true;
      }
    }
  }

  return false;
}

pos_t FlappyGame::get_next_pipe() {
  // start pipe off screen
  return {
    GAME_WIDTH - 1,
    static_cast<int8_t>(random(pipe_gap_min_y, pipe_gap_max_y))
  };
}

void FlappyGame::on_flap() {
  _BUZZ(5, 300);
}

void FlappyGame::on_score() {
  score++;
  _BUZZ(5, 220);
  _BUZZ(5, 280);
}

void FlappyGame::on_game_over() {
  game_state = 0;
  _BUZZ(400, 40);
}

void FlappyGame::draw_bird() {
  set_color(color::YELLOW);
  draw_frame(
    bird_x - (bird_size / 2),
    state.bird_y - (bird_size / 2),
    bird_size,
    bird_size
  );
}

void FlappyGame::draw_pipe(const pipe_t &pipe) {
  if (!pipe.active) return;

  // don't draw pipe if it's off the end of the screen
  if (pipe.gap.x < pipe_width + (pipe_flare_width / 2)) return;
  if (pipe.gap.x > GAME_WIDTH - (pipe_width * 2) - pipe_flare_width) return;

  // pipes are green
  set_color(color::GREEN);

  // top
  draw_box(
    pipe.gap.x - pipe_width,
    0,
    pipe_width * 2,
    pipe.gap.y - pipe_gap
  );
  draw_box(
    pipe.gap.x - pipe_width - (pipe_flare_width / 2),
    pipe.gap.y - pipe_gap - pipe_flare_height,
    pipe_flare_width * 2,
    pipe_flare_height
  );

  // bottom
  draw_box(
    pipe.gap.x - pipe_width,
    pipe.gap.y + pipe_gap,
    pipe_width * 2,
    ground_y - pipe.gap.y - pipe_gap
  );
  draw_box(
    pipe.gap.x - pipe_width - (pipe_flare_width / 2),
    pipe.gap.y + pipe_gap,
    pipe_flare_width * 2,
    pipe_flare_height
  );
}

#endif // MARLIN_FLAPPY
