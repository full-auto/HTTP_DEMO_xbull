#ifndef HTTPTASK_H
#define HTTPTASK_H

#include <httpselfstudyfacegateconfig.h>
#include <httpschooluseritems.h>
#include <httpincrementuseritems.h>
#include <httpitemsbyid.h>
#include <httpreportstatus.h>
#include <httpcorrection.h>
#include <httpschoolleaveuser.h>

/* 任务分发类，按照轮询回任务类型进行相应操作 */
class HttpTask:public QObject
{
    Q_OBJECT

public:
    HttpTask();
    ~HttpTask();

signals:
    /* 任务结束信号 */
    void task_end();
    /* 给其它线程进行清空数据处理信号 */
    void cleardata();
    /* 界面显示MAC和版本号 学校 */
    void macvis(QString,QString,QString);
    /* 界面显示用户数, 人脸数 */
    void user_num(int,int);
    /* 更新数据进度
     * int更新接口类型:0下发数据-1数据修复
     * int用户类型
     * float进度
     * int 该更新类型更新总数 */
    void updates(int,int,float,int);
    /* 显示全量更新后的提示 */
    void allupdate(int);
    /* 显示增量更新后的提示 */
    void addupdate(int,int,int);
    /* 发送平台的阈值给判断线程使用--开机和设备数据更新时都发送 */
    void send_facethr(float,int,int,int,int);
    /* 门禁规则对应信息，用于发送规则的数据链表给开门线程 */
    void send_rule2(int,QList<QStringList>);
    /* 发送测温参数--给识别结果处理线程 */
    void send_tem_vulan(int,float,int,int,float,int,QString,QString,QString);
    /* 用户数据有变，让门禁备份表一起更新 */
    void send_update_user();
    /* 开门线程更新请假表格数据 */
    void send_update_lrave();

    /* 是否在界面显示调试文本框 */
    void send_show_debug(int);
    /* 在界面显示任务耗时 */
    //
    void sendtime(int,int,int,int);

    /* 发送静音参数 */
    void send_ismute(int);

    /* 刷卡，音箱 相关参数 */
    //是否刷卡
    void send_cardopen(int);
    //否使用音箱
    void send_boxopen(int,int,QString);
    //设备编号[服务器判断]--给服务器线程
    void send_ser_num(int);
    //播报频道  设备编号--串口线程
    void send_channl_num(int,int);

    //发送任务结果给任务反馈线程
    void send_tastover(int,int);

public slots:
    //初始化
    void init();
    void uninit();
    //接收到的任务信息
    void task_info(int id1,int id2,int id3);
    //在更新时，轮询进度
    void update_progress();
    //开机后进行一次版本判断，不同版本就自动更新设备配置信息，用于应对字段有变的情况
    int change_vis();

private:
    //清空任务的处理函数
    void tasks1();

    //设备信息发送槽函数信号的专用函数
    void dev_set_func();

    //上报+数据校正任务的处理函数
    //0成功 1失败
    int status_correcting();

public:
    /* 任务执行的相关请求类 */
    HttpselfStudyFaceGateConfig *h_selfStudyFaceGateConfig;
    HttpschoolUserItems *h_schoolUserItems;
    httpIncrementUserItems *h_IncrementUserItems;
    httpItemsByID *h_ItemsByID;
    httpreportStatus *h_reportStatus;
    httpcorrection *h_correction;
    httpSchoolLeaveUser *h_httpschoolleaveuser;
    //任务
    int id;
    int resource_id;
    int action_id;
    //根据ID获得用户信息的使用接口[只是用于显示的优化]:0下发数据 1数据修复
    int loaduser_tpye;
    //
    bool user_change;//记录执行的任务是否有用户数据更新的

    QTimer *pro_times;//数据更新时，百分比定时在界面更新

    QTimer *v_times;//开机时，定时判断版本号是否一致，不一致就更新设备数据
};

#endif // HTTPTASK_H
