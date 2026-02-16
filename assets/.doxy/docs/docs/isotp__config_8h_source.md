

# File isotp\_config.h

[**File List**](files.md) **>** [**inc**](dir_bfccd401955b95cf8c75461437045ac0.md) **>** [**isotp\_config.h**](isotp__config_8h.md)

[Go to the documentation of this file](isotp__config_8h.md)


```C++

#ifndef ISOTPC_CONFIG_H
#define ISOTPC_CONFIG_H

#ifndef ISO_TP_DEFAULT_BLOCK_SIZE
#define ISO_TP_DEFAULT_BLOCK_SIZE (8)
#endif

#ifndef ISO_TP_DEFAULT_ST_MIN_US
#define ISO_TP_DEFAULT_ST_MIN_US (0)
#endif

#ifndef ISO_TP_MAX_WFT_NUMBER
#define ISO_TP_MAX_WFT_NUMBER (1)
#endif

#ifndef ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US
#define ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US (100000)
#endif

/* #define ISO_TP_FRAME_PADDING */

#ifndef ISO_TP_FRAME_PADDING_VALUE
#define ISO_TP_FRAME_PADDING_VALUE (0xAA)
#endif

/* #define ISO_TP_USER_SEND_CAN_ARG */

/* #define ISO_TP_TRANSMIT_COMPLETE_CALLBACK */

/* #define ISO_TP_RECEIVE_COMPLETE_CALLBACK */

#endif /* ISOTPC_CONFIG_H */
```


