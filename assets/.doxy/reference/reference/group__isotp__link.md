

# Group isotp\_link



[**Modules**](modules.md) **>** [**isotp\_link**](group__isotp__link.md)



_Transport layer of one ISO-TP connection: segmentation, flow control, timing._ [More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**isotp\_link**](structisotp__link.md) <br>_Link state._  |
| struct | [**isotp\_link\_config**](structisotp__link__config.md) <br>_Link configuration passed to_ [_**isotp\_link\_init**_](group__isotp__link.md#function-isotp_link_init) _._ |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef int(\* | [**isotp\_can\_write\_fn**](#typedef-isotp_can_write_fn)  <br>_Write one CAN frame to the bus._  |
| typedef void(\* | [**isotp\_message\_handler\_t**](#typedef-isotp_message_handler_t)  <br>_Handle one complete received message._  |
| typedef void(\* | [**isotp\_result\_fn**](#typedef-isotp_result_fn)  <br>_Report the outcome of a transmission or a failed reception._  |
| enum  | [**isotp\_rx\_state**](#enum-isotp_rx_state)  <br>_Receiver state._  |
| enum  | [**isotp\_tx\_state**](#enum-isotp_tx_state)  <br>_Transmitter state._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**isotp\_link\_config\_default**](#function-isotp_link_config_default) (struct [**isotp\_link\_config**](structisotp__link__config.md) \* config) <br>_Fill a configuration with the defaults of_ [_**isotp\_config.h**_](isotp__config_8h.md) _._ |
|  int | [**isotp\_link\_init**](#function-isotp_link_init) (struct [**isotp\_link**](structisotp__link.md) \* link, const struct [**isotp\_link\_config**](structisotp__link__config.md) \* config) <br>_Initialize a link: copy the configuration, clear both directions._  |
|  uint32\_t | [**isotp\_link\_invalid\_frames**](#function-isotp_link_invalid_frames) (const struct [**isotp\_link**](structisotp__link.md) \* link) <br>_Malformed frames ignored since_ [_**isotp\_link\_init**_](group__isotp__link.md#function-isotp_link_init) _(0 for NULL)._ |
|  void | [**isotp\_link\_poll**](#function-isotp_link_poll) (struct [**isotp\_link**](structisotp__link.md) \* link, uint32\_t now\_us) <br>_Advance the timers: send the next Consecutive Frame, detect timeouts._  |
|  void | [**isotp\_link\_receive**](#function-isotp_link_receive) (struct [**isotp\_link**](structisotp__link.md) \* link, const uint8\_t \* data, size\_t length, uint32\_t now\_us) <br>_Process one received CAN frame addressed to this link._  |
|  int | [**isotp\_link\_rx\_result**](#function-isotp_link_rx_result) (const struct [**isotp\_link**](structisotp__link.md) \* link) <br>_N\_Result of the last finished or failed reception (ISOTP\_N\_ERROR for NULL)._  |
|  int | [**isotp\_link\_send**](#function-isotp_link_send) (struct [**isotp\_link**](structisotp__link.md) \* link, const uint8\_t \* payload, uint32\_t length, uint32\_t now\_us) <br>_Start sending one message to the peer._  |
|  int | [**isotp\_link\_send\_functional**](#function-isotp_link_send_functional) (const struct [**isotp\_link**](structisotp__link.md) \* link, uint32\_t can\_id, const uint8\_t \* payload, uint32\_t length) <br>_Send a Single Frame with a functional (broadcast) CAN ID._  |
|  bool | [**isotp\_link\_tx\_busy**](#function-isotp_link_tx_busy) (const struct [**isotp\_link**](structisotp__link.md) \* link) <br>_True while a segmented transmission is in progress (false for NULL)._  |
|  int | [**isotp\_link\_tx\_result**](#function-isotp_link_tx_result) (const struct [**isotp\_link**](structisotp__link.md) \* link) <br>_N\_Result of the last finished transmission (ISOTP\_N\_ERROR for NULL)._  |




























## Detailed Description


A link connects this node to one peer: it sends with the CAN ID [**isotp\_link\_config::tx\_id**](structisotp__link__config.md#variable-tx_id) and receives the frames the application routes to it (normally the frames with the peer's CAN ID). The link never reads a clock or touches hardware. The integrator passes every received CAN frame to [**isotp\_link\_receive**](group__isotp__link.md#function-isotp_link_receive), provides the frame writer [**isotp\_can\_write\_fn**](group__isotp__link.md#typedef-isotp_can_write_fn), and calls [**isotp\_link\_poll**](group__isotp__link.md#function-isotp_link_poll) regularly; the current time in microseconds is an argument of every call.


Transmission and reception are independent (full duplex). A complete received message is delivered to [**isotp\_link\_config::on\_message**](structisotp__link__config.md#variable-on_message); the end of a transmission is reported to [**isotp\_link\_config::on\_tx\_done**](structisotp__link__config.md#variable-on_tx_done).


A link is not thread-safe. Call [**isotp\_link\_receive**](group__isotp__link.md#function-isotp_link_receive) and [**isotp\_link\_poll**](group__isotp__link.md#function-isotp_link_poll) from one context (for example a CAN task, not an interrupt handler) and serialize [**isotp\_link\_send**](group__isotp__link.md#function-isotp_link_send) with them.




**
**


* Lifecycle: [**isotp\_link\_config\_default**](group__isotp__link.md#function-isotp_link_config_default), [**isotp\_link\_init**](group__isotp__link.md#function-isotp_link_init). 
* Message flow: [**isotp\_link\_receive**](group__isotp__link.md#function-isotp_link_receive), [**isotp\_link\_send**](group__isotp__link.md#function-isotp_link_send), [**isotp\_link\_send\_functional**](group__isotp__link.md#function-isotp_link_send_functional), [**isotp\_link\_poll**](group__isotp__link.md#function-isotp_link_poll). 
* Diagnostics: [**isotp\_link\_tx\_busy**](group__isotp__link.md#function-isotp_link_tx_busy), [**isotp\_link\_tx\_result**](group__isotp__link.md#function-isotp_link_tx_result), [**isotp\_link\_rx\_result**](group__isotp__link.md#function-isotp_link_rx_result), [**isotp\_link\_invalid\_frames**](group__isotp__link.md#function-isotp_link_invalid_frames). 







    
## Public Types Documentation




### typedef isotp\_can\_write\_fn 

_Write one CAN frame to the bus._ 
```
typedef int(* isotp_can_write_fn) (void *context, uint32_t can_id, const uint8_t *data, size_t length);
```





**Parameters:**


* `context` Value of [**isotp\_link\_config::write\_context**](structisotp__link__config.md#variable-write_context). 
* `can_id` CAN identifier of the frame. 
* `data` Frame data; valid only during the call. 
* `length` Number of data bytes, 1..8. 



**Return value:**


* `0` Frame queued for transmission. 
* `ISOTP_BUSY` No space in the transmit queue; the link retries a Consecutive Frame on the next [**isotp\_link\_poll**](group__isotp__link.md#function-isotp_link_poll). 
* `Other` Driver error: the transmission ends with ISOTP\_N\_ERROR. 




        

<hr>



### typedef isotp\_message\_handler\_t 

_Handle one complete received message._ 
```
typedef void(* isotp_message_handler_t) (const uint8_t *data, uint32_t length, void *context);
```



The data is valid only for the duration of the callback; copy what must be kept. The handler may call [**isotp\_link\_send**](group__isotp__link.md#function-isotp_link_send) on the same link to answer. 

**Parameters:**


* `data` Message bytes. 
* `length` Message length in bytes. 
* `context` Value of [**isotp\_link\_config::app\_context**](structisotp__link__config.md#variable-app_context). 




        

<hr>



### typedef isotp\_result\_fn 

_Report the outcome of a transmission or a failed reception._ 
```
typedef void(* isotp_result_fn) (int n_result, void *context);
```





**Parameters:**


* `n_result` ISOTP\_N\_\* code. 
* `context` Value of [**isotp\_link\_config::app\_context**](structisotp__link__config.md#variable-app_context). 




        

<hr>



### enum isotp\_rx\_state 

_Receiver state._ 
```
enum isotp_rx_state {
    ISOTP_RX_IDLE = 0,
    ISOTP_RX_RECEIVING
};
```




<hr>



### enum isotp\_tx\_state 

_Transmitter state._ 
```
enum isotp_tx_state {
    ISOTP_TX_IDLE = 0,
    ISOTP_TX_WAIT_FC,
    ISOTP_TX_SEND_CF
};
```




<hr>
## Public Functions Documentation




### function isotp\_link\_config\_default 

_Fill a configuration with the defaults of_ [_**isotp\_config.h**_](isotp__config_8h.md) _._
```
void isotp_link_config_default (
    struct isotp_link_config * config
) 
```



CAN ID 0, no writer, no callbacks, no padding (padding value 0xCC), ISOTP\_DEFAULT\_\* flow control and timeouts. 

**Parameters:**


* `config` Configuration to fill; nothing happens when NULL. 




        

<hr>



### function isotp\_link\_init 

_Initialize a link: copy the configuration, clear both directions._ 
```
int isotp_link_init (
    struct isotp_link * link,
    const struct isotp_link_config * config
) 
```





**Parameters:**


* `link` Link state to initialize. 
* `config` Configuration copied into the link. 



**Return value:**


* `ISOTP_OK` Link ready. 
* `ISOTP_ERROR` NULL argument, no writer, or a value out of range. 




        

<hr>



### function isotp\_link\_invalid\_frames 

_Malformed frames ignored since_ [_**isotp\_link\_init**_](group__isotp__link.md#function-isotp_link_init) _(0 for NULL)._
```
uint32_t isotp_link_invalid_frames (
    const struct isotp_link * link
) 
```




<hr>



### function isotp\_link\_poll 

_Advance the timers: send the next Consecutive Frame, detect timeouts._ 
```
void isotp_link_poll (
    struct isotp_link * link,
    uint32_t now_us
) 
```



Sends at most one Consecutive Frame per call, when the separation time has passed. Call it at least as often as the shortest STmin you want to achieve (for example from a 1 ms tick or a loop). 

**Parameters:**


* `link` Link state. 
* `now_us` Current time in microseconds. 




        

<hr>



### function isotp\_link\_receive 

_Process one received CAN frame addressed to this link._ 
```
void isotp_link_receive (
    struct isotp_link * link,
    const uint8_t * data,
    size_t length,
    uint32_t now_us
) 
```



Single Frames and the last Consecutive Frame complete a message and call the message handler. A First Frame is answered with a Flow Control frame. Malformed frames are ignored and counted; unexpected Consecutive and Flow Control frames are ignored. 

**Parameters:**


* `link` Link state. 
* `data` CAN frame data. 
* `length` CAN data length, 1..8. 
* `now_us` Current time in microseconds (free-running, wraps at 2^32). 




        

<hr>



### function isotp\_link\_rx\_result 

_N\_Result of the last finished or failed reception (ISOTP\_N\_ERROR for NULL)._ 
```
int isotp_link_rx_result (
    const struct isotp_link * link
) 
```




<hr>



### function isotp\_link\_send 

_Start sending one message to the peer._ 
```
int isotp_link_send (
    struct isotp_link * link,
    const uint8_t * payload,
    uint32_t length,
    uint32_t now_us
) 
```



A message of up to 7 bytes is sent at once as a Single Frame and reported to on\_tx\_done before the call returns. A longer message is copied into the link, its First Frame is sent, and the rest follows from [**isotp\_link\_poll**](group__isotp__link.md#function-isotp_link_poll) as the receiver's flow control allows. 

**Parameters:**


* `link` Link state. 
* `payload` Message bytes. 
* `length` Message length, 1..ISOTP\_TX\_BUFFER\_SIZE. 
* `now_us` Current time in microseconds. 



**Return value:**


* `ISOTP_OK` Message accepted. 
* `ISOTP_BUSY` A segmented transmission is in progress, or the writer is busy. 
* `ISOTP_ERROR` Invalid argument or message too long. 
* `Other` Error returned by the writer. 




        

<hr>



### function isotp\_link\_send\_functional 

_Send a Single Frame with a functional (broadcast) CAN ID._ 
```
int isotp_link_send_functional (
    const struct isotp_link * link,
    uint32_t can_id,
    const uint8_t * payload,
    uint32_t length
) 
```



ISO 15765-2 allows only unsegmented messages with functional addressing. The frame is independent of the link's transmission state and does not call on\_tx\_done. 

**Parameters:**


* `link` Link state. 
* `can_id` Functional CAN ID. 
* `payload` Message bytes. 
* `length` Message length, 1..7. 



**Return value:**


* `ISOTP_OK` Frame written. 
* `ISOTP_ERROR` Invalid argument or message longer than 7 bytes. 
* `Other` Error returned by the writer. 




        

<hr>



### function isotp\_link\_tx\_busy 

_True while a segmented transmission is in progress (false for NULL)._ 
```
bool isotp_link_tx_busy (
    const struct isotp_link * link
) 
```




<hr>



### function isotp\_link\_tx\_result 

_N\_Result of the last finished transmission (ISOTP\_N\_ERROR for NULL)._ 
```
int isotp_link_tx_result (
    const struct isotp_link * link
) 
```




<hr>

------------------------------


