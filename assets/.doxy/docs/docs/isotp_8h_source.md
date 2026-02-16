

# File isotp.h

[**File List**](files.md) **>** [**isotp.h**](isotp_8h.md)

[Go to the documentation of this file](isotp_8h.md)


```C++

#ifndef ISOTPC_H
#define ISOTPC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "isotp_config.h"
#include "isotp_defines.h"
#include "isotp_user.h"

typedef struct IsoTpLink
{
    /* sender parameters */
    uint32_t send_arbitration_id; 
    /* message buffer */
    uint8_t* send_buffer;   
    uint32_t send_buf_size; 
    uint32_t send_size;     
    uint32_t send_offset;   
    /* multi-frame flags */
    uint8_t send_sn;              
    uint32_t send_bs_remain;      
    uint32_t send_st_min_us;      
    uint8_t send_wtf_count;       
    uint32_t send_timer_st;       
    uint32_t send_timer_bs;       
    int32_t send_protocol_result; 
    uint8_t send_status;          
    /* receiver parameters */
    uint32_t receive_arbitration_id; 
    /* message buffer */
    uint8_t* receive_buffer;   
    uint32_t receive_buf_size; 
    uint32_t receive_size;     
    uint32_t receive_offset;   
    /* multi-frame control */
    uint8_t receive_sn;          
    uint8_t receive_bs_count;    
    uint32_t receive_timer_cr;   
    int receive_protocol_result; 
    uint8_t receive_status;      
#if defined(ISO_TP_USER_SEND_CAN_ARG)
    void* user_send_can_arg; 
#endif

#ifdef ISO_TP_TRANSMIT_COMPLETE_CALLBACK
    isotp_tx_done_cb tx_done_cb; 
    void* tx_done_cb_arg;        
#endif

#ifdef ISO_TP_RECEIVE_COMPLETE_CALLBACK
    isotp_rx_done_cb rx_done_cb; 
    void* rx_done_cb_arg;        
#endif

} IsoTpLink;

void isotp_init_link(IsoTpLink* link, uint32_t sendid, uint8_t* sendbuf, uint32_t sendbufsize,
                     uint8_t* recvbuf, uint32_t recvbufsize);

void isotp_destroy_link(IsoTpLink* link);

void isotp_poll(IsoTpLink* link);

void isotp_on_can_message(IsoTpLink* link, const uint8_t* data, uint8_t len);

int isotp_send(IsoTpLink* link, const uint8_t payload[], uint32_t size);

int isotp_send_with_id(IsoTpLink* link, uint32_t id, const uint8_t payload[], uint32_t size);

int isotp_receive(IsoTpLink* link, uint8_t* payload, const uint32_t payload_size,
                  uint32_t* out_size);

#ifdef ISO_TP_TRANSMIT_COMPLETE_CALLBACK
void isotp_set_tx_done_cb(IsoTpLink* link, isotp_tx_done_cb cb, void* arg);
#endif

#ifdef ISO_TP_RECEIVE_COMPLETE_CALLBACK
void isotp_set_rx_done_cb(IsoTpLink* link, isotp_rx_done_cb cb, void* arg);
#endif

#ifdef __cplusplus
}
#endif

#endif /* ISOTPC_H */
```


