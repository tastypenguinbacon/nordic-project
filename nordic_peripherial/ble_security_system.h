/* This file was generated by plugin 'Nordic Semiconductor nRF5x v.1.2.4' (BDS version 1.1.3135.0) */

#ifndef BLE_SECURITY_SYSTEM_H__
#define BLE_SECURITY_SYSTEM_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "app_util_bds.h"



/**@brief security_system event type. */
typedef enum
{ 
    BLE_SECURITY_SYSTEM_MOVE_SENSORS_EVT_NOTIFICATION_ENABLED,  /**< move_sensors value notification enabled event. */
    BLE_SECURITY_SYSTEM_MOVE_SENSORS_EVT_NOTIFICATION_DISABLED, /**< move_sensors value notification disabled event. */
    BLE_SECURITY_SYSTEM_DOOR_LOCK_EVT_NOTIFICATION_ENABLED,  /**< door_lock value notification enabled event. */
    BLE_SECURITY_SYSTEM_DOOR_LOCK_EVT_NOTIFICATION_DISABLED, /**< door_lock value notification disabled event. */
    BLE_SECURITY_SYSTEM_DOOR_LOCK_EVT_WRITE, /**< door_lock write event. */
    BLE_SECURITY_SYSTEM_DOOR_CHECK_EVT_NOTIFICATION_ENABLED,  /**< door_check value notification enabled event. */
    BLE_SECURITY_SYSTEM_DOOR_CHECK_EVT_NOTIFICATION_DISABLED, /**< door_check value notification disabled event. */
} ble_security_system_evt_type_t;

// Forward declaration of the ble_security_system_t type.
typedef struct ble_security_system_s ble_security_system_t;








/**@brief move_sensors structure. */
typedef struct
{
    uint8_t sensor_1;
} ble_security_system_move_sensors_t;
/**@brief door_lock structure. */
typedef struct
{
    uint8_t door_closed;
} ble_security_system_door_lock_t;
/**@brief door_check structure. */
typedef struct
{
    uint8_t door_control;
} ble_security_system_door_check_t;

/**@brief security_system Service event. */
typedef struct
{
    ble_security_system_evt_type_t evt_type;    /**< Type of event. */
    union {
        uint16_t cccd_value; /**< Holds decoded data in Notify and Indicate event handler. */
        ble_security_system_door_lock_t door_lock; /**< Holds decoded data in Write event handler. */
    } params;
} ble_security_system_evt_t;

/**@brief security_system Service event handler type. */
typedef void (*ble_security_system_evt_handler_t) (ble_security_system_t * p_security_system, ble_security_system_evt_t * p_evt);

/**@brief security_system Service init structure. This contains all options and data needed for initialization of the service */
typedef struct
{
    ble_security_system_evt_handler_t     evt_handler; /**< Event handler to be called for handling events in the security_system Service. */
    ble_security_system_move_sensors_t ble_security_system_move_sensors_initial_value; /**< If not NULL, initial value of the move_sensors characteristic. */ 
    ble_security_system_door_lock_t ble_security_system_door_lock_initial_value; /**< If not NULL, initial value of the door_lock characteristic. */ 
    ble_security_system_door_check_t ble_security_system_door_check_initial_value; /**< If not NULL, initial value of the door_check characteristic. */ 
} ble_security_system_init_t;

/**@brief security_system Service structure. This contains various status information for the service.*/
struct ble_security_system_s
{
    ble_security_system_evt_handler_t evt_handler; /**< Event handler to be called for handling events in the security_system Service. */
    uint16_t service_handle; /**< Handle of security_system Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t move_sensors_handles; /**< Handles related to the move_sensors characteristic. */
    ble_gatts_char_handles_t door_lock_handles; /**< Handles related to the door_lock characteristic. */
    ble_gatts_char_handles_t door_check_handles; /**< Handles related to the door_check characteristic. */
    uint16_t conn_handle; /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
};

/**@brief Function for initializing the security_system.
 *
 * @param[out]  p_security_system       security_system Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_security_system_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_security_system_init(ble_security_system_t * p_security_system, const ble_security_system_init_t * p_security_system_init);

/**@brief Function for handling the Application's BLE Stack events.*/
void ble_security_system_on_ble_evt(ble_security_system_t * p_security_system, ble_evt_t * p_ble_evt);

/**@brief Function for setting the move_sensors.
 *
 * @details Sets a new value of the move_sensors characteristic. The new value will be sent
 *          to the client the next time the client reads the move_sensors characteristic.
 *          This function is only generated if the characteristic's Read property is not 'Excluded'.
 *
 * @param[in]   p_security_system                 security_system Service structure.
 * @param[in]   p_move_sensors  New move_sensors.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_security_system_move_sensors_set(ble_security_system_t * p_security_system, ble_security_system_move_sensors_t * p_move_sensors);

/**@brief Function for setting the door_check.
 *
 * @details Sets a new value of the door_check characteristic. The new value will be sent
 *          to the client the next time the client reads the door_check characteristic.
 *          This function is only generated if the characteristic's Read property is not 'Excluded'.
 *
 * @param[in]   p_security_system                 security_system Service structure.
 * @param[in]   p_door_check  New door_check.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_security_system_door_check_set(ble_security_system_t * p_security_system, ble_security_system_door_check_t * p_door_check);

#endif //_BLE_SECURITY_SYSTEM_H__
