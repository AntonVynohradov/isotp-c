

# File gtest\_isotp\_poll.cpp



[**FileList**](files.md) **>** [**tests**](dir_59425e443f801f1f2fd8bbe4959a3ccf.md) **>** [**unit**](dir_7711e90abc3d4bd55823d9013a185d04.md) **>** [**gtest\_isotp\_poll.cpp**](gtest__isotp__poll_8cpp.md)

[Go to the source code of this file](gtest__isotp__poll_8cpp_source.md)

_Unit tests for isotp\_poll._ [More...](#detailed-description)

* `#include <gtest/gtest.h>`
* `#include "isotp_test_support.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TEST**](#function-test) (IsotpPoll, SendsConsecutiveFrameAndCompletes) <br>_Sends the final CF and completes the send._  |
|   | [**TEST**](#function-test) (IsotpPoll, ReceiveTimeoutSetsIdle) <br>_Receive timeout sets idle and timeout result._  |
|   | [**TEST**](#function-test) (IsotpPoll, SendDoesNotStartWhenStMinNotElapsed) <br>_STmin delay prevents sending early._  |
|   | [**TEST**](#function-test) (IsotpPoll, SendNoSpaceDoesNotChangeState) <br>_NOSPACE keeps send state unchanged._  |
|   | [**TEST**](#function-test) (IsotpPoll, SendErrorSetsSendStatusError) <br>_Send error sets send status to error._  |
|   | [**TEST**](#function-test) (IsotpPoll, SendDecrementsBlockSizeRemain) <br>_Send decrements block size and advances timers._  |
|   | [**TEST**](#function-test) (IsotpPoll, SendTimeoutSetsError) <br>_Send timeout sets protocol error._  |
|   | [**TEST**](#function-test) (IsotpPoll, ReceiveDoesNotTimeoutWhenTimerZero) <br>_Zero receive timer does not trigger timeout._  |




























## Detailed Description


Validates send/receive state updates and timeout handling. 


    
## Public Functions Documentation




### function TEST 

_Sends the final CF and completes the send._ 
```C++
TEST (
    IsotpPoll,
    SendsConsecutiveFrameAndCompletes
) 
```




<hr>



### function TEST 

_Receive timeout sets idle and timeout result._ 
```C++
TEST (
    IsotpPoll,
    ReceiveTimeoutSetsIdle
) 
```




<hr>



### function TEST 

_STmin delay prevents sending early._ 
```C++
TEST (
    IsotpPoll,
    SendDoesNotStartWhenStMinNotElapsed
) 
```




<hr>



### function TEST 

_NOSPACE keeps send state unchanged._ 
```C++
TEST (
    IsotpPoll,
    SendNoSpaceDoesNotChangeState
) 
```




<hr>



### function TEST 

_Send error sets send status to error._ 
```C++
TEST (
    IsotpPoll,
    SendErrorSetsSendStatusError
) 
```




<hr>



### function TEST 

_Send decrements block size and advances timers._ 
```C++
TEST (
    IsotpPoll,
    SendDecrementsBlockSizeRemain
) 
```




<hr>



### function TEST 

_Send timeout sets protocol error._ 
```C++
TEST (
    IsotpPoll,
    SendTimeoutSetsError
) 
```




<hr>



### function TEST 

_Zero receive timer does not trigger timeout._ 
```C++
TEST (
    IsotpPoll,
    ReceiveDoesNotTimeoutWhenTimerZero
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `tests/unit/gtest_isotp_poll.cpp`

