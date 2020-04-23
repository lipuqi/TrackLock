#include "ruleSelector.h"

DeviceStatus Device_Status;

char batteryMinNum;
char onlineMinNum;
char offlineMinNum;
u16 position_interval;

double longitude_Onenet = 116.403694;
double latitude_Onenet = 39.913164;

void ruleInit(void)
{
	batteryMinNum = 0;
	onlineMinNum = 0;
	offlineMinNum = 0;
	position_interval = readConfig().positionInterval;
}

void onlineRule(char battery, double lat_Onenet, double lon_Onenet)
{
	
	double distanc = get_distance(latitude_Onenet, longitude_Onenet, lat_Onenet, lon_Onenet);
	longitude_Onenet = lon_Onenet;
	latitude_Onenet = lat_Onenet;
		
	
		if(battery < batteryMin){
			//触发低电量规则
			if(distanc < batteryMinDistance)
			{
				// 低电量并且小于规则距离
				if(batteryMinNum < batteryMinDistanceNum){
						batteryMinNum++;
					} else {
						Device_Status.positionInterval = position_interval * batteryMinDistanceRule;
					}
			} else {
				Device_Status.positionInterval = position_interval * batteryMinRule;
				batteryMinNum = 0;
			}
		} else {
			//在线
			if(distanc < onlineMinDistance)
			{
				// 在线并且小于规则距离
				if(onlineMinNum < onlineMinDistanceNum){
					onlineMinNum++;
				} else {
						Device_Status.positionInterval = position_interval * onlineMinDistanceRule;
				}
			} else {
				Device_Status.positionInterval = position_interval;
				onlineMinNum = 0;
			}
		}
}

void offlineRule(double lat_Onenet, double lon_Onenet)
{
	
	double distanc = get_distance(latitude_Onenet, longitude_Onenet, lat_Onenet, lon_Onenet);
	longitude_Onenet = lon_Onenet;
	latitude_Onenet = lat_Onenet;
	
		// 随机重启策略
		if(distanc > offlineMinDistance){
			BC35_Restart();
			BC35_Init();
		}
			//离线
		if(distanc < offlineMinDistance)
		{
			// 离线并且小于规则距离
			if(offlineMinNum < offlineMinDistanceNum){
				offlineMinNum++;
			} else {
				Device_Status.positionInterval = position_interval * offlineMinDistanceRule;		
			}
		} else {
			Device_Status.positionInterval = position_interval;
			offlineMinNum = 0;
		}

}
