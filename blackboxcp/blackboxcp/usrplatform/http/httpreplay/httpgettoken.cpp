#include "httpgettoken.h"

httpgettoken::httpgettoken()
{
    //qDebug()<<"httpgettoken init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.getToken);
    //qDebug()<<"httpgettoken ok";
}

httpgettoken::~httpgettoken()
{

}

int httpgettoken::http_replay_func()
{
    s_token="";
    QNetworkRequest req;
    QUrl ht=http_url.toString()+net_MAC;
    ht.addQueryItem("api",apiurl);
    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif

    QString ptr = get_and_return(req);

    int ret=0;
    if(ptr=="")//超时.
         ret=-999;

    ret = replay_return_func(ptr);
    return ret;
}

int httpgettoken::replay_return_func(QString ptr)
{
    int ret=status_func(ptr);
    //token返回ok为1
    if(ret!=1)
        return ret;

    Json::Reader jr;
    Json::Value jv;
    if(jr.parse(ptr.toStdString(),jv))
    {
        Json::Value jd=jv["data"];
        /* 授权token */
        s_token=QString::fromStdString(jd["access_token"].asString());
      //  qDebug()<<"get token access_token:-----------------:"<<s_token;
    }
    return 0;
}
