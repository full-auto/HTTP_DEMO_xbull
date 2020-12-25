#include "httpschooluseritems.h"

HttpschoolUserItems::HttpschoolUserItems()
{
 //   qDebug()<<"schoolUserItems init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.schoolUserItems);
    //   qDebug()<<"schoolUserItems ok";
}

HttpschoolUserItems::~HttpschoolUserItems()
{

}

int HttpschoolUserItems::http_replay_func()
{
    QNetworkRequest req;
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

    int ret=replay_return_func(ptr);
    return ret;
}

int HttpschoolUserItems::replay_return_func(QString ptr)
{
    int ret=status_func(ptr);
    if(ret!=0)
        return ret;

    int v=version.toInt();
    if(v<25)
        low_version_func(ptr);
    else if(v>24&&v<69)
        middle_version_func(ptr);
    else
        high_version_func(ptr);

    return 0;
}

int HttpschoolUserItems::low_version_func(QString ptr)
{
    cleardb();
 //   qDebug()<<"low add school items";
    Json::Reader jr;
    Json::Value jv;
    jr.parse(ptr.toStdString(),jv);
    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mSchoolUserItems");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mSchoolUserItems"));
    /* 检测是否有用户列表，没有则创建 */
    if(!db.tables().contains("SchoolUserItems"))
    {
        query.exec("create table SchoolUserItems(id int,\
                                                type int,\
                                                name string,\
                                                broadcast_name string,\
                                                attendance_label string,\
                                                attendance_label_arr string,\
                                                class_name string,\
                                                face_url string,\
                                                broadcast_position_items string,\
                                                card_items string,\
                                                person_hash string)");
    }

    db.transaction();
    /* 下载对象 */
    HttpGetLoad loadimg;
    /*  */
    Global_class *g_global=Global_class::GetInstance();
//    QMutexLocker locker(&g_global->mutexall);

    int id;/* 用户ID */
    int type;/* 用户类型：0 学生、1 家长、2 职工、3 访客 */
    QString name;/* 用户名字 */
    QString broadcast_name;/* 用户播报名字 */
    QString attendance_label;/* 用户考勤标签 */
    QString attendance_label_arr;/* 多个用户考勤标签，多个存放此处，目前职工为多个考勤标签 */
    QString class_name;/* 用户班级：用户类型为 1, 2, 3 时，为空 */
    QString face_url;/* 用户人脸地址 */
    for(int i=0;i<jv["data"].size();i++)
    {
        /* 用户ID */
        if(jv["data"][i]["id"].isInt())
            id=jv["data"][i]["id"].asInt();
        else
            id=QString::fromStdString(jv["data"][i]["id"].asString()).toInt();
        /* 用户类型：0 学生、1 家长、2 职工、3 访客 */
        if(jv["data"][i]["type"].isInt())
            type=jv["data"][i]["type"].asInt();
        else
            type=QString::fromStdString(jv["data"][i]["type"].asString()).toInt();
        /* 用户名字 */
        name=QString::fromLocal8Bit(jv["data"][i]["name"].asString().data());
        /* 用户播报名字 */
        broadcast_name=QString::fromLocal8Bit(jv["data"][i]["broadcast_name"].asString().data());
        /* 用户考勤标签 */
        attendance_label=QString::fromLocal8Bit(jv["data"][i]["attendance_label"].asString().data());
        /* 多个用户考勤标签，多个存放此处，目前职工为多个考勤标签 */
        attendance_label_arr=QString::fromLocal8Bit(jv["data"][i]["attendance_label_arr"].asString().data());
        /* 用户班级：用户类型为 1, 2, 3 时，为空 */
        class_name=QString::fromLocal8Bit(jv["data"][i]["class_name"].asString().data());
        /* 用户人脸地址 */
        face_url=QString::fromStdString(jv["data"][i]["face_url"].asString());

        bool newfaceimg=0;//新的图片路径是否有区别，不一样或没有才下载注册.
        query.prepare("INSERT INTO SchoolUserItems(id,\
                                                type,\
                                                name,\
                                                broadcast_name,\
                                                attendance_label,\
                                                attendance_label_arr,\
                                                class_name,\
                                                face_url)\
                                        VALUES(:id,\
                                                :type,\
                                                :name,\
                                                :broadcast_name,\
                                                :attendance_label,\
                                                :attendance_label_arr,\
                                                :class_name,\
                                                :face_url)");
        query.bindValue(":id",id);
        query.bindValue(":type",type);
        query.bindValue(":name",name);
        query.bindValue(":broadcast_name",broadcast_name);
        query.bindValue(":attendance_label",attendance_label);
        query.bindValue(":class_name",class_name);
        query.bindValue(":face_url",face_url);

        newfaceimg=1;

        query.exec();

        /* 下载图片，在本线程中下载，直接调用为阻塞，下载完才会继续执行 */
        loadimg.load_flag=-1;//-1默认为图片没有更改，不下载.
        if(face_url=="")//如果图片下载路径为空，不下载
            newfaceimg=0;

        if(newfaceimg)
        {
            loadimg.urladdr=face_url;
            loadimg.run();
        }
        /* 图片注册，也可能在其他线程批量注册，具体看人脸识别模块的功能 */
        if(loadimg.load_flag==0)
        {
            if(g_global->g_HciFunc!=NULL)
            {
                HCM_SQLITE_OPERATE stOperate;
                memset(&stOperate, 0, sizeof(stOperate));
                //人脸库.
                stOperate.stSqConfig.i_iWbmode = FACE_WHITELIST;
                //用户id.
                string idstring=QString::number(id).toStdString();
                strncpy(stOperate.stSqConfig.i_sUserID, idstring.data(), sizeof(stOperate.stSqConfig.i_sUserID)-1 );
                //名字.
                string namestring=name.toStdString();
                strncpy(stOperate.stSqConfig.i_sUserInfo.name, namestring.data(), sizeof(stOperate.stSqConfig.i_sUserInfo.name)-1);

                //人脸图片路径.
           //     qDebug()<<"QT img path:"<<loadimg.imgnamepath;
                std::string lsname=loadimg.imgnamepath.toStdString();
                stOperate.pPicPath=const_cast<char*>(lsname.c_str());
           //     qDebug()<<"arm ing path:"<<stOperate.pPicPath;
                //添加用户.
                //ret = AddUser( &stOperate );
                HCM_MSG_HEAD_S stMsgHead;
                memset(&stMsgHead, 0, sizeof(stMsgHead));
                stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules
                stMsgHead.u32MsgType = IN_SQLITE_PICTURE_ADD;// IN_SQLITE_USER_INFO_UPDATA;//IN_SQLITE_PICTURE_ADD;
                if(g_global->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, &stOperate, sizeof(HCM_SQLITE_OPERATE), NULL, 0)!=0)
                {
                    //人脸注册失败，记录到注册失败表.
                    //qDebug()<<"add user face fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
                }
            }
        }
        else if(loadimg.load_flag==1)
        {
            /* 下载失败，把该信息记录到下载失败记录表 */
        }
    }
    db.commit();
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSchoolUserItems");
    return 0;
}

int HttpschoolUserItems::middle_version_func(QString ptr)
{
    //qDebug()<<"middle add school items";
    cleardb();
    Json::Reader jr;
    Json::Value jv;
    jr.parse(ptr.toStdString(),jv);

    QString data_compress=QString::fromStdString(jv["data"].asString());
    QByteArray d1=data_compress.toLocal8Bit();
    QByteArray d2=QByteArray::fromBase64(d1);
    QString d_uncompress=gzipUncompress(d2);
    Json::Value jd;
    jr.parse(d_uncompress.toStdString(),jd);

    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mSchoolUserItems");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mSchoolUserItems"));
    /* 检测是否有用户列表，没有则创建 */
    if(!db.tables().contains("SchoolUserItems"))
    {
        query.exec("create table SchoolUserItems(id int,\
                                                type int,\
                                                name string,\
                                                broadcast_name string,\
                                                attendance_label string,\
                                                attendance_label_arr string,\
                                                class_name string,\
                                                face_url string,\
                                                broadcast_position_items string,\
                                                card_items string,\
                                                person_hash string)");
    }

    db.transaction();
    /* 下载对象 */
    HttpGetLoad loadimg;
    /*  */
    Global_class *g_global=Global_class::GetInstance();
//    QMutexLocker locker(&g_global->mutexall);

    int id;// 用户ID
    int type;// 用户类型：0 学生、1 家长、2 职工、3 访客
    QString name;// 用户名字
    QString broadcast_name;// 用户播报名字
    QString attendance_label;// 用户考勤标签
    QString attendance_label_arr;// 多个用户考勤标签，多个存放此处，目前职工为多个考勤标签 array*/
    QString class_name;// 用户班级：用户类型为 1, 2, 3 时，为空 */
    QString face_url;// 用户人脸地址
    QString broadcast_position_items;/*播报位置 array*/
    QString card_items;//卡信息 array*/
    QString person_hash;// 人物 hash 值，用于校验是否跟平台数据一致性 */
    for(int i=0;i<jd.size();i++)
    {
        // 用户ID
        if(jd[i]["id"].isInt())
            id=jd[i]["id"].asInt();
        else
            id=QString::fromStdString(jd[i]["id"].asString()).toInt();
        // 用户类型：0 学生、1 家长、2 职工、3 访客
        if(jd[i]["type"].isInt())
            type=jd[i]["type"].asInt();
        else
            type=QString::fromStdString(jd[i]["type"].asString()).toInt();
        // 用户名字
        name=QString::fromLocal8Bit(jd[i]["name"].asString().data());
        // 用户播报名字
        broadcast_name=QString::fromLocal8Bit(jd[i]["broadcast_name"].asString().data());
        // 用户考勤标签
        attendance_label=QString::fromLocal8Bit(jd[i]["attendance_label"].asString().data());
        // 多个用户考勤标签，多个存放此处，目前职工为多个考勤标签
        attendance_label_arr=QString::fromStdString(Json::FastWriter().write(jd[i]["attendance_label_arr"]));
        // 用户班级：用户类型为 1, 2, 3 时，为空
        class_name=QString::fromLocal8Bit(jd[i]["class_name"].asString().data());
        // 用户人脸地址
        face_url=QString::fromStdString(jd[i]["face_url"].asString());
        // 播报位置
        broadcast_position_items=QString::fromStdString(Json::FastWriter().write(jd[i]["broadcast_position_items"]));
        // 卡信息
        card_items=QString::fromStdString(Json::FastWriter().write(jd[i]["card_items"]));
        // 人物 hash 值
        person_hash=QString::fromStdString(jd[i]["person_hash"].asString());
        bool newfaceimg=0;//新的图片路径是否有区别，不一样或没有才下载注册.

        // 用户添加
        newfaceimg=1;
        query.prepare("INSERT INTO SchoolUserItems(id,\
                                                type,\
                                                name,\
                                                broadcast_name,\
                                                attendance_label,\
                                                attendance_label_arr,\
                                                class_name,\
                                                face_url,\
                                                broadcast_position_items,\
                                                card_items,\
                                                person_hash)\
                                        VALUES(:id,\
                                               :type,\
                                               :name,\
                                               :broadcast_name,\
                                               :attendance_label,\
                                               :attendance_label_arr\
                                               :class_name,\
                                               :face_url,\
                                               :broadcast_position_items,\
                                               :card_items,\
                                               :person_hash)");
        query.bindValue(":id",id);
        query.bindValue(":type",type);
        query.bindValue(":name",name);
        query.bindValue(":broadcast_name",broadcast_name);
        query.bindValue(":attendance_label",attendance_label);
        query.bindValue(":attendance_label_arr",attendance_label_arr);
        query.bindValue(":class_name",class_name);
        query.bindValue(":face_url",face_url);
        query.bindValue(":broadcast_position_items",broadcast_position_items);
        query.bindValue(":card_items",card_items);
        query.bindValue(":person_hash",person_hash);
        query.exec();

        /* 下载图片，在本线程中下载，直接调用为阻塞，下载完才会继续执行 */
        loadimg.load_flag=-1;//-1默认为图片没有更改，不下载.
        if(face_url=="")//如果图片下载路径为空，不下载
            newfaceimg=0;
        if(newfaceimg)//newfaceimg为1时下载--图片路径变化，新加用户才下载
        {
            loadimg.urladdr=face_url;
            loadimg.run();
        }
        /* 图片注册，也可能在其他线程批量注册，具体看人脸识别模块的功能 */
        if(loadimg.load_flag==0)
        {
            if(g_global->g_HciFunc!=NULL)
            {
                HCM_SQLITE_OPERATE stOperate;
                memset(&stOperate, 0, sizeof(stOperate));
                //人脸库.
                stOperate.stSqConfig.i_iWbmode = FACE_WHITELIST;
                //用户id.
                string idstring=QString::number(id).toStdString();
                strncpy(stOperate.stSqConfig.i_sUserID, idstring.data(), sizeof(stOperate.stSqConfig.i_sUserID)-1 );
                //名字.
                string namestring=name.toStdString();
                strncpy(stOperate.stSqConfig.i_sUserInfo.name, namestring.data(), sizeof(stOperate.stSqConfig.i_sUserInfo.name)-1);

                //人脸图片路径.
                //qDebug()<<"QT img path:"<<loadimg.imgnamepath;
                std::string lsname=loadimg.imgnamepath.toStdString();
                stOperate.pPicPath=const_cast<char*>(lsname.c_str());
                //qDebug()<<"arm ing path:"<<stOperate.pPicPath;
                //添加用户.
                //ret = AddUser( &stOperate );
                HCM_MSG_HEAD_S stMsgHead;
                memset(&stMsgHead, 0, sizeof(stMsgHead));
                stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules
                stMsgHead.u32MsgType = IN_SQLITE_PICTURE_ADD;// IN_SQLITE_USER_INFO_UPDATA;//IN_SQLITE_PICTURE_ADD;
                if(g_global->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, &stOperate, sizeof(HCM_SQLITE_OPERATE), NULL, 0)!=0)
                {
                    //人脸注册失败，记录到注册失败表.
                    //qDebug()<<"add user face fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
                }
            }
        }
        else if(loadimg.load_flag==1)
        {
            /* 下载失败，把该信息记录到下载失败记录表 */
        }
    }
    db.commit();
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSchoolUserItems");
    return 0;
}

int HttpschoolUserItems::high_version_func(QString ptr)
{
 //   qDebug()<<"high add school items";
    Json::Reader jr;
    Json::Value jv;
    jr.parse(ptr.toStdString(),jv);

    QString data_compress=QString::fromStdString(jv["data"].asString());
    //给 根据ID获得用户数据 分配函数 进行处理.
    comperr_json_alldata=data_compress;
    return 0;
}

int HttpschoolUserItems::cleardb()
{
#ifdef QT_DEBUG
    qDebug()<<"clear db * SchoolUserItems Synckey";
#endif

    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mSchoolUserItems");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mSchoolUserItems"));
    QString sqlstr;
    /* 检测是否有用户列表 */
    if(db.tables().contains("SchoolUserItems"))
    {
        query.exec("delete from SchoolUserItems");
        query.exec("update sqlite_sequence SET seq = 0 where name ='SchoolUserItems'");
    }
    if(db.tables().contains("Synckey"))
    {
        query.exec("delete from Synckey");
        query.exec("update sqlite_sequence SET seq = 0 where name ='Synckey'");
    }
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSchoolUserItems");

    if(version.toInt()<69)
    {
        //清空AI人脸--具体参数看HCM_MSG_HEAD_S
        Global_class *t1_glo=Global_class::GetInstance();
//        QMutexLocker locker(&t1_glo->mutexall);
        HCM_MSG_HEAD_S stMsgHead;
        HCM_SQLITE_OPERATE stOperate;
        memset(&stMsgHead, 0, sizeof(stMsgHead));
        memset(&stOperate, 0, sizeof(stOperate));
        stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules.
        stMsgHead.u32MsgType = OUT_SQLITE_PICTURE_DELLIST;//删除名单列表.
        stOperate.stSqConfig.i_iWbmode = FACE_WHITELIST; //白名单.
      //  qDebug()<<"do delete allusers:";
        int p = t1_glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, &stOperate, sizeof(HCM_SQLITE_OPERATE), NULL, 0);
      //  qDebug()<<"Intf_SendModuleMsg over:"<<p;
    }
}

