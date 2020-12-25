#ifndef HTTPITEMSBYID_H
#define HTTPITEMSBYID_H

#include <httpperson.h>
/* 根据ID集合获取数据, 包含 学生 家长 职工 访客 4种类型 */
class httpItemsByID:public HttpPerson
{
    Q_OBJECT

public:
    httpItemsByID();
    ~httpItemsByID();

signals:
    void update100();//更新百分比信号

public slots:
    /* 重载网络请求的处理函数 */
    int http_replay_func();
    int replay_return_func(QString ptr);

    /* 全量更新对应的操作函数 */
    int all_updata_func();
    /* 增量更新对应的操作函数 */
    int inc_updata_func();

    /* 数据修复对应的操作函数 type-用户类型，在数据修复中，返回的数据只有ID不包含用户类型的*/
    int correct_func(int type);

    /* 清空数据表 */
    int cleardb();

    /* 统计用户数 */
    void count_usernum();

public:
    int usertype;//用户类型.
    QString id_json;//用户ID的json合集 一次最多4000.
    QString comp_data;//全量或增量获得的未解压字符串.

    //用户数 人脸数.
    int userallnum;
    int faceallnum;

    //
    int addusernum;//本次要新加的用户
    int subusernum;//本次要删除的用户
    int thisnum;//新加时，已操作的用户数

    //输出失败的文本，临时使用
    QString failstr;

    Global_class *glo;
};

#endif // HTTPITEMSBYID_H
