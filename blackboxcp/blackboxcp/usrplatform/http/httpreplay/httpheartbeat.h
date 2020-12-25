#ifndef HTTPHEARTBEAT_H
#define HTTPHEARTBEAT_H

#include <httpperson.h>
/* 心跳包 */
class Httpheartbeat:public HttpPerson
{
    Q_OBJECT

public:
    Httpheartbeat();
    ~Httpheartbeat();

signals:

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);

public:
    bool netflag;
};

#endif // HTTPHEARTBEAT_H
