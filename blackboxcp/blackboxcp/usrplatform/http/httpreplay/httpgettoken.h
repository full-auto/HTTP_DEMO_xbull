#ifndef HTTPGETTOKEN_H
#define HTTPGETTOKEN_H

#include <httpperson.h>
/* 获得人脸数据上传的token */
class httpgettoken:public HttpPerson
{
    Q_OBJECT

public:
    httpgettoken();
    ~httpgettoken();

signals:

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);

public:
    QString apiurl;
    QString s_token;
};

#endif // HTTPGETTOKEN_H
