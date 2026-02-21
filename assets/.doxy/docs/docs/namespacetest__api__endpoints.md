

# Namespace test\_api\_endpoints



[**Namespace List**](namespaces.md) **>** [**test\_api\_endpoints**](namespacetest__api__endpoints.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**test\_api\_endpoints\_inject\_unexpected\_cf**](#function-test_api_endpoints_inject_unexpected_cf) () <br>_Validate unexpected CF injection handling._  |
|   | [**test\_api\_endpoints\_inject\_wrong\_sn**](#function-test_api_endpoints_inject_wrong_sn) () <br>_Validate wrong sequence number handling._  |
|   | [**test\_api\_endpoints\_return\_codes**](#function-test_api_endpoints_return_codes) () <br>_Validate return codes for overflow and in-progress sends._  |
|   | [**test\_api\_endpoints\_smoke**](#function-test_api_endpoints_smoke) () <br>_Verify basic API calls and protocol result handling._  |
|   | [**test\_api\_endpoints\_smoke\_multi\_frame**](#function-test_api_endpoints_smoke_multi_frame) () <br>_Verify multi-frame send/receive path._  |
|   | [**test\_api\_endpoints\_validation\_errors**](#function-test_api_endpoints_validation_errors) () <br>_Validate input checks and error signaling in API._  |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|   | [**\_cf\_frame**](#function-_cf_frame) (sn sn, payload payload) <br>_Build a Consecutive Frame (CF) with a given sequence number._  |
|   | [**\_ff\_frame**](#function-_ff_frame) (payload\_size payload\_size, payload payload) <br>_Build a First Frame (FF) for an ISO-TP multi-frame transfer._  |
|   | [**\_make\_payload**](#function-_make_payload) (size size) <br>_Build a deterministic payload of a given size._  |
|   | [**\_poll\_until\_receive**](#function-_poll_until_receive) (link link, payload\_size payload\_size, steps steps, advance\_ms advance\_ms=0) <br>_Poll the link until a response is received or steps are exhausted._  |




## Public Functions Documentation




### function test\_api\_endpoints\_inject\_unexpected\_cf 

_Validate unexpected CF injection handling._ 
```Python
test_api_endpoints::test_api_endpoints_inject_unexpected_cf () 
```




<hr>



### function test\_api\_endpoints\_inject\_wrong\_sn 

_Validate wrong sequence number handling._ 
```Python
test_api_endpoints::test_api_endpoints_inject_wrong_sn () 
```




<hr>



### function test\_api\_endpoints\_return\_codes 

_Validate return codes for overflow and in-progress sends._ 
```Python
test_api_endpoints::test_api_endpoints_return_codes () 
```




<hr>



### function test\_api\_endpoints\_smoke 

_Verify basic API calls and protocol result handling._ 
```Python
test_api_endpoints::test_api_endpoints_smoke () 
```




<hr>



### function test\_api\_endpoints\_smoke\_multi\_frame 

_Verify multi-frame send/receive path._ 
```Python
test_api_endpoints::test_api_endpoints_smoke_multi_frame () 
```




<hr>



### function test\_api\_endpoints\_validation\_errors 

_Validate input checks and error signaling in API._ 
```Python
test_api_endpoints::test_api_endpoints_validation_errors () 
```




<hr>
## Protected Functions Documentation




### function \_cf\_frame 

_Build a Consecutive Frame (CF) with a given sequence number._ 
```Python
test_api_endpoints::_cf_frame (
    sn sn,
    payload payload
) 
```





**Parameters:**


* `sn` Sequence number (0-15). 
* `payload` Payload bytes to embed in CF. 



**Returns:**

CAN frame bytes representing the CF. 





        

<hr>



### function \_ff\_frame 

_Build a First Frame (FF) for an ISO-TP multi-frame transfer._ 
```Python
test_api_endpoints::_ff_frame (
    payload_size payload_size,
    payload payload
) 
```





**Parameters:**


* `payload_size` Total payload size. 
* `payload` Payload bytes to embed in FF. 



**Returns:**

CAN frame bytes representing the FF. 





        

<hr>



### function \_make\_payload 

_Build a deterministic payload of a given size._ 
```Python
test_api_endpoints::_make_payload (
    size size
) 
```





**Parameters:**


* `size` Number of bytes to generate. 



**Returns:**

Payload bytes. 





        

<hr>



### function \_poll\_until\_receive 

_Poll the link until a response is received or steps are exhausted._ 
```Python
test_api_endpoints::_poll_until_receive (
    link link,
    payload_size payload_size,
    steps steps,
    advance_ms advance_ms=0
) 
```





**Parameters:**


* `link` ISO-TP link handle. 
* `payload_size` Expected payload size for receive. 
* `steps` Maximum number of poll iterations. 
* `advance_ms` Optional time advance per step in milliseconds. 



**Returns:**

Received payload bytes or None when nothing is received. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `tests/integration/test_api_endpoints.py`

