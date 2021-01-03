#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "stm32l5xx_hal.h"


#define MAX_GT_DEV_ID_BYTE_LEN 		8
#define MAX_GT_VER_STR_LEN 			16
#define MAX_GT_IMEI_LEN (16) 
#define MAX_GT_SEND_LEN 256

typedef struct 
{
    uint16_t nYear;
    uint8_t nMonth;
    uint8_t nDay;
    uint8_t nHour;
    uint8_t nMin;
    uint8_t nSec;
} GPSDATETIME;

typedef struct
{
	uint8_t sat_num[32];
	uint8_t sat_db[32];
}GPSGSV;

typedef struct 
{
	GPSDATETIME dt;
	uint8_t state;
	double latitude;
	uint8_t NS;
	double longitude;
	uint8_t EW;
	double speed;
	double angle;
	double magnetic_value;
	uint8_t magnetic_ind;
	uint8_t mode;	//A 自主 D 差分 E估算 N 数据无效

	uint8_t sat_uesd;	//可用卫星
	double altitude;	//海拔
	double hdop;	//精度

	uint8_t sat_view;	//可见卫星
	uint8_t type;	//定位类型1=未定位2=2D定位3=3D定位
	GPSGSV gsv;
}gps_info_struct;

typedef struct
{
	double lat;
	double lon;
}lat_lon_struct;

typedef enum
{
	AT_UP_RMC=0x01,
	AT_UP_GGA,
	AT_UP_LOGIN,
	AT_UP_VER,
	AT_UP_IMSI,
	AT_UP_BT_ADDR,
	AT_UP_GPS,
	AT_UP_ALARM,
	AT_UP_EBIKE,
	AT_UP_HB,
	AT_UP_CSQ,
	AT_UP_GIVEBACK,
}AT_UPLOAD_TYPE;


#pragma pack (1)	//强制字节对齐
typedef struct 
{
	uint16_t start;     	//起始位
	uint16_t crc;		//crc校验
	uint8_t pack_len;	//包长度	---- 内容长度
	uint8_t prot_type;	//协议类型
	uint16_t sn;	 		//包序列号
	uint8_t datetime[6];	//包时间
}pkg_head_struct;

typedef struct 
{	
	uint8_t pwr_low:1;//低电报警开关
	uint8_t pwr_off:1;//断电告警开关
	uint8_t vibr:1;   //震动告警开关
	uint8_t oil_pwr:1;//断油电告警开关
	uint8_t sos:1;	 //sos告警开关
	uint8_t temp:1;   //温度告警开关
	uint8_t speed:1;  //超速告警开关
	uint8_t fence:1;
}switch_struct;


/********* 0x01 登陆包****************/
typedef struct 
{
	uint8_t   dev_id[MAX_GT_DEV_ID_BYTE_LEN];	//设备id 15位IMEI转化成的8位字节码
	uint8_t   dev_type;					//设备类型
	uint32_t auth_code;					//认证码
}login_pkg_struct;

/********* 0x02 GPS 包****************/
typedef struct 
{	
	uint8_t lat_ind:1;	//0 南纬 1 北纬
	uint8_t long_ind:1;	//0  西经1东经
	uint8_t mode:2;   	//0 实时 1 差分 2 估算 3 无效 ；如果是实时、差分或者估算，说明gps已经成功定位。
}gps_property_struct;

typedef struct 
{
	uint8_t loc_type;       	//定位类型
	uint8_t reserv_satnum;	//前四位gps保留长度 + 后四位卫星数
	gps_property_struct property;		//gps数据属性
	uint32_t latitude;		//纬度
	uint32_t longitude;		//经度
	uint16_t speed;			//速度
	uint16_t course;			//航向

	uint8_t reserv_sigstren;	//前4bit基站数据保留长度 后4bit基站信号强度0-15
	uint16_t mcc;			//mcc
	uint8_t  mnc;			//mnc
	uint16_t lac_sid;		//lac
	uint16_t cellid_nid;		//cellid
	uint16_t bid;		    //暂保留
}gps_pkg_struct;

/********* 0x03 status 包****************/
typedef struct 
{	
	uint8_t oil_pwr_state:1;		//断油电状态 0 油电接通 1 断油断电
	uint8_t sos_state:1;			//sos 状态 0 无sos  1 有sos
	uint8_t volt_level:3;        //电压等级 0-6
	
	uint8_t temp;				//摄氏度温度，暂未实现
}status_pkg_struct;

/********* 0x04 hb 包****************/
//心跳包无内容
/********* 0x05 告警 包****************/
typedef struct 
{	
	uint8_t type;			//告警类型
	uint8_t value_len;		//值的长度类型
	uint8_t value[6];	//告警值
}alarm_pkg_struct;

typedef  struct
{
	//系统参数
	int8_t 	ver[MAX_GT_VER_STR_LEN];
	int8_t  dev_type;
	
	//防护报警
	int8_t vibr2_thr;	//车的震动报警
	uint8_t vibr_thr;	
	uint16_t speed_thr;

	int16_t time_zone; //正负24时区	
	switch_struct alarm_switch;		//报警开关
	int8_t defence; 					//0 撤防 1设防
} config_struct;

/********* 0x06 数据 包****************/
typedef struct 
{	
	uint8_t type;			//配置类型
	uint8_t value_len;		//配置数据类型 长度
	uint8_t value[16];	//配置数据值
}data_pkg_struct;

/****************************0x07获取控制器数据包**************************/
typedef struct
{
	uint8_t addr;		//0x1a 电动车控制器，0x1b充电站控制器，0x1c单片机蓝牙智控器
	uint8_t value_len;
	uint8_t value[64];
}ebike_pkg_struct;

#pragma pack ()//强制字节对齐

void push_interval_package_process(void);
void PopATcmd(void);
uint8_t parse_http_pkg(unsigned char *pBuf, int len);
#endif


