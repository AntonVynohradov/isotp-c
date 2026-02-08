

# File isotp.c



[**FileList**](files.md) **>** [**Src**](dir_413f4e031a85da0d68269c6fd2f76e1c.md) **>** [**isotp.c**](isotp_8c.md)

[Go to the source code of this file](isotp_8c_source.md)



* `#include <assert.h>`
* `#include <stdint.h>`
* `#include "isotp.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**isotp\_destroy\_link**](#function-isotp_destroy_link) (IsoTpLink \* link) <br> |
|  void | [**isotp\_init\_link**](#function-isotp_init_link) (IsoTpLink \* link, uint32\_t sendid, uint8\_t \* sendbuf, uint32\_t sendbufsize, uint8\_t \* recvbuf, uint32\_t recvbufsize) <br> |
|  void | [**isotp\_on\_can\_message**](#function-isotp_on_can_message) (IsoTpLink \* link, const uint8\_t \* data, uint8\_t len) <br> |
|  void | [**isotp\_poll**](#function-isotp_poll) (IsoTpLink \* link) <br> |
|  int | [**isotp\_receive**](#function-isotp_receive) (IsoTpLink \* link, uint8\_t \* payload, const uint32\_t payload\_size, uint32\_t \* out\_size) <br> |
|  int | [**isotp\_send**](#function-isotp_send) (IsoTpLink \* link, const uint8\_t payload, uint32\_t size) <br>_PUBLIC FUNCTIONS ///._  |
|  int | [**isotp\_send\_with\_id**](#function-isotp_send_with_id) (IsoTpLink \* link, uint32\_t id, const uint8\_t payload, uint32\_t size) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  int | [**isotp\_receive\_consecutive\_frame**](#function-isotp_receive_consecutive_frame) (IsoTpLink \* link, const [**IsoTpCanMessage**](structIsoTpCanMessage.md) \* message, uint8\_t len) <br>_Receive ISO-TP consecutive frame._  |
|  int | [**isotp\_receive\_first\_frame**](#function-isotp_receive_first_frame) (IsoTpLink \* link, [**IsoTpCanMessage**](structIsoTpCanMessage.md) \* message, uint8\_t len) <br>_Receive ISO-TP first frame of multi-frame message._  |
|  int | [**isotp\_receive\_flow\_control\_frame**](#function-isotp_receive_flow_control_frame) (IsoTpLink \* link, [**IsoTpCanMessage**](structIsoTpCanMessage.md) \* message, uint8\_t len) <br>_Receive ISO-TP flow control frame._  |
|  int | [**isotp\_receive\_single\_frame**](#function-isotp_receive_single_frame) (IsoTpLink \* link, const [**IsoTpCanMessage**](structIsoTpCanMessage.md) \* message, uint8\_t len) <br>_Receive ISO-TP single frame._  |
|  int | [**isotp\_send\_consecutive\_frame**](#function-isotp_send_consecutive_frame) (IsoTpLink \* link) <br>_Send ISO-TP consecutive frame._  |
|  int | [**isotp\_send\_first\_frame**](#function-isotp_send_first_frame) (IsoTpLink \* link, uint32\_t id) <br>_Send ISO-TP first frame of multi-frame message._  |
|  int | [**isotp\_send\_flow\_control**](#function-isotp_send_flow_control) (const IsoTpLink \* link, uint8\_t flow\_status, uint8\_t block\_size, uint32\_t st\_min\_us) <br>_Send ISO-TP flow control frame._  |
|  int | [**isotp\_send\_single\_frame**](#function-isotp_send_single_frame) (const IsoTpLink \* link, uint32\_t id) <br>_Send ISO-TP single frame._  |
|  uint32\_t | [**isotp\_st\_min\_to\_us**](#function-isotp_st_min_to_us) (uint8\_t st\_min) <br>_Convert ST\_min encoding format to microseconds._  |
|  uint8\_t | [**isotp\_us\_to\_st\_min**](#function-isotp_us_to_st_min) (uint32\_t us) <br>_Convert microseconds to ST\_min encoding format._  |


























## Public Functions Documentation




### function isotp\_destroy\_link 

```C++
void isotp_destroy_link (
    IsoTpLink * link
) 
```




<hr>



### function isotp\_init\_link 

```C++
void isotp_init_link (
    IsoTpLink * link,
    uint32_t sendid,
    uint8_t * sendbuf,
    uint32_t sendbufsize,
    uint8_t * recvbuf,
    uint32_t recvbufsize
) 
```




<hr>



### function isotp\_on\_can\_message 

```C++
void isotp_on_can_message (
    IsoTpLink * link,
    const uint8_t * data,
    uint8_t len
) 
```




<hr>



### function isotp\_poll 

```C++
void isotp_poll (
    IsoTpLink * link
) 
```




<hr>



### function isotp\_receive 

```C++
int isotp_receive (
    IsoTpLink * link,
    uint8_t * payload,
    const uint32_t payload_size,
    uint32_t * out_size
) 
```




<hr>



### function isotp\_send 

_PUBLIC FUNCTIONS ///._ 
```C++
int isotp_send (
    IsoTpLink * link,
    const uint8_t payload,
    uint32_t size
) 
```




<hr>



### function isotp\_send\_with\_id 

```C++
int isotp_send_with_id (
    IsoTpLink * link,
    uint32_t id,
    const uint8_t payload,
    uint32_t size
) 
```




<hr>
## Public Static Functions Documentation




### function isotp\_receive\_consecutive\_frame 

_Receive ISO-TP consecutive frame._ 
```C++
static int isotp_receive_consecutive_frame (
    IsoTpLink * link,
    const IsoTpCanMessage * message,
    uint8_t len
) 
```





**Parameters:**


* `link` - ISO-TP link instance 
* `message` - Received CAN message 
* `len` - Message length in bytes 



**Returns:**

ISOTP\_RET\_OK on success, error code on failure 





        

<hr>



### function isotp\_receive\_first\_frame 

_Receive ISO-TP first frame of multi-frame message._ 
```C++
static int isotp_receive_first_frame (
    IsoTpLink * link,
    IsoTpCanMessage * message,
    uint8_t len
) 
```





**Parameters:**


* `link` - ISO-TP link instance 
* `message` - Received CAN message 
* `len` - Message length in bytes 



**Returns:**

ISOTP\_RET\_OK on success, error code on failure 





        

<hr>



### function isotp\_receive\_flow\_control\_frame 

_Receive ISO-TP flow control frame._ 
```C++
static int isotp_receive_flow_control_frame (
    IsoTpLink * link,
    IsoTpCanMessage * message,
    uint8_t len
) 
```





**Parameters:**


* `link` - ISO-TP link instance 
* `message` - Received CAN message 
* `len` - Message length in bytes 



**Returns:**

ISOTP\_RET\_OK on success, error code on failure 





        

<hr>



### function isotp\_receive\_single\_frame 

_Receive ISO-TP single frame._ 
```C++
static int isotp_receive_single_frame (
    IsoTpLink * link,
    const IsoTpCanMessage * message,
    uint8_t len
) 
```





**Parameters:**


* `link` - ISO-TP link instance 
* `message` - Received CAN message 
* `len` - Message length in bytes 



**Returns:**

ISOTP\_RET\_OK on success, error code on failure 





        

<hr>



### function isotp\_send\_consecutive\_frame 

_Send ISO-TP consecutive frame._ 
```C++
static int isotp_send_consecutive_frame (
    IsoTpLink * link
) 
```





**Parameters:**


* `link` - ISO-TP link instance 



**Returns:**

ISOTP\_RET\_OK on success, error code on failure 





        

<hr>



### function isotp\_send\_first\_frame 

_Send ISO-TP first frame of multi-frame message._ 
```C++
static int isotp_send_first_frame (
    IsoTpLink * link,
    uint32_t id
) 
```





**Parameters:**


* `link` - ISO-TP link instance 
* `id` - CAN arbitration ID 



**Returns:**

ISOTP\_RET\_OK on success, error code on failure 





        

<hr>



### function isotp\_send\_flow\_control 

_Send ISO-TP flow control frame._ 
```C++
static int isotp_send_flow_control (
    const IsoTpLink * link,
    uint8_t flow_status,
    uint8_t block_size,
    uint32_t st_min_us
) 
```





**Parameters:**


* `link` - ISO-TP link instance 
* `flow_status` - Flow control status 
* `block_size` - Block size for flow control 
* `st_min_us` - Minimum separation time in microseconds 



**Returns:**

ISOTP\_RET\_OK on success, error code on failure 





        

<hr>



### function isotp\_send\_single\_frame 

_Send ISO-TP single frame._ 
```C++
static int isotp_send_single_frame (
    const IsoTpLink * link,
    uint32_t id
) 
```





**Parameters:**


* `link` - ISO-TP link instance 
* `id` - CAN arbitration ID 



**Returns:**

ISOTP\_RET\_OK on success, error code on failure 





        

<hr>



### function isotp\_st\_min\_to\_us 

_Convert ST\_min encoding format to microseconds._ 
```C++
static uint32_t isotp_st_min_to_us (
    uint8_t st_min
) 
```





**Parameters:**


* `st_min` - Encoded ST\_min value 



**Returns:**

Time in microseconds 





        

<hr>



### function isotp\_us\_to\_st\_min 

_Convert microseconds to ST\_min encoding format._ 
```C++
static uint8_t isotp_us_to_st_min (
    uint32_t us
) 
```





**Parameters:**


* `us` - Time in microseconds 



**Returns:**

Encoded ST\_min value 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `Src/isotp.c`

