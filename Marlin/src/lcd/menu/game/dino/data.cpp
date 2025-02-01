#include "dino.h"

static const uint8_t DINO_SPRITE_A[] PROGMEM =
{
  0b00000111,
  0b10000111,
  0b01111100,
  0b00111000,
  0b00100000,
};

static const uint8_t DINO_SPRITE_B[] PROGMEM =
{
  0b00000111,
  0b10000111,
  0b01111100,
  0b00111000,
  0b00001000,
};

static const uint8_t* DINO_SPRITES[] PROGMEM =
{
  DINO_SPRITE_A,
  DINO_SPRITE_B,
};

const DinoGame::sprite_info_t DinoGame::PLAYER_INFO = 
{
  .sprite = DINO_SPRITES,
  .frames = 2,
  .width = 8,
  .height = 5,
};

static const uint8_t CACTUS_SMALL_SPRITE[] PROGMEM =
{
  0b10100000,
  0b10101000,
  0b01101000,
  0b00110000,
  0b00100000,
};

static const uint8_t* CACTUS_SMALL_SPRITES[] PROGMEM =
{
  CACTUS_SMALL_SPRITE,
};

static const uint8_t CACTUS_LARGE_SPRITE[] PROGMEM =
{
  0b00001001,
  0b00011011,
  0b10011011,
  0b11011110,
  0b11011100,
  0b01111000,
  0b00111000,
  0b00011000,
};

static const uint8_t* CACTUS_LARGE_SPRITES[] PROGMEM =
{
  CACTUS_LARGE_SPRITE,
};

static const uint8_t BIRD_SPRITE_A[] PROGMEM =
{
  0b00010000,
  0b00011000,
  0b01011100,
  0b11111110,
  0b00011111,
  0b00000000,
  0b00000000,
  0b00000000,
};

static const uint8_t BIRD_SPRITE_B[] PROGMEM =
{
  0b00000000,
  0b00000000,
  0b01000000,
  0b11111110,
  0b00011111,
  0b00011100,
  0b00011000,
  0b00010000,
};

static const uint8_t* BIRD_SPRITES[] PROGMEM =
{
  BIRD_SPRITE_A,
  BIRD_SPRITE_B,
};

const DinoGame::obstacle_info_t DinoGame::OBSTACLE_INFO[static_cast<int>(obstacle_type::NONE)] = 
{
  { // CACTUS_SMALL
    .sprite =
    {
      .sprite = CACTUS_SMALL_SPRITES,
      .frames = 1,
      .width = 5,
      .height = 5,
    },
    .y_offset = FTOF(0),
    .min_score = 0,
  },
  { // CACTUS_LARGE
    .sprite =
    {
      .sprite = CACTUS_LARGE_SPRITES,
      .frames = 1,
      .width = 8,
      .height = 8,
    },
    .y_offset = FTOF(0),
    .min_score = 0,
  },
  { // BIRD
    .sprite =
    {
      .sprite = BIRD_SPRITES,
      .frames = 2,
      .width = 8,
      .height = 8,
    },
    .y_offset = FTOF(15),
    .min_score = 100,
  },
};
