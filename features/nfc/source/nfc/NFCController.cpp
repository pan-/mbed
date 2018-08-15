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

#include "NFCController.h"
#include "NFCControllerDriver.h"
#include "Type4RemoteInitiator.h"

#include "stack/transceiver/transceiver.h"

using namespace mbed;
using namespace mbed::nfc;

NFCController::NFCController(NFCControllerDriver* driver, events::EventQueue* queue) : 
    _driver(driver), _queue(queue), _transceiver(NULL), _delegate(NULL), _discovery_running(false)
{

}

void NFCController::initialize()
{
    MBED_ASSERT(_transceiver == NULL); // Initialize should only be called once
    _transceiver = _driver->initialize((nfc_scheduler_timer_t*)&_timer);
}

void NFCController::set_delegate(Delegate* delegate)
{
    _delegate = delegate;
}

nfc_rf_protocols_bitmask_t NFCController::get_supported_rf_protocols() const
{
    // nfc_rf_protocols_bitmask_t is mapped on NFC Forum types, nfc_tech_t is mapped on the underlying RF techs
    // We therefore need to convert these

    nfc_rf_protocols_bitmask_t rf_protocols = {0};
    nfc_tech_t initiator_tech;
    nfc_tech_t target_tech;
    _driver->get_supported_nfc_techs(&initiator_tech, &target_tech);

    // Note: we only support ISO-DEP tag emulation in this release,
    // so mask out all other protocols

    // rf_protocols.initiator_t1t = initiator_tech.nfc_type1;
    // rf_protocols.initiator_t2t = initiator_tech.nfc_type2;
    // rf_protocols.initiator_t3t = initiator_tech.nfc_type3;
    // rf_protocols.initiator_iso_dep = initiator_tech.nfc_iso_dep_a || initiator_tech.nfc_iso_dep_b;
    // rf_protocols.initiator_nfc_dep = initiator_tech.nfc_nfc_dep_a || initiator_tech.nfc_nfc_dep_f_212 || initiator_tech.nfc_nfc_dep_f_424;
    
    // rf_protocols.target_t1t = target_tech.nfc_type1;
    // rf_protocols.target_t2t = target_tech.nfc_type2;
    // rf_protocols.target_t3t = target_tech.nfc_type3;
    rf_protocols.target_iso_dep = target_tech.nfc_iso_dep_a || target_tech.nfc_iso_dep_b;
    // rf_protocols.target_nfc_dep = target_tech.nfc_nfc_dep_a || target_tech.nfc_nfc_dep_f_212 || target_tech.nfc_nfc_dep_f_424;
    
    return rf_protocols;
}

nfc_err_t NFCController::configure_rf_protocols(nfc_rf_protocols_bitmask_t rf_protocols)
{
    if( _discovery_running ) {
        // Cannot configure RF protocols if discovery is running
        return NFC_ERR_BUSY;
    }

    // Map to NFC techs
    nfc_tech_t initiator_tech = {0};
    nfc_tech_t target_tech = {0};

    // Note: we only support ISO-DEP tag emulation in this release,
    // so mask out all other protocols

    target_tech.nfc_iso_dep_a = target_tech.nfc_iso_dep_b =  true;

    // Configure polling options (no need to set bailing flags as we're only using target mode)
    polling_options_t options = {0};

    transceiver_set_protocols(_transceiver, initiator_tech, target_tech, options);
}

nfc_err_t NFCController::start_discovery()
{
    if( _discovery_running ) {
        // Cannot start discovery if it's already running
        return NFC_ERR_BUSY;
    }

    transceiver_poll(_transceiver, &NFCController::s_polling_callback, this /* use this as callback argument */);
}

nfc_err_t NFCController::cancel_discovery()
{
    if( !_discovery_running ) {
        return NFC_OK;
    }

    transceiver_abort(_transceiver);
}

nfc_transceiver_t* NFCController::transceiver() const
{
    return _transceiver;
}

void NFCController::polling_callback(nfc_err_t ret)
{
    // Polling has completed
    _discovery_running = false;

    if( ret == NFC_OK ) {
        // Check if a remote initiator was detected and if so, instantiate it
        if( !transceiver_is_initiator_mode(_transceiver) ) {        
            nfc_tech_t active_tech = transceiver_get_active_techs(_transceiver);
            if( active_tech.nfc_iso_dep_a || active_tech.nfc_iso_dep_b ) {
                SharedPtr<Type4RemoteInitiator> type4_remote_initiator( new Type4RemoteInitiator(_transceiver) );
                if( _delegate != NULL ) {
                    _delegate->on_nfc_initiator_discovered(type4_remote_initiator);
                }
            }
        }
    }

    if( _delegate != NULL ) {
        nfc_discovery_terminated_reason_t reason;
        
        // Map reason
        switch(ret) {
            case NFC_OK:
                reason = nfc_discovery_terminated_completed;
                break;
            case NFC_ERR_ABORTED:
                reason = nfc_discovery_terminated_canceled;
                break;
            default:
                // Any other error code means there was an error during the discovery process
                reason = nfc_discovery_terminated_rf_error;
                break;
        }

        _delegate->on_discovery_terminated(reason);
    }
}

static void NFCController::s_polling_callback(nfc_transceiver_t* pTransceiver, nfc_err_t ret, void* pUserData)
{
    NFCController* self = (NFCController*) pUserData;
    self->polling_callback(ret);
}