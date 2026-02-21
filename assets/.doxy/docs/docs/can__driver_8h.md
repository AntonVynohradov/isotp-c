

# File can\_driver.h



[**FileList**](files.md) **>** [**examples**](dir_d28a4824dc47e487b107a5db32ef43c4.md) **>** [**python\_api**](dir_181ea403426f4b233011860116dcb1b5.md) **>** [**inc**](dir_a048739b84e6d1db49a725d8f1a9c5e6.md) **>** [**can\_driver.h**](can__driver_8h.md)

[Go to the source code of this file](can__driver_8h_source.md)



* `#include <stdint.h>`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**can\_driver\_t**](structcan__driver__t.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  int | [**can\_receive**](#function-can_receive) (uint32\_t \* id, uint8\_t \* data, uint8\_t \* len) <br>_Receives a CAN frame using the active CAN driver._  |
|  int | [**can\_send**](#function-can_send) (uint32\_t id, const uint8\_t \* data, uint8\_t len) <br>_Sends a CAN frame using the active CAN driver._  |
|  void | [**can\_set\_driver**](#function-can_set_driver) (const [**can\_driver\_t**](structcan__driver__t.md) \* driver) <br>_Sets the active CAN driver to be used by the ISO-TP implementation._  |




























## Public Functions Documentation




### function can\_receive 

_Receives a CAN frame using the active CAN driver._ 
```C++
int can_receive (
    uint32_t * id,
    uint8_t * data,
    uint8_t * len
) 
```





**Parameters:**


* `id` Pointer to store the received CAN ID. 
* `data` Pointer to buffer to store the received CAN data. 
* `len` Pointer to variable to store the length of the received CAN data. 



**Returns:**

int 0 on success, -1 if the queue is empty or if the driver is not set. 





        

<hr>



### function can\_send 

_Sends a CAN frame using the active CAN driver._ 
```C++
int can_send (
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

int 0 on success, -1 on failure (e.g., if the driver is not set or if send fails). 





        

<hr>



### function can\_set\_driver 

_Sets the active CAN driver to be used by the ISO-TP implementation._ 
```C++
void can_set_driver (
    const can_driver_t * driver
) 
```





**Parameters:**


* `driver` Pointer to the CAN driver to set as active. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `examples/python_api/inc/can_driver.h`

