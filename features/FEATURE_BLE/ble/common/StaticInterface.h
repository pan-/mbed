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

#ifndef BLE_COMMON_STATICINTERFACE_H
#define BLE_COMMON_STATICINTERFACE_H

namespace ble {

template <typename Impl, template<typename> class Interface>
struct StaticInterface
{
    Impl* impl()
    {
        return static_cast<Impl*>(this);
    }

    const Impl* impl() const
    {
        return static_cast<const Impl*>(this);
    }

private:
    StaticInterface() {}
    friend Interface<Impl>;
};

} // namespace ble

#endif //BLE_COMMON_STATICINTERFACE_H
