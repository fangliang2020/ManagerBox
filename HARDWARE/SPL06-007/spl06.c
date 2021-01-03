#include "main.h"

#define PRESSURE_REG 0X00
#define TEMP_REG 0X03
#define PRS_CFG 0x06
#define TMP_CFG 0x07
#define MEAS_CFG 0x08

#define SPL06_REST_VALUE 0x09
#define SPL06_REST_REG 0x0C

#define PRODUCT_ID 0X0D

#define uint32 unsigned int

static struct spl0607_t spl0607;
static struct spl0607_t *p_spl0607;

/**
  * @brief  写寄存器
  * @param  uint8_t hwadr     器件地址
						uint8_t regadr    寄存器地址
						uint8_t val       值
  * @retval 无
  */

void spl0607_write(uint8_t hwadr,uint8_t regadr,uint8_t val)
{
 HAL_I2C_Mem_Write(&hi2c1,hwadr, regadr,I2C_MEMADD_SIZE_8BIT,&val, 1,500);
}

/**
  * @brief  读寄存器
  * @param  uint8_t hwadr     器件地址
						uint8_t regadr    寄存器地址
  * @retval 无
  */
uint8 spl0607_read(uint8_t hwadr,uint8_t regadr)
{
	uint8 reg_data;
	HAL_I2C_Mem_Read(&hi2c1,hwadr, regadr,I2C_MEMADD_SIZE_8BIT,&reg_data,1,500);
	return reg_data;
}

/**
  * @brief  设置温度传感器的每秒采样次数以及过采样率
  * @param   uint8 u8OverSmpl  过采样率         Maximal = 128
             uint8 u8SmplRate  每秒采样次数(Hz) Maximal = 128
             uint8 iSensor     0: Pressure; 1: Temperature
  * @retval 无
  */
void spl0607_rateset(uint8 iSensor, uint8 u8SmplRate, uint8 u8OverSmpl)
{
    uint8 reg = 0;
    int32 i32kPkT = 0;
    switch(u8SmplRate)
    {
        case 2:
            reg |= (1<<5);
            break;
        case 4:
            reg |= (2<<5);
            break;
        case 8:
            reg |= (3<<5);
            break;
        case 16:
            reg |= (4<<5);
            break;
        case 32:
            reg |= (5<<5);
            break;
        case 64:
            reg |= (6<<5);
            break;
        case 128:
            reg |= (7<<5);
            break;
        case 1:
        default:
            break;
    }
    switch(u8OverSmpl)
    {
        case 2:
            reg |= 1;
            i32kPkT = 1572864;
            break;
        case 4:
            reg |= 2;
            i32kPkT = 3670016;
            break;
        case 8:
            reg |= 3;
            i32kPkT = 7864320;
            break;
        case 16:
            i32kPkT = 253952;
            reg |= 4;
            break;
        case 32:
            i32kPkT = 516096;
            reg |= 5;
            break;
        case 64:
            i32kPkT = 1040384;
            reg |= 6;
            break;
        case 128:
            i32kPkT = 2088960;
            reg |= 7;
            break;
        case 1:
        default:
            i32kPkT = 524288;
            break;
    }

    if(iSensor == 0)
    {
        p_spl0607->i32kP = i32kPkT;
        spl0607_write(HW_ADR, 0x06, reg);
        if(u8OverSmpl > 8)
        {
            reg = spl0607_read(HW_ADR, 0x09);
            spl0607_write(HW_ADR, 0x09, reg | 0x04);
        }
    }
    if(iSensor == 1)
    {
        p_spl0607->i32kT = i32kPkT;
        spl0607_write(HW_ADR, 0x07, reg|0x80);  //Using mems temperature
        if(u8OverSmpl > 8)
        {
            reg = spl0607_read(HW_ADR, 0x09);
            spl0607_write(HW_ADR, 0x09, reg | 0x08);
        }
    }
}
/**
  * @brief  获取校准参数
  * @param  
  * @retval 无
  */
void spl0607_get_calib_param(void)
{
    uint32 h;
    uint32 m;
    uint32 l;
    h =  spl0607_read(HW_ADR, 0x10);
    l  =  spl0607_read(HW_ADR, 0x11);
    p_spl0607->calib_param.c0 = (int16)h<<4 | l>>4;
    p_spl0607->calib_param.c0 = (p_spl0607->calib_param.c0&0x0800)?(0xF000|p_spl0607->calib_param.c0):p_spl0607->calib_param.c0;
    h =  spl0607_read(HW_ADR, 0x11);
    l  =  spl0607_read(HW_ADR, 0x12);
    p_spl0607->calib_param.c1 = (int16)(h&0x0F)<<8 | l;
    p_spl0607->calib_param.c1 = (p_spl0607->calib_param.c1&0x0800)?(0xF000|p_spl0607->calib_param.c1):p_spl0607->calib_param.c1;
    h =  spl0607_read(HW_ADR, 0x13);
    m =  spl0607_read(HW_ADR, 0x14);
    l =  spl0607_read(HW_ADR, 0x15);
    p_spl0607->calib_param.c00 = (int32)h<<12 | (int32)m<<4 | (int32)l>>4;
    p_spl0607->calib_param.c00 = (p_spl0607->calib_param.c00&0x080000)?(0xFFF00000|p_spl0607->calib_param.c00):p_spl0607->calib_param.c00;
    h =  spl0607_read(HW_ADR, 0x15);
    m =  spl0607_read(HW_ADR, 0x16);
    l =  spl0607_read(HW_ADR, 0x17);
    p_spl0607->calib_param.c10 = (int32)h<<16 | (int32)m<<8 | l;
    p_spl0607->calib_param.c10 = (p_spl0607->calib_param.c10&0x080000)?(0xFFF00000|p_spl0607->calib_param.c10):p_spl0607->calib_param.c10;
    h =  spl0607_read(HW_ADR, 0x18);
    l  =  spl0607_read(HW_ADR, 0x19);
    p_spl0607->calib_param.c01 = (int16)h<<8 | l;
    h =  spl0607_read(HW_ADR, 0x1A);
    l  =  spl0607_read(HW_ADR, 0x1B);
    p_spl0607->calib_param.c11 = (int16)h<<8 | l;
    h =  spl0607_read(HW_ADR, 0x1C);
    l  =  spl0607_read(HW_ADR, 0x1D);
    p_spl0607->calib_param.c20 = (int16)h<<8 | l;
    h =  spl0607_read(HW_ADR, 0x1E);
    l  =  spl0607_read(HW_ADR, 0x1F);
    p_spl0607->calib_param.c21 = (int16)h<<8 | l;
    h =  spl0607_read(HW_ADR, 0x20);
    l  =  spl0607_read(HW_ADR, 0x21);
    p_spl0607->calib_param.c30 = (int16)h<<8 | l;
}
/**
  * @brief  发起一次温度测量
  * @param  
  * @retval 无
  */
void spl0607_start_temperature(void)
{
    spl0607_write(HW_ADR, 0x08, 0x02);
}
/**
  * @brief  发起一次压力测量
  * @param  
  * @retval 无
  */
void spl0607_start_pressure(void)
{
    spl0607_write(HW_ADR, 0x08, 0x01);
}
/**
  * @brief  循环模式
  * @param  
  * @retval 无
  */
void spl0607_start_continuous(uint8 mode)
{
    spl0607_write(HW_ADR, 0x08, mode+4);
}

/**
  * @brief  获取温度的原始值，并转换成32bit整数
  * @param  
  * @retval 无
  */
void spl0607_get_raw_temp(void)
{
    uint8 h[3] = {0};
    
		h[0] = spl0607_read(HW_ADR, 0x03);
		h[1] = spl0607_read(HW_ADR, 0x04);
		h[2] = spl0607_read(HW_ADR, 0x05);

    p_spl0607->i32rawTemperature = (int32)h[0]<<16 | (int32)h[1]<<8 | (int32)h[2];
    p_spl0607->i32rawTemperature= (p_spl0607->i32rawTemperature&0x800000) ? (0xFF000000|p_spl0607->i32rawTemperature) : p_spl0607->i32rawTemperature;
}

/**
  * @brief  获取压力的原始值，并转换成32bit整数
  * @param  
  * @retval 无
  */
void spl0607_get_raw_pressure(void)
{
    uint8 h[3];
    
		h[0] = spl0607_read(HW_ADR, 0x00);
		h[1] = spl0607_read(HW_ADR, 0x01);
		h[2] = spl0607_read(HW_ADR, 0x02);
    
    p_spl0607->i32rawPressure = (int32)h[0]<<16 | (int32)h[1]<<8 | (int32)h[2];
    p_spl0607->i32rawPressure= (p_spl0607->i32rawPressure&0x800000) ? (0xFF000000|p_spl0607->i32rawPressure) : p_spl0607->i32rawPressure;
}
/**
  * @brief  获取温度的浮点数
  * @param  
  * @retval 无
  */
float spl0607_get_temperature(void)
{
    float fTCompensate;
    float fTsc;

    fTsc = p_spl0607->i32rawTemperature / (float)p_spl0607->i32kT;
    fTCompensate =  p_spl0607->calib_param.c0 * 0.5 + p_spl0607->calib_param.c1 * fTsc;
    return fTCompensate;
}
/**
  * @brief  获取校准后的压力值浮点数
  * @param  
  * @retval 无
  */
float spl0607_get_pressure(void)
{
    float fTsc, fPsc;
    float qua2, qua3;
    float fPCompensate;

    fTsc = p_spl0607->i32rawTemperature / (float)p_spl0607->i32kT;
    fPsc = p_spl0607->i32rawPressure / (float)p_spl0607->i32kP;
    qua2 = p_spl0607->calib_param.c10 + fPsc * (p_spl0607->calib_param.c20 + fPsc* p_spl0607->calib_param.c30);
    qua3 = fTsc * fPsc * (p_spl0607->calib_param.c11 + fPsc * p_spl0607->calib_param.c21);
		//qua3 = 0.9f *fTsc * fPsc * (p_spl0607->calib_param.c11 + fPsc * p_spl0607->calib_param.c21);
	
    fPCompensate = p_spl0607->calib_param.c00 + fPsc * qua2 + fTsc * p_spl0607->calib_param.c01 + qua3;
		//fPCompensate = p_spl0607->calib_param.c00 + fPsc * qua2 + 0.9f *fTsc  * p_spl0607->calib_param.c01 + qua3;
    return fPCompensate;
}
/**
  * @brief  初始化函数
  * @param  
  * @retval 无
  */
void spl0607_init(void)
{
	uint8 ID;
	p_spl0607=&spl0607;
	p_spl0607->i32rawPressure=0;
	p_spl0607->i32rawTemperature=0;
	p_spl0607->chip_id=0x34;
	ID=spl0607_read(HW_ADR,0x0D);
  spl0607_get_calib_param();	
  spl0607_rateset(PRESSURE_SENSOR,128, 32);    
  spl0607_rateset(TEMPERATURE_SENSOR,32, 8);
	spl0607_start_continuous(CONTINUOUS_P_AND_T);	
}

float temperature;
float pressure;
float user_spl0607_get(void)
{	
		spl0607_get_raw_temp();
		g_Info.temperature = spl0607_get_temperature();	
		spl0607_get_raw_pressure();
		g_Info.pressure = spl0607_get_pressure();	
		return 0;
}	
