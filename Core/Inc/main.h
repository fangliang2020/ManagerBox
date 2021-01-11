/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "spl06.h"
#include "LSM6DSO.h"
#include "W25QXX.h"
#include "bsp_uart.h"
#include "es600s.h"
#include "protocol.h"
#include "queue.h"
#include "malloc.h"
#include "cJSON.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct{
	uint8_t status;
	float X_ACCEL;
	float Y_ACCEL;
	float Z_ACCEL;
	float YAW;
	float ROLL;
	float PITCH;
}IMUDATA;
typedef struct{
	float temperature;
	float pressure;
	IMUDATA Imu;
}SYSTEM;
extern SYSTEM g_Info;

typedef struct
{
	 uint8_t status;
	 uint8_t sample_timer;
	 int16_t longitude; //经度
	 int16_t Latitude;// 经度
}GPS_DATA;
extern GPS_DATA GPS_Info;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern 	uint8_t flag_delay200ms,flag_delay500ms,flag_delay1s,flag_delay3s;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define D_FATAL 1
#define D_ALARM 2
#define D_ERROR 3
#define D_WARN  4
#define D_INFO  5
#define D_DEBUG 6

#define D_INFOR_LEVEL D_DEBUG
#define Logln(level, format, ...) do { \
    if (level <= D_INFOR_LEVEL) { \
        printf(format"\r\n", ##__VA_ARGS__); \
    } \
} while(0)
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MODEM_PWR_ON_OFF_Pin GPIO_PIN_13
#define MODEM_PWR_ON_OFF_GPIO_Port GPIOC
#define PWR_I2C3_SCL_Pin GPIO_PIN_0
#define PWR_I2C3_SCL_GPIO_Port GPIOC
#define TIMEART_Pin GPIO_PIN_1
#define TIMEART_GPIO_Port GPIOC
#define GPS_WAKEUP_Pin GPIO_PIN_2
#define GPS_WAKEUP_GPIO_Port GPIOC
#define GPS_TX_Pin GPIO_PIN_0
#define GPS_TX_GPIO_Port GPIOA
#define GPS_RX_Pin GPIO_PIN_1
#define GPS_RX_GPIO_Port GPIOA
#define DEBUG_TX_Pin GPIO_PIN_2
#define DEBUG_TX_GPIO_Port GPIOA
#define DEBUG_RX_Pin GPIO_PIN_3
#define DEBUG_RX_GPIO_Port GPIOA
#define SPI3_CS_Pin GPIO_PIN_4
#define SPI3_CS_GPIO_Port GPIOA
#define MODEM_RST_Pin GPIO_PIN_5
#define MODEM_RST_GPIO_Port GPIOA
#define MODEM_RX_Pin GPIO_PIN_5
#define MODEM_RX_GPIO_Port GPIOC
#define MODEM_PWR_CTRL_Pin GPIO_PIN_12
#define MODEM_PWR_CTRL_GPIO_Port GPIOB
#define MODEM_CTS_Pin GPIO_PIN_13
#define MODEM_CTS_GPIO_Port GPIOB
#define MODEM_RTS_Pin GPIO_PIN_14
#define MODEM_RTS_GPIO_Port GPIOB
#define KEY_Pin GPIO_PIN_8
#define KEY_GPIO_Port GPIOC
#define KEY_EXTI_IRQn EXTI8_IRQn
#define IS_CTRL_Pin GPIO_PIN_9
#define IS_CTRL_GPIO_Port GPIOC
#define GPS_PWR_EN_Pin GPIO_PIN_8
#define GPS_PWR_EN_GPIO_Port GPIOA
#define IMU_PWR_CTRL_Pin GPIO_PIN_9
#define IMU_PWR_CTRL_GPIO_Port GPIOA
#define MODEM_TX_Pin GPIO_PIN_10
#define MODEM_TX_GPIO_Port GPIOC
#define BAT_INT_Pin GPIO_PIN_2
#define BAT_INT_GPIO_Port GPIOD
#define BAT_INT_EXTI_IRQn EXTI2_IRQn
#define PWR_I2C3_SDA_Pin GPIO_PIN_4
#define PWR_I2C3_SDA_GPIO_Port GPIOB
#define IMU_INT_Pin GPIO_PIN_5
#define IMU_INT_GPIO_Port GPIOB
#define IMU_INT_EXTI_IRQn EXTI5_IRQn
#define IMU_I2C4_SCL_Pin GPIO_PIN_6
#define IMU_I2C4_SCL_GPIO_Port GPIOB
#define IMU_I2C4_SDA_Pin GPIO_PIN_7
#define IMU_I2C4_SDA_GPIO_Port GPIOB
#define QY_I2C1_SCL_Pin GPIO_PIN_8
#define QY_I2C1_SCL_GPIO_Port GPIOB
#define QY_I2C1_SDA_Pin GPIO_PIN_9
#define QY_I2C1_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
