

# File gtest\_isotp\_send.cpp



[**FileList**](files.md) **>** [**tests**](dir_59425e443f801f1f2fd8bbe4959a3ccf.md) **>** [**unit**](dir_7711e90abc3d4bd55823d9013a185d04.md) **>** [**gtest\_isotp\_send.cpp**](gtest__isotp__send_8cpp.md)

[Go to the source code of this file](gtest__isotp__send_8cpp_source.md)

_Unit tests for isotp\_send._ [More...](#detailed-description)

* `#include <gtest/gtest.h>`
* `#include "isotp_test_support.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TEST**](#function-test) (IsotpSend, UsesLinkArbitrationId) <br>_Uses link arbitration ID for sending._  |
|   | [**TEST**](#function-test) (IsotpSend, OversizeReturnsOverflow) <br>_Oversize payload returns overflow._  |
|   | [**TEST**](#function-test) (IsotpSend, InProgressReturnsInProgress) <br>_In-progress send returns in-progress._  |




























## Detailed Description


Verifies arbitration ID usage and error returns. 


    
## Public Functions Documentation




### function TEST 

_Uses link arbitration ID for sending._ 
```C++
TEST (
    IsotpSend,
    UsesLinkArbitrationId
) 
```




<hr>



### function TEST 

_Oversize payload returns overflow._ 
```C++
TEST (
    IsotpSend,
    OversizeReturnsOverflow
) 
```




<hr>



### function TEST 

_In-progress send returns in-progress._ 
```C++
TEST (
    IsotpSend,
    InProgressReturnsInProgress
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `tests/unit/gtest_isotp_send.cpp`

