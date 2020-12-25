#include "httpreportstatus.h"

httpreportStatus::httpreportStatus()
{
   // qDebug()<<"reportStatus init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.reportStatus);
    //   qDebug()<<"reportStatus ok";
}

httpreportStatus::~httpreportStatus()
{

}

int httpreportStatus::http_replay_func()
{
  //  qDebug()<<"report status";

    config_status=1;
    for(int i=0;i<4;i++)
        usertype_status[i]=1;

    QNetworkRequest req;
    QUrl ht=http_url;
    QByteArray post_data;

    //req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));

    int type_count[4]={0,0,0,0};
    QString p_hash[4]={"","","",""};
    QString d_hash;

    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","ALLT");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("ALLT"));

    db.transaction();
    query.exec("SELECT id,type,person_hash from SchoolUserItems ORDER BY id asc");
    while(query.next())
    {
        int type=query.value(1).toInt();
        QString phash=query.value(2).toString();
        type_count[type]++;
        p_hash[type]=p_hash[type]+phash;
//        qDebug()<<query.value(0).toInt()<<"|type:"<<type<<"|"<<"p-hash"<<phash;
    }

    query.exec("SELECT config_hash from DeviceConfig ORDER BY id asc");
    while(query.next())
    {
        d_hash=query.value(0).toString();
        break;
    }
    db.commit();

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("ALLT");

    QString p_hash_md5[4]={"","","",""};
    for(int i=0;i<4;i++)
    {
        if(type_count[i]>0)
        {
            QByteArray p_md5=QCryptographicHash::hash(p_hash[i].toUtf8(), QCryptographicHash::Md5);
            p_hash_md5[i].append(p_md5.toHex());
        }
//        qDebug()<<"user num type :"<<i<<"|"<<type_count[i];
//        qDebug()<<p_hash_md5[i];
    }

    //设备号
    QString pnumber="number="+net_MAC+"&";
    post_data.append(pnumber);
    //版本号
    QString pversion="version="+version+"&";
    post_data.append(pversion);
    //学生总数和hash值
    QString pstudent_count="student_count="+QString::number(type_count[0])+"&";
    post_data.append(pstudent_count);
    QString pstudent_hash="student_hash="+p_hash_md5[0]+"&";
    post_data.append(pstudent_hash);
    //家长总数和hash值
    QString pparent_count="parent_count="+QString::number(type_count[1])+"&";
    post_data.append(pparent_count);
    QString pparent_hash="parent_hash="+p_hash_md5[1]+"&";
    post_data.append(pparent_hash);
    //职工总数和hash值
    QString pstaff_count="staff_count="+QString::number(type_count[2])+"&";
    post_data.append(pstaff_count);
    QString pstaff_hash="staff_hash="+p_hash_md5[2]+"&";
    post_data.append(pstaff_hash);
    //访客总数和hash值
    QString pvisitor_count="visitor_count="+QString::number(type_count[3])+"&";
    post_data.append(pvisitor_count);
    QString pvisitor_hash="visitor_hash="+p_hash_md5[3]+"&";
    post_data.append(pvisitor_hash);
    //设备设置hash值
    QString config_hash="config_hash="+d_hash+"&";
    post_data.append(config_hash);

    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif

    QString ptr = post_and_return(req,post_data);
    if(ptr=="")//超时.
        return -999;

    int ret=replay_return_func(ptr);
    return ret;
}

int httpreportStatus::replay_return_func(QString ptr)
{
    int ret=status_func(ptr);
    if(ret!=0)
        return ret;

    Json::Reader jr;
    Json::Value jv;
    if(!jr.parse(ptr.toStdString(),jv))
    {
        return -999;
    }

    if(jv["data"]["student_status"].isInt())
        usertype_status[0]=jv["data"]["student_status"].asInt();
    else if(jv["data"]["student_status"].isString())
        usertype_status[0]=QString::fromStdString(jv["data"]["student_status"].asString()).toInt();

    if(jv["data"]["parent_status"].isInt())
        usertype_status[1]=jv["data"]["parent_status"].asInt();
    else if(jv["data"]["parent_status"].isString())
        usertype_status[1]=QString::fromStdString(jv["data"]["parent_status"].asString()).toInt();

    if(jv["data"]["staff_status"].isInt())
        usertype_status[2]=jv["data"]["staff_status"].asInt();
    else if(jv["data"]["staff_status"].isString())
        usertype_status[2]=QString::fromStdString(jv["data"]["student_status"].asString()).toInt();

    if(jv["data"]["visitor_status"].isInt())
        usertype_status[3]=jv["data"]["visitor_status"].asInt();
    else if(jv["data"]["student_status"].isString())
        usertype_status[3]=QString::fromStdString(jv["data"]["visitor_status"].asString()).toInt();

    if(jv["data"]["config_status"].isInt())
        config_status=jv["data"]["config_status"].asInt();
    else if(jv["data"]["config_status"].isString())
        config_status=QString::fromStdString(jv["data"]["config_status"].asString()).toInt();

    return 0;
}
