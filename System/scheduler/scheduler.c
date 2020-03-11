#include "scheduler.h"

InstrucData instruc_data;
_SaveData Save_Data;
extern SqQueue struc_queue;
extern char isUnLock;
char battery;

extern char isTim2;
extern char isTim3;

u16 heartbeatInterval = 1000;
u16 stateInterval = 3;
u16 positionInterval = 2;

u16 getHeartbeatInterval(void)
{
	return heartbeatInterval;
}
u16 getStateInterval(void)
{
	return stateInterval;
}
u16 getPositionInterval(void)
{
	return positionInterval;
}

void clrInstruc(void)
{
	instruc_data.isParseData = false;
	instruc_data.isGetData = false;
	instruc_data.isNull = true;
	memset(instruc_data.instruc_Buffer, 0, instruc_Buffer_Length); //清空
	memset(instruc_data.messgaeId, 0, messgaeId_Length);      
	memset(instruc_data.mid, 0, mid_Length);
	memset(instruc_data.data, 0, data_Length);
	
}

void taskScheduler_init(void)
{
	isTim2 = 0;
	isTim3 = 0;
	isUnLock = 0;
	battery = 100;
	clrInstruc();
	InitQueue(&struc_queue);
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

int reportPosition(char* code)
{
	getGpsBuffer(positionNum);
	if (Save_Data.isUsefull)
	{
		char gpsInfo[50] = { 0 };
		char result[100] = { 0 };
		char data[100] = { 0 };
		
		sprintf(gpsInfo, "%s%s%s", Save_Data.longitude, Save_Data.latitude, Save_Data.UTCTime);
		charToHex(gpsInfo, result);
		
		if(strcmp(code, "0000")){
			char codeHex[20] = { 0 };
			charToHex(code, codeHex);
			sprintf(data, "%s%s%s", result, "00", codeHex);
		} else {
			sprintf(data, "%s%s%s", result, "01", code);
		}
		sendReport(position_report, data);
		clrStruct();
		return 1;
	}
	clrStruct();
	return 0;
}

int reportLockState(void)
{
	char data[3] = { 0 };
	sprintf(data, "%02x", getLockStatus());
	sendReport(lockState_report, data);
	return 0;
}

int reportDeviceState(void)
{
	char data[13] = { 0 };
	sprintf(data, "%02x%04x%04x", battery, stateInterval, positionInterval);
	sendReport(device_state_report, data);
	return 0;
}

int reportHeartbeat(void)
{
	sendReport(heartbeat_report, "01");
	return 0;
}

void runGetPosition(void)
{
	int r = reportPosition("0000");
	if(r){
		sendResponse(GET_POSITION_RES, instruc_data.mid, SUCCESS_CODE, (char*)SUCCESS_STATUS);
	} else {
		sendResponse(GET_POSITION_RES, instruc_data.mid, FAIL_CODE, (char*)FAIL_STATUS);
	}
	clrInstruc();
}

void runUnlock(void)
{
	int index = 10;
	char data[10] = { 0 };
	while(index > 0 && !isUnLock){
		delay_ms(1000);
		index--;
	}
	if(isUnLock){
		toggleLockStatus();
		char uid[5] = {0};
		char codeHex[20] = { 0 };
		rand4UUID(uid);
		charToHex(uid, codeHex);
		sprintf(data, "%s%s", SUCCESS_STATUS, codeHex);
		sendResponse(UN_LOCK_RES, instruc_data.mid, SUCCESS_CODE, data);
		reportPosition(codeHex);
	} else {
		sprintf(data, "%s%s", FAIL_STATUS, "0000");
		
		sendResponse(UN_LOCK_RES, instruc_data.mid, FAIL_CODE, data);
	}
	isUnLock = 0;
	LED_OFF;
	clrInstruc();
}

void runGetDeviceState(void)
{
	reportDeviceState();
	sendResponse(GET_DEVICE_STATE_RES, instruc_data.mid, SUCCESS_CODE, (char*)SUCCESS_STATUS);
	clrInstruc();
}

void runSetStateInterval(void)
{
	char stateTypeC[3] = {0};
	char intervalC[5] = {0};
	strncpy(stateTypeC, instruc_data.data, 2);
	strncpy(intervalC, instruc_data.data + 2, 4);
	int stateType = charhex_to_dec(stateTypeC); 
	int interval = charhex_to_dec(intervalC); 
	switch(stateType)
		{
			case 0:
				heartbeatInterval = interval;
				sendResponse(SET_STATE_INTERVAL_RES, instruc_data.mid, SUCCESS_CODE, (char*)SUCCESS_STATUS);
				break;
			case 1:
				stateInterval = interval;
				sendResponse(SET_STATE_INTERVAL_RES, instruc_data.mid, SUCCESS_CODE, (char*)SUCCESS_STATUS);
				break;
			case 2:
				positionInterval = interval;
				sendResponse(SET_STATE_INTERVAL_RES, instruc_data.mid, SUCCESS_CODE, (char*)SUCCESS_STATUS);
				break;

			default:
			sendResponse(SET_STATE_INTERVAL_RES, instruc_data.mid, FAIL_CODE, (char*)FAIL_STATUS);	
			break;
		}
	clrInstruc();
}

void runSleepingShutdown(void)
{
	sendResponse(SLEEPING_SHUTDOWN_RES, instruc_data.mid, SUCCESS_CODE, (char*)SUCCESS_STATUS);
	BC35_Shutdown();
	clrInstruc();
}

// 获取数据单元
void getInstructions(void)
{
	char ais[instruc_Buffer_Length];
	if(instruc_data.isNull){
		if (DeQueue(&struc_queue, ais)) {
			clrInstruc();
			clean_string(ais);
			memcpy(instruc_data.instruc_Buffer, ais + 7, instruc_Buffer_Length);
			instruc_data.isNull = false;
			instruc_data.isGetData = true;
		}
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
		int msgId = charhex_to_dec(instruc_data.messgaeId); 
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

void getTimeState(void){
	if(isTim2){
		reportPosition("0000");
		isTim2 = 0;
	}
	if(isTim3){
		reportDeviceState();
		isTim3 = 0;
	}
}


void taskScheduler(void)
{
	getInstructions();
	parseInstructions();
	executionInstructions();
	getTimeState();
}
