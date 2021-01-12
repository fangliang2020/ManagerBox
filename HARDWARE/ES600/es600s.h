#ifndef ES600S_H
#define ES600S_H
#include "stm32l5xx_hal.h"

typedef void (*parseFun)(void*,int);
typedef char bool;

typedef enum
{
	EN_POWER_ON=0,
	EN_INIT_STATE,
	EN_LOGING_STATE,
	EN_CONNECT_STATE,	
	EN_CONNECTED_STATE,
}NET_WORK_STATE;

typedef struct
{
	char devip[15];
	char imsi[20];
	char imei[20];
	char iccid[24];
	char mcc[5];
	char mnc[5];
	char rssi[4];
	char lac[16];
	char cid[10];
	char username[12];
	char password[3];
	char secretld[5];
	NET_WORK_STATE status;
	
}ES600S_DEV;
extern ES600S_DEV Modem_Dev;


typedef enum{
	AT_ATE0,
	AT_GMM,
	AT_CPIN,
	AT_COPS,   //mmc mnc码  ---+COPS:(1,"CHN-UNICOM","UNICOM","46001",2)
	AT_CIMI,  //IMSI 码  AT+CIMI
	AT_GSN,   //IMEI 码  AT+GSN
	AT_QCCID,  //SIM ICCID AT+QCCID
	AT_CSQ,   //RSSI 信号质量 AT+CSQ	
	AT_CREG, //+CREG: 1,“D509”,”80D413D”,7    设备基站 LAC 位置区域码   设备基站 CID 基站编号，是个16位的数据（范围是0到65535）
	

	AT_MAX,
}AT_CMD;

typedef struct
{
	AT_CMD cmd;
	char cmd_txt[128];
	char cmd_ret[20];
	int timeout;
	parseFun fun;
}AT_STRUCT;
extern AT_STRUCT at_pack[];

uint8_t Send_AT_Command(AT_CMD cmd);
bool Send_AT_Command_Timeout(AT_CMD cmd, uint8_t timeout);
void MODEM_PWRON(void);
void MODEM_PWRKEY(void);
void MODEM_RST(void);
void module_init(void);
void at_process(void);
bool at_parse_recv(void);
bool get_work_state(void);
void Json_Pack(char *pBuf);
#endif

