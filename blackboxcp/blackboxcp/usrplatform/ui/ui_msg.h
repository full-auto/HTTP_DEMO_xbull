#ifndef _UI_MSG_H_
#define _UI_MSG_H_


#include <QThread>
#include <QMetaType>

#ifndef PACKED
#define PACKED		__attribute__((packed, aligned(1)))
#define PACKED4		__attribute__((packed, aligned(4)))
#endif


#define UI_MSG_KEY  0x8024
#define UI_MEM_KEY  0x8048
#define UI_MSG_HCP  0x88
#define UI_MSG_UI   0x89

#define UI_SHARE_MEM_SIZE 1920*1080*3/2+16

typedef enum _HCP_RESOLUTION_E
{
    E_RESOLUTION_800_1280			= 0x00,
    E_RESOLUTION_720_1280			= 0x01,
    E_RESOLUTION_600_1024			= 0x02,

}HCP_RESOLUTION_E;

typedef enum _HIFACE_PLATFORM_TYPE_E_UI
{
    E_PLATFORM_HIFACE_UI			= 0x00, //HIFACE
    E_PLATFORM_DS_UI				= 0x01,//大树
    E_PLATFORM_TM_UI				= 0x02,//天瞐

    E_PLATFORM_UNAVAILABLE_UI	= 500,

}HIFACE_PLATFORM_TYPE_E_UI;

typedef enum _HCP_FD_MSG_TYPE_E
{
    HCP_MSG_INIT,                           //UI 启动初始化界面，等待IPC 初始化完成后进入视频界面
    HCP_MSG_INIT_READY,                      //IPC 启动完成
    HCP_MSG_FACE_DETECT,						// UI 通知ipc进入闸机模式
    HCP_MSG_FACE_DETECT_ACK,					// ipc 发送人脸信息给ui
    HCP_MSG_FACE_REGISTER,						// UI 通知ipc进入人脸注册模式，
    HCP_MSG_FACE_REGISTER_ACK,					// 人脸注册模式，人脸模式需要填充FaceStatus内容
    HCP_MSG_FACE_,								// UI 注册新的人脸，传入姓名等信息到ipc
    HCP_MSG_SET_FACE_CTL,					//ipc通知UI 修改测温区域, 报警温度等
    HCP_MSG_SET_ENVIR_TEMP,				//ipc 通知UI 当前环境温度
    HCP_MSG_HEARTBEAT,
    HCP_MSG_DEV_ONLINE,                 // IPC在线状态
    //...


}HCP_FD_MSG_TYPE_E;

typedef enum _HCP_PLAT_TYPE_E
{
    HCP_PLAT_HIFACE,
    HCP_PLAT_DASHU,  // 大树
    HCP_PLAT_FENGNIAO,  // 蜂鸟
    HCP_PLAT_TENNIU,    // 十牛
}HCP_PLAT_TYPE_E;

typedef enum _HCP_FACE_STUTES_TYPE_E
{
    HCP_FS_GOODE,
    HCP_FS_BIG,                             // 人脸太大
    HCP_FS_SMALL,							// 人脸太小
    HCP_FS_OVER_BRIGHT,						// 太亮
    HCP_FS_DARK,							// 太暗

    //...


}HCP_FACE_STUTES_TYPE_E;


typedef struct _HCP_FaceInfo {

    unsigned char bGetFace;             //是否有人脸进入
    unsigned char bLiving;              //是否活体
    unsigned char bDetected;            //是否识别（识别是否大于阈值）
    unsigned char reserved;            //
    HCP_FACE_STUTES_TYPE_E FaceStatus;     //不能识别的原因

    unsigned int  nPosX;
    unsigned int  nPosY;
    unsigned int  nWidth;
    unsigned int  nHeight;

    char data[256];         //识别到的用户姓名，如未能识别发送“unknow"
}PACKED HCP_FaceInfo;


typedef struct _msg_buf {

    int msg_to;    //
    HCP_FD_MSG_TYPE_E type;      //消息类型

    HCP_FaceInfo FaceInfo;
}msg_buf;


typedef struct _HCP_ShareMem {
    int width;      //picture width
    int height;     //picture height
    int bpp;        //bit per pixel, 注意是bit，要转换为byte 需要除以8
    int reserved;
    char buf[];
}HCP_ShareMem;

typedef struct _HCP_DETECT_INFO
{
    char szPath[128]; // 抓拍图片保持位置
    char szName[32]; // 抓拍人员姓名
    char szInfo[64]; // 抓拍人员相关信息
    int  iKnow; // 比对是否成功
}HCP_DETECT_INFO;
Q_DECLARE_METATYPE(HCP_DETECT_INFO);

#endif // _UI_MSG_H_
