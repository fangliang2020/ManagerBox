#ifndef BSP_UART_H
#define BSP_UART_H
#include "stm32l5xx_hal.h"

#define BUFFER_SIZE 255
extern uint8_t 	Gsm_recv_flag;
extern char module_recv_buffer[BUFFER_SIZE];
extern unsigned short module_recv_write_index;

typedef struct UartData
{	
	uint8_t gpsTxData[255];
	uint8_t gsmTxData[255];
	uint8_t pcTxData[255];
	
	uint8_t gpsRxData[255];
	uint8_t gsmRxData[255];
	uint8_t pcRxData[255];
}UartData;

extern UartData gUartData;
void GsmSendData(uint8_t* sendData, uint16_t sendLength);
void USER_UART3_IDLECallback(void);
void 	USER_UART_IRQHandler(UART_HandleTypeDef *huart);
void USER_UART_IDLECallback(UART_HandleTypeDef *huart);
#endif


