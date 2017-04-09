/* This file was generated by plugin 'Nordic Semiconductor nRF5x v.1.2.4' (BDS version 1.1.3135.0) */

#include "ble_home_controller.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_util.h"
#include "app_util_bds.h"
#include "nrf_temp.h"
#include "nrf_gpio.h"

#define OPCODE_LENGTH 1 /**< Length of opcode inside Home_controller packet. */
#define HANDLE_LENGTH 2 /**< Length of handle inside Home_controller packet. */

/* TODO Consider changing the max values if encoded data for characteristic/descriptor is fixed length */ 
#define MAX_TEMPERATURE_LEN (BLE_L2CAP_MTU_DEF - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Temperature. */ 
#define MAX_LIGHT_CONTROLLER_LEN (BLE_L2CAP_MTU_DEF - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Light_controller. */ 

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
    len += bds_uint32_encode(&p_temperature->temperature_value, &encoded_buffer[len]); 
    return len;
}

/**@brief Function for decoding Temperature.
 *
 * @param[in]   data_len              Length of the field to be decoded.
 * @param[in]   p_data                Buffer where the encoded data is stored.
 * @param[out]  p_write_val           Decoded data.
 *
 * @return      Length of the decoded field.
 */
static uint8_t temperature_decode(uint8_t data_len, uint8_t * p_data, ble_home_controller_temperature_t * p_write_val)
{
    uint8_t pos = 0;
    pos += bds_uint32_decode((data_len-pos), &p_data[pos], &p_write_val->temperature_value); 

    return pos;
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
static uint8_t temperature_controller_encode(ble_home_controller_temperature_t * p_temperature_controller, uint8_t * encoded_buffer)
{
    uint8_t len = 0; 
    encoded_buffer[len++] = (uint8_t)p_temperature_controller->temperature_value;
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
    
    if(p_ble_evt->handle == p_home_controller->temperature_handles.value_handle)
    {
        if(p_home_controller->evt_handler != NULL)
        {
						//wlasny kod /wlasny kod/wlasny kod/wlasny kod/wlasny kod/wlasny kod/wlasny kod/wlasny kod
					  //p_ble_evt->data[0] =(uint32_t)temp;	
					
            ble_home_controller_evt_t evt;
            evt.evt_type = BLE_HOME_CONTROLLER_TEMPERATURE_EVT_WRITE;
            temperature_decode(p_ble_evt->len, p_ble_evt->data, &evt.params.temperature);
					
            p_home_controller->evt_handler(p_home_controller, &evt);
        }
    }
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
        
            if (p_auth_req->request.write.handle == p_home_controller->temperature_handles.value_handle)
            {
                on_write(p_home_controller, &p_auth_req->request.write);
            }
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
    
    add_temperature_params.uuid                = 0xDD5F;
    add_temperature_params.uuid_type           = ble_uuid.type; 
    add_temperature_params.max_len             = 1;
    add_temperature_params.init_len            = temperature_encode(&temperature_initial_value, temperature_encoded_value);
    add_temperature_params.p_init_value        = temperature_encoded_value; 
    add_temperature_params.char_props.write    = 1; 
    add_temperature_params.write_access        = SEC_OPEN; 
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
    
    add_light_controller_params.uuid                = 0xB9F3;
    add_light_controller_params.uuid_type           = ble_uuid.type; 
    add_light_controller_params.max_len             = 1;
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

    return NRF_SUCCESS;
}

/**@brief Function for setting the Light_controller. */
uint32_t ble_home_controller_light_controller_set(ble_home_controller_t * p_home_controller, ble_home_controller_light_controller_t * p_light_controller)
{
    ble_gatts_value_t gatts_value;
    uint8_t encoded_value[1];
			
		//wlasny kod /wlasny kod/wlasny kod/wlasny kod/wlasny kod/wlasny kod/wlasny kod/wlasny kod
					
					
    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = light_controller_encode(p_light_controller, encoded_value);
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;
	
		sd_ble_gatts_value_set(p_home_controller->conn_handle, p_home_controller->light_controller_handles.value_handle, &gatts_value);
		
		//???????????????????????????????????????????
            ble_gatts_hvx_params_t hvx_params;

            memset(&hvx_params, 0, sizeof(hvx_params));

            hvx_params.handle = p_home_controller->light_controller_handles.value_handle;
            hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
            hvx_params.offset = gatts_value.offset;
            hvx_params.p_len  = &gatts_value.len;
            hvx_params.p_data = gatts_value.p_value;

            sd_ble_gatts_hvx(p_home_controller->conn_handle, &hvx_params);
     //??????????????????????????????????????????? 
	
		return 1;
}
uint32_t ble_home_controller_temperature_controller_set(ble_home_controller_t * p_home_controller, ble_home_controller_temperature_t * p_temperature_controller)
{
		ble_gatts_value_t gatts_value;
    uint8_t encoded_value[1];
			
		//wlasny kod /wlasny kod/wlasny kod/wlasny kod/wlasny kod/wlasny kod/wlasny kod/wlasny kod
					
					
    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = temperature_controller_encode(p_temperature_controller, encoded_value);
    gatts_value.offset  = 0;
    gatts_value.p_value = encoded_value;
		
	
		
		
    sd_ble_gatts_value_set(p_home_controller->conn_handle, p_home_controller->temperature_handles.value_handle, &gatts_value);
		
		//???????????????????????????????????????????
            ble_gatts_hvx_params_t hvx_params;

            memset(&hvx_params, 0, sizeof(hvx_params));

            hvx_params.handle = p_home_controller->temperature_handles.value_handle;
            hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
            hvx_params.offset = gatts_value.offset;
            hvx_params.p_len  = &gatts_value.len;
            hvx_params.p_data = gatts_value.p_value;

            sd_ble_gatts_hvx(p_home_controller->conn_handle, &hvx_params);
     //??????????????????????????????????????????? 
	
		return 1;
}
