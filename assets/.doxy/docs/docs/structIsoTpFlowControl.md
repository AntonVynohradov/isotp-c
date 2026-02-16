

# Struct IsoTpFlowControl



[**ClassList**](annotated.md) **>** [**IsoTpFlowControl**](structIsoTpFlowControl.md)



_Flow control frame layout._ [More...](#detailed-description)

* `#include <isotp_defines.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  uint8\_t | [**BS**](#variable-bs)  <br> |
|  uint8\_t | [**FS**](#variable-fs)  <br> |
|  uint8\_t | [**STmin**](#variable-stmin)  <br> |
|  uint8\_t | [**reserve**](#variable-reserve)  <br> |
|  uint8\_t | [**type**](#variable-type)  <br> |












































## Detailed Description



```C++
+-------------------------+-----------------------+-----------------------+-----+
| byte #0                 | byte #1               | byte #2               | ... |
+-------------------------+-----------+-----------+-----------+-----------+-----+
| nibble #0   | nibble #1 | nibble #2 | nibble #3 | nibble #4 | nibble #5 | ... |
+-------------+-----------+-----------+-----------+-----------+-----------+-----+
| PCIType = 1 | FS        | BS                    | STmin                 | ... |
+-------------+-----------+-----------------------+-----------------------+-----+
```
 


    
## Public Attributes Documentation




### variable BS 

```C++
uint8_t IsoTpFlowControl::BS;
```



Block size. 


        

<hr>



### variable FS 

```C++
uint8_t IsoTpFlowControl::FS;
```



Flow status. 


        

<hr>



### variable STmin 

```C++
uint8_t IsoTpFlowControl::STmin;
```



Minimum separation time. 


        

<hr>



### variable reserve 

```C++
uint8_t IsoTpFlowControl::reserve[5];
```



Reserved. 


        

<hr>



### variable type 

```C++
uint8_t IsoTpFlowControl::type;
```



PCI type. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `inc/isotp_defines.h`

