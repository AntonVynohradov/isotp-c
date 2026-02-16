

# Struct IsoTpConsecutiveFrame



[**ClassList**](annotated.md) **>** [**IsoTpConsecutiveFrame**](structIsoTpConsecutiveFrame.md)



_Consecutive frame layout._ [More...](#detailed-description)

* `#include <isotp_defines.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  uint8\_t | [**SN**](#variable-sn)  <br> |
|  uint8\_t | [**data**](#variable-data)  <br> |
|  uint8\_t | [**type**](#variable-type)  <br> |












































## Detailed Description



```C++
+-------------------------+-----+
| byte #0                 | ... |
+-------------------------+-----+
| nibble #0   | nibble #1 | ... |
+-------------+-----------+ ... +
| PCIType = 0 | SN        | ... |
+-------------+-----------+-----+
```
 


    
## Public Attributes Documentation




### variable SN 

```C++
uint8_t IsoTpConsecutiveFrame::SN;
```



Sequence number. 


        

<hr>



### variable data 

```C++
uint8_t IsoTpConsecutiveFrame::data[7];
```



Payload bytes. 


        

<hr>



### variable type 

```C++
uint8_t IsoTpConsecutiveFrame::type;
```



PCI type. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `inc/isotp_defines.h`

