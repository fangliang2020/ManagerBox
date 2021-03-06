#include "main.h"

#define SERVERIP "AT+QIOPEN=\"TCP\",\"jiutongtang.net\",\"2883\"\r\n"
#define true  1
#define false 0	
ES600S_DEV Modem_Dev;
uint16_t connect_times;

unsigned short recv_read_start_index = 0;
unsigned short recv_read_end_index = 0;
char http_buf[255];
char G_Post_Buf[1024];
uint16_t Post_Len = 0;
static uint8_t url_statuc = 0;
static uint16_t rCount = 0;
static uint16_t rTime = 0;

unsigned short http_size=0;
char another_buf[255];

void parse_cops_cmd(char *buf,int len);
void parse_imei_cmd(char *buf,int len);
void parse_imsi_cmd(char *buf,int len);
void parse_qccid_cmd(char *buf,int len);
void parse_csq_cmd(char *buf,int len);
void parse_creg_cmd(char *buf,int len);

AT_STRUCT at_pack[]={
	{AT_ATE0,"ATE0\r\n","OK",300,NULL},
	{AT_GMM,"AT+GMM\r\n","OK",300,NULL},
	{AT_CPIN,"AT+CPIN?\r\n","OK",300,NULL},
	{AT_COPS,"AT+COPS=?\r\n","OK",300,parse_cops_cmd},  //mmc mnc码  ---+COPS:(1,"CHN-UNICOM","UNICOM","46001",2)
	{AT_CIMI,"AT+CIMI\r\n","OK",300,parse_imsi_cmd}, //IMSI 码  AT+CIMI----  460023210226023 
	{AT_GSN,"AT+GSN\r\n","OK",300,parse_imei_cmd}, //IMEI 码  AT+GSN ----  
	{AT_QCCID,"AT+QCCID\r\n","OK",300,parse_qccid_cmd}, //SIM ICCID AT+QCCID---+QCCID: 89860025128306012474
	{AT_CSQ,"AT+CSQ\r\n","OK",300,parse_csq_cmd},//RSSI 信号质量 AT+CSQ  +CSQ: <rssi>,<ber>  +CSQ: (0-31,99),(0-7,99)
	{AT_CREG,"AT+CREG=2\r\n","+CREG: 1",300,parse_creg_cmd},  //+CREG: 1,“D509”,”80D413D”,7    设备基站 LAC 位置区域码   设备基站 CID 基站编号，是个16位的数据（范围是0到65535）
	{AT_CONTEXTID,"AT+QHTTPCFG=\"contextid\",1\r\n","OK",500,NULL},//配置PDP上下文ID为1；
	{AT_RESPONSE,"AT+QHTTPCFG=\"responseheader\",1\r\n","OK",500,NULL}, //启用输出HTTP响应头信息
	{AT_QIACTSET,"AT+QIACT=1\r\n","OK",500,NULL},
	{AT_QIACTQA,"AT+QIACT?\r\n","+QIACT:1,1,1",500,NULL},
	{AT_MAX,"","",0,NULL}
};



void	pure_gsmuart_buf(void)
{
	//Logln(D_INFO, "pure_gsmuart_buf");	
	memset(module_recv_buffer, 0, 255);
	module_recv_write_index = 0;
}

void MODEM_PWRON(void)
{
	HAL_GPIO_WritePin(GPIOA, MODEM_RST_Pin,GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(MODEM_PWR_CTRL_GPIO_Port, MODEM_PWR_CTRL_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, MODEM_PWR_ON_OFF_Pin,GPIO_PIN_RESET);
	HAL_Delay(5000);
	HAL_GPIO_WritePin(GPIOC, MODEM_PWR_ON_OFF_Pin,GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(MODEM_PWR_CTRL_GPIO_Port, MODEM_PWR_CTRL_Pin,GPIO_PIN_SET);
	HAL_Delay(11000);	
}

void MODEM_PWRKEY(void)
{
//	HAL_GPIO_WritePin(GPIOA, MODEM_RST_Pin,GPIO_PIN_SET);
//	HAL_GPIO_WritePin(MODEM_PWR_CTRL_GPIO_Port, MODEM_PWR_CTRL_Pin,GPIO_PIN_RESET);
//	HAL_Delay(1500);
//	HAL_GPIO_WritePin(MODEM_PWR_CTRL_GPIO_Port, MODEM_PWR_CTRL_Pin,GPIO_PIN_SET);	
//	HAL_Delay(1500);
//	HAL_GPIO_WritePin(GPIOA, MODEM_RST_Pin,GPIO_PIN_RESET);	
//	HAL_GPIO_WritePin(MODEM_PWR_CTRL_GPIO_Port, MODEM_PWR_CTRL_Pin,GPIO_PIN_RESET);
}
void MODEM_RST(void)
{		
	HAL_Delay(100);
}

bool get_work_state(void)
{
	if(Modem_Dev.status==EN_CONNECTED_STATE)
			return true;
	else
			return false;
}

void module_init(void)
{
	//Logln(D_INFO, "IOT_module Start Init");
	Send_AT_Command_Timeout(AT_ATE0, 1); 
	Send_AT_Command_Timeout(AT_GMM, 1);
	Send_AT_Command_Timeout(AT_CPIN, 1);
	Send_AT_Command_Timeout(AT_COPS, 1);
	Send_AT_Command_Timeout(AT_CIMI, 1);
	Send_AT_Command_Timeout(AT_GSN, 1);
	Send_AT_Command_Timeout(AT_QCCID, 1); 
	Send_AT_Command_Timeout(AT_CSQ, 1); 
	//Logln(D_INFO,"Init Complete");	
}

void at_process(void)
{
	if(Modem_Dev.status==EN_POWER_ON)
	{
		MODEM_PWRON();
		MODEM_PWRKEY();	
		Modem_Dev.status=EN_INIT_STATE;
	}
	else if(Modem_Dev.status==EN_INIT_STATE)  
	{
//		MODEM_RST();
		module_init();
		Modem_Dev.status=EN_LOGING_STATE;
	}
	else if(Modem_Dev.status==EN_LOGING_STATE) //查看注册信息
	{
		connect_times++;
		Send_AT_Command_Timeout(AT_CREG, 10); 	
		if(connect_times > 10)
		{
			//Logln(D_INFO, "reconnect %d times, restart ME",connect_times/40);
			Modem_Dev.status=EN_INIT_STATE;
		}
	}
	else if(Modem_Dev.status==EN_CONNECT_STATE)  //HTTP连接
	{
		
	}
	else if(Modem_Dev.status==EN_CONNECTED_STATE)
	{
		connect_times = 0;
		push_interval_package_process();
	}
	 PopATcmd();
	 at_parse_recv();  //解析 GSM得到的数据
	 HAL_Delay(50);
}


void hex_convert_str(uint8_t *in,uint8_t len, uint8_t *out)
{
	uint16_t i;
	uint8_t high,low;

	for(i=0; i<len; i++)
	{
		high = *(in+i)/16;
		low = *(in+i)%16;
		
		if(high>=0 && high<=9)
			*(out+2*i) = high+48;
		else if(high >=10 && high<=15)
			*(out+2*i) = high+55;

		if(low>=0 && low<=9)
			*(out+2*i+1) = low+48;
		else if(low >=10 && low<=15)
			*(out+2*i+1) = low+55;
	}
}
int htoi(char s[])  
{  
    int i;  
    int n = 0;  
    if (s[0] == '0' && (s[1]=='x' || s[1]=='X'))  
    {  
        i = 2;  
    }  
    else  
    {  
        i = 0;  
    }  
    for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i)  
    {  
        if (tolower(s[i]) > '9')  
        {  
            n = 16 * n + (10 + tolower(s[i]) - 'a');  
        }  
        else  
        {  
            n = 16 * n + (tolower(s[i]) - '0');  
        }  
    }  
    return n;  
}
void str_convert_hex(char* in, int len,unsigned char* out)
{
	int i;
	char tmp[3];

	for(i=0; i<len/2; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, in+2*i, 2);
		*(out+i) = (int)htoi(tmp);
	}
}
/*****************************************************************************
 * FUNCTION
 *	hex_str_2_bytes
 * DESCRIPTION
 *	将16进制的串转换成字节数组
 * PARAMETERS
 *	void
 * RETURNS
 *	void
 *****************************************************************************/
uint32_t hex_str_2_bytes(char* str, uint32_t str_len, uint8_t* bytes, uint32_t bytes_len)
{
	uint32_t i;
	uint8_t hex = 0;
	uint8_t offset = 0;

	if (str == NULL || bytes == NULL)
		return false;
	
	if((str_len +1) /2 > bytes_len)
	{
		return false;
	}
	
	if (str_len%2 != 0)
	{
		offset = 1;
	}

	for(i =0; i < str_len; i++)
	{
		//判断hex 字符串的有效性
		if(str[i] >= '0' && str[i] <= '9')
		{
			hex = str[i] - '0';
		}
		else if( str[i] >= 'a' && str[i] <= 'z')
		{
			hex = str[i] - 'a';
		}
		else if(str[i] >= 'A' && str[i] <= 'Z')
		{
			hex = str[i] - 'A';
		}
		else
		{
			return false;
		}

		bytes[(i + offset)/2] += hex << 4*((i + offset + 1)%2);
	}

	return true;
}


int GetComma(int num,char *str)
{
	int i,j=0;
	int len = strlen(str);
	for(i=0;i<len;i++)
	{
	    if(str[i]==',')
	    {
	         j++;
	    }

	    if(j==num)
	        return i+1;
	}
	return 0;
}
float get_double_number(char *s)
{
	char buf[16];
	int i;
	float rev;
	i=GetComma(1,s);
	memset(buf,0,16);
	strncpy(buf,s,i);
	buf[i-1]='\0';
	rev=atof(buf);
	return rev;
}

int8_t GetATIndex(AT_CMD cmd)  //得到AT_CMD的索引
{
	int8_t i=0;
	while(cmd!=at_pack[i].cmd)
	{
		if(at_pack[i].cmd==AT_MAX)
		{
			i=-1;
			break;
		}
		i++;
	}
	return i;
}
int get_uart_data(char*buf, int count)
{
	uint16_t ulen = module_recv_write_index;

	if (ulen > 0)
	{
		if(count >= ulen)
		{
			memcpy(buf, module_recv_buffer, ulen);
//			memset(module_recv_buffer,0,MODULE_BUFFER_SIZE);
			buf[ulen] = 0;
			module_recv_write_index = 0;
			return ulen;
		}
		else
			return 0;
	}
	else
	{
		return 0;
	}	
}

uint8_t Send_AT_Command(AT_CMD cmd)
{
	int len;
	uint8_t end = 0x1a;	
	char buf[255]={0};	
	int8_t i=GetATIndex(cmd), ret=0;
	if(i==-1)
	{
		//Logln(D_INFO, "error cmd");
		return ret;
	}
	GsmSendData(at_pack[i].cmd_txt, strlen(at_pack[i].cmd_txt));
//	GsmSendData("\r\n", strlen("\r\n"));
	HAL_Delay(at_pack[i].timeout);	
	len = get_uart_data(buf, 255);
	//Logln(D_INFO, "rcv %d,%s", len,buf);
	if(strstr(buf,at_pack[i].cmd_ret))
	{
		if(at_pack[i].fun)
		{
			at_pack[i].fun(buf,len);
		}
		ret = 1;
	}
	return ret;
}
/******************发送数据函数并检测接收值 *****************/
bool Send_AT_Command_Timeout(AT_CMD cmd, uint8_t timeout)
{
	while(1)
	{
		if(Send_AT_Command(cmd))
		{
			return true;
		}
		else
		{
			timeout--;			
			if(timeout==0)
				return false;
		}
	}
}
/*************比较接收数据*********************/
bool cmpdata(char* data1, char* data2, unsigned short len)
{
	while(len--)
	{
		if(*(data1+len) != *(data2+len))
			return false;
	}

	return true;
}
/***********返回最后一个包的索引值**********/
unsigned short split_diff_type(char* buf, unsigned short size)
{
	char  *head,*tail;
	char http[]={'+','Q','H','T','T','P'};  //MQTT接收数据
	char *find="OK\r\n";
	unsigned short i,http_index=0;
	unsigned short j=0,ret=0,last_http=0,last_end=0;

	memset(http_buf, 0, sizeof(http_buf));
	memset(another_buf, 0, sizeof(another_buf));
	
	for(i=0; i<size; i++)
	{
		if(*(buf+i) == '+')
		{
			head = buf+i;
			if(size-i>=sizeof(http) && cmpdata(head, http, sizeof(http)))  //比较索引值
			{
				 tail=strstr(head,find);
				 if(tail)
				 {
					 memcpy(http_buf+http_index, head, tail+strlen(find)-head);  //从'+' 到 'OK'末尾
					 http_index += tail+strlen(find)-head;
					 last_http = tail+strlen(find)-head+i;
					 i = last_http-1;
				 }
					memcpy(http_buf,buf,size);  //判断是http
			}
			else
			{
				another_buf[j] = *(buf+i);   //其他数据
				j++;
				if(size-i>=1 && *(buf+i)=='\r'&& *(buf+i+1)=='\n')
				last_end = i+2;
			}
		}
		
	}
	ret = ret>last_end?ret:last_end;
	return size-ret;
}
void parse_package_type(void)  //解析数据包
{
		
	unsigned short size=0,offset=0;
	char tmp[255];
	char *buf=module_recv_buffer;
	unsigned short write_index = module_recv_write_index;	//????????,??????????

	memset(tmp, 0, 255);
	recv_read_start_index = recv_read_end_index;  //函数重新刷新后，头索引又会和上一帧的尾索引相等
	recv_read_end_index = write_index;	
	if(recv_read_end_index > recv_read_start_index)
	{
		size = recv_read_end_index-recv_read_start_index;
		memcpy(tmp, buf+recv_read_start_index, size);
	}
	else if(recv_read_end_index < recv_read_start_index)
	{
		size = BUFFER_SIZE-recv_read_start_index;
		memcpy(tmp, buf+recv_read_start_index, size);
		memcpy(tmp+size, buf, recv_read_end_index);
		size += recv_read_end_index;
	}
		offset = split_diff_type(tmp, size);
	if(write_index>=offset)
		recv_read_end_index = write_index-offset;
	else
		recv_read_end_index = BUFFER_SIZE+write_index-offset;
}

void parse_another_cmd(char* buf, int len)
{
	char* tmp1=NULL,*tmp2 = NULL;

	//Logln(D_INFO,"parse_another_cmd,%s", buf);	

	if(tmp1 = strstr(buf,"CMS ERROR:"))
	{
		char data[6]={0};
		int err;
		tmp2 = strstr(tmp1,"\r\n");
		memcpy(data,tmp1+strlen("CMS ERROR:"),tmp2-(tmp1+strlen("CMS ERROR:")));
		err = atoi(data);
		//Logln(D_INFO,"ERROR code = %d",err);
		Modem_Dev.status=EN_INIT_STATE;
	}
	else if(tmp1 = strstr(buf,"CME ERROR:"))
	{
		char data[6]={0};
		int err;
		tmp2 = strstr(tmp1,"\r\n");
		memcpy(data,tmp1+strlen("CME ERROR:"),tmp2-(tmp1+strlen("CME ERROR:")));
		err = atoi(data);
		//Logln(D_INFO,"CME ERROR code = %d",err);
		Modem_Dev.status=EN_INIT_STATE;
	}
	else if(strstr(buf,"CONNECT OK"))
	{
		if(Modem_Dev.status!=EN_CONNECTED_STATE)
		{

		}
	} 
	else if(strstr(buf,"ALREADY CONNECT"))
	{
		Modem_Dev.status=EN_CONNECTED_STATE;
	}
	else if(strstr(buf,"RDY"))
	{
		module_init();	
	}
	else if(strstr(buf,"RING"))
	{
	}
	else if(strstr(buf,"PDP DEACT") /*|| strstr(buf,"CLOSED")*/)
	{//NETWORD disconnect
		//Logln(D_INFO,"CLOSED ---%s",buf);
		Modem_Dev.status=EN_CONNECT_STATE;
	}
}


bool at_parse_recv(void)
{
	parse_package_type();
	if(strlen(http_buf)>0)
	{
		parse_http_pkg(http_buf, strlen(http_buf));
		memset(http_buf,0,sizeof(http_buf));
	}
	if(strlen(another_buf)>0)
	{
		parse_another_cmd(another_buf, strlen(another_buf));
		memset(another_buf,0,sizeof(another_buf));
	}
	return true;	
}
void parse_cops_cmd(char *buf,int len) //+COPS:(1,"CHN-UNICOM","UNICOM","46001",2)
{
	char *tmp1=NULL,*tmp2=NULL;
	char *tmp=NULL;
	int i=0,j=0;
	if(strstr(buf,"+COPS"))
	{
		tmp1=buf;		
		i=GetComma(3,buf);
		j=GetComma(4,buf);
		memcpy(tmp,tmp1+i+1,j-i-2);
		memcpy(Modem_Dev.mcc,tmp,3);
		memcpy(Modem_Dev.mnc,tmp+3,2);
		//Logln(D_INFO,"mmc=%s,mnc=%s",Modem_Dev.mmc,Modem_Dev.mnc);
	}
	
}
void parse_imei_cmd(char *buf,int len)
{
	char* tmp1=NULL,*tmp2=NULL;
	memset(Modem_Dev.imei,0,sizeof(Modem_Dev.imei));
	tmp1 = strstr(buf,"\r\n");	
	tmp2 = strstr(tmp1+2,"\r\n");
	memcpy(Modem_Dev.imei,tmp1+strlen("\r\n"),tmp2-(tmp1+strlen("\r\n")));
	//Logln(D_INFO,"imei=%s",Modem_Dev.imei);
}
void parse_imsi_cmd(char *buf,int len)
{
	char* tmp1=NULL,*tmp2=NULL;
	memset(Modem_Dev.imsi,0,sizeof(Modem_Dev.imsi));
	tmp1 = strstr(buf,"\r\n");	
	tmp2 = strstr(tmp1+2,"\r\n");
	memcpy(Modem_Dev.imsi,tmp1+strlen("\r\n"),tmp2-(tmp1+strlen("\r\n")));
	//Logln(D_INFO,"imsi=%s",Modem_Dev.imsi);
}
void parse_qccid_cmd(char *buf,int len)
{
	char* tmp1=NULL,*tmp2=NULL;
	memset(Modem_Dev.iccid,0,sizeof(Modem_Dev.iccid));
	tmp1 = strstr(buf,"+QCCID: ");	
	tmp2 = strstr(tmp1,"\r\n");
	memcpy(Modem_Dev.iccid,tmp1+strlen("+QCCID: "),tmp2-(tmp1+strlen("+QCCID: ")));
	//Logln(D_INFO,"iccid=%s",Modem_Dev.iccid);	
}
void parse_csq_cmd(char *buf,int len)
{
	char* tmp1 = NULL,*tmp2= NULL;
	char tmp[3]={0};
	memset(Modem_Dev.imei,0,sizeof(Modem_Dev.imei));
	tmp1 = strstr(buf,"CSQ: ");	
	tmp2 = strstr(tmp1,",");
	memcpy(Modem_Dev.rssi,tmp1+strlen("CSQ: "),tmp2-(tmp1+strlen("CSQ: ")));
	//Logln(D_INFO,"csq=%s",Modem_Dev.rssi);	
}
void parse_creg_cmd(char *buf,int len)
{
	char *tmp1=NULL,*tmp2=NULL;
	char *tmp=NULL;
	int i=0,j=0,k=0;
	char regist_flag;
	if(tmp2=strstr(buf,"+CREG: "))
	{
		tmp1=buf;		
		i=GetComma(1,buf);
		j=GetComma(2,buf);
		k=GetComma(3,buf);
		memcpy(Modem_Dev.lac,tmp1+i+1,j-i-2);
		memcpy(Modem_Dev.cid,tmp1+k+1,k-j-2);
		//Logln(D_INFO,"lac=%s,cid=%s",Modem_Dev.lac,Modem_Dev.cid);
		memcpy(&regist_flag,tmp2+strlen("+CREG: "),1);
		if(regist_flag=='1')
		{
			Modem_Dev.status=EN_CONNECT_STATE;
		}
	}
}

void Json_Pack(char *pBuf)
{	
		cJSON *root;
		cJSON *Array;
	  cJSON *objId;
		char *pStr;
	  root=cJSON_CreateObject();
	  Array=cJSON_CreateArray();
		objId=cJSON_CreateObject();
	  cJSON_AddStringToObject(root,"appId",g_Info.appid); //根目录
	
		cJSON_AddStringToObject(objId,"pubIp",g_Info.ES600S_data->devip);
		cJSON_AddStringToObject(objId,"bootVersion",g_Info.bootVersion);
		cJSON_AddStringToObject(objId,"hwVersion",g_Info.hwVersion);
		cJSON_AddStringToObject(objId,"swVersion",g_Info.swVersion);
	  cJSON_AddStringToObject(objId,"imsi",g_Info.ES600S_data->imsi);
		cJSON_AddStringToObject(objId,"imei",g_Info.ES600S_data->imei);
		cJSON_AddStringToObject(objId,"iccid",g_Info.ES600S_data->iccid);
	  cJSON_AddStringToObject(objId,"rssi",g_Info.ES600S_data->rssi);
	  cJSON_AddStringToObject(objId,"mcc",g_Info.ES600S_data->mcc);
		cJSON_AddStringToObject(objId,"mnc",g_Info.ES600S_data->mnc);
		cJSON_AddStringToObject(objId,"lac",g_Info.ES600S_data->lac);
	  cJSON_AddStringToObject(objId,"cid",g_Info.ES600S_data->cid);
		cJSON_AddStringToObject(objId,"onlinetime",g_Info.onlinetime);
		cJSON_AddNumberToObject(objId,"coordinate",g_Info.coordinate);
		cJSON_AddNumberToObject(objId,"course",g_Info.course);
		cJSON_AddNumberToObject(objId,"flightTime",g_Info.flightTime);
		cJSON_AddNumberToObject(objId,"gs",g_Info.gs);
		cJSON_AddNumberToObject(objId,"posiAccuracy",g_Info.GPS_data->posiAccuracy);
		sprintf(g_Info.yali,"%.0f",g_Info.pressure);
    cJSON_AddStringToObject(objId,"pressure",g_Info.yali);
		cJSON_AddNumberToObject(objId,"latitude",g_Info.GPS_data->Latitude);
		cJSON_AddNumberToObject(objId,"longitude",g_Info.GPS_data->longitude);
		cJSON_AddStringToObject(objId,"orderId",g_Info.orderId);
		cJSON_AddStringToObject(objId,"rtTime",g_Info.rtTime);
		cJSON_AddItemToArray(Array, objId);
		cJSON_AddItemToObject(root, "bizContent", Array);
		cJSON_AddStringToObject(root,"charset","utf-8");
		cJSON_AddStringToObject(root,"format","json");
		cJSON_AddStringToObject(root,"sign","10095C58E762D8B32933F3FA3F7C7250");
		cJSON_AddStringToObject(root,"signType","md5");
		cJSON_AddStringToObject(root,"timestamp","20201221144800");
		pStr = cJSON_Print(root);
		strcpy(pBuf, pStr);
}
	
u8 Set_Post_Buf(char *sendstr,uint16_t len)
{
	memcpy(G_Post_Buf,sendstr,len);
	return 0;
}

u8 Clr_Post_Buf(void)
{
	Post_Len=0;
	return 0;
}

u8 Http_Set_Url(char *url,uint16_t len)
{
		char buf[50];
	if(Modem_Dev.status!=EN_CONNECT_STATE)
	{
		url_statuc=0;
	}
	else if(Modem_Dev.status==EN_CONNECTED_STATE)
	{
		switch(url_statuc)
		{
			case 0:
					module_recv_write_index=0;
					GsmSendData("AT+QHTTPCFG=\"contextid\",1\r\n", strlen("AT+QHTTPCFG=\"contextid\",1\r\n"));
					url_statuc=1;
					rCount++;
					rTime=20;
					break;
			case 1:
					rTime--;
					if(strstr((const char*)module_recv_buffer,(const char*)"OK\r\n"))
					{
						url_statuc=2;
						rCount=0;
					}
					else if(rTime==0)
					{
						url_statuc=0;
					}
					break;
			case 2:
					module_recv_write_index=0;
					GsmSendData("AT+QHTTPCFG=\"responseheader\",1\r\n", strlen("AT+QHTTPCFG=\"responseheader\",1\r\n"));
					url_statuc=3;
					rCount++;
					rTime=20;
					break;
			case 3:
					rTime --;
					if(strstr((const char*)module_recv_buffer,(const char*)"OK\r\n")){
						url_statuc = 4;
						rCount = 0;
					}
					else if(rTime == 0)
					{
						url_statuc = 2;
					}
					break;
			case 4:
					module_recv_write_index=0;
					sprintf(buf,"AT+QHTTPURL=%d,80\r\n",len);
					GsmSendData(buf,strlen(buf));
					url_statuc = 5;
					rCount ++;
					rTime = 20;
					break;
			case 5:
					rTime --;
					if(strstr((const char*)module_recv_buffer,(const char*)"CONNECT\r\n")){
						url_statuc = 6;
						rCount = 0;
					}
					else if(rTime == 0)
					{
						url_statuc = 4;
					}
					break;
			case 6:
					sprintf(buf,"%s\r\n",url);
					GsmSendData(buf,strlen(buf));
					url_statuc = 7;
					rCount ++;
					rTime = 20;
					break;
			case 7:
					if(strstr((const char*)module_recv_buffer,(const char*)"OK\r\n")){
					url_statuc = 8;
					rCount = 0;
					}
					else if(rTime == 0)
					{
						url_statuc = 6;
					}
					break;		
		}
	}
	if(rCount > 3)
	{
		rCount = 0;
		url_statuc = 0;
		Modem_Dev.status=EN_POWER_ON;
	}
	return 0;
}

char httpbuf[1024];
u8 Http_Post_Data(void)
{
	static u8 post_sta = 0;
	uint16_t len = Post_Len;
	char  buf[50];
	if(post_sta == 0){
		memcpy(httpbuf,G_Post_Buf,len);
		httpbuf[len]='\0';
	}
	
	while(len>0 && url_statuc == 8){
		{
			switch(post_sta)
			{
				case 0:
					module_recv_write_index=0;
				  memset(buf,0,50);
				  sprintf(buf,"AT+QHTTPPOST=%d,80,80\r\n",len);
					GsmSendData(buf,strlen(buf));
					post_sta = 1;
					rCount++;
					rTime = 20;
					break;
				case 1:
					module_recv_write_index=0;
					rTime --;
					if(strstr((const char*)module_recv_buffer,(const char*)"CONNECT\r\n"))
					{
						post_sta = 2;
						rCount = 0;
					}
					else if(rTime == 0)
					{
						post_sta = 1;
					}					
					break;
				case 2:
					module_recv_write_index=0;
				  memset(buf,0,50);
					sprintf(buf,"AT+QHTTPPOST=%d,80,80\r\n",len);
					GsmSendData(buf,strlen(buf));
					rCount++;
					rTime = 20;
					post_sta = 3;
					break;
				case 3:
					rTime --;
					module_recv_write_index=0;
					if(strstr((const char*)module_recv_buffer,(const char*)"+QHTTPPOST: 0,200,"))
					{
						post_sta = 0;	
						Clr_Post_Buf();
					}
					else if(rTime == 0)
					{
						post_sta = 2;
					}	
					break;
			}
		}
		
		if(rCount > 3)
		{
			post_sta = 0;
			rCount = 0;
			url_statuc = 0;
			Modem_Dev.status=EN_POWER_ON;
			return 1;
		}
	}
	
	return 0;
}






