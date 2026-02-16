

# Struct IsoTpFirstFrameShort



[**ClassList**](annotated.md) **>** [**IsoTpFirstFrameShort**](structIsoTpFirstFrameShort.md)



_First frame (short) layout._ [More...](#detailed-description)

* `#include <isotp_defines.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  uint8\_t | [**FF\_DL\_high**](#variable-ff_dl_high)  <br> |
|  uint8\_t | [**FF\_DL\_low**](#variable-ff_dl_low)  <br> |
|  uint8\_t | [**data**](#variable-data)  <br> |
|  uint8\_t | [**type**](#variable-type)  <br> |












































## Detailed Description



```C++
+-------------------------+-----------------------+-----+
| byte #0                 | byte #1               | ... |
+-------------------------+-----------+-----------+-----+
| nibble #0   | nibble #1 | nibble #2 | nibble #3 | ... |
+-------------+-----------+-----------+-----------+-----+
| PCIType = 1 | FF_DL                             | ... |
+-------------+-----------+-----------------------+-----+
```
 


    
## Public Attributes Documentation




### variable FF\_DL\_high 

```C++
uint8_t IsoTpFirstFrameShort::FF_DL_high;
```



First-frame data length (high nibble). 


        

<hr>



### variable FF\_DL\_low 

```C++
uint8_t IsoTpFirstFrameShort::FF_DL_low;
```



First-frame data length (low byte). 


        

<hr>



### variable data 

```C++
uint8_t IsoTpFirstFrameShort::data[6];
```



Payload bytes. 


        

<hr>



### variable type 

```C++
uint8_t IsoTpFirstFrameShort::type;
```



PCI type. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `inc/isotp_defines.h`

