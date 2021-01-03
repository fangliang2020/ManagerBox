#ifndef __LSM6DS0_H
#define __LSM6DS0_H

#include "stdint.h"
#include "stm32f4xx_hal.h"
/************************************* 器件地址 ***************************************************************/

#define LSM6DS0_ADDRESS_LOW   		0xD4  // SAD[0] = 0
#define LSM6DS0_ADDRESS_HIGH  		0xD6  // SAD[0] = 1 此时sda接高 用这个

/************************************* 器件ID  *****************************************************************/

#define LSM6DS0_WHO_AM_I         	0x68

/************************************* 寄存器地址  *****************************************************************/
#define LSM6DS0_ACT_THS    				0X04
#define LSM6DS0_ACT_DUR    				0X05
#define LSM6DS0_INT_GEN_CFG_XL   	0X06
#define LSM6DS0_INT_GEN_THS_X_XL  0X07
#define LSM6DS0_INT_GEN_THS_Y_XL  0X08
#define LSM6DS0_INT_GEN_THS_Z_XL  0X09
#define LSM6DS0_INT_GEN_DUR_XL    0X0A
#define LSM6DS0_REFERENCE_G    		0X0B
#define LSM6DS0_INT_CTRL   				0X0C
#define LSM6DS0_WHO_AM_I_REG   		0X0F
#define LSM6DS0_CTRL_REG1_G    		0X10
#define LSM6DS0_CTRL_REG2_G    		0X11
#define LSM6DS0_CTRL_REG3_G    		0X12
#define LSM6DS0_ORIENT_CFG_G   		0X13
#define LSM6DS0_INT_GEN_SRC_G    	0X14
#define LSM6DS0_OUT_TEMP_L   			0X15
#define LSM6DS0_OUT_TEMP_H   			0X16
#define LSM6DS0_OUT_X_L_G    			0X18
#define LSM6DS0_OUT_X_H_G    			0X19
#define LSM6DS0_OUT_Y_L_G    			0X1A
#define LSM6DS0_OUT_Y_H_G    			0X1B
#define LSM6DS0_OUT_Z_L_G    			0X1C
#define LSM6DS0_OUT_Z_H_G    			0X1D
#define LSM6DS0_CTRL_REG4    			0X1E
#define LSM6DS0_CTRL_REG5_XL   		0X1F
#define LSM6DS0_CTRL_REG6_XL   		0X20
#define LSM6DS0_CTRL_REG7_XL   		0X21
#define LSM6DS0_CTRL_REG8      		0X22
#define LSM6DS0_CTRL_REG9      		0X23
#define LSM6DS0_CTRL_REG10     		0X24
#define LSM6DS0_INT_GEN_SRC_XL    0X26
#define LSM6DS0_STATUS_REG     		0X27
#define LSM6DS0_OUT_X_L_XL     		0X28
#define LSM6DS0_OUT_X_H_XL     		0X29
#define LSM6DS0_OUT_Y_L_XL     		0X2A
#define LSM6DS0_OUT_Y_H_XL     		0X2B
#define LSM6DS0_OUT_Z_L_XL     		0X2C
#define LSM6DS0_OUT_Z_H_XL     		0X2D
#define LSM6DS0_FIFO_CTRL      		0X2E
#define LSM6DS0_FIFO_SRC   				0X2F
#define LSM6DS0_INT_GEN_CFG_G     0X30
#define LSM6DS0_INT_GEN_THS_XH_G  0X31
#define LSM6DS0_INT_GEN_THS_XL_G  0X32
#define LSM6DS0_INT_GEN_THS_YH_G  0X33
#define LSM6DS0_INT_GEN_THS_YL_G  0X34
#define LSM6DS0_INT_GEN_THS_ZH_G  0X35
#define LSM6DS0_INT_GEN_THS_ZL_G  0X36
#define LSM6DS0_INT_GEN_DUR_G     0X37

/*************************************end***************************************************************/


/************************************数据参数***********************************************************/

#define LSM6DS0_SENSORS_MAX_NUM  1     /**< LSM6DS0 最大值 */

/** @addtogroup LSM6DS0加速度值参数
 * @{
 */

#define LSM6DS0_ACC_FS_2G   0.061  /**<  2 g full scale [mg/LSB] */
#define LSM6DS0_ACC_FS_4G   0.122  /**<  4 g full scale [mg/LSB] */
#define LSM6DS0_ACC_FS_8G   0.244  /**<  8 g full scale [mg/LSB] */
#define LSM6DS0_ACC_FS_16G  0.732  /**<  16 g full scale [mg/LSB] */

/**
 * @}
 */

/** @addtogroup LSM6DS0陀螺仪值参数
 * @{
 */

#define LSM6DS0_GYRO_FS_245DPS   08.75  /**<  245 dps full scale [mdps/LSB] */
#define LSM6DS0_GYRO_FS_500DPS   17.50  /**<  500 dps full scale [mdps/LSB] */
#define LSM6DS0_GYRO_FS_2000DPS  70.00  /**<  2000 dps full scale [mdps/LSB] */

/*************************************数据结构体********************************************************/


/**
 * 描述 ：用于存放三轴加速度值
 */
typedef struct
{
  int32_t AXIS_X;
  int32_t AXIS_Y;
  int32_t AXIS_Z;
} SensorAxes_t;


/**************************************寄存器参数表*****************************************************/

/*******************************************************************************
* 寄存器  : CTRL_REG1_G
* 地址    : 0X10
* 所属组	: FS_G
* 权限    : 读写
*******************************************************************************/
typedef enum
{
    LSM6DS0_FS_G_245dps     = 0x00,
    LSM6DS0_FS_G_500dps     = 0x08,
    LSM6DS0_FS_G_1000dps      = 0x10,
    LSM6DS0_FS_G_2000dps      = 0x18,
} LSM6DS0_FS_G_t;
#define   LSM6DS0_FS_G_MASK    0x18

/*******************************************************************************
* 寄存器  : CTRL_REG1_G
* 地址    : 0X10
* 所属组	: ODR_G
* 权限    : 读写
*******************************************************************************/
typedef enum
{
    LSM6DS0_ODR_G_POWER_DOWN      = 0x00,
    LSM6DS0_ODR_G_15Hz      = 0x20,
    LSM6DS0_ODR_G_60Hz      = 0x40,
    LSM6DS0_ODR_G_119Hz     = 0x60,
    LSM6DS0_ODR_G_238Hz     = 0x80,
    LSM6DS0_ODR_G_476Hz     = 0xA0,
    LSM6DS0_ODR_G_952Hz     = 0xC0,
} LSM6DS0_ODR_G_t;

#define   LSM6DS0_ODR_G_MASK   0xE0

/*******************************************************************************
* 寄存器  : CTRL_REG1_G
* 地址    : 0X10
* 所属组	: BW_G
* 权限    : 读写
*******************************************************************************/
#define  LSM6DS0_BW_G_LOW      		0x00
#define  LSM6DS0_BW_G_NORMAL       0x01
#define  LSM6DS0_BW_G_HIGH         0x02
#define  LSM6DS0_BW_G_ULTRA_HIGH   0x03



/*******************************************************************************
* 寄存器  : CTRL_REG4_XL
* 地址    : 0X1E
* 所属组	: XEN_G
* 权限	  : RW
*******************************************************************************/
typedef enum
{
  LSM6DS0_XEN_G_DISABLE     = 0x00,
  LSM6DS0_XEN_G_ENABLE      = 0x08,
} LSM6DS0_XEN_G_t;

#define   LSM6DS0_XEN_G_MASK   0x08
/*******************************************************************************
* 寄存器  : CTRL_REG4_XL
* 地址    : 0X1E
* 所属组	: YEN_G
* 权限	  : RW
*******************************************************************************/
typedef enum
{
  LSM6DS0_YEN_G_DISABLE     = 0x00,
  LSM6DS0_YEN_G_ENABLE      = 0x10,
} LSM6DS0_YEN_G_t;

#define   LSM6DS0_YEN_G_MASK   0x10
/*******************************************************************************
* 寄存器  : CTRL_REG4_XL
* 地址    : 0X1E
* 所属组	: YEN_G
* 权限	  : RW
*******************************************************************************/
typedef enum
{
  LSM6DS0_ZEN_G_DISABLE     = 0x00,
  LSM6DS0_ZEN_G_ENABLE      = 0x20,
} LSM6DS0_ZEN_G_t;

#define   LSM6DS0_ZEN_G_MASK   0x20

/*******************************************************************************
* 寄存器  : CTRL_REG5_XL
* 地址    : 0X1F
* 所属组	: XEN_XL
* 权限	  : RW
*******************************************************************************/
typedef enum
{
    LSM6DS0_XEN_XL_DISABLE      = 0x00,
    LSM6DS0_XEN_XL_ENABLE     = 0x08,
} LSM6DS0_XEN_XL_t;

#define   LSM6DS0_XEN_XL_MASK    0x08

/*******************************************************************************
* 寄存器  : CTRL_REG5_XL
* 地址    : 0X1F
* 所属组	: YEN_XL
* 权限	  : RW
*******************************************************************************/
typedef enum
{
    LSM6DS0_YEN_XL_DISABLE      = 0x00,
    LSM6DS0_YEN_XL_ENABLE     = 0x10,
} LSM6DS0_YEN_XL_t;

#define   LSM6DS0_YEN_XL_MASK    0x10
/*******************************************************************************
* 寄存器  : CTRL_REG5_XL
* 地址    : 0X1F
* 所属组	: ZEN_XL
* 权限	  : RW
*******************************************************************************/
typedef enum
{
    LSM6DS0_ZEN_XL_DISABLE      = 0x00,
    LSM6DS0_ZEN_XL_ENABLE     = 0x20,
} LSM6DS0_ZEN_XL_t;

#define   LSM6DS0_ZEN_XL_MASK    0x20

/*******************************************************************************
* 寄存器  : CTRL_REG6_XL
* 地址    : 0X20
* 所属组	: FS_XL
* 权限    : 读写
*******************************************************************************/
typedef enum
{
    LSM6DS0_FS_XL_2g      = 0x00,
    LSM6DS0_FS_XL_4g      = 0x10,
    LSM6DS0_FS_XL_8g      = 0x18,
		LSM6DS0_FS_XL_16g     = 0x08,
} LSM6DS0_FS_XL_t;
#define   LSM6DS0_FS_XL_MASK   0x18

/*******************************************************************************
* 寄存器  : CTRL_REG6_XL
* 地址    : 0X20
* 所属组	: ODR_XL
* 权限    : 读写
*******************************************************************************/
typedef enum
{
    LSM6DS0_ODR_XL_POWER_DOWN     = 0x00,
    LSM6DS0_ODR_XL_10Hz     	  = 0x20,
    LSM6DS0_ODR_XL_50Hz     	  = 0x40,
    LSM6DS0_ODR_XL_119Hz      	  = 0x60,
    LSM6DS0_ODR_XL_238Hz     	  = 0x80,
    LSM6DS0_ODR_XL_476Hz      	  = 0xA0,
    LSM6DS0_ODR_XL_952Hz      	  = 0xC0,
} LSM6DS0_ODR_XL_t;

#define   LSM6DS0_ODR_XL_MASK    0xE0
/*******************************************************************************
* 寄存器  : CTRL_REG6_XL
* 地址    : 0X20
* 所属组	: BW_XL
* 权限    : 读写
*******************************************************************************/
typedef enum
{
    LSM6DS0_BW_XL_408Hz     = 0x00,
    LSM6DS0_BW_XL_211Hz     = 0x01,
    LSM6DS0_BW_XL_105Hz     = 0x02,
    LSM6DS0_BW_XL_50Hz      = 0x03,
} LSM6DS0_BW_XL_t;

#define   LSM6DS0_BW_XL_MASK   0x03
/*******************************************************************************
* 寄存器  : CTRL_REG6_XL
* 地址    : 0X20
* 所属组	: BW_SCAL_ODR
* 权限    : 读写
*******************************************************************************/
typedef enum
{
    LSM6DS0_BW_SCAL_ODR_WITH_ODR      = 0x00,
    LSM6DS0_BW_SCAL_ODR_WITH_BW_XL    = 0x04,
} LSM6DS0_BW_SCAL_ODR_t;

#define   LSM6DS0_BW_SCAL_ODR_MASK   0x04

/*******************************************************************************
* 寄存器  : CTRL_REG8
* 地址    : 0X22
* 所属组	: BDU
* 权限    : 读写
*******************************************************************************/
typedef enum
{
    LSM6DS0_BDU_DISABLE     = 0x00,
    LSM6DS0_BDU_ENABLE      = 0x40,
} LSM6DS0_BDU_t;

#define   LSM6DS0_BDU_MASK   0x40
/*******************************************************************************
* 寄存器  : CTRL_REG8
* 地址    : 0X22
* 所属组	: IF_ADD_INC
* 权限    : RW
* 描述    ：在使用多字节访问时自动增加寄存器地址  串行接口
*******************************************************************************/
typedef enum
{
    LSM6DS0_IF_ADD_INC_DISABLE      = 0x00,
    LSM6DS0_IF_ADD_INC_ENABLE     = 0x04,
} LSM6DS0_IF_ADD_INC_t;

#define   LSM6DS0_IF_ADD_INC_MASK    0x04



/*****************************************函数**********************************************************/



/****底层函数******/
extern HAL_StatusTypeDef LSM6DS0_GET_ID(uint8_t *id);
extern HAL_StatusTypeDef LSM6DS0_Check_ID(void);
extern HAL_StatusTypeDef LSM6DS0_SET_AutoIndexOnMultiAccess(LSM6DS0_IF_ADD_INC_t newValue);
extern HAL_StatusTypeDef LSM6DS0_SET_BlockDataUpdate(LSM6DS0_BDU_t newValue);


extern HAL_StatusTypeDef LSM6DS0_SET_AccelerometerDataRate(LSM6DS0_ODR_XL_t newValue);
extern HAL_StatusTypeDef LSM6DS0_X_SET_FS(LSM6DS0_FS_XL_t newValue);
extern HAL_StatusTypeDef LSM6DS0_X_GET_FS(LSM6DS0_FS_XL_t *value);
extern HAL_StatusTypeDef LSM6DS0_SET_AccelerometerAxisX(LSM6DS0_XEN_XL_t newValue);
extern HAL_StatusTypeDef LSM6DS0_SET_AccelerometerAxisY(LSM6DS0_YEN_XL_t newValue);
extern HAL_StatusTypeDef LSM6DS0_SET_AccelerometerAxisZ(LSM6DS0_ZEN_XL_t newValue);
extern HAL_StatusTypeDef LSM6DS0_X_Get_Axes_Status(uint8_t *xyz_enabled);
extern HAL_StatusTypeDef LSM6DS0_X_SET_Axes_Status(uint8_t *enable_xyz);
extern HAL_StatusTypeDef LSM6DS0_X_GET_Axes_Raw( int16_t *pData );
extern HAL_StatusTypeDef	LSM6DS0_X_GET_Sensitivity(float *sensitivity);


extern HAL_StatusTypeDef LSM6DS0_SET_GyroDataRate(LSM6DS0_ODR_G_t newValue);
extern HAL_StatusTypeDef LSM6DS0_G_SET_FS(LSM6DS0_FS_G_t newValue);
extern HAL_StatusTypeDef LSM6DS0_G_GET_FS(LSM6DS0_FS_G_t *value);
extern HAL_StatusTypeDef LSM6DS0_SET_GyroAxisX(LSM6DS0_XEN_G_t newValue);
extern HAL_StatusTypeDef LSM6DS0_SET_GyroAxisY(LSM6DS0_YEN_G_t newValue);
extern HAL_StatusTypeDef LSM6DS0_SET_GyroAxisZ(LSM6DS0_ZEN_G_t newValue);
extern HAL_StatusTypeDef LSM6DS0_G_Set_Axes_Status(uint8_t *enable_xyz);
extern HAL_StatusTypeDef LSM6DS0_G_Get_Axes_Raw(int16_t *pData);
extern HAL_StatusTypeDef LSM6DS0_G_Get_Sensitivity( float *sensitivity );



/****高层函数******/
extern HAL_StatusTypeDef LSM6DS0_X_Init(void);
extern HAL_StatusTypeDef LSM6DS0_X_GET_Axes( SensorAxes_t *acceleration);
extern HAL_StatusTypeDef LSM6DS0_G_Get_Axes(SensorAxes_t *angular_velocity);

























#endif
















