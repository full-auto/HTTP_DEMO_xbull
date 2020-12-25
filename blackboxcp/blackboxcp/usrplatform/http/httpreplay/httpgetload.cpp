#include "httpgetload.h"

HttpGetLoad::HttpGetLoad()
{

}

HttpGetLoad::~HttpGetLoad()
{

}

void HttpGetLoad::tDownloadProgress(qint64 bytesSent, qint64 bytesTotal)
{

}

void HttpGetLoad::run()
{
    load_flag=0;
    QTime loadtime;
    loadtime.start();
    QNetworkAccessManager *loadnet=new QNetworkAccessManager();
    QUrl http_urll;
    QNetworkRequest reql;
    http_urll.setUrl(urladdr);

    /* 获取文件名，并设置保存路径 */
    QFileInfo dfileInfo = http_urll.path();
    QString filename_part=FACEIMG_PATH;
    filename_part=filename_part+dfileInfo.fileName();
    //把文件名统一设为.jpg
    int old_index=filename_part.lastIndexOf(".");
    filename_part.truncate(old_index);
    filename_part=filename_part+".jpg";
    //完整路径提供给人脸录入使用
    imgnamepath=filename_part;

    //对OSS请求时,要求返回的图片为jpg, 且图片宽高像素不可为奇数，所以统一都缩放到h*w
    //具体可参考阿里云OSS图片处理--https://help.aliyun.com/document_detail/107797.html?spm=a2c4g.11174283.6.1104.359f7da2W6LA0c
    http_urll.addQueryItem("x-oss-process","image/resize,m_pad,h_1280,w_720,limit_0/format,jpg");//1280*720  1920*1080

    QFile *mfile;
    mfile=new QFile(filename_part);
    /* 打开方式 */
    mfile->open(QIODevice::WriteOnly);

    reql.setUrl(http_urll);
#ifdef QT_DEBUG
    qDebug()<<"load img url:"<<http_urll;
#endif

    QNetworkReply* lreply;
    //connect(lreply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(tDownloadProgress(qint64,qint64)));//download文件时进度
    //connect((QObject *)lreply, SIGNAL(readyRead()),this, SLOT(onReadyRead()));

    QTimer xtime;
    xtime.setSingleShot(true);
    QEventLoop eventLoop;
    connect(loadnet, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    connect(&xtime,SIGNAL(timeout()),&eventLoop, SLOT(quit()));
    lreply = loadnet->get(reql);
    xtime.start(60000*3);//规定时间内没下载完就退出.
    eventLoop.exec();
    if(xtime.isActive())
    {
        xtime.stop();

        if(lreply->error()!=QNetworkReply::NoError)
        {
#ifdef QT_DEBUG
            qDebug()<<"load fail!!!";
#else
#endif
            mfile->close();
            load_flag=2;
        }
        else
        {
#ifdef QT_DEBUG
            qDebug()<<"load OK";
#else
#endif
            mfile->write(lreply->readAll());
            mfile->close();
            load_flag=1;
        }
    }
    else
    {
        mfile->close();
        load_flag=0;
#ifdef QT_DEBUG
        qDebug()<<"load timeout!!!";
#endif
    }

    delete lreply;
    delete mfile;
    delete loadnet;
    lreply=NULL;
    mfile=NULL;
    loadnet=NULL;
#ifdef QT_DEBUG
    qDebug()<<"load times is :"<<loadtime.elapsed()<<" ms";
#endif
    //usleep(500);//等一会让文件彻底释放.
}
