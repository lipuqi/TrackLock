#include "flash_memory.h"

uint8_t write_data[4096] = {0};	
uint8_t read_data[4096] = {0};

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;

void clrFlashData(void)
{
	memset(write_data, 0, 4069);
	memset(read_data, 0, 4069);
}


int Flash_Init(void)
{
	SPI_FLASH_Init();
	
	delay_ms(200);
	
	/* Get SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();
	
//	SPI_FLASH_BulkErase();
//	DeviceStatus device_Status;
//	device_Status.heartbeatInterval = 10;
//	device_Status.positionInterval = 2;
//	device_Status.stateInterval = 5;
//	writeConfig(device_Status);
//	offlineLengthInit();
	
	clrFlashData();	
	return FlashID == 0xEF4018;
}

	//初始化离线数据扇区
void offlineDataInit(int data_index)
{	
	SPI_FLASH_SectorErase(W25Q128FV_SUBSECTOR_SIZE * data_index);
	uint8_t data_length[5] = {0};
	sprintf((char*)data_length, "%04x", 0);
	SPI_FLASH_BufferWrite(data_length, W25Q128FV_SUBSECTOR_SIZE * data_index, 4);
}

// 初始化离线数据标识扇区
void offlineLengthInit(void)
{
	uint8_t data_index_length[5] = {0};
	SPI_FLASH_SectorErase(FLASH_OFFLINE_LENGTH);
	sprintf((char*)data_index_length, "%04x", 6);
	SPI_FLASH_BufferWrite(data_index_length, FLASH_OFFLINE_LENGTH, 4);
	
	offlineDataInit(6);
}

// 修改离线数据标识
u32 newOfflineLength(int d_index)
{	
	if(d_index < 6 ){
		d_index = 6;
	}
	if(d_index > 256 ){
		d_index = 256;
	}
	
	uint8_t data_index[5] = {0};
	SPI_FLASH_SectorErase(FLASH_OFFLINE_LENGTH);
	sprintf((char*)data_index, "%04x", d_index);
	SPI_FLASH_BufferWrite(data_index, FLASH_OFFLINE_LENGTH, 4);
	
	return W25Q128FV_SUBSECTOR_SIZE * d_index;
}

DeviceStatus readConfig(void)
{
	DeviceStatus device_Status;
	uint8_t Rx_Buf[13];
	char heartbeatInterval[5] = {0};
	char stateInterval[5] = {0};
	char positionInterval[5] = {0};
	SPI_FLASH_BufferRead(Rx_Buf, FLASH_CONFIG_INFO, 13);
	strncpy(heartbeatInterval, (char*)Rx_Buf, 4);
	strncpy(stateInterval, (char*)Rx_Buf + 4, 4);
	strncpy(positionInterval, (char*)Rx_Buf + 8, 4);
	device_Status.heartbeatInterval = charhex_to_dec(heartbeatInterval);
	device_Status.stateInterval = charhex_to_dec(stateInterval);
	device_Status.positionInterval = charhex_to_dec(positionInterval);
	clrFlashData();
	return device_Status;
}

void readUpgrade(void)
{
	uint8_t data_length[5] = {0};
	SPI_FLASH_BufferRead(data_length, FLASH_UPGRADE_LENGTH, 5);
	int d_length = charhex_to_dec((char*)data_length);
	
	SPI_FLASH_BufferRead(read_data, FLASH_UPGRADE_PROCEDURE, d_length);
	SPI_FLASH_SectorErase(FLASH_UPGRADE_PROCEDURE);
	clrFlashData();
}

void sendOfflineData(void)
{
	char delims[] = ",";
	char* result = NULL;
	result = strtok((char*)read_data + 4, delims);


	while (result != NULL)
	{
		BC35_COAPdata(result);
		result = strtok(NULL, delims);
	}
}

void readOfflineData(void)
	{
	// 查询离线数据标识确定最后数据地址
	uint8_t data_index[5] = {0};
	SPI_FLASH_BufferRead(data_index, FLASH_OFFLINE_LENGTH, 4);
	int d_index = charhex_to_dec((char*)data_index);
	u32 data_address = W25Q128FV_SUBSECTOR_SIZE * d_index;
	
	// 查询离线数据标识扇区内数据长度标识
	uint8_t data_length[5] = {0};
	SPI_FLASH_BufferRead(data_length, data_address, 4);
	int d_length = charhex_to_dec((char*)data_length);
	
	SPI_FLASH_BufferRead(read_data, data_address + 4, d_length);
	if(d_length != 0){
		sendOfflineData();
		SPI_FLASH_SectorErase(data_address);
		if(d_index > 6){
		newOfflineLength(d_index - 1);
		}
		clrFlashData();
	}
}

void writeConfig(DeviceStatus device_Status)
{
	sprintf((char*)write_data, "%04x%04x%04x", device_Status.heartbeatInterval, device_Status.stateInterval, device_Status.positionInterval);
	
	SPI_FLASH_SectorErase(FLASH_CONFIG_INFO);
	
	SPI_FLASH_BufferWrite(write_data, FLASH_CONFIG_INFO, strlen((char*)write_data));
	clrFlashData();
}

void writeUpgrade(char* data, int dataLength)
{
	uint8_t data_length[5] = {0};
	SPI_FLASH_SectorErase(FLASH_UPGRADE_LENGTH);
	SPI_FLASH_SectorErase(FLASH_UPGRADE_PROCEDURE);
	sprintf((char*)data_length, "%04x", dataLength);
	sprintf((char*)write_data, "%s", data);
	
	SPI_FLASH_BufferWrite(data_length, FLASH_UPGRADE_LENGTH, strlen((char*)data_length));
	SPI_FLASH_BufferWrite(write_data, FLASH_UPGRADE_PROCEDURE, dataLength);	
	clrFlashData();
}


void writeOfflineData(char* data, int dataLength)
{

	// 查询离线数据标识确定最后数据地址
	uint8_t data_index[5] = {0};
	SPI_FLASH_BufferRead(data_index, FLASH_OFFLINE_LENGTH, 4);
	int d_index = charhex_to_dec((char*)data_index);
	u32 data_address = W25Q128FV_SUBSECTOR_SIZE * d_index;
	
	// 查询离线数据标识扇区内数据长度标识
	uint8_t data_length[5] = {0};
	SPI_FLASH_BufferRead(data_length, data_address, 4);
	int d_length = charhex_to_dec((char*)data_length);
	
	
	// 如果新数据+扇区数据长度>扇区长度
	if((dataLength + d_length) > W25Q128FV_SUBSECTOR_SIZE){
		// 开辟新扇区
		data_address = newOfflineLength(d_index + 1);
		SPI_FLASH_SectorErase(data_address);
		
		
		sprintf((char*)write_data, "%04x%s", dataLength, data);
		SPI_FLASH_BufferWrite(write_data, data_address,  strlen((char*)write_data));
		
	} else if(d_length == 0)
	{
		SPI_FLASH_SectorErase(data_address);
			
		sprintf((char*)write_data, "%04x%s", dataLength, data);
		SPI_FLASH_BufferWrite(write_data, data_address, strlen((char*)write_data));
	} else
	{
		// 将旧数据读出，修改扇区数据长度，末尾追加新数据
		SPI_FLASH_BufferRead(read_data, data_address + 4, d_length);
		SPI_FLASH_SectorErase(data_address);
		
		sprintf((char*)write_data, "%04x%s%s%s", dataLength + d_length + 1, read_data, ",", data);

		SPI_FLASH_BufferWrite(write_data, data_address, strlen((char*)write_data));
	}
	clrFlashData();
}

