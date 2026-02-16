

# File isotp\_user.h

[**File List**](files.md) **>** [**inc**](dir_bfccd401955b95cf8c75461437045ac0.md) **>** [**isotp\_user.h**](isotp__user_8h.md)

[Go to the documentation of this file](isotp__user_8h.md)


```C++

#ifndef ISOTPC_USER_H
#define ISOTPC_USER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

void isotp_user_debug(const char* message, ...);

int isotp_user_send_can(const uint32_t arbitration_id, const uint8_t* data, const uint8_t size
#ifdef ISO_TP_USER_SEND_CAN_ARG
                        ,
                        void* arg
#endif
);

uint32_t isotp_user_get_us(void);

#ifdef __cplusplus
}
#endif

#endif /* ISOTPC_USER_H */
```


