#ifndef HTTPUPDATAINFO_H
#define HTTPUPDATAINFO_H

#include <httpperson.h>

//获取程序更新信息.
class httpupdatainfo:public HttpPerson
{
    Q_OBJECT

public:
    httpupdatainfo();
    ~httpupdatainfo();

public slots:
    //重载网络请求的处理函数.
    int http_replay_func();
    int replay_return_func(QString ptr);

public:
    int status;
    QString name;
    QString new_version;
    QString is_urgent;
    QString allowed_upgrade_time;
    QString shell;
    QString load_url;
    QString md5;

    QString showstr;//给界面显示，暂时
};

#endif // HTTPUPDATAINFO_H
