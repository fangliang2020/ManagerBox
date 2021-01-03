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
	uint8_t mode;	//A ���� D ��� E���� N ������Ч

	uint8_t sat_uesd;	//��������
	double altitude;	//����
	double hdop;	//����

	uint8_t sat_view;	//�ɼ�����
	uint8_t type;	//��λ����1=δ��λ2=2D��λ3=3D��λ
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


#pragma pack (1)	//ǿ���ֽڶ���
typedef struct 
{
	uint16_t start;     	//��ʼλ
	uint16_t crc;		//crcУ��
	uint8_t pack_len;	//������	---- ���ݳ���
	uint8_t prot_type;	//Э������
	uint16_t sn;	 		//�����к�
	uint8_t datetime[6];	//��ʱ��
}pkg_head_struct;

typedef struct 
{	
	uint8_t pwr_low:1;//�͵籨������
	uint8_t pwr_off:1;//�ϵ�澯����
	uint8_t vibr:1;   //�𶯸澯����
	uint8_t oil_pwr:1;//���͵�澯����
	uint8_t sos:1;	 //sos�澯����
	uint8_t temp:1;   //�¶ȸ澯����
	uint8_t speed:1;  //���ٸ澯����
	uint8_t fence:1;
}switch_struct;


/********* 0x01 ��½��****************/
typedef struct 
{
	uint8_t   dev_id[MAX_GT_DEV_ID_BYTE_LEN];	//�豸id 15λIMEIת���ɵ�8λ�ֽ���
	uint8_t   dev_type;					//�豸����
	uint32_t auth_code;					//��֤��
}login_pkg_struct;

/********* 0x02 GPS ��****************/
typedef struct 
{	
	uint8_t lat_ind:1;	//0 ��γ 1 ��γ
	uint8_t long_ind:1;	//0  ����1����
	uint8_t mode:2;   	//0 ʵʱ 1 ��� 2 ���� 3 ��Ч �������ʵʱ����ֻ��߹��㣬˵��gps�Ѿ��ɹ���λ��
}gps_property_struct;

typedef struct 
{
	uint8_t loc_type;       	//��λ����
	uint8_t reserv_satnum;	//ǰ��λgps�������� + ����λ������
	gps_property_struct property;		//gps��������
	uint32_t latitude;		//γ��
	uint32_t longitude;		//����
	uint16_t speed;			//�ٶ�
	uint16_t course;			//����

	uint8_t reserv_sigstren;	//ǰ4bit��վ���ݱ������� ��4bit��վ�ź�ǿ��0-15
	uint16_t mcc;			//mcc
	uint8_t  mnc;			//mnc
	uint16_t lac_sid;		//lac
	uint16_t cellid_nid;		//cellid
	uint16_t bid;		    //�ݱ���
}gps_pkg_struct;

/********* 0x03 status ��****************/
typedef struct 
{	
	uint8_t oil_pwr_state:1;		//���͵�״̬ 0 �͵��ͨ 1 ���Ͷϵ�
	uint8_t sos_state:1;			//sos ״̬ 0 ��sos  1 ��sos
	uint8_t volt_level:3;        //��ѹ�ȼ� 0-6
	
	uint8_t temp;				//���϶��¶ȣ���δʵ��
}status_pkg_struct;

/********* 0x04 hb ��****************/
//������������
/********* 0x05 �澯 ��****************/
typedef struct 
{	
	uint8_t type;			//�澯����
	uint8_t value_len;		//ֵ�ĳ�������
	uint8_t value[6];	//�澯ֵ
}alarm_pkg_struct;

typedef  struct
{
	//ϵͳ����
	int8_t 	ver[MAX_GT_VER_STR_LEN];
	int8_t  dev_type;
	
	//��������
	int8_t vibr2_thr;	//�����𶯱���
	uint8_t vibr_thr;	
	uint16_t speed_thr;

	int16_t time_zone; //����24ʱ��	
	switch_struct alarm_switch;		//��������
	int8_t defence; 					//0 ���� 1���
} config_struct;

/********* 0x06 ���� ��****************/
typedef struct 
{	
	uint8_t type;			//��������
	uint8_t value_len;		//������������ ����
	uint8_t value[16];	//��������ֵ
}data_pkg_struct;

/****************************0x07��ȡ���������ݰ�**************************/
typedef struct
{
	uint8_t addr;		//0x1a �綯����������0x1b���վ��������0x1c��Ƭ�������ǿ���
	uint8_t value_len;
	uint8_t value[64];
}ebike_pkg_struct;

#pragma pack ()//ǿ���ֽڶ���

void push_interval_package_process(void);
void PopATcmd(void);
uint8_t parse_http_pkg(unsigned char *pBuf, int len);
#endif


