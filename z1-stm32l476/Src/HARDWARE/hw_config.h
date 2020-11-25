#ifndef _CONFIG_GPIO_H
#define _CONFIG_GPIO_H
/*
#include "hw_gpio.h"
#include "hw_key.h"
#include "hw_led.h"
#include "hw_lcd.h"
#include "hw_adc.h"
#include "hw_audio.h"
#include "hw_exti.h"
#include "hw_charge.h"
#include "hw_gps.h"
#include "hw_gsensor.h"
#include "hw_heartrate.h"
#include "hw_lora.h"
#include "hw_i2c.h"
*/
#if 0
#define KEY_CALL_Pin GPIO_PIN_9
#define KEY_CALL_GPIO_Port GPIOG
#define KEY_NEXT_Pin GPIO_PIN_14
#define KEY_NEXT_GPIO_Port GPIOG
#define LED_CHG_GREEN_Pin GPIO_PIN_3
#define LED_CHG_GREEN_GPIO_Port GPIOB
//#define SENSOR_SDA_Pin GPIO_PIN_7
//#define SENSOR_SDA_GPIO_Port GPIOB
#define GPS_VDD_EN_Pin GPIO_PIN_12
#define GPS_VDD_EN_GPIO_Port GPIOC
//#define SENSOR_SCL_Pin GPIO_PIN_6
//#define SENSOR_SCL_GPIO_Port GPIOB
#define GSENSOR_INT_Pin GPIO_PIN_13
#define GSENSOR_INT_GPIO_Port GPIOC
#define LED_KEY_R1_Pin GPIO_PIN_12
#define LED_KEY_R1_GPIO_Port GPIOA
#define PRECHG_Pin GPIO_PIN_5
#define PRECHG_GPIO_Port GPIOB
#define LORA_RESET_Pin GPIO_PIN_11
#define LORA_RESET_GPIO_Port GPIOA
#define GPS_UART_RX_Pin GPIO_PIN_10
#define GPS_UART_RX_GPIO_Port GPIOA
#define LED_KEY_R2_Pin GPIO_PIN_9
#define LED_KEY_R2_GPIO_Port GPIOC
#define LCD_RESET_Pin GPIO_PIN_8
#define LCD_RESET_GPIO_Port GPIOA
#define GPS_UART_TX_Pin GPIO_PIN_9
#define GPS_UART_TX_GPIO_Port GPIOA
#define MOTOR_PWM_Pin GPIO_PIN_9
#define MOTOR_PWM_GPIO_Port GPIOB
#define LED_CHG_RED_Pin GPIO_PIN_7
#define LED_CHG_RED_GPIO_Port GPIOC
#define KEY_SOS_Pin GPIO_PIN_8
#define KEY_SOS_GPIO_Port GPIOC
#define GPS_RESET_Pin GPIO_PIN_2
#define GPS_RESET_GPIO_Port GPIOC
#define HEARTRATE_SENSOR_INT_Pin GPIO_PIN_1
#define HEARTRATE_SENSOR_INT_GPIO_Port GPIOC
#define GPS_POWER_ON_Pin GPIO_PIN_0
#define GPS_POWER_ON_GPIO_Port GPIOC
#define LORA_POWER_ON_Pin GPIO_PIN_15
#define LORA_POWER_ON_GPIO_Port GPIOB
#define LCD_I2C_SDA_Pin GPIO_PIN_14
#define LCD_I2C_SDA_GPIO_Port GPIOB
#define LORA_UART_TX_Pin GPIO_PIN_2
#define LORA_UART_TX_GPIO_Port GPIOA
#define CHARGE_Pin GPIO_PIN_12
#define CHARGE_GPIO_Port GPIOB
#define LCD_I2C_SCL_Pin GPIO_PIN_13
#define LCD_I2C_SCL_GPIO_Port GPIOB
#define LCD_SA0_Pin GPIO_PIN_7
#define LCD_SA0_GPIO_Port GPIOA
#define LORA_UART_RX_Pin GPIO_PIN_3
#define LORA_UART_RX_GPIO_Port GPIOA
#define SYSTEM_POWERON_IND_Pin GPIO_PIN_0
#define SYSTEM_POWERON_IND_GPIO_Port GPIOA
#define SYSTEM_POWERON_Pin GPIO_PIN_2
#define SYSTEM_POWERON_GPIO_Port GPIOB
#define HEARTRATE_SENSOR_RESET_Pin GPIO_PIN_1
#define HEARTRATE_SENSOR_RESET_GPIO_Port GPIOB
#define BATTERY_ADC_Pin GPIO_PIN_0
#define BATTERY_ADC_GPIO_Port GPIOB
#define SENSOR_VCC_EN_Pin GPIO_PIN_5
#define SENSOR_VCC_EN_GPIO_Port GPIOC
#define LCD_VCC_EN_Pin GPIO_PIN_4
#define LCD_VCC_EN_GPIO_Port GPIOC
#endif


/*gpio define for project Z1*/

#define SYSTEM_POWER_LOCK_Pin GPIO_PIN_2
#define SYSTEM_POWER_LOCK_GPIO_Port GPIOB

#define SYSTEM_POWERON_IND_Pin GPIO_PIN_13
#define SYSTEM_POWERON_IND_GPIO_Port GPIOC


#define RD_POWERLIGHT_SET_Pin GPIO_PIN_11
#define RD_POWERLIGHT_SET_GPIO_Port GPIOG


#define SD_POWER_IND_Pin GPIO_PIN_11
#define SD_POWER_IND_GPIO_Port GPIOA
#define DEBUG_LED_1_Pin GPIO_PIN_7//GPIO_PIN_11
#define DEBUG_LED_1_GPIO_Port GPIOC//GPIOA
#define DEBUG_LED_2_Pin GPIO_PIN_12
#define DEBUG_LED_2_GPIO_Port GPIOA

#define AOA_UART_TX_Pin GPIO_PIN_9
#define AOA_UART_TX_GPIO_Port GPIOA
#define AOA_UART_RX_Pin GPIO_PIN_10
#define AOA_UART_TX_GPIO_Port GPIOA

#define BD1_POWER_Pin GPIO_PIN_5
#define BD1_POWER_GPIO_Port GPIOB
#define BD1_ENABLE_Pin GPIO_PIN_10
#define BD1_ENABLE_GPIO_Port GPIOG
#define BD1_POWER_1_Pin GPIO_PIN_2
#define BD1_POWER_1_GPIO_Port GPIOC


#define BD1_DETECT_Pin GPIO_PIN_3
#define BD1_DETECT_GPIO_Port GPIOC


#define BD1_UART_RX_Pin GPIO_PIN_3
#define BD1_UART_RX_GPIO_Port GPIOA
#define BD1_UART_TX_Pin GPIO_PIN_2
#define BD1_UART_TX_GPIO_Port GPIOA


#define BD2_POWER_Pin GPIO_PIN_15
#define BD2_POWER_GPIO_Port GPIOB
#define BD2_RESET_Pin GPIO_PIN_12
#define BD2_RESET_GPIO_Port GPIOA


#define BD2_UART_TX_Pin GPIO_PIN_4
#define BD2_UART_TX_GPIO_Port GPIOC
#define BD2_UART_RX_Pin GPIO_PIN_5
#define BD2_UART_RX_GPIO_Port GPIOC

#define CHARGE_KEY_Pin GPIO_PIN_4
#define CHARGE_KEY_GPIO_Port GPIOA

#define CHARGE1_KEY_Pin GPIO_PIN_11
#define CHARGE1_KEY_GPIO_Port GPIOG

#define VCHG_OUT_CH9343_Pin GPIO_PIN_11
#define VCHG_OUT_CH9343_GPIO_Port GPIOB

#define CH9343_POWER_Pin GPIO_PIN_7
#define CH9343_POWER_GPIO_Port GPIOA

#define VCHG_POWER_Pin GPIO_PIN_12
#define VCHG_POWER_GPIO_Port GPIOB

#define BLE_UART_TX_Pin GPIO_PIN_0
#define BLE_UART_TX_GPIO_Port GPIOA
#define BLE_UART_RX_Pin GPIO_PIN_1
#define BLE_UART_RX_GPIO_Port GPIOA

#define BLE_POWER_Pin GPIO_PIN_6
#define BLE_POWER_GPIO_Port GPIOC
#define BLE_RESET_Pin GPIO_PIN_14
#define BLE_RESET_GPIO_Port GPIOG
#define BLE_2_MCU_INT_Pin GPIO_PIN_8
#define BLE_2_MCU_INT_GPIO_Port GPIOB
#define MCU_2_BLE_INT_Pin GPIO_PIN_10
#define MCU_2_BLE_INT_GPIO_Port GPIOB

#define LED_9523_RESET_Pin GPIO_PIN_3
#define LED_9523_RESET_GPIO_Port GPIOB
#define LED_I2C_SDA_Pin GPIO_PIN_14
#define LED_I2C_SDA_GPIO_Port GPIOB
#define LED_I2C_SCL_Pin GPIO_PIN_13
#define LED_I2C_SCL_GPIO_Port GPIOB

#define SCL_Pin GPIO_PIN_7
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_6
#define SDA_GPIO_Port GPIOB

#endif
