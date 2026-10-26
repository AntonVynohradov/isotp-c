

# Group isotp



[**Modules**](modules.md) **>** [**isotp**](group__isotp.md)



_Public API of the ISO 15765-2 (ISO-TP) library._ [More...](#detailed-description)












## Modules

| Type | Name |
| ---: | :--- |
| module | [**Configuration**](group__isotp__config.md) <br>_Build-time configuration for ISOTP-C._  |
| module | [**Definitions**](group__isotp__defs.md) <br>_Protocol constants of ISO 15765-2 and the library result codes._  |
| module | [**Link**](group__isotp__link.md) <br>_Transport layer of one ISO-TP connection: segmentation, flow control, timing._  |
| module | [**Codec**](group__isotp__protocol.md) <br>_Pure functions that translate between CAN frames and ISO-TP PDUs._  |





















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ISOTP\_VERSION**](group__isotp.md#define-isotp_version)  `"2.0.1"`<br>_Version string "MAJOR.MINOR.PATCH"._  |
| define  | [**ISOTP\_VERSION\_MAJOR**](group__isotp.md#define-isotp_version_major)  `2`<br> |
| define  | [**ISOTP\_VERSION\_MINOR**](group__isotp.md#define-isotp_version_minor)  `0`<br> |
| define  | [**ISOTP\_VERSION\_PATCH**](group__isotp.md#define-isotp_version_patch)  `1`<br> |

## Detailed Description


This module groups the public interface and its supporting submodules: [**Configuration**](group__isotp__config.md), [**Definitions**](group__isotp__defs.md), [**Codec**](group__isotp__protocol.md) and [**Link**](group__isotp__link.md).


Include this single header to use the library: 
* [**isotp\_config.h**](isotp__config_8h.md) build-time configuration (buffer sizes, protocol defaults) 
* [**isotp\_defs.h**](isotp__defs_8h.md) frame types, flow status, N\_Result codes, big-endian helpers 
* [**isotp\_protocol.h**](isotp__protocol_8h.md) frame codec: decode, encode SF/FF/CF/FC, STmin 
* [**isotp\_link.h**](isotp__link_8h.md) transport layer of one connection (segmentation, flow control, timing) 




    
## Macro Definition Documentation





### define ISOTP\_VERSION 

_Version string "MAJOR.MINOR.PATCH"._ 
```
#define ISOTP_VERSION `"2.0.1"`
```




<hr>



### define ISOTP\_VERSION\_MAJOR 

```
#define ISOTP_VERSION_MAJOR `2`
```




<hr>



### define ISOTP\_VERSION\_MINOR 

```
#define ISOTP_VERSION_MINOR `0`
```




<hr>



### define ISOTP\_VERSION\_PATCH 

```
#define ISOTP_VERSION_PATCH `1`
```




<hr>

------------------------------


