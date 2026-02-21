

# Struct can\_driver\_t



[**ClassList**](annotated.md) **>** [**can\_driver\_t**](structcan__driver__t.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  int(\* | [**receive**](#variable-receive)  <br> |
|  int(\* | [**send**](#variable-send)  <br> |












































## Public Attributes Documentation




### variable receive 

```C++
int(* can_driver_t::receive) (uint32_t *id, uint8_t *data, uint8_t *len);
```




<hr>



### variable send 

```C++
int(* can_driver_t::send) (uint32_t id, const uint8_t *data, uint8_t len);
```




<hr>

------------------------------
The documentation for this class was generated from the following file `examples/python_api/inc/can_driver.h`

