

# Namespace test\_buffer\_limits



[**Namespace List**](namespaces.md) **>** [**test\_buffer\_limits**](namespacetest__buffer__limits.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**test\_receive\_truncates\_payload**](#function-test_receive_truncates_payload) () <br>_Verify receive truncates payload to buffer size._  |
|   | [**test\_send\_in\_progress\_returns\_code**](#function-test_send_in_progress_returns_code) () <br>_Verify in-progress is reported on concurrent send._  |
|   | [**test\_send\_overflow\_returns\_code**](#function-test_send_overflow_returns_code) () <br>_Verify overflow is reported on send._  |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|   | [**\_make\_payload**](#function-_make_payload) (size size) <br>_Build a deterministic payload of a given size._  |
|   | [**\_poll\_until\_receive**](#function-_poll_until_receive) (link link, payload\_size payload\_size, steps steps) <br>_Poll the link until a response is received or steps are exhausted._  |




## Public Functions Documentation




### function test\_receive\_truncates\_payload 

_Verify receive truncates payload to buffer size._ 
```Python
test_buffer_limits::test_receive_truncates_payload () 
```



The returned payload must be capped to the buffer length. 


        

<hr>



### function test\_send\_in\_progress\_returns\_code 

_Verify in-progress is reported on concurrent send._ 
```Python
test_buffer_limits::test_send_in_progress_returns_code () 
```



Second send while active returns ISOTP\_RET\_INPROGRESS. 


        

<hr>



### function test\_send\_overflow\_returns\_code 

_Verify overflow is reported on send._ 
```Python
test_buffer_limits::test_send_overflow_returns_code () 
```



Exceeding the send buffer returns ISOTP\_RET\_OVERFLOW. 


        

<hr>
## Protected Functions Documentation




### function \_make\_payload 

_Build a deterministic payload of a given size._ 
```Python
test_buffer_limits::_make_payload (
    size size
) 
```





**Parameters:**


* `size` Number of bytes to generate. 



**Returns:**

Payload bytes.


Uses a simple modulo pattern for repeatability. 


        

<hr>



### function \_poll\_until\_receive 

_Poll the link until a response is received or steps are exhausted._ 
```Python
test_buffer_limits::_poll_until_receive (
    link link,
    payload_size payload_size,
    steps steps
) 
```





**Parameters:**


* `link` ISO-TP link handle. 
* `payload_size` Expected payload size for receive. 
* `steps` Maximum number of poll iterations. 



**Returns:**

Received payload bytes or None when nothing is received.


Polls the core without advancing time. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `tests/integration/test_buffer_limits.py`

