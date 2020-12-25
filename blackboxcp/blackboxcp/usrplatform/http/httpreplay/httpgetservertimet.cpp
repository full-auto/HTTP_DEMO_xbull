#include "httpgetservertimet.h"

httpgetservertimet::httpgetservertimet()
{
    //qDebug()<<"httpgetservertimet init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.getservertime);
    //qDebug()<<"httpgetservertimet ok"<<QThread::currentThread();
}

httpgetservertimet::~httpgetservertimet()
{

}

int httpgetservertimet::http_replay_func()
{
    server_time=0;
    QNetworkRequest req;
    QUrl ht=http_url;
    ht.addQueryItem("number",net_MAC);
    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif

    QString ptr = get_and_return(req);
    if(ptr=="")//超时不处理.
        return -999;

    replay_return_func(ptr);
    return 0;
}

int httpgetservertimet::replay_return_func(QString ptr)
{
    int ret=status_func(ptr);
    if(ret!=0)
        return ret;

    Json::Reader jr;
    Json::Value jv;
    if(jr.parse(ptr.toStdString(),jv))
    {
        /* time */
        if(jv["current_server_time"].isInt64())
            server_time=jv["current_server_time"].asInt64();
        else if(jv["current_server_time"].isString())
            server_time=QString::fromStdString(jv["current_server_time"].asString()).toLong();
    }
//    qDebug()<<" tt time:"<<server_time;
}
