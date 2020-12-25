#include "httpschoolleaveuser.h"

httpSchoolLeaveUser::httpSchoolLeaveUser()
{
    HTTPURL allurl;
    init(allurl.schoolLeaveUser);
}

httpSchoolLeaveUser::~httpSchoolLeaveUser()
{

}

int httpSchoolLeaveUser::http_replay_func()
{
    QNetworkRequest req;
    QUrl ht=http_url;

    QString synckey="0";
    {/* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mLeaveUser");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mLeaveUser"));
    /* 检测是否有请假记录列表，没有synckey则使用默认0 */
    if(db.tables().contains("LeaveSyn"))
    {
        query.exec("SELECT count(*) from LeaveSyn");
        query.next();
        if(query.value(0).toInt()!=0)//记录表内有数据.
        {
            query.exec("SELECT * from LeaveSyn limit 1"); //获得第一条记录.
            query.seek(0);
            QSqlRecord rdata = query.record();
            synckey=QString::number(rdata.value("synckey").toInt());
        }
    }

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mLeaveUser");

    ht.addQueryItem("number",net_MAC);
    ht.addQueryItem("synckey",synckey);
    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif

    QString ptr = get_and_return(req);
    if(ptr=="")//超时.
        return -999;

    int ret=replay_return_func(ptr);
    return ret;
}

int httpSchoolLeaveUser::replay_return_func(QString ptr)
{
    int ret=status_func(ptr);
    if(ret!=0)
        return ret;

    Json::Reader jr;
    Json::Value js;
    if(!jr.parse(ptr.toStdString(),js))
    {
        return -999;
    }

    QString ysstr=QString::fromStdString(js["data"].asString());
    QByteArray d1=ysstr.toLocal8Bit();
    QByteArray d2=QByteArray::fromBase64(d1);
    QString d_uncompress=gzipUncompress(d2);

#ifdef QT_DEBUG
    qDebug()<<"请假数据";
    qDebug()<<"解压前:"<<ysstr;
    qDebug()<<"解压后:"<<d_uncompress;
#endif

    Json::Value jv;
    if(!jr.parse(d_uncompress.toStdString(),jv))
    {
        return -999;
    }

    {/* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mLeaveUser");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mLeaveUser"));

    /* 检测是否有请假Synckey记录列表，没有则创建 */
    if(!db.tables().contains("LeaveSyn"))
    {
        query.exec("create table LeaveSyn(synckey int)");
    }
    /* 单个设备信息，当前做法直接把原来的清空把新的放进去 */
    int synckey=0;
    if(jv["synckey"].isInt())
        synckey=jv["synckey"].asInt();
    else
        synckey=QString::fromStdString(jv["synckey"].asString()).toInt();

    query.exec("delete from LeaveSyn");
    query.exec("update sqlite_sequence SET seq = 0 where name ='LeaveSyn'");
    query.prepare("INSERT INTO LeaveSyn(synckey)VALUES(:synckey)");
    query.bindValue(":synckey",synckey);
    query.exec();

    /* 检测是否有请假用户列表，没有则创建 */
    if(!db.tables().contains("LeaveUser"))
    {
        query.exec("create table LeaveUser(user_id int,\
                                            user_type int,\
                                            leave_id int,\
                                            leave_type string,\
                                            start_time int,\
                                            end_time int)");
    }
    //新加请假
    Json::Value jnew=jv["new_items"];
    for(int i=0;i<jnew.size();i++)
    {
        //用户id
        int user_id;
        if(jnew[i]["user_id"].isInt())
            user_id=jnew[i]["user_id"].asInt();
        else
            user_id=QString::fromStdString(jnew[i]["user_id"].asString()).toInt();
        //用户类型
        int user_type;
        if(jnew[i]["user_type"].isInt())
            user_type=jnew[i]["user_type"].asInt();
        else
            user_type=QString::fromStdString(jnew[i]["user_type"].asString()).toInt();
        //请假id
        int leave_id;
        if(jnew[i]["leave_id"].isInt())
            leave_id=jnew[i]["leave_id"].asInt();
        else
            leave_id=QString::fromStdString(jnew[i]["leave_id"].asString()).toInt();
        //请假类型
        QString leave_type;
        leave_type=QString::fromStdString(jnew[i]["leave_type"].asString());
        //请假开始时间
        int start_time;
        if(jnew[i]["start_time"].isInt())
            start_time=jnew[i]["start_time"].asInt();
        else
            start_time=QString::fromStdString(jnew[i]["start_time"].asString()).toInt();
        //请假结束时间
        int end_time;
        if(jnew[i]["end_time"].isInt())
            end_time=jnew[i]["end_time"].asInt();
        else
            end_time=QString::fromStdString(jnew[i]["end_time"].asString()).toInt();

        query.prepare("select leave_id from LeaveUser where leave_id=:leave_id");
        query.bindValue(":leave_id",leave_id);
        query.exec();
        int h=0;
        while(query.next())//没有该请假记录才添加
        {
            h++;
            break;
        }
        if(h==0)
        {
            query.prepare("INSERT INTO LeaveUser(user_id,user_type,leave_id,leave_type,start_time,end_time)VALUES\
                                                (:user_id,:user_type,:leave_id,:leave_type,:start_time,:end_time)");

            query.bindValue(":user_id",user_id);
            query.bindValue(":user_type",user_type);
            query.bindValue(":leave_id",leave_id);
            query.bindValue(":leave_type",leave_type);
            query.bindValue(":start_time",start_time);
            query.bindValue(":end_time",end_time);
            query.exec();
        }
    }

    //删除请假
    Json::Value jdel=jv["del_items"];
    for(int i=0;i<jdel.size();i++)
    {
        //请假id
        int leave_id;
        if(jdel[i]["leave_id"].isInt())
            leave_id=jdel[i]["leave_id"].asInt();
        else
            leave_id=QString::fromStdString(jdel[i]["leave_id"].asString()).toInt();

        query.prepare("Delete from LeaveUser where leave_id=:leave_id");
        query.bindValue(":leave_id",leave_id);
        query.exec();
    }

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mLeaveUser");
    return ret;
}

void httpSchoolLeaveUser::cleandb()
{
    {/* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mLeaveUser");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mLeaveUser"));

    query.exec("DROP TABLE LeaveUser");
    query.exec("DROP TABLE LeaveSyn");

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mLeaveUser");
}
