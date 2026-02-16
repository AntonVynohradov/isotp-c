

# Group isotp\_defines



[**Modules**](modules.md) **>** [**isotp\_defines**](group__isotp__defines.md)



_Internal protocol definitions and type structures._ [More...](#detailed-description)


































































## Detailed Description


This group contains the low-level building blocks used by the ISO-TP core (frame layouts, PCI types, status codes, and timing helpers). These definitions describe how CAN frames are encoded/decoded and provide enums and constants that drive the state machine in isotp.c.




**
**


* Frame layout structs: [**IsoTpSingleFrame**](structIsoTpSingleFrame.md), [**IsoTpFirstFrameShort**](structIsoTpFirstFrameShort.md), IsoTpFirstFrameLong, [**IsoTpConsecutiveFrame**](structIsoTpConsecutiveFrame.md), [**IsoTpFlowControl**](structIsoTpFlowControl.md). 
* Frame container: [**IsoTpCanMessage**](structIsoTpCanMessage.md) and [**IsoTpDataArray**](structIsoTpDataArray.md). 
* PCI type and flow status enums: [**IsoTpProtocolControlInformation**](isotp__defines_8h.md#enum-isotpprotocolcontrolinformation), [**IsoTpFlowStatus**](isotp__defines_8h.md#enum-isotpflowstatus). 
* Return codes and protocol result codes: [**ISOTP\_RET\_OK**](isotp__defines_8h.md#define-isotp_ret_ok), ISOTP\_RET\_ERROR, ISOTP\_RET\_INPROGRESS, ISOTP\_RET\_OVERFLOW, ISOTP\_RET\_WRONG\_SN, ISOTP\_RET\_NO\_DATA, ISOTP\_RET\_TIMEOUT, ISOTP\_RET\_LENGTH, ISOTP\_RET\_NOSPACE, [**ISOTP\_PROTOCOL\_RESULT\_OK**](isotp__defines_8h.md#define-isotp_protocol_result_ok), ISOTP\_PROTOCOL\_RESULT\_TIMEOUT\_A, ISOTP\_PROTOCOL\_RESULT\_TIMEOUT\_BS, ISOTP\_PROTOCOL\_RESULT\_TIMEOUT\_CR, ISOTP\_PROTOCOL\_RESULT\_WRONG\_SN, ISOTP\_PROTOCOL\_RESULT\_INVALID\_FS, ISOTP\_PROTOCOL\_RESULT\_UNEXP\_PDU, ISOTP\_PROTOCOL\_RESULT\_WFT\_OVRN, ISOTP\_PROTOCOL\_RESULT\_BUFFER\_OVFLW, ISOTP\_PROTOCOL\_RESULT\_ERROR. 
* Byte order and packing helpers: ISOTP\_BYTE\_ORDER\_LITTLE\_ENDIAN, [**ISOTP\_PACKED\_STRUCT**](isotp__defines_8h.md#function-isotp_packed_struct), [**LE32TOH**](isotp__defines_8h.md#define-le32toh). 
* Status types and helpers: [**IsoTpSendStatusTypes**](isotp__defines_8h.md#enum-isotpsendstatustypes), [**IsoTpReceiveStatusTypes**](isotp__defines_8h.md#enum-isotpreceivestatustypes), [**IsoTpTimeAfter**](isotp__defines_8h.md#define-isotptimeafter), [**ISOTP\_INVALID\_BS**](isotp__defines_8h.md#define-isotp_invalid_bs), [**ISOTP\_MAX\_ERROR\_MSG\_SIZE**](isotp__defines_8h.md#define-isotp_max_error_msg_size). 







    

------------------------------


