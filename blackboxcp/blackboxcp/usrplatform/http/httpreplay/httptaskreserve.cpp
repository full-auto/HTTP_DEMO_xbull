#include "httptaskreserve.h"

HttptaskReserve::HttptaskReserve()
{
 //   qDebug()<<"taskReserve init:"<<QThread::currentThread();
    HTTPURL allurl;
    task_flag=0;
    init(allurl.taskReserve);
    //  qDebug()<<"taskReserve ok";
}

HttptaskReserve::~HttptaskReserve()
{

}

int HttptaskReserve::http_replay_func()
{
    task_flag=0;
    t_id=0;
    t_resource_id=0;
    t_action_id=0;

    QNetworkRequest req;
    QUrl ht=http_url;
    ht.addQueryItem("number",net_MAC);
    ht.addQueryItem("version",version);
    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif
   //qDebug()<<ht;

    QString ptr = get_and_return(req);
    if(ptr=="")//超时.
        return -999;

    sta=0;//
    int ret = replay_return_func(ptr);
    if(ret==-2)
    {
        sta=-2;
    }
    return 0;
}


int HttptaskReserve::replay_return_func(QString ptr)
{
    int ret=status_func(ptr);
    if(ret!=0)
        return ret;

    Json::Reader jr;
    Json::Value jv;
    if(!jr.parse(ptr.toStdString(),jv))
    {
        return 0;
    }

    if(jv["data"].isNull())//无任务
    {
        return 0;
    }

    int id,resource_id,action_id;

    if(jv["data"]["id"].isInt())
        id=jv["data"]["id"].asInt();
    else if(jv["data"]["id"].isString())
        id=QString::fromStdString(jv["data"]["id"].asString()).toInt();

    if(jv["data"]["resource_id"].isInt())
        resource_id=jv["data"]["resource_id"].asInt();
    else if(jv["data"]["resource_id"].isString())
        resource_id=QString::fromStdString(jv["data"]["resource_id"].asString()).toInt();

    if(jv["data"]["action_id"].isInt())
        action_id=jv["data"]["action_id"].asInt();
    else if(jv["data"]["action_id"].isString())
        action_id=QString::fromStdString(jv["data"]["action_id"].asString()).toInt();

    task_flag=1;
    t_id=id;
    t_resource_id=resource_id;
    t_action_id=action_id;

    return 0;
}
