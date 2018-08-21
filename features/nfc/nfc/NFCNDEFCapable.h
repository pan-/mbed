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

#ifndef MBED_NFC_NDEF_CAPABLE_H
#define MBED_NFC_NDEF_CAPABLE_H

#include <stdint.h>

#include "NFCDefinitions.h"

namespace mbed {
namespace nfc {

/**
 * @addtogroup nfc
 * @{
 */

/**
 * The base class for all endpoints that can support NDEF content.
 */
class NFCNDEFCapable {
public:
    /**
     * Construct a NFCNDEFCapable instance.
     * @param[in] buffer a bytes array used to store NDEF messages
     * @param[in] buffer_size the array size in bytes
     */
    NFCNDEFCapable(uint8_t *buffer, size_t buffer_size);

    /**
     * Check if this instance actually supports NDEF content.
     *
     * @return whether NDEF content is supported
     */
    virtual bool is_ndef_supported() const
    {
        return false;
    }

    struct Delegate {
        /**
         * Parse a NDEF message.
         *
         * @param[in] buffer a buffer containing the message to parse
         * @param[in] size the buffer's size
         */
        virtual void parse_ndef_message(const uint8_t *buffer, size_t size) { }

        /**
         * Build a NDEF message.
         *
         * @param[in] buffer a mutable buffer in which the message should be stored
         * @param[in] capacity the buffer's capacity
         * @return the number of bytes actually used
         */
        virtual size_t build_ndef_message(uint8_t *buffer, size_t capacity)
        {
            return 0;
        }
    };

    /**
     * Set the delegate that will receive events generated by this class.
     *
     * @oaram[in] delegate the delegate instance to use
     */
    void set_ndef_delegate(Delegate *delegate);

protected:
    /**
     * Parse a NDEF message.
     *
     * @param[in] a buffer containing a NDEF message
     */
    void parse_ndef_message(const ac_buffer_t &buffer);

    /**
     * Build NDEF message.
     *
     * @param[in,out] a buffer builder in which to create the NDEF message.
     * The backing buffer is guaranteed to be continuous.
     */
    void build_ndef_message(ac_buffer_builder_t &buffer_builder);

    /**
     * Retrieve underlying NDEF message instance
     * @return pointer to NDEF message instance
     */
    ndef_msg_t *ndef_message();

private:
    // Callbacks from NDEF stack
    static nfc_err_t s_ndef_encode(ndef_msg_t *pTag, buffer_builder_t *pBufferBldr, void *pUserData);
    static nfc_err_t s_ndef_decode(ndef_msg_t *pTag, buffer_t *pBuffer, void *pUserData);
    nfc_err_t ndef_encode(buffer_builder_t *pBufferBldr);
    nfc_err_t ndef_decode(buffer_t *pBuffer);


    Delegate *_delegate;
    ndef_msg_t _ndef_message;
};

/**
 * @}
 */

} // namespace nfc
} // namespace mbed

#endif
