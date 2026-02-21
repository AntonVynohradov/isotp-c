

# File mock\_time.h



[**FileList**](files.md) **>** [**examples**](dir_d28a4824dc47e487b107a5db32ef43c4.md) **>** [**python\_api**](dir_181ea403426f4b233011860116dcb1b5.md) **>** [**inc**](dir_a048739b84e6d1db49a725d8f1a9c5e6.md) **>** [**mock\_time.h**](mock__time_8h.md)

[Go to the source code of this file](mock__time_8h_source.md)



* `#include <stdint.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**mock\_time\_advance**](#function-mock_time_advance) (uint32\_t delta) <br>_Advances the virtual time by the specified number of microseconds._  |
|  uint32\_t | [**mock\_time\_now**](#function-mock_time_now) (void) <br>_Returns the current virtual time in microseconds._  |
|  void | [**mock\_time\_reset**](#function-mock_time_reset) (void) <br>_Resets the virtual time to zero._  |
|  void | [**mock\_time\_set**](#function-mock_time_set) (uint32\_t value) <br>_Sets the virtual time to a specific value._  |




























## Public Functions Documentation




### function mock\_time\_advance 

_Advances the virtual time by the specified number of microseconds._ 
```C++
void mock_time_advance (
    uint32_t delta
) 
```





**Parameters:**


* `delta` - Number of microseconds to advance the virtual time. 




        

<hr>



### function mock\_time\_now 

_Returns the current virtual time in microseconds._ 
```C++
uint32_t mock_time_now (
    void
) 
```





**Returns:**

uint32\_t Current virtual time in microseconds. 





        

<hr>



### function mock\_time\_reset 

_Resets the virtual time to zero._ 
```C++
void mock_time_reset (
    void
) 
```




<hr>



### function mock\_time\_set 

_Sets the virtual time to a specific value._ 
```C++
void mock_time_set (
    uint32_t value
) 
```





**Parameters:**


* `value` - The value to set the virtual time to, in microseconds. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `examples/python_api/inc/mock_time.h`

