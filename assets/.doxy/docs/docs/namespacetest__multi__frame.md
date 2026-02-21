

# Namespace test\_multi\_frame



[**Namespace List**](namespaces.md) **>** [**test\_multi\_frame**](namespacetest__multi__frame.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**test\_multi\_frame\_boundary\_sizes**](#function-test_multi_frame_boundary_sizes) (size size) <br>_Verify multi-frame behavior around boundary sizes._  |
|   | [**test\_multi\_frame\_bs\_one\_stmin**](#function-test_multi_frame_bs_one_stmin) () <br>_Verify block size 1 with STmin pacing._  |
|   | [**test\_multi\_frame\_bs\_unlimited\_stmin**](#function-test_multi_frame_bs_unlimited_stmin) () <br>_Verify unlimited block size with STmin pacing._  |
|   | [**test\_multi\_frame\_max\_payload**](#function-test_multi_frame_max_payload) () <br>_Verify maximum payload size roundtrip._  |
|   | [**test\_multi\_frame\_roundtrip**](#function-test_multi_frame_roundtrip) () <br>_Verify multi-frame roundtrip delivery._  |
|   | [**test\_multi\_frame\_timeout\_n\_bs**](#function-test_multi_frame_timeout_n_bs) () <br>_Verify N\_Bs timeout during multi-frame send._  |
|   | [**test\_multi\_frame\_timeout\_n\_cr**](#function-test_multi_frame_timeout_n_cr) () <br>_Verify N\_Cr timeout during multi-frame receive._  |
|   | [**test\_multi\_frame\_unexpected\_cf**](#function-test_multi_frame_unexpected_cf) () <br>_Verify unexpected CF is reported._  |
|   | [**test\_multi\_frame\_wrong\_sn**](#function-test_multi_frame_wrong_sn) () <br>_Verify wrong sequence number is reported._  |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|   | [**\_cf\_frame**](#function-_cf_frame) (sn sn, payload payload) <br>_Build a Consecutive Frame (CF) with a given sequence number._  |
|   | [**\_ff\_frame**](#function-_ff_frame) (payload\_size payload\_size, payload payload) <br>_Build a First Frame (FF) for an ISO-TP transfer._  |
|   | [**\_make\_payload**](#function-_make_payload) (size size) <br>_Build a deterministic payload of a given size._  |
|   | [**\_poll\_until\_receive**](#function-_poll_until_receive) (link link, payload\_size payload\_size, steps steps, advance\_ms advance\_ms=0) <br>_Poll the link until a response is received or steps are exhausted._  |




## Public Functions Documentation




### function test\_multi\_frame\_boundary\_sizes 

_Verify multi-frame behavior around boundary sizes._ 
```Python
test_multi_frame::test_multi_frame_boundary_sizes (
    size size
) 
```





**Parameters:**


* `size` Payload size under test.

Exercises sizes near the SF/FF boundary. 


        

<hr>



### function test\_multi\_frame\_bs\_one\_stmin 

_Verify block size 1 with STmin pacing._ 
```Python
test_multi_frame::test_multi_frame_bs_one_stmin () 
```



Advances time between frames to satisfy STmin. 


        

<hr>



### function test\_multi\_frame\_bs\_unlimited\_stmin 

_Verify unlimited block size with STmin pacing._ 
```Python
test_multi_frame::test_multi_frame_bs_unlimited_stmin () 
```



Exercises BS=0 with STmin delay. 


        

<hr>



### function test\_multi\_frame\_max\_payload 

_Verify maximum payload size roundtrip._ 
```Python
test_multi_frame::test_multi_frame_max_payload () 
```



Uses a large payload to stress segmentation and reassembly. 


        

<hr>



### function test\_multi\_frame\_roundtrip 

_Verify multi-frame roundtrip delivery._ 
```Python
test_multi_frame::test_multi_frame_roundtrip () 
```



Ensures payload integrity across segmented transfer. 


        

<hr>



### function test\_multi\_frame\_timeout\_n\_bs 

_Verify N\_Bs timeout during multi-frame send._ 
```Python
test_multi_frame::test_multi_frame_timeout_n_bs () 
```



Disables FC to force sender block-size timeout. 


        

<hr>



### function test\_multi\_frame\_timeout\_n\_cr 

_Verify N\_Cr timeout during multi-frame receive._ 
```Python
test_multi_frame::test_multi_frame_timeout_n_cr () 
```



Drops incoming frames to force receiver timeout. 


        

<hr>



### function test\_multi\_frame\_unexpected\_cf 

_Verify unexpected CF is reported._ 
```Python
test_multi_frame::test_multi_frame_unexpected_cf () 
```



Injects a CF when receiver is idle. 


        

<hr>



### function test\_multi\_frame\_wrong\_sn 

_Verify wrong sequence number is reported._ 
```Python
test_multi_frame::test_multi_frame_wrong_sn () 
```



Injects a CF with unexpected SN after FF. 


        

<hr>
## Protected Functions Documentation




### function \_cf\_frame 

_Build a Consecutive Frame (CF) with a given sequence number._ 
```Python
test_multi_frame::_cf_frame (
    sn sn,
    payload payload
) 
```





**Parameters:**


* `sn` Sequence number (0-15). 
* `payload` Payload bytes to embed in CF. 



**Returns:**

CAN frame bytes representing the CF.


Encodes the 0x20 PCI with SN. 


        

<hr>



### function \_ff\_frame 

_Build a First Frame (FF) for an ISO-TP transfer._ 
```Python
test_multi_frame::_ff_frame (
    payload_size payload_size,
    payload payload
) 
```





**Parameters:**


* `payload_size` Total payload size. 
* `payload` Payload bytes to embed in FF. 



**Returns:**

CAN frame bytes representing the FF.


Encodes the 0x10 PCI with 12-bit length. 


        

<hr>



### function \_make\_payload 

_Build a deterministic payload of a given size._ 
```Python
test_multi_frame::_make_payload (
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
test_multi_frame::_poll_until_receive (
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


Optionally advances the mock time between polls. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `tests/integration/test_multi_frame.py`

