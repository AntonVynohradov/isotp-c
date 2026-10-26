

# Struct isotp\_pdu



[**ClassList**](annotated.md) **>** [**isotp\_pdu**](structisotp__pdu.md)



_One decoded ISO-TP frame (protocol data unit)._ [More...](#detailed-description)

* `#include <isotp_protocol.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  uint8\_t | [**block\_size**](#variable-block_size)  <br>_BS (FC)._  |
|  uint8\_t | [**data\_length**](#variable-data_length)  <br>_Payload bytes in this frame (SF, FF, CF)._  |
|  uint8\_t | [**data\_offset**](#variable-data_offset)  <br>_Index of the first payload byte in the frame._  |
|  uint8\_t | [**flow\_status**](#variable-flow_status)  <br>_FS (FC), not range-checked: see_ [_**isotp\_decode**_](group__isotp__protocol.md#function-isotp_decode) _._ |
|  uint32\_t | [**message\_length**](#variable-message_length)  <br>_SF\_DL (SF) or FF\_DL (FF): length of the whole message._  |
|  uint8\_t | [**sequence\_number**](#variable-sequence_number)  <br>_SN (CF)._  |
|  uint8\_t | [**st\_min**](#variable-st_min)  <br>_Raw STmin byte (FC); convert with_ [_**isotp\_st\_min\_decode**_](group__isotp__protocol.md#function-isotp_st_min_decode) _._ |
|  uint8\_t | [**type**](#variable-type)  <br>_ISOTP\_PCI\_\* frame type._  |












































## Detailed Description


Only the members of the decoded frame type are meaningful; the others are zero. The payload is not copied: it is `frame[data_offset]` .. `frame[data_offset + data_length - 1]` of the frame passed to [**isotp\_decode**](group__isotp__protocol.md#function-isotp_decode). 


    
## Public Attributes Documentation




### variable block\_size 

_BS (FC)._ 
```C++
uint8_t isotp_pdu::block_size;
```




<hr>



### variable data\_length 

_Payload bytes in this frame (SF, FF, CF)._ 
```C++
uint8_t isotp_pdu::data_length;
```




<hr>



### variable data\_offset 

_Index of the first payload byte in the frame._ 
```C++
uint8_t isotp_pdu::data_offset;
```




<hr>



### variable flow\_status 

_FS (FC), not range-checked: see_ [_**isotp\_decode**_](group__isotp__protocol.md#function-isotp_decode) _._
```C++
uint8_t isotp_pdu::flow_status;
```




<hr>



### variable message\_length 

_SF\_DL (SF) or FF\_DL (FF): length of the whole message._ 
```C++
uint32_t isotp_pdu::message_length;
```




<hr>



### variable sequence\_number 

_SN (CF)._ 
```C++
uint8_t isotp_pdu::sequence_number;
```




<hr>



### variable st\_min 

_Raw STmin byte (FC); convert with_ [_**isotp\_st\_min\_decode**_](group__isotp__protocol.md#function-isotp_st_min_decode) _._
```C++
uint8_t isotp_pdu::st_min;
```




<hr>



### variable type 

_ISOTP\_PCI\_\* frame type._ 
```C++
uint8_t isotp_pdu::type;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `inc/isotp_protocol.h`

