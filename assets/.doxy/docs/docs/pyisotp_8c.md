

# File pyisotp.c



[**FileList**](files.md) **>** [**examples**](dir_d28a4824dc47e487b107a5db32ef43c4.md) **>** [**python\_api**](dir_181ea403426f4b233011860116dcb1b5.md) **>** [**pyisotp.c**](pyisotp_8c.md)

[Go to the source code of this file](pyisotp_8c_source.md)



* `#include <Python.h>`
* `#include "isotp.h"`
* `#include "can_driver.h"`
* `#include "mock_can.h"`
* `#include "mock_time.h"`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  PyMethodDef | [**PyIsoTpMethods**](#variable-pyisotpmethods)   = `/* multi line expression */`<br> |
|  struct PyModuleDef | [**pyisotp\_module**](#variable-pyisotp_module)   = `/* multi line expression */`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  PyMODINIT\_FUNC | [**PyInit\_pyisotp**](#function-pyinit_pyisotp) (void) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  PyObject \* | [**py\_get\_last\_protocol\_result**](#function-py_get_last_protocol_result) (PyObject \* self, PyObject \* args) <br>_Returns the last protocol result for the sender side._  |
|  PyObject \* | [**py\_get\_last\_receive\_protocol\_result**](#function-py_get_last_receive_protocol_result) (PyObject \* self, PyObject \* args) <br>_Returns the last protocol result for the receiver side._  |
|  PyObject \* | [**py\_inject\_can**](#function-py_inject_can) (PyObject \* self, PyObject \* args) <br>_Injects a raw CAN frame into the ISO-TP link for testing purposes._  |
|  PyObject \* | [**py\_mock\_disable\_fc**](#function-py_mock_disable_fc) (PyObject \* self, PyObject \* args) <br>_Enables or disables FlowControl frames in the mock CAN driver._  |
|  PyObject \* | [**py\_mock\_enable\_drop**](#function-py_mock_enable_drop) (PyObject \* self, PyObject \* args) <br>_Enables or disables CAN frame dropping in the mock CAN driver._  |
|  PyObject \* | [**py\_set\_fc\_params**](#function-py_set_fc_params) (PyObject \* self, PyObject \* args) <br>_Sets the FlowControl parameters for the ISO-TP link._  |
|  PyObject \* | [**py\_set\_timeouts**](#function-py_set_timeouts) (PyObject \* self, PyObject \* args) <br>_Sets the timeouts for the ISO-TP link._  |
|  PyObject \* | [**py\_time\_advance**](#function-py_time_advance) (PyObject \* self, PyObject \* args) <br>_Advances the virtual time in the mock time implementation by a specified delta._  |
|  PyObject \* | [**py\_time\_reset**](#function-py_time_reset) (PyObject \* self, PyObject \* args) <br>_Resets the virtual time in the mock time implementation to zero._  |
|  PyObject \* | [**py\_time\_set**](#function-py_time_set) (PyObject \* self, PyObject \* args) <br>_Sets the virtual time in the mock time implementation to a specific value._  |
|  void | [**pyisotp\_capsule\_destructor**](#function-pyisotp_capsule_destructor) (PyObject \* capsule) <br>_Releases an ISO-TP link capsule and its allocated buffers._  |
|  PyObject \* | [**pyisotp\_init**](#function-pyisotp_init) (PyObject \* self, PyObject \* args) <br>_Initializes an ISO-TP link and returns a capsule containing the link pointer to Python._  |
|  PyObject \* | [**pyisotp\_poll**](#function-pyisotp_poll) (PyObject \* self, PyObject \* args) <br>_Polls an ISO-TP link for timeouts and state updates._  |
|  PyObject \* | [**pyisotp\_receive**](#function-pyisotp_receive) (PyObject \* self, PyObject \* args) <br>_Receives data from an ISO-TP link._  |
|  PyObject \* | [**pyisotp\_send**](#function-pyisotp_send) (PyObject \* self, PyObject \* args) <br>_Sends data over an ISO-TP link._  |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**PY\_SSIZE\_T\_CLEAN**](pyisotp_8c.md#define-py_ssize_t_clean)  <br> |

## Public Static Attributes Documentation




### variable PyIsoTpMethods 

```C++
PyMethodDef PyIsoTpMethods[];
```




<hr>



### variable pyisotp\_module 

```C++
struct PyModuleDef pyisotp_module;
```




<hr>
## Public Functions Documentation




### function PyInit\_pyisotp 

```C++
PyMODINIT_FUNC PyInit_pyisotp (
    void
) 
```




<hr>
## Public Static Functions Documentation




### function py\_get\_last\_protocol\_result 

_Returns the last protocol result for the sender side._ 
```C++
static PyObject * py_get_last_protocol_result (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python (expected to be empty for this function) 



**Returns:**

PyObject\* - Python object representing the result of the operation, typically an integer status code 





        

<hr>



### function py\_get\_last\_receive\_protocol\_result 

_Returns the last protocol result for the receiver side._ 
```C++
static PyObject * py_get_last_receive_protocol_result (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python (expected to be empty for this function) 



**Returns:**

PyObject\* - Python object representing the result of the operation, typically an integer status code 





        

<hr>



### function py\_inject\_can 

_Injects a raw CAN frame into the ISO-TP link for testing purposes._ 
```C++
static PyObject * py_inject_can (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python, expected to contain:
  * py\_link (PyObject\*): Capsule containing the [**IsoTpLink**](structIsoTpLink.md) pointer
  * data (bytes): Raw CAN frame data to inject (must be 2..8 bytes) 





**Returns:**

PyObject\* - Python object representing the result of the operation, typically None 





        

<hr>



### function py\_mock\_disable\_fc 

_Enables or disables FlowControl frames in the mock CAN driver._ 
```C++
static PyObject * py_mock_disable_fc (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python, expected to contain:
  * enable (int): Flag to enable or disable FlowControl frames 





**Returns:**

PyObject\* - Python object representing the result of the operation, typically None 





        

<hr>



### function py\_mock\_enable\_drop 

_Enables or disables CAN frame dropping in the mock CAN driver._ 
```C++
static PyObject * py_mock_enable_drop (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python, expected to contain:
  * enable (int): Flag to enable or disable CAN frame dropping 





**Returns:**

PyObject\* - Python object representing the result of the operation, typically None 





        

<hr>



### function py\_set\_fc\_params 

_Sets the FlowControl parameters for the ISO-TP link._ 
```C++
static PyObject * py_set_fc_params (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python, expected to contain:
  * py\_link (PyObject\*): Capsule containing the [**IsoTpLink**](structIsoTpLink.md) pointer




* block\_size (uint32\_t): Block size for FlowControl (0 means unlimited)




st\_min\_ms (uint32\_t): STmin in milliseconds 

**Returns:**

PyObject\* - Python object representing the result of the operation, typically None 





        

<hr>



### function py\_set\_timeouts 

_Sets the timeouts for the ISO-TP link._ 
```C++
static PyObject * py_set_timeouts (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python, expected to contain:
  * py\_link (PyObject\*): Capsule containing the [**IsoTpLink**](structIsoTpLink.md) pointer




* n\_bs\_ms (uint32\_t): N\_Bs timeout in milliseconds




n\_cr\_ms (uint32\_t): N\_Cr timeout in milliseconds 

**Returns:**

PyObject\* - Python object representing the result of the operation, typically None 





        

<hr>



### function py\_time\_advance 

_Advances the virtual time in the mock time implementation by a specified delta._ 
```C++
static PyObject * py_time_advance (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python, expected to contain:
  * delta (uint32\_t): The number of milliseconds to advance the virtual time 





**Returns:**

PyObject\* - Python object representing the result of the operation, typically None 





        

<hr>



### function py\_time\_reset 

_Resets the virtual time in the mock time implementation to zero._ 
```C++
static PyObject * py_time_reset (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python (expected to be empty for this function) 



**Returns:**

PyObject\* - Python object representing the result of the operation, typically None 





        

<hr>



### function py\_time\_set 

_Sets the virtual time in the mock time implementation to a specific value._ 
```C++
static PyObject * py_time_set (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python, expected to contain:
  * value (uint32\_t): The value to set the virtual time to, in milliseconds 





**Returns:**

PyObject\* - Python object representing the result of the operation, typically None 





        

<hr>



### function pyisotp\_capsule\_destructor 

_Releases an ISO-TP link capsule and its allocated buffers._ 
```C++
static void pyisotp_capsule_destructor (
    PyObject * capsule
) 
```




<hr>



### function pyisotp\_init 

_Initializes an ISO-TP link and returns a capsule containing the link pointer to Python._ 
```C++
static PyObject * pyisotp_init (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python, expected to contain:
  * sendid (uint32\_t): CAN ID to send on 





**Returns:**

PyObject\* - Python object representing the result of the operation, typically a capsule containing the [**IsoTpLink**](structIsoTpLink.md) pointer or an error indicator 





        

<hr>



### function pyisotp\_poll 

_Polls an ISO-TP link for timeouts and state updates._ 
```C++
static PyObject * pyisotp_poll (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python, expected to contain:
  * py\_link (PyObject\*): Capsule containing the [**IsoTpLink**](structIsoTpLink.md) pointer 





**Returns:**

PyObject\* - Python object representing the result of the operation, typically None 





        

<hr>



### function pyisotp\_receive 

_Receives data from an ISO-TP link._ 
```C++
static PyObject * pyisotp_receive (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python, expected to contain:
  * py\_link (PyObject\*): Capsule containing the [**IsoTpLink**](structIsoTpLink.md) pointer
  * bufsize (Py\_ssize\_t): Size of the buffer to receive data into 





**Returns:**

PyObject\* - Python object representing the result of the operation, typically bytes containing received data or None if no data 





        

<hr>



### function pyisotp\_send 

_Sends data over an ISO-TP link._ 
```C++
static PyObject * pyisotp_send (
    PyObject * self,
    PyObject * args
) 
```





**Parameters:**


* `self` - Pointer to the module object (unused in this context) 
* `args` - Tuple of arguments passed from Python, expected to contain:
  * py\_link (PyObject\*): Capsule containing the [**IsoTpLink**](structIsoTpLink.md) pointer
  * data (bytes): Data to send 





**Returns:**

PyObject\* - Python object representing the result of the operation, typically an integer status code 





        

<hr>
## Macro Definition Documentation





### define PY\_SSIZE\_T\_CLEAN 

```C++
#define PY_SSIZE_T_CLEAN 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `examples/python_api/pyisotp.c`

