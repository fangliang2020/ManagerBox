#include "main.h"


void push_interval_package_process(void)
{
	static uint8_t delay_index=0;
    
	if(!get_work_state())
		return;
  if(flag_delay1s)
	{
		flag_delay1s=0;
	  if ((delay_index)%15 == 0)	//15秒
		{
			EnterQue(&at_send_Queue, AT_UP_GPS);
		}
		else 	if((delay_index+1)%30==0)
		{
			EnterQue(&at_send_Queue, AT_UP_ALARM);
		} 
		else if((delay_index+2)%30==0)
		{
			EnterQue(&at_send_Queue, AT_UP_CSQ);
		}
		else if((delay_index+3)%60==0)
		{
			EnterQue(&at_send_Queue, AT_UP_HB);
		}
		else if((delay_index+4)%5==0)
		{
			EnterQue(&at_send_Queue, AT_UP_HB);
		}

				delay_index++;
		if(delay_index>59)
			delay_index = 0;
	}
}

void PopATcmd(void)
{
	int p_workQue;
	p_workQue=LeaveQue(&at_send_Queue); //读取序列
	if(p_workQue==-1)
	{

	}
	else
	{
//		switch(at_send_type.Data[0])
//		{
//			case AT_UP_RMC:
//				send_gps_rmc_cmd();
//				break;
//			case AT_UP_GGA:
//				send_gps_gga_cmd();
//				break;
//			case AT_UP_LOGIN:
//				upload_login_package();
//				break;
//			case AT_UP_VER:
//				upload_version_package();
//				break;
//			case AT_UP_IMSI:
//				upload_imsi_package();
//				break;
//			case AT_UP_BT_ADDR:
//				upload_bt_addr_package();
//				break;
//			case AT_UP_GPS:
//				upload_gps_package();
//				break;
//			case AT_UP_ALARM:
//				upload_alarm_package();
//				break;
//			case AT_UP_EBIKE:
//				upload_ebike_data_package();
//				break;
//			case AT_UP_HB:
//				upload_hb_package();
//				break;
//			case AT_UP_CSQ:
//				Send_AT_Command_ext(AT_CSQ, 1);
//				break;
//			case AT_UP_GIVEBACK:
//				upload_give_back_package(g_flash.acc);
//				break;
//			default:
//				break;
//		}
	}

}
/*
void calibration_time(uint8_t* buf)
{
	uint8_t* date_time;
	RTC_DateTypeDef sdatestructure;
	RTC_TimeTypeDef stimestructure;


	date_time = ((uint8_t*)buf + sizeof(pkg_head_struct) - 6);
	sdatestructure.Year = date_time[0];
	sdatestructure.Month = date_time[1];
	sdatestructure.Date = date_time[2];
	sdatestructure.WeekDay = 0x01;
	stimestructure.Hours = date_time[3];
	stimestructure.Minutes = date_time[4];
	stimestructure.Seconds = date_time[5];
	stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

	Logln(D_INFO, "cali %d-%d-%d %d:%d:%d",sdatestructure.Year,sdatestructure.Month,sdatestructure.Date,stimestructure.Hours,stimestructure.Minutes,stimestructure.Seconds);
  	HAL_RTC_SetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);//设置时分秒
	HAL_RTC_SetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN);//设置年月日

	HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN);
	Logln(D_INFO, " %d-%d-%d %d:%d:%d",sdatestructure.Year,sdatestructure.Month,sdatestructure.Date,stimestructure.Hours,stimestructure.Minutes,stimestructure.Seconds);

}
*/

bool protocol_proc(unsigned char* buf ,int len)
{	
//	pkg_head_struct* head;
////	char out[256]={0};
//	int crc1,crc2;

//	if (buf == NULL)
//		return true;

//	head = (pkg_head_struct*)buf;

//	crc2 = buf[2]*0x100+buf[3];
//	crc1 = get_crc16(buf+4, len-6);
////	hex_convert_str(buf,len, out);

//	if(crc1 != crc2)
//	{
//		Logln(D_INFO,"check sum error");
//		return false;
//	}

//	switch(head->prot_type)
//	{
//		case EN_GT_PT_LOGIN:						
//		{	
//			RxMsgTypeDef msgType;
//			
//			Logln(D_INFO,"login rsp sn ok");
//			calibration_time(buf);
//			msgType.Data[0] = AT_UP_VER;
//			PushElement(&at_send_Queue, msgType, 1);
//			msgType.Data[0] = AT_UP_IMSI;
//			PushElement(&at_send_Queue, msgType, 1);
//			msgType.Data[0] = AT_UP_BT_ADDR;
//			PushElement(&at_send_Queue, msgType, 1);
//			net_work_state = EN_CONNECTED_STATE;
//			break;																	
//		}		
//		case EN_GT_PT_GPS: 			
//		{	
//			Logln(D_INFO,"EN_GT_PT_GPS");
//			break;
//		}
//		case EN_GT_PT_STATUS: 			
//		{	
//			Logln(D_INFO,"EN_GT_PT_STATUS");
//			break;
//		}
//		case EN_GT_PT_HB: 
//		{	
//			g_hb_send_times = 0;
//			Logln(D_INFO,"HB rsp sn ok################");
//			break; 
//		}
//		case EN_GT_PT_ALARM: 					
//		{	
//			Logln(D_INFO,"EN_GT_PT_ALARM");
//			break;
//		}		
//		case EN_GT_PT_DEV_DATA:			
//		{	
//			data_pkg_struct* data;

//			Logln(D_INFO,"EN_GT_PT_DEV_DATA");
//			data = (data_pkg_struct*)((uint8_t*)head + sizeof(pkg_head_struct));	
//			parse_dev_data(data);
//			break;						
//		}			
//		case EN_GT_PT_CONTROL: 					
//		{	
//			ebike_cmd_struct* control_data;
//			Logln(D_INFO,"EN_GT_PT_CONTROL");
//			control_data = (ebike_cmd_struct*)((uint8_t*)head + sizeof(pkg_head_struct));
//			parse_network_cmd(control_data);
//			break;
//		}
//		case EN_GT_PT_LBS:
//		{
//			Logln(D_INFO,"EN_GT_PT_LBS");
//			break;
//		}
//		case EN_GT_PT_SRV_DATA:
//		{
//			Logln(D_INFO,"EN_GT_PT_SRV_DATA");
//			break;	
//		}		
//		default:
//			Logln(D_INFO,"EN_GT_PT_DEFAULT");
//			break;			
//	}					

//	return true;
}

uint8_t parse_http_pkg(unsigned char *pBuf, int len)
{
//	unsigned char req[512]={0};
//	int i;
//	char *head,*tail,head_first=1;
//	uint8_t ret=0;

//	if(len<256)
//	{
//		hex_convert_str(pBuf,len, req);
//		Logln(D_INFO,"len = %d,rec=%s",len,req);
//	}
//	
//	for(i = 0; i<len-1; i++)
//	{
//		//找包头 0xffff
//		if(pBuf[i]==0xff && pBuf[i+1]==0xff && head_first)
//		{
//			head = pBuf+i;
//			head_first = 0;
//		}
//		else if((pBuf[i]==0x0d && pBuf[i+1]==0x0a))
//		{
//			tail = pBuf+i+2;//结尾

//			//验证长度合法性
//			if(tail - head == head[4] + PACKET_FRAME_LEN)
//			{
//				memset(req, 0, sizeof(req));
//				memcpy(req, head, tail-head);
//				protocol_proc(req,tail-head);
//                		head_first = 1;
//				//找到合法结尾
//				head = tail;
//               			ret = 1;
//			}
//			//继续找结尾
//			i +=1;	//代码加1，for循环自加1 ，等效于向后偏移2个字节（0d0a）			
//		}
//	}
//    return ret;
}

