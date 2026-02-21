

# File gtest\_isotp\_send\_with\_id.cpp



[**FileList**](files.md) **>** [**tests**](dir_59425e443f801f1f2fd8bbe4959a3ccf.md) **>** [**unit**](dir_7711e90abc3d4bd55823d9013a185d04.md) **>** [**gtest\_isotp\_send\_with\_id.cpp**](gtest__isotp__send__with__id_8cpp.md)

[Go to the source code of this file](gtest__isotp__send__with__id_8cpp_source.md)

_Unit tests for isotp\_send\_with\_id._ [More...](#detailed-description)

* `#include <gtest/gtest.h>`
* `#include "isotp_test_support.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TEST**](#function-test) (IsotpSendWithId, NullLinkReturnsError) <br>_Null link returns error._  |
|   | [**TEST**](#function-test) (IsotpSendWithId, OversizeReturnsOverflow) <br>_Oversize payload returns overflow without sending._  |
|   | [**TEST**](#function-test) (IsotpSendWithId, InProgressReturnsInProgress) <br>_In-progress send returns in-progress._  |
|   | [**TEST**](#function-test) (IsotpSendWithId, SingleFrameSendsPayload) <br>_Single-frame payload is sent with provided ID._  |
|   | [**TEST**](#function-test) (IsotpSendWithId, MultiFrameFirstFrameStartsSend) <br>_Multi-frame FF starts send and sets timers._  |
|   | [**TEST**](#function-test) (IsotpSendWithId, MultiFrameFirstFrameSendErrorDoesNotStart) <br>_FF send error does not start send state._  |




























## Detailed Description


Covers error handling, overflow, and multi-frame start logic. 


    
## Public Functions Documentation




### function TEST 

_Null link returns error._ 
```C++
TEST (
    IsotpSendWithId,
    NullLinkReturnsError
) 
```




<hr>



### function TEST 

_Oversize payload returns overflow without sending._ 
```C++
TEST (
    IsotpSendWithId,
    OversizeReturnsOverflow
) 
```




<hr>



### function TEST 

_In-progress send returns in-progress._ 
```C++
TEST (
    IsotpSendWithId,
    InProgressReturnsInProgress
) 
```




<hr>



### function TEST 

_Single-frame payload is sent with provided ID._ 
```C++
TEST (
    IsotpSendWithId,
    SingleFrameSendsPayload
) 
```




<hr>



### function TEST 

_Multi-frame FF starts send and sets timers._ 
```C++
TEST (
    IsotpSendWithId,
    MultiFrameFirstFrameStartsSend
) 
```




<hr>



### function TEST 

_FF send error does not start send state._ 
```C++
TEST (
    IsotpSendWithId,
    MultiFrameFirstFrameSendErrorDoesNotStart
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `tests/unit/gtest_isotp_send_with_id.cpp`

