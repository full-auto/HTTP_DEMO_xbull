#ifndef _HCI_HCIPCONFIG_H
#define _HCI_HCIPCONFIG_H


#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
	#define PACKED
	#pragma pack(push)	
	#pragma pack(1)										//1 byte alignment
#else
#define PACKED							__attribute__((packed, aligned(1)))
#endif


#include "hci_def.h"


//#define HCI_CFG_BROADPARAM 	6040


//Intf_GetParam(HCI_CFG_BROADPARAM, 0, HIFACE_DEVICE_CFG_S , UINT nRsvd) 
//Intf_SetParam(HHCI_CFG_BROADPARAM, 0,HIFACE_DEVICE_CFG_S , UINT nRsvd)

typedef enum _VIDEO_SENSOR_TYPE_E
{

	E_SENSOR_TYPE_1080P_IMX327 		= 0x100,
	E_SENSOR_TYPE_1080P_IMX327_DBL	= 0x101,	
	E_SENSOR_TYPE_1080P_IMX385     	= 0x102,
	E_SENSOR_TYPE_1080P_IMX307		= 0x103,
	E_SENSOR_TYPE_1080P_IMX307_DBL	= 0x104,
	E_SENSOR_TYPE_500W_IMX335		= 0x105,
	E_SENSOR_TYPE_1080P_GC2053_DBL	= 0x106,
}VIDEO_SENSOR_TYPE_E;

typedef enum _VIDEO_VO_TYPE_E
{
	E_VO_NONE						= 0x00,
	E_VO_CVBS						= 0x01,
	E_VO_HDMI 						= 0x02,
	E_VO_BT1120						= 0x03,
	E_VO_LCD_8INCHES_SCD_ILI9881C 	= 0x04, // 视成达 ILI9881C 8寸高亮
	E_VO_LCD_8INCHES_HX				= 0x05, //宏显 8寸屏
	E_VO_LCD_8INCHES_XZM			= 0x06, // 鑫芝美 8寸
	E_VO_LCD_5INCHES_SY 			= 0x07, // 世阳 5寸
	E_VO_LCD_5INCHES_FRD			= 0x08, // 福瑞达 5寸
	E_VO_LCD_7INCHES_ZZW			= 0x09, //中正威 7寸屏 test
	E_VO_LCD_8INCHES_XZM_ILI9881C	= 0x0A, // 鑫美芝 8寸 ILI9881C
}VIDEO_VO_TYPE_E;



typedef enum _HIFACE_PLATFORM_TYPE_E
{
	E_PLATFORM_HIFACE			= 0x00, //HIFACE
	E_PLATFORM_DS				= 0x01,//大树
	E_PLATFORM_TJ				= 0x02,//天晶
	E_PLATFORM_FN    			= 0x03,//百度 蜂鸟
	E_PLATFORM_TN				= 0x04,//ten 牛
	E_PLATFORM_UNAVAILABLE 		= 0x100,
}HIFACE_PLATFORM_TYPE_E;

typedef enum _HIFACE_TEMP_TYPE_E
{
	E_TEMP_NONE				= 0x00, //NONE
	E_TEMP_DS				= 0x01,//大树
	E_TEMP_TM				= 0x02,//天瞐
	E_TEMP_TN				= 0x03,//ten 牛
}HIFACE_TEMP_TYPE_E;

typedef enum _HCM_4G_TYPE_E
{
	HCM_4GMODULES_NONE		= 0x00, //无4G模块
	HCM_4GMODULES_EC20		= 0x01,//移远EC20模块，带GPS
	
	HCM_4GMODULES_RESERVED,			// 保留类型
}HCM_4G_TYPE_E;


typedef struct _HCI_DEV_SERIALNO_S // 256byte
{
	UCHAR			byCfg_Flag;				// 是否已配置
	UCHAR			byMac[6];				//MAC地址
	CHAR			byDeviceID[32];			//设备ID
	CHAR			byUUID[64];				//UUID
	CHAR			byGUID[36];				//GUID
	UCHAR            byRes[117];				//预留
} PACKED HCI_DEV_SERIALNO_S; 


typedef struct _HCI_DEVICE_CFG_S  // 
{
	INT				bySensorType;			//传感器类型 VIDEO_SENSOR_TYPE_E
	UCHAR			byWiFi;					//WiFi
	UCHAR			byPhotoresistorLevel;	//光敏电阻检测 0.低 1.高 2.ADC
	UCHAR			byIRCUTLevel;	        //IRCUT控制 0.低 1.高
	UCHAR			byInfraredLampLevel;	//红外灯控制 0.低 1.高 2.PWM
	UCHAR           byLanguange;			 //0中文 1英文 
	UINT			dwDefaultETHIPAddr;		//默认ETH IP地址
	UINT			dwDefaultWIFIIPAddr;	//默认WIFI IP地址
	UCHAR           byOSDMode;		 		//OSD时间日期  显示模式 (0 时间左上标题右下 1 时间右上标题左下)
	UCHAR			byNetDhcp;				//有线dhcp   0 关 1开
	UCHAR			bywifiDhcp;				//wifi dhcp 0 关 1开
	UCHAR			bywifiDefaultMode;		//wifi 0.AP 1.STA
	UCHAR			byIRCUTMode; 			//IR CUT切换检测模式 0 --光敏检测  1-- 时间控制  2-- 视频检测 3 禁止红外模式		
	UCHAR 			byVOdev; 				//VO视频设备 VIDEO_VO_TYPE_E
	UCHAR			byAfSerialChn;     		//自动变倍串口号
	UCHAR			byTempModules;     		//0 close, 1 ds modules //HIFACE_TEMP_TYPE_E
	UCHAR			byPlatformCode;     	//platform code  0 default HIFACE_PLATFORM_TYPE_E
	UCHAR           byRes[1893];			//预留
} PACKED HCI_DEVICE_CFG_S;  //HHCI_CFG_BROADPARAM




#ifdef __cplusplus
}
#endif

#endif

