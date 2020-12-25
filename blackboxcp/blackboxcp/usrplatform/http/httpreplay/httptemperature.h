#ifndef HTTPTEMPERATURE_H
#define HTTPTEMPERATURE_H

#include <httpperson.h>
//上传测温数据
class httptemperature:public HttpPerson
{
    Q_OBJECT
public:
    httptemperature();
    ~httptemperature();

signals:

public slots:
    //重载网络请求的处理函数.
    int http_replay_func();
    int replay_return_func(QString ptr);

public:
    int userid;
    int usertype;
    double temperature;
    int temperature_t;
};

#endif // HTTPTEMPERATURE_H
