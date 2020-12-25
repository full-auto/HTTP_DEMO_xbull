#ifndef HTTPFACEGATESWIPECARD_H
#define HTTPFACEGATESWIPECARD_H

#include <httpperson.h>
#include <httpgettoken.h>
#include <httpgetservertimet.h>
//人脸识别结果数据接入平台.
class HttpfaceGateSwipeCard:public HttpPerson
{
    Q_OBJECT

public:
    HttpfaceGateSwipeCard();
    ~HttpfaceGateSwipeCard();

signals:

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);
    //更新token
    void update_token();

public:
    httpgettoken *h_gettoken;
    FACE_POST fpd;

    QTimer *token_time;
};

#endif // HTTPFACEGATESWIPECARD_H
