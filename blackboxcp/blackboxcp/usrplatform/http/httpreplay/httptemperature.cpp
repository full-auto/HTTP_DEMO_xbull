#include "httptemperature.h"

httptemperature::httptemperature()
{
    HTTPURL allurl;
    init(allurl.temperature);
}

httptemperature::~httptemperature()
{

}

int httptemperature::http_replay_func()
{
    QNetworkRequest req;
    QUrl ht=http_url;
    QByteArray post_data;

    //设备号
    QString pnumber="number="+net_MAC+"&";
    post_data.append(pnumber);
    //用户id
    QString puser_id="user_id="+QString::number(userid)+"&";
    post_data.append(puser_id);
    //用户类型
    QString puser_type="user_type="+QString::number(usertype)+"&";
    post_data.append(puser_type);
    //温度--精度小数点后1位
    QString ptemperature="temperature="+QString::number(temperature,'f',1)+"&";
    post_data.append(ptemperature);
    //测温时间
    QString ptemperature_at="temperature_at="+QString::number(temperature_t)+"&";
    post_data.append(ptemperature_at);

    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif
    QString ptr = post_and_return(req,post_data);
    if(ptr=="")//超时.
        return -999;

    int ret = replay_return_func(ptr);
    return ret;
}

int httptemperature::replay_return_func(QString ptr)
{
    int ret=status_func(ptr);
    return ret;//0--ok   -1--找不到用户信息，上传温度失败
}
