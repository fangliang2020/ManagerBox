/* Includes ------------------------------------------------------------------*/
#include "i2c.h"
#include "gpio.h"



I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 400000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if(i2cHandle->Instance == I2C1)
    {
        /* USER CODE BEGIN I2C1_MspInit 0 */

        /* USER CODE END I2C1_MspInit 0 */

        /**I2C1 GPIO Configuration
        PB8     ------> I2C1_SCL
        PB9     ------> I2C1_SDA
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();
        /* USER CODE BEGIN I2C1_MspInit 1 */

        /* USER CODE END I2C1_MspInit 1 */
    }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

    if(i2cHandle->Instance == I2C1)
    {
        /* USER CODE BEGIN I2C1_MspDeInit 0 */

        /* USER CODE END I2C1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_I2C1_CLK_DISABLE();

        /**I2C1 GPIO Configuration
        PB8     ------> I2C1_SCL
        PB9     ------> I2C1_SDA
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);

        /* USER CODE BEGIN I2C1_MspDeInit 1 */

        /* USER CODE END I2C1_MspDeInit 1 */
    }
}


/**
 * @brief  错误回调
 * @param  addr：i2c地址
 * @retval 返回值：无
 */
static void I2C_Error( uint8_t Addr )
{

    /* De-initialize the I2C comunication bus */
    HAL_I2C_DeInit( &hi2c1 );

    /* Re-Initiaize the I2C comunication bus */
    HAL_I2C_Init(&hi2c1);

    printf("Restart i2c err=%d!\r\n", Addr);
}




/**
* @brief 	通过I2C写入一个值到指定寄存器内
* @param 	Addr：I2C设备地址
* @param 	Reg：目标寄存器
* @param 	Value：值
* @retval 0: 成功
* @retval 1: 失败
*/
HAL_StatusTypeDef I2C_WriteData( uint8_t Addr, uint8_t Reg, uint8_t* Value)
{
    HAL_StatusTypeDef status = HAL_OK;
    status = HAL_I2C_Mem_Write( &hi2c1, Addr, ( uint16_t )Reg, I2C_MEMADD_SIZE_8BIT, Value, 1, 0x1000 );
    if( status != HAL_OK )
    {
        I2C_Error( Addr );
        return HAL_ERROR;
    }
    else
    {
        return HAL_OK;
    }
}

/**
* @brief 	通过I2C写入一段数据到指定寄存器内
* @param 	Addr：I2C设备地址
* @param 	Reg：目标寄存器
* @param 	pBuffer：数据
* @param 	size : 数据长度
* @retval 0: 成功
* @retval 1: 失败
* @tips   在循环调用是需加一定延时时间
*/
HAL_StatusTypeDef I2C_WriteBuffer( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size )
{

    HAL_StatusTypeDef status = HAL_OK;
    status = HAL_I2C_Mem_Write( &hi2c1, Addr, ( uint16_t )Reg, I2C_MEMADD_SIZE_8BIT, pBuffer, Size, 0x1000 );
    /* Check the communication status */
    if( status != HAL_OK )
    {
        I2C_Error( Addr );
        return HAL_ERROR;
    }
    else
    {
        return HAL_OK;
    }
}


/**
* @brief 	通过I2C读取一个指定寄存器内容
* @param 	Addr：I2C设备地址
* @param 	Reg：目标寄存器
* @retval uint8_t：寄存器内容
*/
HAL_StatusTypeDef I2C_ReadData( uint8_t Addr, uint8_t Reg, uint8_t* value)
{

    HAL_StatusTypeDef status = HAL_OK;
    status = HAL_I2C_Mem_Read( &hi2c1, Addr, ( uint16_t )Reg, I2C_MEMADD_SIZE_8BIT, value, 1, 0x1000);
    if( status != HAL_OK )
    {
        I2C_Error( Addr );
        return HAL_ERROR;
    }
    else
    {
        return HAL_OK;
    }
}


/**
 * @brief  通过I2C读取一段寄存器内容存放到指定的缓冲区内
 * @param  Addr：I2C设备地址
 * @param  Reg：目标寄存器
 * @param  pBuffer：缓冲区指针
 * @param  Size:缓存区长度
 * @retval 0:成功
 * @retval 1:失败
 */
HAL_StatusTypeDef I2C_ReadBuffer( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size )
{
    HAL_StatusTypeDef status = HAL_OK;
    status = HAL_I2C_Mem_Read( &hi2c1, Addr, ( uint16_t )Reg, I2C_MEMADD_SIZE_8BIT, pBuffer, Size, 0x1000); //数据位8位
    if( status != HAL_OK )
    {
        I2C_Error( Addr );
        return HAL_ERROR;
    }
    else
    {
        return HAL_OK;
    }
}

/**
 * @brief  检测I2C设备是否处于准备好可以通信状态
 * @param  DevAddress：I2C设备地址
 * @param  Trials：尝试测试次数
 * @retval HAL_StatusTypeDef：操作结果
 */
HAL_StatusTypeDef I2C_IsDeviceReady(uint8_t DevAddress, uint8_t Trials)
{
    return(HAL_I2C_IsDeviceReady(&hi2c1, DevAddress, Trials, 0x1000));
}







