#pragma once

#include "types.h"

typedef struct {
  /**
   * @brief position of the pipe's gap's center 
   */
  pos_t gap;

  /**
   * @brief is the pipe currently used?
   */
  bool active;
} pipe_t;

typedef struct {
  /**
   * @brief bird Y position 
   */
  int8_t bird_y;

  /**
   * @brief bird y velocity
   * @note positive values move the bird down, negative values move the bird up
   */
  int8_t bird_velocity;

  /**
   * @brief pipes
   */
  pipe_t pipes[4];
} flappy_data_t;

class FlappyGame : MarlinGame { 
public: 
  static void enter_game(),
              game_screen();

private:
  /**
   * @brief reset game state 
   */
  static void reset();

  /**
   * @brief move the bird
   * @param did_flap true if the bird flapped
   */
  static void move_bird(bool did_flap);

  /**
   * @brief move the pipes
   */
  static void move_pipes();

  /**
   * @brief spawn new pipes if needed 
   */
  static void spawn_pipes();

  /**
   * @brief did the bird hit the ground? 
   */
  static bool is_bird_out_of_bounds();

  /**
   * @brief is the bird colliding with a pipe?
   */
  static bool is_bird_colliding();

  /**
   * @brief get the next pipe position 
   */
  static pos_t get_next_pipe();

  /**
   * @brief called when the bird flaps
   */
  static void on_flap();

  /**
   * @brief called to update the score by one
   */
  static void on_score();

  /**
   * @brief called on game over
   */
  static void on_game_over();

  /**
   * @brief draw the bird
   */
  static void draw_bird();

  /**
   * @brief draw a pipe
   * @param pipe the pipe
   */
  static void draw_pipe(const pipe_t &pipe);
};

extern FlappyGame flappy;
