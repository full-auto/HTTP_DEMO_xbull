#include "httpincrementuseritems.h"

httpIncrementUserItems::httpIncrementUserItems()
{
 //   qDebug()<<"IncrementUserItems init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.incrementUserItems);
    //   qDebug()<<"IncrementUserItems ok";
}

httpIncrementUserItems::~httpIncrementUserItems()
{

}

int httpIncrementUserItems::http_replay_func()
{
    QNetworkRequest req;
    QUrl ht=http_url;

    QString synckey="0";
    {/* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mSynckey");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mSynckey"));
    /* 检测是否有Synckey记录列表，没有synckey则使用默认0 */
    if(db.tables().contains("Synckey"))
    {
        query.exec("SELECT count(*) from Synckey");
        query.next();
        if(!query.value(0).toInt()==0)//记录表内无数据.
        {
            query.exec("SELECT * from Synckey limit 1"); //获得第一条记录.
            query.seek(0);
            QSqlRecord rdata = query.record();
            synckey=QString::number(rdata.value("synckey").toInt());
        //    qDebug()<<"synckey:"<<synckey;
        }
        else {
       //     qDebug()<<"NO data in Synckey table!!!!!!!!";
        }
    }
    else {
       // qDebug()<<"NO Synckey table!!!!!!!!";
    }
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSynckey");

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

int httpIncrementUserItems::replay_return_func(QString ptr)
{
    int ret=status_func(ptr);
    if(ret!=0)
        return ret;

    Json::Reader jr;
    Json::Value jv;
    if(jr.parse(ptr.toStdString(),jv))
    {
        QString data_compress=QString::fromStdString(jv["data"].asString());
        //给 根据ID获得用户数据 分配函数 进行处理.
        comp_inc_data=data_compress;
    }
    return 0;
}
