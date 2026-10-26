

# File isotp\_defs.h

[**File List**](files.md) **>** [**inc**](dir_bfccd401955b95cf8c75461437045ac0.md) **>** [**isotp\_defs.h**](isotp__defs_8h.md)

[Go to the documentation of this file](isotp__defs_8h.md)


```C++

#ifndef ISOTP_DEFS_H
#define ISOTP_DEFS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define ISOTP_OK 0     
#define ISOTP_ERROR -1 
#define ISOTP_BUSY -2  
#define ISOTP_PCI_SINGLE_FRAME 0x0U      
#define ISOTP_PCI_FIRST_FRAME 0x1U       
#define ISOTP_PCI_CONSECUTIVE_FRAME 0x2U 
#define ISOTP_PCI_FLOW_CONTROL 0x3U      
#define ISOTP_FS_CONTINUE 0x0U 
#define ISOTP_FS_WAIT 0x1U     
#define ISOTP_FS_OVERFLOW 0x2U 
#define ISOTP_CAN_DL 8U             
#define ISOTP_SF_MAX_DL 7U          
#define ISOTP_FF_DL_12BIT_MAX 4095U 
#define ISOTP_FF_SHORT_DATA 6U      
#define ISOTP_FF_LONG_DATA 2U       
#define ISOTP_CF_MAX_DATA 7U        
#define ISOTP_FC_DL 3U              
#define ISOTP_SN_MASK 0x0FU         
#define ISOTP_FIRST_SN 1U           
#define ISOTP_ST_MIN_MAX_US 127000U 
#define ISOTP_N_OK 0            
#define ISOTP_N_TIMEOUT_A -1    
#define ISOTP_N_TIMEOUT_BS -2   
#define ISOTP_N_TIMEOUT_CR -3   
#define ISOTP_N_WRONG_SN -4     
#define ISOTP_N_INVALID_FS -5   
#define ISOTP_N_UNEXP_PDU -6    
#define ISOTP_N_WFT_OVRN -7     
#define ISOTP_N_BUFFER_OVFLW -8 
#define ISOTP_N_ERROR -9        
static inline uint32_t isotp_get_u32_be(const uint8_t* data)
{
    return (((uint32_t) data[0]) << 24U) | (((uint32_t) data[1]) << 16U)
           | (((uint32_t) data[2]) << 8U) | ((uint32_t) data[3]);
}

static inline void isotp_put_u32_be(uint8_t* data, uint32_t value)
{
    data[0] = (uint8_t) (value >> 24U);
    data[1] = (uint8_t) (value >> 16U);
    data[2] = (uint8_t) (value >> 8U);
    data[3] = (uint8_t) value;
}

#ifdef __cplusplus
}
#endif

#endif /* ISOTP_DEFS_H */

```


