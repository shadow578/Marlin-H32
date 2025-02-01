#pragma once

#include "../types.h"
#include "assert.h"

class DinoGame : MarlinGame
{
public:
  static void enter_game();
  static void game_screen();

private:
  struct player_t
  {
    uint8_t frame;
    fixed_t y_position;
    fixed_t y_velocity;

    void set_grounded()
    {
      y_velocity = FTOF(0);
    }

    bool grounded() const
    {
      return y_velocity == FTOF(0);
    }
  };

  enum class obstacle_type 
  {
    CACTUS_SMALL,
    CACTUS_LARGE,
    BIRD,
    NONE
  };

  struct obstacle_t
  {
    uint8_t frame;
    fixed_t x;
    obstacle_type type;
  };

  struct aabb_t
  {
    fixed_t x;
    fixed_t y;
    fixed_t width;
    fixed_t height;

    bool intersects(const aabb_t &other) const
    {
      return x < other.x + other.width
          && x + width > other.x
          && y < other.y + other.height
          && y + height > other.y;
    }
  };

  struct sprite_info_t
  {
    const uint8_t** sprite;
    uint8_t frames;
    game_dim_t width;
    game_dim_t height;
  };

  struct obstacle_info_t
  {
    sprite_info_t sprite;
    fixed_t y_offset;
    int min_score;
  };

public:
  struct state_t
  {
    player_t player;
    obstacle_t obstacles[4];
    uint8_t last_spawned_obstacle_index;
    fixed_t next_spawn_distance;
  };

private:
  static void on_jump();
  static void on_obstacle_despawn(obstacle_t &obstacle);
  static void on_collision(const player_t &player, const obstacle_t &obstacle);

  static void update_player(player_t &player);
  static void update_world(state_t &state);

  static void spawn_obstacle(obstacle_t &slot);

  static void draw_player(const player_t &player);
  static void draw_obstacle(const obstacle_t &obstacle);
  static void draw_ground(const uint8_t offset);
  static void draw_aabb(const aabb_t &box);

  static void get_player_bounding_box(const player_t &player, aabb_t &box);
  static void get_obstacle_bounding_box(const obstacle_t &obstacle, aabb_t &box);
  static void get_ground_bounding_box(aabb_t &box);

  static const sprite_info_t PLAYER_INFO;
  static const obstacle_info_t OBSTACLE_INFO[static_cast<int>(obstacle_type::NONE)];

  static const obstacle_info_t* get_obstacle_info(const obstacle_type type);
};

extern DinoGame dino_game;
