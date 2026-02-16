

# Struct IsoTpSingleFrame



[**ClassList**](annotated.md) **>** [**IsoTpSingleFrame**](structIsoTpSingleFrame.md)



_Single frame layout._ [More...](#detailed-description)

* `#include <isotp_defines.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  uint8\_t | [**SF\_DL**](#variable-sf_dl)  <br> |
|  uint8\_t | [**data**](#variable-data)  <br> |
|  uint8\_t | [**type**](#variable-type)  <br> |












































## Detailed Description



```C++
+-------------------------+-----+
| byte #0                 | ... |
+-------------------------+-----+
| nibble #0   | nibble #1 | ... |
+-------------+-----------+ ... +
| PCIType = 0 | SF_DL     | ... |
+-------------+-----------+-----+
```
 


    
## Public Attributes Documentation




### variable SF\_DL 

```C++
uint8_t IsoTpSingleFrame::SF_DL;
```



Single-frame data length. 


        

<hr>



### variable data 

```C++
uint8_t IsoTpSingleFrame::data[7];
```



Payload bytes. 


        

<hr>



### variable type 

```C++
uint8_t IsoTpSingleFrame::type;
```



PCI type. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `inc/isotp_defines.h`

