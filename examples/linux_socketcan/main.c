/**
 * @file main.c
 * @brief SocketCAN example: send one ISO-TP request from a Linux PC and print the response.
 *
 * Project: ISOTP-C - Hardware-Agnostic ISO 15765-2 (ISO-TP) Library
 * Author: Anton Vynohradov
 * Email: avynohradov@systemfromscratch.com
 *
 * @copyright Copyright (c) 2026 Anton Vynohradov
 * @license MIT License
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
 */

/*
 * Usage:   isotp_request <can-interface> <tx-id> <rx-id> <hex bytes...>
 * Example: isotp_request can0 7E0 7E8 22 F1 90
 *
 * Opens a raw CAN socket, filters the response ID, sends the request through an
 * ISO-TP link and waits up to 2 s for a complete response. Try it without
 * hardware on a virtual bus:
 *   sudo ip link add dev vcan0 type vcan && sudo ip link set up vcan0
 *   isotp_request vcan0 7E0 7E8 3E 00     (with a responder on vcan0)
 */

/* ==============================================================================
 * INCLUDES
 * =============================================================================*/

#define _DEFAULT_SOURCE

#include <errno.h>
#include <net/if.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "isotp.h"

/* ==============================================================================
 * DEFINES & MACROS
 * =============================================================================*/

#define RESPONSE_TIMEOUT_US 2000000U
#define MAX_REQUEST 4095U

/* ==============================================================================
 * PRIVATE VARIABLES (static)
 * =============================================================================*/

static bool response_done;
static int exit_code = EXIT_FAILURE;

/* ==============================================================================
 * PRIVATE FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

static uint32_t micros(void)
{
    struct timespec ts;

    (void) clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t) ((uint64_t) ts.tv_sec * 1000000U + (uint64_t) ts.tv_nsec / 1000U);
}

static int socket_write(void* context, uint32_t can_id, const uint8_t* data, size_t length)
{
    const int fd = *(const int*) context;
    struct can_frame frame;
    int result = 0;

    memset(&frame, 0, sizeof(frame));
    frame.can_id = (can_id > CAN_SFF_MASK) ? (can_id | CAN_EFF_FLAG) : can_id;
    frame.can_dlc = (uint8_t) length;
    memcpy(frame.data, data, length);
    if (write(fd, &frame, sizeof(frame)) != (ssize_t) sizeof(frame))
    {
        result = ((errno == ENOBUFS) || (errno == EAGAIN)) ? ISOTP_BUSY : -errno;
    }
    return result;
}

static void on_response(const uint8_t* data, uint32_t length, void* context)
{
    (void) context;
    printf("response (%u bytes):", (unsigned) length);
    for (uint32_t i = 0U; i < length; i++)
    {
        printf(" %02X", data[i]);
    }
    printf("\n");
    response_done = true;
    exit_code = EXIT_SUCCESS;
}

static void on_tx_done(int n_result, void* context)
{
    (void) context;
    if (n_result != ISOTP_N_OK)
    {
        fprintf(stderr, "request failed: N_Result %d\n", n_result);
        response_done = true;
    }
}

static void on_rx_error(int n_result, void* context)
{
    (void) context;
    fprintf(stderr, "response failed: N_Result %d\n", n_result);
    response_done = true;
}

static int open_socket(const char* interface, uint32_t rx_id)
{
    struct sockaddr_can address;
    struct can_filter filter;
    struct ifreq ifr;
    int fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (fd < 0)
    {
        perror("socket");
        return -1;
    }
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1U);
    if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0)
    {
        perror(interface);
        close(fd);
        return -1;
    }
    filter.can_id = (rx_id > CAN_SFF_MASK) ? (rx_id | CAN_EFF_FLAG) : rx_id;
    filter.can_mask = (rx_id > CAN_SFF_MASK) ? (CAN_EFF_MASK | CAN_EFF_FLAG) : CAN_SFF_MASK;
    (void) setsockopt(fd, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));

    memset(&address, 0, sizeof(address));
    address.can_family = AF_CAN;
    address.can_ifindex = ifr.ifr_ifindex;
    if (bind(fd, (struct sockaddr*) &address, sizeof(address)) < 0)
    {
        perror("bind");
        close(fd);
        return -1;
    }
    return fd;
}

/* ==============================================================================
 * PUBLIC FUNCTION IMPLEMENTATIONS
 * =============================================================================*/

int main(int argc, char** argv)
{
    static struct isotp_link link;
    struct isotp_link_config config;
    uint8_t request[MAX_REQUEST];
    uint32_t length = 0U;
    int fd;
    uint32_t start;

    if (argc < 5)
    {
        fprintf(stderr, "usage: %s <can-interface> <tx-id> <rx-id> <hex bytes...>\n", argv[0]);
        return EXIT_FAILURE;
    }
    for (int i = 4; (i < argc) && (length < MAX_REQUEST); i++)
    {
        request[length] = (uint8_t) strtoul(argv[i], NULL, 16);
        length++;
    }

    fd = open_socket(argv[1], (uint32_t) strtoul(argv[3], NULL, 16));
    if (fd < 0)
    {
        return EXIT_FAILURE;
    }

    isotp_link_config_default(&config);
    config.tx_id = (uint32_t) strtoul(argv[2], NULL, 16);
    config.write = socket_write;
    config.write_context = &fd;
    config.on_message = on_response;
    config.on_tx_done = on_tx_done;
    config.on_rx_error = on_rx_error;
    config.padding = true; /* most ECUs expect 8-byte frames */
    if ((isotp_link_init(&link, &config) != ISOTP_OK)
        || (isotp_link_send(&link, request, length, micros()) != ISOTP_OK))
    {
        fprintf(stderr, "cannot send the request\n");
        close(fd);
        return EXIT_FAILURE;
    }

    start = micros();
    while (!response_done && ((micros() - start) < RESPONSE_TIMEOUT_US))
    {
        struct pollfd pfd = {fd, POLLIN, 0};

        if (poll(&pfd, 1, 1) > 0)
        {
            struct can_frame frame;
            if (read(fd, &frame, sizeof(frame)) == (ssize_t) sizeof(frame))
            {
                isotp_link_receive(&link, frame.data, frame.can_dlc, micros());
            }
        }
        isotp_link_poll(&link, micros());
    }
    if (!response_done)
    {
        fprintf(stderr, "no response within %u ms\n", RESPONSE_TIMEOUT_US / 1000U);
    }
    close(fd);
    return exit_code;
}
