/*
Copyright (c) 2017, Witold Gebarowski.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "ble_security_system.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_util.h"
#include "app_util_bds.h"

#define OPCODE_LENGTH 1 /**< Length of opcode inside security_system packet. */
#define HANDLE_LENGTH 2 /**< Length of handle inside security_system packet. */

/* TODO Consider changing the max values if encoded data for characteristic/descriptor is fixed length */ 
#define MAX_MOVE_SENSORS_LEN (BLE_L2CAP_MTU_DEF - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted move_sensors. */ 
#define MAX_DOOR_LOCK_LEN (BLE_L2CAP_MTU_DEF - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted door_lock. */ 
#define MAX_DOOR_CHECK_LEN (BLE_L2CAP_MTU_DEF - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted door_check. */ 

/**@brief Function for encoding move_sensors.
 *
 * @param[in]   p_move_sensors              move_sensors characteristic structure to be encoded.
 * @param[out]  p_encoded_buffer   Buffer where the encoded data will be written.
 *
 * @return      Size of encoded data.
 */
static uint8_t move_sensors_encode(ble_security_system_move_sensors_t * p_move_sensors, uint8_t * encoded_buffer)
{
    uint8_t len = 0; 
    encoded_buffer[len++] = p_move_sensors->sensor_1;
    return len;
}

/**@brief Function for encoding door_lock.
 *
 * @param[in]   p_door_lock              door_lock characteristic structure to be encoded.
 * @param[out]  p_encoded_buffer   Buffer where the encoded data will be written.
 *
 * @return      Size of encoded data.
 */
static uint8_t door_lock_encode(ble_security_system_door_lock_t * p_door_lock, uint8_t * encoded_buffer)
{
    uint8_t len = 0; 
    encoded_buffer[len++] = p_door_lock->door_closed;
    return len;
}

/**@brief Function for decoding door_lock.
 *
 * @param[in]   data_len              Length of the field to be decoded.
 * @param[in]   p_data                Buffer where the encoded data is stored.
 * @param[out]  p_write_val           Decoded data.
 *
 * @return      Length of the decoded field.
 */
static uint8_t door_lock_decode(uint8_t data_len, uint8_t * p_data, ble_security_system_door_lock_t * p_write_val)
{
    uint8_t pos = 0;
    p_write_val->door_closed = p_data[pos++]; 

    return pos;
} 
/**@brief Function for encoding door_check.
 *
 * @param[in]   p_door_check              door_check characteristic structure to be encoded.
 * @param[out]  p_encoded_buffer   Buffer where the encoded data will be written.
 *
 * @return      Size of encoded data.
 */
static uint8_t door_check_encode(ble_security_system_door_check_t * p_door_check, uint8_t * encoded_buffer)
{
    uint8_t len = 0; 
    encoded_buffer[len++] = p_door_check->door_control;
    return len;
}


/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_security_system       security_system Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_security_system_t * p_security_system, ble_evt_t * p_ble_evt)
{
    p_security_system->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_security_system       security_system Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_security_system_t * p_security_system, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_security_system->conn_handle = BLE_CONN_HANDLE_INVALID;
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_security_system       security_system Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_security_system_t * p_security_system, ble_gatts_evt_write_t * p_ble_evt)
{
    
    if(p_ble_evt->handle == p_security_system->door_lock_handles.value_handle)
    {
        if(p_security_system->evt_handler != NULL)
        {
            ble_security_system_evt_t evt;
            evt.evt_type = BLE_SECURITY_SYSTEM_DOOR_LOCK_EVT_WRITE;
            door_lock_decode(p_ble_evt->len, p_ble_evt->data, &evt.params.door_lock);
            p_security_system->evt_handler(p_security_system, &evt);
        }
    }
}

/**@brief Authorize WRITE request event handler.
 *
 * @details Handles WRITE events from the BLE stack.
 *
 * @param[in]   p_sc_ctrlpt  SC Ctrlpt structure.
 * @param[in]   p_gatts_evt  GATTS Event received from the BLE stack.
 *
 */
static void on_rw_authorize_request(ble_security_system_t * p_security_system, ble_gatts_evt_t * p_gatts_evt)
{
    ble_gatts_evt_rw_authorize_request_t * p_auth_req = &p_gatts_evt->params.authorize_request;
    if (p_auth_req->type == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
    {
        if (   (p_gatts_evt->params.authorize_request.request.write.op
                != BLE_GATTS_OP_PREP_WRITE_REQ)
            && (p_gatts_evt->params.authorize_request.request.write.op
                != BLE_GATTS_OP_EXEC_WRITE_REQ_NOW)
            && (p_gatts_evt->params.authorize_request.request.write.op
                != BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL)
           )
        {
        
            if (p_auth_req->request.write.handle == p_security_system->door_lock_handles.value_handle)
            {
                on_write(p_security_system, &p_auth_req->request.write);
            }
        }
    }
}

/**@brief Function for handling BLE events.
 *
 * @param[in]   p_security_system       security_system Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
void ble_security_system_on_ble_evt(ble_security_system_t * p_security_system, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_security_system, p_ble_evt);
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_security_system, p_ble_evt);
            break;
        case BLE_GATTS_EVT_WRITE:
            on_write(p_security_system, &p_ble_evt->evt.gatts_evt.params.write);
            break;
         case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            on_rw_authorize_request(p_security_system, &p_ble_evt->evt.gatts_evt);
            break;
        default:
            //No implementation needed.
            break;
    }
}

/**@brief Function for initializing the security_system. */
uint32_t ble_security_system_init(ble_security_system_t * p_security_system, const ble_security_system_init_t * p_security_system_init)
{
    uint32_t err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_security_system->evt_handler = p_security_system_init->evt_handler;
    p_security_system->conn_handle = BLE_CONN_HANDLE_INVALID;
    
    BLE_UUID_BLE_ASSIGN(ble_uuid, 0x1811);
        
    // Add service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_security_system->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    } 

    // Add move_sensors characteristic
    ble_security_system_move_sensors_t move_sensors_initial_value = p_security_system_init->ble_security_system_move_sensors_initial_value; 

    uint8_t move_sensors_encoded_value[1];
    ble_add_char_params_t add_move_sensors_params;
    memset(&add_move_sensors_params, 0, sizeof(add_move_sensors_params));
    
    add_move_sensors_params.uuid                = 0x2A46; 
    add_move_sensors_params.max_len             = 8;
    add_move_sensors_params.init_len            = move_sensors_encode(&move_sensors_initial_value, move_sensors_encoded_value);
    add_move_sensors_params.p_init_value        = move_sensors_encoded_value; 
    add_move_sensors_params.char_props.read     = 1; 
    add_move_sensors_params.read_access         = SEC_OPEN; 
    // 1 for variable length and 0 for fixed length.
    add_move_sensors_params.is_var_len          = 1; 

    err_code = characteristic_add(p_security_system->service_handle, &add_move_sensors_params, &(p_security_system->move_sensors_handles));
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    } 

    // Add door_lock characteristic
    ble_security_system_door_lock_t door_lock_initial_value = p_security_system_init->ble_security_system_door_lock_initial_value; 

    uint8_t door_lock_encoded_value[1];
    ble_add_char_params_t add_door_lock_params;
    memset(&add_door_lock_params, 0, sizeof(add_door_lock_params));
    
    add_door_lock_params.uuid                = 0x9871;
    add_door_lock_params.uuid_type           = ble_uuid.type; 
    add_door_lock_params.max_len             = 8;
    add_door_lock_params.init_len            = door_lock_encode(&door_lock_initial_value, door_lock_encoded_value);
    add_door_lock_params.p_init_value        = door_lock_encoded_value; 
    add_door_lock_params.char_props.write    = 1; 
    add_door_lock_params.write_access        = SEC_OPEN; 
    // 1 for variable length and 0 for fixed length.
    add_door_lock_params.is_var_len          = 1; 

    err_code = characteristic_add(p_security_system->service_handle, &add_door_lock_params, &(p_security_system->door_lock_handles));
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    } 

    // Add door_check characteristic
    ble_security_system_door_check_t door_check_initial_value = p_security_system_init->ble_security_system_door_check_initial_value; 

    uint8_t door_check_encoded_value[1];
    ble_add_char_params_t add_door_check_params;
    memset(&add_door_check_params, 0, sizeof(add_door_check_params));
    
    add_door_check_params.uuid                = 0xAFF7;
    add_door_check_params.uuid_type           = ble_uuid.type; 
    add_door_check_params.max_len             = 8;
    add_door_check_params.init_len            = door_check_encode(&door_check_initial_value, door_check_encoded_value);
    add_door_check_params.p_init_value        = door_check_encoded_value; 
    add_door_check_params.char_props.read     = 1; 
    add_door_check_params.read_access         = SEC_OPEN; 
    // 1 for variable length and 0 for fixed length.
    add_door_check_params.is_var_len          = 1; 

    err_code = characteristic_add(p_security_system->service_handle, &add_door_check_params, &(p_security_system->door_check_handles));
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    } 

    return NRF_SUCCESS;
}

/**@brief Function for setting the move_sensors. */
uint32_t ble_security_system_move_sensors_set(ble_security_system_t * p_security_system, ble_security_system_move_sensors_t * p_move_sensors)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[1];

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = move_sensors_encode(p_move_sensors, encoded_value);
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;

    return sd_ble_gatts_value_set(p_security_system->conn_handle, p_security_system->move_sensors_handles.value_handle, &gatts_value);
}

/**@brief Function for setting the door_check. */
uint32_t ble_security_system_door_check_set(ble_security_system_t * p_security_system, ble_security_system_door_check_t * p_door_check)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[1];

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = door_check_encode(p_door_check, encoded_value);
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;

    return sd_ble_gatts_value_set(p_security_system->conn_handle, p_security_system->door_check_handles.value_handle, &gatts_value);
}

