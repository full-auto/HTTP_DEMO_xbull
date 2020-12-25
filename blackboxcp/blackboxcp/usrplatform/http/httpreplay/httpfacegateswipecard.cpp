#include "httpfacegateswipecard.h"

HttpfaceGateSwipeCard::HttpfaceGateSwipeCard()
{
    //qDebug()<<"faceGateSwipeCard init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.faceGateSwipeCard);
    h_gettoken=new httpgettoken();
    h_gettoken->apiurl="Attendance/faceGateSwipeCard";

    token_time=new QTimer();
    connect(token_time,SIGNAL(timeout()),this,SLOT(update_token()));
    token_time->start(86400000);//24小时更新一次

    //qDebug()<<"faceGateSwipeCard ok";
}

HttpfaceGateSwipeCard::~HttpfaceGateSwipeCard()
{
    token_time->stop();
    delete token_time;
    token_time=NULL;

    delete h_gettoken;
    h_gettoken=NULL;
}

int HttpfaceGateSwipeCard::http_replay_func()
{
#ifdef QT_DEBUG
    qDebug()<<"go send data";
#endif

    int ret;
    QNetworkRequest *req=new QNetworkRequest();
    QUrl ht=http_url;

    /* 时间戳: QDateTime::currentMSecsSinceEpoch() 毫秒 | QDateTime::currentMSecsSinceEpoch()/1000 秒 */
    QString timestamp=QString::number( QDateTime::currentMSecsSinceEpoch()/1000-(3600*8) );
    /* 抓拍时间 */
    QString capture_time=QString::number(fpd.capture_time);
 //   qDebug()<<"capture_time:"<<capture_time;
    /* token */
    if(h_gettoken->s_token=="")
        update_token();
    QString token=h_gettoken->s_token;
    if(token=="")
        return 2;
 //   qDebug()<<"token:"<<token;
    /* 设备号 */
    QString number=net_MAC;
//    qDebug()<<"number:"<<number;
    /* 图片路径  在OSS上的路径-- "attendance_device/ + 本地图片的md5[本地路径/图片名字.jpg] + .jpg */
    QString picture_path;
    QByteArray picture_path_sha1=QCryptographicHash::hash(fpd.picture_path.toUtf8(),QCryptographicHash::Sha1);
    picture_path.append(picture_path_sha1.toHex());
    picture_path="attendance_device/"+picture_path+".jpg";
    /* 用户 id */
    QString user_id=QString::number(fpd.user_id);
//    qDebug()<<"user_id"<<user_id;
    /* 用户类型:1=学生, 2=家长, 3=职工, 4=访客 */
    QString user_type=QString::number(fpd.user_type);
    /* 识别状态:1=已经识别 2=未识别; version > 15, 此字段为必填 */
    QString identify_status=QString::number(fpd.identify_status);
 //   qDebug()<<"识别结果:"<<identify_status;
    /* 人脸相似度,保留小数点后两位 */
    QString rate=QString::number(fpd.rate,'f',2);
    /* 版本号 */
    QString versionnum="&version="+version;
    /* 图片 md5 ----picture_path的md5 */
    QByteArray p_sha1=QCryptographicHash::hash(picture_path.toUtf8(), QCryptographicHash::Sha1);
    QString picture_md5;
    picture_md5.append(p_sha1.toHex());
//    qDebug()<<"pic oss path:"<<picture_path;
//    qDebug()<<"pic oss md5:"<<picture_md5;
//    qDebug()<<"----------------------";
    /* apitoken */
    QString shai_token=timestamp+
                        token+
                        number+
                        capture_time+
                        picture_path+
                        user_id+
                        user_type+
                        identify_status+
                        rate+
                        version+
                        picture_md5;
    QByteArray t_sha1=QCryptographicHash::hash(shai_token.toUtf8(), QCryptographicHash::Sha1);
    QString apitoken;
    apitoken.append(t_sha1.toHex());

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QString bd="---011000010111000001101001";
    multiPart->setBoundary(bd.toLatin1());
    req->setHeader(QNetworkRequest::ContentTypeHeader,"multipart/form-data; boundary="+bd);

    QHttpPart textPart1;
    textPart1.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"timestamp\""));
    textPart1.setBody(timestamp.toUtf8());

    QHttpPart textPart2;
    textPart2.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"apitoken\""));
    textPart2.setBody(apitoken.toUtf8());

    QHttpPart textPart3;
    textPart3.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"number\""));
    textPart3.setBody(number.toUtf8());

    QHttpPart textPart4;
    textPart4.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"capture_time\""));
    textPart4.setBody(capture_time.toUtf8());

    QHttpPart textPart5;
    textPart5.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"picture_path\""));
    textPart5.setBody(picture_path.toUtf8());

    QHttpPart textPart6;
    textPart6.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"user_id\""));
    textPart6.setBody(user_id.toUtf8());

    QHttpPart textPart7;
    textPart7.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"user_type\""));
    textPart7.setBody(user_type.toUtf8());

    QHttpPart textPart8;
    textPart8.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"identify_status\""));
    textPart8.setBody(identify_status.toUtf8());

    QHttpPart textPart9;
    textPart9.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"rate\""));
    textPart9.setBody(rate.toUtf8());

    QHttpPart textPart10;
    textPart10.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"version\""));
    textPart10.setBody(version.toUtf8());

    QHttpPart textPart11;
    textPart11.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"picture_md5\""));
    textPart11.setBody(picture_md5.toUtf8());

    multiPart->append(textPart1);
    multiPart->append(textPart2);
    multiPart->append(textPart3);
    multiPart->append(textPart4);
    multiPart->append(textPart5);
    multiPart->append(textPart6);
    multiPart->append(textPart7);
    multiPart->append(textPart8);
    multiPart->append(textPart9);
    multiPart->append(textPart10);
    multiPart->append(textPart11);

    multiPart->setBoundary(bd.toLatin1());
    req->setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif

    QString ptr = post_and_return(req,multiPart);

    delete req;
    delete multiPart;
    req=NULL;
    multiPart=NULL;

    if(ptr=="")//
        return -999;
    else
        ret=replay_return_func(ptr);

    return ret;
}

int HttpfaceGateSwipeCard::replay_return_func(QString ptr)
{
    int ret;
    int sss = status_func(ptr);
    if(sss==0)
    {
        ret=0;
    }
    else
    {
        ret=1;
    }
    return ret;
}

void HttpfaceGateSwipeCard::update_token()
{
    h_gettoken->http_replay_func();
}
