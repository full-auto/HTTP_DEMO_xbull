#include "httpupdatainfo.h"

httpupdatainfo::httpupdatainfo()
{
 //   qDebug()<<"updatainfo init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.getUpgrade);
    //   qDebug()<<"updatainfo ok";
}

httpupdatainfo::~httpupdatainfo()
{

}

int httpupdatainfo::http_replay_func()
{
    QNetworkRequest req;
    QUrl ht=http_url;
    ht.addQueryItem("number",net_MAC);
    ht.addQueryItem("type","3");
    ht.addQueryItem("version",version);
    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif
    //qDebug()<<ht;

    status=11;
    QString ptr = get_and_return(req);
    if(ptr=="")//超时.
        return -999;

    return replay_return_func(ptr);
}

int httpupdatainfo::replay_return_func(QString ptr)
{
    showstr=ptr;
    int ret=status_func(ptr);
    status=ret;
    if(ret!=0)
        return ret;

    Json::Reader jr;
    Json::Value jv;
    if(!jr.parse(ptr.toStdString(),jv))
    {
        return 0;
    }

    name=QString::fromStdString(jv["name"].asString());
    new_version=QString::fromStdString(jv["version"].asString());
    is_urgent=QString::fromStdString(jv["is_urgent"].asString());
    allowed_upgrade_time=QString::fromStdString(jv["allowed_upgrade_time"].asString());
    shell=QString::fromStdString(jv["shell"].asString());
    load_url=QString::fromStdString(jv["url"].asString());
    md5=QString::fromStdString(jv["md5"].asString());
    return 0;
}
