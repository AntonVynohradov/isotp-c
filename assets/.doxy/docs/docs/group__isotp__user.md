

# Group isotp\_user



[**Modules**](modules.md) **>** [**isotp\_user**](group__isotp__user.md)



_User-implemented callbacks and platform hooks._ [More...](#detailed-description)






































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**isotp\_user\_debug**](#function-isotp_user_debug) (const char \* message, ...) <br>_User implemented, print debug message._  |
|  uint32\_t | [**isotp\_user\_get\_us**](#function-isotp_user_get_us) (void) <br>_User implemented, return a monotonically increasing timebase in microseconds._  |
|  int | [**isotp\_user\_send\_can**](#function-isotp_user_send_can) (const uint32\_t arbitration\_id, const uint8\_t \* data, const uint8\_t size) <br>_User implemented, send CAN message._  |




























## Detailed Description


This group defines the platform abstraction points required by the ISO-TP core. Applications must provide these functions to integrate with their CAN driver, timing source, and optional debug output.




**
**


* Debug output hook: [**isotp\_user\_debug**](group__isotp__user.md#function-isotp_user_debug). 
* CAN transmit hook: [**isotp\_user\_send\_can**](group__isotp__user.md#function-isotp_user_send_can) (optionally with ISO\_TP\_USER\_SEND\_CAN\_ARG). 
* Time source hook: [**isotp\_user\_get\_us**](group__isotp__user.md#function-isotp_user_get_us). 







    
## Public Functions Documentation




### function isotp\_user\_debug 

_User implemented, print debug message._ 
```
void isotp_user_debug (
    const char * message,
    ...
) 
```





**Parameters:**


* `message` Debug message format string.

User implemented, print debug message.




**Parameters:**


* `message` The message to print.

User implemented, print debug message. 


        

<hr>



### function isotp\_user\_get\_us 

_User implemented, return a monotonically increasing timebase in microseconds._ 
```
uint32_t isotp_user_get_us (
    void
) 
```



This function must provide a free-running timestamp in microseconds that increases monotonically over time. The ISO-TP core uses this value for timeout and delay calculations by performing arithmetic on the returned timestamp (for example, adding a timeout interval to it).


Natural wrap-around of the 32-bit counter is allowed and will be handled by the ISO-TP core, but the value must not jump backwards except for such wrap-around.




**Returns:**

Current timebase value in microseconds.


User implemented, return a monotonically increasing timebase in microseconds.




**Returns:**

uint32\_t Current time in microseconds


User implemented, return a monotonically increasing timebase in microseconds. 


        

<hr>



### function isotp\_user\_send\_can 

_User implemented, send CAN message._ 
```
int isotp_user_send_can (
    const uint32_t arbitration_id,
    const uint8_t * data,
    const uint8_t size
) 
```



Should return ISOTP\_RET\_OK on success. 

**Parameters:**


* `arbitration_id` CAN message arbitration ID. 
* `data` Pointer to message data buffer. 
* `size` Size of message data in bytes. 



**Returns:**

ISOTP\_RET\_OK on success, ISOTP\_RET\_NOSPACE if transfer should be retried later, or ISOTP\_RET\_ERROR on failure.


User implemented, send CAN message.




**Parameters:**


* `arbitration_id` The CAN ID to send with. 
* `data` Pointer to the data buffer to send. 
* `size` Size of the data buffer (max 8 bytes). 



**Returns:**

int ISOTP\_RET\_OK on success, ISOTP\_RET\_ERROR on failure.


User implemented, send CAN message. 


        

<hr>

------------------------------


