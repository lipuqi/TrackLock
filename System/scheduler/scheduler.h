#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "../../Devices/common.h"
#include "../utils/utils.h"

#define positionNum					5

//定义数组长度
#define instruc_Buffer_Length 	30
#define messgaeId_Length    		3
#define mid_Length          		5
#define data_Length         		20

//消息ID
#define position_report     			0x00
#define lockState_report     			0x07
#define device_state_report     	0x0a
#define heartbeat_report    			0x0b
#define GET_POSITION_REQ     			0x01
#define GET_POSITION_RES     			0x02
#define UN_LOCK_REQ     					0x08
#define UN_LOCK_RES     					0x09
#define GET_DEVICE_STATE_REQ     	0x0c
#define GET_DEVICE_STATE_RES     	0x0d
#define SET_STATE_INTERVAL_REQ    0x10
#define SET_STATE_INTERVAL_RES    0x11
#define SLEEPING_SHUTDOWN_REQ     0x12
#define SLEEPING_SHUTDOWN_RES     0x13


#define SUCCESS_CODE           0
#define FAIL_CODE              1

#define SUCCESS_STATUS         "01"
#define FAIL_STATUS            "00"

typedef struct Instructions 
{
	char instruc_Buffer[instruc_Buffer_Length];
	char messgaeId[messgaeId_Length];		
	char mid[mid_Length];			
	char data[data_Length];	
	char isGetData;	//是否响获取到数据
	char isParseData;	// 是否解析
	char isNull;
} InstrucData;

u16 getHeartbeatInterval(void);
u16 getStateInterval(void);
u16 getPositionInterval(void);
void showBattery(void);
void taskScheduler_init(void);
void taskScheduler(void);
int reportPosition(char* code, char type);
int reportLockState(char* code);
int reportDeviceState(void);
int reportHeartbeat(void);

#endif
