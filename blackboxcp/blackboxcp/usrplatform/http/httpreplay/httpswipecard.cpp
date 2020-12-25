#include "httpswipecard.h"

httpswipecard::httpswipecard()
{
    HTTPURL allurl;
    init(allurl.swipeCard);
}

httpswipecard::~httpswipecard()
{

}

int httpswipecard::http_replay_func()
{
    QNetworkRequest req;
    QUrl ht=http_url;
    QByteArray post_data;

    //卡ID
    QString pcard="card="+ccard+"&";
    post_data.append(pcard);
    //刷卡时间
    QString ptime="time="+ctime+"&";
    post_data.append(ptime);
    //设备号
    QString pnumber="number="+net_MAC+"&";
    post_data.append(pnumber);
    if(cpicture_path=="")
    {
        //图片id 模拟刷卡默认是123
        QString possid="oss_id=125&";
        post_data.append(possid);
    }

    /*  ---下面是为以md5为路径，不过刷卡以path为准
    //图片地址
    QString ppath="picture_path="+cpicture_path+"&";
    post_data.append(ppath);
    //图片md5
    QByteArray p_sha1=QCryptographicHash::hash(cpicture_path.toUtf8(), QCryptographicHash::Sha1);
    QString picture_md5;
    picture_md5.append(p_sha1.toHex());
    picture_md5="attendance_device/"+picture_md5+".jpg";
    QString pmd5="picture_md5="+picture_md5+"&";
    post_data.append(pmd5);
    */

    //此处平台是以picture_path的路径为准的[把md5赋值给picture_path]，picture_md5不重要
    //图片md5
    QByteArray p_sha1=QCryptographicHash::hash(cpicture_path.toUtf8(), QCryptographicHash::Sha1);
    QString picture_md5;
    picture_md5.append(p_sha1.toHex());
    picture_md5="attendance_device/"+picture_md5+".jpg";
    QString pmd5="picture_path="+picture_md5+"&";
    post_data.append(pmd5);

    //图片地址
    QString ppath="picture_md5="+cpicture_path+"&";
    post_data.append(ppath);
#ifdef QT_DEBUG
    qDebug()<<"刷卡图片"<<cpicture_path;
    qDebug()<<"刷卡图片md5"<<picture_md5;
#endif

    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif
    QString ptr = post_and_return(req,post_data);
    if(ptr=="")//超时.
        return -999;

    int ret = replay_return_func(ptr);
    qDebug()<<"刷卡返回"<<ptr;
    return ret;
}

int httpswipecard::replay_return_func(QString ptr)
{
    int ret=status_func(ptr);
    return ret;//0--ok   1--其它参数错误
}
