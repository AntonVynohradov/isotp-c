

# Group isotp\_defs



[**Modules**](modules.md) **>** [**isotp\_defs**](group__isotp__defs.md)



_Protocol constants of ISO 15765-2 and the library result codes._ [More...](#detailed-description)








































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  uint32\_t | [**isotp\_get\_u32\_be**](#function-isotp_get_u32_be) (const uint8\_t \* data) <br>_Read a big-endian 32-bit value._  |
|  void | [**isotp\_put\_u32\_be**](#function-isotp_put_u32_be) (uint8\_t \* data, uint32\_t value) <br>_Write a 32-bit value in big-endian byte order._  |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ISOTP\_BUSY**](group__isotp__defs.md#define-isotp_busy)  `-2`<br>_Try again later: transmission in progress or CAN driver busy._  |
| define  | [**ISOTP\_CAN\_DL**](group__isotp__defs.md#define-isotp_can_dl)  `8U`<br>_Data bytes of a classical CAN frame._  |
| define  | [**ISOTP\_CF\_MAX\_DATA**](group__isotp__defs.md#define-isotp_cf_max_data)  `7U`<br>_Payload bytes in a full Consecutive Frame._  |
| define  | [**ISOTP\_ERROR**](group__isotp__defs.md#define-isotp_error)  `-1`<br>_Invalid argument, malformed frame or insufficient capacity._  |
| define  | [**ISOTP\_FC\_DL**](group__isotp__defs.md#define-isotp_fc_dl)  `3U`<br>_Bytes of a Flow Control frame without padding._  |
| define  | [**ISOTP\_FF\_DL\_12BIT\_MAX**](group__isotp__defs.md#define-isotp_ff_dl_12bit_max)  `4095U`<br>_Largest length in the 12-bit FF\_DL field._  |
| define  | [**ISOTP\_FF\_LONG\_DATA**](group__isotp__defs.md#define-isotp_ff_long_data)  `2U`<br>_Payload bytes in a First Frame with 32-bit FF\_DL._  |
| define  | [**ISOTP\_FF\_SHORT\_DATA**](group__isotp__defs.md#define-isotp_ff_short_data)  `6U`<br>_Payload bytes in a First Frame with 12-bit FF\_DL._  |
| define  | [**ISOTP\_FIRST\_SN**](group__isotp__defs.md#define-isotp_first_sn)  `1U`<br>_SN of the first Consecutive Frame._  |
| define  | [**ISOTP\_FS\_CONTINUE**](group__isotp__defs.md#define-isotp_fs_continue)  `0x0U`<br>_CTS: continue to send._  |
| define  | [**ISOTP\_FS\_OVERFLOW**](group__isotp__defs.md#define-isotp_fs_overflow)  `0x2U`<br>_OVFLW: message longer than the receiver buffer._  |
| define  | [**ISOTP\_FS\_WAIT**](group__isotp__defs.md#define-isotp_fs_wait)  `0x1U`<br>_WAIT: wait for the next Flow Control frame._  |
| define  | [**ISOTP\_N\_BUFFER\_OVFLW**](group__isotp__defs.md#define-isotp_n_buffer_ovflw)  `-8`<br>_Message longer than the receiver buffer._  |
| define  | [**ISOTP\_N\_ERROR**](group__isotp__defs.md#define-isotp_n_error)  `-9`<br>_CAN driver error while sending a frame._  |
| define  | [**ISOTP\_N\_INVALID\_FS**](group__isotp__defs.md#define-isotp_n_invalid_fs)  `-5`<br>_Sender: Flow Control with an unknown flow status._  |
| define  | [**ISOTP\_N\_OK**](group__isotp__defs.md#define-isotp_n_ok)  `0`<br>_Completed successfully._  |
| define  | [**ISOTP\_N\_TIMEOUT\_A**](group__isotp__defs.md#define-isotp_n_timeout_a)  `-1`<br>_N\_As/N\_Ar expired (not produced: writes are synchronous)._  |
| define  | [**ISOTP\_N\_TIMEOUT\_BS**](group__isotp__defs.md#define-isotp_n_timeout_bs)  `-2`<br>_Sender: no Flow Control within N\_Bs._  |
| define  | [**ISOTP\_N\_TIMEOUT\_CR**](group__isotp__defs.md#define-isotp_n_timeout_cr)  `-3`<br>_Receiver: no Consecutive Frame within N\_Cr._  |
| define  | [**ISOTP\_N\_UNEXP\_PDU**](group__isotp__defs.md#define-isotp_n_unexp_pdu)  `-6`<br>_Receiver: new SF/FF interrupted a reception._  |
| define  | [**ISOTP\_N\_WFT\_OVRN**](group__isotp__defs.md#define-isotp_n_wft_ovrn)  `-7`<br>_Sender: more than N\_WFTmax Wait frames._  |
| define  | [**ISOTP\_N\_WRONG\_SN**](group__isotp__defs.md#define-isotp_n_wrong_sn)  `-4`<br>_Receiver: Consecutive Frame with an unexpected SN._  |
| define  | [**ISOTP\_OK**](group__isotp__defs.md#define-isotp_ok)  `0`<br>_Success._  |
| define  | [**ISOTP\_PCI\_CONSECUTIVE\_FRAME**](group__isotp__defs.md#define-isotp_pci_consecutive_frame)  `0x2U`<br>_CF: next segment of a message._  |
| define  | [**ISOTP\_PCI\_FIRST\_FRAME**](group__isotp__defs.md#define-isotp_pci_first_frame)  `0x1U`<br>_FF: start of a segmented message._  |
| define  | [**ISOTP\_PCI\_FLOW\_CONTROL**](group__isotp__defs.md#define-isotp_pci_flow_control)  `0x3U`<br>_FC: receiver tells the sender how to continue._  |
| define  | [**ISOTP\_PCI\_SINGLE\_FRAME**](group__isotp__defs.md#define-isotp_pci_single_frame)  `0x0U`<br>_SF: complete message of up to 7 bytes._  |
| define  | [**ISOTP\_SF\_MAX\_DL**](group__isotp__defs.md#define-isotp_sf_max_dl)  `7U`<br>_Largest Single Frame payload._  |
| define  | [**ISOTP\_SN\_MASK**](group__isotp__defs.md#define-isotp_sn_mask)  `0x0FU`<br>_Sequence numbers count 0..15 and wrap._  |
| define  | [**ISOTP\_ST\_MIN\_MAX\_US**](group__isotp__defs.md#define-isotp_st_min_max_us)  `127000U`<br>_Largest STmin, 0x7F = 127 ms._  |

## Detailed Description


Wire values (frame types, flow status, field limits) are taken from ISO 15765-2 for classical CAN (8 data bytes) with normal addressing. The result codes are library values: the standard defines the N\_Result names, not their numbers.




**
**


* Function return codes: [**ISOTP\_OK**](group__isotp__defs.md#define-isotp_ok), [**ISOTP\_ERROR**](group__isotp__defs.md#define-isotp_error), [**ISOTP\_BUSY**](group__isotp__defs.md#define-isotp_busy). 
* Frame types (N\_PCItype) and flow status values. 
* Frame layout limits: [**ISOTP\_CAN\_DL**](group__isotp__defs.md#define-isotp_can_dl), [**ISOTP\_SF\_MAX\_DL**](group__isotp__defs.md#define-isotp_sf_max_dl), ... 
* N\_Result codes reported by the link: [**ISOTP\_N\_OK**](group__isotp__defs.md#define-isotp_n_ok), ... 
* Big-endian helpers: [**isotp\_get\_u32\_be**](group__isotp__defs.md#function-isotp_get_u32_be), [**isotp\_put\_u32\_be**](group__isotp__defs.md#function-isotp_put_u32_be). 







    
## Public Static Functions Documentation




### function isotp\_get\_u32\_be 

_Read a big-endian 32-bit value._ 
```
static inline uint32_t isotp_get_u32_be (
    const uint8_t * data
) 
```





**Parameters:**


* `data` At least four bytes. 



**Returns:**

The value. 





        

<hr>



### function isotp\_put\_u32\_be 

_Write a 32-bit value in big-endian byte order._ 
```
static inline void isotp_put_u32_be (
    uint8_t * data,
    uint32_t value
) 
```





**Parameters:**


* `data` At least four bytes. 
* `value` Value to write. 




        

<hr>
## Macro Definition Documentation





### define ISOTP\_BUSY 

_Try again later: transmission in progress or CAN driver busy._ 
```
#define ISOTP_BUSY `-2`
```




<hr>



### define ISOTP\_CAN\_DL 

_Data bytes of a classical CAN frame._ 
```
#define ISOTP_CAN_DL `8U`
```




<hr>



### define ISOTP\_CF\_MAX\_DATA 

_Payload bytes in a full Consecutive Frame._ 
```
#define ISOTP_CF_MAX_DATA `7U`
```




<hr>



### define ISOTP\_ERROR 

_Invalid argument, malformed frame or insufficient capacity._ 
```
#define ISOTP_ERROR `-1`
```




<hr>



### define ISOTP\_FC\_DL 

_Bytes of a Flow Control frame without padding._ 
```
#define ISOTP_FC_DL `3U`
```




<hr>



### define ISOTP\_FF\_DL\_12BIT\_MAX 

_Largest length in the 12-bit FF\_DL field._ 
```
#define ISOTP_FF_DL_12BIT_MAX `4095U`
```




<hr>



### define ISOTP\_FF\_LONG\_DATA 

_Payload bytes in a First Frame with 32-bit FF\_DL._ 
```
#define ISOTP_FF_LONG_DATA `2U`
```




<hr>



### define ISOTP\_FF\_SHORT\_DATA 

_Payload bytes in a First Frame with 12-bit FF\_DL._ 
```
#define ISOTP_FF_SHORT_DATA `6U`
```




<hr>



### define ISOTP\_FIRST\_SN 

_SN of the first Consecutive Frame._ 
```
#define ISOTP_FIRST_SN `1U`
```




<hr>



### define ISOTP\_FS\_CONTINUE 

_CTS: continue to send._ 
```
#define ISOTP_FS_CONTINUE `0x0U`
```




<hr>



### define ISOTP\_FS\_OVERFLOW 

_OVFLW: message longer than the receiver buffer._ 
```
#define ISOTP_FS_OVERFLOW `0x2U`
```




<hr>



### define ISOTP\_FS\_WAIT 

_WAIT: wait for the next Flow Control frame._ 
```
#define ISOTP_FS_WAIT `0x1U`
```




<hr>



### define ISOTP\_N\_BUFFER\_OVFLW 

_Message longer than the receiver buffer._ 
```
#define ISOTP_N_BUFFER_OVFLW `-8`
```




<hr>



### define ISOTP\_N\_ERROR 

_CAN driver error while sending a frame._ 
```
#define ISOTP_N_ERROR `-9`
```




<hr>



### define ISOTP\_N\_INVALID\_FS 

_Sender: Flow Control with an unknown flow status._ 
```
#define ISOTP_N_INVALID_FS `-5`
```




<hr>



### define ISOTP\_N\_OK 

_Completed successfully._ 
```
#define ISOTP_N_OK `0`
```




<hr>



### define ISOTP\_N\_TIMEOUT\_A 

_N\_As/N\_Ar expired (not produced: writes are synchronous)._ 
```
#define ISOTP_N_TIMEOUT_A `-1`
```




<hr>



### define ISOTP\_N\_TIMEOUT\_BS 

_Sender: no Flow Control within N\_Bs._ 
```
#define ISOTP_N_TIMEOUT_BS `-2`
```




<hr>



### define ISOTP\_N\_TIMEOUT\_CR 

_Receiver: no Consecutive Frame within N\_Cr._ 
```
#define ISOTP_N_TIMEOUT_CR `-3`
```




<hr>



### define ISOTP\_N\_UNEXP\_PDU 

_Receiver: new SF/FF interrupted a reception._ 
```
#define ISOTP_N_UNEXP_PDU `-6`
```




<hr>



### define ISOTP\_N\_WFT\_OVRN 

_Sender: more than N\_WFTmax Wait frames._ 
```
#define ISOTP_N_WFT_OVRN `-7`
```




<hr>



### define ISOTP\_N\_WRONG\_SN 

_Receiver: Consecutive Frame with an unexpected SN._ 
```
#define ISOTP_N_WRONG_SN `-4`
```




<hr>



### define ISOTP\_OK 

_Success._ 
```
#define ISOTP_OK `0`
```




<hr>



### define ISOTP\_PCI\_CONSECUTIVE\_FRAME 

_CF: next segment of a message._ 
```
#define ISOTP_PCI_CONSECUTIVE_FRAME `0x2U`
```




<hr>



### define ISOTP\_PCI\_FIRST\_FRAME 

_FF: start of a segmented message._ 
```
#define ISOTP_PCI_FIRST_FRAME `0x1U`
```




<hr>



### define ISOTP\_PCI\_FLOW\_CONTROL 

_FC: receiver tells the sender how to continue._ 
```
#define ISOTP_PCI_FLOW_CONTROL `0x3U`
```




<hr>



### define ISOTP\_PCI\_SINGLE\_FRAME 

_SF: complete message of up to 7 bytes._ 
```
#define ISOTP_PCI_SINGLE_FRAME `0x0U`
```




<hr>



### define ISOTP\_SF\_MAX\_DL 

_Largest Single Frame payload._ 
```
#define ISOTP_SF_MAX_DL `7U`
```




<hr>



### define ISOTP\_SN\_MASK 

_Sequence numbers count 0..15 and wrap._ 
```
#define ISOTP_SN_MASK `0x0FU`
```




<hr>



### define ISOTP\_ST\_MIN\_MAX\_US 

_Largest STmin, 0x7F = 127 ms._ 
```
#define ISOTP_ST_MIN_MAX_US `127000U`
```




<hr>

------------------------------


