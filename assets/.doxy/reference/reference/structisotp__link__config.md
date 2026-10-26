

# Struct isotp\_link\_config



[**ClassList**](annotated.md) **>** [**isotp\_link\_config**](structisotp__link__config.md)



_Link configuration passed to_ [_**isotp\_link\_init**_](group__isotp__link.md#function-isotp_link_init) _._[More...](#detailed-description)

* `#include <isotp_link.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  void \* | [**app\_context**](#variable-app_context)  <br>_Context for the three callbacks above._  |
|  uint8\_t | [**block\_size**](#variable-block_size)  <br>_BS requested from the sender, 0 = unlimited._  |
|  uint32\_t | [**n\_bs\_timeout\_us**](#variable-n_bs_timeout_us)  <br>_N\_Bs, 1..2^31-1 us._  |
|  uint32\_t | [**n\_cr\_timeout\_us**](#variable-n_cr_timeout_us)  <br>_N\_Cr, 1..2^31-1 us._  |
|  [**isotp\_message\_handler\_t**](group__isotp__link.md#typedef-isotp_message_handler_t) | [**on\_message**](#variable-on_message)  <br>_Received message; NULL discards it._  |
|  [**isotp\_result\_fn**](group__isotp__link.md#typedef-isotp_result_fn) | [**on\_rx\_error**](#variable-on_rx_error)  <br>_Failed reception; may be NULL._  |
|  [**isotp\_result\_fn**](group__isotp__link.md#typedef-isotp_result_fn) | [**on\_tx\_done**](#variable-on_tx_done)  <br>_End of a transmission; may be NULL._  |
|  bool | [**padding**](#variable-padding)  <br>_Pad every sent frame to 8 bytes._  |
|  uint8\_t | [**padding\_value**](#variable-padding_value)  <br>_Padding byte, used when padding is true._  |
|  uint32\_t | [**st\_min\_us**](#variable-st_min_us)  <br>_STmin requested from the sender, 0..127000 us._  |
|  uint32\_t | [**tx\_id**](#variable-tx_id)  <br>_CAN ID of every frame this link sends._  |
|  uint8\_t | [**wft\_max**](#variable-wft_max)  <br>_N\_WFTmax: Wait frames accepted in a row._  |
|  [**isotp\_can\_write\_fn**](group__isotp__link.md#typedef-isotp_can_write_fn) | [**write**](#variable-write)  <br>_Frame writer (required)._  |
|  void \* | [**write\_context**](#variable-write_context)  <br>_Context for write._  |












































## Detailed Description


Start from [**isotp\_link\_config\_default**](group__isotp__link.md#function-isotp_link_config_default), then set the CAN ID, the writer and the callbacks. The link keeps a copy. 


    
## Public Attributes Documentation




### variable app\_context 

_Context for the three callbacks above._ 
```C++
void* isotp_link_config::app_context;
```




<hr>



### variable block\_size 

_BS requested from the sender, 0 = unlimited._ 
```C++
uint8_t isotp_link_config::block_size;
```




<hr>



### variable n\_bs\_timeout\_us 

_N\_Bs, 1..2^31-1 us._ 
```C++
uint32_t isotp_link_config::n_bs_timeout_us;
```




<hr>



### variable n\_cr\_timeout\_us 

_N\_Cr, 1..2^31-1 us._ 
```C++
uint32_t isotp_link_config::n_cr_timeout_us;
```




<hr>



### variable on\_message 

_Received message; NULL discards it._ 
```C++
isotp_message_handler_t isotp_link_config::on_message;
```




<hr>



### variable on\_rx\_error 

_Failed reception; may be NULL._ 
```C++
isotp_result_fn isotp_link_config::on_rx_error;
```




<hr>



### variable on\_tx\_done 

_End of a transmission; may be NULL._ 
```C++
isotp_result_fn isotp_link_config::on_tx_done;
```




<hr>



### variable padding 

_Pad every sent frame to 8 bytes._ 
```C++
bool isotp_link_config::padding;
```




<hr>



### variable padding\_value 

_Padding byte, used when padding is true._ 
```C++
uint8_t isotp_link_config::padding_value;
```




<hr>



### variable st\_min\_us 

_STmin requested from the sender, 0..127000 us._ 
```C++
uint32_t isotp_link_config::st_min_us;
```




<hr>



### variable tx\_id 

_CAN ID of every frame this link sends._ 
```C++
uint32_t isotp_link_config::tx_id;
```




<hr>



### variable wft\_max 

_N\_WFTmax: Wait frames accepted in a row._ 
```C++
uint8_t isotp_link_config::wft_max;
```




<hr>



### variable write 

_Frame writer (required)._ 
```C++
isotp_can_write_fn isotp_link_config::write;
```




<hr>



### variable write\_context 

_Context for write._ 
```C++
void* isotp_link_config::write_context;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `inc/isotp_link.h`

