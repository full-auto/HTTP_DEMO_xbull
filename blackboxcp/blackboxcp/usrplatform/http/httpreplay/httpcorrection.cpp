#include "httpcorrection.h"

httpcorrection::httpcorrection()
{
    //qDebug()<<"correction init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.correctionStudent);
    //qDebug()<<"correction ok";
}

httpcorrection::~httpcorrection()
{

}

int httpcorrection::http_replay_func()
{
#ifdef QT_DEBUG
    qDebug()<<"correction func";
#endif

    QNetworkRequest req;
    QUrl ht;
    BASE_URL h_url;
    HTTPURL allurl;
    QString usertypeurl;
    if(type==0)
        usertypeurl=QString::fromStdString(allurl.correctionStudent);
    else if(type==1)
        usertypeurl=QString::fromStdString(allurl.correctionParent);
    else if(type==2)
        usertypeurl=QString::fromStdString(allurl.correctionStaff);
    else
        usertypeurl=QString::fromStdString(allurl.correctionVisitor);
    http_url.clear();
    if(BASE==1)
        http_url=QString::fromStdString(h_url.BASE_URL_TEST)+usertypeurl;
    else if(BASE==2)
        http_url=QString::fromStdString(h_url.BASE_URL_GRAY)+usertypeurl;
    else if(BASE==3)
        http_url=QString::fromStdString(h_url.BASE_URL_PRODUCT)+usertypeurl;
    ht=http_url;

    QString ysdata=user_hj;
    //gzip压缩
    QByteArray gzip_userhash=gzipCompress(ysdata.toLatin1());
    //base64加密
    QByteArray base64_userhash=gzip_userhash.toBase64();
    //
    QString hj_str;
    hj_str.append(base64_userhash);
//    qDebug()<<"json  gzip压缩+base64加密:"<<hj_str;

    //把"+"号都替换成"%2B"，否则在服务器获得的是空格
    QString hj_th=hj_str.replace("+","%2B");
//    qDebug()<<"+替换后:"<<hj_th;

    QByteArray post_data;
    QString number="number="+net_MAC+"&";
    post_data.append(number);
    QString pversion="version="+version+"&";
    post_data.append(pversion);
    QString correction_data="correction_data="+hj_th+"&";
    post_data.append(correction_data);

    req.setUrl(ht);
#ifdef QT_DEBUG
    qDebug()<<ht;
#endif

    QString ptr = post_and_return(req,post_data);

    if(ptr=="")//超时.
    {
        return -999;
    }
    int ret=replay_return_func(ptr);

    return ret;
}

int httpcorrection::replay_return_func(QString ptr)
{
    int ret=status_func(ptr);
    if(ret!=0)
        return ret;

    Json::Reader jr;
    Json::Value jv;
    if(jr.parse(ptr.toStdString(),jv))
    {
        QString data_compress=QString::fromStdString(jv["data"].asString());
        //给 根据ID获得用户数据 分配函数 进行处理.
        correc_date=data_compress;
    }

    return 0;
}

int httpcorrection::correction_func(int t)
{
    type=t;
    correc_date.clear();
    Json::Value j_data;//
    user_hj.clear();
    {/* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mSchoolUserItems");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mSchoolUserItems"));
    db.transaction();

    query.prepare("select id,person_hash from SchoolUserItems where type=:type");//ORDER BY id asc
    query.bindValue(":type",type);
    query.exec();
    while(query.next())
    {
        int user_id=query.value(0).toInt();
        QString p_hash=query.value(1).toString();

        Json::Value person;
        person["user_id"]=user_id;
        person["person_hash"]=p_hash.toStdString();
//        qDebug()<<"id:"<<user_id<<"hash:"<<p_hash;
        j_data.append(person);
    }

    db.commit();
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSchoolUserItems");

    user_hj="";
    if(j_data.size()>0)
       user_hj=QString::fromStdString(Json::FastWriter().write(j_data));

//    qDebug()<<"json数据:"<<user_hj;
    int ret=http_replay_func();
    return ret;
}
