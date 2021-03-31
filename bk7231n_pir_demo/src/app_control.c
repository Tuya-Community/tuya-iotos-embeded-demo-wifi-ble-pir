/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: sht21.h
 * @Description: SHT21 IIC drive src file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-12-16 18:51:29
 * @LastEditTime: 2021-03-25 11:46:13
 */

#include "app_control.h"
#include "tuya_gpio.h"
#include "tuya_uart.h"
#include "BkDriverUart.h"
#include "sys_timer.h"
#include "uni_time.h"
/***********************************************************
*************************types define***********************
***********************************************************/
typedef enum
{
    LOW = 0,
    HIGH,
}default_level;

APP_CTRL_DATA_T app_ctrl_data = {0};

/***********************************************************
*************************IO control device define***********
***********************************************************/
#define SWITCH_IO                           (6)
#define SEN_3                               (20)
#define SEN_2                               (22)
#define SEN_1                               (14)
#define OT_3                                (17)
#define OT_2                                (8)
#define OT_1                                (5)
/***********************************************************
*************************about adc init*********************
***********************************************************/

/***********************************************************
*************************about iic init*********************
***********************************************************/

/***********************************************************
*************************function***************************
***********************************************************/

STATIC VOID __ctrl_gpio_init(CONST TY_GPIO_PORT_E port, CONST BOOL_T high)
{   
    //Set IO port as output mode
    tuya_gpio_inout_set(port, FALSE);
    //Set IO port level
    tuya_gpio_write(port, high);
}


VOID app_device_init(VOID)
{
    tuya_gpio_inout_set(SWITCH_IO, TRUE);
    __ctrl_gpio_init(SEN_3,HIGH);
    __ctrl_gpio_init(SEN_2,LOW);
    __ctrl_gpio_init(SEN_1,LOW);
    __ctrl_gpio_init(OT_3,LOW);
    __ctrl_gpio_init(OT_2,LOW);
    __ctrl_gpio_init(OT_1,HIGH);
}

VOID app_set_sensitivity(SENSITIVITY sens)
{
    switch (sens)
    {
    case sensitivity_3m:
        tuya_gpio_write(SEN_3, HIGH);
        tuya_gpio_write(SEN_2, LOW);
        tuya_gpio_write(SEN_1, LOW);
        break;
    case sensitivity_5m:
        tuya_gpio_write(SEN_3, LOW);
        tuya_gpio_write(SEN_2, HIGH);
        tuya_gpio_write(SEN_1, LOW);
        break;
    case sensitivity_7m:
        tuya_gpio_write(SEN_3, LOW);
        tuya_gpio_write(SEN_2, LOW);
        tuya_gpio_write(SEN_1, HIGH);        
        break;    
    default:
        break;
    }
}

VOID app_set_ontime(ONTIME ontime)
{
    switch (ontime)
    {
    case ontime_30s:
        tuya_gpio_write(OT_3, LOW);
        tuya_gpio_write(OT_2, LOW);
        tuya_gpio_write(OT_1, HIGH);
        break;
    case ontime_60s:
        tuya_gpio_write(OT_3, LOW);
        tuya_gpio_write(OT_2, HIGH);
        tuya_gpio_write(OT_1, LOW);
        break;
    case ontime_120s:
        tuya_gpio_write(OT_3, HIGH);
        tuya_gpio_write(OT_2, LOW);
        tuya_gpio_write(OT_1, LOW);       
        break;    
    default:
        break;
    }
}

VOID app_ctrl_handle(VOID)
{   
    app_set_sensitivity(app_ctrl_data.sens);
    app_set_ontime(app_ctrl_data.ontime);
    app_ctrl_data.sensor_state = tuya_gpio_read(SWITCH_IO);
    if(app_ctrl_data.sensor_state != app_ctrl_data.sensor_state_old) {
        app_report_all_dp_status();
    }
    app_ctrl_data.sensor_state_old = app_ctrl_data.sensor_state;
}

VOID app_ctrl_all_off(VOID)
{   

}

 