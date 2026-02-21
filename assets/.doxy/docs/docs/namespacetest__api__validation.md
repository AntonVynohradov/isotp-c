

# Namespace test\_api\_validation



[**Namespace List**](namespaces.md) **>** [**test\_api\_validation**](namespacetest__api__validation.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**test\_init\_rejects\_zero\_buffers**](#function-test_init_rejects_zero_buffers) () <br>_Verify init rejects zero-length buffers._  |
|   | [**test\_receive\_rejects\_invalid\_bufsize**](#function-test_receive_rejects_invalid_bufsize) () <br>_Verify receive rejects invalid buffer sizes._  |
|   | [**test\_set\_fc\_params\_rejects\_large\_block\_size**](#function-test_set_fc_params_rejects_large_block_size) () <br>_Verify flow control params reject oversized block size._  |




























## Public Functions Documentation




### function test\_init\_rejects\_zero\_buffers 

_Verify init rejects zero-length buffers._ 
```Python
test_api_validation::test_init_rejects_zero_buffers () 
```



Validates both send and receive buffer size checks. 


        

<hr>



### function test\_receive\_rejects\_invalid\_bufsize 

_Verify receive rejects invalid buffer sizes._ 
```Python
test_api_validation::test_receive_rejects_invalid_bufsize () 
```



Ensures zero length is rejected by the binding. 


        

<hr>



### function test\_set\_fc\_params\_rejects\_large\_block\_size 

_Verify flow control params reject oversized block size._ 
```Python
test_api_validation::test_set_fc_params_rejects_large_block_size () 
```



Block size values above 255 must be rejected. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `tests/integration/test_api_validation.py`

