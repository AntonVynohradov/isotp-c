

# File isotp\_test\_mocks.cpp



[**FileList**](files.md) **>** [**tests**](dir_59425e443f801f1f2fd8bbe4959a3ccf.md) **>** [**unit**](dir_7711e90abc3d4bd55823d9013a185d04.md) **>** [**isotp\_test\_mocks.cpp**](isotp__test__mocks_8cpp.md)

[Go to the source code of this file](isotp__test__mocks_8cpp_source.md)

_Test doubles for ISO-TP user callbacks._ [More...](#detailed-description)

* `#include "isotp_test_support.h"`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**MockCanState**](structMockCanState.md) | [**g\_can\_state**](#variable-g_can_state)   = `{0}`<br>_Global state for CAN send mock._  |
|  int | [**g\_debug\_call\_count**](#variable-g_debug_call_count)   = `0`<br>_Global state for counting debug calls._  |
|  uint32\_t | [**g\_now\_us**](#variable-g_now_us)   = `0`<br>_Global state for current time in microseconds._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**reset\_mocks**](#function-reset_mocks) () <br>_Reset all mock state to defaults._  |




























## Detailed Description


Provides mock time and CAN send implementations. 


    
## Public Attributes Documentation




### variable g\_can\_state 

_Global state for CAN send mock._ 
```C++
MockCanState g_can_state;
```




<hr>



### variable g\_debug\_call\_count 

_Global state for counting debug calls._ 
```C++
int g_debug_call_count;
```




<hr>



### variable g\_now\_us 

_Global state for current time in microseconds._ 
```C++
uint32_t g_now_us;
```




<hr>
## Public Functions Documentation




### function reset\_mocks 

_Reset all mock state to defaults._ 
```C++
void reset_mocks () 
```



Reset all mock state to initial values. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `tests/unit/isotp_test_mocks.cpp`

