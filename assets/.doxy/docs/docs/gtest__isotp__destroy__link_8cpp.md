

# File gtest\_isotp\_destroy\_link.cpp



[**FileList**](files.md) **>** [**tests**](dir_59425e443f801f1f2fd8bbe4959a3ccf.md) **>** [**unit**](dir_7711e90abc3d4bd55823d9013a185d04.md) **>** [**gtest\_isotp\_destroy\_link.cpp**](gtest__isotp__destroy__link_8cpp.md)

[Go to the source code of this file](gtest__isotp__destroy__link_8cpp_source.md)

_Unit tests for isotp\_destroy\_link._ [More...](#detailed-description)

* `#include <gtest/gtest.h>`
* `#include "isotp_test_support.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TEST**](#function-test) (IsotpDestroyLink, ClearsState) <br>_Link memory is zeroed after destroy._  |
|   | [**TEST**](#function-test) (IsotpDestroyLink, NullIsNoOp) <br>_Null link is treated as a no-op._  |




























## Detailed Description


Validates link cleanup and null safety. 


    
## Public Functions Documentation




### function TEST 

_Link memory is zeroed after destroy._ 
```C++
TEST (
    IsotpDestroyLink,
    ClearsState
) 
```




<hr>



### function TEST 

_Null link is treated as a no-op._ 
```C++
TEST (
    IsotpDestroyLink,
    NullIsNoOp
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `tests/unit/gtest_isotp_destroy_link.cpp`

