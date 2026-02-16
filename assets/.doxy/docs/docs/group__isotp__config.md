

# Group isotp\_config



[**Modules**](modules.md) **>** [**isotp\_config**](group__isotp__config.md)



_Build-time configuration for ISO-TP._ [More...](#detailed-description)


































































## Detailed Description


Configuration parameters in this group control protocol behavior as exposed by the API in [**isotp.h**](isotp_8h.md) and implemented in isotp.c. They cover limits and timing used when sending and receiving single- and multi-frame messages, including block size (BS), minimum separation time (ST\_min), and response timeouts. They also enable optional features such as frame padding, an extra argument for isotp\_user\_send\_can, and callbacks for transmit and receive completion.




**
**


* [**ISO\_TP\_DEFAULT\_BLOCK\_SIZE**](isotp__config_8h.md#define-iso_tp_default_block_size) : default receiver block size (BS). 
* [**ISO\_TP\_DEFAULT\_ST\_MIN\_US**](isotp__config_8h.md#define-iso_tp_default_st_min_us) : default minimum separation time (ST\_min) in us. 
* [**ISO\_TP\_MAX\_WFT\_NUMBER**](isotp__config_8h.md#define-iso_tp_max_wft_number) : maximum number of FC.Wait frames in a row. 
* [**ISO\_TP\_DEFAULT\_RESPONSE\_TIMEOUT\_US**](isotp__config_8h.md#define-iso_tp_default_response_timeout_us) : response timeout during multi-frame transfer. 
* ISO\_TP\_FRAME\_PADDING : enable frame padding for ISO-TP frames. 
* [**ISO\_TP\_FRAME\_PADDING\_VALUE**](isotp__config_8h.md#define-iso_tp_frame_padding_value) : padding byte value when padding is enabled. 
* ISO\_TP\_USER\_SEND\_CAN\_ARG : add a user argument to [**isotp\_user\_send\_can**](group__isotp__user.md#function-isotp_user_send_can). 
* ISO\_TP\_TRANSMIT\_COMPLETE\_CALLBACK : enable transmit complete callback support. 
* ISO\_TP\_RECEIVE\_COMPLETE\_CALLBACK: enable receive complete callback support. 







    

------------------------------


