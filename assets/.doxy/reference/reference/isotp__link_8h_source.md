

# File isotp\_link.h

[**File List**](files.md) **>** [**inc**](dir_bfccd401955b95cf8c75461437045ac0.md) **>** [**isotp\_link.h**](isotp__link_8h.md)

[Go to the documentation of this file](isotp__link_8h.md)


```C++

#ifndef ISOTP_LINK_H
#define ISOTP_LINK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "isotp_config.h"
#include "isotp_protocol.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*isotp_can_write_fn)(void* context, uint32_t can_id, const uint8_t* data,
                                  size_t length);

typedef void (*isotp_message_handler_t)(const uint8_t* data, uint32_t length, void* context);

typedef void (*isotp_result_fn)(int n_result, void* context);

struct isotp_link_config
{
    uint32_t tx_id;                     
    isotp_can_write_fn write;           
    void* write_context;                
    isotp_message_handler_t on_message; 
    isotp_result_fn on_tx_done;         
    isotp_result_fn on_rx_error;        
    void* app_context;                  
    uint8_t block_size;                 
    uint32_t st_min_us;                 
    uint32_t n_bs_timeout_us;           
    uint32_t n_cr_timeout_us;           
    uint8_t wft_max;                    
    bool padding;                       
    uint8_t padding_value;              
};

enum isotp_tx_state
{
    ISOTP_TX_IDLE = 0, 
    ISOTP_TX_WAIT_FC,  
    ISOTP_TX_SEND_CF   
};

enum isotp_rx_state
{
    ISOTP_RX_IDLE = 0, 
    ISOTP_RX_RECEIVING 
};

struct isotp_link
{
    struct isotp_link_config config;         
    uint8_t tx_buffer[ISOTP_TX_BUFFER_SIZE]; 
    uint32_t tx_size;                        
    uint32_t tx_offset;                      
    uint32_t tx_st_min_us;                   
    uint32_t tx_deadline_us;                 
    enum isotp_tx_state tx_state;            
    int tx_result;                           
    uint8_t tx_sn;                           
    uint8_t tx_block_remaining;              
    bool tx_block_unlimited;                 
    uint8_t tx_wft_count;                    
    uint8_t rx_buffer[ISOTP_RX_BUFFER_SIZE]; 
    uint32_t rx_size;                        
    uint32_t rx_offset;                      
    uint32_t rx_deadline_us;                 
    enum isotp_rx_state rx_state;            
    int rx_result;                           
    uint8_t rx_sn;                           
    uint8_t rx_block_count;                  
    uint32_t invalid_frames;                 
};

void isotp_link_config_default(struct isotp_link_config* config);

int isotp_link_init(struct isotp_link* link, const struct isotp_link_config* config);

void isotp_link_receive(struct isotp_link* link, const uint8_t* data, size_t length,
                        uint32_t now_us);

int isotp_link_send(struct isotp_link* link, const uint8_t* payload, uint32_t length,
                    uint32_t now_us);

int isotp_link_send_functional(const struct isotp_link* link, uint32_t can_id,
                               const uint8_t* payload, uint32_t length);

void isotp_link_poll(struct isotp_link* link, uint32_t now_us);

bool isotp_link_tx_busy(const struct isotp_link* link);

int isotp_link_tx_result(const struct isotp_link* link);

int isotp_link_rx_result(const struct isotp_link* link);

uint32_t isotp_link_invalid_frames(const struct isotp_link* link);

#ifdef __cplusplus
}
#endif

#endif /* ISOTP_LINK_H */

```


