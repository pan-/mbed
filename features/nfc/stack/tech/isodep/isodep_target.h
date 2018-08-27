/*
 * Copyright (c) 2015-2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file isodep_target.h
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#ifndef ISODEP_TARGET_H_
#define ISODEP_TARGET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/nfc.h"
#include "transceiver/transceiver.h"
#include "isodep.h"

struct nfc_tech_isodep_target;
typedef struct nfc_tech_isodep_target nfc_tech_isodep_target_t;

typedef struct nfc_tech_isodep_target nfc_tech_isodep_target_t;
struct nfc_tech_isodep_target
{
  nfc_tech_isodep_t isodep;
  nfc_transceiver_t* pTransceiver;

  struct
  {
    ostream_t* pReqStream;
    istream_t* pResStream;

    nfc_tech_isodep_cb_t reqCb;
    void* pReqUserData;

    nfc_tech_isodep_cb_t resCb;
    void* pResUserData;

    buffer_t res;
    bool chaining;

    uint8_t blockNumber;

    enum
    {
      ISO_DEP_TARGET_DEP_FRAME_IDLE,
      ISO_DEP_TARGET_DEP_FRAME_WTX_RECEIVED,
      ISO_DEP_TARGET_DEP_FRAME_WTX_SENT,
      ISO_DEP_TARGET_DEP_FRAME_INFORMATION_RECEIVED,
      ISO_DEP_TARGET_DEP_FRAME_INFORMATION_SENT,
      ISO_DEP_TARGET_DEP_FRAME_NACK_RECEIVED,
	    ISO_DEP_TARGET_DEP_FRAME_NACK_DIFF_BLOCK_NUMBER_RECEIVED,
      ISO_DEP_TARGET_DEP_FRAME_NACK_SENT,
      ISO_DEP_TARGET_DEP_FRAME_ACK_RECEIVED,
      ISO_DEP_TARGET_DEP_FRAME_ACK_SENT,
      ISO_DEP_TARGET_DEP_FRAME_DESELECT_RECEIVED,
      ISO_DEP_TARGET_DEP_FRAME_DESELECT_SENT,
    } frameState;
  } dep;
  struct
  {
    enum
    {
      ISO_DEP_TARGET_COMMANDS_DISCONNECTED,

      ISO_DEP_TARGET_COMMANDS_CONNECTING,

      ISO_DEP_TARGET_COMMANDS_ATS_REQ_RECVD,
      ISO_DEP_TARGET_COMMANDS_ATS_RES_SENT,

      ISO_DEP_TARGET_COMMANDS_DEP_REQ_RECVD,
      ISO_DEP_TARGET_COMMANDS_DEP_RES_SENT,
    } state;

    size_t inPayloadSize;

    buffer_builder_t respBldr;
    uint8_t respBuf[32];

    buffer_t* pReq;
  } commands;

  buffer_t* pHist;

  nfc_tech_isodep_disconnected_cb disconnectedCb;
  void* pUserData;
};

//High-level Target functions
void nfc_tech_isodep_target_init(nfc_tech_isodep_target_t* pIsodepTarget, nfc_transceiver_t* pTransceiver,
    buffer_t* pHist, nfc_tech_isodep_disconnected_cb disconnectedCb, void* pUserData);

nfc_err_t nfc_tech_isodep_target_connect(nfc_tech_isodep_target_t* pIsodepTarget);
void nfc_tech_isodep_target_disconnect(nfc_tech_isodep_target_t* pIsodepTarget);

nfc_err_t nfc_tech_isodep_target_transmit(nfc_tech_isodep_target_t* pIsodepTarget, istream_t* pStream, nfc_tech_isodep_cb_t cb, void* pUserData);
nfc_err_t nfc_tech_isodep_target_receive(nfc_tech_isodep_target_t* pIsodepTarget, ostream_t* pStream, nfc_tech_isodep_cb_t cb, void* pUserData);


#ifdef __cplusplus
}
#endif

#endif /* ISODEP_TARGET_H_ */
