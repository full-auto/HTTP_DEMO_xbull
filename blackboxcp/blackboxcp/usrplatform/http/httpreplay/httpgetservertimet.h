#ifndef HTTPGETSERVERTIMET_H
#define HTTPGETSERVERTIMET_H

#include <httpperson.h>
/* 获得服务器的时间戳 */
class httpgetservertimet:public HttpPerson
{
    Q_OBJECT

public:
    httpgetservertimet();
    ~httpgetservertimet();

signals:

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);

public:
    int64_t server_time;
};

#endif // HTTPGETSERVERTIMET_H
