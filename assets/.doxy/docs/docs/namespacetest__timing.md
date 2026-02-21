

# Namespace test\_timing



[**Namespace List**](namespaces.md) **>** [**test\_timing**](namespacetest__timing.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**test\_block\_size\_one\_roundtrip**](#function-test_block_size_one_roundtrip) () <br>_Verify block size one roundtrip behavior._  |
|   | [**test\_st\_min\_enforced**](#function-test_st_min_enforced) () <br>_Verify STmin delay is enforced._  |
|   | [**test\_timeout\_n\_ar\_placeholder**](#function-test_timeout_n_ar_placeholder) () <br>_Placeholder for N\_Ar timeout support._  |
|   | [**test\_timeout\_n\_as\_placeholder**](#function-test_timeout_n_as_placeholder) () <br>_Placeholder for N\_As timeout support._  |
|   | [**test\_timeout\_n\_br\_placeholder**](#function-test_timeout_n_br_placeholder) () <br>_Placeholder for N\_Br timeout support._  |
|   | [**test\_timeout\_n\_bs**](#function-test_timeout_n_bs) () <br>_Verify N\_Bs timeout handling._  |
|   | [**test\_timeout\_n\_cr**](#function-test_timeout_n_cr) () <br>_Verify N\_Cr timeout handling._  |
|   | [**test\_timeout\_n\_cs\_placeholder**](#function-test_timeout_n_cs_placeholder) () <br>_Placeholder for N\_Cs timeout support._  |




























## Public Functions Documentation




### function test\_block\_size\_one\_roundtrip 

_Verify block size one roundtrip behavior._ 
```Python
test_timing::test_block_size_one_roundtrip () 
```



Uses BS=1 to enforce FC after each CF. 


        

<hr>



### function test\_st\_min\_enforced 

_Verify STmin delay is enforced._ 
```Python
test_timing::test_st_min_enforced () 
```



Ensures receive does not complete before STmin elapses. 


        

<hr>



### function test\_timeout\_n\_ar\_placeholder 

_Placeholder for N\_Ar timeout support._ 
```Python
test_timing::test_timeout_n_ar_placeholder () 
```





**Note:**

Marked xfail until core enforces N\_Ar. 





        

<hr>



### function test\_timeout\_n\_as\_placeholder 

_Placeholder for N\_As timeout support._ 
```Python
test_timing::test_timeout_n_as_placeholder () 
```





**Note:**

Marked xfail until core enforces N\_As. 





        

<hr>



### function test\_timeout\_n\_br\_placeholder 

_Placeholder for N\_Br timeout support._ 
```Python
test_timing::test_timeout_n_br_placeholder () 
```





**Note:**

Marked xfail until core enforces N\_Br. 





        

<hr>



### function test\_timeout\_n\_bs 

_Verify N\_Bs timeout handling._ 
```Python
test_timing::test_timeout_n_bs () 
```



Disables FC to force sender timeout. 


        

<hr>



### function test\_timeout\_n\_cr 

_Verify N\_Cr timeout handling._ 
```Python
test_timing::test_timeout_n_cr () 
```



Drops incoming frames to force receiver timeout. 


        

<hr>



### function test\_timeout\_n\_cs\_placeholder 

_Placeholder for N\_Cs timeout support._ 
```Python
test_timing::test_timeout_n_cs_placeholder () 
```





**Note:**

Marked xfail until core enforces N\_Cs. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `tests/integration/test_timing.py`

