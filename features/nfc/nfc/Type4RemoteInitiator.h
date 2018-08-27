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

#ifndef MBED_TYPE4_REMOTE_INITIATOR_H
#define MBED_TYPE4_REMOTE_INITIATOR_H

#include <stdint.h>
#include <stddef.h>

#include "ISO7816App.h"
#include "NFCNDEFCapable.h"
#include "NFCRemoteInitiator.h"

#include "acore/buffer.h"

namespace mbed {
namespace nfc {

/**
 * @addtogroup nfc
 * @{
 */

/**
 * This class is an implementation of the Type 4 tag application.
 */
class Type4RemoteInitiator : public NFCRemoteInitiator {
private:
    /**
     * Create a Type4RemoteInitiator.
     *
     * @param[in] controller pointer to the NFCController instance that created this object
     * @param[in] buffer a bytes array used to store NDEF messages
     * @param[in] buffer_size the array size in bytes
     */
    Type4RemoteInitiator(NFCController *controller, uint8_t *buffer, size_t buffer_size);

    // NFCRemoteEndpoint implementation
    virtual nfc_err_t connect();
    virtual nfc_err_t disconnect();
    virtual bool is_connected();
    virtual bool is_disconnected();
    virtual nfc_rf_protocols_bitmask_t rf_protocols();

    // NFCRemoteInitiator implementation
    virtual nfc_tag_type_t nfc_tag_type() const;
    virtual bool is_iso7816_supported() const;
    virtual void add_iso7816_application(ISO7816App *application);

    // NFCNDEFCapable implementation
    virtual bool is_ndef_supported() const;

    // Callbacks from NFC stack
    void disconnected_callback(bool deselected);
    static void s_disconnected_callback(nfc_tech_iso7816_t *pIso7816, bool deselected, void *pUserData);

    NFCController *_controller;
    bool _is_connected;
    bool _is_disconnected;
    nfc_tech_iso7816_t _iso7816;
    nfc_tech_type4_target_t _type4;
};

/**
 * @}
 */

} // namespace nfc
} // namespace mbed

#endif
