#ifndef HTTPSCHOOLLEAVEUSER_H
#define HTTPSCHOOLLEAVEUSER_H
#include <httpperson.h>
//获得请假用户的信息
class httpSchoolLeaveUser:public HttpPerson
{
    Q_OBJECT
public:
    httpSchoolLeaveUser();
    ~httpSchoolLeaveUser();

signals:

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);

    void cleandb();

public:

};

#endif // HTTPSCHOOLLEAVEUSER_H
