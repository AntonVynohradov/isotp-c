

# File mock\_can.c



[**FileList**](files.md) **>** [**examples**](dir_d28a4824dc47e487b107a5db32ef43c4.md) **>** [**python\_api**](dir_181ea403426f4b233011860116dcb1b5.md) **>** [**src**](dir_63ad0292e4d57c8e6c47fd30f33819f0.md) **>** [**mock\_can.c**](mock__can_8c.md)

[Go to the source code of this file](mock__can_8c_source.md)



* `#include "mock_can.h"`
* `#include "can_driver.h"`
* `#include <string.h>`
* `#include <unistd.h>`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**can\_frame\_t**](structcan__frame__t.md) <br>_Structure representing a CAN frame in the mock driver._  |








## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  int | [**delay\_ms**](#variable-delay_ms)   = `0`<br>_Artificial delay in milliseconds for send operations (0 for no delay)_  |
|  int | [**disable\_fc**](#variable-disable_fc)   = `0`<br>_Flag to disable FlowControl frames in the mock driver._  |
|  int | [**drop\_enabled**](#variable-drop_enabled)   = `0`<br>_Flag to enable or disable frame dropping in the mock driver._  |
|  int | [**head**](#variable-head)   = `0`<br>_Index of the head of the queue (next frame to receive)_  |
|  [**can\_frame\_t**](structcan__frame__t.md) | [**queue**](#variable-queue)  <br>_Internal queue to hold CAN frames for simulation._  |
|  int | [**tail**](#variable-tail)   = `0`<br>_Index of the tail of the queue (next position to send a frame)_  |














## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**mock\_can\_disable\_fc**](#function-mock_can_disable_fc) (int enable) <br>_Disables sending FlowControl frames in the mock CAN driver._  |
|  void | [**mock\_can\_enable\_drop**](#function-mock_can_enable_drop) (int enable) <br>_Enables or disables frame dropping in the mock CAN driver._  |
|  void | [**mock\_can\_init**](#function-mock_can_init) (void) <br>_Initializes the mock CAN driver and sets it as the active driver._  |
|  void | [**mock\_can\_set\_delay\_ms**](#function-mock_can_set_delay_ms) (int delay) <br>_Sets the artificial delay for the mock CAN driver._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  int | [**mock\_receive**](#function-mock_receive) (uint32\_t \* id, uint8\_t \* data, uint8\_t \* len) <br>_Mock implementation of CAN receive function._  |
|  int | [**mock\_send**](#function-mock_send) (uint32\_t id, const uint8\_t \* data, uint8\_t len) <br>_Mock implementation of CAN send function._  |
|  void | [**sleep\_ms**](#function-sleep_ms) (int ms) <br>_Sleep for a specified number of milliseconds._  |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**CAN\_MAX\_DLEN**](mock__can_8c.md#define-can_max_dlen)  `8`<br>_Maximum data length for CAN frames._  |
| define  | [**MOCK\_QUEUE\_SIZE**](mock__can_8c.md#define-mock_queue_size)  `256`<br>_Size of the internal queue for simulating CAN frames._  |

## Public Static Attributes Documentation




### variable delay\_ms 

_Artificial delay in milliseconds for send operations (0 for no delay)_ 
```C++
int delay_ms;
```




<hr>



### variable disable\_fc 

_Flag to disable FlowControl frames in the mock driver._ 
```C++
int disable_fc;
```




<hr>



### variable drop\_enabled 

_Flag to enable or disable frame dropping in the mock driver._ 
```C++
int drop_enabled;
```




<hr>



### variable head 

_Index of the head of the queue (next frame to receive)_ 
```C++
int head;
```




<hr>



### variable queue 

_Internal queue to hold CAN frames for simulation._ 
```C++
can_frame_t queue[MOCK_QUEUE_SIZE];
```




<hr>



### variable tail 

_Index of the tail of the queue (next position to send a frame)_ 
```C++
int tail;
```




<hr>
## Public Functions Documentation




### function mock\_can\_disable\_fc 

_Disables sending FlowControl frames in the mock CAN driver._ 
```C++
void mock_can_disable_fc (
    int enable
) 
```





**Parameters:**


* `enable` - 1 to disable FlowControl frames, 0 to allow them. 




        

<hr>



### function mock\_can\_enable\_drop 

_Enables or disables frame dropping in the mock CAN driver._ 
```C++
void mock_can_enable_drop (
    int enable
) 
```





**Parameters:**


* `enable` - 1 to enable frame dropping, 0 to disable. 




        

<hr>



### function mock\_can\_init 

_Initializes the mock CAN driver and sets it as the active driver._ 
```C++
void mock_can_init (
    void
) 
```




<hr>



### function mock\_can\_set\_delay\_ms 

_Sets the artificial delay for the mock CAN driver._ 
```C++
void mock_can_set_delay_ms (
    int delay
) 
```





**Parameters:**


* `delay` - Delay in milliseconds. 




        

<hr>
## Public Static Functions Documentation




### function mock\_receive 

_Mock implementation of CAN receive function._ 
```C++
static int mock_receive (
    uint32_t * id,
    uint8_t * data,
    uint8_t * len
) 
```





**Parameters:**


* `id` Pointer to store the received CAN ID. 
* `data` Pointer to store the received CAN data. 
* `len` Pointer to store the length of the received CAN data. 



**Returns:**

int 0 on success, -1 if the queue is empty. 





        

<hr>



### function mock\_send 

_Mock implementation of CAN send function._ 
```C++
static int mock_send (
    uint32_t id,
    const uint8_t * data,
    uint8_t len
) 
```





**Parameters:**


* `id` CAN ID to send. 
* `data` Pointer to the CAN data to send. 
* `len` Length of the CAN data to send. 



**Returns:**

int 0 on success (including when drop is enabled and a frame is discarded), -1 if the queue is full. 





        

<hr>



### function sleep\_ms 

_Sleep for a specified number of milliseconds._ 
```C++
static void sleep_ms (
    int ms
) 
```





**Parameters:**


* `ms` Number of milliseconds to sleep. 




        

<hr>
## Macro Definition Documentation





### define CAN\_MAX\_DLEN 

_Maximum data length for CAN frames._ 
```C++
#define CAN_MAX_DLEN `8`
```




<hr>



### define MOCK\_QUEUE\_SIZE 

_Size of the internal queue for simulating CAN frames._ 
```C++
#define MOCK_QUEUE_SIZE `256`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `examples/python_api/src/mock_can.c`

