#include "../game.h"

/**
 * Shapes, each one being a bitmap of 4 rows x 4 bits
 * The top-left corner (row 0, bit 4) is the origin of the shape
 */
const uint8_t BlocksGame::BLOCK_SHAPES[/*id*/ 7][/*rotation*/ 4][/*row*/ 4] = {
  { // I
    { // 0 degrees
      0b0000,
      0b1111,
      0b0000,
      0b0000,
    },

    { // 90 degrees
      0b0010,
      0b0010,
      0b0010,
      0b0010,
    },

    { // 180 degrees
      0b0000,
      0b0000,
      0b1111,
      0b0000,
    },
    { // 270 degrees
      0b0100,
      0b0100,
      0b0100,
      0b0100,
    },
  },
  { // J
    { // 0 degrees
      0b1000,
      0b1110,
      0b0000,
      0b0000,
    },

    { // 90 degrees
      0b0110,
      0b0100,
      0b0100,
      0b0000,
    },

    { // 180 degrees
      0b0000,
      0b1110,
      0b0010,
      0b0000,
    },
    { // 270 degrees
      0b0100,
      0b0100,
      0b1100,
      0b0000,
    },
  },
  { // L
    { // 0 degrees
      0b0010,
      0b1110,
      0b0000,
      0b0000,
    },

    { // 90 degrees
      0b0100,
      0b0100,
      0b0110,
      0b0000,
    },

    { // 180 degrees
      0b0000,
      0b1110,
      0b1000,
      0b0000,
    },
    { // 270 degrees
      0b1100,
      0b0100,
      0b0100,
      0b0000,
    },
  },
  { // O
    { // 0 degrees
      0b1100,
      0b1100,
      0b0000,
      0b0000,
    },

    { // 90 degrees
      0b1100,
      0b1100,
      0b0000,
      0b0000,
    },

    { // 180 degrees
      0b1100,
      0b1100,
      0b0000,
      0b0000,
    },
    { // 270 degrees
      0b1100,
      0b1100,
      0b0000,
      0b0000,
    },
  },
  { // S
    { // 0 degrees
      0b0110,
      0b1100,
      0b0000,
      0b0000,
    },

    { // 90 degrees
      0b0100,
      0b0110,
      0b0010,
      0b0000,
    },

    { // 180 degrees
      0b0000,
      0b0110,
      0b1100,
      0b0000,
    },
    { // 270 degrees
      0b1000,
      0b1100,
      0b0100,
      0b0000,
    },
  },
  { // T
    { // 0 degrees
      0b0100,
      0b1110,
      0b0000,
      0b0000,
    },

    { // 90 degrees
      0b0100,
      0b0110,
      0b0100,
      0b0000,
    },

    { // 180 degrees
      0b0000,
      0b1110,
      0b0100,
      0b0000,
    },
    { // 270 degrees
      0b0100,
      0b1100,
      0b0100,
      0b0000,
    },
  },
  { // Z 
    { // 0 degrees
      0b1100,
      0b0110,
      0b0000,
      0b0000,
    },

    { // 90 degrees
      0b0010,
      0b0110,
      0b0100,
      0b0000,
    },

    { // 180 degrees
      0b0000,
      0b1100,
      0b0110,
      0b0000,
    },
    { // 270 degrees
      0b0100,
      0b1100,
      0b1000,
      0b0000,
    },
  }
};

/**
 * Colors of the blocks
 */
const BlocksGame::color BlocksGame::BLOCK_COLORS[7] = {
  color::CYAN,    // I
  color::BLUE,    // J
  color::WHITE,   // L
  color::YELLOW,  // O
  color::GREEN,   // S
  color::MAGENTA, // T
  color::RED,     // Z
};
