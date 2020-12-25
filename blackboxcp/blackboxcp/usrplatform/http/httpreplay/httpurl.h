#ifndef HTTPURL_H
#define HTTPURL_H

#include <string>

/* 通用QT模块 */
#include <QObject>
/* QT网络模块 */
/* 注意  在url的附带参数中，qt5和qt4的有些区别 */
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QNetworkInterface>
#include <QUrl>
/* QT文件操作模块 */
#include <QFile>
#include <QDir>
/* QT事件，线程，定时器 */
#include <QEventLoop>
#include <QTimer>
#include <QDateTime>
#include <QThread>
/* QT数据库模块 */
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

#include <QTextCodec>
/* json，qt自带的json模块需要qt5.0以上 */
#include <json/json.h>
/* 加密类 */
#include <QCryptographicHash>
/* 互斥锁 */
#include <QMutex>
#include <QMutexLocker>
/* 自定义结构体信号 */
#include <QMetaType>
/* 开发板主程序对接模块 */
//#include <hciConfig.h>
#include <hcmComInfo.h>
#include <hci_def.h>
#include <hci_intf.h>
#include <hci_struct.h>

/* 人脸注册图片保存路径 */
#define FACEIMG_PATH "/disk/faceimg/"
/* 人脸识别结果图临时保存路径 */
#define FACE_RESULT_PATH "/disk/faceimgresult/"
/* 数据库路径 */
#define DB_PATH "/disk/xbulldata/"
/* 运行环境对于的URL 1测试，2灰度，3正式 */
#define BASE 3

using namespace std;

/* 人脸识别后传输的数据 AI到数据上传线程 */
typedef struct FACE_DATA
{
    int capture_time;   //抓拍时间戳  秒级别
    string user_id;     //AI模块的用户id，注：该id为平台上用户id+类型(id+type)组合
    int identify_status;//识别结果 1成功 2失败
    float rate;         //结果阈值
    string imgpath;     //抓拍图片路径
}FACE_DATA;
Q_DECLARE_METATYPE(FACE_DATA);

/* 数据发送时需要的部分参数 */
typedef struct FACE_POST
{
    int capture_time;       //抓拍时间戳 秒级别
    QString picture_path;   //抓拍图片路径
    int user_id;            //用户id
    int user_type;          //用户类型
    int identify_status;    //识别结果 1成功 2失败
    float rate;             //结果阈值
}FACE_POST;
Q_DECLARE_METATYPE(FACE_POST);

/* 运行环境 */
typedef struct  BASE_URL
{
    //测试环境域名
    const string BASE_URL_TEST = "http://ps.10niu.cn/PrimarySchool-develop/Api/";

    //灰度环境域名
    const string BASE_URL_GRAY = "http://www.x-bull.com/pre-release2/Api/";

    //正式环境域名
    const string BASE_URL_PRODUCT = "http://www.x-bull.com/v2/Api/";
}BASE_URL;

/* 请求网址接口汇总 */
typedef struct HTTPURL
{
	/*
    * 获取人脸闸门百度激活密钥 get
    * 非百度的用不到
	*/
    //const string sgetBaiduActivationKeyToFaceGateDevices = "V2Device/getBaiduActivationKeyToFaceGateDevices";

	/*
    * 人脸识别数据接入 post
	*/
    const string faceGateSwipeCard = "Attendance/faceGateSwipeCard";
	
	/*
    * 获取阿里云token授权 get
	*/
    const string selfStudyFaceDevice = "Sts/selfStudyFaceDevice";

    /*
    * 获取上传oss图片token授权 get
    */
    const string selfImageFace = "Picture/stsApi";
	
	/*
    * 获取设备配置信息 get
	*/
    const string selfStudyFaceGateConfig = "V2Device/selfStudyFaceGateConfig";
	
	/*
    * 根据设备获取学校用户列表 get
	*/
    const string schoolUserItems = "V2Device/schoolUserItems";
	
	/*
    * 获取设备任务 get
	*/
    const string taskReserve = "V2Device/taskReserve";
	
	/*
    * 获取学生信息 get
	*/
    const string schoolStudent = "V2Device/schoolStudent";
	
	/*
    * 获取职工信息 get
	*/
    const string schoolStaff = "V2Device/schoolStaff";
	
	/*
    * 获取家长信息 get
	*/
    const string schoolParent = "V2Device/schoolParent";
	
	/*
    * 上报闸机状态 post
	*/
    const string reportStatus = "V2Device/reportStatus";
	
	/*
    * 获取学生信息列表 get
	*/
    const string schoolStudentItems = "V2Device/schoolStudentItems";
	
	/*
    * 获取家长信息列表 get
	*/
    const string schoolParentItems = "V2Device/schoolParentItems";
	
	/*
    * 获取家长信息列表 get
	*/
    const string schoolStaffItems = "V2Device/schoolStaffItems";
	
	/*
    * 获取访客信息 get
	*/
    const string schoolVisitor = "V2Device/schoolVisitor";
	
	/*
    * 设备心跳接口，用于检测设备是否联网 get
	*/
    const string heartbeat = "V2Device/heartbeat";
	
	/*
    * 学生数据修复 post
	*/
    const string correctionStudent = "V2Device/correctionStudent";
	
	/*
    * 家长数据修复 post
	*/
    const string correctionParent = "V2Device/correctionParent";
	
	/*
    * 职工数据修复 post
	*/
    const string correctionStaff = "V2Device/correctionStaff";
	
	/*
    * 访客数据修复 post
	*/
    const string correctionVisitor = "V2Device/correctionVisitor";
	
	/*
    * 学校用户增量更新接口 get
	*/
    const string incrementUserItems = "V2Device/incrementUserItems";
	
	/*
    * 根据学生ID集合获取学生数据 post
	*/
    const string schoolStudentItemsByIdItems = "V2Device/schoolStudentItemsByIdItems";
	
	/*
    * 根据家长ID集合获取家长数据 post
	*/
    const string schoolParentItemsByIdItems = "V2Device/schoolParentItemsByIdItems";
	
	/*
    * 根据职工ID集合获取职工数据 post
	*/
    const string schoolStaffItemsByIdItems = "V2Device/schoolStaffItemsByIdItems";
	
	/*
    * 根据访客ID集合获取访客数据 post
	*/
    const string schoolVisitorItemsByIdItems = "V2Device/schoolVisitorItemsByIdItems";

    /*
     *获得请假用户信息 get
     */
    const string schoolLeaveUser = "V2Device/schoolUserAbsences";

    /*
    * 设备任务反馈 post
    */
    const string taskRelease = "V2Device/taskRelease";

    /*
     *获得人脸数据上传的token get
     */
    const string getToken = "Common/getAccessToken/number/";

    /*
     *获得服务器时间 get
     */
    const string getservertime = "Common/getServerTime/";

    /*
     *获得升级信息 get
     */
    const string getUpgrade = "Device/getUpgrade";

    /*
     *上传测温数据 post
     */
    const string temperature = "V2Device/temperature";

    /*
     * 上传刷卡记录 post
     */
    const string swipeCard = "Attendance/swipeCard";
}HTTPURL;

/* 阿里云服务器相关的参数，包括url，backet等 */
typedef struct HTTPALIYUNURL
{
    //测试环境backet
    const string ALY_BASE_BACKET_TEST = "xbulltest";

    //正式环境backet
    const string ALY_BASE_BACKET_PRODUCT = "xbull";

    //OSS功能对应的url
    const string ALY_BASE_URL_OSSURL="oss-cn-shenzhen.aliyuncs.com";
}HTTPALIYUNURL;


#endif // HTTPURL_H
