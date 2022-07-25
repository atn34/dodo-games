#pragma once

#include <stdint.h>

uint8_t menu_impl(const char **options, uint8_t length, int8_t beginRow,
                  int8_t beginColumn, int8_t endRow, int8_t endColumn,
                  uint8_t selection = 0);

template <int kLen>
uint8_t menu(const char *(&array)[kLen], int8_t beginRow, int8_t beginColumn,
             int8_t endRow, int8_t endColumn, uint8_t selection = 0) {
  return menu_impl(array, kLen, beginRow, beginColumn, endRow, endColumn,
                   selection);
}
