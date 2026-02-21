

# File linux\_socket.c



[**FileList**](files.md) **>** [**examples**](dir_d28a4824dc47e487b107a5db32ef43c4.md) **>** [**linux\_socket**](dir_7880a7b12136fba6b737e09baa66bc78.md) **>** [**linux\_socket.c**](linux__socket_8c.md)

[Go to the source code of this file](linux__socket_8c_source.md)



* `#include <stdio.h>`
* `#include <stdlib.h>`
* `#include <stdbool.h>`
* `#include <string.h>`
* `#include <unistd.h>`
* `#include <time.h>`
* `#include <stdarg.h>`
* `#include <net/if.h>`
* `#include <sys/ioctl.h>`
* `#include <sys/socket.h>`
* `#include <linux/can.h>`
* `#include <linux/can/raw.h>`
* `#include <isotp.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  int | [**\_socket**](#variable-_socket)  <br>_SocketCAN file descriptor._  |
|  uint8\_t | [**g\_isotpRecvBuf**](#variable-g_isotprecvbuf)  <br> |
|  uint8\_t | [**g\_isotpSendBuf**](#variable-g_isotpsendbuf)  <br> |
|  [**IsoTpLink**](structIsoTpLink.md) | [**g\_link**](#variable-g_link)  <br>_ISO-TP link instance, allocated statically in RAM._  |














## Public Functions

| Type | Name |
| ---: | :--- |
|  int | [**main**](#function-main) (int argc, char \*\* argv) <br> |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**SEC\_TO\_US**](linux__socket_8c.md#define-sec_to_us) (sec) `((sec) \* 1000000)`<br>_Macro to convert seconds to microseconds._  |
| define  | [**\_CAN\_INTERFACE**](linux__socket_8c.md#define-_can_interface)  `"slcan0"`<br>_CAN interface to use (e.g., "can0", "slcan0", etc.)_  |
| define  | [**\_ISOTP\_BUFSIZE**](linux__socket_8c.md#define-_isotp_bufsize)  `(128)`<br>_Buffer size for ISO-TP messages (must be &gt;= 4095 for full ISO-TP support)/\*#end#\*_  _/._ |
| define  | [**\_ISOTP\_CAN\_ID**](linux__socket_8c.md#define-_isotp_can_id)  `(0x0C1)`<br>_CAN ID to use for sending ISO-TP messages (must match the receiver's expected ID)/\*#end#\*_  _/._ |

## Public Static Attributes Documentation




### variable \_socket 

_SocketCAN file descriptor._ 
```C++
int _socket;
```




<hr>



### variable g\_isotpRecvBuf 

```C++
uint8_t g_isotpRecvBuf[_ISOTP_BUFSIZE];
```



Buffer for receiving ISO-TP messages, allocated statically in RAM 


        

<hr>



### variable g\_isotpSendBuf 

```C++
uint8_t g_isotpSendBuf[_ISOTP_BUFSIZE];
```



Buffer for sending ISO-TP messages, allocated statically in RAM 


        

<hr>



### variable g\_link 

_ISO-TP link instance, allocated statically in RAM._ 
```C++
IsoTpLink g_link;
```




<hr>
## Public Functions Documentation




### function main 

```C++
int main (
    int argc,
    char ** argv
) 
```




<hr>
## Macro Definition Documentation





### define SEC\_TO\_US 

_Macro to convert seconds to microseconds._ 
```C++
#define SEC_TO_US (
    sec
) `((sec) * 1000000)`
```




<hr>



### define \_CAN\_INTERFACE 

_CAN interface to use (e.g., "can0", "slcan0", etc.)_ 
```C++
#define _CAN_INTERFACE `"slcan0"`
```




<hr>



### define \_ISOTP\_BUFSIZE 

_Buffer size for ISO-TP messages (must be &gt;= 4095 for full ISO-TP support)/\*#end#\*_  _/._
```C++
#define _ISOTP_BUFSIZE `(128)`
```




<hr>



### define \_ISOTP\_CAN\_ID 

_CAN ID to use for sending ISO-TP messages (must match the receiver's expected ID)/\*#end#\*_  _/._
```C++
#define _ISOTP_CAN_ID `(0x0C1)`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `examples/linux_socket/linux_socket.c`

