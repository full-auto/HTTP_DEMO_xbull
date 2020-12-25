#ifndef HTTPTASTRETURN_H
#define HTTPTASTRETURN_H

#include <httptaskrelease.h>

//任务反馈上传线程
class httptastreturn:public QObject
{
    Q_OBJECT

public:
    httptastreturn();
    ~httptastreturn();

public slots:
    //初始化
    void init();
    void uninit();
    //接收到的任务处理结果，
    //任务id，处理结果1ok 0失败
    void tast_over(int tastid,int over);

    //上传结果
    void uptask_over();

public:
    HttptaskRelease *h_taskRelease;//上传任务结果
    QTimer *uptime;
};

#endif // HTTPTASTRETURN_H
