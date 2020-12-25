#include "httptaskrelease.h"

HttptaskRelease::HttptaskRelease()
{
 //   qDebug()<<"taskRet init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.taskRelease);
    //  qDebug()<<"taskRet ok";
}

HttptaskRelease::~HttptaskRelease()
{

}

int HttptaskRelease::http_replay_func()
{
 //   qDebug()<<"task release";
    QNetworkRequest req;
    QUrl ht=http_url;
    QByteArray post_data;

    //req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));

    QString sid="id="+QString::number(id)+"&";
    post_data.append(sid);

    QString number="number="+net_MAC+"&";
    post_data.append(number);

    QString sis_su="is_success="+QString::number(1);//is_success
    post_data.append(sis_su);

    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif

    QString ptr = post_and_return(req,post_data);
#ifdef QT_DEBUG
    qDebug()<<http_url<<"return:"<<ptr;
#endif

    if(ptr=="")//超时.
        return -999;

    replay_return_func(ptr);
    return 0;
}


int HttptaskRelease::replay_return_func(QString ptr)
{
    //目前对任务上报返回不作处理.
    return 0;
}
