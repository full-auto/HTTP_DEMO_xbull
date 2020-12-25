#ifndef HTTPSCHOOLUSERITEMS_H
#define HTTPSCHOOLUSERITEMS_H

#include <httpperson.h>

/* 根据设备获取学校用户列表 */
class HttpschoolUserItems:public HttpPerson
{
    Q_OBJECT

public:
    HttpschoolUserItems();
    ~HttpschoolUserItems();

signals:

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);
    /* 低版本处理函数 V<25 */
    int low_version_func(QString ptr);
    /* 中版本处理函数 24<V<69 */
    int middle_version_func(QString ptr);
    /* 高版本处理函数 68<V */
    int high_version_func(QString ptr);

    int cleardb();

public:
    QString comperr_json_alldata;//高版本，获得的未解压data----包含 synckey,用户ID和类型.
};

#endif // HTTPSCHOOLUSERITEMS_H
