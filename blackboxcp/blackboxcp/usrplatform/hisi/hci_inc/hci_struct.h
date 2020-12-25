#ifndef _HCI_STRUCT_H
#define _HCI_STRUCT_H


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

#define HCI_MAX_DISK_NUM		8
#define	MAX_STRING_32_LEN		32
#define	MAX_STRING_64_LEN		64
#define HCI_MAX_TIME_NUM		7
#define HCI_MAX_USER_NUM		3
#define HCI_MAX_GROUPTIME_NUM   20

#define HCI_DISABLE			0
#define HCI_ENABLE			1


typedef enum _HCI_DISK_TYPE_E
{
    DISK_NONE_E 	= 0,
	DISK_USB_E  	= 1,				//"/usb"
	DISK_HDD_E  	= 2,				//
	DISK_NETDISK_E 	= 3,
	DISK_EMMC_E		= 4,			//"/disk"
	DISK_SD_E		= 5,			// "/sd"
}HCI_DISK_TYPE_E;




typedef enum _HCI_UPLOADEPIC_TYPE_E
{
	UPLOADE_PIC_FACE 	= 0, //人脸图
	UPLOADE_PIC_BG	 	= 1, //背景图
	UPLOADE_PIC_FACE_BG = 2, //人脸图+背景图
}HCI_UPLOADEPIC_TYPE_E;



// configuration type
typedef enum _HCI_IOCTL_TYPE_E
{
	IOCTL_GET_ALARMIN,	  //报警输入
	IOCTL_GET_CDNS,       //光敏获取
	IOCTL_SET_ALARMOUT1,  //报警输出1控制
	IOCTL_SET_ALARMOUT2, //报警输出2控制
	IOCTL_SET_485RxTx,  //485 收发状态控制
	IOCTL_SET_LCD_LIGHT,//屏背光控制
	IOCTL_SET_IR,   //红外灯控制
	IOCTL_SET_WLED, //白光灯控制
}HCI_IOCTL_TYPE_E;



// 白光控制
typedef enum _HCI_WLED_TYPE_E
{
	WLED_CLOSE = 0,	  //从不开启
	WLED_LINK_FACE = 1,   //联动人脸控制
	WLED_LINK_MOTION = 2, //联动移动侦测
	WLED_LINK_TIME = 3,  //联动时间控制
	WLED_LINK_FACETIME = 4,  //联动人脸时间控制
	WLED_LINK_MOTIONTIME = 5,  //联动移动侦测时间控制
}HCI_WLED_TYPE_E;


//音频编码算法
typedef enum  _HCI_AENC_TYPE_E
{
	AENC_NONE   = 0x0,
	AENC_G726   = 0x01,
	AENC_G722   = 0x02,
	AENC_G711A  = 0x03,
	AENC_ADPCM  = 0x04,
	AENC_MP3    = 0x05,
	AENC_PCM    = 0x06,
	AENC_G711U  = 0x07,
	AENC_AACLC  = 0x08,
}HCI_AENC_TYPE_E;

//音频采样率
typedef enum  _HCI_SAMPLERATE_E
{
	SAMPLE_RATE_8000		= 8000,		//8kHz
	SAMPLE_RATE_11025	= 11025,	//11.025kHz
	SAMPLE_RATE_16000	= 16000,	//16kHz
	SAMPLE_RATE_22050	= 22050,	//22.050kHz
	SAMPLE_RATE_24000	= 24000,	//24kHz
	SAMPLE_RATE_32000	= 32000,	//32kHz
	SAMPLE_RATE_44100	= 44100,	//44.1kHz
	SAMPLE_RATE_48000	= 48000,	//48kHz
}HCI_SAMPLERATE_E;

//音频编码码率(G726)
typedef enum  _HCI_G726_BPS_E
{
	BPS_G726_16K	= 16000,
	BPS_G726_24K	= 24000,
	BPS_G726_32K	= 32000,
	BPS_G726_40K	= 40000,
	BPS_G726_128K= 128000,
}HCI_G726_BPS_E;

//lcd  size
typedef enum  _HCI_LCD_SIZE_E
{	LCD_NONE = 0,
	LCD_SIZE_8,
	LCD_SIZE_7,
	LCD_SIZE_4_3,
	LCD_SIZE_5,
}HCI_LCD_SIZE;



//日志的操作命令
typedef enum  _M_DOLOG_TYPE_E
{
	M_DOLOG_CLEAR 		= 0, //清除日志, 但是不清楚复位相关的日志信息
	M_DOLOG_CLEARALL 	= 1, //清除所有日志信息
	M_DOLOG_WRITE		= 2, //写日志,  HCI_WRITE_LOG_INFO_S
	M_DOLOG_SEARCH		= 3, //查询日志信息
	M_DOLOG_GETLOGNUM	= 4, //获取日志信息条目
}M_DOLOG_TYPE_E;


typedef enum  _M_LOG_TYPE_E
{
	M_LOG_TYPE_NONE    = 0x00,  //搜索时, 代表查询所有
	M_LOG_TYPE_SENSOR  = 0x01,  //报警输入
	M_LOG_TYPE_MOTION  = 0x02,  //移动侦测
	M_LOG_TYPE_VILOST  = 0x04,	//视频丢失
	M_LOG_TYPE_RF		= 0x08,
	M_LOG_TYPE_PIR		= 0x09,
	M_LOG_TYPE_NET		= 0x10,
	M_LOG_TYPE_TIMER	= 0x20,
	M_LOG_TYPE_MANUAL	= 0x40,
	M_LOG_TYPE_POWERON = 0x41,	 //设备开机
	M_LOG_TYPE_POWEROFF= 0x42,	 //设备关机

	M_LOG_TYPE_MANUAL_SW_RST= 0x80, //软复位
	M_LOG_TYPE_MANUAL_HW_RST= 0x81,  //硬件复位
	M_LOG_TYPE_MANUAL_GO_RST= 0x82,  //NO USE
	M_LOG_TYPE_PARAM_FILE_RST=0x83,
	M_LOG_TYPE_AUTO_RST     = 0x84,  //NO USE
	M_LOG_TYPE_MANUAL_PTZ_RST = 0x85,
	M_LOG_TYPE_VENC_ERR 	= 0x86,
	M_LOG_TYPE_SHELTER      = 0x88,
	M_LOG_TYPE_PULL_OUT_SD_CARD=0X89,

}M_LOG_TYPE_E;



typedef struct _HCI_DISK_INFO_S
{
	HCI_DISK_TYPE_E				nDiskType;					//HCI_DISK_TYPE_E
	UCHAR						cDiskExist;					//磁盘是否存在
	UCHAR						cDiskNo;					//磁盘号, 取值 0 ~ HCI_MAX_DISK_NUM - 1
	UCHAR						cDiskGroup;					//属于哪个盘组 1 ~ MAX_DISK_GROUP
	UINT						nAllCapability;				//总容量
	UINT						nFreeCapability;			//空余容量
	CHAR						ucMountPath[32];			//挂载目录
	UCHAR						cRes[16];
}PACKED HCI_DISK_INFO_S;


typedef struct _HCI_STORAGE_S
{
	UINT				nSize;
	UINT				nLocalDiskNum;
	HCI_DISK_INFO_S		stLocalDisk[HCI_MAX_DISK_NUM];
 	UCHAR				ucRes[256];
} PACKED HCI_STORAGE_S;


//HCI_CFG_TIME
typedef struct _HCI_DATE_S
{
	UINT			second : 6;							//秒:  0 ~ 59
	UINT			minute : 6;							//分:  0 ~ 59
	UINT			hour : 5;							//时:  0 ~ 23
	UINT			day : 5;							//日:  1 ~ 31
	UINT			month : 4;							//月:  1 ~ 12
	UINT			year : 6;							//年:  2000 ~
}HCI_DATE_S;


typedef struct _HCI_TIME_S
{
	UINT			week 	 : 5;							//星期
	UINT			millisec : 10;							//毫秒: 0 ~ 999
	UINT			second 	 : 6;							//秒:	0 ~ 59
	UINT			minute 	 : 6;							//分:   0 ~ 59
	UINT			hour 	 : 5;							//时:	0 ~ 23
}HCI_TIME_S;


typedef struct _HCI_DATE_TIME_S
{
	SHORT min;
	SHORT hour;
	SHORT day;
	SHORT mon;
	SHORT dayofweek;
	SHORT year;
} PACKED HCI_DATE_TIME_S;



typedef struct _HCI_DECTECT_RECT_S
{
	UCHAR				ucEnable;
	HCI_AREA_S			stRect;
	UCHAR				ucRes[32];
}HCI_DECTECT_RECT_S;


typedef struct _HCI_CHECK_TIME_S
{
	UCHAR			bEnable;							//是否检测
	UCHAR 			ucRes[3];
	HCI_TIME_S		stBgnTime;							//开始时间
	HCI_TIME_S		stEndTime;							//结束时间
}HCI_CHECK_TIME_S;


//===============log============================
#define MAX_LOG_COUNT  512

//M_DOLOG_WRITE
typedef struct _HCI_WRITE_LOG_INFO_S
{
	M_LOG_TYPE_E 			eLogType;
	UINT 					nChn;
	UCHAR					cRes[32];
}PACKED HCI_WRITE_LOG_INFO_S;

//返回的日志信息
typedef struct _HCILOG_SEARCH_INFO_S
{
	HCI_DATE_S		stTime;
	UINT			eLogType;
	UINT			nChn;
}PACKED HCILOG_SEARCH_INFO_S;

//M_DOLOG_SEARCH
typedef struct HCI_SEARCH_LOG_S
{
	HCILOG_SEARCH_INFO_S stLog[MAX_LOG_COUNT];
	HCI_DATE_S 		stStartTime;
	HCI_DATE_S 		stEndTime;
	UINT  			nmaxlog;
	M_LOG_TYPE_E	eLogType;
}PACKED HCI_SEARCH_LOG_S;


//==================================================================

//HCI_CFG_SYS
typedef struct _HCM_SYSTEM_INFO_S
{
	CHAR			szDevName[MAX_STRING_64_LEN];		//设备名称

	CHAR			szDevVer[MAX_STRING_64_LEN];		//设备版本 (只读)

	HCI_DATE_S		stDevCurTime;						//设备端当前时间(只读)

	UCHAR 			uclanguage;							//语言
	UCHAR 			res[3];
	UCHAR			szAiVer[MAX_STRING_32_LEN];
	UCHAR			szUiVer[MAX_STRING_32_LEN];
	UCHAR			szPlatformVer[MAX_STRING_32_LEN];
	UCHAR			ucRes[36];

}PACKED HCM_SYSTEM_INFO_S;



//HCI_CFG_DEVICE  function
typedef struct _HCI_DEVICE_INFO_S
{
	CHAR			szDevID[MAX_STRING_32_LEN];			//设备ID DevSerial
	UCHAR			ucTempModules; 						//0 close
	UCHAR			ucLCDSize;							//HCI_LCD_SIZE
	UCHAR			szDevType[MAX_STRING_32_LEN];		//设备类型
	UCHAR			ucRes[224];

}PACKED HCI_DEVICE_INFO_S;

//HCI_CONFIG_TYPE_E is  HCI_CFG_NET
typedef struct _HCI_NETBASE_S
{
	INT				ibDhcp;							//是否使用DHCP
	UINT			uiIPAddr;						//IP地址
	UINT			uiSubmask;						//子网掩码
	UINT			uiGateway;						//网关
	UCHAR			ucMacAddr[6];					//MAC地址
	UCHAR			res1[2];
	UINT			uiDNS1;							//DNS 1
	UINT			uiDNS2;							//DNS 2
	UINT			uiWebPort;						//WEB服务端口
	UINT			uiOnvifPort;					//ONVIF 端口

	UCHAR			ucRes[404];			//
}PACKED HCI_NETBASE_S;


//命令: HCI_CFG_USER
typedef struct _HCI_USERINFO_S
{
	INT				iExist;
	UINT			uiPriority;
	CHAR			szUserName[MAX_STRING_32_LEN];
	CHAR			szPassword[MAX_STRING_32_LEN];
	UCHAR			ucRes[260];
}PACKED HCI_USERINFO_S;



//HCI_CFG_FACEDETEC
typedef struct _HCM_FACE_DETECT_S
{
	UCHAR				ucEnable;				//开关
	UCHAR				ucquality;			//人脸质量阈值
	UCHAR				ucSnapMode;			//
	UCHAR 				ucTrackEnable;		//人脸跟踪  	 0:关闭 1:开启
	UCHAR				ucSnapNum;			//最大抓拍次数
	UCHAR				ucMaskCheckEnable;	//口罩检测
	UCHAR				ucUploadePicMode;	//图片上传  , 0 人脸图  , 1 背景图, 2 人脸+背景图
	UCHAR 				ucLivenessEnable;	//活体检测开关
	INT					iFaceMinSize;		//人脸识别最小尺寸(30~300)
	INT					iFaceMaxSize;		//人脸识别最小尺寸(300~500)

	HCI_DECTECT_RECT_S 	stDetectRect;		//检测区域设置
	HCI_CHECK_TIME_S  	stDetectTime; 		//检测时间控制
	FLOAT 				fLivescore;			//活体阈值(0.01 ~ 0.99)
	UCHAR				ucRes[512-4];
	//UCHAR		byRes[16];
}PACKED HCM_FACE_DETECT_S;



//人脸控制 HCI_CFG_FACECTL
typedef struct _AI_FACE_CTL_S
{
	UCHAR				ucCmpEnable;		//比对控制开关
	UCHAR				ucIoEnable;		//IO输出使能
	UCHAR				ucIoType;		//IO输出类型 IO输出模式 常开 常闭
	UCHAR				cres1;
	UINT				sIoTime;		//IO输出时间 ms
	HCI_CHECK_TIME_S	stcheckTime;	//布防时间
	SHORT				sCmpMode;		//识别模式
	SHORT				sCmpNum;		//识别次数 (1-10)
	UCHAR				ucAudioCtlEnable;
	UCHAR				ucIoMode;		//
	UCHAR				ucUiDisPlayMode;//0 显示当前抓拍图， 1 显示底库图片
	UCHAR 				cres2;
	FLOAT				fCmpSore;		//比对阈值(0.0 ~ 100.0)
	UINT				sIo2Time;		//IO输出时间 ms
	UCHAR				ucIo2Enable;	//IO输出2使能
	UCHAR				ucIo2Type;		//IO输出2 类型 IO输出模式 常开 常闭

	UCHAR 				ucLcdDisplayMode; //0 无人后关闭显示,  1一直显示  //默认0
	SHORT				sLcdCloseTime;   //无人后多久关闭显示  10 ~ 600 秒, 默认30秒
	UCHAR 				ucWLedMode; // HCI_WLED_TYPE_E 默认 WLED_LINK_FACE //
	SHORT				sLedFaceTime;   //无人后多久关闭led  10 ~ 600 秒, 默认30秒
	SHORT				sLedMotionTime; //无移动物体后多久关闭led  10 ~ 600 秒, 默认30秒
	HCI_TIME_S			stWledDayTime; //白光灯时间模式下       , 白天 关闭白光灯
	HCI_TIME_S			stWledNigTime; //白光灯时间模式下 晚上 开启白光灯

	FLOAT				fAlarmTempValue;// default 37.3
	HCI_AREA_S 			stTempCheckRect;//温度检测区域设置
	UCHAR				ucTempEnable;// 是否开启温度模块 0 关闭   , 1 开启
	UINT				uiTempMinFaceSize; //测温 最小像素  default 300    (0-1080)
	UINT				uiTempMaxFaceSize; //测温 最大像素 default 900 (500-1080)
	UCHAR				ucScreenLcdTime; //屏保时间
	UCHAR				ucUiShowUsrName; // 识别通过时是否显示人名
	HCI_TIME_S		 	stScreenstartTime; //屏保起始时间
	HCI_TIME_S 			stScreenendTime; //屏保结束时间
	UCHAR		 		ucRes1[12];
	UCHAR		 		ucRes[512];
}PACKED AI_FACE_CTL_S;


typedef struct _HCM_AI_VERSION_INFO_S
{
	CHAR 	szAiLib_version[64] ;
	CHAR 	szModel_version[64];
	UCHAR  	ucLic;
	UCHAR  	ucRes[255];
}PACKED HCM_AI_VERSION_INFO_S;



typedef struct _CHM_HTTP_CONFIG_S   //1948
{
	UCHAR 	 ucEnable; //0 disable  1 enable
	UCHAR 	 ucUploadeImgEnable; //0  disable  1 enable
	UCHAR	 ucSupNetSend; //支持断网续传 0  disable  1 enable
	UCHAR	 res1[1];
	INT      serverport;
	CHAR     serverip[128]; //(或域名)

	INT  	wsServerPort;
	CHAR    wsServerIp[128]; //(或域名)
	INT  	nHttpClientCode;

	UCHAR	 ucRes[1912];
}PACKED HCM_HTTP_CONFIG_S;



//功能: 定时重启    , HCI_CFG_TIMEREBOOT
typedef struct _TIME_REBOOT_S
{
	UCHAR 			ucEnable; //0: 关闭         1: 每小时	  2: 每天		4: 每周

	HCI_TIME_S		stTime;	//定时重启时间

	UCHAR			ucRes[128];
}PACKED TIME_REBOOT_S;


//NTP HCI_CFG_NTP
typedef struct _HCM_NTP_S
{
	//属性
	UCHAR 	 		ucEnable; 						//0 disable  1 enable
	UINT			unTimeZone;						//时区
	CHAR			szServerUrl[MAX_STRING_64_LEN];	//NTP 域名地址
	UINT			unPort;							//端口
	UINT			unIntervalTime;				//校时间隔

	UCHAR			byRes[128];

}PACKED HCM_NTP_S;

//Factory
typedef struct _HCM_Factory_S
{
	UCHAR 			ucNetEnable; //0 不恢复网络参数  ,      1 恢复网络参数
	UCHAR 			ucUsrInfoEnable;//0 不恢复用户参数  ,      1 恢复用户参数

	UCHAR			byRes[128];

}PACKED HCM_Factory_S;



typedef enum  _E_VENC_TYPE
{
	E_VENC_NONE   = 0x0,
	E_VENC_H264   = 0x01,
	E_VENC_MPEG4  = 0x02,
	E_VENC_MJPEG  = 0x03,
	E_VENC_JPEG   = 0x04,
	E_VENC_H265   = 0x05,
	E_VENC_H265_S = 0x06,
}E_VENC_TYPE;

//HCI_CFG_VIDEO nch = 0 为主码流  1 为副码流
typedef struct _HCM_VENC_INFO_S
{
	UINT				nPicWidth;					//宽
    UINT				nPicHeight;					//高
	UINT				nFramerate;					//帧率
	UINT				nGop;						//I帧间隔 取值范围: [0, 200]，以帧为单位
	UINT				nBitrate;					//码率
	UINT				nVencType;					//编码格式 E_VENC_TYPE

	UCHAR				ucprofile;					//0: baseline 1:main profile 2:high profile
	UCHAR				ucRes[83];

}PACKED HCM_VENC_INFO_S;


typedef struct _HCM_AUDIO_INFO_S
{

	UCHAR				bEnable;						//是否打开

	UCHAR				bMicLine;						//麦克或线输入

	UCHAR				ucVolumeIn;						//输入音量, 0~15

	UCHAR				ucVolumeOut;					//输出音量, 0~15

	HCI_SAMPLERATE_E	eSampleRate;					//采样率

	HCI_AENC_TYPE_E		eAencType;						//编码算法

	HCI_G726_BPS_E		eBitrate;						//码率

	UCHAR				byRes[60];
}PACKED HCM_AUDIO_INFO_S;

typedef struct _HCM_UART_INFO_S
{
	UINT	baudrate;		// 波特率
	UCHAR	datawidth;		// 数据位
	UCHAR 	parity;			// 校验位
	UCHAR	stopbit;		// 停止位
	UCHAR	RS485;			// 这个串口是否为485 0否 1是
}HCM_UART_INFO_S;

typedef enum {
	HCM_WIEGAND_NONE	= 0,

	HCM_WIEGAND_26		= 0x26,
	HCM_WIEGAND_34		= 0x34,
	HCM_WIEGAND_36		= 0x36,
	HCM_WIEGAND_44		= 0x44,

	HCM_WIEGAND_ERROR	= 0xff, // ERROR
} HCM_WIEGAND_TYPE_E;

// =============== WG =============
typedef struct HCM_Wiegand_Data26_s
{
	unsigned char	HID;		// Wiegand 26 HID 8bit
	unsigned short	PID;
}HCM_Wiegand_Data26;

typedef struct HCM_Wiegand_Data34_s
{
	unsigned short	HID;		// Wiegand 34 HID 16bit
	unsigned short	PID;
}HCM_Wiegand_Data34;

typedef struct HCM_Wiegand_Data_s
{
	HCM_WIEGAND_TYPE_E Wiegand_Type;		// Wiegand Type 26 / 34 , if 0xff ,ERROR
	union {
		HCM_Wiegand_Data26 Wiegand26_Data;
		HCM_Wiegand_Data34 Wiegand34_Data;
	};
}HCM_Wiegand_Data;


typedef struct HCM_Frame_Info_s
{
	UINT	nFrameNo;		// 帧号
	ULONG	nTimeStamp;		// 时间戳
	UCHAR	nKeyFrame;		// 是否为关键帧 1表示为I帧，0表示其他帧
	ULONG	nByteLen;		// 帧长度(含帧头12个字节)
	UCHAR	res[32];		// 保留字
}HCM_Frame_Info;



//功能:报警输入
//命令: HCI_CFG_ALARMIN
typedef struct _HCM_ALARM_IN_S
{

	UCHAR			 bEnable;		//开关
	UCHAR 			 nAlmIOType; 	//0: 常开,1: 常闭
	HCI_CHECK_TIME_S stCheckTime;	//时间段

	UCHAR			 byRes[256];
}HCM_ALARM_IN_S;


typedef struct _HCM_GNSS_TIME_S
{
    int     year;       /**< Years since 1900 */
    int     mon;        /**< Months since January - [0,11] */
    int     day;        /**< Day of the month - [1,31] */
    int     hour;       /**< Hours since midnight - [0,23] */
    int     min;        /**< Minutes after the hour - [0,59] */
    int     sec;        /**< Seconds after the minute - [0,59] */
    int     hsec;       /**< Hundredth part of second - [0,99] */

} HCM_GNSS_TIME;

typedef struct  _HCM_GNSS_MSG_S
{
	int			Sig;			// GPS 定位质量  （0=无效；1=固定；2=差分，3=敏感）
	HCM_GNSS_TIME	UTC;			//GNSS 卫星授时
	double		Latitude;		//纬度 Latitude in NDEG - +/-[degree][min].[sec/60]
	double		Longitude;		//经度 Longitude in NDEG - +/-[degree][min].[sec/60]
	double		Altitude;		//海拔
    double  	PDOP;       	//位置精度因子 /**< Position Dilution Of Precision */
    double  	HDOP;       	//水平精度因子 /**< Horizontal Dilution Of Precision */
    double  	VDOP;      	 	//垂直精度因子 /**< Vertical Dilution Of Precision */
    double  	Speed;			//地面速率 Speed over the ground in kilometers/hour
}HCM_GNSS_MSG;

typedef enum _HCM_4G_TYPE_E
{
	HCM_4GMODULES_NONE		= 0x00, //无4G模块
	HCM_4GMODULES_EC20		= 0x01,//移远EC20模块，带GPS

	HCM_4GMODULES_RESERVED,			// 保留类型
}HCM_4G_TYPE_E;

typedef struct _HCM_4G_INFO_S
{
	INT					dwDevStatus;					//4G模块是否存在
	HCM_4G_TYPE_E		dwModulesType;					//模块类型
	INT					dwNetStatus;					//网络状态 0 断开 1 联通 -1 守护线程终止
	INT					dwSimStatus;					//SIM卡状态 0 拔出 1 插入
	INT					dwCSQ;							//信号强度 0-31信号正常 其他值未不正常
	INT 				dwTelecomCode;					// 运营商代码 (用于识别运营商)
	/*
	46000 中国移动 （GSM）
	46001 中国联通 （GSM）
	46002 中国移动 （TD-S）
	46003 中国电信（CDMA）
	46004 空
	46005 中国电信 （CDMA）
	46006 中国联通 （WCDMA）
	46007 中国移动 （TD-S）
	46008
	46009
	46010
	46011 中国电信 （FDD-LTE） 这个是电信的4G网络测试信号频道
	46020 是铁路专用网络,高铁动车的移动GSM_R网,调度用的
	*/
}HCM_4G_INFO_S;

typedef struct _HCM_4G_S
{
	UCHAR				bEnable;			// 使能4G
	HCM_4G_TYPE_E		dwModulesType;		//模块类型
	UINT				dwGuardIP;			//守护IP
	INT					dwAPN_Auto;			//自动选择APN
	CHAR				dwAPN[32];			//APN
	CHAR				dwAPN_User[32];		//APN用户名
	CHAR				dwAPN_Passwd[32];	//APN密码
	CHAR				dwDNS_Type;			//0 使用系统DNS 1 使用4G DNS
	UCHAR				byRes[128];
}PACKED HCM_4G_S;

typedef struct _HCM_WIFI_S
{
	UCHAR				bEnable;			// 使能WIFI
	UINT				dwIPAddr;
	UINT				dwGateway;
	UINT				dwSubmask;
	UCHAR				byWifiType;  // WIFI模块类型，参考 HIFACE_WIFITPYE
	UCHAR				bDhcp;		 // wifi dhcp 0 关 1开
	UCHAR				byWifiMode;	 // 当前wifi 模式  0  sta  1 ap
	UCHAR				bySSID[32];
	UCHAR				byPasswd[32];
	UCHAR				byEncryptType;
	UCHAR				byConnectStatus;	// WIFI模块是否已连接AP
	UCHAR				byMacAddr[6];		//MAC地址
	UINT				dwDNS1;
	UINT				dwDNS2;
	UCHAR				byExistStatus;		// WIFI模块是否存在
	UCHAR				byRes[127];
}PACKED HCM_WIFI_S;

typedef struct _HCM_WIFISCANINFO_S
{
	CHAR	byBSSID[6];			// BSSID
	INT		dwFrequency;		// 频道
	INT		dwSignal;			// 信号
	CHAR	byEncryptType[32];	// 加密类型
	CHAR	bySSID[32];			// SSID
}PACKED HCM_WIFISCANINFO_S;

typedef struct _HCM_WIFILIST_S
{
	HCM_WIFISCANINFO_S stList[64];
	CHAR	byWifiNum;
}PACKED HCM_WIFILIST_S;

typedef enum _HCM_WIFILINKSTATUS_S
{
	HCM_WIFI_UNK,	//未知状态
	HCM_WIFI_INACTIVE,		// wifi未启用
	HCM_WIFI_COMPLETED,		// wifi连接完成
	HCM_WIFI_DISABLED,		// wifi被禁用
	HCM_WIFI_SCANNING,		// wifi扫描
	HCM_WIFI_DISCONNECTED,	// wifi未连接
}HCM_WIFILINKSTATUS_S;

typedef struct _HCM_WIFISTATUS_S
{
	HCM_WIFILINKSTATUS_S	enWpa_State;		// = INACTIVE
	CHAR	byAddress[6];					// MAC地址
	CHAR	byBSSID[6]; 					// BSSID
	INT 	dwFreq; 						// 信道
	CHAR	bySSID[128];					// SSID
	CHAR	byGroup_Cipher[16];
	CHAR	byPairwise_Cipher[16];
	CHAR	byKey_Mgmt[16];
}PACKED HCM_WIFISTATUS_S;




//144 韦根
typedef struct _HCM_WG_S
{
	UCHAR  bWGEnable;//韦根使能
	UCHAR  bWGEndianMode; //韦根大小端
	UCHAR  bWGProtocol;// 韦根26 34
	UCHAR  bWGInModes;//韦根输入模式 0 同步抓取人脸,   1 直接通行
	UINT 	nPublicNum; //公共卡号
	UINT 	nMinNum;	//卡号范围
	UINT 	nMaxNum;
	UCHAR  byRes[128];
}PACKED HCM_WG_S;


//HCI_CFG_EPTZ
typedef struct _HCM_EPTZPARAM_S
{
	UCHAR  bEptzEnable;//使能
	UCHAR  byRes[63];
}PACKED HCM_EPTZPARAM_S;



//FACE_GROUP_TIME_S  len 128 HCI_CFG_GROUPTIME
typedef struct _HCM_GROUP_TIME_S
{
	UCHAR nEnable;
	UCHAR nres[3];
	CHAR szGroupName[32]; //组的名称
	CHAR szTime[80];//时间
	UCHAR byRes[12];

}PACKED HCM_GROUP_TIME_S;


typedef struct _HCM_IDCARD_S
{
	UCHAR  bEnable;//使能
	UCHAR  bMode; //身份证验证模式, 0  白名单 或 身份证  ， 1 身份证+人脸验证
	SHORT sCmpVal;//身份证识别阈值

	UCHAR  byRes[64];
}PACKED  HCM_IDCARD_S;



#ifdef __cplusplus
}
#endif

#endif

