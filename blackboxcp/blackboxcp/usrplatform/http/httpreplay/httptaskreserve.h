#ifndef HTTPTASKRESERVE_H
#define HTTPTASKRESERVE_H

#include <httpperson.h>
//获取设备任务.
class HttptaskReserve:public HttpPerson
{
    Q_OBJECT

public:
    HttptaskReserve();
    ~HttptaskReserve();

signals:

public slots:
    //重载网络请求的处理函数.
    int http_replay_func();
    int replay_return_func(QString ptr);

public:
    bool task_flag;//是否在执行任务 标识符
    //任务的3个参数
    int t_id;
    int t_resource_id;
    int t_action_id;

    int sta;//特殊状态返回标识，用于对应设备未登录的情况
};

#endif // HTTPTASKRESERVE_H
