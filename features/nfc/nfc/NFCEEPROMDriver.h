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

#ifndef MBED_NFC_EEPROM_DRIVER_H
#define MBED_NFC_EEPROM_DRIVER_H

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
     * The abstraction for a NFC EEPROM driver.
     * Implementers need to derive from this class and implement its methods.
     */ 
    class NFCEEPROMDriver  {
    public:
        /**
         * Construct a NFCEEPROM driver instance.
         */
        NFCEEPROMDriver();
        
        /**
         * The NFCEEPROMDriver delegate.
         * Methods in this class are called by the driver on completion of long-running operations.
         */
        struct Delegate {
            /**
             * Completion of session start operation.
             * 
             * @param[in] success whether this operation succeeded
             */  
            virtual void on_session_started(bool success) = 0;

            /**
             * Completion of session end operation.
             * 
             * @param[in] success whether this operation succeeded
             */  
            virtual void on_session_ended(bool success) = 0;

            /**
             * Completion of read operation.
             * 
             * @param[in] count number of bytes actually read
             */  
            virtual void on_bytes_read(size_t count) = 0;

            /**
             * Completion of write operation.
             * 
             * @param[in] count number of bytes actually written
             */  
            virtual void on_bytes_written(size_t count) = 0;

            /**
             * Completion of size retrieval operation.
             * 
             * @param[in] success whether this operation succeeded
             * @param[out] the current addressable memory size
             */  
            virtual void on_size_read(bool success, size_t size) = 0;

            /**
             * Completion of size setting operation.
             * 
             * @param[in] success whether this operation succeeded
             */  
            virtual void on_size_written(bool success) = 0;

            /**
             * Completion of erasing operation.
             * 
             * @param[in] count number of bytes actually erased
             */  
            virtual void on_bytes_erased(size_t count) = 0;

            /**
             * Signal the user that the process_events() need to be called
             * 
             * @note this function can be called in interrupt context
             */
            virtual void on_event();
        };

        /** 
         * Set the delegate that will receive events generated by this EEPROM.
         * 
         * @oaram[in] delegate the delegate instance to use
         */ 
        void set_delegate(Delegate* delegate);

        /** 
         * Reset and initialize the EEPROM.
         * This method should complete synchronously.
         */ 
        virtual void reset() = 0;

        /**
         * Process events raised by the driver in interrupt context.
         */ 
        virtual void process_events();

        /**
         * Get the maximum memory size addressable by the EEPROM.
         */ 
        virtual size_t get_max_size() = 0;

        /**
         * Start a session of operations (reads, writes, erases, size gets/sets).
         * This method is called prior to any memory access to allow the underlying implementation 
         * to disable the RF interface or abort the transaction if it's being used. 
         * This method should complete asynchronously by calling has_started_session().
         */ 
        virtual void start_session() = 0; // This could lock the chip's RF interface

        /**
         * End a session.
         * This method should complete asynchronously by calling has_ended_session().
         */ 
        virtual void end_session() = 0;

        /**
         * Read bytes from memory.
         * @param[in] address the virtual address (starting from 0) from which to start the read.
         * @param[out] bytes a buffer in which the read bytes will be stored.
         *  This buffer should remain valid till the callback is called.
         * @oaram[in] count the number of bytes to read.
         * This method should complete asynchronously by calling has_read_bytes().
         */ 
        virtual void read_bytes(uint32_t address, uint8_t* bytes, size_t count) = 0;

        /**
         * Write bytes to memory.
         * @param[in] address the virtual address (starting from 0) from which to start the write.
         * @param[in] bytes a buffer from to copy.
         *  This buffer should remain valid till the callback is called.
         * @oaram[in] count the number of bytes to write.
         * This method should complete asynchronously by calling has_written_bytes().
         */ 
        virtual void write_bytes(uint32_t address, const uint8_t* bytes, size_t count) = 0;

        /**
         * Retrieve the size of the addressable memory.
         * This method should complete asynchronously by calling has_gotten_size().
         */ 
        virtual void read_size() = 0;

        /**
         * Set the size of the addressable memory.
         * @oaram[in] count the number of addressable bytes.
         * This method should complete asynchronously by calling has_set_size().
         */ 
        virtual void write_size(size_t count) = 0;

        /**
         * Erase bytes from memory.
         * @param[in] address the virtual address (starting from 0) from which to start erasing.
         * @oaram[in] count the number of bytes to erase.
         * This method should complete asynchronously by calling has_erased_bytes().
         */ 
        virtual void erase_bytes(uint32_t address, size_t size) = 0;

    protected:  
        Delegate* delegate();

    private:
        Delegate* _delegate;
    };

    /**
     * @}
     */

} // namespace nfc
} // namespace mbed

#endif
