/**
 * @file hal_func.c
 * @brief Hardware abstraction layer utility function implementation.
 *
 * This source file implements common utility functions used throughout the
 * hardware abstraction layer (HAL). These functions provide generic services
 * that are independent of the underlying hardware platform and are shared by
 * both the HAL core and platform-specific backends.
 *
 * The utilities implemented by this module include helper routines for
 * converting HAL data types into human-readable representations, validating
 * parameters, and performing other common operations that are not specific to
 * any supported microcontroller family.
 *
 * Responsibilities include:
 * - Enumeration-to-string conversion
 * - Common helper routines
 * - Generic HAL utility functions
 * - Shared support functionality
 *
 * This module is platform-independent and is compiled for all supported HAL
 * targets.
 *
 * @ingroup HAL
 *
 * @author Michael Van Gool
 * @date 2026-07-07
 *
 * MIT License
 *
 * Copyright (c) 2026 Michael Van Gool
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "hal.h"

const char* hal_log_level_to_string(hal_log_level_t level)
{
    switch (level)
    {
        case HAL_LOG_ERR:   return "ERROR";
        case HAL_LOG_WARN:  return "WARN";
        case HAL_LOG_INFO:  return "INFO";
        case HAL_LOG_DEBUG: return "DEBUG";
    
        default:            return "<INVALID>";
    }
}
