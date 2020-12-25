#ifndef HTTPSWIPECARD_H
#define HTTPSWIPECARD_H

#include <httpperson.h>

//上传刷卡记录
class httpswipecard:public HttpPerson
{
    Q_OBJECT

public:
    httpswipecard();
    ~httpswipecard();

signals:

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);

public:
    QString ccard;
    QString ctime;
    QString cpicture_path;
};

#endif // HTTPSWIPECARD_H
