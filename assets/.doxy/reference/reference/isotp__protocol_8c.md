

# File isotp\_protocol.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**isotp\_protocol.c**](isotp__protocol_8c.md)

[Go to the source code of this file](isotp__protocol_8c_source.md)

_ISO-TP frame codec: decoding, encoding and STmin conversion._ [More...](#detailed-description)

* `#include <stdbool.h>`
* `#include <string.h>`
* `#include "isotp_protocol.h"`







































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  bool | [**isotp\_decode\_consecutive**](#function-isotp_decode_consecutive) (const uint8\_t \* frame, size\_t length, struct [**isotp\_pdu**](structisotp__pdu.md) \* pdu) <br> |
|  bool | [**isotp\_decode\_first**](#function-isotp_decode_first) (const uint8\_t \* frame, size\_t length, struct [**isotp\_pdu**](structisotp__pdu.md) \* pdu) <br> |
|  bool | [**isotp\_decode\_flow\_control**](#function-isotp_decode_flow_control) (const uint8\_t \* frame, size\_t length, struct [**isotp\_pdu**](structisotp__pdu.md) \* pdu) <br> |
|  bool | [**isotp\_decode\_single**](#function-isotp_decode_single) (const uint8\_t \* frame, size\_t length, struct [**isotp\_pdu**](structisotp__pdu.md) \* pdu) <br> |
|  void | [**isotp\_pdu\_clear**](#function-isotp_pdu_clear) (struct [**isotp\_pdu**](structisotp__pdu.md) \* pdu) <br> |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ISOTP\_FF\_ESCAPE\_OFFSET**](isotp__protocol_8c.md#define-isotp_ff_escape_offset)  `6U /\* first payload byte with the 32-bit FF\_DL \*/`<br> |
| define  | [**ISOTP\_FF\_SHORT\_OFFSET**](isotp__protocol_8c.md#define-isotp_ff_short_offset)  `2U  /\* first payload byte with the 12-bit FF\_DL \*/`<br> |
| define  | [**ISOTP\_LOW\_NIBBLE**](isotp__protocol_8c.md#define-isotp_low_nibble)  `0x0FU    /\* SF\_DL, FF\_DL high bits, SN and FS \*/`<br> |
| define  | [**ISOTP\_PCI\_TYPE\_SHIFT**](isotp__protocol_8c.md#define-isotp_pci_type_shift)  `4U   /\* frame type is the high nibble of byte 0 \*/`<br> |
| define  | [**ISOTP\_ST\_MIN\_MS\_MAX**](isotp__protocol_8c.md#define-isotp_st_min_ms_max)  `0x7FU /\* 0x00..0x7F: milliseconds \*/`<br> |
| define  | [**ISOTP\_ST\_MIN\_US\_BASE**](isotp__protocol_8c.md#define-isotp_st_min_us_base)  `0xF0U`<br> |
| define  | [**ISOTP\_ST\_MIN\_US\_FIRST**](isotp__protocol_8c.md#define-isotp_st_min_us_first)  `0xF1U`<br> |
| define  | [**ISOTP\_ST\_MIN\_US\_LAST**](isotp__protocol_8c.md#define-isotp_st_min_us_last)  `0xF9U`<br> |
| define  | [**ISOTP\_ST\_MIN\_US\_STEPS\_MAX**](isotp__protocol_8c.md#define-isotp_st_min_us_steps_max)  `900U`<br> |
| define  | [**ISOTP\_US\_PER\_MS**](isotp__protocol_8c.md#define-isotp_us_per_ms)  `1000U`<br> |
| define  | [**ISOTP\_US\_PER\_STEP**](isotp__protocol_8c.md#define-isotp_us_per_step)  `100U`<br> |

## Detailed Description


Project: ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library Author: Anton Vynohradov Email: [avynohradov@systemfromscratch.com](mailto:avynohradov@systemfromscratch.com)




**Copyright:**

Copyright (c) 2026 Anton Vynohradov @license MIT License


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


SPDX-License-Identifier: MIT 


    
## Public Static Functions Documentation




### function isotp\_decode\_consecutive 

```C++
static bool isotp_decode_consecutive (
    const uint8_t * frame,
    size_t length,
    struct isotp_pdu * pdu
) 
```




<hr>



### function isotp\_decode\_first 

```C++
static bool isotp_decode_first (
    const uint8_t * frame,
    size_t length,
    struct isotp_pdu * pdu
) 
```




<hr>



### function isotp\_decode\_flow\_control 

```C++
static bool isotp_decode_flow_control (
    const uint8_t * frame,
    size_t length,
    struct isotp_pdu * pdu
) 
```




<hr>



### function isotp\_decode\_single 

```C++
static bool isotp_decode_single (
    const uint8_t * frame,
    size_t length,
    struct isotp_pdu * pdu
) 
```




<hr>



### function isotp\_pdu\_clear 

```C++
static void isotp_pdu_clear (
    struct isotp_pdu * pdu
) 
```




<hr>
## Macro Definition Documentation





### define ISOTP\_FF\_ESCAPE\_OFFSET 

```C++
#define ISOTP_FF_ESCAPE_OFFSET `6U /* first payload byte with the 32-bit FF_DL */`
```




<hr>



### define ISOTP\_FF\_SHORT\_OFFSET 

```C++
#define ISOTP_FF_SHORT_OFFSET `2U  /* first payload byte with the 12-bit FF_DL */`
```




<hr>



### define ISOTP\_LOW\_NIBBLE 

```C++
#define ISOTP_LOW_NIBBLE `0x0FU    /* SF_DL, FF_DL high bits, SN and FS */`
```




<hr>



### define ISOTP\_PCI\_TYPE\_SHIFT 

```C++
#define ISOTP_PCI_TYPE_SHIFT `4U   /* frame type is the high nibble of byte 0 */`
```




<hr>



### define ISOTP\_ST\_MIN\_MS\_MAX 

```C++
#define ISOTP_ST_MIN_MS_MAX `0x7FU /* 0x00..0x7F: milliseconds */`
```




<hr>



### define ISOTP\_ST\_MIN\_US\_BASE 

```C++
#define ISOTP_ST_MIN_US_BASE `0xF0U`
```




<hr>



### define ISOTP\_ST\_MIN\_US\_FIRST 

```C++
#define ISOTP_ST_MIN_US_FIRST `0xF1U`
```




<hr>



### define ISOTP\_ST\_MIN\_US\_LAST 

```C++
#define ISOTP_ST_MIN_US_LAST `0xF9U`
```




<hr>



### define ISOTP\_ST\_MIN\_US\_STEPS\_MAX 

```C++
#define ISOTP_ST_MIN_US_STEPS_MAX `900U`
```




<hr>



### define ISOTP\_US\_PER\_MS 

```C++
#define ISOTP_US_PER_MS `1000U`
```




<hr>



### define ISOTP\_US\_PER\_STEP 

```C++
#define ISOTP_US_PER_STEP `100U`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/isotp_protocol.c`

