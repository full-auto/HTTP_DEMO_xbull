#ifndef HTTPTASKRELEASE_H
#define HTTPTASKRELEASE_H

#include <httpperson.h>
//设备任务反馈.
class HttptaskRelease:public HttpPerson
{
    Q_OBJECT

public:
    HttptaskRelease();
    ~HttptaskRelease();

signals:

public slots:
    //重载网络请求的处理函数.
    int http_replay_func();
    int replay_return_func(QString ptr);

public:
    int id;
    int is_success;
};

#endif // HTTPTASKRELEASE_H
