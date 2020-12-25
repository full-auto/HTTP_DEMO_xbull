#ifndef _HCI_HCMCOMINFO_H
#define _HCI_HCMCOMINFO_H


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


//页面控件上传名单库图片
#define FACE_UPDATA_PICTURE					15

//黑白名单图片接收状态
#define RECEIVE_FILE_IDLE					0
#define RECEIVE_FILE_START					1
#define RECEIVE_FILE_END					2

#define MAX_FEATUR_LEN						256		//人脸特征值长度 st


//名单模式
typedef enum _WBLIST_MODE
{
	FACE_LISTNULL = 0,
	FACE_BLACKLIST = 1,
	FACE_WHITELIST = 2,
	FACE_VIPLIST = 3,
	FACE_TEMPONE = 4,
	FACE_TEMPTWO = 5,
	FACE_LISTMAX,
}WBLIST_MODE;


//协议对黑白名单操作
typedef enum _HCM_MSG_OPERATION_MODE
{
    IN_SQLITE_PICTURE_ADD = 1,		//通过导入图片添加一张人脸库1
    IN_PICTURE_GET_FEATURE,			//通过导入一张图片获取特征值2
    OUT_SQLITE_LISTINFO, 	 		//导出数据库所有列表信息3
    OUT_SQLITE_PICTURE_DELONE,	 	//删除数据库中单张图片4
    OUT_SQLITE_PICTURE_DELLIST,	 	//删除名单列表5
    OUT_SQLITE_USER_INFO,   		//导出用户详细信息6
    IN_SQLITE_USER_INFO_UPDATA,	 	//用户信息更新7
    IN_SQLITE_FEATURE_ADD, 			//通过特征值添加一张黑白名单8
    OUT_FACEREC_INFO ,				//人脸记录查询9
    OUT_CHECKON_INFO ,				//考勤导出10
    OUT_PHYLINK_STATUS,				//平台网络连接状态11
    IN_SQLITE_CARD_ADD,				//卡号添加12
    IN_SNAPCUR_PIC_PATH,			//获取当前抓拍图片13
    IN_UPLOADE_IDFACE,				//下发人脸比对信息14
    OUT_SQLITE_UNSEND_REC,			//获取未发送数据15
    IN_SQLITE_SET_REC,				//设置发送数据16
    OUT_FACE_CMP_PIC = 1000,		//人脸比对图片17
}HCM_MSG_OPERATION_MODE;




/*人脸属性类别*/
typedef enum _FACE_ATTRIBUTE_CATEGORY_E
{
	ATTR_NONE 		= 0x0,
	ATTR_MAN  		= 0x1,
	ATTR_WOMAN  		= 0x2,
	ATTR_GLASSES  	= 0x3,
	ATTR_SUNGLASSES	= 0x4,
	ATTR_SMILE 		= 0x5,
	ATTR_MASK		= 0x6,
	ATTR_YELLOW		= 0x7,
	ATTR_BLACK		= 0x8,
	ATTR_WHITE		= 0x9,
	ATTR_EYE_CLOSE	= 0xa,
	ATTR_EYE_OPEN	= 0xb,
	ATTR_MOUTH_CLOSE=0xc,
	ATTR_MOUTH_OPEN= 0xd,
	ATTR_BEARD		= 0xe,
	ATTR_ANGRY		= 0xf,
	ATTR_CLAM		= 0x10,
	ATTR_DISGUST		= 0x11,
	ATTR_HAPPY		= 0x12,
	ATTR_SAD 			= 0x13,
	ATTR_SCARED		= 0x14,
	ATTR_SQUINT 		= 0x15,
	ATTR_SCREAM 		= 0x16,
	ATTR_SURPRISED	= 0x17,
}FACE_ATTRIBUTE_CATEGORY_E;


typedef enum _HCM_ANALYSE_TYPE_E
{
	ANALYSE_NONE_E			= 0x0,
	ANALYSE_START_E			= 0x1,
	ANALYSE_FACEINFO_E		= 0x2, //识别成功信息
	ANALYSE_CMPFAIL_E		= 0x3, //识别失败信息
	ANALYSE_SNAP_E			= 0x4, //抓拍模式下, 人脸抓拍信息
	ANALYSE_ENVTEMP_E		= 0x5, // 环境温度  float
	ANALYSE_SQILTE_E		= 0x6, //底库数据 HCM_SQLITE_COUNTINFO
	ANALYSE_SCREENPROTECT_E = 0x7, //屏保 HCM_SCREENPROTECT
	ANALYSE_FACEID_CHANGE_E = 0x8, //人脸ID 发生变化
	ANALYSE_FACEID_INFO_E 	= 0x9, //人脸ID 信息 HCM_FACE_IDINFO
	ANALYSE_FACE_SWIPING	= 0x10, //请刷脸
	ANALYSE_FACE_NOTIME     = 0x11,  //不在时间范围内
	ANALYSE_FACE_IDCARD		= 0x12, //请刷身份证
	ANALYSE_FACE_CARD		= 0x13, //请刷卡
	ANALYSE_FACE_CEWEN		= 0x14, //请测温
}HCM_ANALYSE_TYPE_E;


typedef enum _AI_STARTSTATUS_TYPE_E
{
	AI_STARTING_E				= 0x0, //启动中
	AI_START_SUCESS_E			= 0x1, //启动成功
	AI_START_FAIL_E				= 0x2, //启动失败

}AI_STARTSTATUS_TYPE_E;



typedef enum HCM_ERROR_CODE
{
	HCM_OK 						= 0,		//成功
	HCM_FAILE 					= -1,		//失败
	HCM_GETFRATURE_ERROR 		= -100,		//提取特征值失败
	HCM_FILEINDEX_ERROR 		= -101,		//文件名字编号重复
	HCM_SQLITE_FULL 			= -102,		//库满
	HCM_TIME_OUT	 			= -103,		//添加超时
	HCM_PARAM_ERROR				= -104,		//参数错误
	HCM_FILE_BIG				= -105,		//文件太大
	HCM_SPACE_ERROR				= -106,		//存储空间不足
	HCM_FILE_OPEN_ERROR			= -107,		//文件打开失败
	HCM_NO_SQLITE_FILE			= -108,		//没有数据库
	HCM_READ_FILE_ERROR			= -109,		//图片读取失败
	HCM_SQLITE_FILE_BAD			= -110,		//数据库文件损坏
	HCM_PIC_QUALITY_LOW 		= -111, 	//图片质量差
	HCM_PIC_SIZE_ERROR 			= -112, 	 // 图片尺寸错误，宽高不能为奇数
	HCM_DETECT_FACE_ERROR		= -113,  	//检测人脸失败（没检测到人脸或者检测到多人脸）
	HCM_PIC_FORMAT_ERROR 		= -114,   	//图片格式错误
	HCM_FACE_AREA_FAIL			= -115, 	//人脸区域错误
 	HCM_FACE_HAND_ERROR			= -116,	 	//算法创建句柄错误
 	HCM_FACE_BUSY				= -117, 	//设备忙
 	HCM_FILE_WRITE_ERROR		= -118,		//文件写入失败
 	HCM_FILE_DELETE_ERROR		= -119,		//删除失败(未找到对应的ID删除)
	HCM_MEMORY_ALLOCATE_FAIL	= -120,		//分配内存失败
	HCM_LISTE_IS_NULL			= -121,		//名单库人数为空
	HCM_TIME_ERROR 				= -122,		//   有效时间错误
	HCM_WRITEFRATURE_ERROR		= -123,		//特征值写入失败
	HCM_AI_UNINIT				= -124,		//ai初始化未完成
}HCM_ERROR_CODE;



typedef enum _HCM_TEMP_MODULES
{
	HCM_TEMP_NONE 	= 0,
	HCM_TEMP_DS	  	= 1, //大树温度模块
	HCM_TEMP_MSA   	= 2, //天瞐定制温度模块
	HCM_TEMP_TENNIU_HAND = 3, //十牛手腕定制温度模块
	HCM_TEMP_HAIMAN = 4, //海曼测温
	HCM_TEMP_HIK = 5, //海康测温
}HCM_TEMP_MODULES;



//精确到微妙抓拍时间   8byte
typedef struct _AI_SNAP_TIME_USEC_S
{
	struct timeval stTimeBuf;

}AI_SNAP_TIME_USEC_S;


typedef struct _AI_POSITION_S
{
	INT left;
	INT top;
	INT right;
	INT bottom;
}AI_POSITION_S;


typedef struct _AI_SQILE_USERINFO { //2048
	INT 	sqlistid;					//系统ID 在添加时不用赋值，由系统生成
	INT 	dirindex;					//库编号 在添加时不用赋值，由系统生成
	INT 	fileindex;					//图片索引编号 在添加时不用赋值，由系统生成
	UCHAR 	enable;						//使能开关
	UCHAR  	gropindex;					//组
	UCHAR 	timemode;					//名单有效期 0 永久有效，1 周期有效，有效时间参照sTime和 eTime
	UCHAR  	listmode;					//名单模式: 1 黑名单，2白名单，3 VIP名单        	 , 4 tempone, 5 temptwo, WBLIST_MODE
	UINT  	count;						//统计次数
	INT  	sex;						//性别  1 男   2 女
	CHAR    usrid[36];					//用户ID或者编号
	CHAR    idcard[32];					//身份ID
	CHAR 	name[64];					//姓名
	CHAR  	picname[64];				//图片名   系统生成，不支持修改
	CHAR    nation[12];					//民族
	CHAR 	birth[24];					//生日
	CHAR    phone[16];					//号码
	CHAR    areacode[32];				//区域编码
	CHAR	usrcode1[32];				//班级
	CHAR	usrcode2[32];				//小区编码
	CHAR 	type[12];					//类型
	CHAR    dTime[20];					//入库时间
	CHAR    sTime[20];					//开始时间
	CHAR    eTime[20];					//结束时间
	UINT	FirstCmptime;				//第一次比对时间
	UINT	UpdateCmptime;				//更新比对时间
	UINT	SysRunTime;					//系统运行时间

	INT		usrparam1;					//卡号

	INT		usrparam2;					//备用参数2
	INT		usrparam3;					//备用参数3
	INT		usrparam4;					//备用参数4

	CHAR	resID[32];					//房屋ID	   //备用ID
	CHAR	areaAddress[100];			//区域地址
	CHAR	resname[32];				//房屋名称 //备用名称
	CHAR  	usrdata1[32];				//用户备用数据1,
	CHAR  	usrdata2[24];				//用户备用数据2
	CHAR  	usrdata3[32];				//用户备用数据3
	CHAR  	usrdata4[36];				//用户备用数据4
	union
	{
		CHAR  	usrdata5[88];				//用户备用数据5
		CHAR	downloadName[88];
	};
	CHAR 	remark[180];				//标签

	FLOAT feature[MAX_FEATUR_LEN];//特征值

}PACKED AI_SQILE_USERINFO;




typedef struct _AI_COMPARE_REC_INFO_S		//size = 1802
{
	UINT   			deviceid;		//预留
	UINT   			channelid;		//预留
	UINT   			streamid;		// rowid
	UINT   			wbmode;			//黑白名单   // WBLIST_MODE
	UINT   			sqdirindex;		//黑白名单库索引
	UINT   			sqfileindex;	//黑白名单库里编号
	CHAR   			sqpicname[64];	//人脸库图片名字
	CHAR   			snapname[64];	//实时比对图片名字
	FLOAT			result;		    //比对结果 0.0 ~ 99.9
	CHAR    		SnapTime[32];	////抓拍时间
	UINT   			alarmcmd;
	CHAR			snapPicPath[128]; //当前抓拍人脸图路径
	UINT   			facequality;	//人脸质量分数
	UINT   			guest;			//客流统计
	UCHAR 			Age;      		//年龄
	UCHAR 			sex;			//性别
	UCHAR 			Glasses; 		//眼镜
	UCHAR 			Mask; 			//面具 口罩
	UCHAR 			Race;			//种族
	UCHAR 			Eye;			//眼睛
	UCHAR 			charm;			//魅力值
	UCHAR 			beard;			//
	UCHAR 			emotion;		//表情
	UCHAR			res[3];
	CHAR    		CompareTime[32];	////比对时间
	UCHAR  			 unauthorized;	////未认证标志
	CHAR			audiotype ; // 0 pass  1 faile	2 ti wen  3 kou zao
	UCHAR			temper_ByRes[2];
	FLOAT 			temperature;//人体温度     温度为0 不显示
	FLOAT 			envir_temperature;//周边环境温度
	UCHAR   		gropindex;	//组号
	UCHAR			uiDisplayMode; //0 显示当前抓拍  ; 1 显示底图
	UCHAR			ByRes[62];			//预留

	FLOAT			FaceFeature[256];	//人脸特征值

	CHAR			backgroundPicPath[128];	//背景图路径
	AI_POSITION_S 	stFacePos;                  //人脸坐标
	INT     		i_sex;
	INT 			timemode;					//限制  有效或无效
	CHAR    		usrid[36];					//用户ID或者编号
	CHAR    		idcard[32];					//身份ID
	CHAR 			name[64];					//姓名
	CHAR    		nation[12];					//民族
	CHAR 			birth[24];					//生日
	CHAR   			phone[20];					//号码
	CHAR			resname[32];					//房屋名称
	CHAR			resID[32];					//房屋ID
	CHAR   		 	areacode[32];				//区域编码
	CHAR			areaAddress[100];			//区域地址
	CHAR			usrcode1[32];				//班级
	CHAR			usrcode2[32];				//小区编码
	CHAR 			type[12];					//类型
	CHAR   		 	dTime[20];					//入库时间
	CHAR    		sTime[20];					//开始时间
	CHAR    		eTime[20];					//结束时间
	UINT			FirstCmptime;				//第一次比对时间
	UINT			UpdateCmptime;				//更新比对时间
	INT				SysRunTime;					//系统运行时间
	INT				usrparam1;					//卡号
	INT				usrparam2;					//备用参数2
	INT				usrparam3;					//备用参数3
	INT				usrparam4;					//备用参数4
	CHAR  			usrdata1[32];				//用户备用数据1
	CHAR  			usrdata2[24];				//用户备用数据2
	CHAR  			usrdata3[32];				//用户备用数据3
	CHAR  			usrdata4[36];				//用户备用数据4
	CHAR  			usrdata5[88];				//用户备用数据5
	CHAR 			remark[180];				//标签
	AI_SNAP_TIME_USEC_S stSnapUsecTime; 		//抓拍时间
	CHAR			idCardPicPath[128];			//证件图路径
	CHAR			cres[1024];
}PACKED AI_COMPARE_REC_INFO, *PAI_COMPARE_REC_INFO;




//考勤：考勤记录查询，查询返回_SMART_CUSTOM_ATTEND_STATISTICS数组512
typedef struct _SMART_CUSTOM_ATTEND_STATISTICS //1024
{
	//如果是查询当天的时间只显示考勤时间,其他是显示统计上班情况次数
	INT			i_iChannel;					//通道

	CHAR		i_sStartTime[32];			//开始时间：日期有效
	CHAR		i_sEndTime[32];				//结束时间：日期有效

	CHAR		i_sName[64];				//名称(可选)
	CHAR		i_sUserID[36];				//用户ID(可选)
	INT			i_iPage;					//第几页，从第1页开始，0的话表示只返回数量，不返回信息
	INT			i_iPageCount;				//每页多少数量
	INT			i_iQueryOrPaged;			//查询还是翻页  1:查询2:翻页
	INT 		i_iQueryStatus;    			//通过状态查询信息
	INT 		i_sLibMode;					//
	INT			i_iQuerySomedayOrStatistic; //查询某天信息还是统计信息
	CHAR		i_sRes[824];

	INT			o_iTotalCount;				//总数数量
	INT			o_iCount;					//输出数量
}PACKED SMART_CUSTOM_ATTEND_STATISTICS;

//人脸记录信息，结构体后是人脸图片数据416
typedef struct HCM_RECORD_QUERY_INFO_
{
	INT			sLibRowid;				//库行号
	FLOAT		sLibSimilarity;			//相似度

	CHAR		sName[64];				//名称(可选)
	CHAR		sUserID[36];				//用户ID(可选)
	CHAR		sLibSnapTime[32];		//抓拍时间
	INT 		sLibChannelid;			//通道号
	INT			sLibMode;				//名单	类型
	INT			sSysListID;				//系统ID
	INT 		sqdirindex;				//目录

	INT 	  	nLibGuest;				//客流统计
	CHAR		cLibAge;      			//年龄
	CHAR  		cLibGender;				//性别
	CHAR		cLibGlasses; 			//眼镜
	CHAR		cLibMask; 				//面具
	CHAR		cLibRace;				//种族
	CHAR		cLibEye;				//眼睛
	CHAR		cLibGroup;			//组号
	CHAR		cLibBeard;				//记录断网续传标识
	CHAR		cLibEmotion;			//表情
	CHAR		TemperatureStatus;		//体温状态 0 1
	CHAR		sRes[2];
	CHAR		sLibCpicname[128];		//实时比对成功时抓拍的图片名称路径
	CHAR		sBackPicName[128];		//背景图片名称
	CHAR		sBodyPicName[128];		//人形图片名称 或 证件图片
	CHAR		sFirstCmpTime[32];		//首次比对时间
	FLOAT		BodyTemperature;		//体温
	INT			exlen;
	CHAR		pdata[0];
	UINT	    cardNum;				//卡号
	CHAR        szIDCard[32];			//证件号
	UCHAR		slibsnapname[64];			//实时比对成功时抓拍的图片名称
	CHAR 		res[1024-36 - 64];				//预留1024
}PACKED HCM_RECORD_QUERY_INFO;


//比对：记录查询，查询数据返回HCM_RECORD_CONDITION数组信息 1024
typedef struct HCM_RECORD_CONDITION_
{
	INT				i_iChannel;					//通道
	CHAR			i_sStartTime[32];			//开始时间：日期有效
	CHAR			i_sEndTime[32];				//结束时间：日期有效

	CHAR			i_sName[64];				//名称(可选)
	CHAR			i_sUserID[36];				//用户ID
	INT				i_sLibMode;					//0:陌生人,1:黑,2:白,3:Vip 名单        WBLIST_MODE,  0xFF 所有
	INT				i_iPage;					//第几页，从第1页开始，0的话表示只返回数量，不返回信息
	INT				i_iPageCount;				//每页多少数量
	INT				i_iQueryOrPaged;			//查询还是翻页  1:查询2:翻页
	CHAR			i_sRes[832];
}PACKED HCM_RECORD_CONDITION;


typedef struct HCM_RECORD_RESULTS_
{
	HCM_RECORD_QUERY_INFO *pCmpRecInfo;
	INT				o_iTotalCount;				//总数数量
	INT				o_iCount;					//输出数量

}PACKED HCM_RECORD_RESULTS;



typedef struct _AI_SQILE_CONFIG		//  2560
{
	INT						i_iOpMode;			//名单库操作模式，内容见WBLIST_OPERATION_MODE值
	INT						i_iWbmode;			//名单模式，内容见WBLIST_MODE值
	INT						i_iOpFlag;			//操作唯一标识:0 通过i_sUserID来操作，1 通过i_iSyslistID来操作，默认0
	INT						i_iSyslistID;		//名单系统ID;
	CHAR					i_sUserID[36];		//用户ID，唯一标识
	CHAR					i_sFaceID[36];		//人脸ID，用于表示同一用户多张人脸ID,如果不需要的话就不用赋值
	INT						i_iBodyLen;			//配置结构体长度，主要是指sdk_fileindex_info_Ex1长度，如果为0，则没有该参数

	INT						o_ErrorCode;		//错误码
	INT						o_LibIndex;			//库索引
	INT						o_FileIndex;		//文件索引
	INT						o_AllNums;			//实际库数目
	INT						o_syslistid;
	INT 					o_FeatureType;		//特征值类型  0 FLOAT,1 CHAR ,2 INT
	INT						o_DataSize;
	INT 					i_IsUpFeature;
	FLOAT					o_Similarity;		//相似度
	CHAR					i_sRes0[96];		//预留

union
{
	AI_SQILE_USERINFO   	i_sUserInfo;		//用户名单详细信息，在添加、更新时候需要添加该信息
	SMART_CUSTOM_ATTEND_STATISTICS		i_Checkon_Out;//考勤导出
	HCM_RECORD_CONDITION			i_CmpInfo_Out; //比对记录导出
};


	CHAR					i_sRes[288];		//后期可用于sdk_fileindex_info_Ex1扩展
}PACKED AI_SQILE_CONFIG;



typedef struct _HCM_SQLITE_OPERATE
{
	CHAR *pPicPath;
	CHAR* pImgBuf;
	INT nImgBufPiclen;
	CHAR  szMsg[64];
	AI_SQILE_CONFIG stSqConfig;

}HCM_SQLITE_OPERATE;



typedef struct _HCM_SQLITE_COUNTINFO
{
	UINT nMaxSqliteCount;
	UINT nCurSqliteCount;
	CHAR  szRes[64];
}HCM_SQLITE_COUNTINFO;



typedef struct _HCM_SCREENPROTECT
{
	UCHAR nScreenStatus; //0 取消屏保, 1 进入屏保状态
	UCHAR nScreenTime; //屏保刷新时间 (1~200S)
	CHAR  szRes[64];
}PACKED HCM_SCREENPROTECT;


//ANALYSE_FACEID_INFO_E
typedef struct _HCM_FACE_IDINFO_S
{
	INT id;
	INT confidence;
	INT quality;
	FLOAT ftemp;
	AI_POSITION_S stPosition;
}PACKED HCM_FACE_IDINFO;


#ifdef __cplusplus
}
#endif

#endif


