#include <api.h>

#include "player.h"
#include <stdint.h>

byte *const frames[] = {(byte *)player_bin, (byte *)player2_bin};

constexpr int kRegenShieldsCooldownInit = 50;
uint8_t regenShieldsCooldown = 0;
int8_t health = 64;
int8_t energy = 64;

void take_damage(int8_t damage) {
  /* take damage */
  regenShieldsCooldown = kRegenShieldsCooldownInit;
  if (energy > 0) {
    energy -= damage;
  } else {
    health -= damage;
    if (health <= 0) {
      CLEAR();
      DRAW_STRING("game over");
      DISPLAY();
      for (;;)
        WAIT();
    }
  }
}

int main() {
  uint8_t x = 54;
  uint8_t y = 48;
  constexpr int kMaxBullets = 64;
  constexpr int kShootCooldownInit = 5;
  uint8_t shootCooldown = 0;
  constexpr int kYSpeed = 2;
  constexpr int kXSpeed = 3;
  constexpr int kHudWidth = 1;

  int8_t bulletX[kMaxBullets] = {-1};
  int8_t bulletY[kMaxBullets] = {-1};
  int8_t nextBullet = 0;
  uint8_t frame = 0;
  uint8_t counter = 0;
  for (;;) {
    if (++counter == 2) {
      frame = (frame + 1) % 2;
      counter = 0;
    }
    constexpr int kBulletSpeed = 3;
    for (int i = 0; i < kMaxBullets; ++i) {
      if (bulletY[i] >= 0) {
        bulletY[i] -= kBulletSpeed;
      }
    }
    if (shootCooldown > 0) {
      --shootCooldown;
    }
    if (regenShieldsCooldown == 0 && energy < 64) {
      ++energy;
    }
    if (regenShieldsCooldown > 0) {
      --regenShieldsCooldown;
    }

    uint8_t buttons = ~READ_BUTTONS();
    if (buttons & 1 && y >= kYSpeed) {
      y -= kYSpeed;
    }
    if (buttons & 2 && y <= 64 - 16 - kYSpeed) {
      y += kYSpeed;
    }
    if (buttons & 4 && x >= kHudWidth + kXSpeed) {
      x -= kXSpeed;
    }
    if (buttons & 8 && x <= 128 - kHudWidth - 20 - kXSpeed) {
      x += kXSpeed;
    }
    if (buttons & 16 && shootCooldown == 0 && energy > 0) {
      --energy;
      regenShieldsCooldown = kRegenShieldsCooldownInit;
      shootCooldown = kShootCooldownInit;
      bulletX[nextBullet] = x + 4;
      bulletY[nextBullet] = y;
      nextBullet = (nextBullet + 1) % kMaxBullets;
    }
    if (buttons & 32) {
      take_damage(4);
    }

    CLEAR();
    for (int i = 0; i < kMaxBullets; ++i) {
      if (bulletY[i] >= 0) {
        SET_PIXEL(bulletX[i], bulletY[i], 1);
        SET_PIXEL(bulletX[i] + 11, bulletY[i], 1);
      }
    }
    DRAW_SPRITE(frames[frame], x, y, 20, 16, 0, DRAW_OR);
    DRAW_LINE(0, 64 - energy, 0, 64, 1);
    DRAW_LINE(127, 64 - health, 127, 64, 1);
    DISPLAY();

    WAIT();
  }
}
