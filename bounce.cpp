#include "fixed_point.hpp"
#include "menu.hpp"

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

const char *itoa(uint32_t n) {
  static char buf[11];
  buf[10] = 0;
  for (int i = 9; i >= 0; --i) {
    buf[i] = '0' + (n % 10);
    n /= 10;
    if (!n) {
      return buf + i;
    }
  }
  return "?";
}

int main() {
  uint8_t status;
  uint8_t level = 0;
  CLEAR();
  // Initialize the API
  api_init();

BEGIN_GAME:
  uint32_t timer = 0;
  level = menu(
      {
          "level 1",
          "level 2",
          "level 3",
          "level 4",
          "level 5",
          "level 6",
          "level 7",
          "level 8",
      },
      1, 4, 7, 14, level);

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
    DRAW_STRING(itoa(timer));

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
