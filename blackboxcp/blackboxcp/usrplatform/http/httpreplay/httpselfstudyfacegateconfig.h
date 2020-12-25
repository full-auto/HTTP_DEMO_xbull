#ifndef HTTPSELFSTUDYFACEGATECONFIG_H
#define HTTPSELFSTUDYFACEGATECONFIG_H

#include <httpperson.h>
/* 获取设备配置信息 */
class HttpselfStudyFaceGateConfig:public HttpPerson
{
    Q_OBJECT

public:
    HttpselfStudyFaceGateConfig();
    ~HttpselfStudyFaceGateConfig();

signals:

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);
    /* 清空数据 */
    void cleardb();
    /* 获得设备基本信息 */
    void getdecinfo();

public:
    QString devname_user;//设备名字--非MAC
    QString school_user;//学校名
    float face_thr;//人脸识别阈值
    int debug_showui;//是否显示下方的消息框
    QString wifi_id;//wifi名称
    QString wifi_key;//wifi密码
    /*门禁规则的链表--用于槽函数发送到开门线程使用,链表排序规则已经固定，请查看插入方式
     *  0开始日期--时间戳
     *  1是否永久
     *  2结束日期--时间戳
     *  3不生效日期--yyyy-mm-dd
     *  4学生标签--全部-住宿-走读...
     *  5家长标签
     *  6职工标签
     *  7详细规则--json数组[week:生效周几，start_time:开始时间--当天秒数，end_time:结束时间--当天秒数，access_type_id:进出类型 0 禁止进出、1 允许进出、2 只进不出、3 只出不进]
     */
    QList<QStringList> rule2_strlist;
    int dopenflag;//0：允许进出；1：禁止进出
    int leaceflag;//是否支持请假通行
    int start_leave;//请假通行时段
    int end_leave;//请假通行时段
    int voice_f;//是否语音播报

    //测温的参数
    int tem_flag;                   //是否测温
    float tem_thr;                  //发烧阈值
    QString tem_ptime;              //不测温时段  开始时间[0]-结束时间[1]  会存在多个时段--json
    QString tem_nousertype;         //不测温用户类型  将以000的格式传过来，顺序分别为学生，家长，职工，请按照对应的用户获取对应的字符进行判断
    QString tem_usertype_timeout;   //可选测温类型--超过 测温等待时长 没有测温也放行;结构同上
    int tem_type;                   //测温类型:0-默认，发烧不放行+上传数据; 1-发烧不放行+不上传数据 2-发烧发行+上传数据;  3-发烧放行+不上传数据
    int tem_pc_flag;                //测温补偿开关，0 关闭，1 开启
    float tem_pc;                   //温度补偿值
    int tem_wait;                   //测温等待时长

    //刷卡，音箱参数
    int card_open;                  //是否开启刷卡
    int hbox_number;                //设备编号，判断使用音箱时，是否为服务端[1的为服务端，其它为客户端]
    int hbox_open;                  //是否使用音箱
    int hbox_cha;                   //音箱频道
    int hbox_angin;                 //播报两次
    QString hbox_notype;            //不播报用户类型

    Global_class *glo;
};

#endif // HTTPSELFSTUDYFACEGATECONFIG_H
