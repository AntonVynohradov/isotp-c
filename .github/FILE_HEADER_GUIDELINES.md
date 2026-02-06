# File Header Guidelines for ISO-TP-C

## Overview

All files in the ISO-TP-C project should include a standard header containing:
- Project name and description
- Author information
- License information
- Copyright notice
- References and links
- Version information

## Why Headers Matter

✅ **Legal Compliance** - Copyright and license information  
✅ **Attribution** - Proper credit to original authors  
✅ **Maintenance** - Easy identification of file purpose and ownership  
✅ **SPDX Compliance** - License identifier for automated tools  
✅ **Professional** - Industry-standard practice  

## Header Template for C Files (.c and .h)

Use the standard C comment block format:

```c
/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: [File-specific description - what this file does]
 *
 * Author:      Anton Vynohradov
 * Email:       avynohradovair@gmail.com
 * 
 * License:     MIT License
 *
 * Copyright (c) 2026 Anton Vynohradov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Original Inspiration:
 *   - openxc/isotp-c (https://github.com/openxc/isotp-c)
 *   - SimonCahill/isotp-c (https://github.com/SimonCahill/isotp-c)
 *
 * Refactoring Goals:
 *   ✓ Embedded systems optimization
 *   ✓ 100% unit test coverage
 *   ✓ MISRA-2012 compliance
 *   ✓ CERT C Coding Standard adherence
 *   ✓ Professional documentation
 *
 * References:
 *   - ISO 15765-2 Standard (ISO-TP Protocol)
 *   - CERT C Coding Standard: https://wiki.sei.cmu.edu/confluence/display/c/
 *   - MISRA C:2012: https://www.misra.org.uk/
 *
 * Last Updated: February 2026
 * Version: 1.0.0
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/
```

## Header Template for CMake Files

For CMakeLists.txt and *.cmake files:

```cmake
################################################################################
# ISO-TP-C: ISO 15765-2 Protocol Implementation
#
# Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
# Description: [File-specific description]
#
# Author:      Anton Vynohradov
# Email:       avynohradovair@gmail.com
# 
# License:     MIT License
#
# Copyright (c) 2026 Anton Vynohradov
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# Original Inspiration:
#   - openxc/isotp-c (https://github.com/openxc/isotp-c)
#   - SimonCahill/isotp-c (https://github.com/SimonCahill/isotp-c)
#
# Refactoring Goals:
#   ✓ Embedded systems optimization
#   ✓ 100% unit test coverage
#   ✓ MISRA-2012 compliance
#   ✓ CERT C Coding Standard adherence
#   ✓ Professional documentation
#
# References:
#   - ISO 15765-2 Standard (ISO-TP Protocol)
#   - CERT C Coding Standard: https://wiki.sei.cmu.edu/confluence/display/c/
#   - MISRA C:2012: https://www.misra.org.uk/
#
# Last Updated: February 2026
# Version: 1.0.0
#
################################################################################
```

## Header Template for Makefile

```makefile
################################################################################
# ISO-TP-C: ISO 15765-2 Protocol Implementation - Makefile
#
# Author:      Anton Vynohradov
# Email:       avynohradovair@gmail.com
# License:     MIT License
# Copyright:   (c) 2026 Anton Vynohradov
#
# References:
#   - ISO 15765-2 Standard (ISO-TP Protocol)
#   - CERT C Coding Standard: https://wiki.sei.cmu.edu/confluence/display/c/
#
# Last Updated: February 2026
#
# SPDX-License-Identifier: MIT
################################################################################
```

## Header Template for Shell Scripts

```bash
#!/bin/bash
################################################################################
# ISO-TP-C: ISO 15765-2 Protocol Implementation
#
# Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
# Description: [Script purpose]
#
# Author:      Anton Vynohradov
# Email:       avynohradovair@gmail.com
# License:     MIT License
#
# Last Updated: February 2026
#
# SPDX-License-Identifier: MIT
################################################################################
```

## Header Template for Python Scripts

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
################################################################################
# ISO-TP-C: ISO 15765-2 Protocol Implementation
#
# Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
# Description: [Script purpose]
#
# Author:      Anton Vynohradov
# Email:       avynohradovair@gmail.com
# License:     MIT License
#
# Copyright (c) 2026 Anton Vynohradov
#
# Last Updated: February 2026
# SPDX-License-Identifier: MIT
################################################################################
```

## Header Template for Markdown

```markdown
# [File Title]

**Project:** ISO-TP-C - Embedded-Grade Refactoring & Optimization  
**Author:** Anton Vynohradov (avynohradovair@gmail.com)  
**License:** MIT License  
**Last Updated:** February 2026  
**SPDX-License-Identifier:** MIT

---

[Document content...]
```

## Files Requiring Headers

✅ **Must Have Headers:**
- All `.c` source files
- All `.h` header files
- All test files (`*.test.c`)
- Build scripts (Makefile, CMakeLists.txt, *.cmake)
- Shell scripts (`.sh`)
- Python scripts (`.py`)

⚠️ **Optional Headers:**
- Configuration files (`.json`, `.yaml`)
- Markdown documentation (nice to have)
- License file (already MIT)

❌ **No Header Needed:**
- Build artifacts
- Binary files
- Generated files
- Third-party libraries

## Customization

When adding file headers, customize:

```
Description: [Specific purpose of this file]
Last Updated: [Current date]
Version: [File/Module version if applicable]
```

The rest should remain consistent across all project files.

## Verification

To check if files have proper headers:

```bash
# Check for MIT license marker in all C files
grep -r "SPDX-License-Identifier: MIT" Src/ Inc/ tests/

# Check for author information
grep -r "Anton Vynohradov" Src/ Inc/ tests/

# Check for copyright notice
grep -r "Copyright.*2026" Src/ Inc/ tests/
```

## Tools for Adding Headers

### Bulk Add Headers to Existing Files

Using a script to add headers:

```bash
#!/bin/bash
# Add header to all C files
for file in Src/*.c Inc/*.h; do
    if ! grep -q "SPDX-License-Identifier" "$file"; then
        cat FILE_HEADER_TEMPLATE.h "$file" > "${file}.tmp"
        mv "${file}.tmp" "$file"
        echo "Added header to $file"
    fi
done
```

## References

- **SPDX License Identifiers**: https://spdx.org/licenses/
- **MIT License**: https://opensource.org/licenses/MIT
- **Standard Headers in Open Source**: https://opensource.stackexchange.com/

---

**Document Version:** 1.0  
**Last Updated:** February 2026  
**Author:** Anton Vynohradov (avynohradovair@gmail.com)
