

/* Includes ------------------------------------------------------------------*/
#include "main.h"


uint8_t who_am_i=0;
LSM6DSO_ACC_GYRO_GDA_t value_G;
int AngularRate_mdps[3];

LSM6DSO_ACC_GYRO_XLDA_t value_XL;
int Acceleration_mG[3];
/* 6dsO Accelerometer test parameters */
static struct {
	uint8_t t_enable;
	LSM6DSO_ACC_GYRO_ODR_XL_t t_odr;  
	uint16_t  t_odr_hz_val;
	LSM6DSO_ACC_GYRO_FS_XL_t  t_fs;
	uint8_t t_decimation;
	uint8_t t_samples_num_in_pattern;
} lsm6dso_xl = {
					 0,
					 LSM6DSO_ACC_GYRO_ODR_XL_104Hz,
					 0,
					 LSM6DSO_ACC_GYRO_FS_XL_16g,
					 0,
					 0,
					 };


/* 6dsO Gyroscope test parameters */
static struct {
  u8_t				t_enable;
  LSM6DSO_ACC_GYRO_ODR_G_t	t_odr;
  u16_t				t_odr_hz_val;
  LSM6DSO_ACC_GYRO_FS_G_t	  t_fs;
  u8_t                          t_decimation;
  u8_t                          t_samples_num_in_pattern;
} lsm6dso_gyro = {
                  0,
                  LSM6DSO_ACC_GYRO_ODR_G_104Hz,
                  0,
                  LSM6DSO_ACC_GYRO_FS_G_2000dps,
                  0,
                  0,
                };

/*******LSM6DSO 初始化函数**********/		
int init_LSM6DSO_GYRO(void)
{
	/* Gyro ODR and full scale */
	status_t response;
	response=LSM6DSO_ACC_GYRO_W_ODR_G(0,lsm6dso_gyro.t_odr);  //写入采样率
	if(response==MEMS_ERROR) return -1;
	response=LSM6DSO_ACC_GYRO_W_FS_G(0, lsm6dso_gyro.t_fs);   //写入量程
	if(response==MEMS_ERROR) return -1;
	  /* Enable gyro */
  lsm6dso_gyro.t_enable = 1;
	return 0;
}	

int init_LSM6DSO_ACC(void)
{
	status_t response;
  /* Set ACC ODR  */
  response = LSM6DSO_ACC_GYRO_W_ODR_XL(0, lsm6dso_xl.t_odr);
  if(response==MEMS_ERROR) return -1; //manage here comunication error
  
  /* Set ACC full scale */
  response = LSM6DSO_ACC_GYRO_W_FS_XL(0, lsm6dso_xl.t_fs);
  if(response==MEMS_ERROR) return -1; //manage here comunication error

  /* BDU Enable */
  response = LSM6DSO_ACC_GYRO_W_BDU(0, LSM6DSO_ACC_GYRO_BDU_BLOCK_UPDATE);
  if(response==MEMS_ERROR) return -1; //manage here comunication error

  /* Enable Acc */
  lsm6dso_xl.t_enable = 1;
	return 0;
}
void LSM6DSO_Init(void)
{
	LSM6DSO_ACC_GYRO_R_WHO_AM_I(0,&who_am_i);
	if(who_am_i!=LSM6DSO_ACC_GYRO_WHO_AM_I)
	{
		g_Info.Imu.status=0;
	}
	else
	{
		g_Info.Imu.status=1;
		/* Soft Reset the LSM6DS3 device */
		LSM6DSO_ACC_GYRO_W_SW_RESET(0, LSM6DSO_ACC_GYRO_SW_RESET_RESET_DEVICE);	
		HAL_Delay(500);
	  /* configure device */
		init_LSM6DSO_GYRO();
		HAL_Delay(500);
		init_LSM6DSO_ACC();
		HAL_Delay(500);
		 LSM6DSO_ACC_GYRO_ReadReg(0,LSM6DSO_ACC_GYRO_CTRL6_G,&who_am_i, 1);
			HAL_Delay(500);
	}

}	

void   Loop_Sample_Aquisition(void)
{
	status_t response;
	 /*
	 * Read ACC output only if new ACC value is available
	 */
  response =  LSM6DSO_ACC_GYRO_R_XLDA(0, &value_XL); //加速度新值更新
	if(response==MEMS_SUCCESS) 
	{
		if (LSM6DSO_ACC_GYRO_XLDA_DATA_AVAIL==value_XL)  //0x01
		{
			LSM6DSO_ACC_Get_Acceleration(0, Acceleration_mG, 0);  //最后一个0不从FIFO中读取
		}
	}
	/* 
	 * Read GYRO output only if new gyro value is available
	 */
	response =  LSM6DSO_ACC_GYRO_R_GDA(0, &value_G);  //角速度新值更新
	if(response==MEMS_SUCCESS)
	{
		if (LSM6DSO_ACC_GYRO_GDA_DATA_AVAIL==value_G)
		{
			LSM6DSO_ACC_Get_AngularRate(0, AngularRate_mdps, 0);
		}	
	}
}
/*******************************************************************************
* Function Name		: LSM6DSO_ACC_GYRO_WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*					: I2C or SPI writing function
* Input				: Register Address, ptr to buffer to be written,
*                                 length of buffer
* Output			: None
* Return			: None
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_WriteReg(void *handle, u8_t Reg, u8_t *Bufp, u16_t len)
{
	HAL_I2C_Mem_Write(&hi2c4,LSM6DSO_ACC_GYRO_I2C_ADDRESS, Reg,I2C_MEMADD_SIZE_8BIT,Bufp,len,500);
//  I2Cx_Write(Bufp, LSM6DSO_ACC_GYRO_I2C_ADDRESS, Reg, len);    //[Example]
  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name		: LSM6DSO_ACC_GYRO_ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*					: I2C or SPI writing function
* Input				: Register Address, ptr to buffer to be read,
*                                 length of buffer
* Output			: None
* Return			: None
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_ReadReg(void *handle, u8_t Reg, u8_t *Bufp, u16_t len)
{
	HAL_I2C_Mem_Read(&hi2c4,LSM6DSO_ACC_GYRO_I2C_ADDRESS, Reg,I2C_MEMADD_SIZE_8BIT,Bufp,len,500);	
  //I2Cx_Read(Bufp, LSM6DSO_ACC_GYRO_I2C_ADDRESS, Reg, len); //[Example]
  return MEMS_SUCCESS; 
}

/**************** Base Function  *******************/

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_R_WHO_AM_I
* Description    : Read WHO_AM_I_BIT
* Input          : Pointer to u8_t
* Output         : Status of WHO_AM_I_BIT 
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_R_WHO_AM_I(void *handle, u8_t *value)
{
 if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_WHO_AM_I_REG, (u8_t *)value, 1) )
    return MEMS_ERROR;

  *value &= LSM6DSO_ACC_GYRO_WHO_AM_I_BIT_MASK; //coerce	
  *value = *value >> LSM6DSO_ACC_GYRO_WHO_AM_I_BIT_POSITION; //mask	

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_W_BDU
* Description    : Write BDU
* Input          : LSM6DSO_ACC_GYRO_BDU_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LSM6DSO_ACC_GYRO_W_BDU(void *handle, LSM6DSO_ACC_GYRO_BDU_t newValue)
{
  u8_t value;

  if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL3_C, &value, 1) )
    return MEMS_ERROR;

  value &= ~LSM6DSO_ACC_GYRO_BDU_MASK; 
  value |= newValue;
  
  if( !LSM6DSO_ACC_GYRO_WriteReg(handle, LSM6DSO_ACC_GYRO_CTRL3_C, &value, 1) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_R_BDU
* Description    : Read BDU
* Input          : Pointer to LSM6DSO_ACC_GYRO_BDU_t
* Output         : Status of BDU see LSM6DSO_ACC_GYRO_BDU_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_R_BDU(void *handle, LSM6DSO_ACC_GYRO_BDU_t *value)
{
 if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL3_C, (u8_t *)value, 1) )
    return MEMS_ERROR;

  *value &= LSM6DSO_ACC_GYRO_BDU_MASK; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_W_FS_XL
* Description    : Write FS_XL
* Input          : LSM6DSO_ACC_GYRO_FS_XL_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LSM6DSO_ACC_GYRO_W_FS_XL(void *handle, LSM6DSO_ACC_GYRO_FS_XL_t newValue)
{
  u8_t value;

  if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL1_XL, &value, 1) )
    return MEMS_ERROR;

  value &= ~LSM6DSO_ACC_GYRO_FS_XL_MASK; 
  value |= newValue;
  
  if( !LSM6DSO_ACC_GYRO_WriteReg(handle, LSM6DSO_ACC_GYRO_CTRL1_XL, &value, 1) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_R_FS_XL
* Description    : Read FS_XL
* Input          : Pointer to LSM6DSO_ACC_GYRO_FS_XL_t
* Output         : Status of FS_XL see LSM6DSO_ACC_GYRO_FS_XL_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_R_FS_XL(void *handle, LSM6DSO_ACC_GYRO_FS_XL_t *value)
{
 if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL1_XL, (u8_t *)value, 1) )
    return MEMS_ERROR;

  *value &= LSM6DSO_ACC_GYRO_FS_XL_MASK; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : status_t LSM6DSO_ACC_GYRO_GetRawAccData(u8_t *buff)
* Description    : Read GetAccData output register
* Input          : pointer to [u8_t]
* Output         : GetAccData buffer u8_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_GetRawAccData(void *handle, u8_t *buff) 
{
  u8_t i, j, k;
  u8_t numberOfByteForDimension;
  
  numberOfByteForDimension=6/3;

  k=0;
  for (i=0; i<3;i++ ) 
  {
	for (j=0; j<numberOfByteForDimension;j++ )
	{	
		if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_OUTX_L_XL+k, &buff[k], 1))
		  return MEMS_ERROR;
		k++;	
	}
  }

  return MEMS_SUCCESS; 
}

/*******************************************************************************
* Function Name  : status_t LSM6DSO_ACC_Get_Acceleration(void *handle, int *buff, u8_t from_fifo)
* Description    : Read GetAccData output register
* Input          : pointer to [u8_t]
* Output         : values are expressed in mg
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
/*
 * Following is the table of sensitivity values for each case.
 * Values are espressed in ug/digit.
 */
static const long long LSM6DSO_ACC_Sensitivity_List[4] = {
      61,	/* FS @2g */
      122,	/* FS @4g */
      244,	/* FS @8g */
      488,	/* FS @16g */
};
status_t LSM6DSO_ACC_Get_Acceleration(void *handle, int *buff, u8_t from_fifo)
{
  LSM6DSO_ACC_GYRO_FS_XL_t fs;
  long long sensitivity;
  Type3Axis16bit_U raw_data_tmp;

  /* Read out current odr, fs, hf setting */
  LSM6DSO_ACC_GYRO_R_FS_XL(handle, &fs);

  /* Determine the sensitivity according to fs */
  switch(fs) {
  case LSM6DSO_ACC_GYRO_FS_XL_2g:
    sensitivity = LSM6DSO_ACC_Sensitivity_List[0];
    break;

  case LSM6DSO_ACC_GYRO_FS_XL_4g:
    sensitivity = LSM6DSO_ACC_Sensitivity_List[1];
    break;

  case LSM6DSO_ACC_GYRO_FS_XL_8g:
    sensitivity = LSM6DSO_ACC_Sensitivity_List[2];
    break;

  case LSM6DSO_ACC_GYRO_FS_XL_16g:
    sensitivity = LSM6DSO_ACC_Sensitivity_List[3];
    break;
  }

  /* Read out raw accelerometer samples */
  if (from_fifo) {
    u8_t i;

    /* read all 3 axis from FIFO */
    for(i = 0; i < 3; i++)
      LSM6DSO_ACC_GYRO_Get_GetFIFOData(handle, raw_data_tmp.u8bit + 2*i);
  } else
    LSM6DSO_ACC_GYRO_GetRawAccData(handle, raw_data_tmp.u8bit);

  /* Apply proper shift and sensitivity */
  buff[0] = (raw_data_tmp.i16bit[0] * sensitivity + 500)/1000;
  buff[1] = (raw_data_tmp.i16bit[1] * sensitivity + 500)/1000;
  buff[2] = (raw_data_tmp.i16bit[2] * sensitivity + 500)/1000;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_W_ODR_XL
* Description    : Write ODR_XL
* Input          : LSM6DSO_ACC_GYRO_ODR_XL_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LSM6DSO_ACC_GYRO_W_ODR_XL(void *handle, LSM6DSO_ACC_GYRO_ODR_XL_t newValue)
{
  u8_t value;

  if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL1_XL, &value, 1) )
    return MEMS_ERROR;

  value &= ~LSM6DSO_ACC_GYRO_ODR_XL_MASK; 
  value |= newValue;
  
  if( !LSM6DSO_ACC_GYRO_WriteReg(handle, LSM6DSO_ACC_GYRO_CTRL1_XL, &value, 1) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_R_ODR_XL
* Description    : Read ODR_XL
* Input          : Pointer to LSM6DSO_ACC_GYRO_ODR_XL_t
* Output         : Status of ODR_XL see LSM6DSO_ACC_GYRO_ODR_XL_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_R_ODR_XL(void *handle, LSM6DSO_ACC_GYRO_ODR_XL_t *value)
{
 if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL1_XL, (u8_t *)value, 1) )
    return MEMS_ERROR;

  *value &= LSM6DSO_ACC_GYRO_ODR_XL_MASK; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_translate_ODR_XL
* Description    : Read ODR_XL
* Input          : LSM6DSL_ACC_GYRO_ODR_XL_t
* Output         : The ODR value in Hz
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_translate_ODR_XL(LSM6DSO_ACC_GYRO_ODR_XL_t value, u16_t *odr_hz_val)
{
  switch(value) {
  case LSM6DSO_ACC_GYRO_ODR_XL_POWER_DOWN:
    *odr_hz_val = 0;
    break;

  case LSM6DSO_ACC_GYRO_ODR_XL_13Hz:
    *odr_hz_val = 13;
    break;

  case LSM6DSO_ACC_GYRO_ODR_XL_26Hz:
    *odr_hz_val = 26;
    break;

  case LSM6DSO_ACC_GYRO_ODR_XL_52Hz:
    *odr_hz_val = 52;
    break;

  case LSM6DSO_ACC_GYRO_ODR_XL_104Hz:
    *odr_hz_val = 104;
    break;

  case LSM6DSO_ACC_GYRO_ODR_XL_208Hz:
    *odr_hz_val = 208;
    break;

  case LSM6DSO_ACC_GYRO_ODR_XL_416Hz:
    *odr_hz_val = 416;
    break;

  case LSM6DSO_ACC_GYRO_ODR_XL_833Hz:
    *odr_hz_val = 833;
    break;

  case LSM6DSO_ACC_GYRO_ODR_XL_1660Hz:
    *odr_hz_val = 1660;
    break;

  default:
    return MEMS_ERROR;
  }

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_W_FS_G
* Description    : Write FS_G
* Input          : LSM6DSO_ACC_GYRO_FS_G_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LSM6DSO_ACC_GYRO_W_FS_G(void *handle, LSM6DSO_ACC_GYRO_FS_G_t newValue)
{
  u8_t value;

  if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL2_G, &value, 1) )
    return MEMS_ERROR;

  value &= ~LSM6DSO_ACC_GYRO_FS_G_MASK; 
  value |= newValue;
  
  if( !LSM6DSO_ACC_GYRO_WriteReg(handle, LSM6DSO_ACC_GYRO_CTRL2_G, &value, 1) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_R_FS_G
* Description    : Read FS_G
* Input          : Pointer to LSM6DSO_ACC_GYRO_FS_G_t
* Output         : Status of FS_G see LSM6DSO_ACC_GYRO_FS_G_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_R_FS_G(void *handle, LSM6DSO_ACC_GYRO_FS_G_t *value)
{
 if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL2_G, (u8_t *)value, 1) )
    return MEMS_ERROR;

  *value &= LSM6DSO_ACC_GYRO_FS_G_MASK; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_W_ODR_G
* Description    : Write ODR_G
* Input          : LSM6DSO_ACC_GYRO_ODR_G_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LSM6DSO_ACC_GYRO_W_ODR_G(void *handle, LSM6DSO_ACC_GYRO_ODR_G_t newValue)
{
  u8_t value;

  if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL2_G, &value, 1) )
    return MEMS_ERROR;

  value &= ~LSM6DSO_ACC_GYRO_ODR_G_MASK; 
  value |= newValue;
  
  if( !LSM6DSO_ACC_GYRO_WriteReg(handle, LSM6DSO_ACC_GYRO_CTRL2_G, &value, 1) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_R_ODR_G
* Description    : Read ODR_G
* Input          : Pointer to LSM6DSO_ACC_GYRO_ODR_G_t
* Output         : Status of ODR_G see LSM6DSO_ACC_GYRO_ODR_G_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_R_ODR_G(void *handle, LSM6DSO_ACC_GYRO_ODR_G_t *value)
{
 if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL2_G, (u8_t *)value, 1) )
    return MEMS_ERROR;

  *value &= LSM6DSO_ACC_GYRO_ODR_G_MASK; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_translate_ODR_G
* Description    : Read ODR_G
* Input          : LSM6DSL_ACC_GYRO_ODR_G_t
* Output         : The ODR value in Hz
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_translate_ODR_G(LSM6DSO_ACC_GYRO_ODR_G_t value, u16_t *odr_hz_val)
{
  switch(value) {
  case LSM6DSO_ACC_GYRO_ODR_G_POWER_DOWN:
    *odr_hz_val = 0;
    break;

  case LSM6DSO_ACC_GYRO_ODR_G_13Hz:
    *odr_hz_val = 13;
    break;

  case LSM6DSO_ACC_GYRO_ODR_G_26Hz:
    *odr_hz_val = 26;
    break;

  case LSM6DSO_ACC_GYRO_ODR_G_52Hz:
    *odr_hz_val = 52;
    break;

  case LSM6DSO_ACC_GYRO_ODR_G_104Hz:
    *odr_hz_val = 104;
    break;

  case LSM6DSO_ACC_GYRO_ODR_G_208Hz:
    *odr_hz_val = 208;
    break;

  case LSM6DSO_ACC_GYRO_ODR_G_416Hz:
    *odr_hz_val = 416;
    break;

  case LSM6DSO_ACC_GYRO_ODR_G_833Hz:
    *odr_hz_val = 833;
    break;

  case LSM6DSO_ACC_GYRO_ODR_G_1660Hz:
    *odr_hz_val = 1660;
    break;

  default:
    return MEMS_ERROR;
  }

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : status_t LSM6DSO_ACC_GYRO_GetRawGyroData(u8_t *buff)
* Description    : Read GetGyroData output register
* Input          : pointer to [u8_t]
* Output         : GetGyroData buffer u8_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_GetRawGyroData(void *handle, u8_t *buff) 
{
  u8_t i, j, k;
  u8_t numberOfByteForDimension;
  
  numberOfByteForDimension=6/3;

  k=0;
  for (i=0; i<3;i++ ) 
  {
	for (j=0; j<numberOfByteForDimension;j++ )
	{	
		if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_OUTX_L_G+k, &buff[k], 1))
		  return MEMS_ERROR;
		k++;	
	}
  }

  return MEMS_SUCCESS; 
}

/*******************************************************************************
* Function Name  : status_t LSM6DSO_ACC_Get_AngularRate(u8_t *buff)
* Description    : Read GetGyroData output register
* Input          : pointer to [u8_t]
* Output         : Returned values are espressed in mdps
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
/*
 * Following is the table of sensitivity values for each case.
 * Values are espressed in udps/digit.
 */
static const long long LSM6DSO_GYRO_Sensitivity_List[5] = {
      4375,	/* FS @125 */
      8750,	/* FS @245 */
      17500,	/* FS @500 */
      35000,	/* FS @1000 */
      70000,	/* FS @2000 */
};
status_t LSM6DSO_ACC_Get_AngularRate(void *handle, int *buff, u8_t from_fifo)
{
  LSM6DSO_ACC_GYRO_FS_125_t fs_125;
  LSM6DSO_ACC_GYRO_FS_G_t fs;
  long long sensitivity;
  Type3Axis16bit_U raw_data_tmp;

  /* Read out current odr, fs, hf setting */
  LSM6DSO_ACC_GYRO_R_FS_125(handle, &fs_125);
  if (fs_125 == LSM6DSO_ACC_GYRO_FS_125_ENABLED) {
    sensitivity = LSM6DSO_GYRO_Sensitivity_List[0];
  } else {
    LSM6DSO_ACC_GYRO_R_FS_G(handle, &fs);

    /* Determine the sensitivity according to fs */
    switch(fs) {
    case LSM6DSO_ACC_GYRO_FS_G_245dps:
      sensitivity = LSM6DSO_GYRO_Sensitivity_List[1];
      break;

    case LSM6DSO_ACC_GYRO_FS_G_500dps:
      sensitivity = LSM6DSO_GYRO_Sensitivity_List[2];
      break;

    case LSM6DSO_ACC_GYRO_FS_G_1000dps:
      sensitivity = LSM6DSO_GYRO_Sensitivity_List[3];
      break;

    case LSM6DSO_ACC_GYRO_FS_G_2000dps:
      sensitivity = LSM6DSO_GYRO_Sensitivity_List[4];
      break;
    }
  }

  /* Read out raw accelerometer samples */
  if (from_fifo) {
    u8_t i;

    /* read all 3 axis from FIFO */
    for(i = 0; i < 3; i++)
      LSM6DSO_ACC_GYRO_Get_GetFIFOData(handle, raw_data_tmp.u8bit + 2*i);
  } else
    LSM6DSO_ACC_GYRO_GetRawGyroData(handle, raw_data_tmp.u8bit);

  /* Apply proper shift and sensitivity */
  buff[0] = (raw_data_tmp.i16bit[0] * sensitivity + 500)/1000;
  buff[1] = (raw_data_tmp.i16bit[1] * sensitivity + 500)/1000;
  buff[2] = (raw_data_tmp.i16bit[2] * sensitivity + 500)/1000;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_W_BW_XL
* Description    : Write BW_XL
* Input          : LSM6DSO_ACC_GYRO_BW_XL_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LSM6DSO_ACC_GYRO_W_BW_XL(void *handle, LSM6DSO_ACC_GYRO_BW_XL_t newValue)
{
  u8_t value;

  if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL1_XL, &value, 1) )
    return MEMS_ERROR;

  value &= ~LSM6DSO_ACC_GYRO_BW_XL_MASK; 
  value |= newValue;
  
  if( !LSM6DSO_ACC_GYRO_WriteReg(handle, LSM6DSO_ACC_GYRO_CTRL1_XL, &value, 1) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DSO_ACC_GYRO_R_BW_XL
* Description    : Read BW_XL
* Input          : Pointer to LSM6DSO_ACC_GYRO_BW_XL_t
* Output         : Status of BW_XL see LSM6DSO_ACC_GYRO_BW_XL_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_R_BW_XL(void *handle, LSM6DSO_ACC_GYRO_BW_XL_t *value)
{
 if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL1_XL, (u8_t *)value, 1) )
    return MEMS_ERROR;

  *value &= LSM6DSO_ACC_GYRO_BW_XL_MASK; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DS3_ACC_GYRO_R_XLDA
* Description    : Read XLDA
* Input          : Pointer to LSM6DS3_ACC_GYRO_XLDA_t
* Output         : Status of XLDA see LSM6DS3_ACC_GYRO_XLDA_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_R_XLDA(void *handle, LSM6DSO_ACC_GYRO_XLDA_t *value)
{
 if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_STATUS_REG, (u8_t *)value, 1) )
    return MEMS_ERROR;

  *value &= LSM6DSO_ACC_GYRO_XLDA_MASK; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM6DS3_ACC_GYRO_R_GDA
* Description    : Read GDA
* Input          : Pointer to LSM6DS3_ACC_GYRO_GDA_t
* Output         : Status of GDA see LSM6DS3_ACC_GYRO_GDA_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_R_GDA(void *handle, LSM6DSO_ACC_GYRO_GDA_t *value)
{
 if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_STATUS_REG, (u8_t *)value, 1) )
    return MEMS_ERROR;

  *value &= LSM6DSO_ACC_GYRO_GDA_MASK; //mask

  return MEMS_SUCCESS;
}
/*******************************************************************************
* Function Name  : LSM6DS3_ACC_GYRO_W_SW_RESET
* Description    : Write SW_RESET
* Input          : LSM6DS3_ACC_GYRO_SW_RESET_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LSM6DSO_ACC_GYRO_W_SW_RESET(void *handle, LSM6DSO_ACC_GYRO_SW_RESET_t newValue)
{
  u8_t value;

  if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL3_C, &value, 1) )
    return MEMS_ERROR;

  value &= ~LSM6DSO_ACC_GYRO_SW_RESET_MASK; 
  value |= newValue;
  
  if( !LSM6DSO_ACC_GYRO_WriteReg(handle, LSM6DSO_ACC_GYRO_CTRL3_C, &value, 1) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}
/*******************************************************************************
* Function Name  : LSM6DS3_ACC_GYRO_R_FS_125
* Description    : Read FS_125
* Input          : Pointer to LSM6DS3_ACC_GYRO_FS_125_t
* Output         : Status of FS_125 see LSM6DS3_ACC_GYRO_FS_125_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_R_FS_125(void *handle, LSM6DSO_ACC_GYRO_FS_125_t *value)
{
 if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_CTRL2_G, (u8_t *)value, 1) )
    return MEMS_ERROR;

  *value &= LSM6DSO_ACC_GYRO_FS_125_MASK; //mask

  return MEMS_SUCCESS;
}
/*******************************************************************************
* Function Name  : status_t LSM6DS3_ACC_GYRO_Get_GetFIFOData(u8_t *buff)
* Description    : Read GetFIFOData output register
* Input          : pointer to [u8_t]
* Output         : GetFIFOData buffer u8_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM6DSO_ACC_GYRO_Get_GetFIFOData(void *handle, u8_t *buff) 
{
  u8_t i, j, k;
  u8_t numberOfByteForDimension;
  
  numberOfByteForDimension=2/1;

  k=0;
  for (i=0; i<1;i++ ) 
  {
	for (j=0; j<numberOfByteForDimension;j++ )
	{	
		if( !LSM6DSO_ACC_GYRO_ReadReg(handle, LSM6DSO_ACC_GYRO_FIFO_DATA_OUT_L+k, &buff[k], 1))
		  return MEMS_ERROR;
		k++;	
	}
  }

  return MEMS_SUCCESS; 
}


