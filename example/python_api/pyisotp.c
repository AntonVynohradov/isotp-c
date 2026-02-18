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

/**
 * @brief Initializes an ISO-TP link and returns a capsule containing the link pointer to Python.
 *
 * @param self - Pointer to the module object (unused in this context)
 * @param args - Tuple of arguments passed from Python, expected to contain:
 *               - sendid (uint32_t): CAN ID to send on
 * @return PyObject* - Python object representing the result of the operation, typically a capsule
 * containing the IsoTpLink pointer or an error indicator
 */
static PyObject* pyisotp_init(PyObject* self, PyObject* args);

/**
 * @brief Sends data over an ISO-TP link.
 *
 * @param self - Pointer to the module object (unused in this context)
 * @param args - Tuple of arguments passed from Python, expected to contain:
 *               - py_link (PyObject*): Capsule containing the IsoTpLink pointer
 *               - data (bytes): Data to send
 * @return PyObject* - Python object representing the result of the operation, typically an integer
 * status code
 */
static PyObject* pyisotp_send(PyObject* self, PyObject* args);

/**
 * @brief Polls an ISO-TP link for timeouts and state updates.
 *
 * @param self - Pointer to the module object (unused in this context)
 * @param args - Tuple of arguments passed from Python, expected to contain:
 *               - py_link (PyObject*): Capsule containing the IsoTpLink pointer
 * @return PyObject* - Python object representing the result of the operation, typically None
 */
static PyObject* pyisotp_poll(PyObject* self, PyObject* args);

/**
 * @brief Receives data from an ISO-TP link.

 *
 * @param self - Pointer to the module object (unused in this context)
 * @param args - Tuple of arguments passed from Python, expected to contain:
 *               - py_link (PyObject*): Capsule containing the IsoTpLink pointer
 *               - bufsize (Py_ssize_t): Size of the buffer to receive data into
 * @return PyObject* - Python object representing the result of the operation, typically bytes
 * containing received data or None if no data
 */
static PyObject* pyisotp_receive(PyObject* self, PyObject* args);

/**
 * @brief Enables or disables CAN frame dropping in the mock CAN driver.
 *
 * @param self - Pointer to the module object (unused in this context)
 * @param args - Tuple of arguments passed from Python, expected to contain:
 *               - enable (int): Flag to enable or disable CAN frame dropping
 * @return PyObject* - Python object representing the result of the operation, typically None
 */
static PyObject* py_mock_enable_drop(PyObject* self, PyObject* args);

/**
 * @brief Enables or disables FlowControl frames in the mock CAN driver.
 */
static PyObject* py_mock_disable_fc(PyObject* self, PyObject* args);

/**
 * @brief Returns the last protocol result for the sender side.
 */
static PyObject* py_get_last_protocol_result(PyObject* self, PyObject* args);

/**
 * @brief Advances the virtual time in the mock time implementation by a specified delta.
 *
 * @param self - Pointer to the module object (unused in this context)
 * @param args - Tuple of arguments passed from Python, expected to contain:
 *               - delta (uint32_t): The number of milliseconds to advance the virtual time
 * @return PyObject* - Python object representing the result of the operation, typically None
 */
static PyObject* py_time_advance(PyObject* self, PyObject* args);

/**
 * @brief Sets the virtual time in the mock time implementation to a specific value.
 *
 * @param self - Pointer to the module object (unused in this context)
 * @param args - Tuple of arguments passed from Python, expected to contain:
 *               - value (uint32_t): The value to set the virtual time to, in milliseconds
 * @return PyObject* - Python object representing the result of the operation, typically None
 */
static PyObject* py_time_set(PyObject* self, PyObject* args);

/**
 * @brief Resets the virtual time in the mock time implementation to zero.
 *
 * @param self - Pointer to the module object (unused in this context)
 * @param args - Tuple of arguments passed from Python (expected to be empty for this function)
 * @return PyObject* - Python object representing the result of the operation, typically None
 */
static PyObject* py_time_reset(PyObject* self, PyObject* args);

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

static int g_mock_initialized = 0;

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

static PyObject* pyisotp_init(PyObject* self, PyObject* args)
{
    uint32_t sendid;
    Py_ssize_t sbufsz, rbufsz;
    if (!PyArg_ParseTuple(args, "Inn", &sendid, &sbufsz, &rbufsz))
    {
        return NULL;
    }

    if (!g_mock_initialized)
    {
        mock_can_init();
        mock_time_reset();
        g_mock_initialized = 1;
    }

    IsoTpLink* link = PyMem_Malloc(sizeof(IsoTpLink));
    uint8_t* sbuf = PyMem_Malloc(sbufsz);
    uint8_t* rbuf = PyMem_Malloc(rbufsz);

    if (!link || !sbuf || !rbuf)
    {
        PyMem_Free(link);
        PyMem_Free(sbuf);
        PyMem_Free(rbuf);

        return PyErr_NoMemory();
    }

    isotp_init_link(link, sendid, sbuf, (uint32_t) sbufsz, rbuf, (uint32_t) rbufsz);

    return PyCapsule_New(link, "IsoTpLink", NULL);
}

static PyObject* pyisotp_send(PyObject* self, PyObject* args)
{
    PyObject* py_link;
    const char* data;
    Py_ssize_t len;

    if (!PyArg_ParseTuple(args, "Oy#", &py_link, &data, &len))
    {
        return NULL;
    }

    IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");

    if (!link)
    {
        return NULL;
    }

    int res = isotp_send(link, (const uint8_t*) data, (uint32_t) len);

    return PyLong_FromLong(res);
}

static PyObject* pyisotp_poll(PyObject* self, PyObject* args)
{
    PyObject* py_link;
    if (!PyArg_ParseTuple(args, "O", &py_link))
    {
        return NULL;
    }

    IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");
    if (!link)
    {
        return NULL;
    }

    isotp_poll(link);

    uint32_t id = 0;
    uint8_t data[8];
    uint8_t len = 0;

    while (can_receive(&id, data, &len) == 0)
    {
        (void) id;
        isotp_on_can_message(link, data, len);
    }

    Py_RETURN_NONE;
}

static PyObject* pyisotp_receive(PyObject* self, PyObject* args)
{
    PyObject* py_link;
    Py_ssize_t bufsize;
    if (!PyArg_ParseTuple(args, "On", &py_link, &bufsize))
    {
        return NULL;
    }

    IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");
    if (!link)
    {
        return NULL;
    }

    if (bufsize <= 0)
    {
        Py_RETURN_NONE;
    }

    uint8_t* buf = PyMem_Malloc(bufsize);
    if (!buf)
    {
        return PyErr_NoMemory();
    }

    uint32_t outsz;
    int ret = isotp_receive(link, buf, (uint32_t) bufsize, &outsz);

    if (ret == ISOTP_RET_OK)
    {
        PyObject* result = Py_BuildValue("y#", buf, outsz);
        PyMem_Free(buf);
        return result;
    }

    PyMem_Free(buf);
    Py_RETURN_NONE;
}

static PyObject* py_mock_enable_drop(PyObject* self, PyObject* args)
{
    int enable;
    PyArg_ParseTuple(args, "p", &enable);
    mock_can_enable_drop(enable);
    Py_RETURN_NONE;
}

static PyObject* py_mock_disable_fc(PyObject* self, PyObject* args)
{
    int enable;
    PyArg_ParseTuple(args, "p", &enable);
    mock_can_disable_fc(enable);
    Py_RETURN_NONE;
}

static PyObject* py_time_advance(PyObject* self, PyObject* args)
{
    uint32_t delta_ms;
    PyArg_ParseTuple(args, "I", &delta_ms);
    uint32_t delta_us = (delta_ms * 1000U);
    if (delta_us > 0)
    {
        delta_us += 1U;
    }
    mock_time_advance(delta_us);
    Py_RETURN_NONE;
}

static PyObject* py_time_set(PyObject* self, PyObject* args)
{
    uint32_t value;
    PyArg_ParseTuple(args, "I", &value);
    mock_time_set(value * 1000U);
    Py_RETURN_NONE;
}

static PyObject* py_time_reset(PyObject* self, PyObject* args)
{
    mock_time_reset();
    Py_RETURN_NONE;
}

static PyObject* py_get_last_protocol_result(PyObject* self, PyObject* args)
{
    PyObject* py_link;
    if (!PyArg_ParseTuple(args, "O", &py_link))
    {
        return NULL;
    }

    IsoTpLink* link = PyCapsule_GetPointer(py_link, "IsoTpLink");
    if (!link)
    {
        return NULL;
    }
    return PyLong_FromLong(link->send_protocol_result);
}

/* ==============================================================================
 * PUBLIC FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

PyMODINIT_FUNC PyInit_pyisotp(void)
{
    PyObject* module = PyModule_Create(&pyisotp_module);
    if (!module)
    {
        return NULL;
    }

    mock_can_init();
    mock_time_reset();
    g_mock_initialized = 1;

    PyModule_AddIntConstant(module, "ISOTP_PROTOCOL_RESULT_TIMEOUT_BS",
                            ISOTP_PROTOCOL_RESULT_TIMEOUT_BS);

    return module;
}