#include "menu.hpp"

#include <api.h>
#include <string.h>

constexpr int8_t kCharWidth = 7;
constexpr int8_t kCharHeight = 8;
const char *ellipses = "\xfa\xfa\xfa";

uint8_t menu_impl(const char **options, uint8_t length, int8_t beginRow,
                  int8_t beginColumn, int8_t endRow, int8_t endColumn,
                  uint8_t selection) {
  const int8_t kHeight = endRow - beginRow;
  const int8_t kWidth = endColumn - beginColumn;
  const uint8_t kMaxLen = kWidth - 2;
  const int8_t kMaxRowsVisible = kHeight - 2;

  const uint8_t width = (endColumn - beginColumn) * kCharWidth;
  const uint8_t height = (endRow - beginRow) * kCharHeight;

  uint8_t horizontalScroll = 0;

  const int size = int(width) * int(height + 1);
  uint8_t background[size];
  COPY_BACKGROUND(background, beginColumn * kCharWidth, beginRow * kCharHeight,
                  width, height, 0 /*vmem -> buffer*/);

  constexpr uint8_t kInitialRepeatDelay = 8;
  uint8_t buttons = 0;
  uint8_t lastButtons = 0;
  bool sameAsLast = false;
  uint8_t repeatDelay = kInitialRepeatDelay;
  bool noInputRecorded = false;
  goto display;
  for (;;) {
    buttons = ~READ_BUTTONS();
    sameAsLast = buttons == lastButtons;
    lastButtons = buttons;
    if (!buttons) {
      noInputRecorded = true;
      continue;
    }
    if (!noInputRecorded) {
      continue;
    }
    if (sameAsLast) {
      if (repeatDelay) {
        --repeatDelay;
        WAIT();
        continue;
      }
    } else {
      repeatDelay = kInitialRepeatDelay;
    }
    if (buttons & 1 /*up*/) {
      selection = selection == 0 ? length - 1 : selection - 1;
      horizontalScroll = 0;
    } else if (buttons & 2 /*down*/) {
      selection = (selection + 1) % length;
      horizontalScroll = 0;
    } else if (buttons & 4 /*left*/ && horizontalScroll > 0) {
      horizontalScroll -= 1;
    } else if (buttons & 8 /*right*/ &&
               strlen(options[selection]) - horizontalScroll > kMaxLen) {
      horizontalScroll += 1;
    } else if (buttons & 16 /*A*/) {
      break;
    }

  display:
    CLEAR_SPRITE(beginColumn * kCharWidth + 1, beginRow * kCharHeight, width,
                 height);

    SET_CURSOR(beginRow, beginColumn);
    DRAW_STRING("\xc9");
    SET_CURSOR(beginRow, endColumn - 1);
    DRAW_STRING("\xbb");
    SET_CURSOR(endRow - 1, beginColumn);
    DRAW_STRING("\xc8");
    SET_CURSOR(endRow - 1, endColumn - 1);
    DRAW_STRING("\xbc");

    const uint8_t view = [&]() {
      if (selection + kMaxRowsVisible / 2 >= length) {
        return length - kMaxRowsVisible;
      } else if (selection >= kMaxRowsVisible / 2) {
        return selection - kMaxRowsVisible / 2;
      } else {
        return 0;
      }
    }();

    uint8_t i = view;
    if (i > 0) {
      SET_CURSOR(beginRow, beginColumn + 1);
      DRAW_STRING(ellipses);
    }

    for (; i < length && i < view + kMaxRowsVisible; ++i) {
      uint8_t scroll = i == selection ? horizontalScroll : 0;
      SET_CURSOR(beginRow + 1 + i - view, beginColumn + 1);
      if (strlen(options[i]) - scroll > kMaxLen) {
        char truncated[kMaxLen + 2];
        memcpy(truncated, options[i] + scroll, kMaxLen);
        truncated[kMaxLen] = '\xfa';
        truncated[kMaxLen + 1] = 0;
        DRAW_STRING(truncated);
      } else {
        DRAW_STRING(options[i] + scroll);
      }
    }
    if (i < length) {
      SET_CURSOR(endRow - 1, beginColumn + 1);
      DRAW_STRING(ellipses);
    }
    SET_CURSOR(beginRow + 1 + selection - view, beginColumn);
    DRAW_STRING(">");

    DRAW_LINE(beginColumn * kCharWidth + 1,
              beginRow * kCharHeight + kCharHeight - 1,
              beginColumn * kCharWidth + width - 1,
              beginRow * kCharHeight + kCharHeight - 1, 0);

    DRAW_LINE(beginColumn * kCharWidth, beginRow * kCharHeight,
              beginColumn * kCharWidth, beginRow * kCharHeight + height - 2, 1);
    DRAW_LINE(beginColumn * kCharWidth + width - 1, beginRow * kCharHeight,
              beginColumn * kCharWidth + width - 1,
              beginRow * kCharHeight + height - 2, 1);
    DRAW_LINE(beginColumn * kCharWidth, beginRow * kCharHeight,
              beginColumn * kCharWidth + width - 1, beginRow * kCharHeight, 1);
    DRAW_LINE(beginColumn * kCharWidth, beginRow * kCharHeight + height - 2,
              beginColumn * kCharWidth + width - 1,
              beginRow * kCharHeight + height - 2, 1);

    DISPLAY();
    WAIT();
  }

  COPY_BACKGROUND(background, beginColumn * kCharWidth, beginRow * kCharHeight,
                  width, height, 1 /*buffer -> vmem*/);
  DISPLAY();
  return selection;
}