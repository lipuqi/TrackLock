#ifndef __FLASH_MEMORY_H
#define __FLASH_MEMORY_H

#include "stm32f10x.h"
#include "./spi_flash.h"
#include "../../System/utils/utils.h"
#include "../bc35g/BC35.h"

#define W25Q128FV_FLASH_SIZE                  0x1000000 /* 128 MBits => 16MBytes */
#define W25Q128FV_SECTOR_SIZE                 0x10000   /* 256 sectors of 64KBytes */
#define W25Q128FV_SUBSECTOR_SIZE              0x1000    /* 4096 subsectors of 4kBytes */
#define W25Q128FV_PAGE_SIZE                   0x100     /* 65536 pages of 256 bytes */

#define FLASH_CONFIG_INFO          0x00000
#define FLASH_UPGRADE_LENGTH       W25Q128FV_SUBSECTOR_SIZE
#define FLASH_UPGRADE_PROCEDURE    W25Q128FV_SUBSECTOR_SIZE * 2
#define FLASH_OFFLINE_LENGTH       W25Q128FV_SUBSECTOR_SIZE * 5

typedef struct DeviceData 
{
	u16 heartbeatInterval;
	u16 stateInterval;
	u16 positionInterval;
} DeviceStatus;


int Flash_Init(void);
void offlineLengthInit(void);

DeviceStatus readConfig(void);
void readUpgrade(void);
void readOfflineData(void);

void writeConfig(DeviceStatus device_Status);
void writeUpgrade(char* data, int dataLength);
void writeOfflineData(char* data, int dataLength);

#endif
