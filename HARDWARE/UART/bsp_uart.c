#include "main.h"

uint8_t 	Gsm_recv_flag;
char module_recv_buffer[BUFFER_SIZE] = {0};
unsigned short module_recv_write_index = 0;
/*************GSM���ݽ���*********/
int GsmProtocolParse(uint8_t* msg, uint16_t frameLength)
{	
	memcpy(module_recv_buffer,msg,frameLength);  
	module_recv_write_index=frameLength;
	Gsm_recv_flag=1;
	return 0;
}
/*void 	USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART3)
	{
		if(RESET!=__HAL_UART_GET_FLAG(&huart3,UART_FLAG_IDLE)) //�ж��Ƿ�Ϊ�����ж�
		{			
			 __HAL_UART_CLEAR_IDLEFLAG(&huart3); //�����־λ
			 USER_UART_IDLECallback(huart);	
		}		
	}
	
}

void USER_UART_IDLECallback(UART_HandleTypeDef *huart)
{	
	 HAL_UART_DMAStop(&huart3);  //ֹͣDMA����
	 uint8_t data_length  = BUFFER_SIZE -__HAL_DMA_GET_COUNTER(&hdma_usart3_rx);   //������յ������ݳ���			
	 GsmProtocolParse(gUartData.gsmRxData, data_length);
	 memset(gUartData.gsmRxData,0,data_length); 
	 data_length=0;
	 HAL_UART_Receive_DMA(&huart3,gUartData.gsmRxData, 255);                    //������ʼDMA���� ÿ��255�ֽ�����			
}
*/
//void USER_UART3_IDLECallback(void)
//{
//		if(RESET!=__HAL_UART_GET_FLAG(&huart3,UART_FLAG_IDLE)) //�ж��Ƿ�Ϊ�����ж�
//		{			
//			 __HAL_UART_CLEAR_IDLEFLAG(&huart3); //�����־λ
//		   HAL_UART_DMAStop(&huart3);  //ֹͣDMA����
//			 uint8_t data_length  = BUFFER_SIZE -__HAL_DMA_GET_COUNTER(&hdma_usart3_rx);   //������յ������ݳ���			
//			 GsmProtocolParse(gUartData.gsmRxData, data_length);
//			 memset(gUartData.gsmRxData,0,data_length); 
//			HAL_UART_Receive_DMA(&huart3,gUartData.gsmRxData, 255);                    //������ʼDMA���� ÿ��255�ֽ�����			
//		}
//	
//}
//void DMA_USART_SEND(UART_HandleTypeDef *huart,uint8_t *buf,uint8_t len)
//{
//	if(HAL_UART_Transmit_DMA(huart, buf,len)!= HAL_OK) //�ж��Ƿ�����������������쳣������쳣�жϺ���
//  {
//		Error_Handler();
//  }
//}

void GsmSendData(uint8_t* sendData, uint16_t sendLength)	//GSM���ͺ���
{
	HAL_UART_Transmit(&huart3, sendData,sendLength,10);
}