
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


#include "boards.h"
#include "nrf_delay.h"
#include "service_if.h"
#include <stdint.h>
#include "bsp.h"
//#include "app_trace.h" 
#include "ble_home_controller.h" 
#include "ble_security_system.h" 
#include "nrf_temp.h"
#include "nrf_soc.h"
#include "nrf_log.h"
#include "app_error.h"
#include "app_pwm.h"
#include "nrf_saadc.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_gpiote.h"
#include "nrf_gpio.h"

#define ADC_REF_VOLTAGE_IN_MILLIVOLTS   600                                     /**< Reference voltage (in milli volts) used by ADC while doing conversion. */
#define ADC_PRE_SCALING_COMPENSATION    6                                       /**< The ADC is configured to use VDD with 1/3 prescaling as input. And hence the result of conversion is to be multiplied by 3 to get the actual value of the battery voltage.*/
#define DIODE_FWD_VOLT_DROP_MILLIVOLTS  270                                     /**< Typical forward voltage drop of the diode . */
#define ADC_RES_10BIT                   1024  
APP_PWM_INSTANCE(PWM1,2);   


static ble_home_controller_t    m_home_controller; 
static ble_security_system_t    m_security_system; 

static uint8_t light_memory;
static uint8_t timer_memory;
static bool door_controler_LOG;

static nrf_saadc_value_t adc_buf[2];
static volatile bool ready_flag;  
void pwm_ready_callback(uint32_t pwm_id)    // PWM callback function
{ready_flag = true;}


void light_set(uint8_t light)
{
		if(light>100)light=100;
		if(light<1)light=1;
		app_pwm_channel_duty_set(&PWM1, 1, (app_pwm_duty_t)light);//value1-100;
	  app_pwm_channel_duty_set(&PWM1, 0, (app_pwm_duty_t)light);	
		
}	
void timer_handler_button(void * p_context)
{	
		UNUSED_PARAMETER(p_context);
		if(bsp_board_button_state_get((uint32_t)2))
		{		
				if (timer_memory==0) light_memory=light_memory-10;
				timer_memory=1;
		}
		if(bsp_board_button_state_get((uint32_t)3))
		{		
				if (timer_memory==0) light_memory=light_memory+10;
				timer_memory=1;
		}
		if (light_memory>100) light_memory=100;
		if (light_memory<10) light_memory=10;
}

static void humidity_meassure(void)
{
		ble_home_controller_humidity_t  p_humidity;
	
		uint8_t dane=0;
		char maska=1;
		nrf_gpio_pin_clear((uint32_t)18);
		nrf_delay_ms(25);
		nrf_gpio_pin_set((uint32_t)18);
		nrf_delay_us(30);
		nrf_gpio_pin_clear((uint32_t)18);
		nrf_gpio_cfg_input((uint32_t)18,NRF_GPIO_PIN_NOPULL);

		nrf_delay_us(30);
		while(!nrf_gpio_pin_read((uint32_t)18)){
		}
		while(nrf_gpio_pin_read((uint32_t)18)){
		}

    for(int i=0;i<8;i++)
    {
      while(!nrf_gpio_pin_read((uint32_t)18)){
      }
      nrf_delay_us(35);
      if(nrf_gpio_pin_read((uint32_t)18))
      {
        dane=dane|(maska<<(7-i));
        while(nrf_gpio_pin_read((uint32_t)18)){
        }
      }
    }
		nrf_gpio_cfg_output((uint32_t)18);
		nrf_gpio_pin_set((uint32_t)18);
		NRF_LOG_INFO("Poziom wilgotnosci %d%%\r\n", dane);	
		p_humidity.humidity_value=dane;
		ble_home_controller_humidity_set(&m_home_controller,&p_humidity);	
}

static void temperature_meassure(void)
{
	 	int32_t temperature;
		ble_home_controller_temperature_t p_temperature_controller;
	  sd_temp_get(&temperature);
		nrf_delay_ms(60);
		temperature=temperature/4;
		p_temperature_controller.temperature_value=	(int16_t)temperature;		
		ble_home_controller_temperature_set(&m_home_controller,&p_temperature_controller);
		NRF_LOG_INFO("Temperatura %d C\r\n", temperature);	
}

static void light_send(void)
{
		ble_home_controller_light_controller_t p_light_controller;
		p_light_controller.light=light_memory;
		light_set(light_memory);
		timer_memory=0;
		ble_home_controller_light_controller_set(&m_home_controller,&p_light_controller);
		NRF_LOG_INFO("Lampa LED swieci z moca: %d%%\r\n", light_memory);	
}

static void security_parameter_send(void)
{
		ble_security_system_move_sensors_t  p_move_sensors;
		ble_security_system_door_check_t p_door_check;
	
		p_door_check.door_control=(uint8_t)nrf_gpio_pin_read((uint32_t)6);
		p_move_sensors.sensor_1=(uint8_t)nrf_gpio_pin_read((uint32_t)8);
	
		ble_security_system_move_sensors_set(&m_security_system,&p_move_sensors);
		ble_security_system_door_check_set(&m_security_system,&p_door_check);
	
		if(p_door_check.door_control==0)
		NRF_LOG_INFO("Dzwi otwarte, ");	
		if(p_door_check.door_control!=0)
		NRF_LOG_INFO("Dzwi zamkniete, ");	
		if(door_controler_LOG!=0)
		NRF_LOG_INFO("Elektryczny zamek wlaczony \r\n");	
		if(door_controler_LOG==0)
		NRF_LOG_INFO("Elektryczny zamek wylaczony \r\n");	
		if(p_move_sensors.sensor_1==0)
		NRF_LOG_INFO("Nie wykryto ruchu, \r\n");	
		if(p_move_sensors.sensor_1!=0)
		NRF_LOG_INFO("Wykryto ruch w zasiegu dzialania czujnika, \r\n");	

}	

void timer_handler(void * p_context)
{	
    UNUSED_PARAMETER(p_context);
	
		light_send();
		security_parameter_send();
		temperature_meassure();
		nrf_drv_saadc_sample();
		humidity_meassure();
}

/**@brief Function for handling the Home_controller events.
 *
 * @details This function will be called for all Home_controller events which are passed to
 *          the application.
 *
 * @param[in]   p_home_controller   Home_controller structure.
 * @param[in]   p_evt   Event received from the Home_controller.
 */
static void on_home_controller_evt(ble_home_controller_t * p_home_controller, ble_home_controller_evt_t * p_evt)
{
    switch (p_evt->evt_type)
    { 
        case BLE_HOME_CONTROLLER_LIGHT_CONTROLLER_EVT_WRITE:
						light_memory=p_evt->params.light_controller.light;
						light_set(p_evt->params.light_controller.light);
            //app_trace_log("[Bluetooth_IF]: HOME_CONTROLLER_LIGHT_CONTROLLER evt WRITE. \r\n");
            break; 
        default:
            // No implementation needed.
            break;
    }
}

static void on_security_system_evt(ble_security_system_t * p_security_system, ble_security_system_evt_t * p_evt)
{
    switch (p_evt->evt_type)
    { 
        case BLE_SECURITY_SYSTEM_DOOR_LOCK_EVT_WRITE:
						if(p_evt->params.door_lock.door_closed==0)
						{
							nrf_gpio_pin_clear((uint32_t)7);
							door_controler_LOG=0;
						}
						else
						{
							nrf_gpio_pin_set((uint32_t)7);
							door_controler_LOG=1;
						}				
            //app_trace_log("[Bluetooth_IF]: SECURITY_SYSTEM_DOOR_LOCK evt WRITE. \r\n");
            break; 
        default:
            // No implementation needed.
            break;
    }
}

static void pwm_init(void)
{
	  //uint32_t    err_code; 
		app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_2CH(5000L, 12, 11);
    pwm1_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
		pwm1_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
		app_pwm_init(&PWM1,&pwm1_cfg,pwm_ready_callback);
		app_pwm_enable(&PWM1);
		app_pwm_channel_duty_set(&PWM1, 1, 50);//value1-100;
	  app_pwm_channel_duty_set(&PWM1, 0, 50);	
}

void saadc_event_handler(nrf_drv_saadc_evt_t const * p_event)
{
		ble_home_controller_light_intensity_t light_intensity;
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        nrf_saadc_value_t adc_result;
        double          light_level;
        uint32_t          err_code;

        adc_result = p_event->data.done.p_buffer[0];
        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, 1);
        APP_ERROR_CHECK(err_code);

        light_level =((double)adc_result*20/1024)*6;
        NRF_LOG_INFO("Natezenie oswietlenia: %d%%\r\n\r\n", light_level);
				light_intensity.light_intensiity_value =(uint8_t) light_level;
        ble_home_controller_light_intensity_set(&m_home_controller,&light_intensity); 
    }
}

static void adc_configure(void)
{
    ret_code_t err_code = nrf_drv_saadc_init(NULL, saadc_event_handler);
    APP_ERROR_CHECK(err_code);

    nrf_saadc_channel_config_t config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);
    err_code = nrf_drv_saadc_channel_init(0, &config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[0], 1);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[1], 1);
    APP_ERROR_CHECK(err_code);
}
static void security_init()
{
		nrf_gpio_cfg_input((uint32_t)8,NRF_GPIO_PIN_NOPULL);//czujnik ruchu
		nrf_gpio_cfg_input((uint32_t)6,NRF_GPIO_PIN_NOPULL);//przycisk drzwi
		nrf_gpio_cfg_output((uint32_t)7);//zamykanie drzwi-dioda
}

static void humidity_init()
{
		nrf_gpio_cfg_output((uint32_t)18);
		nrf_gpio_pin_set((uint32_t)18);
}

/**@brief Function for initializing the Services generated by Bluetooth Developer Studio.
 *
 *
 * @return      NRF_SUCCESS on successful initialization of services, otherwise an error code.
 */
uint32_t bluetooth_init(void)
{
    uint32_t    err_code; 
	
		pwm_init();
		adc_configure();
		humidity_init();
		security_init();
	
	
    ble_home_controller_init_t    home_controller_init; 
    ble_security_system_init_t    security_system_init; 
    

    // Initialize Home_controller.
    memset(&home_controller_init, 0, sizeof(home_controller_init));

    home_controller_init.evt_handler = on_home_controller_evt; 
    memset(&home_controller_init.ble_home_controller_temperature_initial_value.temperature_value,
           0x00,
           sizeof(home_controller_init.ble_home_controller_temperature_initial_value.temperature_value));
    memset(&home_controller_init.ble_home_controller_light_controller_initial_value.light,
           0x00,
           sizeof(home_controller_init.ble_home_controller_light_controller_initial_value.light));
    memset(&home_controller_init.ble_home_controller_light_intensity_initial_value.light_intensiity_value,
           0x00,
           sizeof(home_controller_init.ble_home_controller_light_intensity_initial_value.light_intensiity_value));
    memset(&home_controller_init.ble_home_controller_humidity_initial_value.humidity_value,
           0x00,
           sizeof(home_controller_init.ble_home_controller_humidity_initial_value.humidity_value));

    err_code = ble_home_controller_init(&m_home_controller, &home_controller_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    } 

    // Initialize security_system.
    memset(&security_system_init, 0, sizeof(security_system_init));

    security_system_init.evt_handler = on_security_system_evt; 
    memset(&security_system_init.ble_security_system_move_sensors_initial_value.sensor_1,
           0x00,
           sizeof(security_system_init.ble_security_system_move_sensors_initial_value.sensor_1));
    memset(&security_system_init.ble_security_system_door_lock_initial_value.door_closed,
           0x00,
           sizeof(security_system_init.ble_security_system_door_lock_initial_value.door_closed));
    memset(&security_system_init.ble_security_system_door_check_initial_value.door_control,
           0x00,
           sizeof(security_system_init.ble_security_system_door_check_initial_value.door_control));

    err_code = ble_security_system_init(&m_security_system, &security_system_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    } 

    return NRF_SUCCESS;
}

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to all Bluetooth Developer Studio generated Services.
 *
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void bluetooth_on_ble_evt(ble_evt_t * p_ble_evt)
{ 
    ble_home_controller_on_ble_evt(&m_home_controller, p_ble_evt); 
    ble_security_system_on_ble_evt(&m_security_system, p_ble_evt); 
}

