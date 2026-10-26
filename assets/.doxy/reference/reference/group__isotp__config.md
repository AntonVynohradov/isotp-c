

# Group isotp\_config



[**Modules**](modules.md) **>** [**isotp\_config**](group__isotp__config.md)



_Build-time configuration for ISOTP-C._ [More...](#detailed-description)

































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ISOTP\_DEFAULT\_BLOCK\_SIZE**](group__isotp__config.md#define-isotp_default_block_size)  `8U`<br>_Block size (BS) this receiver requests; 0 means no further Flow Control._  |
| define  | [**ISOTP\_DEFAULT\_N\_BS\_TIMEOUT\_US**](group__isotp__config.md#define-isotp_default_n_bs_timeout_us)  `1000000U`<br>_Sender timeout N\_Bs: longest wait for a Flow Control frame, in microseconds._  |
| define  | [**ISOTP\_DEFAULT\_N\_CR\_TIMEOUT\_US**](group__isotp__config.md#define-isotp_default_n_cr_timeout_us)  `1000000U`<br>_Receiver timeout N\_Cr: longest wait for the next Consecutive Frame, in microseconds._  |
| define  | [**ISOTP\_DEFAULT\_ST\_MIN\_US**](group__isotp__config.md#define-isotp_default_st_min_us)  `0U`<br>_Minimum separation time (STmin) this receiver requests, in microseconds._  |
| define  | [**ISOTP\_DEFAULT\_WFT\_MAX**](group__isotp__config.md#define-isotp_default_wft_max)  `10U`<br>_N\_WFTmax: Flow Control frames with the status Wait accepted in a row._  |
| define  | [**ISOTP\_RX\_BUFFER\_SIZE**](group__isotp__config.md#define-isotp_rx_buffer_size)  `4095U`<br>_Largest message one link can receive, in bytes._  |
| define  | [**ISOTP\_TX\_BUFFER\_SIZE**](group__isotp__config.md#define-isotp_tx_buffer_size)  `4095U`<br>_Largest message one link can transmit, in bytes._  |

## Detailed Description


Every parameter can be overridden by the integrator with a compiler definition (for example `-DISOTP_RX_BUFFER_SIZE=64U`) without editing this file. The buffer sizes are compiled into [**Link**](group__isotp__link.md), so they set the RAM footprint of one link. The protocol defaults are only the values written by [**isotp\_link\_config\_default**](group__isotp__link.md#function-isotp_link_config_default); every link can use its own values at run time.




**
**


* [**ISOTP\_TX\_BUFFER\_SIZE**](group__isotp__config.md#define-isotp_tx_buffer_size), [**ISOTP\_RX\_BUFFER\_SIZE**](group__isotp__config.md#define-isotp_rx_buffer_size) : largest message in each direction. 
* [**ISOTP\_DEFAULT\_BLOCK\_SIZE**](group__isotp__config.md#define-isotp_default_block_size), [**ISOTP\_DEFAULT\_ST\_MIN\_US**](group__isotp__config.md#define-isotp_default_st_min_us) : flow control we request. 
* [**ISOTP\_DEFAULT\_N\_BS\_TIMEOUT\_US**](group__isotp__config.md#define-isotp_default_n_bs_timeout_us), [**ISOTP\_DEFAULT\_N\_CR\_TIMEOUT\_US**](group__isotp__config.md#define-isotp_default_n_cr_timeout_us) : timeouts. 
* [**ISOTP\_DEFAULT\_WFT\_MAX**](group__isotp__config.md#define-isotp_default_wft_max) : flow-control wait frames we accept. 







    
## Macro Definition Documentation





### define ISOTP\_DEFAULT\_BLOCK\_SIZE 

_Block size (BS) this receiver requests; 0 means no further Flow Control._ 
```
#define ISOTP_DEFAULT_BLOCK_SIZE `8U`
```




<hr>



### define ISOTP\_DEFAULT\_N\_BS\_TIMEOUT\_US 

_Sender timeout N\_Bs: longest wait for a Flow Control frame, in microseconds._ 
```
#define ISOTP_DEFAULT_N_BS_TIMEOUT_US `1000000U`
```



ISO 15765-2 specifies 1000 ms. 


        

<hr>



### define ISOTP\_DEFAULT\_N\_CR\_TIMEOUT\_US 

_Receiver timeout N\_Cr: longest wait for the next Consecutive Frame, in microseconds._ 
```
#define ISOTP_DEFAULT_N_CR_TIMEOUT_US `1000000U`
```



ISO 15765-2 specifies 1000 ms. 


        

<hr>



### define ISOTP\_DEFAULT\_ST\_MIN\_US 

_Minimum separation time (STmin) this receiver requests, in microseconds._ 
```
#define ISOTP_DEFAULT_ST_MIN_US `0U`
```




<hr>



### define ISOTP\_DEFAULT\_WFT\_MAX 

_N\_WFTmax: Flow Control frames with the status Wait accepted in a row._ 
```
#define ISOTP_DEFAULT_WFT_MAX `10U`
```




<hr>



### define ISOTP\_RX\_BUFFER\_SIZE 

_Largest message one link can receive, in bytes._ 
```
#define ISOTP_RX_BUFFER_SIZE `4095U`
```



A First Frame announcing a longer message is answered with a Flow Control frame with the status Overflow. 


        

<hr>



### define ISOTP\_TX\_BUFFER\_SIZE 

_Largest message one link can transmit, in bytes._ 
```
#define ISOTP_TX_BUFFER_SIZE `4095U`
```



The link copies the message into a buffer of this size, so the caller may reuse its own buffer as soon as [**isotp\_link\_send**](group__isotp__link.md#function-isotp_link_send) returns. 


        

<hr>

------------------------------


