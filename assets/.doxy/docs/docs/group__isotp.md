

# Group isotp



[**Modules**](modules.md) **>** [**isotp**](group__isotp.md)



_Public API for ISO-TP link management and message handling._ [More...](#detailed-description)












## Modules

| Type | Name |
| ---: | :--- |
| module | [**Configuration**](group__isotp__config.md) <br>_Build-time configuration for ISO-TP._  |
| module | [**Protocol definitions**](group__isotp__defines.md) <br>_Internal protocol definitions and type structures._  |
| module | [**User hooks**](group__isotp__user.md) <br>_User-implemented callbacks and platform hooks._  |




## Classes

| Type | Name |
| ---: | :--- |
| struct | [**IsoTpLink**](structIsoTpLink.md) <br>_Link state for a single ISO-TP connection._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef struct [**IsoTpLink**](structIsoTpLink.md) | [**IsoTpLink**](#typedef-isotplink)  <br>_Link state for a single ISO-TP connection._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**isotp\_destroy\_link**](#function-isotp_destroy_link) ([**IsoTpLink**](structIsoTpLink.md) \* link) <br>_Destroys the ISO-TP link and releases associated resources._  |
|  void | [**isotp\_init\_link**](#function-isotp_init_link) ([**IsoTpLink**](structIsoTpLink.md) \* link, uint32\_t sendid, uint8\_t \* sendbuf, uint32\_t sendbufsize, uint8\_t \* recvbuf, uint32\_t recvbufsize) <br>_Initialises the ISO-TP library._  |
|  void | [**isotp\_on\_can\_message**](#function-isotp_on_can_message) ([**IsoTpLink**](structIsoTpLink.md) \* link, const uint8\_t \* data, uint8\_t len) <br>_Handles incoming CAN messages. Determines whether an incoming message is a valid ISO-TP frame or not and handles it accordingly._  |
|  void | [**isotp\_poll**](#function-isotp_poll) ([**IsoTpLink**](structIsoTpLink.md) \* link) <br>_Polling function; call this function periodically to handle timeouts, send consecutive frames, etc._  |
|  int | [**isotp\_receive**](#function-isotp_receive) ([**IsoTpLink**](structIsoTpLink.md) \* link, uint8\_t \* payload, const uint32\_t payload\_size, uint32\_t \* out\_size) <br>_Receives and parses the received data and copies the parsed data into the internal buffer._  |
|  int | [**isotp\_send**](#function-isotp_send) ([**IsoTpLink**](structIsoTpLink.md) \* link, const uint8\_t payload, uint32\_t size) <br>_Sends ISO-TP frames via CAN, using the ID set in the initialising function._  |
|  int | [**isotp\_send\_with\_id**](#function-isotp_send_with_id) ([**IsoTpLink**](structIsoTpLink.md) \* link, uint32\_t id, const uint8\_t payload, uint32\_t size) <br>_See_ [_**isotp\_send**_](group__isotp.md#function-isotp_send) _, with the exception that this function is used only for functional addressing._ |
|  void | [**isotp\_set\_fc\_params**](#function-isotp_set_fc_params) ([**IsoTpLink**](structIsoTpLink.md) \* link, uint8\_t block\_size, uint32\_t st\_min\_us) <br>_Sets FlowControl parameters used by the receiver._  |
|  void | [**isotp\_set\_timeouts**](#function-isotp_set_timeouts) ([**IsoTpLink**](structIsoTpLink.md) \* link, uint32\_t n\_bs\_us, uint32\_t n\_cr\_us) <br>_Sets timing parameters for waiting on FlowControl and ConsecutiveFrame._  |




























## Detailed Description


This module groups the public interface and its supporting submodules: [**Configuration**](group__isotp__config.md), [**Protocol definitions**](group__isotp__defines.md), and [**User hooks**](group__isotp__user.md).


This group defines the public interface for initializing links, sending and receiving messages, handling incoming CAN frames, and polling the protocol state machine. It also exposes optional callback registration when enabled in configuration.




**
**


* Link lifecycle: [**isotp\_init\_link**](group__isotp.md#function-isotp_init_link), [**isotp\_destroy\_link**](group__isotp.md#function-isotp_destroy_link). 
* Message flow: [**isotp\_send**](group__isotp.md#function-isotp_send), [**isotp\_send\_with\_id**](group__isotp.md#function-isotp_send_with_id), [**isotp\_on\_can\_message**](group__isotp.md#function-isotp_on_can_message), [**isotp\_receive**](group__isotp.md#function-isotp_receive), [**isotp\_poll**](group__isotp.md#function-isotp_poll). 
* Link data model: [**IsoTpLink**](structIsoTpLink.md). 
* Optional callbacks: isotp\_set\_tx\_done\_cb, isotp\_set\_rx\_done\_cb. 







    
## Public Types Documentation




### typedef IsoTpLink 

_Link state for a single ISO-TP connection._ 
```
typedef struct IsoTpLink IsoTpLink;
```



The data stored in this struct is used internally and may be used by software programs using this library. 


        

<hr>
## Public Functions Documentation




### function isotp\_destroy\_link 

_Destroys the ISO-TP link and releases associated resources._ 
```
void isotp_destroy_link (
    IsoTpLink * link
) 
```





**Parameters:**


* `link` The 
```
IsoTpLink 
```
 instance to destroy. 




        

<hr>



### function isotp\_init\_link 

_Initialises the ISO-TP library._ 
```
void isotp_init_link (
    IsoTpLink * link,
    uint32_t sendid,
    uint8_t * sendbuf,
    uint32_t sendbufsize,
    uint8_t * recvbuf,
    uint32_t recvbufsize
) 
```





**Parameters:**


* `link` The 
```
IsoTpLink 
```
 instance used for transceiving data. 
* `sendid` The ID used to send data to other CAN nodes. 
* `sendbuf` A pointer to an area in memory which can be used as a buffer for data to be sent. 
* `sendbufsize` The size of the buffer area. 
* `recvbuf` A pointer to an area in memory which can be used as a buffer for data to be received. 
* `recvbufsize` The size of the buffer area. 




        

<hr>



### function isotp\_on\_can\_message 

_Handles incoming CAN messages. Determines whether an incoming message is a valid ISO-TP frame or not and handles it accordingly._ 
```
void isotp_on_can_message (
    IsoTpLink * link,
    const uint8_t * data,
    uint8_t len
) 
```





**Parameters:**


* `link` The 
```
IsoTpLink 
```
 instance used for transceiving data. 
* `data` The data received via CAN. 
* `len` The length of the data received. 




        

<hr>



### function isotp\_poll 

_Polling function; call this function periodically to handle timeouts, send consecutive frames, etc._ 
```
void isotp_poll (
    IsoTpLink * link
) 
```





**Parameters:**


* `link` The 
```
IsoTpLink 
```
 instance used. 




        

<hr>



### function isotp\_receive 

_Receives and parses the received data and copies the parsed data into the internal buffer._ 
```
int isotp_receive (
    IsoTpLink * link,
    uint8_t * payload,
    const uint32_t payload_size,
    uint32_t * out_size
) 
```





**Parameters:**


* `link` The [**IsoTpLink**](structIsoTpLink.md) instance used to transceive data. 
* `payload` A pointer to an area in memory where the raw data is copied from. 
* `payload_size` The size of the received (raw) CAN data. 
* `out_size` A reference to a variable which will contain the size of the actual (parsed) data.



**Returns:**

Possible return values:
* [**ISOTP\_RET\_OK**](isotp__defines_8h.md#define-isotp_ret_ok)
* ISOTP\_RET\_NO\_DATA 







        

<hr>



### function isotp\_send 

_Sends ISO-TP frames via CAN, using the ID set in the initialising function._ 
```
int isotp_send (
    IsoTpLink * link,
    const uint8_t payload,
    uint32_t size
) 
```



Single-frame messages will be sent immediately when calling this function. Multi-frame messages will be sent consecutively when calling isotp\_poll.




**Parameters:**


* `link` The 
```
IsoTpLink 
```
 instance used for transceiving data. 
* `payload` The payload to be sent. (Up to 4095 bytes). 
* `size` The size of the payload to be sent.



**Returns:**

Possible return values:
* 
```
ISOTP_RET_OVERFLOW 
```

* 
```
ISOTP_RET_INPROGRESS 
```

* 
```
ISOTP_RET_OK 
```

* The return value of the user shim function [**isotp\_user\_send\_can()**](group__isotp__user.md#function-isotp_user_send_can). 







        

<hr>



### function isotp\_send\_with\_id 

_See_ [_**isotp\_send**_](group__isotp.md#function-isotp_send) _, with the exception that this function is used only for functional addressing._
```
int isotp_send_with_id (
    IsoTpLink * link,
    uint32_t id,
    const uint8_t payload,
    uint32_t size
) 
```




<hr>



### function isotp\_set\_fc\_params 

_Sets FlowControl parameters used by the receiver._ 
```
void isotp_set_fc_params (
    IsoTpLink * link,
    uint8_t block_size,
    uint32_t st_min_us
) 
```





**Parameters:**


* `link` The 
```
IsoTpLink 
```
 instance used for transceiving data. 
* `block_size` Block size (BS) for FlowControl. Zero means unlimited. 
* `st_min_us` Minimum separation time (STmin) in microseconds. 




        

<hr>



### function isotp\_set\_timeouts 

_Sets timing parameters for waiting on FlowControl and ConsecutiveFrame._ 
```
void isotp_set_timeouts (
    IsoTpLink * link,
    uint32_t n_bs_us,
    uint32_t n_cr_us
) 
```





**Parameters:**


* `link` The 
```
IsoTpLink 
```
 instance used for transceiving data. 
* `n_bs_us` Timeout for N\_Bs in microseconds. 
* `n_cr_us` Timeout for N\_Cr in microseconds. 




        

<hr>

------------------------------


