#include "ruleSelector.h"

DeviceStatus Device_Status;

char batteryMinNum = 0;
char onlineMinNum = 0;
char offlineMinNum = 0;

void onlineRule(char battery, double lat_Onenet, double lon_Onenet)
{
	
		double distanc = get_distance(latitude_Onenet, longitude_Onenet, lat_Onenet, lon_Onenet);
	
		if(battery < batteryMin){
			//�����͵�������
			if(distanc < batteryMinDistance)
			{
				// �͵�������С�ڹ������
				if(batteryMinNum < batteryMinDistanceNum){
						batteryMinNum++;
					} else {
						Device_Status.positionInterval = Device_Status.positionInterval * batteryMinDistanceRule;
					}
			} else {
				Device_Status.positionInterval = Device_Status.positionInterval * batteryMinRule;
				batteryMinNum = 0;
			}
		} else {
			//����
			if(distanc < onlineMinDistance)
			{
				// ���߲���С�ڹ������
				if(onlineMinNum < onlineMinDistanceNum){
					onlineMinNum++;
				} else {
						Device_Status.positionInterval = Device_Status.positionInterval * onlineMinDistanceRule;
				}
			} else {
				Device_Status = readConfig();
				onlineMinNum = 0;
			}
		}
}

void offlineRule(double lat_Onenet, double lon_Onenet)
{
	
	double distanc = get_distance(latitude_Onenet, longitude_Onenet, lat_Onenet, lon_Onenet);
		// �����������
		if(distanc > offlineMinDistance){
			BC35_Restart();
			BC35_Init();
		}
			//����
		if(distanc < offlineMinDistance)
		{
			// ���߲���С�ڹ������
			if(offlineMinNum < offlineMinDistanceNum){
				offlineMinNum++;
			} else {
				Device_Status.positionInterval = Device_Status.positionInterval * offlineMinDistanceRule;		
			}
		} else {
			Device_Status.positionInterval = Device_Status.positionInterval * offlineMinRule;
			offlineMinNum = 0;
		}

}
