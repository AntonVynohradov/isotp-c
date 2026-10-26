

# File isotp\_config.h

[**File List**](files.md) **>** [**inc**](dir_bfccd401955b95cf8c75461437045ac0.md) **>** [**isotp\_config.h**](isotp__config_8h.md)

[Go to the documentation of this file](isotp__config_8h.md)


```C++

#ifndef ISOTP_CONFIG_H
#define ISOTP_CONFIG_H

#ifndef ISOTP_TX_BUFFER_SIZE
#define ISOTP_TX_BUFFER_SIZE 4095U
#endif

#ifndef ISOTP_RX_BUFFER_SIZE
#define ISOTP_RX_BUFFER_SIZE 4095U
#endif

#ifndef ISOTP_DEFAULT_BLOCK_SIZE
#define ISOTP_DEFAULT_BLOCK_SIZE 8U
#endif

#ifndef ISOTP_DEFAULT_ST_MIN_US
#define ISOTP_DEFAULT_ST_MIN_US 0U
#endif

#ifndef ISOTP_DEFAULT_N_BS_TIMEOUT_US
#define ISOTP_DEFAULT_N_BS_TIMEOUT_US 1000000U
#endif

#ifndef ISOTP_DEFAULT_N_CR_TIMEOUT_US
#define ISOTP_DEFAULT_N_CR_TIMEOUT_US 1000000U
#endif

#ifndef ISOTP_DEFAULT_WFT_MAX
#define ISOTP_DEFAULT_WFT_MAX 10U
#endif

/*
 * A buffer holds at least one segmented message (8 bytes is the smallest) and
 * stays below 2^31, so every size and offset fits a uint32_t with headroom.
 */
#if (ISOTP_TX_BUFFER_SIZE < 8U) || (ISOTP_TX_BUFFER_SIZE > 0x7FFFFFFFU)
#error "ISOTP_TX_BUFFER_SIZE must be in the range 8..2147483647"
#endif

#if (ISOTP_RX_BUFFER_SIZE < 8U) || (ISOTP_RX_BUFFER_SIZE > 0x7FFFFFFFU)
#error "ISOTP_RX_BUFFER_SIZE must be in the range 8..2147483647"
#endif

#if (ISOTP_DEFAULT_BLOCK_SIZE > 255U)
#error "ISOTP_DEFAULT_BLOCK_SIZE must be in the range 0..255"
#endif

#if (ISOTP_DEFAULT_ST_MIN_US > 127000U)
#error "ISOTP_DEFAULT_ST_MIN_US must be in the range 0..127000"
#endif

#if (ISOTP_DEFAULT_N_BS_TIMEOUT_US < 1U) || (ISOTP_DEFAULT_N_BS_TIMEOUT_US > 0x7FFFFFFFU)
#error "ISOTP_DEFAULT_N_BS_TIMEOUT_US must be in the range 1..2147483647"
#endif

#if (ISOTP_DEFAULT_N_CR_TIMEOUT_US < 1U) || (ISOTP_DEFAULT_N_CR_TIMEOUT_US > 0x7FFFFFFFU)
#error "ISOTP_DEFAULT_N_CR_TIMEOUT_US must be in the range 1..2147483647"
#endif

#if (ISOTP_DEFAULT_WFT_MAX > 255U)
#error "ISOTP_DEFAULT_WFT_MAX must be in the range 0..255"
#endif

#endif /* ISOTP_CONFIG_H */

```


