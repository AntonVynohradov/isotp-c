

# File isotp\_link.c



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**isotp\_link.c**](isotp__link_8c.md)

[Go to the source code of this file](isotp__link_8c_source.md)

_ISO-TP link: transmitter and receiver state machines (ISO 15765-2)._ [More...](#detailed-description)

* `#include <string.h>`
* `#include "isotp_link.h"`







































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**isotp\_link\_rx\_consecutive**](#function-isotp_link_rx_consecutive) (struct [**isotp\_link**](structisotp__link.md) \* link, const uint8\_t \* data, const struct [**isotp\_pdu**](structisotp__pdu.md) \* pdu, uint32\_t now\_us) <br> |
|  void | [**isotp\_link\_rx\_fail**](#function-isotp_link_rx_fail) (struct [**isotp\_link**](structisotp__link.md) \* link, int n\_result) <br> |
|  void | [**isotp\_link\_rx\_first**](#function-isotp_link_rx_first) (struct [**isotp\_link**](structisotp__link.md) \* link, const uint8\_t \* data, const struct [**isotp\_pdu**](structisotp__pdu.md) \* pdu, uint32\_t now\_us) <br> |
|  void | [**isotp\_link\_rx\_flow\_control**](#function-isotp_link_rx_flow_control) (struct [**isotp\_link**](structisotp__link.md) \* link, const struct [**isotp\_pdu**](structisotp__pdu.md) \* pdu, uint32\_t now\_us) <br> |
|  void | [**isotp\_link\_rx\_interrupt**](#function-isotp_link_rx_interrupt) (struct [**isotp\_link**](structisotp__link.md) \* link) <br> |
|  void | [**isotp\_link\_rx\_single**](#function-isotp_link_rx_single) (struct [**isotp\_link**](structisotp__link.md) \* link, const uint8\_t \* data, const struct [**isotp\_pdu**](structisotp__pdu.md) \* pdu) <br> |
|  int | [**isotp\_link\_send\_fc**](#function-isotp_link_send_fc) (const struct [**isotp\_link**](structisotp__link.md) \* link, uint8\_t flow\_status) <br> |
|  void | [**isotp\_link\_tx\_consecutive**](#function-isotp_link_tx_consecutive) (struct [**isotp\_link**](structisotp__link.md) \* link, uint32\_t now\_us) <br> |
|  void | [**isotp\_link\_tx\_finish**](#function-isotp_link_tx_finish) (struct [**isotp\_link**](structisotp__link.md) \* link, int n\_result) <br> |
|  int | [**isotp\_link\_write**](#function-isotp_link_write) (const struct [**isotp\_link**](structisotp__link.md) \* link, uint32\_t can\_id, uint8\_t \* frame, int length) <br> |
|  uint32\_t | [**isotp\_min\_u32**](#function-isotp_min_u32) (uint32\_t a, uint32\_t b) <br> |
|  bool | [**isotp\_time\_reached**](#function-isotp_time_reached) (uint32\_t now\_us, uint32\_t deadline\_us) <br> |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ISOTP\_DEFAULT\_PADDING\_VALUE**](isotp__link_8c.md#define-isotp_default_padding_value)  `0xCCU`<br> |
| define  | [**ISOTP\_TIMEOUT\_MAX\_US**](isotp__link_8c.md#define-isotp_timeout_max_us)  `0x7FFFFFFFU /\* deadlines are compared modulo 2^32 \*/`<br> |
| define  | [**ISOTP\_TIME\_HALF\_RANGE**](isotp__link_8c.md#define-isotp_time_half_range)  `0x80000000U`<br> |

## Detailed Description


Project: ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library Author: Anton Vynohradov Email: [avynohradov@systemfromscratch.com](mailto:avynohradov@systemfromscratch.com)




**Copyright:**

Copyright (c) 2026 Anton Vynohradov @license MIT License


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


SPDX-License-Identifier: MIT 


    
## Public Static Functions Documentation




### function isotp\_link\_rx\_consecutive 

```C++
static void isotp_link_rx_consecutive (
    struct isotp_link * link,
    const uint8_t * data,
    const struct isotp_pdu * pdu,
    uint32_t now_us
) 
```




<hr>



### function isotp\_link\_rx\_fail 

```C++
static void isotp_link_rx_fail (
    struct isotp_link * link,
    int n_result
) 
```




<hr>



### function isotp\_link\_rx\_first 

```C++
static void isotp_link_rx_first (
    struct isotp_link * link,
    const uint8_t * data,
    const struct isotp_pdu * pdu,
    uint32_t now_us
) 
```




<hr>



### function isotp\_link\_rx\_flow\_control 

```C++
static void isotp_link_rx_flow_control (
    struct isotp_link * link,
    const struct isotp_pdu * pdu,
    uint32_t now_us
) 
```




<hr>



### function isotp\_link\_rx\_interrupt 

```C++
static void isotp_link_rx_interrupt (
    struct isotp_link * link
) 
```




<hr>



### function isotp\_link\_rx\_single 

```C++
static void isotp_link_rx_single (
    struct isotp_link * link,
    const uint8_t * data,
    const struct isotp_pdu * pdu
) 
```




<hr>



### function isotp\_link\_send\_fc 

```C++
static int isotp_link_send_fc (
    const struct isotp_link * link,
    uint8_t flow_status
) 
```




<hr>



### function isotp\_link\_tx\_consecutive 

```C++
static void isotp_link_tx_consecutive (
    struct isotp_link * link,
    uint32_t now_us
) 
```




<hr>



### function isotp\_link\_tx\_finish 

```C++
static void isotp_link_tx_finish (
    struct isotp_link * link,
    int n_result
) 
```




<hr>



### function isotp\_link\_write 

```C++
static int isotp_link_write (
    const struct isotp_link * link,
    uint32_t can_id,
    uint8_t * frame,
    int length
) 
```




<hr>



### function isotp\_min\_u32 

```C++
static uint32_t isotp_min_u32 (
    uint32_t a,
    uint32_t b
) 
```




<hr>



### function isotp\_time\_reached 

```C++
static bool isotp_time_reached (
    uint32_t now_us,
    uint32_t deadline_us
) 
```




<hr>
## Macro Definition Documentation





### define ISOTP\_DEFAULT\_PADDING\_VALUE 

```C++
#define ISOTP_DEFAULT_PADDING_VALUE `0xCCU`
```




<hr>



### define ISOTP\_TIMEOUT\_MAX\_US 

```C++
#define ISOTP_TIMEOUT_MAX_US `0x7FFFFFFFU /* deadlines are compared modulo 2^32 */`
```




<hr>



### define ISOTP\_TIME\_HALF\_RANGE 

```C++
#define ISOTP_TIME_HALF_RANGE `0x80000000U`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/isotp_link.c`

