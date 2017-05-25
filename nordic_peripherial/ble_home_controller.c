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
#include "ble_home_controller.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_util.h"
#include "app_util_bds.h"
#include "boards.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define OPCODE_LENGTH 1 /**< Length of opcode inside Home_controller packet. */
#define HANDLE_LENGTH 2 /**< Length of handle inside Home_controller packet. */

/* TODO Consider changing the max values if encoded data for characteristic/descriptor is fixed length */ 
#define MAX_TEMPERATURE_LEN (BLE_L2CAP_MTU_DEF - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Temperature. */ 
#define MAX_LIGHT_CONTROLLER_LEN (BLE_L2CAP_MTU_DEF - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Light_controller. */ 
#define MAX_LIGHT_INTENSITY_LEN (BLE_L2CAP_MTU_DEF - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Light_intensity. */ 
#define MAX_HUMIDITY_LEN (BLE_L2CAP_MTU_DEF - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Humidity. */ 

/**@brief Function for encoding Temperature.
 *
 * @param[in]   p_temperature              Temperature characteristic structure to be encoded.
 * @param[out]  p_encoded_buffer   Buffer where the encoded data will be written.
 *
 * @return      Size of encoded data.
 */
static uint8_t temperature_encode(ble_home_controller_temperature_t * p_temperature, uint8_t * encoded_buffer)
{
    uint8_t len = 0; 
    len += bds_int16_encode(&p_temperature->temperature_value, &encoded_buffer[len]); 
    return len;
}

/**@brief Function for encoding Light_controller.
 *
 * @param[in]   p_light_controller              Light_controller characteristic structure to be encoded.
 * @param[out]  p_encoded_buffer   Buffer where the encoded data will be written.
 *
 * @return      Size of encoded data.
 */
static uint8_t light_controller_encode(ble_home_controller_light_controller_t * p_light_controller, uint8_t * encoded_buffer)
{
    uint8_t len = 0; 
    encoded_buffer[len++] = p_light_controller->light;
    return len;
}

/**@brief Function for decoding Light_controller.
 *
 * @param[in]   data_len              Length of the field to be decoded.
 * @param[in]   p_data                Buffer where the encoded data is stored.
 * @param[out]  p_write_val           Decoded data.
 *
 * @return      Length of the decoded field.
 */
static uint8_t light_controller_decode(uint8_t data_len, uint8_t * p_data, ble_home_controller_light_controller_t * p_write_val)
{
    uint8_t pos = 0;
    p_write_val->light = p_data[pos++]; 

    return pos;
} 
/**@brief Function for encoding Light_intensity.
 *
 * @param[in]   p_light_intensity              Light_intensity characteristic structure to be encoded.
 * @param[out]  p_encoded_buffer   Buffer where the encoded data will be written.
 *
 * @return      Size of encoded data.
 */
static uint8_t light_intensity_encode(ble_home_controller_light_intensity_t * p_light_intensity, uint8_t * encoded_buffer)
{
    uint8_t len = 0; 
    encoded_buffer[len++] = p_light_intensity->light_intensiity_value;
    return len;
}

/**@brief Function for encoding Humidity.
 *
 * @param[in]   p_humidity              Humidity characteristic structure to be encoded.
 * @param[out]  p_encoded_buffer   Buffer where the encoded data will be written.
 *
 * @return      Size of encoded data.
 */
static uint8_t humidity_encode(ble_home_controller_humidity_t * p_humidity, uint8_t * encoded_buffer)
{
    uint8_t len = 0; 
    encoded_buffer[len++] = p_humidity->humidity_value;
    return len;
}


/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_home_controller       Home_controller Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_home_controller_t * p_home_controller, ble_evt_t * p_ble_evt)
{
    p_home_controller->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_home_controller       Home_controller Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_home_controller_t * p_home_controller, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_home_controller->conn_handle = BLE_CONN_HANDLE_INVALID;
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_home_controller       Home_controller Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_home_controller_t * p_home_controller, ble_gatts_evt_write_t * p_ble_evt)
{
    
    if(p_ble_evt->handle == p_home_controller->light_controller_handles.value_handle)
    {
        if(p_home_controller->evt_handler != NULL)
        {
            ble_home_controller_evt_t evt;
            evt.evt_type = BLE_HOME_CONTROLLER_LIGHT_CONTROLLER_EVT_WRITE;
            light_controller_decode(p_ble_evt->len, p_ble_evt->data, &evt.params.light_controller);
            p_home_controller->evt_handler(p_home_controller, &evt);
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
static void on_rw_authorize_request(ble_home_controller_t * p_home_controller, ble_gatts_evt_t * p_gatts_evt)
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
        
            if (p_auth_req->request.write.handle == p_home_controller->light_controller_handles.value_handle)
            {
                on_write(p_home_controller, &p_auth_req->request.write);
            }
        }
    }
}

/**@brief Function for handling BLE events.
 *
 * @param[in]   p_home_controller       Home_controller Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
void ble_home_controller_on_ble_evt(ble_home_controller_t * p_home_controller, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_home_controller, p_ble_evt);
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_home_controller, p_ble_evt);
            break;
        case BLE_GATTS_EVT_WRITE:
            on_write(p_home_controller, &p_ble_evt->evt.gatts_evt.params.write);
            break;
         case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            on_rw_authorize_request(p_home_controller, &p_ble_evt->evt.gatts_evt);
            break;
        default:
            //No implementation needed.
            break;
    }
}

/**@brief Function for initializing the Home_controller. */
uint32_t ble_home_controller_init(ble_home_controller_t * p_home_controller, const ble_home_controller_init_t * p_home_controller_init)
{
    uint32_t err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_home_controller->evt_handler = p_home_controller_init->evt_handler;
    p_home_controller->conn_handle = BLE_CONN_HANDLE_INVALID;
    
    // Add a custom base UUID.
    ble_uuid128_t bds_base_uuid = {{0x70, 0x6F, 0x8F, 0x08, 0xE5, 0x4E, 0x0C, 0x91, 0xE9, 0x45, 0xBB, 0xF8, 0x00, 0x00, 0x0B, 0x72}};
    uint8_t       uuid_type;
    err_code = sd_ble_uuid_vs_add(&bds_base_uuid, &uuid_type);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    ble_uuid.type = uuid_type;
    ble_uuid.uuid = 0x5CA0;
				
		BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_SCAN_PARAMETERS_SERVICE);  
		
    // Add service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_home_controller->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    } 

    // Add Temperature characteristic
    ble_home_controller_temperature_t temperature_initial_value = p_home_controller_init->ble_home_controller_temperature_initial_value; 

    uint8_t temperature_encoded_value[1];
    ble_add_char_params_t add_temperature_params;
    memset(&add_temperature_params, 0, sizeof(add_temperature_params));
		
    BLE_UUID_BLE_ASSIGN(ble_uuid, 0x2A6E);
		
    add_temperature_params.uuid                = ble_uuid.uuid;
    add_temperature_params.uuid_type           = ble_uuid.type; 
    add_temperature_params.max_len             = 8;
    add_temperature_params.init_len            = temperature_encode(&temperature_initial_value, temperature_encoded_value);
    add_temperature_params.p_init_value        = temperature_encoded_value; 
    add_temperature_params.char_props.read     = 1; 
    add_temperature_params.read_access         = SEC_OPEN; 
    // 1 for variable length and 0 for fixed length.
    add_temperature_params.is_var_len          = 1; 

    err_code = characteristic_add(p_home_controller->service_handle, &add_temperature_params, &(p_home_controller->temperature_handles));
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    } 

    // Add Light_controller characteristic
    ble_home_controller_light_controller_t light_controller_initial_value = p_home_controller_init->ble_home_controller_light_controller_initial_value; 

    uint8_t light_controller_encoded_value[1];
    ble_add_char_params_t add_light_controller_params;
    memset(&add_light_controller_params, 0, sizeof(add_light_controller_params));
    
		BLE_UUID_BLE_ASSIGN(ble_uuid, 0x2A56);
		
    add_light_controller_params.uuid                = ble_uuid.uuid;
    add_light_controller_params.uuid_type           = ble_uuid.type; 
    add_light_controller_params.max_len             = 8;
    add_light_controller_params.init_len            = light_controller_encode(&light_controller_initial_value, light_controller_encoded_value);
    add_light_controller_params.p_init_value        = light_controller_encoded_value; 
    add_light_controller_params.char_props.read     = 1; 
    add_light_controller_params.read_access         = SEC_OPEN; 
    add_light_controller_params.char_props.write    = 1; 
    add_light_controller_params.write_access        = SEC_OPEN; 
    // 1 for variable length and 0 for fixed length.
    add_light_controller_params.is_var_len          = 1; 

    err_code = characteristic_add(p_home_controller->service_handle, &add_light_controller_params, &(p_home_controller->light_controller_handles));
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    } 

    // Add Light_intensity characteristic
    ble_home_controller_light_intensity_t light_intensity_initial_value = p_home_controller_init->ble_home_controller_light_intensity_initial_value; 

    uint8_t light_intensity_encoded_value[1];
    ble_add_char_params_t add_light_intensity_params;
    memset(&add_light_intensity_params, 0, sizeof(add_light_intensity_params));
    
    add_light_intensity_params.uuid                = 0x835D;
    add_light_intensity_params.uuid_type           = ble_uuid.type; 
    add_light_intensity_params.max_len             = 8;
    add_light_intensity_params.init_len            = light_intensity_encode(&light_intensity_initial_value, light_intensity_encoded_value);
    add_light_intensity_params.p_init_value        = light_intensity_encoded_value; 
    add_light_intensity_params.char_props.read     = 1; 
    add_light_intensity_params.read_access         = SEC_OPEN; 
    // 1 for variable length and 0 for fixed length.
    add_light_intensity_params.is_var_len          = 1; 

    err_code = characteristic_add(p_home_controller->service_handle, &add_light_intensity_params, &(p_home_controller->light_intensity_handles));
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    } 

    // Add Humidity characteristic
    ble_home_controller_humidity_t humidity_initial_value = p_home_controller_init->ble_home_controller_humidity_initial_value; 

    uint8_t humidity_encoded_value[1];
    ble_add_char_params_t add_humidity_params;
    memset(&add_humidity_params, 0, sizeof(add_humidity_params));
    
    add_humidity_params.uuid                = 0x1D62;
    add_humidity_params.uuid_type           = ble_uuid.type; 
    add_humidity_params.max_len             = 8;
    add_humidity_params.init_len            = humidity_encode(&humidity_initial_value, humidity_encoded_value);
    add_humidity_params.p_init_value        = humidity_encoded_value; 
    add_humidity_params.char_props.read     = 1; 
    add_humidity_params.read_access         = SEC_OPEN; 
    // 1 for variable length and 0 for fixed length.
    add_humidity_params.is_var_len          = 1; 

    err_code = characteristic_add(p_home_controller->service_handle, &add_humidity_params, &(p_home_controller->humidity_handles));
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    } 

    return NRF_SUCCESS;
}

/**@brief Function for setting the Temperature. */
uint32_t ble_home_controller_temperature_set(ble_home_controller_t * p_home_controller, ble_home_controller_temperature_t * p_temperature)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[1];

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = temperature_encode(p_temperature, encoded_value);
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;

    return sd_ble_gatts_value_set(p_home_controller->conn_handle, p_home_controller->temperature_handles.value_handle, &gatts_value);
}

/**@brief Function for setting the Light_controller. */
uint32_t ble_home_controller_light_controller_set(ble_home_controller_t * p_home_controller, ble_home_controller_light_controller_t * p_light_controller)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[1];

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = light_controller_encode(p_light_controller, encoded_value);
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;

    return sd_ble_gatts_value_set(p_home_controller->conn_handle, p_home_controller->light_controller_handles.value_handle, &gatts_value);
}

/**@brief Function for setting the Light_intensity. */
uint32_t ble_home_controller_light_intensity_set(ble_home_controller_t * p_home_controller, ble_home_controller_light_intensity_t * p_light_intensity)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[1];

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = light_intensity_encode(p_light_intensity, encoded_value);
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;

    return sd_ble_gatts_value_set(p_home_controller->conn_handle, p_home_controller->light_intensity_handles.value_handle, &gatts_value);
}

/**@brief Function for setting the Humidity. */
uint32_t ble_home_controller_humidity_set(ble_home_controller_t * p_home_controller, ble_home_controller_humidity_t * p_humidity)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[1];

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = humidity_encode(p_humidity, encoded_value);
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;

    return sd_ble_gatts_value_set(p_home_controller->conn_handle, p_home_controller->humidity_handles.value_handle, &gatts_value);
}

