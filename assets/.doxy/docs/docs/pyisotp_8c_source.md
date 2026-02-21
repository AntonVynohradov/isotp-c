

# File pyisotp.c

[**File List**](files.md) **>** [**examples**](dir_d28a4824dc47e487b107a5db32ef43c4.md) **>** [**python\_api**](dir_181ea403426f4b233011860116dcb1b5.md) **>** [**pyisotp.c**](pyisotp_8c.md)

[Go to the documentation of this file](pyisotp_8c.md)


```C++
/*******************************************************************************
 * ISO-TP-C: ISO 15765-2 Protocol Implementation
 *
 * Project:     ISO-TP-C - Embedded-Grade Refactoring & Optimization
 * Description: File description - briefly describe purpose of this source file
 *
 * Author:      Anton Vynohradov
 * Email:       avynohradov@systemfromscratch.com
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
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

#define PY_SSIZE_T_CLEAN

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#include <Python.h>

#include "isotp.h"
#include "can_driver.h"
#include "mock_can.h"
#include "mock_time.h"

/* ==============================================================================
 * PRIVATE TYPE DEFINITIONS
 * =============================================================================*/

/* Static type definitions local to this file */

/* ==============================================================================
 * PRIVATE FUNCTION DECLARATIONS (static)
 * =============================================================================*/

static PyObject* pyisotp_init(PyObject* self, PyObject* args);

static PyObject* pyisotp_send(PyObject* self, PyObject* args);

static PyObject* pyisotp_poll(PyObject* self, PyObject* args);

static PyObject* pyisotp_receive(PyObject* self, PyObject* args);

static PyObject* py_mock_enable_drop(PyObject* self, PyObject* args);

static PyObject* py_mock_disable_fc(PyObject* self, PyObject* args);

static PyObject* py_get_last_protocol_result(PyObject* self, PyObject* args);

static PyObject* py_get_last_receive_protocol_result(PyObject* self, PyObject* args);

static PyObject* py_set_timeouts(PyObject* self, PyObject* args);

static PyObject* py_set_fc_params(PyObject* self, PyObject* args);

static PyObject* py_inject_can(PyObject* self, PyObject* args);

static PyObject* py_time_advance(PyObject* self, PyObject* args);

static PyObject* py_time_set(PyObject* self, PyObject* args);

static PyObject* py_time_reset(PyObject* self, PyObject* args);

static void pyisotp_capsule_destructor(PyObject* capsule);

/* ==============================================================================
 * PRIVATE VARIABLES (static)
 * =============================================================================*/

// clang-format off
static PyMethodDef PyIsoTpMethods[] =
{
    {"init", pyisotp_init, METH_VARARGS, "Initialize ISO-TP link"},
    {"send", pyisotp_send, METH_VARARGS, "Send data over ISO-TP"},
    {"poll", pyisotp_poll, METH_VARARGS, "Poll ISO-TP link for timeouts and state updates"},
    {"receive", pyisotp_receive, METH_VARARGS, "Receive data from ISO-TP link"},
    {"mock_enable_drop", py_mock_enable_drop, METH_VARARGS, "Enable or disable CAN frame dropping in the mock CAN driver"},
    {"mock_disable_fc", py_mock_disable_fc, METH_VARARGS, "Enable or disable FlowControl frames in the mock CAN driver"},
    {"time_advance", py_time_advance, METH_VARARGS, "Advance virtual time in the mock time implementation"},
    {"time_set", py_time_set, METH_VARARGS, "Set virtual time in the mock time implementation to a specific value"},
    {"time_reset", py_time_reset, METH_VARARGS, "Reset virtual time in the mock time implementation to zero"},
    {"get_last_protocol_result", py_get_last_protocol_result, METH_VARARGS, "Get last sender protocol result"},
    {"get_last_receive_protocol_result", py_get_last_receive_protocol_result, METH_VARARGS, "Get last receiver protocol result"},
    {"set_timeouts", py_set_timeouts, METH_VARARGS, "Set N_Bs and N_Cr timeouts in milliseconds"},
    {"set_fc_params", py_set_fc_params, METH_VARARGS, "Set FlowControl block size and STmin in milliseconds"},
    {"inject_can", py_inject_can, METH_VARARGS, "Inject raw CAN frame into the ISO-TP link"},
    {NULL, NULL, 0, NULL} // Sentinel

};

static struct PyModuleDef pyisotp_module =
{
    PyModuleDef_HEAD_INIT,
    "pyisotp",
    "Python bindings for ISO-TP-C library",
    -1,
    PyIsoTpMethods
};
// clang-format on

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

static PyObject* pyisotp_init(PyObject* self, PyObject* args)
{
    uint32_t sendid = 0;
    Py_ssize_t sbufsz = {0};
    Py_ssize_t rbufsz = {0};

    if (PyArg_ParseTuple(args, "Inn", &sendid, &sbufsz, &rbufsz))
    {
        if ((sbufsz <= 0) || (rbufsz <= 0) || (sbufsz > UINT32_MAX) || (rbufsz > UINT32_MAX))
        {
            PyErr_SetString(PyExc_ValueError, "sbufsz/rbufsz must be > 0 and <= UINT32_MAX");
            return NULL;
        }

        mock_can_init();
        mock_time_reset();

        IsoTpLink* link = PyMem_Malloc(sizeof(IsoTpLink));
        uint8_t* sbuf = PyMem_Malloc(sbufsz);
        uint8_t* rbuf = PyMem_Malloc(rbufsz);

        if ((link == NULL) || (sbuf == NULL) || (rbuf == NULL))
        {
            PyMem_Free(link);
            PyMem_Free(sbuf);
            PyMem_Free(rbuf);

            return PyErr_NoMemory();
        }

        isotp_init_link(link, sendid, sbuf, (uint32_t) sbufsz, rbuf, (uint32_t) rbufsz);

        PyObject* capsule = PyCapsule_New(link, "IsoTpLink", pyisotp_capsule_destructor);
        if (capsule == NULL)
        {
            isotp_destroy_link(link);
            PyMem_Free(sbuf);
            PyMem_Free(rbuf);
            PyMem_Free(link);

            return NULL;
        }

        return capsule;
    }

    return NULL;
}

static PyObject* pyisotp_send(PyObject* self, PyObject* args)
{
    PyObject* py_link = NULL;
    const char* data = NULL;
    Py_ssize_t len = {0};

    if (PyArg_ParseTuple(args, "Oy#", &py_link, &data, &len))
    {
        IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");

        if (link != NULL)
        {
            if (len > (Py_ssize_t) link->send_buf_size)
            {
                return PyLong_FromLong(ISOTP_RET_OVERFLOW);
            }

            int res = isotp_send(link, (const uint8_t*) data, (uint32_t) len);

            return PyLong_FromLong(res);
        }
    }

    return NULL;
}

static PyObject* pyisotp_poll(PyObject* self, PyObject* args)
{
    PyObject* py_link = NULL;
    if (PyArg_ParseTuple(args, "O", &py_link))
    {
        IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");
        if (link != NULL)
        {
            isotp_poll(link);

            uint32_t id = 0;
            uint8_t len = 0;
            uint8_t data[8] = {0};

            while (can_receive(&id, data, &len) == 0)
            {
                isotp_on_can_message(link, data, len);
            }

            Py_RETURN_NONE;
        }
    }

    return NULL;
}

static PyObject* pyisotp_receive(PyObject* self, PyObject* args)
{
    PyObject* py_link = NULL;
    Py_ssize_t bufsize = {0};

    if (PyArg_ParseTuple(args, "On", &py_link, &bufsize))
    {
        if (bufsize <= 0 || bufsize > UINT32_MAX)
        {
            PyErr_SetString(PyExc_ValueError, "bufsize must be > 0 and <= UINT32_MAX");
            return NULL;
        }

        IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");
        if (link != NULL)
        {
            uint8_t* buf = PyMem_Malloc(bufsize);
            if (buf == NULL)
            {
                return PyErr_NoMemory();
            }

            uint32_t outsz = 0;

            int ret = isotp_receive(link, buf, (uint32_t) bufsize, &outsz);
            if (ret == ISOTP_RET_OK)
            {
                if (outsz > (uint32_t) PY_SSIZE_T_MAX)
                {
                    PyMem_Free(buf);
                    PyErr_SetString(PyExc_OverflowError,
                                    "received size exceeds Python maximum size");
                    return NULL;
                }

                PyObject* result = Py_BuildValue("y#", buf, (Py_ssize_t) outsz);
                PyMem_Free(buf);

                return result;
            }

            PyMem_Free(buf);

            Py_RETURN_NONE;
        }
    }

    return NULL;
}

static PyObject* py_mock_enable_drop(PyObject* self, PyObject* args)
{
    int enable = 0;

    if (PyArg_ParseTuple(args, "p", &enable))
    {
        mock_can_enable_drop(enable);

        Py_RETURN_NONE;
    }

    return NULL;
}

static PyObject* py_mock_disable_fc(PyObject* self, PyObject* args)
{
    int enable = 0;
    if (PyArg_ParseTuple(args, "p", &enable))
    {
        mock_can_disable_fc(enable);

        Py_RETURN_NONE;
    }

    return NULL;
}

static PyObject* py_time_advance(PyObject* self, PyObject* args)
{
    uint32_t delta_ms = 0;

    if (PyArg_ParseTuple(args, "I", &delta_ms))
    {
        if (delta_ms > (UINT32_MAX / 1000U))
        {
            PyErr_SetString(PyExc_OverflowError, "delta_ms too large to convert to microseconds");
            return NULL;
        }

        uint32_t delta_us = (delta_ms * 1000U);

        mock_time_advance(delta_us);

        Py_RETURN_NONE;
    }

    return NULL;
}

static PyObject* py_time_set(PyObject* self, PyObject* args)
{
    uint32_t value = 0;

    if (PyArg_ParseTuple(args, "I", &value))
    {
        if (value > (UINT32_MAX / 1000U))
        {
            PyErr_SetString(PyExc_OverflowError, "value too large to convert to microseconds");
            return NULL;
        }

        mock_time_set(value * 1000U);

        Py_RETURN_NONE;
    }

    return NULL;
}

static PyObject* py_time_reset(PyObject* self, PyObject* args)
{
    mock_time_reset();

    Py_RETURN_NONE;
}

static PyObject* py_get_last_protocol_result(PyObject* self, PyObject* args)
{
    PyObject* py_link = NULL;
    if (PyArg_ParseTuple(args, "O", &py_link))
    {
        IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");
        if (link != NULL)
        {
            return PyLong_FromLong(link->send_protocol_result);
        }
    }

    return NULL;
}

static PyObject* py_get_last_receive_protocol_result(PyObject* self, PyObject* args)
{
    PyObject* py_link = NULL;
    if (PyArg_ParseTuple(args, "O", &py_link))
    {
        IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");
        if (link != NULL)
        {
            return PyLong_FromLong(link->receive_protocol_result);
        }
    }

    return NULL;
}

static PyObject* py_set_timeouts(PyObject* self, PyObject* args)
{
    PyObject* py_link = NULL;
    uint32_t n_bs_ms = 0;
    uint32_t n_cr_ms = 0;

    if (PyArg_ParseTuple(args, "OII", &py_link, &n_bs_ms, &n_cr_ms))
    {
        if ((n_bs_ms > (UINT32_MAX / 1000U)) || (n_cr_ms > (UINT32_MAX / 1000U)))
        {
            PyErr_SetString(PyExc_OverflowError, "timeout too large to convert to microseconds");
            return NULL;
        }

        IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");
        if (link != NULL)
        {
            isotp_set_timeouts(link, n_bs_ms * 1000U, n_cr_ms * 1000U);
            Py_RETURN_NONE;
        }
    }

    return NULL;
}

static PyObject* py_set_fc_params(PyObject* self, PyObject* args)
{
    PyObject* py_link = NULL;
    uint32_t block_size = 0;
    uint32_t st_min_ms = 0;

    if (PyArg_ParseTuple(args, "OII", &py_link, &block_size, &st_min_ms))
    {
        if (block_size > 0xFF)
        {
            PyErr_SetString(PyExc_ValueError, "block_size must be <= 255");
            return NULL;
        }

        if (st_min_ms > (UINT32_MAX / 1000U))
        {
            PyErr_SetString(PyExc_OverflowError, "st_min_ms too large to convert to microseconds");
            return NULL;
        }

        IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");
        if (link != NULL)
        {
            isotp_set_fc_params(link, (uint8_t) block_size, st_min_ms * 1000U);
            Py_RETURN_NONE;
        }
    }

    return NULL;
}

static PyObject* py_inject_can(PyObject* self, PyObject* args)
{
    PyObject* py_link = NULL;
    const char* data = NULL;
    Py_ssize_t len = 0;

    if (PyArg_ParseTuple(args, "Oy#", &py_link, &data, &len))
    {
        if (len < 2 || len > 8)
        {
            PyErr_SetString(PyExc_ValueError, "frame length must be 2..8");
            return NULL;
        }

        IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");
        if (link != NULL)
        {
            isotp_on_can_message(link, (const uint8_t*) data, (uint8_t) len);
            Py_RETURN_NONE;
        }
    }

    return NULL;
}

static void pyisotp_capsule_destructor(PyObject* capsule)
{
    IsoTpLink* link = PyCapsule_GetPointer(capsule, "IsoTpLink");
    if (link == NULL)
    {
        return;
    }

    uint8_t* sbuf = link->send_buffer;
    uint8_t* rbuf = link->receive_buffer;

    isotp_destroy_link(link);
    PyMem_Free(sbuf);
    PyMem_Free(rbuf);
    PyMem_Free(link);
}

/* ==============================================================================
 * PUBLIC FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

PyMODINIT_FUNC PyInit_pyisotp(void)
{
    PyObject* module = PyModule_Create(&pyisotp_module);
    if (module != NULL)
    {
        mock_can_init();
        mock_time_reset();

        // clang-format off
        if (PyModule_AddIntConstant(module,
            "ISOTP_PROTOCOL_RESULT_TIMEOUT_BS", ISOTP_PROTOCOL_RESULT_TIMEOUT_BS) < 0)
        {
            Py_DECREF(module);

            return NULL;
        }

        if (PyModule_AddIntConstant(module,
            "ISOTP_PROTOCOL_RESULT_TIMEOUT_CR", ISOTP_PROTOCOL_RESULT_TIMEOUT_CR) < 0)
        {
            Py_DECREF(module);

            return NULL;
        }

        if (PyModule_AddIntConstant(module,
            "ISOTP_PROTOCOL_RESULT_OK", ISOTP_PROTOCOL_RESULT_OK) < 0)
        // clang-format on
        {
            Py_DECREF(module);

            return NULL;
        }

        // clang-format off
        if (PyModule_AddIntConstant(module,
            "ISOTP_PROTOCOL_RESULT_WRONG_SN", ISOTP_PROTOCOL_RESULT_WRONG_SN) < 0)
        // clang-format on
        {
            Py_DECREF(module);

            return NULL;
        }

        // clang-format off
        if (PyModule_AddIntConstant(module,
            "ISOTP_PROTOCOL_RESULT_UNEXP_PDU", ISOTP_PROTOCOL_RESULT_UNEXP_PDU) < 0)
        // clang-format on
        {
            Py_DECREF(module);

            return NULL;
        }

        // clang-format off
        if (PyModule_AddIntConstant(module,
            "ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW", ISOTP_PROTOCOL_RESULT_BUFFER_OVFLW) < 0)
        // clang-format on
        {
            Py_DECREF(module);

            return NULL;
        }

        // clang-format off
        if (PyModule_AddIntConstant(module,
            "ISOTP_PROTOCOL_RESULT_WFT_OVRN", ISOTP_PROTOCOL_RESULT_WFT_OVRN) < 0)
        // clang-format on
        {
            Py_DECREF(module);

            return NULL;
        }

        // clang-format off
        if (PyModule_AddIntConstant(module,
            "ISOTP_RET_INPROGRESS", ISOTP_RET_INPROGRESS) < 0)
        // clang-format on
        {
            Py_DECREF(module);

            return NULL;
        }

        // clang-format off
        if (PyModule_AddIntConstant(module,
            "ISOTP_RET_OVERFLOW", ISOTP_RET_OVERFLOW) < 0)
        // clang-format on
        {
            Py_DECREF(module);

            return NULL;
        }
    }

    return module;
}
```


