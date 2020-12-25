#ifndef HTTPREPORTSTATUS_H
#define HTTPREPORTSTATUS_H

#include <httpperson.h>

//上报闸机状态.
class httpreportStatus:public HttpPerson
{
    Q_OBJECT

public:
    httpreportStatus();
    ~httpreportStatus();

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);

public:
    //数据上报后返回的状态，1正常，0异常-需要修复
    int config_status;
    int usertype_status[4];
};

#endif // HTTPREPORTSTATUS_H
