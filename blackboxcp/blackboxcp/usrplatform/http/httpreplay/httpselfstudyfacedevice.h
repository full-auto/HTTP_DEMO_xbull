#ifndef HTTPSELFSTUDYFACEDEVICE_H
#define HTTPSELFSTUDYFACEDEVICE_H

#include <httpperson.h>
/* 获取STS授权 */
class HttpselfStudyFaceDevice:public HttpPerson
{
    Q_OBJECT

public:
    HttpselfStudyFaceDevice();
    ~HttpselfStudyFaceDevice();

signals:

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);

public:

    /* 获取授权的接口，不同的功能对应的有区别的 */
    QString sts_api;

    /* 授权STS ID */
    string AccessKeyId;
    /* 授权秘钥 */
    string AccessKeySecret;
    /* 过期时间 */
    int Expiration;
    /* 授权token */
    string SecurityToken;
};

#endif // HTTPSELFSTUDYFACEDEVICE_H
