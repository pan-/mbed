/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ARM_GAP_H_
#define _ARM_GAP_H_

#include "mbed.h"
#include "blecommon.h"
#include "GapAdvertisingParams.h"
#include "GapAdvertisingData.h"
#include "ble/Gap.h"
#include "GapScanningParams.h"
#include "dm_api.h"
#include "att_api.h"

/**************************************************************************/
/*!
    \brief

*/
/**************************************************************************/
class ArmGap : public ble::interface::LegacyGap<ArmGap>
{
public:
    static ArmGap &getInstance();

    /* Functions that must be implemented from Gap */
    ble_error_t setAddress_(AddressType_t  type,  const Address_t address);
    ble_error_t getAddress_(AddressType_t *typeP, Address_t address);
    ble_error_t setAdvertisingData_(const GapAdvertisingData &, const GapAdvertisingData &);

    #define BLE_GAP_ADV_INTERVAL_MIN        0x0020 /**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */
    #define BLE_GAP_ADV_NONCON_INTERVAL_MIN 0x00A0 /**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */
    #define BLE_GAP_ADV_INTERVAL_MAX        0x4000 /**< Armum Advertising interval in 625 us units, i.e. 10.24 s. */

    uint16_t getMinAdvertisingInterval_(void) const { return BLE_GAP_ADV_INTERVAL_MIN; }
    uint16_t getMinNonConnectableAdvertisingInterval_(void) const { return BLE_GAP_ADV_NONCON_INTERVAL_MIN; }
    uint16_t getMaxAdvertisingInterval_(void) const { return BLE_GAP_ADV_INTERVAL_MAX; }

    ble_error_t startAdvertising_(const GapAdvertisingParams &);
    ble_error_t stopAdvertising_(void);
    ble_error_t disconnect_(Handle_t connectionHandle, DisconnectionReason_t reason);
    ble_error_t disconnect_(DisconnectionReason_t reason);

    ble_error_t setDeviceName_(const uint8_t *deviceName);
    ble_error_t getDeviceName_(uint8_t *deviceName, unsigned *lengthP);
    ble_error_t setAppearance_(GapAdvertisingData::Appearance appearance);
    ble_error_t getAppearance_(GapAdvertisingData::Appearance *appearanceP);

    ble_error_t setTxPower_(int8_t txPower);
    void        getPermittedTxPowerValues_(const int8_t **valueArrayPP, size_t *countP);

    void     setConnectionHandle(uint16_t m_connectionHandle);
    uint16_t getConnectionHandle(void);

    ble_error_t getPreferredConnectionParams_(ConnectionParams_t *params);
    ble_error_t setPreferredConnectionParams_(const ConnectionParams_t *params);
    ble_error_t updateConnectionParams_(Handle_t handle, const ConnectionParams_t *params);

    ble_error_t startRadioScan_(const GapScanningParams &scanningParams);
    ble_error_t stopScan_(void);

    void     advertisingStopped(void);

protected:
    // import from Gap
    friend ble::interface::Gap<ArmGap>;

    using ble::interface::Gap<ArmGap>::startAdvertising_;
    using ble::interface::Gap<ArmGap>::stopAdvertising_;
    using ble::interface::Gap<ArmGap>::connect_;
    using ble::interface::Gap<ArmGap>::disconnect_;

    // import from LegacyGap
    friend ble::interface::LegacyGap<ArmGap>;

    using ble::interface::LegacyGap<ArmGap>::startAdvertising_;
    using ble::interface::LegacyGap<ArmGap>::stopAdvertising_;
    using ble::interface::LegacyGap<ArmGap>::connect_;
    using ble::interface::LegacyGap<ArmGap>::disconnect_;

private:
    uint16_t m_connectionHandle;
    addr_type_t m_type;
    ArmGap() {
        m_connectionHandle = DM_CONN_ID_NONE;
        m_type = BLEProtocol::AddressType::RANDOM_STATIC;
    }

    ArmGap(ArmGap const &);
    void operator=(ArmGap const &);
};

#endif /* _ARM_GAP_H_ */
