

# Group isotp\_protocol



[**Modules**](modules.md) **>** [**isotp\_protocol**](group__isotp__protocol.md)



_Pure functions that translate between CAN frames and ISO-TP PDUs._ [More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**isotp\_pdu**](structisotp__pdu.md) <br>_One decoded ISO-TP frame (protocol data unit)._  |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  int | [**isotp\_decode**](#function-isotp_decode) (const uint8\_t \* frame, size\_t length, struct [**isotp\_pdu**](structisotp__pdu.md) \* pdu) <br>_Validate one received CAN frame and describe it._  |
|  int | [**isotp\_encode\_consecutive**](#function-isotp_encode_consecutive) (uint8\_t \* frame, size\_t capacity, uint8\_t sequence\_number, const uint8\_t \* data, uint32\_t length) <br>_Encode a Consecutive Frame._  |
|  int | [**isotp\_encode\_first**](#function-isotp_encode_first) (uint8\_t \* frame, size\_t capacity, const uint8\_t \* payload, uint32\_t message\_length) <br>_Encode a First Frame and its first payload bytes._  |
|  int | [**isotp\_encode\_flow\_control**](#function-isotp_encode_flow_control) (uint8\_t \* frame, size\_t capacity, uint8\_t flow\_status, uint8\_t block\_size, uint8\_t st\_min) <br>_Encode a Flow Control frame._  |
|  int | [**isotp\_encode\_single**](#function-isotp_encode_single) (uint8\_t \* frame, size\_t capacity, const uint8\_t \* payload, uint32\_t length) <br>_Encode a Single Frame._  |
|  int | [**isotp\_frame\_pad**](#function-isotp_frame_pad) (uint8\_t \* frame, size\_t capacity, size\_t length, uint8\_t value) <br>_Fill a frame with a padding byte up to 8 data bytes._  |
|  uint32\_t | [**isotp\_st\_min\_decode**](#function-isotp_st_min_decode) (uint8\_t st\_min) <br>_Decode a received STmin byte._  |
|  uint8\_t | [**isotp\_st\_min\_encode**](#function-isotp_st_min_encode) (uint32\_t us) <br>_Encode a separation time for a Flow Control frame._  |




























## Detailed Description


The codec holds no state. [**isotp\_decode**](group__isotp__protocol.md#function-isotp_decode) validates one received CAN frame and describes it in a [**isotp\_pdu**](structisotp__pdu.md); the encoders write one frame into a caller buffer and return its length. Frames are classical CAN (up to 8 data bytes) with normal addressing. The encoders produce the shortest frame; [**isotp\_frame\_pad**](group__isotp__protocol.md#function-isotp_frame_pad) fills it up to 8 bytes when the bus requires padding.




**
**


* Decoding: [**isotp\_decode**](group__isotp__protocol.md#function-isotp_decode). 
* Encoding: [**isotp\_encode\_single**](group__isotp__protocol.md#function-isotp_encode_single), [**isotp\_encode\_first**](group__isotp__protocol.md#function-isotp_encode_first), [**isotp\_encode\_consecutive**](group__isotp__protocol.md#function-isotp_encode_consecutive), [**isotp\_encode\_flow\_control**](group__isotp__protocol.md#function-isotp_encode_flow_control), [**isotp\_frame\_pad**](group__isotp__protocol.md#function-isotp_frame_pad). 
* Separation time: [**isotp\_st\_min\_encode**](group__isotp__protocol.md#function-isotp_st_min_encode), [**isotp\_st\_min\_decode**](group__isotp__protocol.md#function-isotp_st_min_decode). 







    
## Public Functions Documentation




### function isotp\_decode 

_Validate one received CAN frame and describe it._ 
```
int isotp_decode (
    const uint8_t * frame,
    size_t length,
    struct isotp_pdu * pdu
) 
```



Rejects frames that ISO 15765-2 tells a receiver to ignore: unknown frame type, SF\_DL of 0 or longer than the frame, a First Frame that is not 8 bytes long or announces a message that would fit a Single Frame (or, with the 32-bit FF\_DL, fits the 12-bit field), a Consecutive Frame without data, a Flow Control frame shorter than 3 bytes. The flow status is not checked here: an unknown value is an N\_INVALID\_FS condition the sender has to report. 

**Parameters:**


* `frame` CAN frame data. 
* `length` CAN data length, 1..8. 
* `pdu` Receives the description. 



**Return value:**


* `ISOTP_OK` `pdu` describes a valid frame. 
* `ISOTP_ERROR` NULL argument, invalid length or malformed frame. 




        

<hr>



### function isotp\_encode\_consecutive 

_Encode a Consecutive Frame._ 
```
int isotp_encode_consecutive (
    uint8_t * frame,
    size_t capacity,
    uint8_t sequence_number,
    const uint8_t * data,
    uint32_t length
) 
```





**Parameters:**


* `frame` Output buffer. 
* `capacity` Size of `frame`, at least `length` + 1. 
* `sequence_number` SN, 0..15. 
* `data` Segment bytes. 
* `length` Segment length, 1..7. 



**Returns:**

Frame length (`length` + 1), or ISOTP\_ERROR. 





        

<hr>



### function isotp\_encode\_first 

_Encode a First Frame and its first payload bytes._ 
```
int isotp_encode_first (
    uint8_t * frame,
    size_t capacity,
    const uint8_t * payload,
    uint32_t message_length
) 
```



Messages up to 4095 bytes use the 12-bit FF\_DL and carry 6 payload bytes; longer messages use the escape sequence with a 32-bit FF\_DL and carry 2 payload bytes. 

**Parameters:**


* `frame` Output buffer. 
* `capacity` Size of `frame`, at least 8. 
* `payload` Start of the message; at least 6 (or 2) bytes are read. 
* `message_length` Length of the whole message, at least 8. 



**Returns:**

Frame length (8), or ISOTP\_ERROR. 





        

<hr>



### function isotp\_encode\_flow\_control 

_Encode a Flow Control frame._ 
```
int isotp_encode_flow_control (
    uint8_t * frame,
    size_t capacity,
    uint8_t flow_status,
    uint8_t block_size,
    uint8_t st_min
) 
```





**Parameters:**


* `frame` Output buffer. 
* `capacity` Size of `frame`, at least 3. 
* `flow_status` ISOTP\_FS\_CONTINUE, ISOTP\_FS\_WAIT or ISOTP\_FS\_OVERFLOW. 
* `block_size` BS, 0 = no further Flow Control. 
* `st_min` Raw STmin byte, see [**isotp\_st\_min\_encode**](group__isotp__protocol.md#function-isotp_st_min_encode). 



**Returns:**

Frame length (3), or ISOTP\_ERROR. 





        

<hr>



### function isotp\_encode\_single 

_Encode a Single Frame._ 
```
int isotp_encode_single (
    uint8_t * frame,
    size_t capacity,
    const uint8_t * payload,
    uint32_t length
) 
```





**Parameters:**


* `frame` Output buffer. 
* `capacity` Size of `frame`, at least `length` + 1. 
* `payload` Message bytes. 
* `length` Message length, 1..7. 



**Returns:**

Frame length (`length` + 1), or ISOTP\_ERROR. 





        

<hr>



### function isotp\_frame\_pad 

_Fill a frame with a padding byte up to 8 data bytes._ 
```
int isotp_frame_pad (
    uint8_t * frame,
    size_t capacity,
    size_t length,
    uint8_t value
) 
```





**Parameters:**


* `frame` Frame buffer. 
* `capacity` Size of `frame`, at least 8. 
* `length` Current frame length, 1..8. 
* `value` Padding byte. 



**Returns:**

New frame length (8), or ISOTP\_ERROR. 





        

<hr>



### function isotp\_st\_min\_decode 

_Decode a received STmin byte._ 
```
uint32_t isotp_st_min_decode (
    uint8_t st_min
) 
```



0x00..0x7F are milliseconds, 0xF1..0xF9 are 100..900 us. ISO 15765-2 tells the sender to treat the reserved values as 0x7F (127 ms). 

**Parameters:**


* `st_min` Raw STmin byte. 



**Returns:**

Separation time in microseconds. 





        

<hr>



### function isotp\_st\_min\_encode 

_Encode a separation time for a Flow Control frame._ 
```
uint8_t isotp_st_min_encode (
    uint32_t us
) 
```



Rounds up, so the sender never sends faster than requested: 0 us gives 0x00, 1..900 us give 0xF1..0xF9 (100 us steps), 901..127000 us give 0x01..0x7F (1 ms steps), larger values 0x7F. 

**Parameters:**


* `us` Separation time in microseconds. 



**Returns:**

Raw STmin byte. 





        

<hr>

------------------------------


