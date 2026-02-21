

# Namespace test\_flow\_control



[**Namespace List**](namespaces.md) **>** [**test\_flow\_control**](namespacetest__flow__control.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**test\_flow\_control\_overflow\_sets\_protocol\_result**](#function-test_flow_control_overflow_sets_protocol_result) () <br>_Verify overflow FC sets protocol result._  |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|   | [**\_fc\_frame**](#function-_fc_frame) (flow\_status flow\_status, block\_size block\_size, st\_min st\_min) <br>_Build a Flow Control (FC) frame._  |
|   | [**\_make\_payload**](#function-_make_payload) (size size) <br>_Build a deterministic payload of a given size._  |




## Public Functions Documentation




### function test\_flow\_control\_overflow\_sets\_protocol\_result 

_Verify overflow FC sets protocol result._ 
```Python
test_flow_control::test_flow_control_overflow_sets_protocol_result () 
```



Injects PCI\_FLOW\_STATUS\_OVERFLOW and checks protocol state. 


        

<hr>
## Protected Functions Documentation




### function \_fc\_frame 

_Build a Flow Control (FC) frame._ 
```Python
test_flow_control::_fc_frame (
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
test_flow_control::_make_payload (
    size size
) 
```





**Parameters:**


* `size` Number of bytes to generate. 



**Returns:**

Payload bytes.


Uses a simple modulo pattern for repeatability. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `tests/integration/test_flow_control.py`

