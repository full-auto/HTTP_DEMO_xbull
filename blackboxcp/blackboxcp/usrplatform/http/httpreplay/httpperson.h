#ifndef HTTPPERSON_H
#define HTTPPERSON_H

#include <httpurl.h>
#include <httpgetload.h>

#include "zlib/zlib.h"
#include <QByteArray>

#include <global_class.h>

//网络请求父类，有新的处理需求继承该类并重载网络请求处理的相关函数即可.
//接收返回结果的函数除外，在该函数中先统一处理是否有错误，没有在下发给实际的处理函数.
class HttpPerson:public QObject
{
    Q_OBJECT

public:
    HttpPerson();
    ~HttpPerson();

signals:

//private slots:
public slots:

    /*
     * func:发送get请求并获得返回值-同步
     * return:请求返回的数据
    */
    QString get_and_return(QNetworkRequest req);
    /*
     * func:发送post请求并获得返回值-同步
     * return:请求返回的数据
    */
    QString post_and_return(QNetworkRequest req,QByteArray data);
    /*
     * func:发送post请求并获得返回值-同步
     * return:请求返回的数据
    */
    QString post_and_return(QNetworkRequest *req,QHttpMultiPart *data);
    /* 判断请求是否正常 */
    QString NetworkReply_func(QNetworkReply* rel);
    /* 状态码判断函数 返回0说明正常 */
    int status_func(QString str);

    /* 以下函数，请继承后按照实际的请求接口功能进行实现 */
    /* 设置发送的url参数 */
    /* 默认返回0成功，其它为错误，但有特殊的(如获取token的1为成功)，具体请看具体子类重载后的说明 */
    int http_replay_func();
    /* 处理返回结果 */
    int replay_return_func(QString ptr);

public:
    /*
     * func:初始化
     * param:
     *  url:请求的地址 参考httpurl.h
    */
    void init(string url);


    /*
    * func:压缩gzip 字符串
    * param:
    *   in:需要压缩的字符串
    * return:
    *   QByteArray 压缩后的数据
    */
    QByteArray gzipCompress(const QByteArray &in);

    /*
    * func:gzip压缩处理头的函数
    */
    int httpgzcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata);

    /*
    * func:解压gzip 字符串
    * param:
    *   data:输入的压缩QByteArray---BASE64
    * return:
    *   QString 解压后的字符串
    */
    QString gzipUncompress(const QByteArray &data);
    /*
    * func:gzip解压处理头的函数
    */
    int httpgzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);


public:
    QNetworkAccessManager *h_accessManager;//网络请求模块类.
    QUrl http_url;//网址.
    QTimer *http_timeout;//超时设置的定时器.
    QEventLoop *eventLoop;

    QString net_MAC;//网卡MAC.
    QString version;//版本号.
    //bool state;//依赖线程结束的标识符，当线程准备结束时标识符设0，停止内部的http请求发送.
};

#endif // HTTPPERSON_H
