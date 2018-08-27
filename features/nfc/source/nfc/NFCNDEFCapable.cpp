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

#include "NFCNDEFCapable.h"

#include "acore/ac_buffer.h"
#include "acore/ac_buffer_reader.h"
#include "acore/ac_buffer_builder.h"

#include "ndef/ndef.h"

using namespace mbed;
using namespace mbed::nfc;

NFCNDEFCapable::NFCNDEFCapable(uint8_t *buffer, size_t buffer_size) : _delegate(NULL)
{
    ndef_msg_init(&_ndef_message, s_ndef_encode, s_ndef_decode, buffer, buffer_size, this);
}

void NFCNDEFCapable::set_ndef_delegate(NFCNDEFCapable::Delegate *delegate)
{
    _delegate = delegate;
}

void NFCNDEFCapable::parse_ndef_message(const ac_buffer_t &buffer)
{
    ac_buffer_t reader;
    ac_buffer_dup(&reader, &buffer);
    if (_delegate != NULL) {
        _delegate->parse_ndef_message(ac_buffer_reader_current_buffer_pointer(&reader), ac_buffer_reader_current_buffer_length(&reader));
    }
}

void NFCNDEFCapable::build_ndef_message(ac_buffer_builder_t &buffer_builder)
{
    if (_delegate != NULL) {
        size_t count = _delegate->build_ndef_message(ac_buffer_builder_write_position(&buffer_builder), ac_buffer_builder_writable(&buffer_builder));
        ac_buffer_builder_write_n_skip(&buffer_builder, count);
    }
}

nfc_err_t NFCNDEFCapable::s_ndef_encode(ndef_msg_t *pTag, ac_buffer_builder_t *pBufferBldr, void *pUserData)
{
    NFCNDEFCapable *self = (NFCNDEFCapable *)pUserData;
    return self->ndef_encode(pBufferBldr);
}

nfc_err_t NFCNDEFCapable::s_ndef_decode(ndef_msg_t *pTag, ac_buffer_t *pBuffer, void *pUserData)
{
    NFCNDEFCapable *self = (NFCNDEFCapable *)pUserData;
    return self->ndef_decode(pBuffer);
}

nfc_err_t NFCNDEFCapable::ndef_encode(ac_buffer_builder_t *pBufferBldr)
{
    build_ndef_message(*pBufferBldr);
    return NFC_OK;
}

nfc_err_t NFCNDEFCapable::ndef_decode(ac_buffer_t *pBuffer)
{
    parse_ndef_message(*pBuffer);
    return NFC_OK;
}

ndef_msg_t *NFCNDEFCapable::ndef_message()
{
    return &_ndef_message;
}
