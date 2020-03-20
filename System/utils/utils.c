#include "utils.h"

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
	//�����������ַ�����β�� ������ǰ
	--p;
	++start;
	if (*start == 0)
	{
		//�Ѿ����ַ�����ĩβ�� 
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

u8 chrtodec(char chr)
{
	u8 value = 0;

	//��ȫ����Сдת��Ϊ��д
	if ((chr >= 'a') && (chr <= 'z'))
		chr = chr - 32;

	//���ַ�ת������Ӧ������
	if ((chr >= '0') && (chr <= '9'))
		value = chr - 48;
	else if ((chr >= 'A') && (chr <= 'Z'))
		value = chr - 65 + 10;

	return value;
}

int cal16match(u8 n)
{
	u8 i = 0;
	int value = 1;

	for (i = 0; i < n; i++)
		value *= 16;

	return value;
}

int charhex_to_dec(char* chr)
{
	u8 i, chrlen;
	u8 sta = 0; //���Ҫת�����ַ����Ƿ���0x,����0X��ͷ
	int chrtoint = 0;
	int value = 0;

	chrlen = strlen(chr);

	//����ַ����Ƿ�����0x����0X��ͷ�ġ�
	if ((chr[0] == '0') && ((chr[1] == 'x') || (chr[1] == 'X')))
	{
		sta = 2;        //�����0x��0X��ͷ
		chrlen = chrlen - 2;
	}

	//����
	for (i = 0; i < chrlen; i++)
	{
		chrtoint = chrtodec(chr[sta + i]);
		if (i != (chrlen - 1))                //�������һλ
			value += chrtoint * cal16match(chrlen - 1 - i);
		else if (i == (chrlen - 1))         //���һλ
			value += chrtoint;

	}
	return value;
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

int getStrlen(char* str)
{
	int count = 0;
	while (*str++)count++;

	return count;
}

void appendchar(void* source, void* des, int source_len, int des_len)
{
	char* pSource = (char*)source;
	char* pDes = (char*)des;
	while (source_len != 0)
	{
		*((pDes++) + des_len) = *pSource++;
		source_len--;
		if (source_len == 0) {
			*((pDes++) + des_len) = '\0';
		}
	}
}

void rand4UUID(char des[5])
{
	char pool[] =
	{
		'0','1','2','3','4','5','6','7','8','9',
		'a','b','c','d','e','f','g','h','i','j',
		'k','l','m','n','o','p','q','r','s','t',
		'u','v','w','x','y','z','A','B','C','D',
		'E','F','G','H','I','J','K','L','M','N',
		'O','P','Q','R','S','T','U','V','W','X',
		'Y','Z'
	};//�����
	srand(Get_Adc());
	des[4] = '\0';
	int i = 0;
	while (i != 4)
	{
		des[i++] = pool[rand() % sizeof(pool)];
	}
}

/**
 * \brief Convert string to number
 */
int util_atoi(const char *str, int str_sz, int radix)
{
    char *tmp_ptr;
    char buff[NMEA_CONVSTR_BUF];
    int res = 0;

    if(str_sz < NMEA_CONVSTR_BUF)
    {
        memcpy(&buff[0], str, str_sz);
        buff[str_sz] = '\0';
        res = strtol(&buff[0], &tmp_ptr, radix);
    }

    return res;
}

/**
 * \brief Convert string to fraction number
 */
double util_atof(const char *str, int str_sz)
{
    char *tmp_ptr;
    char buff[NMEA_CONVSTR_BUF];
    double res = 0;

    if(str_sz < NMEA_CONVSTR_BUF)
    {
        memcpy(&buff[0], str, str_sz);
        buff[str_sz] = '\0';
        res = strtod(&buff[0], &tmp_ptr);
    }

    return res;
}

// �󻡶�
double radian(double d)
{
	return d * PI / 180.0;   //�Ƕ�1? = �� / 180
}

//�������
double get_distance(double lat1, double lng1, double lat2, double lng2)
{
	double radLat1 = radian(lat1);
	double radLat2 = radian(lat2);
	double a = radLat1 - radLat2;
	double b = radian(lng1) - radian(lng2);
	double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2))));
	dst = dst * EARTH_RADIUS;
	dst = round(dst * 10000) / 10;
	return dst;
}

double longitudeToOnenetFormat(char* lon_str)
{
	double lon_temp = 0;
	long lon_Onenet = 0;
	int dd_int = 0;
	long mm_int = 0;
	double lon_Onenet_double = 0;

	lon_temp = atof(lon_str);
	lon_Onenet = lon_temp * 100000;  //ת��Ϊ����

	dd_int = lon_Onenet / 10000000; //ȡ��dd

	mm_int = lon_Onenet % 10000000;  //ȡ��MM����


	lon_Onenet_double = dd_int + (double)mm_int / 60 / 100000;//����ΪOnenet��ʽ
	return  lon_Onenet_double;
}

double latitudeToOnenetFormat(char* lat_str)
{
	double lat_temp = 0;
	long lat_Onenet = 0;
	int dd_int = 0;
	long mm_int = 0;

	double lat_Onenet_double = 0;

	lat_temp = atof(lat_str);
	lat_Onenet = lat_temp * 100000;  //ת��Ϊ����

	dd_int = lat_Onenet / 10000000; //ȡ��dd

	mm_int = lat_Onenet % 10000000;  //ȡ��MM����

	lat_Onenet_double = dd_int + (double)mm_int / 60 / 100000;//����ΪOnenet��ʽ
	return  lat_Onenet_double;
}

