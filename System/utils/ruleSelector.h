#ifndef __RULESELECTOR_H
#define __RULESELECTOR_H

#include "../scheduler/scheduler.h"

#define onlineMinDistance       (100)
#define onlineMinDistanceNum    (20)
#define onlineMinDistanceRule   (5)

#define offlineMinDistance      (1000)
#define offlineMinDistanceNum   (5)
#define offlineMinRule          (10)
#define offlineMinDistanceRule  (15)

#define batteryMin              (20)
#define batteryMinDistance      (1000)
#define batteryMinDistanceNum   (10)
#define batteryMinRule          (5)
#define batteryMinDistanceRule  (10)

void ruleInit(void);
void onlineRule(char battery, double lat_Onenet, double lon_Onenet);
void offlineRule(double lat_Onenet, double lon_Onenet);

#endif
