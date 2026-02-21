

# File mock\_can.h



[**FileList**](files.md) **>** [**examples**](dir_d28a4824dc47e487b107a5db32ef43c4.md) **>** [**python\_api**](dir_181ea403426f4b233011860116dcb1b5.md) **>** [**inc**](dir_a048739b84e6d1db49a725d8f1a9c5e6.md) **>** [**mock\_can.h**](mock__can_8h.md)

[Go to the source code of this file](mock__can_8h_source.md)



* `#include <stdint.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**mock\_can\_disable\_fc**](#function-mock_can_disable_fc) (int enable) <br>_Disables sending FlowControl frames in the mock CAN driver._  |
|  void | [**mock\_can\_enable\_drop**](#function-mock_can_enable_drop) (int enable) <br>_Enables or disables frame dropping in the mock CAN driver._  |
|  void | [**mock\_can\_init**](#function-mock_can_init) (void) <br>_Initializes the mock CAN driver and sets it as the active driver._  |
|  void | [**mock\_can\_set\_delay\_ms**](#function-mock_can_set_delay_ms) (int delay) <br>_Sets the artificial delay for the mock CAN driver._  |




























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

------------------------------
The documentation for this class was generated from the following file `examples/python_api/inc/mock_can.h`

