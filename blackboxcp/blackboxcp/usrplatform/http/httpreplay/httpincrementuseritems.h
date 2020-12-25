#ifndef HTTPINCREMENTUSERITEMS_H
#define HTTPINCREMENTUSERITEMS_H

#include <httpperson.h>
/* 增量更新 */
class httpIncrementUserItems:public HttpPerson
{
    Q_OBJECT

public:
    httpIncrementUserItems();
    ~httpIncrementUserItems();

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);

public:
    QString comp_inc_data;//高版本，获得的未解压增量data----包含 synckey, updata add del 用户ID和类型.
};

#endif // HTTPINCREMENTUSERITEMS_H
