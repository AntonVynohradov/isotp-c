

# File isotp\_defines.h



[**FileList**](files.md) **>** [**inc**](dir_bfccd401955b95cf8c75461437045ac0.md) **>** [**isotp\_defines.h**](isotp__defines_8h.md)

[Go to the source code of this file](isotp__defines_8h_source.md)

_Internal protocol definitions and type structures for ISO-TP._ [More...](#detailed-description)

* `#include <stdint.h>`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**IsoTpCanMessage**](structIsoTpCanMessage.md) <br> |
| struct | [**IsoTpConsecutiveFrame**](structIsoTpConsecutiveFrame.md) <br>_Consecutive frame layout._  |
| struct | [**IsoTpDataArray**](structIsoTpDataArray.md) <br> |
| struct | [**IsoTpFirstFrameShort**](structIsoTpFirstFrameShort.md) <br>_First frame (short) layout._  |
| struct | [**IsoTpFlowControl**](structIsoTpFlowControl.md) <br>_Flow control frame layout._  |
| struct | [**IsoTpPciType**](structIsoTpPciType.md) <br>_CAN frame definitions._  |
| struct | [**IsoTpSingleFrame**](structIsoTpSingleFrame.md) <br>_Single frame layout._  |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**IsoTpFlowStatus**](#enum-isotpflowstatus)  <br>_Protocol Control Information (PCI) flow control identifiers._  |
| enum  | [**IsoTpProtocolControlInformation**](#enum-isotpprotocolcontrolinformation)  <br>_Protocol-specific defines._  |
| enum  | [**IsoTpReceiveStatusTypes**](#enum-isotpreceivestatustypes)  <br>_ISOTP receiver status._  |
| enum  | [**IsoTpSendStatusTypes**](#enum-isotpsendstatustypes)  <br>_ISOTP sender status._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ISOTP\_PACKED\_STRUCT**](#function-isotp_packed_struct) ({ uint8\_t set\_to\_zero\_high :4;uint8\_t type :4;uint8\_t set\_to\_zero\_low;uint32\_t FF\_DL;uint8\_t data[2];} IsoTpFirstFrameLong) <br>_First frame (long) layout._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ISOTP\_INVALID\_BS**](isotp__defines_8h.md#define-isotp_invalid_bs)  `0xFFFF`<br>_Invalid block size marker._  |
| define  | [**ISOTP\_MAX\_ERROR\_MSG\_SIZE**](isotp__defines_8h.md#define-isotp_max_error_msg_size)  `128`<br>_Max number of characters allowed in an error message._  |
| define  | [**ISOTP\_PROTOCOL\_RESULT\_BUFFER\_OVFLW**](isotp__defines_8h.md#define-isotp_protocol_result_buffer_ovflw)  `-8`<br> |
| define  | [**ISOTP\_PROTOCOL\_RESULT\_ERROR**](isotp__defines_8h.md#define-isotp_protocol_result_error)  `-9`<br> |
| define  | [**ISOTP\_PROTOCOL\_RESULT\_INVALID\_FS**](isotp__defines_8h.md#define-isotp_protocol_result_invalid_fs)  `-5`<br> |
| define  | [**ISOTP\_PROTOCOL\_RESULT\_OK**](isotp__defines_8h.md#define-isotp_protocol_result_ok)  `0`<br>_Network layer result codes._  |
| define  | [**ISOTP\_PROTOCOL\_RESULT\_TIMEOUT\_A**](isotp__defines_8h.md#define-isotp_protocol_result_timeout_a)  `-1`<br> |
| define  | [**ISOTP\_PROTOCOL\_RESULT\_TIMEOUT\_BS**](isotp__defines_8h.md#define-isotp_protocol_result_timeout_bs)  `-2`<br> |
| define  | [**ISOTP\_PROTOCOL\_RESULT\_TIMEOUT\_CR**](isotp__defines_8h.md#define-isotp_protocol_result_timeout_cr)  `-3`<br> |
| define  | [**ISOTP\_PROTOCOL\_RESULT\_UNEXP\_PDU**](isotp__defines_8h.md#define-isotp_protocol_result_unexp_pdu)  `-6`<br> |
| define  | [**ISOTP\_PROTOCOL\_RESULT\_WFT\_OVRN**](isotp__defines_8h.md#define-isotp_protocol_result_wft_ovrn)  `-7`<br> |
| define  | [**ISOTP\_PROTOCOL\_RESULT\_WRONG\_SN**](isotp__defines_8h.md#define-isotp_protocol_result_wrong_sn)  `-4`<br> |
| define  | [**ISOTP\_RET\_ERROR**](isotp__defines_8h.md#define-isotp_ret_error)  `-1`<br> |
| define  | [**ISOTP\_RET\_INPROGRESS**](isotp__defines_8h.md#define-isotp_ret_inprogress)  `-2`<br> |
| define  | [**ISOTP\_RET\_LENGTH**](isotp__defines_8h.md#define-isotp_ret_length)  `-7`<br> |
| define  | [**ISOTP\_RET\_NOSPACE**](isotp__defines_8h.md#define-isotp_ret_nospace)  `-8`<br> |
| define  | [**ISOTP\_RET\_NO\_DATA**](isotp__defines_8h.md#define-isotp_ret_no_data)  `-5`<br> |
| define  | [**ISOTP\_RET\_OK**](isotp__defines_8h.md#define-isotp_ret_ok)  `0`<br>_Internal return codes._  |
| define  | [**ISOTP\_RET\_OVERFLOW**](isotp__defines_8h.md#define-isotp_ret_overflow)  `-3`<br> |
| define  | [**ISOTP\_RET\_TIMEOUT**](isotp__defines_8h.md#define-isotp_ret_timeout)  `-6`<br> |
| define  | [**ISOTP\_RET\_WRONG\_SN**](isotp__defines_8h.md#define-isotp_ret_wrong_sn)  `-4`<br> |
| define  | [**IsoTpTimeAfter**](isotp__defines_8h.md#define-isotptimeafter) (a, b) `((int32\_t) ((int32\_t) (b) - (int32\_t) (a)) &lt; 0)`<br>_Returns true if time_ `a` _is after time_`b` _._ |
| define  | [**LE32TOH**](isotp__defines_8h.md#define-le32toh) (le) `/* multi line expression */`<br>_Compiler-specific defines._  |

## Detailed Description


Project: ISO-TP-C - Embedded-Grade Refactoring & Optimization Author: Anton Vynohradov Email: [avynohradov@systemfromscratch.com](mailto:avynohradov@systemfromscratch.com)




**Copyright:**

Copyright (c) 2026 Anton Vynohradov @license MIT License


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


SPDX-License-Identifier: MIT 


    
## Public Types Documentation




### enum IsoTpFlowStatus 

_Protocol Control Information (PCI) flow control identifiers._ 
```C++
enum IsoTpFlowStatus {
    PCI_FLOW_STATUS_CONTINUE = 0x0,
    PCI_FLOW_STATUS_WAIT = 0x1,
    PCI_FLOW_STATUS_OVERFLOW = 0x2
};
```




<hr>



### enum IsoTpProtocolControlInformation 

_Protocol-specific defines._ 
```C++
enum IsoTpProtocolControlInformation {
    ISOTP_PCI_TYPE_SINGLE = 0x0,
    ISOTP_PCI_TYPE_FIRST_FRAME = 0x1,
    TSOTP_PCI_TYPE_CONSECUTIVE_FRAME = 0x2,
    ISOTP_PCI_TYPE_FLOW_CONTROL_FRAME = 0x3,
    ISOTP_PCI_TYPE_CONSECUTIVE_FRAME = 0x2
};
```



Protocol Control Information (PCI) types for identifying ISO-TP frames. 


        

<hr>



### enum IsoTpReceiveStatusTypes 

_ISOTP receiver status._ 
```C++
enum IsoTpReceiveStatusTypes {
    ISOTP_RECEIVE_STATUS_IDLE,
    ISOTP_RECEIVE_STATUS_INPROGRESS,
    ISOTP_RECEIVE_STATUS_FULL
};
```




<hr>



### enum IsoTpSendStatusTypes 

_ISOTP sender status._ 
```C++
enum IsoTpSendStatusTypes {
    ISOTP_SEND_STATUS_IDLE,
    ISOTP_SEND_STATUS_INPROGRESS,
    ISOTP_SEND_STATUS_ERROR
};
```




<hr>
## Public Functions Documentation




### function ISOTP\_PACKED\_STRUCT 

_First frame (long) layout._ 
```C++
ISOTP_PACKED_STRUCT (
    { uint8_t set_to_zero_high :4;uint8_t type :4;uint8_t set_to_zero_low;uint32_t FF_DL;uint8_t data[2];} IsoTpFirstFrameLong
) 
```




```C++
+-------------------------+-----------------------+---------+---------+---------+---------+
| byte #0                 | byte #1               | byte #2 | byte #3 | byte #4 | byte #5 |
+-------------------------+-----------+-----------+---------+---------+---------+---------+
| nibble #0   | nibble #1 | nibble #2 | nibble #3 | ...                                   |
+-------------+-----------+-----------+-----------+---------------------------------------+
| PCIType = 1 | unused=0  | escape sequence = 0   | FF_DL                                 |
+-------------+-----------+-----------------------+---------------------------------------+
```
 


        

<hr>
## Macro Definition Documentation





### define ISOTP\_INVALID\_BS 

_Invalid block size marker._ 
```C++
#define ISOTP_INVALID_BS `0xFFFF`
```




<hr>



### define ISOTP\_MAX\_ERROR\_MSG\_SIZE 

_Max number of characters allowed in an error message._ 
```C++
#define ISOTP_MAX_ERROR_MSG_SIZE `128`
```




<hr>



### define ISOTP\_PROTOCOL\_RESULT\_BUFFER\_OVFLW 

```C++
#define ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW `-8`
```




<hr>



### define ISOTP\_PROTOCOL\_RESULT\_ERROR 

```C++
#define ISOTP_PROTOCOL_RESULT_ERROR `-9`
```




<hr>



### define ISOTP\_PROTOCOL\_RESULT\_INVALID\_FS 

```C++
#define ISOTP_PROTOCOL_RESULT_INVALID_FS `-5`
```




<hr>



### define ISOTP\_PROTOCOL\_RESULT\_OK 

_Network layer result codes._ 
```C++
#define ISOTP_PROTOCOL_RESULT_OK `0`
```




<hr>



### define ISOTP\_PROTOCOL\_RESULT\_TIMEOUT\_A 

```C++
#define ISOTP_PROTOCOL_RESULT_TIMEOUT_A `-1`
```




<hr>



### define ISOTP\_PROTOCOL\_RESULT\_TIMEOUT\_BS 

```C++
#define ISOTP_PROTOCOL_RESULT_TIMEOUT_BS `-2`
```




<hr>



### define ISOTP\_PROTOCOL\_RESULT\_TIMEOUT\_CR 

```C++
#define ISOTP_PROTOCOL_RESULT_TIMEOUT_CR `-3`
```




<hr>



### define ISOTP\_PROTOCOL\_RESULT\_UNEXP\_PDU 

```C++
#define ISOTP_PROTOCOL_RESULT_UNEXP_PDU `-6`
```




<hr>



### define ISOTP\_PROTOCOL\_RESULT\_WFT\_OVRN 

```C++
#define ISOTP_PROTOCOL_RESULT_WFT_OVRN `-7`
```




<hr>



### define ISOTP\_PROTOCOL\_RESULT\_WRONG\_SN 

```C++
#define ISOTP_PROTOCOL_RESULT_WRONG_SN `-4`
```




<hr>



### define ISOTP\_RET\_ERROR 

```C++
#define ISOTP_RET_ERROR `-1`
```




<hr>



### define ISOTP\_RET\_INPROGRESS 

```C++
#define ISOTP_RET_INPROGRESS `-2`
```




<hr>



### define ISOTP\_RET\_LENGTH 

```C++
#define ISOTP_RET_LENGTH `-7`
```




<hr>



### define ISOTP\_RET\_NOSPACE 

```C++
#define ISOTP_RET_NOSPACE `-8`
```




<hr>



### define ISOTP\_RET\_NO\_DATA 

```C++
#define ISOTP_RET_NO_DATA `-5`
```




<hr>



### define ISOTP\_RET\_OK 

_Internal return codes._ 
```C++
#define ISOTP_RET_OK `0`
```




<hr>



### define ISOTP\_RET\_OVERFLOW 

```C++
#define ISOTP_RET_OVERFLOW `-3`
```




<hr>



### define ISOTP\_RET\_TIMEOUT 

```C++
#define ISOTP_RET_TIMEOUT `-6`
```




<hr>



### define ISOTP\_RET\_WRONG\_SN 

```C++
#define ISOTP_RET_WRONG_SN `-4`
```




<hr>



### define IsoTpTimeAfter 

_Returns true if time_ `a` _is after time_`b` _._
```C++
#define IsoTpTimeAfter (
    a,
    b
) `((int32_t) ((int32_t) (b) - (int32_t) (a)) < 0)`
```




<hr>



### define LE32TOH 

_Compiler-specific defines._ 
```C++
#define LE32TOH (
    le
) `/* multi line expression */`
```



OS-specific defines.


Convert a 32-bit value from little-endian to host byte order. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `inc/isotp_defines.h`

