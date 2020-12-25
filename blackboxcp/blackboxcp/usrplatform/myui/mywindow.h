#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <QBitmap>
#include <QPainter>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QThread>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QImage>
#include <QPixmap>
#include <QPalette>
#include <QRect>

#include <QWSServer>
#include <QTextCodec>

#include <QNetworkInterface>
#include <QNetworkConfigurationManager>
#include <QHostInfo>

//#include <QFontDatabase>

#include <QSound>

#include <ui_msg.h>

#include <global_class.h>

namespace Ui {
class MyWindow;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = nullptr);
    ~MyWindow();

    //初始化
    void InitWindow();

    /* 设置圆形贴图，把人脸结果换成圆形 */
    QPixmap PixmapToRound(QPixmap src, int radius);

public slots:
    //显示识别脸的相关控件
    void showfacelabel();
    //隐藏识别脸的相关控件
    void hidefacelabel();
    //显示提示的相关控件  t:显示类型，在下载时(t=1)最好设置下最小长度，显示才比较美观
    void showinfolabel(int t=0);
    //隐藏提示的相关控件
    void hideinfolabel();
    /* 显示门禁文本框 */
    void show_mj();
    /* 隐藏门禁文本框 */
    void hide_mj();

    /* 结束显示结果图像 */
    void showface_over();
    /* 显示识别人脸截图和信息 人脸信息  本次是否测温 */
    void show_user(HCP_DETECT_INFO pInfo,int temthis);
    /* 显示通行判断结果 */
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
    void show_door(int open_type,float tw);
    /* 显示设备MAC和版本号 学校名 */
    void show_MAC_V(QString mac,QString vis,QString schoolname);
    /* 接收心跳包是否正常,显示网络状态将由其它函数处理 */
    void show_network(bool flag);
    /* 显示网络状态 */
    void show_network_ui();
    /* 显示同步中 */
    void show_syning(int syn_ing);
    /* 显示已同步 */
    void show_synall(int syn_all);
    /* 显示人脸数和用户数 */
    void show_face_user(int usernum,int facenum);
    /* 更新时间 */
    void ShowTime();
    /* 设备未注册 */
    void devnofound(int s,QString mac);
    /* 更新进度 */
    void update_p(int loadtype,int utype,float s,int all);
    /* 全量更新结束 */
    void all_update(int alluser);
    /* 增量更新结束 */
    void add_update(int adduser,int subuser,int alluser);

    /* 是否显示下面的这些文本框 */
    void showui_debug(int s);
    /* 临时显示 */
    //type 0-显示识别处理耗时, htime耗时 后两个参数不用
    //type 1-显示任务处理耗时, htime任务id  wetime任务耗时  taskover任务结果
    void lsdata(int type,int htime,int wetime,int taskover);
    /* 临时显示     环境   */
    void shouhj(int hj);
    /* 临时显示     更新   */
    void updatesta(int utype,QString ustr);
    /* 临时显示        */
    void upfacedata(int us);
    /* 显示tcp组网 */
    void tcpsta(QString sta);

    /* 判断是否可以连接百度网 */
    void onLookupHost(QHostInfo host);
    /* 判断是否可以连接xbull网 */
    void onLookupHost_xbull(QHostInfo host);
    /* 获取wifi的信息 */
    void get_wifiname();

    /* 接收体温的相关参数  是否测温  测温等待时长 不测温时段*/
    void set_temp(int tf,int tt,QList<QStringList> donttem);
    /* 测温异常提示 */
    void temtoohigh(float temvalue);
    /* 测温设备是否正常 */
    void temdev_sta_func(bool flag);
    /* 获取静音参数 */
    void get_vi_ui(int f);
    /* 显示测温模块的状态 */
    void showtem();

    /* 显示测温时串口读到的温度  测温返回的标志位  串口读到的温度  最终结果  补偿值 */
    void showtemnum(int errort,float readtem,float sumtem,float pcz);
    /* 显示测温，门禁判断函数耗时 */
    void showtemdootD(int temt,int doort);

    /* 刷卡结果--警报 */
    void cardisoff(QString cardinfo);

    /* 显示频道，编号 */
    void show_cha_num(int cha,int num);

private:
    Ui::MyWindow *ui;

    QTimer *timer_showtime; //更新时间
    QTimer *showfacetime;   //显示识别脸时间
    QTimer *showinfotime;   //显示提示时间

    bool no_net_last;        //记录刚刚是否为未连接的，用于优化不可用的显示--从未连接状态连接网线时，第一次变为xxx-检测中，按照检测结果再来判断如何显示
    bool xbull_net;         //十牛连接状态
    bool netbaidu;          //ping百度结果
    bool networkport_flag;  //网口状态
    QString n_ip;           //ip地址
    bool wifi_flag;         //当前连接是否为wifi
    QString wifi_name;      //WiFi名称
    int wifi_level;         //wifi信号等级

public:

    Global_class *glo;

    int screen_width;
    int screen_height;

    int login_flag;//是否注册标识符号
    bool dhcp_ip;//DHCP

    int tem_flag;               //是否测温
    int tem_timeout;            //测温等待时长
    int nowtem;                 //当前时段是否测温
    QList<QStringList> notem;   //不测温时段
    bool temdev_ok;             //测温设备是否正常

    int isvoied;                //是否静音
    int showdebug;              //是否显示下方信息

    int upgrade_type;           //记录升级信息，用于显示提示时使用  2升级中   3升级完成，准备重启

    //UI图像
    QImage userfacebj;
    QImage userinfobj;
    QImage mjz_w;
    QImage mjy_w;
    QImage mjt_w;
    QImage mjz_r;
    QImage mjy_r;
    QImage mjt_r;
    QImage t28;
    //背景样式
    QPalette palette_bjtm;
    QPalette palette_bjjb;
    QPalette palette_bjjb_ok;
    QPalette palette_bjjb_ng;
    QPalette palette_bjjb_face;
    QPalette palette_info;
};

#endif // MYWINDOW_H
