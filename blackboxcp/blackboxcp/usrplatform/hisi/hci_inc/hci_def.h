#ifndef _HCI_DEF_H
#define _HCI_DEF_H

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


#ifndef UCHAR
typedef unsigned char 			UCHAR;
#endif

#ifndef CHAR
typedef 	 	char 			CHAR;
#endif

#ifndef INT
typedef          int            INT;
#endif

#ifndef UINT
typedef unsigned int            UINT;
#endif

#ifndef USHORT
typedef unsigned short          USHORT;
#endif

#ifndef SHORT
typedef short					SHORT;
#endif

#ifndef LONG
typedef long					LONG;
#endif

#ifndef ULONG
typedef	unsigned long			ULONG;
#endif

#ifndef UINT64
typedef unsigned long long      UINT64;
#endif


#ifndef UInt64
typedef unsigned long long      UInt64;
#endif

#ifndef VOID
typedef void					VOID;
#endif

#ifndef FLOAT
typedef float					FLOAT;
#endif


#define HCI_SUCCESS		  				0
#define HCI_FAIL_E 						-1 ///< 内部错误
#define HCI_INVALIDARG_E 				-2 	///< 参数不合法
#define HCI_HANDLE_E 					-3 ///< 句柄错误
#define HCI_OUTOFMEMORY_E 				-4 ///< 内存不足
#define HCI_DELNOTFOUND_E				-5 ///< 定义缺失
#define HCI_INVALID_PIXEL_FORMAT_E 		-6 ///< 不支持的图像格式
#define HCI_LIBRARY_VERSION_MISMATCH_E 	-7 ///< 库模块版本不一致
#define HCI_FUNC_REPEAT_REGISTER_E		-8 // < 函数接口重复注册
#define HCI_YUV_STREAM_BUF_FULL_E		-9 // < YUV 缓存满
#define HCI_LICENSE_IS_NOT_ACTIVABLE_E 	-10 //未授权
#define HCI_FILE_OPENFAILE			 	-11 //文件打开失败


#define HCI_UNSUPPORTED_E 				-1000 ///< 不支持的函数调用方式



#define HCI_MAX_DRAWNUM					64  //最大画人脸数

#define COLOR_RED		0x00ff0000
#define COLOR_GREEN		0x0000ff00
#define COLOR_BLUE		0x000000ff
#define COLOR_SLATE_GRAY 0x708090

// configuration type
typedef enum _HCI_CONFIG_TYPE_E
{
	HCI_CFG_SYS,		// System information
	HCI_CFG_DEVICE,		// Device Information
	HCI_CFG_TIME,		// Date time
	HCI_CFG_USER,

	//video audio
	HCI_CFG_VIDEO,
	HCI_CFG_AUDIO,
	HCI_CFG_ISP,
	HCI_CFG_VIMASK,
	HCI_CFG_OSD,

	//alarm
	HCI_CFG_VIMOTION,
	HCI_CFG_IOIN,
	HCI_CFG_VILOSE,
	HCI_CFG_NETLOSE,

	//net...
	HCI_CFG_NET,// Network service settings
	HCI_CFG_FTP,
	HCI_CFG_NTP,
	HCI_CFG_WIFI,
	HCI_CFG_4G,
	HCI_CFG_VPN,
	HCI_CFG_PPPOE,
	HCI_CFG_SMTP,
	HCI_CFG_RTSP,
	HCI_CFG_GB2818,
	HCI_CFG_UPNP,
	HCI_CFG_HTTP,

	//
	HCI_CFG_SERIAL,

	//
	HCI_CFG_FACEDETEC,
	HCI_CFG_FACECTL,
	HCI_CFG_AIVER,

	HCI_CFG_TIMEREBOOT,
	HCI_CFG_REBOOTING,
	HCI_CFG_FACTORY,

	HCI_CFG_SUPERPASSWD,

	HCI_CFG_ALARMIN,

	HCI_CFG_WG,

	HCI_CFG_EPTZ,

	HCI_CFG_UBOOTLOGO,

	HCI_CFG_GROUPTIME,
	HCI_CFG_IDCARD,

	HCI_CFG_BROADCONFIG = 6040,
	HCI_CFG_DEVSERIAL	= 6041,
	HCI_CFG_AUTOCHECK	= 6042,
	HCI_CFG_DEVSERIAL_H = 15630, //高级配置
	//...

}HCI_CONFIG_TYPE_E;


typedef enum _HCI_LOCAL_TYPE_E
{
	HCI_LOCAL_MOTIONALARM,				// motion alarm
	HCI_LOCAL_SENSALARM,				// sens   alarm
	HCI_LOCAL_NETALARM,					// net lose alarm
	HCI_LOCAL_DAYNIGHT,					// day  night status
	HCI_LOCAL_FORMATEMMC,				//disk emmc format  status 1 start, 0 format end
	HCI_LOCAL_TEMP,						//temp data HCI_TEMP_S
	//...

}HCI_LOCAL_TYPE_E;





typedef enum _HCI_CHN_SUPPORT_TYPE_E
{
	HCI_CHN_UNSUPPORT,				// unsupported
	HCI_CHN_SUPPORTUSE,				// support use
	HCI_CHN_MODIFY,					// Support and modify

	HCI_CHN_NONE,				   //chn non-existent
	//...

}HCI_CHN_SUPPORT_TYPE_E;


typedef enum _HCI_PIXEL_FORMAT_TYPE_E
{
	HCI_PIXEL_FORMAT_YUV420 = 1,
	HCI_PIXEL_FORMAT_YUV422 = 2,

	//...

}HCI_PIXEL_FORMAT_TYPE_E;


/*nModuleType*/
typedef enum _HCM_MODULE_TYPE_E
{
	HCM_AIMODULE_E			= 0x1,
	HCM_UIMODULE_E			= 0x2,
	HCM_PLATFORMMODULE_E	= 0x4,
	HCM_USRPLATFORMMODULE_E	= 0x8,
}HCM_MODULE_TYPE_E;



typedef struct _HCM_MSG_HEAD_S
{
	UINT		u32MsgType;  	/*user define*/
	UINT		u32DestModule;	/*HCM_MODULE_TYPE_E*/
	UCHAR		u8Misc;			/*user define*/
	UCHAR		rsvd[31];		/*user define*/
}PACKED HCM_MSG_HEAD_S,* LPHCM_MSG_HEAD_S;

typedef struct _HCI_VIDEO_FRAME_S{
	UINT	nWidth;						//Image width
	UINT	nHeight;					//Image height
	UINT	nPixelFormat;				//Pixel format, 1:yuv420, 2:yuv422
	UINT64 	u64PhyAddr[3];
	UINT64	u64VirAddr[3];
	UINT	nStride[3];					//line size
	ULONG	nPts;						//Image time stamp
}PACKED HCI_VIDEO_FRAME_S,*LPHCI_VIDEO_FRAME_S;


typedef struct _HCI_RECT_S
{
    UINT s32X;
    UINT s32Y;
    UINT u32Width;
    UINT u32Height;
}PACKED HCI_RECT_S;

typedef struct _HCI_POSITION_S
{
	UINT nleft;
	UINT ntop;
	UINT nright;
	UINT nbottom;
}HCI_POSITION_S;

//区域
typedef struct _HCI_AREA_S
{
	SHORT			x;									//X坐标
	SHORT			y;									//Y坐标
	SHORT			w;									//宽度
	SHORT			h;									//高度
}HCI_AREA_S;

typedef struct _HCI_CHNHEAD_S
{
    UINT 	nGropChn; //0 rgb   1  ir
    UINT 	nYuvChn; //yuv chn
	UCHAR	rsvd[16];
}PACKED HCI_CHNHEAD_S;


typedef struct _HCI_CHNINFO_S
{
    INT 	nGropChn; //0 rgb   1  ir
    INT 	nYuvChn; //yuv flag
	UINT	nWidth;
	UINT	nHeight;
	HCI_PIXEL_FORMAT_TYPE_E eFormatType;
	HCI_CHN_SUPPORT_TYPE_E	eSupportType;
	UCHAR	rsvd[32];
}PACKED HCI_CHNINFO_S;



typedef struct _HCI_VIDEO_ENCODE_S{
	UINT 		nEncodeQuality; // encode quality  1 ~ 99
	UINT 		nCropEnable;  //0 关闭抠图即编码原图,     1 启动 编码抠图
	HCI_RECT_S 	stEncodeRect; ////位置需要16字节对齐
	UINT 		nCompressEnable;
	UINT 		nCompressWidth;
	UINT 		nCompressHeight;
}PACKED HCI_VIDEO_ENCODE_S;


typedef struct _HCI_DRAW_INFO_S{
	HCI_POSITION_S 	stDrawPos;
	UINT nColor;
	UINT nLineWidth;
	UINT nLineLen;

}PACKED HCI_DRAW_INFO_S;

typedef struct _HCI_DRAW_S{
	HCI_DRAW_INFO_S 	stDrawInfo[HCI_MAX_DRAWNUM];
	USHORT		nDrawEnable;
	USHORT		nDrawCnt;
	USHORT 		nSrcWidth;
	USHORT 		nSrcHeight;
}PACKED HCI_DRAW_S;


typedef struct _HCM_EPTZ_S
{
	INT 		nEnable;
	USHORT 		nSrcWidth;
	USHORT 		nSrcHeight;
	HCI_AREA_S 	stAera;
}PACKED HCM_EPTZ;


typedef enum HCI_DEV_IDCARD_TPYE_S
{
	HCI_IDCARD_TYPE_NONE,
	HCI_IDCARD_TYPE_CITIZEN,		// 居民身份证
	HCI_IDCARD_TYPE_HMTID,			// 港澳台居民居住证
	HCI_IDCARD_TYPE_FPRID,			// Foreign Permanent Resident IDCard 外国人永久居留证
	HCI_IDCARD_TYPE_THAI,			//泰国身份证
}HCI_IDCARD_TPYE;


// 港澳台居民居住证 信息
typedef struct HCI_DEV_HMTID_IDCARD_INFO_S
{
    UCHAR name[32];			/* 姓名  */
    UCHAR gender[8]; 		//性别
    UCHAR birthday[18];		//出生日期
    UCHAR address[128];		//住址
    UCHAR id[38];			//公民身份证号码
    UCHAR maker[64];			//签发机关
    UCHAR start_date[18];	//有效起始日期
    UCHAR end_date[18];		//有效截止日期
    UCHAR passNumber[24];	//通行证号码
    UCHAR lssueTimes[6];		//签发次数
    UCHAR cardType[6];		//证件类型标识
    UCHAR reserved3[36];		//预留区
}PACKED HCI_HMTID_IDCARD_INFO_S;

// 外国人永久居留证
typedef struct HCI_DEV_FPRID_IDCARD_INFO_S
{
    UCHAR englishName[128];	//英文姓名
    UCHAR gender[8];			//性别
    UCHAR id[32];			//永久居留证号码
    UCHAR areaCode[8];		//国籍或所在区域代码
    UCHAR chineseName[32];	//中文姓名
    UCHAR start_date[18];	//证件签发日期
    UCHAR end_date[18];		//证件终止日志
    UCHAR birthday[18];		//出生日期
    UCHAR versionNumber[6];	//证件版本号
    UCHAR officeNumber[12];	//当次申请授理机关代码
    UCHAR cardType[6];		//证件类型标识
    UCHAR reserved[38];		//预留项
}PACKED HCI_FPRID_IDCARD_INFO_S;

typedef struct HCI_DEV_CITIZEN_IDCARD_INFO_S
{
    UCHAR name[32];			//姓名
    UCHAR gender[8];			//性别
    UCHAR national[16];		//民族
    UCHAR birthday[18];		//出生日期
    UCHAR address[128];		//住址
    UCHAR id[32];			//公民身份证号码
    UCHAR maker[64];			//签发机关
    UCHAR start_date[18];	//有效期开始
    UCHAR end_date[18];		//有效期结束
    UCHAR reserved[48];		//保留
}PACKED HCI_CITIZEN_IDCARD_INFO_S;

// 泰国身份证
typedef struct HCI_DEV_THAI_IDCARD_INFO_S
{
	//注意,泰国身份证的时间均是佛历
	//除英文名和数字外，其他的均为泰文
    UCHAR englishName[128];	//英文姓名
    UCHAR thaiName[128];		//泰文姓名
    UCHAR gender[8];			//性别 0女 1男
    UCHAR id[32];			//身份证号码
    UCHAR start_date[24];	//证件签发日期
    UCHAR end_date[24];		//证件终止日期
    UCHAR birthday[24];		//出生日期
    UCHAR Issuer[64];		//签发机关
	UCHAR address[128];		//地址
    UCHAR reserved[64];		//预留项
}PACKED HCI_THAI_IDCARD_INFO_S;

#define HCI_IDCARD_IMG  38862
typedef struct HCI_DEV_IDCARD_DATA_S
{
	HCI_IDCARD_TPYE IDCard_Type;
	union {
		HCI_CITIZEN_IDCARD_INFO_S	CitizenIDCard; // 居民身份证
		HCI_FPRID_IDCARD_INFO_S		FPRIDCard;		// Foreign Permanent Resident IDCard 外国人永久居留证
		HCI_HMTID_IDCARD_INFO_S		HMTIDCard;		// 港澳台居民居住证
		HCI_THAI_IDCARD_INFO_S		THAIIDCard;
		UCHAR 						szIDCardRes[2048];
	};
	UCHAR	IDCard_Image[HCI_IDCARD_IMG]; // int outLen = (102*3+2)*126+54;
	UCHAR	reserved[128];
}PACKED HCI_IDCARD_DATA_S;


#define HCI_TEMP_WIDTH 	120
#define HCI_TEMP_HEIGHT 160
typedef struct _HCI_TEMP_S{
	FLOAT fMaxTemp;
	FLOAT fMinTemp;
	FLOAT fglobalTemp[HCI_TEMP_HEIGHT][HCI_TEMP_WIDTH];
}PACKED HCI_TEMP_S;

#ifdef __cplusplus
}
#endif

#endif


