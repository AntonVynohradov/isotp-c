

# File isotp\_user.h



[**FileList**](files.md) **>** [**Inc**](dir_f93dfce691d792349f1c14ede440e2e6.md) **>** [**isotp\_user.h**](isotp__user_8h.md)

[Go to the source code of this file](isotp__user_8h_source.md)



* `#include <stdint.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**isotp\_user\_debug**](#function-isotp_user_debug) (const char \* message, ...) <br>_User implemented, print debug message._  |
|  uint32\_t | [**isotp\_user\_get\_us**](#function-isotp_user_get_us) (void) <br>_User implemented, gets the amount of time passed since the last call in microseconds._  |
|  int | [**isotp\_user\_send\_can**](#function-isotp_user_send_can) (const uint32\_t arbitration\_id, const uint8\_t \* data, const uint8\_t size) <br>_User implemented, send CAN message (should return ISOTP\_RET\_OK when success)_  |




























## Public Functions Documentation




### function isotp\_user\_debug 

_User implemented, print debug message._ 
```C++
void isotp_user_debug (
    const char * message,
    ...
) 
```





**Parameters:**


* `message` - Debug message format string 



**Returns:**

None 





        

<hr>



### function isotp\_user\_get\_us 

_User implemented, gets the amount of time passed since the last call in microseconds._ 
```C++
uint32_t isotp_user_get_us (
    void
) 
```





**Parameters:**


* `None` 



**Returns:**

Time elapsed in microseconds 





        

<hr>



### function isotp\_user\_send\_can 

_User implemented, send CAN message (should return ISOTP\_RET\_OK when success)_ 
```C++
int isotp_user_send_can (
    const uint32_t arbitration_id,
    const uint8_t * data,
    const uint8_t size
) 
```





**Parameters:**


* `arbitration_id` - CAN message arbitration ID 
* `data` - Pointer to message data buffer 
* `size` - Size of message data in bytes 



**Returns:**

ISOTP\_RET\_OK on success, ISOTP\_RET\_NOSPACE if transfer should be retried later, or ISOTP\_RET\_ERROR on failure 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `Inc/isotp_user.h`

