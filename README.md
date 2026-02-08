# ISO-TP-C: Embedded-Grade Refactoring & Optimization

## Overview

This is an **active refactoring fork** of [SimonCahill/isotp-c](https://github.com/SimonCahill/isotp-c), a robust C implementation of the ISO 15765-2 (ISO-TP) protocol for CAN bus communication.

### Project Status

🚧 **REFACTORING IN PROGRESS** 🚧

This fork is dedicated to elevating the ISO-TP-C library to production-grade quality standards for embedded systems through comprehensive refactoring, optimization, testing, and documentation initiatives.

---

## About ISO-TP

ISO 15765-2, or **ISO-TP**, is an international standard for transporting data packets over a CAN-Bus. It enables the transmission of messages exceeding the eight-byte maximum payload of standard CAN frames by segmenting longer messages into multiple frames with metadata for reassembly.

**Key Capabilities:**
- Support for up to 4095 bytes of payload per message packet
- Single and multiple frame transmission
- Full-duplex operation mode
- Platform-agnostic design using dependency injection

---

## Refactoring Objectives

This fork aims to transform ISO-TP-C into a production-ready, enterprise-grade embedded systems library through the following initiatives:

### 🎯 Primary Goals

1. **Embedded Systems Optimization**
   - Memory footprint reduction
   - Performance optimization for resource-constrained environments
   - Real-time operation support
   - Deterministic behavior analysis

2. **Comprehensive Testing**
   - Unit test coverage: **100%**
   - Integration test suite
   - Regression testing framework
   - Test-driven development practices

3. **Professional Documentation**
   - API reference documentation
   - Architecture and design guides
   - Integration tutorials and examples
   - Troubleshooting and best practices

4. **MISRA-2012 Compliance**
   - Static code analysis compliance
   - Memory safety and security hardening
   - Safer coding patterns
   - Reduced risk profile for safety-critical applications

---

## Roadmap

- [ ] Code refactoring for embedded systems
- [ ] Memory optimization and profiling
- [ ] 100% unit test coverage implementation
- [ ] Static analysis and MISRA-2012 compliance
- [ ] Comprehensive API documentation
- [ ] Integration guides and examples
- [ ] Performance benchmarking suite

---

## Original Sources

This project builds upon the excellent work of:

1. **[openxc/isotp-c](https://github.com/openxc/isotp-c)** - The original ISO-TP-C reference implementation
2. **[SimonCahill/isotp-c](https://github.com/SimonCahill/isotp-c)** - The fork that serves as the foundation for this refactoring effort

We acknowledge and appreciate the contributions of all original authors and maintainers.

---

## Building & Usage

### Prerequisites
- C compiler (GCC, LLVM, or MSVC)
- CMake 3.10+ (recommended) or Make
- Optional: Python 3.x for testing and documentation generation

### Build Options

#### Using CMake (Recommended)
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

#### Using Make
```bash
make all
```

#### Configuration Options
- `-Disotpc_USE_INCLUDE_DIR=ON` - Use external include directory
- `-Disotpc_STATIC_LIBRARY=ON` - Build as static library
- `-Disotpc_ENABLE_TRANSCEIVE_EVENTS=ON` - Enable event-driven messaging
- `-DISO_TP_USER_SEND_CAN_ARG=ON` - Support multiple CAN interfaces

### Quick Start Example

```c
#include "isotp.h"

/* Initialize buffers */
static uint8_t rx_buffer[ISOTP_BUFSIZE];
static uint8_t tx_buffer[ISOTP_BUFSIZE];
static IsoTpLink link;

int main(void) {
    /* Initialize the link */
    isotp_init_link(&link, 0x7TT, tx_buffer, sizeof(tx_buffer), 
                    rx_buffer, sizeof(rx_buffer));
    
    while (1) {
        /* Handle incoming CAN messages */
        uint32_t can_id;
        uint8_t data[8];
        uint8_t len;
        
        if (can_receive(&can_id, data, &len) == RET_OK) {
            if (can_id == 0x7RR) {
                isotp_on_can_message(&link, data, len);
            }
        }
        
        /* Process link state */
        isotp_poll(&link);
        
        /* Handle received messages */
        uint8_t payload[4095];
        uint32_t payload_len;
        if (isotp_receive(&link, payload, sizeof(payload), &payload_len) == ISOTP_RET_OK) {
            process_message(payload, payload_len);
        }
    }
    
    return 0;
}
```

---

## Project Structure

```
isotp-c/
├── src/                    # Source implementation files
│   └── isotp.c
├── inc/                    # Public header files
│   ├── isotp_config.h
│   ├── isotp_defines.h
│   └── isotp_user.h
├── tests/                  # Unit tests (under development)
├── docs/                   # Documentation (under development)
├── CMakeLists.txt
├── Makefile
├── LICENSE
└── README.md
```

---

## Contributing

We welcome contributions that align with our refactoring objectives. Please ensure:

- Code follows MISRA-2012 guidelines
- Changes include appropriate unit tests
- Documentation is updated accordingly
- Build verification passes on all supported platforms

---

## License

Licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

## Acknowledgments

- Original concept and design by the [OpenXC](https://openxcplatform.com/) project
- Continued development and maintenance by [Simon Cahill](https://github.com/SimonCahill)
- Community contributors and testers
- MISRA-2012 standards guidance and compliance tools

---

## Contact & Support

For questions or issues related to:
- **This refactoring fork**: See the Issues section of this repository
- **Original implementation**: Visit [SimonCahill/isotp-c](https://github.com/SimonCahill/isotp-c)
- **ISO-TP protocol specifications**: Refer to [ISO 15765-2](https://en.wikipedia.org/wiki/ISO_15765-2)

---

**Last Updated:** February 2026  
**Status:** Active Development  
**Target Release:** TBD
