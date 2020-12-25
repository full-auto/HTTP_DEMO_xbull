#ifndef HTTPCORRECTION_H
#define HTTPCORRECTION_H

#include <httpperson.h>

//数据修复接口
class httpcorrection:public HttpPerson
{
    Q_OBJECT

public:
    httpcorrection();
    ~httpcorrection();

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);

    //数据修复处理分配函数 t修复的用户类型
    int correction_func(int t);

public:
    int type;//用户类型
    QString user_hj;//需要发送的json数据合集，还未压缩和加密的
    QString correc_date;//返回未加密的数据
};

#endif // HTTPCORRECTION_H
