#ifndef HTTPTASKPOLLING_H
#define HTTPTASKPOLLING_H

#include <httptaskreserve.h>

/* 任务轮询类 */
class HttpTaskPolling:public QObject
{
    Q_OBJECT

public:
    HttpTaskPolling();
    ~HttpTaskPolling();

signals:
    void ptaskid(int,int,int);//任务信息
    void notask();//无任务在执行，可进行更新

    /* 界面显示/隐藏设备是否注册  0已注册，隐藏  1未注册，显示 mac */
    void dev_login_flag(int,QString);

public slots:
    //初始化
    void init();
    void uninit();

    /*任务轮询.
     *无更新时，请求任务.
     *有更新时，当处于无任务状态就发送信号给更新线程进行更新.
    */
    void taskreplay();
    //执行任务结束修改标识符
    void set_task_flag();
    //有更新，修改更新标识符，切换轮询内容,不再请求任务，等待本次任务执行完发送信号给更新线程进行升级
    void have_update_func(bool flag);
    //判断是否自动上报状态，一小时自动上报一次，高峰期不上报,
    //0不上报   1进行上报
    int status_flag();

public:
    HttptaskReserve *h_taskReserve;

    QTimer *t_task;//任务轮询计时器
    int laststatus;//记录上一次上报时时间戳(秒)，用于判断是否需要上报

    bool update_flag;//是否有更新标识符
};

#endif // HTTPTASKPOLLING_H
