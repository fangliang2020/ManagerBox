#include "LSM6DS0.h"
#include "i2c.h"


/*********************************************基础函数************************************************/



/*******************************************************************************
* 函数名  : LSM6DS0_GET_ID
* 描述    : 获得器件id
* 输入    : 无
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_GET_ID(uint8_t *id)
{
    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_WHO_AM_I_REG, id) != HAL_OK)
        return HAL_ERROR;
    return HAL_OK;
}

/*******************************************************************************
* 函数名  : LSM6DS0_Check_ID
* 描述    : 检查器件id是否正确
* 输入    : 无
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_Check_ID(void)
{
    uint8_t id ;
    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_WHO_AM_I_REG, &id) != HAL_OK)
        return HAL_ERROR;
    if(id != LSM6DS0_WHO_AM_I)
        return HAL_ERROR;
    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_SET_AutoIndexOnMultiAccess
* 描述    : 设置串行端口数据地址自增
* 输入    : LSM6DS0_IF_ADD_INC_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : 设置CTRL_REG1_G8 (22h)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_SET_AutoIndexOnMultiAccess(LSM6DS0_IF_ADD_INC_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG8, &value) != HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_IF_ADD_INC_MASK;//避免改变其他值
    value |= newValue;

    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG8, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_SET_BlockDataUpdate
* 描述    : 设置数据块输出
* 输入    : LSM6DS0_BDU_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : 设置CTRL_REG1_G8 (22h)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_SET_BlockDataUpdate(LSM6DS0_BDU_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG8, &value) != HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_BDU_MASK;
    value |= newValue;

    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG8, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_SET_AccelerometerDataRate
* 描述    : 设置加速度输出速率
* 输入    : LSM6DS0_ODR_XL_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : CTRL_REG6_XL (20h)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_SET_AccelerometerDataRate(LSM6DS0_ODR_XL_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG6_XL, &value)!= HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_ODR_XL_MASK;
    value |= newValue;

    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG6_XL, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_SET_GyroDataRate
* 描述    : 设置陀螺仪输出速率
* 输入    :
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : LSM6DS0_CTRL_REG1_G (10h)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_SET_GyroDataRate(LSM6DS0_ODR_G_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG1_G, &value)!= HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_ODR_G_MASK;
    value |= newValue;

    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG1_G, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}


/*******************************************************************************
* 函数名  : LSM6DS0_X_SET_FS
* 描述    : 设置加速度最大刻度
* 输入    : LSM6DS0_FS_XL_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : CTRL_REG6_XL (20h)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_X_SET_FS(LSM6DS0_FS_XL_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG6_XL, &value) != HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_FS_XL_MASK;
    value |= newValue;
    //先设置加速度计的最大量程
    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG6_XL, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}

/*******************************************************************************
* 函数名  : LSM6DS0_X_GET_FS
* 描述    : 获得加速度最大刻度
* 输入    : LSM6DS0_FS_XL_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : CTRL_REG6_XL (20h)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_X_GET_FS(LSM6DS0_FS_XL_t *value)
{
    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG6_XL, (uint8_t *)value) != HAL_OK)
        return HAL_ERROR;

    *value &= LSM6DS0_FS_XL_MASK; //mask

    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_G_SET_FS
* 描述    : 设置陀螺仪最大刻度
* 输入    : LSM6DS0_FS_XL_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : LSM6DS0_CTRL_REG1_G (10h)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_G_SET_FS(LSM6DS0_FS_G_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG1_G, &value) != HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_FS_G_MASK;
    value |= newValue;
    //先设置加速度计的最大量程
    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG1_G, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}

/*******************************************************************************
* 函数名  : LSM6DS0_G_GET_FS
* 描述    : 获得陀螺仪最大刻度
* 输入    : LSM6DS0_FS_XL_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : LSM6DS0_CTRL_REG1_G (10h)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_G_GET_FS(LSM6DS0_FS_G_t *value)
{
    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG1_G, (uint8_t *)value) != HAL_OK)
        return HAL_ERROR;

    *value &= LSM6DS0_FS_G_MASK; //mask

    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_SET_AccelerometerAxisX
* 描述    : 设置X轴加速度输出状态
* 输入    : LSM6DS0_XEN_XL_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : CTRL_REG5_XL (1Fh)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_SET_AccelerometerAxisX(LSM6DS0_XEN_XL_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG5_XL, &value)!=HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_XEN_XL_MASK;
    value |= newValue;

    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG5_XL, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_SET_AccelerometerAxisY
* 描述    : 设置Y轴加速度输出状态
* 输入    : LSM6DS0_YEN_XL_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : CTRL_REG5_XL (1Fh)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_SET_AccelerometerAxisY(LSM6DS0_YEN_XL_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG5_XL, &value)!=HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_YEN_XL_MASK;
    value |= newValue;

    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG5_XL, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_SET_AccelerometerAxisZ
* 描述    : 设置Z轴加速度输出状态
* 输入    : LSM6DS0_ZEN_XL_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : CTRL_REG5_XL (1Fh)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_SET_AccelerometerAxisZ(LSM6DS0_ZEN_XL_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG5_XL, &value)!=HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_ZEN_XL_MASK;
    value |= newValue;

    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG5_XL, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_X_GET_Axes_Status
* 描述    : 获得三个方向加速度输出状态
* 输入    : uint8_t *xyz_enabled
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    :
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_X_Get_Axes_Status(uint8_t *xyz_enabled)
{
    uint8_t xStatus;
    uint8_t yStatus;
    uint8_t zStatus;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH,LSM6DS0_CTRL_REG5_XL,&xStatus)!=HAL_OK)
        return HAL_ERROR;
    xStatus &= LSM6DS0_XEN_XL_MASK;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH,LSM6DS0_CTRL_REG5_XL,&yStatus)!=HAL_OK)
        return HAL_ERROR;
    yStatus &= LSM6DS0_YEN_XL_MASK;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH,LSM6DS0_CTRL_REG5_XL,&zStatus)!=HAL_OK)
        return HAL_ERROR;
    zStatus &= LSM6DS0_ZEN_XL_MASK;

    xyz_enabled[0]=(xStatus==LSM6DS0_XEN_XL_ENABLE)? 1 : 0;
    xyz_enabled[1]=(yStatus==LSM6DS0_YEN_XL_ENABLE)? 1 : 0;
    xyz_enabled[2]=(zStatus==LSM6DS0_ZEN_XL_ENABLE)? 1 : 0;

    return HAL_OK;
}


/*******************************************************************************
* 函数名  : LSM6DS0_X_SET_Axes_Status
* 描述    : 设置三个方向加速度输出状态
* 输入    : enable_xyz
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    :
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_X_SET_Axes_Status(uint8_t *enable_xyz)
{
    if(LSM6DS0_SET_AccelerometerAxisX((1 == enable_xyz[0]) ? LSM6DS0_XEN_XL_ENABLE : LSM6DS0_XEN_XL_DISABLE) != HAL_OK)
        return HAL_ERROR;
    if(LSM6DS0_SET_AccelerometerAxisY((1 == enable_xyz[1]) ? LSM6DS0_YEN_XL_ENABLE : LSM6DS0_YEN_XL_DISABLE) != HAL_OK)
        return HAL_ERROR;
    if(LSM6DS0_SET_AccelerometerAxisZ((1 == enable_xyz[2]) ? LSM6DS0_ZEN_XL_ENABLE : LSM6DS0_ZEN_XL_DISABLE) != HAL_OK)
        return HAL_ERROR;
    return HAL_OK;
}

/*******************************************************************************
* 函数名  : LSM6DS0_SET_GyroAxisX
* 描述    : 设置X轴陀螺仪输出状态
* 输入    : LSM6DS0_XEN_XL_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : LSM6DS0_CTRL_REG4 (1Eh)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_SET_GyroAxisX(LSM6DS0_XEN_G_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG4, &value)!=HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_XEN_G_MASK;
    value |= newValue;

    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG4, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_SET_GyroAxisY
* 描述    : 设置Y轴陀螺仪输出状态
* 输入    : LSM6DS0_YEN_XL_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : LSM6DS0_CTRL_REG4 (1Eh)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_SET_GyroAxisY(LSM6DS0_YEN_G_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG4, &value)!=HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_YEN_G_MASK;
    value |= newValue;

    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG4, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_SET_GyroAxisZ
* 描述    : 设置Z轴陀螺仪输出状态
* 输入    : LSM6DS0_ZEN_XL_t
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    : LSM6DS0_CTRL_REG4 (1Eh)寄存器
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_SET_GyroAxisZ(LSM6DS0_ZEN_G_t newValue)
{
    uint8_t value ;

    if(I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG4, &value)!=HAL_OK)
        return HAL_ERROR;

    value &= ~LSM6DS0_ZEN_G_MASK;
    value |= newValue;

    if(I2C_WriteData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_CTRL_REG4, &value) != HAL_OK)
        return HAL_ERROR;

    return HAL_OK;
}

/*******************************************************************************
* 函数名  : LSM6DS0_G_Set_Axes_Status
* 描述    : 设置陀螺仪三轴的状态
* 输入    : enable_xyz
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    :
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_G_Set_Axes_Status(uint8_t *enable_xyz)
{
    if(LSM6DS0_SET_GyroAxisX((1 == enable_xyz[0]) ? LSM6DS0_XEN_G_ENABLE : LSM6DS0_XEN_G_DISABLE) != HAL_OK)
        return HAL_ERROR;
    if(LSM6DS0_SET_GyroAxisY((1 == enable_xyz[1]) ? LSM6DS0_YEN_G_ENABLE : LSM6DS0_YEN_G_DISABLE) != HAL_OK)
        return HAL_ERROR;
    if(LSM6DS0_SET_GyroAxisZ((1 == enable_xyz[2]) ? LSM6DS0_ZEN_G_ENABLE : LSM6DS0_ZEN_G_DISABLE) != HAL_OK)
        return HAL_ERROR;
    return HAL_OK;
}

/*******************************************************************************
* 函数名  : LSM6DS0_X_GET_Axes_Raw
* 描述    : 获得三轴加速度原始数据
* 输入    : *pData 数据数组地址
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    :
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_X_GET_Axes_Raw( int16_t *pData )
{
    uint8_t regValue[6] = {0, 0, 0, 0, 0, 0};
    /* 从LSM6DS0_ACC_GYRO_OUT_X_L_XL 到 LSM6DS0_ACC_GYRO_OUT_Z_H_XL 读取数据. */

    uint8_t i, j, k;
    k = 0;
    for (i = 0; i < 3; i++ )
    {
        for (j = 0; j < 2; j++ )
        {
            if( I2C_ReadData( LSM6DS0_ADDRESS_HIGH, LSM6DS0_OUT_X_L_XL + k, &regValue[k] ) != HAL_OK) //从0x28读到2d 每两个地址对应一个轴的数据
                return HAL_ERROR;
            k++;
        }
    }

    /* 格式化输出 */
    pData[0] = ( ( ( ( int16_t )regValue[1] ) << 8 ) + ( int16_t )regValue[0] );
    pData[1] = ( ( ( ( int16_t )regValue[3] ) << 8 ) + ( int16_t )regValue[2] );
    pData[2] = ( ( ( ( int16_t )regValue[5] ) << 8 ) + ( int16_t )regValue[4] );

    return HAL_OK;
}
/*******************************************************************************
* 函数名  : LSM6DS0_X_GET_Sensitivity
* 描述    : 获取加速度灵敏度
* 输入    : *sensitivity
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    :
*******************************************************************************/
HAL_StatusTypeDef	LSM6DS0_X_GET_Sensitivity(float *sensitivity)
{
    LSM6DS0_FS_XL_t fullScale;//最大刻度变量

    /* 读取加速度传感器最大刻度 */
    if(LSM6DS0_X_GET_FS(&fullScale) != HAL_OK)
        return HAL_ERROR;

    switch( fullScale )
    {
    case LSM6DS0_FS_XL_2g:
        *sensitivity = ( float )LSM6DS0_ACC_FS_2G;
        break;
    case LSM6DS0_FS_XL_4g:
        *sensitivity = ( float )LSM6DS0_ACC_FS_4G;
        break;
    case LSM6DS0_FS_XL_8g:
        *sensitivity = ( float )LSM6DS0_ACC_FS_8G;
        break;
    case LSM6DS0_FS_XL_16g:
        *sensitivity = ( float )LSM6DS0_ACC_FS_16G;
        break;
    default:
        *sensitivity = -1.0f;
        return HAL_ERROR;
    }
    return HAL_OK;
}



/*******************************************************************************
* 函数名  : LSM6DS0_G_Get_Axes_Raw
* 描述    : 获得三轴陀螺仪原始数据
* 输入    : *pData 数据数组地址
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    :
*******************************************************************************/
HAL_StatusTypeDef	LSM6DS0_G_Get_Axes_Raw(int16_t *pData)
{
    uint8_t regValue[6] = {0, 0, 0, 0, 0, 0};

    /* 从 LSM6DS0_ACC_GYRO_OUT_X_L_G 读取到 LSM6DS0_ACC_GYRO_OUT_Z_H_G. */

    uint8_t i, j, k;

    k = 0;
    for (i = 0; i < 3; i++ )
    {
        for (j = 0; j < 2; j++ )
        {
            if( I2C_ReadData(LSM6DS0_ADDRESS_HIGH, LSM6DS0_OUT_X_L_G + k, &regValue[k])!=HAL_OK)
                return HAL_ERROR;
            k++;
        }
    }

    /* 格式化数据 */
    pData[0] = ( ( ( ( int16_t )regValue[1] ) << 8 ) + ( int16_t )regValue[0] );
    pData[1] = ( ( ( ( int16_t )regValue[3] ) << 8 ) + ( int16_t )regValue[2] );
    pData[2] = ( ( ( ( int16_t )regValue[5] ) << 8 ) + ( int16_t )regValue[4] );

    return HAL_OK;

}

/*******************************************************************************
* 函数名  : LSM6DS0_X_GET_Sensitivity
* 描述    : 获取陀螺仪灵敏度
* 输入    : *sensitivity
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
* 描述    :
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_G_Get_Sensitivity( float *sensitivity )
{
    LSM6DS0_FS_G_t fullScale;

    /* 读取陀螺仪最大刻度 */
    if ( LSM6DS0_G_GET_FS( &fullScale ) != HAL_OK )
    {
        return HAL_ERROR;
    }

    /* 选择对应的敏感度 */
    switch( fullScale )
    {
    case LSM6DS0_FS_G_245dps:
        *sensitivity = ( float )LSM6DS0_GYRO_FS_245DPS;
        break;
    case LSM6DS0_FS_G_500dps:
        *sensitivity = ( float )LSM6DS0_GYRO_FS_500DPS;
        break;
    case LSM6DS0_FS_G_2000dps:
        *sensitivity = ( float )LSM6DS0_GYRO_FS_2000DPS;
        break;
    default:
        *sensitivity = -1.0f;
        return HAL_ERROR;
    }

    return HAL_OK;

}








/*********************************************高层函数************************************************/




/*******************************************************************************
* 函数名  : LSM6DS0_X_Init
* 描述    : 初始化lsm6ds0加速度部分
* 输入    : 无
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_X_Init(void)
{
    uint8_t axes_status[] = { 1, 1, 1 };
    /* 检查器件ID */
    if(LSM6DS0_Check_ID()!=HAL_OK) {
        return HAL_ERROR;
    }
    /* 使能地址自增 */
    if(LSM6DS0_SET_AutoIndexOnMultiAccess(LSM6DS0_IF_ADD_INC_ENABLE) != HAL_OK) {
        return HAL_ERROR;
    }
    /* 使能BDU */
    if(LSM6DS0_SET_BlockDataUpdate(LSM6DS0_BDU_ENABLE) != HAL_OK) {
        return HAL_ERROR;
    }
    /* 设置加速度计最大刻度 */
    if(LSM6DS0_X_SET_FS(LSM6DS0_FS_XL_2g) != HAL_OK) {
        return HAL_ERROR;
    }
    /* 设置陀螺仪最大刻度 */
    if(LSM6DS0_G_SET_FS(LSM6DS0_FS_G_2000dps) != HAL_OK) {
        return HAL_ERROR;
    }
    /* 使能加速度计 */
    if(LSM6DS0_X_SET_Axes_Status(axes_status) != HAL_OK) {
        return HAL_ERROR;
    }
    /* 使能陀螺仪 */
    if(LSM6DS0_G_Set_Axes_Status(axes_status) != HAL_OK) {
        return HAL_ERROR;
    }
    /* 设置加速度计输出速率 */
    if(LSM6DS0_SET_AccelerometerDataRate(LSM6DS0_ODR_XL_119Hz) != HAL_OK) {
        return HAL_ERROR;
    }
    /* 设置陀螺仪输出速率 */
    if(LSM6DS0_SET_GyroDataRate(LSM6DS0_ODR_G_119Hz) !=HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}


/*******************************************************************************
* 函数名  : LSM6DS0_X_GET_Axes
* 描述    : 获得lsm6dsz 3轴加速度值
* 输入    : SensorAxes_t *acceleration
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_X_GET_Axes( SensorAxes_t *acceleration)
{
    int16_t dataRaw[3];
    uint8_t regValue[6] = {0, 0, 0, 0, 0, 0};

    /* 从寄存器获取原始数据 */
    if(I2C_ReadBuffer(LSM6DS0_ADDRESS_HIGH,LSM6DS0_OUT_X_L_XL,regValue,6)!=HAL_OK)
        return HAL_ERROR;

    /* 格式化输出 */
    dataRaw[0] = ( ( ( ( int16_t )regValue[1] ) << 8 ) + ( int16_t )regValue[0] );
    dataRaw[1] = ( ( ( ( int16_t )regValue[3] ) << 8 ) + ( int16_t )regValue[2] );
    dataRaw[2] = ( ( ( ( int16_t )regValue[5] ) << 8 ) + ( int16_t )regValue[4] );

    /* 计算数据 这里为了提高运行速度,灵敏度直接自己写进来*/
    acceleration->AXIS_X = ( int32_t )( dataRaw[0] * LSM6DS0_ACC_FS_2G );
    acceleration->AXIS_Y = ( int32_t )( dataRaw[1] * LSM6DS0_ACC_FS_2G );
    acceleration->AXIS_Z = ( int32_t )( dataRaw[2] * LSM6DS0_ACC_FS_2G );

    return HAL_OK;
}


/*******************************************************************************
* 函数名  : LSM6DS0_G_Get_Axes
* 描述    : 获得lsm6dsz 3轴角速度值
* 输入    : 无
* 输出    : 无
* 返回值  : HAL_StatusTypeDef
*******************************************************************************/
HAL_StatusTypeDef LSM6DS0_G_Get_Axes(SensorAxes_t *angular_velocity)
{
    int16_t dataRaw[3];
    uint8_t regValue[6] = {0, 0, 0, 0, 0, 0};

    /* 读取原始输出 */
    if(I2C_ReadBuffer(LSM6DS0_ADDRESS_HIGH,LSM6DS0_OUT_X_L_G,regValue,6)!=HAL_OK)
        return HAL_ERROR;
    /* 格式化输出 */
    dataRaw[0] = ( ( ( ( int16_t )regValue[1] ) << 8 ) + ( int16_t )regValue[0] );
    dataRaw[1] = ( ( ( ( int16_t )regValue[3] ) << 8 ) + ( int16_t )regValue[2] );
    dataRaw[2] = ( ( ( ( int16_t )regValue[5] ) << 8 ) + ( int16_t )regValue[4] );


    /* 计算数据 这里为了提高运行速度,灵敏度直接自己写进来*/
    angular_velocity->AXIS_X = ( int32_t )( dataRaw[0] * LSM6DS0_GYRO_FS_2000DPS );
    angular_velocity->AXIS_Y = ( int32_t )( dataRaw[1] * LSM6DS0_GYRO_FS_2000DPS );
    angular_velocity->AXIS_Z = ( int32_t )( dataRaw[2] * LSM6DS0_GYRO_FS_2000DPS );

    return HAL_OK;

}


















