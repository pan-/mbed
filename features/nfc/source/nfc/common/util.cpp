/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "nfc/common/util.h"

#include <cstring>

namespace mbed {
namespace nfc {
namespace common {

Span<const uint8_t> span_from_cstr(const char *cstr)
{
    return Span<const uint8_t>((const uint8_t*)cstr, strlen(cstr));
}

/**
 * @}
 */

} // namespace common
} // namespace nfc
} // namespace mbed

