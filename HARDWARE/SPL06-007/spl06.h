#ifndef SPL06_H
#define SPL06_H

#define s32 int32
#define int16 short
#define int32 int
#define uint8 unsigned char
//#define HW_ADR 0x77 //SDO HIGH OR NC
#define HW_ADR 0x76 //SDO LOW
#define CONTINUOUS_PRESSURE     1
#define CONTINUOUS_TEMPERATURE  2
#define CONTINUOUS_P_AND_T      3
#define PRESSURE_SENSOR     0
#define TEMPERATURE_SENSOR  1

struct spl0607_calib_param_t {	
    int16 c0;
    int16 c1;
    int32 c00;
    int32 c10;
    int16 c01;
    int16 c11;
    int16 c20;
    int16 c21;
    int16 c30;       
};
extern float temperature,temperature2,baro_Offset;
extern unsigned char baro_start;
struct spl0607_t {	
    struct spl0607_calib_param_t calib_param;/**<calibration data*/	
    uint8 chip_id; /**<chip id*/	
    int32 i32rawPressure;
    int32 i32rawTemperature;
    int32 i32kP;    
    int32 i32kT;
};

void spl0607_init(void);
float user_spl0607_get(void);
#endif


