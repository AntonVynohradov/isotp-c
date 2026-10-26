

# Struct isotp\_link



[**ClassList**](annotated.md) **>** [**isotp\_link**](structisotp__link.md)



_Link state._ [More...](#detailed-description)

* `#include <isotp_link.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  struct [**isotp\_link\_config**](structisotp__link__config.md) | [**config**](#variable-config)  <br>_Copy of the configuration._  |
|  uint32\_t | [**invalid\_frames**](#variable-invalid_frames)  <br>_Malformed frames ignored._  |
|  uint8\_t | [**rx\_block\_count**](#variable-rx_block_count)  <br>_CFs left until the next Flow Control._  |
|  uint8\_t | [**rx\_buffer**](#variable-rx_buffer)  <br>_Message being received._  |
|  uint32\_t | [**rx\_deadline\_us**](#variable-rx_deadline_us)  <br>_N\_Cr expiry._  |
|  uint32\_t | [**rx\_offset**](#variable-rx_offset)  <br>_Bytes already received._  |
|  int | [**rx\_result**](#variable-rx_result)  <br>_N\_Result of the last reception._  |
|  uint32\_t | [**rx\_size**](#variable-rx_size)  <br>_Announced message length._  |
|  uint8\_t | [**rx\_sn**](#variable-rx_sn)  <br>_Expected SN._  |
|  enum [**isotp\_rx\_state**](group__isotp__link.md#enum-isotp_rx_state) | [**rx\_state**](#variable-rx_state)  <br>_Receiver state._  |
|  uint8\_t | [**tx\_block\_remaining**](#variable-tx_block_remaining)  <br>_CFs left in the current block._  |
|  bool | [**tx\_block\_unlimited**](#variable-tx_block_unlimited)  <br>_Receiver granted BS = 0._  |
|  uint8\_t | [**tx\_buffer**](#variable-tx_buffer)  <br>_Message being sent._  |
|  uint32\_t | [**tx\_deadline\_us**](#variable-tx_deadline_us)  <br>_N\_Bs expiry or time of the next CF._  |
|  uint32\_t | [**tx\_offset**](#variable-tx_offset)  <br>_Bytes already sent._  |
|  int | [**tx\_result**](#variable-tx_result)  <br>_N\_Result of the last transmission._  |
|  uint32\_t | [**tx\_size**](#variable-tx_size)  <br>_Length of the message being sent._  |
|  uint8\_t | [**tx\_sn**](#variable-tx_sn)  <br>_SN of the next Consecutive Frame._  |
|  uint32\_t | [**tx\_st\_min\_us**](#variable-tx_st_min_us)  <br>_STmin granted by the receiver._  |
|  enum [**isotp\_tx\_state**](group__isotp__link.md#enum-isotp_tx_state) | [**tx\_state**](#variable-tx_state)  <br>_Transmitter state._  |
|  uint8\_t | [**tx\_wft\_count**](#variable-tx_wft_count)  <br>_Wait frames received in a row._  |












































## Detailed Description


Treat the members as private; use the functions below. 


    
## Public Attributes Documentation




### variable config 

_Copy of the configuration._ 
```C++
struct isotp_link_config isotp_link::config;
```




<hr>



### variable invalid\_frames 

_Malformed frames ignored._ 
```C++
uint32_t isotp_link::invalid_frames;
```




<hr>



### variable rx\_block\_count 

_CFs left until the next Flow Control._ 
```C++
uint8_t isotp_link::rx_block_count;
```




<hr>



### variable rx\_buffer 

_Message being received._ 
```C++
uint8_t isotp_link::rx_buffer[ISOTP_RX_BUFFER_SIZE];
```




<hr>



### variable rx\_deadline\_us 

_N\_Cr expiry._ 
```C++
uint32_t isotp_link::rx_deadline_us;
```




<hr>



### variable rx\_offset 

_Bytes already received._ 
```C++
uint32_t isotp_link::rx_offset;
```




<hr>



### variable rx\_result 

_N\_Result of the last reception._ 
```C++
int isotp_link::rx_result;
```




<hr>



### variable rx\_size 

_Announced message length._ 
```C++
uint32_t isotp_link::rx_size;
```




<hr>



### variable rx\_sn 

_Expected SN._ 
```C++
uint8_t isotp_link::rx_sn;
```




<hr>



### variable rx\_state 

_Receiver state._ 
```C++
enum isotp_rx_state isotp_link::rx_state;
```




<hr>



### variable tx\_block\_remaining 

_CFs left in the current block._ 
```C++
uint8_t isotp_link::tx_block_remaining;
```




<hr>



### variable tx\_block\_unlimited 

_Receiver granted BS = 0._ 
```C++
bool isotp_link::tx_block_unlimited;
```




<hr>



### variable tx\_buffer 

_Message being sent._ 
```C++
uint8_t isotp_link::tx_buffer[ISOTP_TX_BUFFER_SIZE];
```




<hr>



### variable tx\_deadline\_us 

_N\_Bs expiry or time of the next CF._ 
```C++
uint32_t isotp_link::tx_deadline_us;
```




<hr>



### variable tx\_offset 

_Bytes already sent._ 
```C++
uint32_t isotp_link::tx_offset;
```




<hr>



### variable tx\_result 

_N\_Result of the last transmission._ 
```C++
int isotp_link::tx_result;
```




<hr>



### variable tx\_size 

_Length of the message being sent._ 
```C++
uint32_t isotp_link::tx_size;
```




<hr>



### variable tx\_sn 

_SN of the next Consecutive Frame._ 
```C++
uint8_t isotp_link::tx_sn;
```




<hr>



### variable tx\_st\_min\_us 

_STmin granted by the receiver._ 
```C++
uint32_t isotp_link::tx_st_min_us;
```




<hr>



### variable tx\_state 

_Transmitter state._ 
```C++
enum isotp_tx_state isotp_link::tx_state;
```




<hr>



### variable tx\_wft\_count 

_Wait frames received in a row._ 
```C++
uint8_t isotp_link::tx_wft_count;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `inc/isotp_link.h`

