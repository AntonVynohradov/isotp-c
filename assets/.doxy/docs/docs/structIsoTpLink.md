

# Struct IsoTpLink



[**ClassList**](annotated.md) **>** [**IsoTpLink**](structIsoTpLink.md)



_Link state for a single ISO-TP connection._ [More...](#detailed-description)

* `#include <isotp.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  uint8\_t | [**param\_block\_size**](#variable-param_block_size)  <br> |
|  uint32\_t | [**param\_n\_bs\_us**](#variable-param_n_bs_us)  <br> |
|  uint32\_t | [**param\_n\_cr\_us**](#variable-param_n_cr_us)  <br> |
|  uint32\_t | [**param\_st\_min\_us**](#variable-param_st_min_us)  <br> |
|  uint32\_t | [**receive\_arbitration\_id**](#variable-receive_arbitration_id)  <br> |
|  uint8\_t | [**receive\_bs\_count**](#variable-receive_bs_count)  <br> |
|  uint32\_t | [**receive\_buf\_size**](#variable-receive_buf_size)  <br> |
|  uint8\_t \* | [**receive\_buffer**](#variable-receive_buffer)  <br> |
|  uint32\_t | [**receive\_offset**](#variable-receive_offset)  <br> |
|  int | [**receive\_protocol\_result**](#variable-receive_protocol_result)  <br> |
|  uint32\_t | [**receive\_size**](#variable-receive_size)  <br> |
|  uint8\_t | [**receive\_sn**](#variable-receive_sn)  <br> |
|  uint8\_t | [**receive\_status**](#variable-receive_status)  <br> |
|  uint32\_t | [**receive\_timer\_cr**](#variable-receive_timer_cr)  <br> |
|  uint32\_t | [**send\_arbitration\_id**](#variable-send_arbitration_id)  <br> |
|  uint32\_t | [**send\_bs\_remain**](#variable-send_bs_remain)  <br> |
|  uint32\_t | [**send\_buf\_size**](#variable-send_buf_size)  <br> |
|  uint8\_t \* | [**send\_buffer**](#variable-send_buffer)  <br> |
|  uint32\_t | [**send\_offset**](#variable-send_offset)  <br> |
|  int32\_t | [**send\_protocol\_result**](#variable-send_protocol_result)  <br> |
|  uint32\_t | [**send\_size**](#variable-send_size)  <br> |
|  uint8\_t | [**send\_sn**](#variable-send_sn)  <br> |
|  uint32\_t | [**send\_st\_min\_us**](#variable-send_st_min_us)  <br> |
|  uint8\_t | [**send\_status**](#variable-send_status)  <br> |
|  uint32\_t | [**send\_timer\_bs**](#variable-send_timer_bs)  <br> |
|  uint32\_t | [**send\_timer\_st**](#variable-send_timer_st)  <br> |
|  uint8\_t | [**send\_wtf\_count**](#variable-send_wtf_count)  <br> |












































## Detailed Description


The data stored in this struct is used internally and may be used by software programs using this library. 


    
## Public Attributes Documentation




### variable param\_block\_size 

```C++
uint8_t IsoTpLink::param_block_size;
```



Block size (BS) for FlowControl. 


        

<hr>



### variable param\_n\_bs\_us 

```C++
uint32_t IsoTpLink::param_n_bs_us;
```



N\_Bs timeout in microseconds. 


        

<hr>



### variable param\_n\_cr\_us 

```C++
uint32_t IsoTpLink::param_n_cr_us;
```



N\_Cr timeout in microseconds. 


        

<hr>



### variable param\_st\_min\_us 

```C++
uint32_t IsoTpLink::param_st_min_us;
```



STmin in microseconds. 


        

<hr>



### variable receive\_arbitration\_id 

```C++
uint32_t IsoTpLink::receive_arbitration_id;
```



Arbitration ID used for reception. 


        

<hr>



### variable receive\_bs\_count 

```C++
uint8_t IsoTpLink::receive_bs_count;
```



Max number of FC.Wait frames to send. 


        

<hr>



### variable receive\_buf\_size 

```C++
uint32_t IsoTpLink::receive_buf_size;
```



Receive buffer size in bytes. 


        

<hr>



### variable receive\_buffer 

```C++
uint8_t* IsoTpLink::receive_buffer;
```



Receive buffer base pointer. 


        

<hr>



### variable receive\_offset 

```C++
uint32_t IsoTpLink::receive_offset;
```



Current receive offset in bytes. 


        

<hr>



### variable receive\_protocol\_result 

```C++
int IsoTpLink::receive_protocol_result;
```



Last protocol result code for receiver. 


        

<hr>



### variable receive\_size 

```C++
uint32_t IsoTpLink::receive_size;
```



Total received size in bytes. 


        

<hr>



### variable receive\_sn 

```C++
uint8_t IsoTpLink::receive_sn;
```



Expected sequence number. 


        

<hr>



### variable receive\_status 

```C++
uint8_t IsoTpLink::receive_status;
```



Receiver status. 


        

<hr>



### variable receive\_timer\_cr 

```C++
uint32_t IsoTpLink::receive_timer_cr;
```



Timer until next ConsecutiveFrame is expected. 


        

<hr>



### variable send\_arbitration\_id 

```C++
uint32_t IsoTpLink::send_arbitration_id;
```



Arbitration ID used for consecutive frames. 


        

<hr>



### variable send\_bs\_remain 

```C++
uint32_t IsoTpLink::send_bs_remain;
```



Remaining block size. 


        

<hr>



### variable send\_buf\_size 

```C++
uint32_t IsoTpLink::send_buf_size;
```



Send buffer size in bytes. 


        

<hr>



### variable send\_buffer 

```C++
uint8_t* IsoTpLink::send_buffer;
```



Send buffer base pointer. 


        

<hr>



### variable send\_offset 

```C++
uint32_t IsoTpLink::send_offset;
```



Current send offset in bytes. 


        

<hr>



### variable send\_protocol\_result 

```C++
int32_t IsoTpLink::send_protocol_result;
```



Last protocol result code for sender. 


        

<hr>



### variable send\_size 

```C++
uint32_t IsoTpLink::send_size;
```



Total size to send in bytes. 


        

<hr>



### variable send\_sn 

```C++
uint8_t IsoTpLink::send_sn;
```



Current sequence number. 


        

<hr>



### variable send\_st\_min\_us 

```C++
uint32_t IsoTpLink::send_st_min_us;
```



Minimum separation time in microseconds. 


        

<hr>



### variable send\_status 

```C++
uint8_t IsoTpLink::send_status;
```



Sender status. 


        

<hr>



### variable send\_timer\_bs 

```C++
uint32_t IsoTpLink::send_timer_bs;
```



Timer until next FlowControl is expected. 


        

<hr>



### variable send\_timer\_st 

```C++
uint32_t IsoTpLink::send_timer_st;
```



Timestamp of last consecutive frame send. 


        

<hr>



### variable send\_wtf\_count 

```C++
uint8_t IsoTpLink::send_wtf_count;
```



Max number of FC.Wait frames to send. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `isotp.h`

