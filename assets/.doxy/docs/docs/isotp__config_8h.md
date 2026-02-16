

# File isotp\_config.h



[**FileList**](files.md) **>** [**inc**](dir_bfccd401955b95cf8c75461437045ac0.md) **>** [**isotp\_config.h**](isotp__config_8h.md)

[Go to the source code of this file](isotp__config_8h_source.md)

_ISO-TP configuration parameters and feature toggles._ [More...](#detailed-description)

































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ISO\_TP\_DEFAULT\_BLOCK\_SIZE**](isotp__config_8h.md#define-iso_tp_default_block_size)  `(8)`<br>_Max number of messages the receiver can receive at one time._  |
| define  | [**ISO\_TP\_DEFAULT\_RESPONSE\_TIMEOUT\_US**](isotp__config_8h.md#define-iso_tp_default_response_timeout_us)  `(100000)`<br>_Default timeout while waiting for a response during a multi-frame transfer._  |
| define  | [**ISO\_TP\_DEFAULT\_ST\_MIN\_US**](isotp__config_8h.md#define-iso_tp_default_st_min_us)  `(0)`<br>_Minimum time gap allowed between consecutive frame N\_PDUs._  |
| define  | [**ISO\_TP\_FRAME\_PADDING\_VALUE**](isotp__config_8h.md#define-iso_tp_frame_padding_value)  `(0xAA)`<br>_Padding byte value when ISO\_TP\_FRAME\_PADDING is enabled._  |
| define  | [**ISO\_TP\_MAX\_WFT\_NUMBER**](isotp__config_8h.md#define-iso_tp_max_wft_number)  `(1)`<br>_Number of FC N\_PDU WTs that can be transmitted by the receiver in a row._  |

## Detailed Description


Project: ISO-TP-C - Embedded-Grade Refactoring & Optimization Author: Anton Vynohradov Email: [avynohradov@systemfromscratch.com](mailto:avynohradov@systemfromscratch.com)




**Copyright:**

Copyright (c) 2026 Anton Vynohradov @license MIT License


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


SPDX-License-Identifier: MIT 


    
## Macro Definition Documentation





### define ISO\_TP\_DEFAULT\_BLOCK\_SIZE 

_Max number of messages the receiver can receive at one time._ 
```C++
#define ISO_TP_DEFAULT_BLOCK_SIZE `(8)`
```



This value is affected by CAN driver queue length. 


        

<hr>



### define ISO\_TP\_DEFAULT\_RESPONSE\_TIMEOUT\_US 

_Default timeout while waiting for a response during a multi-frame transfer._ 
```C++
#define ISO_TP_DEFAULT_RESPONSE_TIMEOUT_US `(100000)`
```




<hr>



### define ISO\_TP\_DEFAULT\_ST\_MIN\_US 

_Minimum time gap allowed between consecutive frame N\_PDUs._ 
```C++
#define ISO_TP_DEFAULT_ST_MIN_US `(0)`
```




<hr>



### define ISO\_TP\_FRAME\_PADDING\_VALUE 

_Padding byte value when ISO\_TP\_FRAME\_PADDING is enabled._ 
```C++
#define ISO_TP_FRAME_PADDING_VALUE `(0xAA)`
```




<hr>



### define ISO\_TP\_MAX\_WFT\_NUMBER 

_Number of FC N\_PDU WTs that can be transmitted by the receiver in a row._ 
```C++
#define ISO_TP_MAX_WFT_NUMBER `(1)`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `inc/isotp_config.h`

