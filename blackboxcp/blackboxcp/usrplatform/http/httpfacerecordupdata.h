#ifndef HTTPFACERECORDUPDATA_H
#define HTTPFACERECORDUPDATA_H

#include <httpselfstudyfacedevice.h>

/* 轮询历史记录表的数据进行上传 */
class httpFaceRecordUpdata:public QObject
{
    Q_OBJECT

public:
    httpFaceRecordUpdata();
    ~httpFaceRecordUpdata();

signals:
    /* 触发数据上传线程执行信号 */
    void emit_sendface(FACE_POST);
    /* 触发图片上传线程执行信号 */
    void emit_sendimg(QString);
    /* 触发测温上传线程执行信号 */
    void emit_sendtem(int,int,float,int);
    /* 触发刷卡上传线程执行信号 */
    void emit_sendcard(QString,int,QString);

    //同步中 已同步 信号
    void emit_syning(int);
    void emit_synall(int);

    //界面查看数据上传状态，测试用
    void emit_updata_flag(int);

    /* 显示用户人脸和信息 人脸的信息  是否测温 */
    void show_faceimg(HCP_DETECT_INFO,int);

    /* 显示测温-门禁的结果 */
    //int 通行类型
    //0正常通行，
    //1门禁阶段，
    //2请假通行，
    //10体温正常-通行
    //11体温正常-门禁阶段[与普通门禁显示一样]，
    //12体温正常-请假通行
    //13体温异常-不通行
    //20体温异常-正常通行
    //21体温异常-正常放行,门禁阶段[与普通门禁显示一样]，
    //22体温异常-正常放行,请假通行,
    //float 体温
    void show_door(int,float);

    //测温结果返回，事件结束
    void quit_tem_event();

    //发送测温信号给串口线程  发送阈值  温度补偿值
    void get_tem(float,float);

    //测温结束
    void stop_tem();

    //发送测温的一些转换好的数据给界面，用于判断是否显示
    //是否测温
    //测温等待时长
    //不测温时段-转换好后的
    void tem_ui(int,int,QList<QStringList>);

    //发送测温 ，门禁的处理耗时给界面显示
    void temdoorDuration(int,int);

    //卡错误信息
    void card_error_info(QString);

    //播报的数据--json
    void send_speakdate(QString);

public slots:
    void init();
    void uninit();

    /* 接收上传结果槽函数，
     * 0-成功，在记录表删除数据
     * 其它错误：不做处理进行下一次轮询
    */
    void senddataquit(int sta);

    /* 接收上传结果槽函数，
     * 0-成功，在记录表删除数据
     * 1-没找到该图片，删除该记录
     * 其它错误：不做处理进行下一次轮询
    */
    void sendimgquit(int sta);

    /* 接收温度上传结果
    * 0  -成功，在记录表删除数据
    * -1 -找不到用户信息，在记录表删除数据
    * 其它错误：不做处理进行下一次轮询
    */
    void sendtemquit(int sta);

    /* 接收刷卡上传结果
    * 0  -成功，在记录表删除数据
    * 1  -参数不对，也在记录表删除数据
    * 其它错误：不做处理进行下一次轮询
    */
    void sendcardquit(int sta);

    /* 轮询需要发送的数据 */
    void send_updata_all();

    /* 接收识别结果并处理 */
    void haveAIfacefunc(FACE_DATA tfd,HCP_DETECT_INFO thdi);

    /* 判断是否测温处理函数
     * param:
     *   t-用户类型
     * return:是否测温 0否 1是
    */
    int tem_func(int t);

    /* 测温对比 t-用户类型 */
    void tem_contrast(int t);

    /* 门禁判断函数
     * temover测温结果
     * 0不测温
     * 1体温正常
     * 2体温异常，但依然通行
     * ps 体温异常不通行的，不会进入此函数
    */
    void door_control_func(int temover);

    /* 清空数据 */
    void cleardb();

    /* 获得平台设备阈值,请假通行，通行时间段等参数 */
    //二次判断阈值  是否支持请假通行 请假通行开始时间  请假通行结束时间  是否语音播报
    void set_thr(float t,int lf,int ls,int le,int vl);

    /* 设置测温参数 */
    /* 是否测温   
     * 发烧阈值  
     * 测温模式 
     * 测温补偿开关   
     * 补偿值   
     * 等待时长 
     * 不测温时间段-json  
     * 不测温用户类型  
     * 测温超时可通行用户类型*/
    void set_tem_num(int t_flag,float t_thr,int t_type,int t_pc_f,float t_pc,int t_wait,QString t_ptime,QString t_no,QString t_out);

    /* 关门 */
    void closedoor_func();

    //接收门禁规则链表数据--mr默认的通行，r_list门禁规则的数据
    void set_rule2(int mr,QList<QStringList> r_list);

    //更新数据表，主要在用户表复制几列数据，用于平时开门时的判断和播报使用,
    //复制新表单独使用是避免在数据更新时，更新线程占有该用户表，此时读导致数据不对
    //不锁是因为开门线程不可以等待太久
    //用户id，类型，标签，标签数组，卡id，播报名称数组
    void update_sql();

    //更新请假用户备份表
    void update_leavesql();

    //接收测温接收信号
    //是否发烧
    //体温结果(含补偿值)
    void have_tem_over(bool fever,float tw);

    //接收音箱开关
    void get_cardbox(int bopen,int anginopen,QString notype);

    //接收串口返回的刷卡信息
    void have_carduser(QString inc_id);

    //音箱播报处理函数
    //用户id 用户类型 播报名称(broadcast_name)  播报位置(broadcast_position_items)   json数据
    void broad_func(int uid,int utype,QString broadname,QString broadnameitems);

public:

    //请假判断-如果没有触发请假发行，就再进入门禁判断  0没触发请假  2触发请假
    int leave_flag(QList<QStringList> start_end);

    //门禁判断函数
    //type-用户类型  user_tag-用户的标签
    //0可以通行  1禁止通行
    int open_flag(int type,QStringList user_tag);

    //测温-门禁处理结束，把处理标识符设回1和重启上传轮询定时器,已经最后的数据表录入
    //int uptype
    //00--人脸结果，体温结果都不录入
    //01--只录入人脸结果
    //10--只录入体温结果
    //11--都录入
    void aifunc_ok();

public:
    QTimer *up_time;    //发送数据的计时器
    bool send_tem_flag; //发送测温数据标识符
    bool send_data_flag;//发送识别数据标识符
    bool send_img_flag; //发送图片标识符
    bool send_card_flag;//发送刷卡标识符
    int synall;         //已经同步时数量--从本次开机后算起
    float thr;          //人脸识别阈值

    QString senddata_path;  //上传数据时的图片路径，作为上传结束的删除查找参数
    QString sendimg_path;   //上传图片时的图片路径，作为上传结束的删除查找参数

    bool old_aifunc;//上一次的识别结果是否处理完毕  0处理中,新来的识别结果忽略  1空闲中
    string old_id;  //上次识别的ID-- AI库内的id
    int old_captime;//上次识别有进行录入的时间
    int nospeak;    //连续同一个人的识别间隔，需要测温的3秒，不用的1秒

    //记录刚刚上传的用户id和类型
    int up_id;
    int up_type;

    //通行请求的类型 1刷脸  2刷卡，用于在最后判断是记录到刷脸记录，还是刷卡记录
    int cometype;

    //卡参数
    QString card_number;//最新的卡ID，用于再刷卡记录时记录使用
    int card_timep;//记录上传刷卡数据对应的刷卡时间

    //
    FACE_DATA this_fd;          //识别记录的数据
    Global_class *glo;
    QTimer *closedoor_time;     //关门用的定时器--考虑到硬件的反应时间，会比设定是时间更长点
    int dopen_flag;             //没门禁时。默认的通行设置
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
    QList<QStringList> rule2_list;  //门禁规则的链表--用于槽函数发送到开门线程使用,链表排序规则已经固定，请查看插入方式
    int leave_ok;                   //是否支持请假通行
    int leave_s;                    //请假通行开始时间
    int leave_e;                    //请假通行结束时间
    int is_voi;                     //是否语音播报

    //测温的参数
    int tem_flag;                   //是否测温
    float tem_thr;                  //发烧阈值
    QList<QStringList> tem_ptime;   //不测温时段  开始时间[0]-结束时间[1]  会存在多个时段
    QString tem_nousertype;         //不测温用户类型 --json
    QString tem_usertype_timeout;   //可选测温类型--超过 测温等待时长 没有测温也放行;结构同上
    int tem_type;                   //测温类型:0-默认，发烧不放行+上传数据; 1-发烧不放行+不上传数据 2-发烧发行+上传数据;  3-发烧放行+不上传数据
    int tem_pc_flag;                //测温补偿开关，0 关闭，1 开启
    float tem_pc;                   //温度补偿值
    int tem_wait;                   //测温等待时长

    float tw_v;                     //测温结果
    bool tem_over;                  //是否通过测温，进行门禁判断  0体温正常  1发烧
    QTimer *tem_time;               //测温的计时器
    QEventLoop *tem_eventloop;      //测温对应的事件
    int tem_uptime;                 //记录上次上传时的测温对应时间戳，用于删除使用

    //记录上传类型
    //00--人脸结果，体温结果都不录入
    //01--只录入人脸结果
    //10--只录入体温结果
    //11--都录入
    //每次开始清零，要记录体温+10，记录人脸+1
    int update_type;

    int temDuration;//记录测温函数时长
    int doorDuration;//记录门禁函数时长

    //音箱
    int vbox_open;//音箱开关
    int angin_Broad;//是否重复播报
    QString no_Broadtype;//不播报用户类型
    QList<QStringList> last_usedid_time;//记录发送过语音的用户id和时间  一个QStringList由id和time组成
};

#endif // HTTPFACERECORDUPDATA_H
