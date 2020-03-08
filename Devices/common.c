#include "common.h"

char code[5] = "0000";
InstrucData instruc_data;
extern SqQueue struc_queue;

void clrInstruc()
{
	instruc_data.isParseData = false;
	instruc_data.isGetData = false;
	memset(instruc_data.instruc_Buffer, 0, instruc_Buffer_Length); //清空
	memset(instruc_data.messgaeId, 0, messgaeId_Length);      
	memset(instruc_data.mid, 0, mid_Length);
	memset(instruc_data.data, 0, data_Length);
	
}

void taskScheduler_init(void)
{
	clrInstruc();
	InitQueue(&struc_queue);
}

void clean_string(char* str)
{
	char* start = str - 1;
	char* end = str;
	char* p = str;
	while (*p)
	{
		switch (*p)
		{
		case ' ':
		case '\r':
		case '\n':
		{
			if (start + 1 == p)
				start = p;
		}
		break;
		default:
			break;
		}
		++p;
	}
	//现在来到了字符串的尾部 反向向前
	--p;
	++start;
	if (*start == 0)
	{
		//已经到字符串的末尾了 
		*str = 0;
		return;
	}
	end = p + 1;
	while (p > start)
	{
		switch (*p)
		{
		case ' ':
		case '\r':
		case '\n':
		{
			if (end - 1 == p)
				end = p;
		}
		break;
		default:
			break;
		}
		--p;
	}
	memmove(str, start, end - start);
	*(str + (int)end - (int)start) = 0;
}

void charToHex(char* src, char* res)
{
	int i;
	for (i = 0; src[i] != '\0'; i++)
	{
		char tmpe[4];
		sprintf(tmpe, "%02x", src[i]);
		strcat(res, tmpe);
	}
}


void sendReport(int msgId, char* data)
{
	char res[100] = { 0 };
	sprintf(res, "%02x%s", msgId, data);
	BC35_COAPdata(res);
}

void sendResponse(int msgId, char* mid, int errCode, char* data)
{
	char res[50] = { 0 };
	sprintf(res, "%02x%s%02x%s", msgId, mid, errCode, data);
	BC35_COAPdata(res);
}

int reportPosition(void)
{
	_SaveData* sd;
	
	if ((sd = getGpsBuffer(positionNum)) != NULL)
	{
		char gpsInfo[50] = { 0 };
		char result[100] = { 0 };
		char data[100] = { 0 };
		char codeHex[20] = { 0 };
		
		sprintf(gpsInfo, "%s%s%s", sd->longitude, sd->latitude, sd->UTCTime);
		charToHex(gpsInfo, result);
		charToHex(code, codeHex);
		
		if(strcmp(code, "0000")){
			sprintf(data, "%s%s%s", result, "00", codeHex);
		} else {
			sprintf(data, "%s%s%s", result, "01", codeHex);
			strcpy(code, "0000");
		}
			sendReport(position_report, data);
		return 1;
	}
	
	return 0;
}

int reportLockState(void)
{
	return 0;
}

int reportDeviceState(void)
{
	return 0;
}

int reportHeartbeat(void)
{
	sendReport(heartbeat_report, "01");
	return 0;
}

void runGetPosition(void)
{
	int r = reportPosition();
	if(r){
		sendResponse(GET_POSITION_RES, instruc_data.mid, SUCCESS_CODE, (char*)SUCCESS_STATUS);
	} else {
		sendResponse(GET_POSITION_RES, instruc_data.mid, FAIL_CODE, (char*)FAIL_STATUS);
	}
	clrInstruc();
}

void runUnlock(void)
{
	BELL_TOGGLE;
	LED_TOGGLE;
	char codeHex[20] = { 0 };
	char data[10] = { 0 };
	charToHex(code, codeHex);
	sprintf(data, "%s%s", SUCCESS_STATUS, codeHex);
	
	sendResponse(UN_LOCK_RES, instruc_data.mid, SUCCESS_CODE, data);
}

void runGetDeviceState(void)
{
	
}

void runSetStateInterval(void)
{
	
}

void runSleepingShutdown(void)
{
	
}

// 获取数据单元
void getInstructions(void)
{
	char ais[instruc_Buffer_Length];
	if (DeQueue(&struc_queue, ais)) {
		clrInstruc();
		clean_string(ais);
		memcpy(instruc_data.instruc_Buffer, ais + 7, instruc_Buffer_Length);
		instruc_data.isGetData = true;
	}
}

// 解析单元
void parseInstructions(void)
{
	if(instruc_data.isGetData){
		instruc_data.isGetData = false;
		strncpy(instruc_data.messgaeId, instruc_data.instruc_Buffer + 1, 2);
		strncpy(instruc_data.mid, instruc_data.instruc_Buffer + 3, 4);
		strncpy(instruc_data.data, instruc_data.instruc_Buffer + 7, strlen(instruc_data.instruc_Buffer) - 7);
		instruc_data.isParseData = true;
	}
}

// 执行单元
void executionInstructions(void)
{
	if(instruc_data.isParseData){
		instruc_data.isParseData = false;
		int msgId = atoi(instruc_data.messgaeId); 
		switch(msgId)
		{
			case UN_LOCK_REQ:
				runUnlock();
				break;
			case GET_POSITION_REQ:
				runGetPosition();
				break;
			case GET_DEVICE_STATE_REQ:
				runGetDeviceState();
				break;
			case SET_STATE_INTERVAL_REQ:
				runSetStateInterval();
				break;
			case SLEEPING_SHUTDOWN_REQ:
				runSleepingShutdown();
				break;

			default:break;
		}
		
	}
}


void taskScheduler(void)
{
	getInstructions();
	parseInstructions();
	executionInstructions();
}



