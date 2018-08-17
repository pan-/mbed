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

#ifndef MBED_NFC_EEPROM_H
#define MBED_NFC_EEPROM_H

#include <stdint.h>

#include "NFCDefinitions.h"
#include "NFCTarget.h"
#include "NFCEEPROMDriver.h"

namespace mbed {
namespace nfc {

    /**
     * @addtogroup nfc
     * @{
     */

    /** 
     * The NFC EEPROM class represents a NFC target device connected using a wired
     * link (I2C, SPI, etc).
     * 
     * These EEPROMs essentially provide addressable memory that can be accessed
     * using either a wired or NFC interface.
     * 
     * In NFC mode these most often conform to one of the NFC tag types defined 
     * by the NFC Forum, therefore encoding NDEF data in these EEPROMs will 
     * ensure that it is understandable by a NFC reader.
     */ 
    class NFCEEPROM : public NFCTarget, public NFCEEPROMDriver::Delegate {
    public:
        /** 
         * Construct a NFCEEPROM instance.
         * 
         * @param[in] driver a pointer to a NFCEEPROMDriver instance
         * @param[in] queue a pointer to the events queue to use
         * @param[in] ndef_buffer a bytes array used to store NDEF messages
         * @param[in] ndef_buffer_sz the array size in bytes
         */  
        NFCEEPROM(NFCEEPROMDriver* driver, events::EventQueue* queue, uint8_t* ndef_buffer, size_t ndef_buffer_sz);

        /**
         * The NFCEEPROM delegate. Users of the NFCEEPROM class need to implement this delegate's methods to receive events.
         */
        struct Delegate : NFCTarget::Delegate {
        };

        /** 
         * Initialize the NFC EEPROM
         * 
         * This method must be called before any other method call.
         *
         * @return NFC_OK, or an error.
         */
        nfc_err_t initialize();

        /** 
         * Set the delegate that will receive events generated by this EEPROM.
         * 
         * @oaram[in] delegate the delegate instance to use
         */ 
        void set_delegate(Delegate* delegate);

    private:
        // Implementation of NFCTarget
        virtual void write_ndef_message();
        virtual void read_ndef_message();
        virtual void erase_ndef_message();

        // Implementation of NFCEEPROMDriver::Delegate
        virtual void on_session_started(bool success);
        virtual void on_session_ended(bool success);
        virtual void on_bytes_read(size_t count);
        virtual void on_bytes_written(size_t count);
        virtual void on_size_written(bool success);
        virtual void on_size_read(bool success, size_t size);
        virtual void on_bytes_erased(size_t count);
        virtual void on_event();

        void handle_error(nfc_err_t ret);
        void continue_write();
        void continue_read();
        void continue_erase();

        enum class nfc_eeprom_operation_t {
            nfc_eeprom_idle,

            nfc_eeprom_write_start_session,
            nfc_eeprom_write_write_bytes,
            nfc_eeprom_write_write_size,
            nfc_eeprom_write_end_session,

            nfc_eeprom_write_start_session,
            nfc_eeprom_read_read_size,
            nfc_eeprom_read_read_bytes,
            nfc_eeprom_read_end_session,

            nfc_eeprom_erase_start_session,
            nfc_eeprom_erase_erase_bytes,
            nfc_eeprom_erase_write_size,
            nfc_eeprom_erase_end_session
        };

        NFCEEPROMDriver* _driver;
        events::EventQueue* _queue;
        bool _initialized;
        uint8_t* _ndef_buffer;
        size_t _ndef_buffer_sz;

        nfc_eeprom_operation_t _current_op;
        buffer_t _ndef_buffer_reader;
        size_t _ndef_buffer_read_sz;
        uint32_t _eeprom_address;
        nfc_err_t _operation_result;
    };

    /**
     * @}
     */


} // namespace nfc
} // namespace mbed

#endif
