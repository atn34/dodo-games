#include "fixed_point.hpp"
#include "menu.hpp"
#include "string.hpp"

#include <api.h>
#include <stdint.h>
#include <string.h>

#define SCREEN_WIDTH_BEGIN 0
#define SCREEN_HEIGHT_BEGIN 0
#define SCREEN_WIDTH_END 128
#define SCREEN_HEIGHT_END 64

#define BLOCK_WIDTH 8
#define BLOCK_HEIGHT 8

#define TILE_ROWS 8
#define TILE_COLUMNS 16

using Real = FixedPoint<2, int>;

struct GameState {
  uint8_t block_x;
  uint8_t block_y;
  int8_t block_velocity_x;
  Real block_velocity_y;
  uint8_t block_want_jump;
  uint8_t block_jump_frames_left;
  uint8_t block_moves_available;
  byte last_buttons;
  byte tiles[TILE_ROWS][TILE_COLUMNS];
  uint8_t coins_remaining;
  uint8_t lastblock_x;
  uint8_t last_block_x;
  uint8_t last_block_y;
} state;

static const char *const tile_sprites[] = {
    "\x00\x00\x00\x00\x00\x00\x00\x00",
    "\xff\x00\xff\x00\xff\x00\xff\x00",
    "\x81\x42\x24\x18\x18\x24\x42\x81",
    "\x00\x3c\x42\x42\x42\x42\x3c\x00",
};

static const char *const block_sprites[] = {
    "\xff\x81\x81\x81\x81\x81\x81\xff",
    "\xff\xff\xc3\xc3\xc3\xc3\xff\xff",
    "\xff\xff\xff\xe7\xe7\xff\xff\xff",
    "\xff\xff\xff\xff\xff\xff\xff\xff",
};

static void processInput(byte buttons) {
  if ((buttons & 16) == 0 && (state.last_buttons & 16) != 0) {
    state.block_want_jump = 1;
  }
  if (state.block_want_jump && state.block_moves_available > 0) {
    // Begin jump
    state.block_want_jump = 0;
    state.block_velocity_y = Real(-6) / Real(4);
    state.block_jump_frames_left = 8;
    state.block_moves_available -= 1;
  }
  if ((buttons & 16) == 0 && (state.last_buttons & 16) == 0 &&
      state.block_jump_frames_left > 0) {
    // Continue jump
    state.block_velocity_y = Real(-6) / Real(4);
    state.block_jump_frames_left--;
  }
  if ((buttons & 16) != 0 && (state.last_buttons & 16) == 0) {
    // End jump
    state.block_jump_frames_left = 0;
    state.block_want_jump = 0;
  }
  if ((buttons & 8) == 0) {
    // Move right
    if (state.block_velocity_x < 2) {
      state.block_velocity_x = 2;
    } else {
      state.block_velocity_x = 4;
    }
  } else if ((buttons & 4) == 0) {
    // Move left
    if (state.block_velocity_x > -2) {
      state.block_velocity_x = -2;
    } else {
      state.block_velocity_x = -4;
    }
  } else {
    state.block_velocity_x = 0;
  }
  if ((buttons & 32) == 0 && (state.last_buttons & 32) != 0 &&
      state.block_moves_available > 0) {
    if ((buttons & 8) == 0) {
      // Dash right
      state.block_velocity_y = Real(0);
      state.block_moves_available -= 1;
      state.block_x += 24;
    }
    if ((buttons & 4) == 0) {
      // Dash left
      state.block_velocity_y = Real(0);
      state.block_moves_available -= 1;
      state.block_x -= 24;
    }
  }
  if (state.block_moves_available < 0) {
    state.block_moves_available = 0;
  }
  state.last_buttons = buttons;
}

static uint8_t handlePhysics() {
  uint8_t i;
  uint8_t j;
  uint8_t i_end;
  uint8_t j_end;
  uint8_t last_x;
  uint8_t last_y;
  uint8_t intersects_blocks = 0;
  state.block_velocity_y += Real(1) / Real(4);
  state.block_y += state.block_velocity_y.asInt();
  state.block_x += state.block_velocity_x;

  if (state.block_x < SCREEN_WIDTH_BEGIN) {
    state.block_x = SCREEN_WIDTH_BEGIN;
  }
  if (state.block_y < SCREEN_HEIGHT_BEGIN) {
    state.block_y = SCREEN_HEIGHT_BEGIN;
    state.block_velocity_y = Real(0);
  }
  if (state.block_x + BLOCK_WIDTH > SCREEN_WIDTH_END) {
    state.block_x = SCREEN_WIDTH_END - BLOCK_WIDTH;
  }
  if (state.block_y + BLOCK_HEIGHT > SCREEN_HEIGHT_END) {
    state.block_y = SCREEN_HEIGHT_END - BLOCK_HEIGHT;
    state.block_velocity_y = Real(0);
  }
  last_x = state.last_block_x;
  last_y = state.last_block_y;
  i_end = state.block_y / BLOCK_HEIGHT + 1 + !!(state.block_y & 7);
  j_end = state.block_x / BLOCK_WIDTH + 1 + !!(state.block_x & 7);
  for (i = state.block_y / BLOCK_HEIGHT; i < i_end; ++i) {
    for (j = state.block_x / BLOCK_WIDTH; j < j_end; ++j) {
      // Indices of all tiles intersecting block
      if (state.tiles[i][j] == 1) {
        intersects_blocks = 1;
        state.block_moves_available = 2;
        if (last_x / 8 + !!(last_x & 7) < j &&
            state.block_x / 8 + !!(state.block_x & 7) == j) {
          state.block_x = j * 8 - 8;
        } else if (last_x / 8 > j && state.block_x / 8 == j) {
          state.block_x = state.block_x / 8 * 8 + 8;
        }
      }
    }
  }
  if (intersects_blocks) {
    i_end = state.block_y / BLOCK_HEIGHT + 1 + !!(state.block_y & 7);
    j_end = state.block_x / BLOCK_WIDTH + 1 + !!(state.block_x & 7);
    for (i = state.block_y / BLOCK_HEIGHT; i < i_end; ++i) {
      for (j = state.block_x / BLOCK_WIDTH; j < j_end; ++j) {
        // Indices of all tiles intersecting block
        if (state.tiles[i][j] == 1) {
          state.block_moves_available = 2;
          if (last_y / 8 + !!(last_y & 7) < i &&
              state.block_y / 8 + !!(state.block_y & 7) == i) {
            state.block_y = i * 8 - 8;
            state.block_velocity_y = Real(0);
          } else if (last_y / 8 > i && state.block_y / 8 == i) {
            state.block_y = state.block_y / 8 * 8 + 8;
            state.block_velocity_y = Real(0);
          }
        }
      }
    }
  }
  i_end = state.block_y / BLOCK_HEIGHT + 1 + !!(state.block_y & 7);
  j_end = state.block_x / BLOCK_WIDTH + 1 + !!(state.block_x & 7);
  for (i = state.block_y / BLOCK_HEIGHT; i < i_end; ++i) {
    for (j = state.block_x / BLOCK_WIDTH; j < j_end; ++j) {
      // Indices of all tiles intersecting block
      if (state.tiles[i][j] == 2) {
        return 1;
      } else if (state.tiles[i][j] == 3) {
        DRAW_SPRITE((byte *)tile_sprites[state.tiles[i][j]], j * 8, i * 8, 8, 8,
                    /*flip*/ 0, DRAW_XOR);
        if (--state.coins_remaining == 0) {
          return 2;
        }
        state.tiles[i][j] = 0;
      }
    }
  }
  state.last_block_x = state.block_x;
  state.last_block_y = state.block_y;
  return 0;
}

static void drawBlock() {
  DRAW_SPRITE((byte *)block_sprites[state.block_moves_available], state.block_x,
              state.block_y, BLOCK_WIDTH, BLOCK_HEIGHT,
              /*flip*/ 0, DRAW_OR);
}

static void eraseBlock() {
  DRAW_SPRITE((byte *)block_sprites[state.block_moves_available], state.block_x,
              state.block_y, BLOCK_WIDTH, BLOCK_HEIGHT,
              /*flip*/ 0, DRAW_XOR);
}

static void drawTiles() {
  uint8_t i;
  uint8_t j;
  for (i = 0; i < TILE_ROWS; ++i) {
    for (j = 0; j < TILE_COLUMNS; ++j) {
      if (state.tiles[i][j] == 3) {
        ++state.coins_remaining;
      }
      DRAW_SPRITE((byte *)tile_sprites[state.tiles[i][j]], j * 8, i * 8, 8, 8,
                  /*flip*/ 0, DRAW_OR);
    }
  }
}

static void initGameState() {
  memset(&state, 0, sizeof(state));
  state.block_moves_available = 2;
}

static void initLevel1() {
  uint8_t i;
  uint8_t j;
  initGameState();

  for (i = 0; i < TILE_ROWS; ++i) {
    for (j = 0; j < TILE_COLUMNS; ++j) {
      state.tiles[i][j] = 3;
    }
  }

  for (i = 0; i < TILE_ROWS; ++i) {
    state.tiles[i][0] = 1;
    state.tiles[i][15] = 1;
  }

  for (j = 0; j < TILE_COLUMNS; ++j) {
    state.tiles[0][j] = 1;
    state.tiles[7][j] = 1;
  }

  state.block_x = 12;
  state.block_y = 8;
}

static void initLevel2() {
  uint8_t j;
  initLevel1();

  for (j = 0; j < 4; ++j) {
    state.tiles[j][7] = 2;
    state.tiles[j + 4][8] = 2;
  }
}

static void initLevel3() {
  uint8_t i;
  initLevel1();

  for (i = 0; i < TILE_COLUMNS; ++i) {
    state.tiles[7][i] = 2;
  }
}

static void initLevel4() {
  uint8_t i;
  initLevel3();

  for (i = 0; i < TILE_COLUMNS; ++i) {
    state.tiles[0][i] = 2;
  }
}

static void initLevel5() {
  uint8_t i;
  uint8_t j;
  initLevel4();

  for (i = 0; i < TILE_COLUMNS; ++i) {
    state.tiles[0][i] = 2;
  }
  for (j = 0; j < TILE_ROWS; ++j) {
    state.tiles[j][0] = 2;
    state.tiles[j][15] = 2;
  }
  for (j = 4; j < 12; ++j) {
    state.tiles[3][j] = 1;
    state.tiles[4][j] = 1;
  }
}

static void initLevel6() {
  uint8_t j;
  initLevel5();

  for (j = 0; j < TILE_ROWS; ++j) {
    state.tiles[j][7] = 2;
    state.tiles[j][8] = 2;
  }
}

static void initLevel7() {
  initLevel6();

  state.tiles[1][6] = 2;
  state.tiles[2][6] = 2;
  state.tiles[1][9] = 2;
  state.tiles[2][9] = 2;
}

static void initLevel8() {
  uint8_t i;
  uint8_t j;
  initLevel1();
  for (i = 0; i < TILE_ROWS; ++i) {
    state.tiles[i][0] = 2;
    state.tiles[i][15] = 2;
  }
  for (j = 3; j < TILE_COLUMNS - 3; ++j) {
    state.tiles[3][j] = 2;
    state.tiles[4][j] = 2;
  }
  for (j = 0; j < TILE_COLUMNS; ++j) {
    state.tiles[0][j] = 2;
    state.tiles[7][j] = 2;
  }
  state.tiles[3][0] = 1;
  state.tiles[4][0] = 1;
  state.tiles[3][15] = 1;
  state.tiles[4][15] = 1;
  state.tiles[0][7] = 1;
  state.tiles[0][8] = 1;
  state.tiles[7][7] = 1;
  state.tiles[7][8] = 1;
}

static void (*levels[])() = {initLevel1, initLevel2, initLevel3, initLevel4,
                             initLevel5, initLevel6, initLevel7, initLevel8};

constexpr unsigned char bounce_splash_bin[] = {
    0x89, 0x00, 0x02, 0x10, 0x02, 0x20, 0x02, 0x40, 0x01, 0x80, 0x04, 0x00,
    0x02, 0x80, 0x03, 0x40, 0x05, 0x20, 0x07, 0x10, 0x01, 0x20, 0x01, 0x40,
    0x01, 0x80, 0x68, 0x00, 0x01, 0x01, 0x01, 0x02, 0x01, 0x05, 0x01, 0x19,
    0x01, 0x60, 0x01, 0x80, 0x10, 0x00, 0x01, 0x80, 0x01, 0x60, 0x01, 0x1f,
    0x6c, 0x00, 0x01, 0x07, 0x01, 0xf8, 0x02, 0x80, 0x03, 0xc0, 0x01, 0xa0,
    0x02, 0x20, 0x02, 0x10, 0x01, 0x08, 0x02, 0x04, 0x01, 0x02, 0x01, 0x01,
    0x0d, 0x00, 0x04, 0x80, 0x07, 0x00, 0x01, 0x80, 0x09, 0x00, 0x01, 0xc0,
    0x05, 0x00, 0x01, 0x60, 0x01, 0x80, 0x02, 0x00, 0x01, 0xc0, 0x01, 0x20,
    0x01, 0x10, 0x01, 0x60, 0x01, 0x80, 0x06, 0x00, 0x02, 0x80, 0x05, 0x40,
    0x07, 0x00, 0x04, 0x80, 0x27, 0x00, 0x01, 0xe1, 0x01, 0x1e, 0x05, 0x00,
    0x04, 0x01, 0x04, 0x02, 0x01, 0x04, 0x02, 0x08, 0x01, 0x10, 0x01, 0x20,
    0x01, 0x40, 0x01, 0x80, 0x04, 0x00, 0x01, 0x70, 0x01, 0x9e, 0x01, 0x01,
    0x04, 0x00, 0x01, 0x01, 0x01, 0x06, 0x01, 0x38, 0x01, 0xc0, 0x03, 0x00,
    0x01, 0x01, 0x01, 0x3e, 0x01, 0xc0, 0x07, 0x00, 0x01, 0x0f, 0x01, 0xf0,
    0x05, 0x00, 0x01, 0x03, 0x01, 0xfc, 0x01, 0x03, 0x05, 0x00, 0x01, 0xff,
    0x02, 0x00, 0x01, 0xfc, 0x01, 0x02, 0x01, 0x01, 0x0a, 0x00, 0x01, 0xf0,
    0x01, 0x4c, 0x01, 0x42, 0x01, 0x41, 0x04, 0x40, 0x01, 0x41, 0x01, 0x21,
    0x01, 0x22, 0x01, 0x14, 0x01, 0x08, 0x1f, 0x00, 0x01, 0xc0, 0x01, 0x30,
    0x01, 0x4e, 0x01, 0x41, 0x0d, 0x80, 0x06, 0x40, 0x01, 0x20, 0x01, 0x10,
    0x01, 0x0f, 0x04, 0x00, 0x01, 0x3f, 0x01, 0xc0, 0x05, 0x00, 0x01, 0x80,
    0x01, 0x40, 0x01, 0x3f, 0x05, 0x00, 0x01, 0x03, 0x01, 0x1c, 0x01, 0x20,
    0x01, 0x40, 0x02, 0x80, 0x01, 0x40, 0x01, 0x20, 0x01, 0x10, 0x01, 0x3f,
    0x01, 0xc0, 0x05, 0x00, 0x01, 0xff, 0x06, 0x00, 0x01, 0x7f, 0x02, 0x00,
    0x01, 0x07, 0x01, 0x18, 0x01, 0x20, 0x01, 0x40, 0x01, 0x80, 0x07, 0x00,
    0x01, 0x80, 0x01, 0x03, 0x01, 0x0c, 0x01, 0x30, 0x01, 0x40, 0x06, 0x80,
    0x01, 0x40, 0x01, 0x20, 0x1f, 0x00, 0x01, 0x04, 0x01, 0x03, 0x07, 0x00,
    0x06, 0x80, 0x08, 0x40, 0x0a, 0x20, 0x05, 0x11, 0x18, 0x10, 0x01, 0x11,
    0x03, 0x10, 0x09, 0x20, 0x02, 0x40, 0x07, 0x41, 0x09, 0x80, 0x1a, 0x00,
    0x04, 0x04, 0x08, 0x02, 0x06, 0x01, 0x54, 0x00, 0x06, 0x01, 0x04, 0x02,
    0x07, 0x00};
constexpr unsigned int bounce_splash_bin_len = 350;

void splash_screen() {
  uint8_t splash[1024];
  uint8_t *out = splash;
  for (int i = 0; i < bounce_splash_bin_len; i += 2) {
    int len = bounce_splash_bin[i];
    memset(out, bounce_splash_bin[i + 1], len);
    out += len;
  }
  DRAW_SPRITE((byte *)splash, 0, 0, 128, 64,
              /*flip*/ 0, DRAW_OR);
}

int main() {
  uint8_t status;
  uint8_t level = 0;
  CLEAR();
  splash_screen();
  DISPLAY();
  while ((~READ_BUTTONS() & 16) == 0)
    ;
  while ((~READ_BUTTONS() & 16) != 0)
    ;
  CLEAR();

  // Initialize the API
  api_init();

BEGIN_GAME:
  uint32_t timer = 0;

  static const char *levelNames[] = {
      "level 1", "level 2", "level 3", "level 4",
      "level 5", "level 6", "level 7", "level 8",
  };
  level = menu(levelNames, 1, 4, 7, 14, level);

  levels[level]();

  // Clear the graphics in video memory
  CLEAR();

  drawTiles();

  for (;;) {
    eraseBlock();

    processInput(READ_BUTTONS());

    status = handlePhysics();

    drawBlock();

    SET_CURSOR(0, 0);
    DRAW_STRING(string::concat(timer));

    // Push video memory to the OLED
    DISPLAY();

    // Wait for next interrupt
    ++timer;
    WAIT();

    if (status == 1) {
      DELAY_MS(255);
      goto BEGIN_GAME;
    }
    if (status == 2) {
      DELAY_MS(255);
      ++level;
      if (level >= (uint8_t)(sizeof(levels) / sizeof(levels[0]))) {
        level = 0;
      }
      goto BEGIN_GAME;
    }
  }
}
