

# File isotp.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**isotp.c**](isotp_8c.md)

[Go to the source code of this file](isotp_8c_source.md)



* `#include <assert.h>`
* `#include <stdint.h>`
* `#include "isotp.h"`







































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  int | [**isotp\_receive\_consecutive\_frame**](#function-isotp_receive_consecutive_frame) ([**IsoTpLink**](structIsoTpLink.md) \* link, const [**IsoTpCanMessage**](structIsoTpCanMessage.md) \* message, uint8\_t len) <br>_Receive ISO-TP consecutive frame._  |
|  int | [**isotp\_receive\_first\_frame**](#function-isotp_receive_first_frame) ([**IsoTpLink**](structIsoTpLink.md) \* link, [**IsoTpCanMessage**](structIsoTpCanMessage.md) \* message, uint8\_t len) <br>_Receive ISO-TP first frame of multi-frame message._  |
|  int | [**isotp\_receive\_flow\_control\_frame**](#function-isotp_receive_flow_control_frame) ([**IsoTpLink**](structIsoTpLink.md) \* link, [**IsoTpCanMessage**](structIsoTpCanMessage.md) \* message, uint8\_t len) <br>_Receive ISO-TP flow control frame._  |
|  int | [**isotp\_receive\_single\_frame**](#function-isotp_receive_single_frame) ([**IsoTpLink**](structIsoTpLink.md) \* link, const [**IsoTpCanMessage**](structIsoTpCanMessage.md) \* message, uint8\_t len) <br>_Receive ISO-TP single frame._  |
|  int | [**isotp\_send\_consecutive\_frame**](#function-isotp_send_consecutive_frame) ([**IsoTpLink**](structIsoTpLink.md) \* link) <br>_Send ISO-TP consecutive frame._  |
|  int | [**isotp\_send\_first\_frame**](#function-isotp_send_first_frame) ([**IsoTpLink**](structIsoTpLink.md) \* link, uint32\_t id) <br>_Send ISO-TP first frame of multi-frame message._  |
|  int | [**isotp\_send\_flow\_control**](#function-isotp_send_flow_control) (const [**IsoTpLink**](structIsoTpLink.md) \* link, uint8\_t flow\_status, uint8\_t block\_size, uint32\_t st\_min\_us) <br>_Send ISO-TP flow control frame._  |
|  int | [**isotp\_send\_single\_frame**](#function-isotp_send_single_frame) (const [**IsoTpLink**](structIsoTpLink.md) \* link, uint32\_t id) <br>_Send ISO-TP single frame._  |
|  uint32\_t | [**isotp\_st\_min\_to\_us**](#function-isotp_st_min_to_us) (uint8\_t st\_min) <br>_Convert ST\_min encoding format to microseconds._  |
|  uint8\_t | [**isotp\_us\_to\_st\_min**](#function-isotp_us_to_st_min) (uint32\_t us) <br>_Convert microseconds to ST\_min encoding format._  |


























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
The documentation for this class was generated from the following file `src/isotp.c`

