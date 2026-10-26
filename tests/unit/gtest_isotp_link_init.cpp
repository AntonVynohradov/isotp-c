/**
 * @file gtest_isotp_link_init.cpp
 * @brief Unit tests: link configuration, initialization and accessors.
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

#include "isotp_test_support.h"

TEST(IsotpLinkInit, DefaultsFromConfigHeader)
{
    struct isotp_link_config config;

    isotp_link_config_default(&config);
    EXPECT_EQ(config.tx_id, 0U);
    EXPECT_EQ(config.write, nullptr);
    EXPECT_EQ(config.on_message, nullptr);
    EXPECT_EQ(config.block_size, ISOTP_DEFAULT_BLOCK_SIZE);
    EXPECT_EQ(config.st_min_us, ISOTP_DEFAULT_ST_MIN_US);
    EXPECT_EQ(config.n_bs_timeout_us, ISOTP_DEFAULT_N_BS_TIMEOUT_US);
    EXPECT_EQ(config.n_cr_timeout_us, ISOTP_DEFAULT_N_CR_TIMEOUT_US);
    EXPECT_EQ(config.wft_max, ISOTP_DEFAULT_WFT_MAX);
    EXPECT_FALSE(config.padding);
    EXPECT_EQ(config.padding_value, 0xCCU);

    isotp_link_config_default(nullptr); /* must not crash */
}

TEST(IsotpLinkInit, AcceptsValidConfiguration)
{
    Capture capture;
    std::unique_ptr<struct isotp_link> link(new struct isotp_link());
    struct isotp_link_config config = make_config(&capture);

    config.st_min_us = 127000U;
    config.n_bs_timeout_us = 0x7FFFFFFFUL;
    config.n_cr_timeout_us = 1U;
    ASSERT_EQ(isotp_link_init(link.get(), &config), ISOTP_OK);
    EXPECT_FALSE(isotp_link_tx_busy(link.get()));
    EXPECT_EQ(isotp_link_tx_result(link.get()), ISOTP_N_OK);
    EXPECT_EQ(isotp_link_rx_result(link.get()), ISOTP_N_OK);
    EXPECT_EQ(isotp_link_invalid_frames(link.get()), 0U);
}

TEST(IsotpLinkInit, RejectsInvalidConfiguration)
{
    Capture capture;
    std::unique_ptr<struct isotp_link> link(new struct isotp_link());
    const struct isotp_link_config good = make_config(&capture);
    struct isotp_link_config config = good;

    EXPECT_EQ(isotp_link_init(nullptr, &config), ISOTP_ERROR);
    EXPECT_EQ(isotp_link_init(link.get(), nullptr), ISOTP_ERROR);

    config.write = nullptr;
    EXPECT_EQ(isotp_link_init(link.get(), &config), ISOTP_ERROR);
    config = good;
    config.st_min_us = 127001U;
    EXPECT_EQ(isotp_link_init(link.get(), &config), ISOTP_ERROR);
    config = good;
    config.n_bs_timeout_us = 0U;
    EXPECT_EQ(isotp_link_init(link.get(), &config), ISOTP_ERROR);
    config = good;
    config.n_bs_timeout_us = 0x80000000UL;
    EXPECT_EQ(isotp_link_init(link.get(), &config), ISOTP_ERROR);
    config = good;
    config.n_cr_timeout_us = 0U;
    EXPECT_EQ(isotp_link_init(link.get(), &config), ISOTP_ERROR);
    config = good;
    config.n_cr_timeout_us = 0x80000000UL;
    EXPECT_EQ(isotp_link_init(link.get(), &config), ISOTP_ERROR);
}

TEST(IsotpLinkInit, AccessorsTolerateNull)
{
    EXPECT_FALSE(isotp_link_tx_busy(nullptr));
    EXPECT_EQ(isotp_link_tx_result(nullptr), ISOTP_N_ERROR);
    EXPECT_EQ(isotp_link_rx_result(nullptr), ISOTP_N_ERROR);
    EXPECT_EQ(isotp_link_invalid_frames(nullptr), 0U);
    isotp_link_poll(nullptr, 0U);
    isotp_link_receive(nullptr, nullptr, 0U, 0U);
}

TEST(IsotpLinkInit, ReinitClearsState)
{
    Capture capture;
    std::unique_ptr<struct isotp_link> link(new struct isotp_link());
    struct isotp_link_config config = make_config(&capture);
    const std::vector<uint8_t> data = pattern(20);

    ASSERT_EQ(isotp_link_init(link.get(), &config), ISOTP_OK);
    ASSERT_EQ(isotp_link_send(link.get(), data.data(), 20U, 0U), ISOTP_OK);
    EXPECT_TRUE(isotp_link_tx_busy(link.get()));
    ASSERT_EQ(isotp_link_init(link.get(), &config), ISOTP_OK);
    EXPECT_FALSE(isotp_link_tx_busy(link.get()));
}
