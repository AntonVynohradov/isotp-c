

# File isotp\_protocol.h

[**File List**](files.md) **>** [**inc**](dir_bfccd401955b95cf8c75461437045ac0.md) **>** [**isotp\_protocol.h**](isotp__protocol_8h.md)

[Go to the documentation of this file](isotp__protocol_8h.md)


```C++

#ifndef ISOTP_PROTOCOL_H
#define ISOTP_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#include "isotp_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct isotp_pdu
{
    uint8_t type;            
    uint32_t message_length; 
    uint8_t data_offset;     
    uint8_t data_length;     
    uint8_t sequence_number; 
    uint8_t flow_status;     
    uint8_t block_size;      
    uint8_t st_min;          
};

int isotp_decode(const uint8_t* frame, size_t length, struct isotp_pdu* pdu);

int isotp_encode_single(uint8_t* frame, size_t capacity, const uint8_t* payload, uint32_t length);

int isotp_encode_first(uint8_t* frame, size_t capacity, const uint8_t* payload,
                       uint32_t message_length);

int isotp_encode_consecutive(uint8_t* frame, size_t capacity, uint8_t sequence_number,
                             const uint8_t* data, uint32_t length);

int isotp_encode_flow_control(uint8_t* frame, size_t capacity, uint8_t flow_status,
                              uint8_t block_size, uint8_t st_min);

int isotp_frame_pad(uint8_t* frame, size_t capacity, size_t length, uint8_t value);

uint8_t isotp_st_min_encode(uint32_t us);

uint32_t isotp_st_min_decode(uint8_t st_min);

#ifdef __cplusplus
}
#endif

#endif /* ISOTP_PROTOCOL_H */

```


