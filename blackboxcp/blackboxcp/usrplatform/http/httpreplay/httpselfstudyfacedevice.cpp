#include "httpselfstudyfacedevice.h"

HttpselfStudyFaceDevice::HttpselfStudyFaceDevice()
{
 //   qDebug()<<"elfStudyFaceDevice init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.selfStudyFaceDevice);
    //   qDebug()<<"elfStudyFaceDevice ok";
}

HttpselfStudyFaceDevice::~HttpselfStudyFaceDevice()
{

}

int HttpselfStudyFaceDevice::http_replay_func()
{
    QNetworkRequest req;

    SecurityToken="";
    //该函数对应多个sts的申请，所有url不固定.
    http_url.clear();
    BASE_URL h_url;
    if(BASE==1)
        http_url=QString::fromStdString(h_url.BASE_URL_TEST)+sts_api;
    else if(BASE==2)
        http_url=QString::fromStdString(h_url.BASE_URL_GRAY)+sts_api;
    else if(BASE==3)
        http_url=QString::fromStdString(h_url.BASE_URL_PRODUCT)+sts_api;

    QUrl ht=http_url;


    ht.addQueryItem("number",net_MAC);
    ht.addQueryItem("version",version);
    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif

    QString ptr = get_and_return(req);
    if(ptr=="")//超时.
        return -999;

    replay_return_func(ptr);
    return 0;
}

int HttpselfStudyFaceDevice::replay_return_func(QString ptr)
{
    //在STS授权返回数据上，没有status的参数,不做status判断

    Json::Reader jr;
    Json::Value jv;
    if(!jr.parse(ptr.toStdString(),jv))
    {
        return 0;
    }

    /* 授权秘钥 */
    AccessKeySecret=jv["AccessKeySecret"].asString();
    /* 授权ID */
    AccessKeyId=jv["AccessKeyId"].asString();
    /* 过期时间 */
    if(jv["Expiration"].isInt())
        Expiration=jv["Expiration"].asInt();
    else
        Expiration=QString::fromStdString(jv["Expiration"].asString()).toInt();
    /* 授权token */
    SecurityToken=jv["SecurityToken"].asString();

    return 0;
}
