

# File gtest\_isotp\_on\_can\_message.cpp



[**FileList**](files.md) **>** [**tests**](dir_59425e443f801f1f2fd8bbe4959a3ccf.md) **>** [**unit**](dir_7711e90abc3d4bd55823d9013a185d04.md) **>** [**gtest\_isotp\_on\_can\_message.cpp**](gtest__isotp__on__can__message_8cpp.md)

[Go to the source code of this file](gtest__isotp__on__can__message_8cpp_source.md)

_Unit tests for isotp\_on\_can\_message._ [More...](#detailed-description)

* `#include <gtest/gtest.h>`
* `#include "isotp_test_support.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, SingleFrameSetsReceiveBuffer) <br>_Single-frame payload fills the receive buffer._  |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, InvalidLengthIsIgnored) <br>_Invalid CAN DLC is ignored._  |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, SingleFrameInProgressLengthErrorSetsUnexpected) <br>_SF length error while in-progress yields unexpected PDU._  |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, FirstFrameOverflowSendsFlowControlOverflow) <br>_FF overflow triggers FC overflow response._  |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, FirstFrameOkStartsReceptionAndSendsFlowControl) <br>_FF starts reception and sends FC continue._  |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, ConsecutiveFrameUnexpectedWhenIdle) <br>_CF while idle is reported as unexpected PDU._  |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, ConsecutiveFrameWrongSnSetsError) <br>_Wrong SN in CF sets protocol error._  |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, ConsecutiveFrameCompletesReceive) <br>_Final CF completes the receive buffer._  |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, ConsecutiveFrameSendsFlowControlWhenBlockSizeExhausted) <br> |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, FlowControlIgnoredWhenSendIdle) <br> |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, FlowControlOverflowSetsSendError) <br> |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, FlowControlWaitExceedsSetsError) <br> |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, FlowControlContinueBsZeroSetsInvalidBsAndStmin) <br> |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, FlowControlContinueBsNonZeroSetsBs) <br> |
|   | [**TEST**](#function-test) (IsotpOnCanMessage, UnknownTypeIgnored) <br> |




























## Detailed Description


Covers SF/FF/CF parsing and error handling paths. 


    
## Public Functions Documentation




### function TEST 

_Single-frame payload fills the receive buffer._ 
```C++
TEST (
    IsotpOnCanMessage,
    SingleFrameSetsReceiveBuffer
) 
```




<hr>



### function TEST 

_Invalid CAN DLC is ignored._ 
```C++
TEST (
    IsotpOnCanMessage,
    InvalidLengthIsIgnored
) 
```




<hr>



### function TEST 

_SF length error while in-progress yields unexpected PDU._ 
```C++
TEST (
    IsotpOnCanMessage,
    SingleFrameInProgressLengthErrorSetsUnexpected
) 
```




<hr>



### function TEST 

_FF overflow triggers FC overflow response._ 
```C++
TEST (
    IsotpOnCanMessage,
    FirstFrameOverflowSendsFlowControlOverflow
) 
```




<hr>



### function TEST 

_FF starts reception and sends FC continue._ 
```C++
TEST (
    IsotpOnCanMessage,
    FirstFrameOkStartsReceptionAndSendsFlowControl
) 
```




<hr>



### function TEST 

_CF while idle is reported as unexpected PDU._ 
```C++
TEST (
    IsotpOnCanMessage,
    ConsecutiveFrameUnexpectedWhenIdle
) 
```




<hr>



### function TEST 

_Wrong SN in CF sets protocol error._ 
```C++
TEST (
    IsotpOnCanMessage,
    ConsecutiveFrameWrongSnSetsError
) 
```




<hr>



### function TEST 

_Final CF completes the receive buffer._ 
```C++
TEST (
    IsotpOnCanMessage,
    ConsecutiveFrameCompletesReceive
) 
```




<hr>



### function TEST 

```C++
TEST (
    IsotpOnCanMessage,
    ConsecutiveFrameSendsFlowControlWhenBlockSizeExhausted
) 
```




<hr>



### function TEST 

```C++
TEST (
    IsotpOnCanMessage,
    FlowControlIgnoredWhenSendIdle
) 
```




<hr>



### function TEST 

```C++
TEST (
    IsotpOnCanMessage,
    FlowControlOverflowSetsSendError
) 
```




<hr>



### function TEST 

```C++
TEST (
    IsotpOnCanMessage,
    FlowControlWaitExceedsSetsError
) 
```




<hr>



### function TEST 

```C++
TEST (
    IsotpOnCanMessage,
    FlowControlContinueBsZeroSetsInvalidBsAndStmin
) 
```




<hr>



### function TEST 

```C++
TEST (
    IsotpOnCanMessage,
    FlowControlContinueBsNonZeroSetsBs
) 
```




<hr>



### function TEST 

```C++
TEST (
    IsotpOnCanMessage,
    UnknownTypeIgnored
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `tests/unit/gtest_isotp_on_can_message.cpp`

