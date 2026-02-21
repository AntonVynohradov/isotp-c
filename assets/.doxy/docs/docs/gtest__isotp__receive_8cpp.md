

# File gtest\_isotp\_receive.cpp



[**FileList**](files.md) **>** [**tests**](dir_59425e443f801f1f2fd8bbe4959a3ccf.md) **>** [**unit**](dir_7711e90abc3d4bd55823d9013a185d04.md) **>** [**gtest\_isotp\_receive.cpp**](gtest__isotp__receive_8cpp.md)

[Go to the source code of this file](gtest__isotp__receive_8cpp_source.md)

_Unit tests for isotp\_receive._ [More...](#detailed-description)

* `#include <gtest/gtest.h>`
* `#include "isotp_test_support.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TEST**](#function-test) (IsotpReceive, NoDataReturnsNoData) <br>_No data returns ISOTP\_RET\_NO\_DATA._  |
|   | [**TEST**](#function-test) (IsotpReceive, CopiesPayloadAndResetsState) <br>_Full payload copy resets receive state._  |
|   | [**TEST**](#function-test) (IsotpReceive, CopiesPartialPayloadWhenBufferTooSmall) <br>_Small output buffer triggers partial copy._  |




























## Detailed Description


Covers empty receive, full copy, and truncation paths. 


    
## Public Functions Documentation




### function TEST 

_No data returns ISOTP\_RET\_NO\_DATA._ 
```C++
TEST (
    IsotpReceive,
    NoDataReturnsNoData
) 
```




<hr>



### function TEST 

_Full payload copy resets receive state._ 
```C++
TEST (
    IsotpReceive,
    CopiesPayloadAndResetsState
) 
```




<hr>



### function TEST 

_Small output buffer triggers partial copy._ 
```C++
TEST (
    IsotpReceive,
    CopiesPartialPayloadWhenBufferTooSmall
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `tests/unit/gtest_isotp_receive.cpp`

