/*
 * Put before every C snippet of the documentation by tools/docs/check_snippets.py:
 * the library and the board functions the snippets stand in for.
 *
 * SPDX-License-Identifier: MIT
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "isotp.h"

/* Board functions used by the snippets of docs/ (declare each one they call). */
int board_can_write(uint32_t can_id, const uint8_t* data, size_t length);
size_t board_can_read(uint32_t* can_id, uint8_t* data);
uint32_t board_micros(void);
