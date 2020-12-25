#include "httpheartbeat.h"

Httpheartbeat::Httpheartbeat()
{
 //   qDebug()<<"heartbeat init:"<<QThread::currentThread();
    netflag=0;
    HTTPURL allurl;
    init(allurl.heartbeat);
    //   qDebug()<<"heartbeat ok";
}

Httpheartbeat::~Httpheartbeat()
{

}

int Httpheartbeat::http_replay_func()
{
    QNetworkRequest req;
    QUrl ht=http_url;
    ht.addQueryItem("number",net_MAC);
    ht.addQueryItem("version",version);
    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif

    QString ptr = get_and_return(req);

    if(ptr=="")//超时.
    {
        netflag=0;
        //qDebug()<<"No network";
        return -999;
    }
    netflag=1;
//    replay_return_func(ptr);
    return 0;
}

int Httpheartbeat::replay_return_func(QString ptr)
{
    int ret=status_func(ptr);
    if(ret!=0)
    {
        netflag=0;
        //qDebug()<<"No network";
    }
    else
    {
        netflag=1;
        //qDebug()<<"have network";
    }
    return 0;
}


