#include "httpperson.h"

HttpPerson::HttpPerson()
{

}

HttpPerson::~HttpPerson()
{   
#ifdef QT_DEBUG
        qDebug()<<"delete person http:"<<http_url;
#endif
    if(http_timeout->isActive())
        http_timeout->stop();
    delete http_timeout;
    http_timeout=NULL;

    h_accessManager->deleteLater();
    eventLoop->deleteLater();
//    delete h_accessManager;
//    delete http_timeout;
    h_accessManager=NULL;
    eventLoop=NULL;
}

QString HttpPerson::post_and_return(QNetworkRequest req,QByteArray data)
{
    http_timeout->start();
    QNetworkReply* reply=h_accessManager->post(req,data);
    eventLoop->exec();

    if(!http_timeout->isActive())/* 超时 */
    {
        QString ptr=QString::fromLocal8Bit(reply->readAll());
#ifdef QT_DEBUG
        qDebug()<<"timeout??pte:"<<ptr;
#endif
        //reply->deleteLater();
        delete reply;
        reply=nullptr;
        if(ptr=="")
            return "";
        else
            return ptr;
    }
    http_timeout->stop();
    QString wr=NetworkReply_func(reply);
    //reply->deleteLater();
    delete reply;
    reply=nullptr;
    return wr;
}

QString HttpPerson::post_and_return(QNetworkRequest *req, QHttpMultiPart *data)
{
    http_timeout->start();
    QNetworkReply* reply=h_accessManager->post(*req,data);
    eventLoop->exec();

    if(!http_timeout->isActive())/* 超时 */
    {
        QString ptr=QString::fromLocal8Bit(reply->readAll());
#ifdef QT_DEBUG
        qDebug()<<"timeout??pte:"<<ptr;
#endif
        //reply->deleteLater();
        delete reply;
        reply=nullptr;
        if(ptr=="")
            return "";
        else
            return ptr;
    }
    http_timeout->stop();
    QString wr=NetworkReply_func(reply);
    //reply->deleteLater();
    delete reply;
    reply=nullptr;
    return wr;
}

QString HttpPerson::get_and_return(QNetworkRequest req)
{
    http_timeout->start();
    QNetworkReply* reply=h_accessManager->get(req);
    eventLoop->exec();

    if(!http_timeout->isActive())/* 超时 */
    {
        QString ptr=QString::fromLocal8Bit(reply->readAll());
#ifdef QT_DEBUG
        qDebug()<<"timeout??pte:"<<ptr;
#endif
        //reply->deleteLater();
        delete reply;
        reply=nullptr;
        if(ptr=="")
            return "";
        else
            return ptr;
    }
    http_timeout->stop();
    QString wr=NetworkReply_func(reply);
    //reply->deleteLater();
    delete reply;
    reply=nullptr;
    return wr;
}

QString HttpPerson::NetworkReply_func(QNetworkReply *rel)
{
    if(rel->error()!=QNetworkReply::NoError)
    {
        /* 请求出错，如果要处理请查看QNetworkReply.NetworkError的解释 */
        return "";
    }

    QString ptr=QString::fromLocal8Bit(rel->readAll());
#ifdef QT_DEBUG
    qDebug()<<"http return: "<<ptr<<QThread::currentThreadId();
#endif
    return ptr;
}

int HttpPerson::status_func(QString str)
{
    int status=-999;//在json中获取状态码失败，在错误中按重来处理
    Json::Reader jr;
    Json::Value jv;
    if(!jr.parse(str.toStdString(),jv))
    {
        //json解析失败
        return status;
    }

    if(jv["status"].isInt())
    {
        status=jv["status"].asInt();
    }
    else if(jv["status"].isString())
    {
        status=QString::fromStdString(jv["status"].asString()).toInt();
    }

    return status;
}

int HttpPerson::http_replay_func()
{

}

int HttpPerson::replay_return_func(QString ptr)
{

}

void HttpPerson::init(string url)
{
    //获取网卡MAC
    QString fisrt_mac;
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        QString netmac(QString::fromStdString(netInterface.hardwareAddress().toStdString()));
        if(netmac=="00:00:00:00:00:00")//排除假MAC
            continue;
        netmac.replace(":","");//剔除‘:’号
        fisrt_mac=fisrt_mac+netmac;
        break;//取得一个就退出，多个网卡的话当前就以第一个有效
    }
    net_MAC=fisrt_mac;
    //net_MAC="AC0123456789";//测试使用
    //qDebug()<<"MAC:"<<net_MAC;
    version="100";

    h_accessManager=new QNetworkAccessManager(this);

    BASE_URL h_url;
    if(BASE==1)
        http_url=QString::fromStdString(h_url.BASE_URL_TEST)+QString::fromStdString(url);
    else if(BASE==2)
        http_url=QString::fromStdString(h_url.BASE_URL_GRAY)+QString::fromStdString(url);
    else if(BASE==3)
        http_url=QString::fromStdString(h_url.BASE_URL_PRODUCT)+QString::fromStdString(url);
#ifdef QT_DEBUG
    qDebug()<<"init http:"<<http_url;
#endif

    http_timeout=new QTimer();
    http_timeout->setInterval(60000*3);//超时时间
    http_timeout->setSingleShot(true);

    eventLoop=new QEventLoop();

    connect(h_accessManager, SIGNAL(finished(QNetworkReply*)),eventLoop, SLOT(quit()));
    connect(http_timeout,SIGNAL(timeout()),eventLoop, SLOT(quit()));
}

QByteArray HttpPerson::gzipCompress(const QByteArray &in)
{
    QByteArray dest(compressBound(in.size()) + 7, 0);
    uLongf destLen = dest.size();
    int r1 = httpgzcompress((Bytef*)in.data(), in.size(), (Bytef*)dest.data(),&destLen);
    dest.resize(destLen);
    return dest;
}

int HttpPerson::httpgzcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata)
{
    z_stream c_stream;
    int err = 0;

    if(data && ndata > 0)
    {
        c_stream.zalloc = (alloc_func)0;
        c_stream.zfree = (free_func)0;
        c_stream.opaque = (voidpf)0;
        if(deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
            return -1;
        c_stream.next_in  = data;
        c_stream.avail_in  = ndata;
        c_stream.next_out = zdata;
        c_stream.avail_out  = *nzdata;
        while (c_stream.avail_in != 0 && c_stream.total_out < *nzdata)
        {
            if(deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
        }
        if(c_stream.avail_in != 0)
            return c_stream.avail_in;
        for (;;)
        {
            if((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
            if(err != Z_OK) return -1;
        }
        if(deflateEnd(&c_stream) != Z_OK)
            return -1;
        *nzdata = c_stream.total_out;
        return 0;
    }
    return -1;
}

QString HttpPerson::gzipUncompress(const QByteArray &data)
{
    if (data.size()<=8)
    {
        return "";
    }
    QByteArray out;
    ulong orign_crc = *((ulong*)(data.data() + data.length() - 8));
    ulong len =*((ulong*)(data.data() + data.length() - 4));

    out.resize(len);
    int ret = httpgzdecompress((Bytef *)data.data(), data.length(),(Bytef *)out.data(), &len); //(Bytef *)(data.data()+10), data.length()-10-8);
    if (ret!=Z_OK)
    {
        //qWarning("gzipUncompress error: %d", ret);
        return "";
    }
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, (Bytef *)(out.data()), out.length());

    QString wtf=QString::fromLocal8Bit(out);

    if (crc==orign_crc)
    {
        return wtf;
    }
    else
    {
        //qWarning("gzipUncompress crc check sum fail");
        out.clear();
        return "";
    }
}

int HttpPerson::httpgzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
{
    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */
    static char dummy_head[2] =
    {
        0x8 + 0x7 * 0x10,
        (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = zdata;
    d_stream.avail_in = 0;
    d_stream.next_out = data;
    if(inflateInit2(&d_stream, 47) != Z_OK)
        return -1;

    while (d_stream.total_out < *ndata && d_stream.total_in < nzdata)
    {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END)
            break;
        if(err != Z_OK )
        {
            if(err == Z_DATA_ERROR)
            {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK)
                {
                    return -1;
                }
            }
            else
                return -1;
        }
    }
    if(inflateEnd(&d_stream) != Z_OK)
        return -1;
    *ndata = d_stream.total_out;
    return 0;
}
