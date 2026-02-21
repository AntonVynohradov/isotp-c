

# Namespace test\_cantp



[**Namespace List**](namespaces.md) **>** [**test\_cantp**](namespacetest__cantp.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**test\_cantp\_stmin\_and\_bs**](#function-test_cantp_stmin_and_bs) () <br>_Verify STmin and block size pacing._  |
|   | [**test\_cantp\_timeout\_n\_bs**](#function-test_cantp_timeout_n_bs) () <br>_Verify N\_Bs timeout is detected._  |
|   | [**test\_cantp\_timeout\_n\_cr**](#function-test_cantp_timeout_n_cr) () <br>_Verify N\_Cr timeout is detected._  |
|   | [**test\_cantp\_unexpected\_pdu**](#function-test_cantp_unexpected_pdu) () <br>_Verify unexpected PDU is reported._  |
|   | [**test\_wait\_frame\_overrun**](#function-test_wait_frame_overrun) () <br>_Verify WAIT frame overrun is reported._  |
|   | [**test\_wait\_frame\_then\_continue**](#function-test_wait_frame_then_continue) () <br>_Verify receiver resumes after WAIT then CONTINUE FC._  |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|   | [**\_fc\_frame**](#function-_fc_frame) (flow\_status flow\_status, block\_size block\_size, st\_min st\_min) <br>_Build a Flow Control (FC) frame._  |
|   | [**\_make\_payload**](#function-_make_payload) (size size) <br>_Build a deterministic payload of a given size._  |
|   | [**\_poll\_until\_receive**](#function-_poll_until_receive) (link link, payload\_size payload\_size, steps steps, advance\_ms advance\_ms=0) <br>_Poll the link until a response is received or steps are exhausted._  |




## Public Functions Documentation




### function test\_cantp\_stmin\_and\_bs 

_Verify STmin and block size pacing._ 
```Python
test_cantp::test_cantp_stmin_and_bs () 
```



Advances time to satisfy STmin between frames. 


        

<hr>



### function test\_cantp\_timeout\_n\_bs 

_Verify N\_Bs timeout is detected._ 
```Python
test_cantp::test_cantp_timeout_n_bs () 
```



Disables FC to force a sender block-size timeout. 


        

<hr>



### function test\_cantp\_timeout\_n\_cr 

_Verify N\_Cr timeout is detected._ 
```Python
test_cantp::test_cantp_timeout_n_cr () 
```



Drops incoming frames to force a receiver timeout. 


        

<hr>



### function test\_cantp\_unexpected\_pdu 

_Verify unexpected PDU is reported._ 
```Python
test_cantp::test_cantp_unexpected_pdu () 
```



Injects a CF when receiver is idle. 


        

<hr>



### function test\_wait\_frame\_overrun 

_Verify WAIT frame overrun is reported._ 
```Python
test_cantp::test_wait_frame_overrun () 
```



Repeated WAIT frames exceed the limit and trigger WFT\_OVRN. 


        

<hr>



### function test\_wait\_frame\_then\_continue 

_Verify receiver resumes after WAIT then CONTINUE FC._ 
```Python
test_cantp::test_wait_frame_then_continue () 
```



WAIT should pause transmission until CONTINUE arrives. 


        

<hr>
## Protected Functions Documentation




### function \_fc\_frame 

_Build a Flow Control (FC) frame._ 
```Python
test_cantp::_fc_frame (
    flow_status flow_status,
    block_size block_size,
    st_min st_min
) 
```





**Parameters:**


* `flow_status` Flow status nibble. 
* `block_size` Block size byte. 
* `st_min` STmin byte. 



**Returns:**

CAN frame bytes representing the FC.


Encodes the 0x30 PCI for flow control. 


        

<hr>



### function \_make\_payload 

_Build a deterministic payload of a given size._ 
```Python
test_cantp::_make_payload (
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
test_cantp::_poll_until_receive (
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
The documentation for this class was generated from the following file `tests/integration/test_cantp.py`

