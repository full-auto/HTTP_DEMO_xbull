#include "httpitemsbyid.h"

httpItemsByID::httpItemsByID()
{
 //   qDebug()<<"httpItemsByID init:"<<QThread::currentThread();
    HTTPURL allurl;
    init(allurl.schoolStudentItemsByIdItems);

    addusernum=0;
    subusernum=0;
    thisnum=0;

    glo=Global_class::GetInstance();

    //  qDebug()<<"httpItemsByID ok";
}

httpItemsByID::~httpItemsByID()
{

}

int httpItemsByID::http_replay_func()
{

#ifdef QT_DEBUG
    qDebug()<<"httpItemsByID";
#endif
    QNetworkRequest req;
    QUrl ht;
    BASE_URL h_url;
    HTTPURL allurl;
    QString usertypeurl;
    if(usertype==0)
        usertypeurl=QString::fromStdString(allurl.schoolStudentItemsByIdItems);
    else if(usertype==1)
        usertypeurl=QString::fromStdString(allurl.schoolParentItemsByIdItems);
    else if(usertype==2)
        usertypeurl=QString::fromStdString(allurl.schoolStaffItemsByIdItems);
    else
        usertypeurl=QString::fromStdString(allurl.schoolVisitorItemsByIdItems);
    
    http_url.clear();
    if(BASE==1)
        http_url=QString::fromStdString(h_url.BASE_URL_TEST)+usertypeurl;
    else if(BASE==2)
        http_url=QString::fromStdString(h_url.BASE_URL_GRAY)+usertypeurl;
    else if(BASE==3)
        http_url=QString::fromStdString(h_url.BASE_URL_PRODUCT)+usertypeurl;
    ht=http_url;
    
    QByteArray post_data;
    QString number="number="+net_MAC+"&";
    post_data.append(number);
    QString user_id_items="user_id_items="+id_json+"&";
    post_data.append(user_id_items);
    
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

int httpItemsByID::replay_return_func(QString ptr)
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

    QString data_compress=QString::fromStdString(jv["data"].asString());
    QByteArray d1=data_compress.toLocal8Bit();
    QByteArray d2=QByteArray::fromBase64(d1);
    QString d_uncompress=gzipUncompress(d2);
#ifdef QT_DEBUG
    qDebug()<<"httpItemsByID uncomp data";
    qDebug()<<"请求返回的数据解压后：";
    qDebug()<<"uncompress data:-----------------------------";
    qDebug()<<d_uncompress;
#endif

    Json::Value jd;
    jr.parse(d_uncompress.toStdString(),jd);
    
    //qDebug()<<"httpItemsByID go";
    {/* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mSchoolUserItems");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mSchoolUserItems"));
    /* 检测是否有用户列表，没有则创建 */
    if(!db.tables().contains("SchoolUserItems"))
    {
        //qDebug()<<"create SchoolUserItems----------------------------";
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

    for(int i=0;i<jd.size();i++)
    {
        thisnum++;

        /* 用户ID */
        int id;
        if(jd[i]["id"].isInt())
            id=jd[i]["id"].asInt();
        else
            id=QString::fromStdString(jd[i]["id"].asString()).toInt();
#ifdef QT_DEBUG
        qDebug()<<"zlgx id:"<<id;
#endif
        /* 用户类型：0 学生、1 家长、2 职工、3 访客 */
        int type;
        if(jd[i]["type"].isInt())
            type=jd[i]["type"].asInt();
        else
            type=QString::fromStdString(jd[i]["type"].asString()).toInt();
        /* 用户名字 */
        QString name=QString::fromLocal8Bit(jd[i]["name"].asString().data());
        /* 用户播报名字 */
        QString broadcast_name=QString::fromLocal8Bit(jd[i]["broadcast_name"].asString().data());
        /* 用户考勤标签 */
        QString attendance_label=QString::fromLocal8Bit(jd[i]["attendance_label"].asString().data());
        /* 多个用户考勤标签，多个存放此处，目前职工为多个考勤标签 */
        QString attendance_label_arr=QString::fromStdString(Json::FastWriter().write(jd[i]["attendance_label_arr"]));
        /* 用户班级：用户类型为 1, 2, 3 时，为空 */
        QString class_name=QString::fromLocal8Bit(jd[i]["class_name"].asString().data());
        /* 用户人脸地址 */
        QString face_url=QString::fromStdString(jd[i]["face_url"].asString());
        /* 播报 */
        QString broadcast_position_items=QString::fromStdString(Json::FastWriter().write(jd[i]["broadcast_position_items"]));
        /* 卡信息 */
        QString card_items=QString::fromStdString(Json::FastWriter().write(jd[i]["card_items"]));
        /* 人物 hash 值 */
        QString person_hash=QString::fromStdString(jd[i]["person_hash"].asString());

        //新旧的图片路径是否有区别
        //如果为已有用户
        //-1新旧没变化但为空,说明在AI中并没有该图片，AI不做处理
        //0新旧没变化，且不为空,不下载图片，仅更新AI的用户数据
        //1新旧有变化，且新不为空，下载并在AI里面进行更新
        //  --因为不判断之前AI是否已经有该人脸成功录入，旧用户统一先删除，再录入
        //2新旧有变化，但新的为空，不下载，且在AI里把旧的删除
        //如果为新用户
        //3有图片，下载并录入AI模块
        //4无图片，不下载，AI不做处理
        int newfaceimg=-1;

        /* 如果在该用户类型有相同的id */
        query.prepare("select face_url from SchoolUserItems where id=:id and type=:type");
        query.bindValue(":id",id);
        query.bindValue(":type",type);
        query.exec();
        if(query.next())
        {
            //判断新旧的图片路径是否一致
            QString old_imgurl=query.value(0).toString();
//            qDebug()<<"old imgurl:"<<old_imgurl;
//            qDebug()<<"new imgurl:"<<face_url;
            if(old_imgurl==face_url && face_url!="")
                newfaceimg=0;
            else if(old_imgurl!=face_url && face_url!="")
                newfaceimg=1;
            else if(old_imgurl!=face_url && face_url=="")
                newfaceimg=2;

            query.prepare("update SchoolUserItems set name=:name,\
                                                            broadcast_name=:broadcast_name,\
                                                            attendance_label=:attendance_label,\
                                                            attendance_label_arr=:attendance_label_arr,\
                                                            class_name=:class_name,\
                                                            face_url=:face_url,\
                                                            broadcast_position_items=:broadcast_position_items,\
                                                            card_items=:card_items,\
                                                            person_hash=:person_hash \
                                                            where id=:id and type=:type");
            query.bindValue(":name",name);
            query.bindValue(":broadcast_name",broadcast_name);
            query.bindValue(":attendance_label",attendance_label);
            query.bindValue(":attendance_label_arr",attendance_label_arr);
            query.bindValue(":class_name",class_name);
            query.bindValue(":face_url",face_url);
            query.bindValue(":broadcast_position_items",broadcast_position_items);
            query.bindValue(":card_items",card_items);
            query.bindValue(":person_hash",person_hash);
            query.bindValue(":id",id);
            query.bindValue(":type",type);
            query.exec();
        }
        else/* 没有该用户则添加 */
        {
            //判断该用户是否有人脸图
            if(face_url!="")
                newfaceimg=3;
            else
                newfaceimg=4;

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
                                                   :attendance_label_arr,\
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
        }
#ifdef QT_DEBUG
        qDebug()<<"will to do newfaceimg:"<<newfaceimg;
#endif

        if(newfaceimg==0)//更新人脸相关的信息--但不更新人脸图片
        {
            //流程逻辑：获取AI内信息列表(包含所有用户)，查找需要修改的用户，获取该用户的详细信息，再把新的信息赋值

            HCM_SQLITE_OPERATE stOperate;
            memset(&stOperate, 0, sizeof(stOperate));
            //人脸库. 白名单
            stOperate.stSqConfig.i_iWbmode = FACE_WHITELIST;

            HCM_MSG_HEAD_S stMsgHead;
            memset(&stMsgHead, 0, sizeof(stMsgHead));
            stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules
            stMsgHead.u32MsgType = OUT_SQLITE_LISTINFO;//获得AI模块数据库的用户列表

            //获取目前AI里面的用户信息
            AI_SQILE_USERINFO *ai_userinfo=NULL;//信息
            unsigned int nCounttemp = 0;//数量
            string fid=QString::number(id).toStdString()+QString::number(type).toStdString();
            const char* findid = fid.c_str();
            if(glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, NULL, 0, (VOID **)&ai_userinfo, &nCounttemp)>=0)
            {
//                qDebug()<<"nCounttemp:"<<nCounttemp;
                for(int i=0;i<nCounttemp;i++)
                {
                    if(strcmp( ai_userinfo[i].usrid, findid ) == 0)//查找要更新ID的对应信息
                    {
                        //把查询到的信息进行赋值
                        memcpy(&stOperate.stSqConfig.i_sUserInfo,&ai_userinfo[i],sizeof(ai_userinfo[i]));

                        //即将进行信息更新的用户ID
                        //用户id. id+type组合
                        string idstring=QString::number(id).toStdString()+QString::number(type).toStdString();
                        strncpy(stOperate.stSqConfig.i_sUserID, idstring.data(), sizeof(stOperate.stSqConfig.i_sUserID)-1 );
                        //修改需要更新的信息
                        //更新后的名字.
                        string namestring=name.toStdString();
                        strncpy(stOperate.stSqConfig.i_sUserInfo.name, namestring.data(), sizeof(stOperate.stSqConfig.i_sUserInfo.name)-1);
                        //更新后的班级
                        string nameclassstring=class_name.toStdString();
                        if(type==3)
                            nameclassstring="访客";
                        strncpy(stOperate.stSqConfig.i_sUserInfo.areaAddress, nameclassstring.data(), sizeof(stOperate.stSqConfig.i_sUserInfo.areaAddress)-1);

                        stMsgHead.u32MsgType = IN_SQLITE_USER_INFO_UPDATA;//更新
                        int ud=glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, &stOperate, sizeof(HCM_SQLITE_OPERATE), NULL, 0);
                        if(ud!=0)//更新失败
                        {
#ifdef QT_DEBUG
                            qDebug()<<"update face data fail!!!!!!!!!!!!!!!!!";
#endif
                        }
                        break;
                    }
                }
            }
            else
            {
#ifdef QT_DEBUG
                qDebug()<<"获取AI人脸库的用户列表失败!!!";
#endif
            }
        }
        else if(newfaceimg==2)//删除人脸
        {
            //先删除已有的
            HCM_SQLITE_OPERATE stOperate;
            memset(&stOperate, 0, sizeof(stOperate));
            //人脸库. 白名单
            stOperate.stSqConfig.i_iWbmode = FACE_WHITELIST;
            //用户id. id+type组合
            string idstring=QString::number(id).toStdString()+QString::number(type).toStdString();
            strncpy(stOperate.stSqConfig.i_sUserID, idstring.data(), sizeof(stOperate.stSqConfig.i_sUserID)-1 );

            HCM_MSG_HEAD_S stMsgHead;
            memset(&stMsgHead, 0, sizeof(stMsgHead));
            stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules
            //
            stMsgHead.u32MsgType = OUT_SQLITE_PICTURE_DELONE;
            if(glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, &stOperate, sizeof(HCM_SQLITE_OPERATE), NULL, 0)!=0)
            {
                //qDebug()<<"delete fail";
            }
        }
        else if(newfaceimg==1||newfaceimg==3)
        {
            /* 下载对象 */
            HttpGetLoad loadimg;
            /* 下载图片，在本线程中下载，直接调用为阻塞，下载完才会继续执行 */
            loadimg.urladdr=face_url;
            loadimg.run();

            int addface=0;
            int face_ret=0;

            if(loadimg.load_flag)
            {
                if(newfaceimg==1)//已有用户更新信息和图片
                {
                    //先删除已有的
                    HCM_SQLITE_OPERATE stOperate0;
                    memset(&stOperate0, 0, sizeof(stOperate0));
                    //人脸库. 白名单
                    stOperate0.stSqConfig.i_iWbmode = FACE_WHITELIST;
                    //用户id. id+type组合
                    string idstring=QString::number(id).toStdString()+QString::number(type).toStdString();
                    strncpy(stOperate0.stSqConfig.i_sUserID, idstring.data(), sizeof(stOperate0.stSqConfig.i_sUserID)-1 );

                    HCM_MSG_HEAD_S stMsgHead0;
                    memset(&stMsgHead0, 0, sizeof(stMsgHead0));
                    stMsgHead0.u32DestModule = HCM_AIMODULE_E; //发送给ai modules
                    //
                    stMsgHead0.u32MsgType = OUT_SQLITE_PICTURE_DELONE;
                    if(glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead0, &stOperate0, sizeof(HCM_SQLITE_OPERATE), NULL, 0)!=0)
                    {
                        //qDebug()<<"delete fail";
                    }
                }

                HCM_SQLITE_OPERATE stOperate;
                memset(&stOperate, 0, sizeof(stOperate));
                //人脸库. 白名单
                stOperate.stSqConfig.i_iWbmode = FACE_WHITELIST;
                //用户id. id+type组合
                string idstring=QString::number(id).toStdString()+QString::number(type).toStdString();
                strncpy(stOperate.stSqConfig.i_sUserID, idstring.data(), sizeof(stOperate.stSqConfig.i_sUserID)-1 );
                //名字.
                string namestring=name.toStdString();
                strncpy(stOperate.stSqConfig.i_sUserInfo.name, namestring.data(), sizeof(stOperate.stSqConfig.i_sUserInfo.name)-1);
                //班级
                string nameclassstring=class_name.toStdString();
                if(type==3)
                    nameclassstring="访客";
                strncpy(stOperate.stSqConfig.i_sUserInfo.areaAddress, nameclassstring.data(), sizeof(stOperate.stSqConfig.i_sUserInfo.areaAddress)-1);
                //人脸图片路径.
                std::string lsname=loadimg.imgnamepath.toStdString();
                stOperate.pPicPath=const_cast<char*>(lsname.c_str());

                HCM_MSG_HEAD_S stMsgHead;
                memset(&stMsgHead, 0, sizeof(stMsgHead));
                stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules
                stMsgHead.u32MsgType = IN_SQLITE_PICTURE_ADD;

                face_ret=glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, &stOperate, sizeof(HCM_SQLITE_OPERATE), NULL, 0);
                if(face_ret!=0)
                {
                    addface=1;
                    //qDebug()<<"add user face NGNGNGNGNGNGNGNGNG--------------------------------";
                    failstr=failstr+" | add face to AI NG:"+QString::number(id)+"-"+name+"-err:"+QString::number(face_ret);
                    //
                }
            }
            else
            {
                addface=2;
                /* 下载失败 */
                failstr=failstr+" | load face img NG:"+QString::number(id)+"-"+name;
            }

            if(addface!=0)
            {
                //下载图片失败 或者 ai注册人脸识别，清空这个用户的 人脸路径[必须] 和 哈希值[或者乱改哈希值，反正就是触发修复机制就行]

                query.prepare("update SchoolUserItems set face_url=:face_url, person_hash=:person_hash \
                                                            where id=:id and type=:type");
                query.bindValue(":face_url","");
                query.bindValue(":person_hash","");//666
                query.bindValue(":id",id);
                query.bindValue(":type",type);

                //需要记录上传到日志
            }
        }
        //界面提示百分比，不再使用1秒更新一次，因为是同一个线程,如果注册时耗时长，导致1秒一次会堆积太多信号在事件列表中
        emit update100();
    }
    db.commit();
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSchoolUserItems");

    return 0;
}

int httpItemsByID::all_updata_func()
{
    int ret=0;
    cleardb();//全量更新是，先清空.
    QByteArray d1=comp_data.toLocal8Bit();
    QByteArray d2=QByteArray::fromBase64(d1);
    QString d_uncompress=gzipUncompress(d2);
    //解压后的数据
#ifdef QT_DEBUG
    qDebug()<<"全量更新";
    qDebug()<<"解压后的数据 all in data unc:";
    qDebug()<<d_uncompress;
#endif
    
    Json::Reader jr;
    Json::Value jv;
    if(!jr.parse(d_uncompress.toStdString(),jv))
    {
//        qDebug()<<"no json";
        return -999;
    }
    
    {/* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mSynckey");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mSynckey"));
    /* 检测是否有Synckey记录列表，没有则创建 */
    if(!db.tables().contains("Synckey"))
    {
        query.exec("create table Synckey(synckey int)");
    }
    /* 单个设备信息，当前做法直接把原来的清空把新的放进去 */
    int synckey;
    if(jv["synckey"].isInt())
        synckey=jv["synckey"].asInt();
    else
        synckey=QString::fromStdString(jv["synckey"].asString()).toInt();

    query.exec("delete from Synckey");
    query.exec("update sqlite_sequence SET seq = 0 where name ='Synckey'");
    query.prepare("INSERT INTO Synckey(synckey)VALUES(:synckey)");
    query.bindValue(":synckey",synckey);
//    qDebug()<<"synckey:"<<synckey;
    query.exec();
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSynckey");

    Json::Value j_data[4];//分别对应4个类型的用户.

    addusernum=jv["items"].size();
    thisnum=0;

    for(int i=0;i<jv["items"].size();i++)
    {
        //获得ID.
        QString u_id;
        if(jv["items"][i]["id"].isInt())
            u_id=QString::number(jv["items"][i]["id"].asInt());
        else
            u_id=QString::fromStdString(jv["items"][i]["id"].asString());
        //获得类型.
        int u_type;
        if(jv["items"][i]["type"].isInt())
            u_type=jv["items"][i]["type"].asInt();
        else
            u_type=QString::fromStdString(jv["items"][i]["type"].asString()).toInt();

        j_data[u_type].append(u_id.toStdString());
    }
    
    Json::Value j_get_data;
    for(int typenum=0;typenum<4;typenum++)
    {
        j_get_data.clear();
        for(int i=0;i<j_data[typenum].size();i++)
        {
            j_get_data.append(j_data[typenum][i]);
            if(j_get_data.size()==4000)//服务器ID集合一次最多可接受4000个.
            {
                //qDebug()<<"add id to userinfo--type:"<<typenum;
                usertype=typenum;
                id_json=QString::fromStdString(Json::FastWriter().write(j_get_data));
                if(http_replay_func()!=0)
                    ret++;
                j_get_data.clear();
            }
        }
        if(j_get_data.size()>0)
        {
            //qDebug()<<"add id to userinfo--type:"<<typenum;
            usertype=typenum;
            id_json=QString::fromStdString(Json::FastWriter().write(j_get_data));
            if(http_replay_func()!=0)
                ret++;
            j_get_data.clear();
        }
    }
    return ret;
}

int httpItemsByID::inc_updata_func()
{
    int ret=0;

    QByteArray d1=comp_data.toLocal8Bit();
    QByteArray d2=QByteArray::fromBase64(d1);
    QString d_uncompress=gzipUncompress(d2);

    //解压后的数据
#ifdef QT_DEBUG
    qDebug()<<"增量更新";
    qDebug()<<"解压后的数据 inc in data unc:";
    qDebug()<<d_uncompress;
#endif
    Json::Reader jr;
    Json::Value jv;
    if(!jr.parse(d_uncompress.toStdString(),jv))
    {
        //qDebug()<<"no json";
        return -999;
    }

    {/* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mSynckey");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mSynckey"));
    /* 检测是否有Synckey记录列表，没有则创建 */
    if(!db.tables().contains("Synckey"))
    {
        query.exec("create table Synckey(synckey int)");
    }
    /* 单个设备信息，当前做法直接把原来的清空把新的放进去 */
    int synckey;
    if(jv["synckey"].isInt())
        synckey=jv["synckey"].asInt();
    else
        synckey=QString::fromStdString(jv["synckey"].asString()).toInt();

    query.exec("delete from Synckey");
    query.exec("update sqlite_sequence SET seq = 0 where name ='Synckey'");
    query.prepare("INSERT INTO Synckey(synckey)VALUES(:synckey)");
    query.bindValue(":synckey",synckey);
    query.exec();
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSynckey");

    //先执行删除
    subusernum=jv["delete_items"].size();
    {/* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mSchoolUserItems");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mSchoolUserItems"));
    if(db.tables().contains("SchoolUserItems"))
    {
        db.transaction();
        //删除.
        for(int i=0;i<jv["delete_items"].size();i++)
        {
            //获得ID.
            int rm_uid;
            if(jv["delete_items"][i]["id"].isInt())
                rm_uid=jv["delete_items"][i]["id"].asInt();
            else
                rm_uid=QString::fromStdString(jv["delete_items"][i]["id"].asString()).toInt();
            //获得类型.
            int rm_type;
            if(jv["delete_items"][i]["type"].isInt())
                rm_type=jv["delete_items"][i]["type"].asInt();
            else
                rm_type=QString::fromStdString(jv["delete_items"][i]["type"].asString()).toInt();

            //删除用户
            query.prepare("Delete from SchoolUserItems where id=:id and type=:type");
            query.bindValue(":id",rm_uid);
            query.bindValue(":type",rm_type);
            query.exec();

#ifdef QT_DEBUG
            qDebug()<<"det id:"<<rm_uid;
            qDebug()<<"det type:"<<rm_type;
#endif

            string rmuid_str=to_string(rm_uid)+to_string(rm_type);
            //清空AI人脸--具体参数看HCM_MSG_HEAD_S
            //Global_class *t1_glo=Global_class::GetInstance();
//            QMutexLocker locker(&t1_glo->mutexall);
            HCM_MSG_HEAD_S stMsgHead;
            HCM_SQLITE_OPERATE stOperate;
            memset(&stMsgHead, 0, sizeof(stMsgHead));
            memset(&stOperate, 0, sizeof(stOperate));
            stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules.
            stMsgHead.u32MsgType = OUT_SQLITE_PICTURE_DELONE;//
            //stOperate.stSqConfig.i_iWbmode = FACE_WHITELIST; //白名单.
            strcpy(stOperate.stSqConfig.i_sUserID,rmuid_str.c_str());//用户ID.
            //qDebug()<<"do delete item:";
            int p = glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, &stOperate, sizeof(HCM_SQLITE_OPERATE), NULL, 0);
            //qDebug()<<"Intf_SendModuleMsg over:"<<p;
        }
        db.commit();
    }
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSchoolUserItems");

    Json::Value j_data[4];//分别对应4个类型的用户.
    addusernum=jv["update_items"].size()+jv["new_items"].size();
    thisnum=0;
    //更新.
    for(int i=0;i<jv["update_items"].size();i++)
    {
        //获得ID.
        QString u_id;
        if(jv["update_items"][i]["id"].isInt())
            u_id=QString::number(jv["update_items"][i]["id"].asInt());
        else
            u_id=QString::fromStdString(jv["update_items"][i]["id"].asString());
        //获得类型.
        int u_type;
        if(jv["update_items"][i]["type"].isInt())
            u_type=jv["update_items"][i]["type"].asInt();
        else
            u_type=QString::fromStdString(jv["update_items"][i]["type"].asString()).toInt();

        j_data[u_type].append(u_id.toStdString());
    }
    //添加.
    for(int i=0;i<jv["new_items"].size();i++)
    {
        //获得ID.
        QString u_id;
        if(jv["new_items"][i]["id"].isInt())
            u_id=QString::number(jv["new_items"][i]["id"].asInt());
        else
            u_id=QString::fromStdString(jv["new_items"][i]["id"].asString());
        //获得类型.
        int u_type;
        if(jv["new_items"][i]["type"].isInt())
            u_type=jv["new_items"][i]["type"].asInt();
        else
            u_type=QString::fromStdString(jv["new_items"][i]["type"].asString()).toInt();

        j_data[u_type].append(u_id.toStdString());
    }
    Json::Value j_get_data;
    for(int typenum=0;typenum<4;typenum++)
    {
        j_get_data.clear();
        for(int i=0;i<j_data[typenum].size();i++)
        {
            j_get_data.append(j_data[typenum][i]);
            if(j_get_data.size()==4000)//服务器ID集合一次最多可接受4000个.
            {
                //qDebug()<<"add or updata id to userinfo--type:"<<typenum;
                usertype=typenum;
                id_json=QString::fromStdString(Json::FastWriter().write(j_get_data));
                if(http_replay_func()!=0)
                    ret++;
                j_get_data.clear();
            }
        }
        if(j_get_data.size()>0)
        {
            //qDebug()<<"add or updata id to userinfo--type:"<<typenum;
            usertype=typenum;
            id_json=QString::fromStdString(Json::FastWriter().write(j_get_data));
            if(http_replay_func()!=0)
                ret++;
            j_get_data.clear();
        }
    }

    return ret;
}

int httpItemsByID::correct_func(int type)
{
    int ret=0;
    usertype=type;

    QByteArray d1=comp_data.toLocal8Bit();
    QByteArray d2=QByteArray::fromBase64(d1);
    QString d_uncompress=gzipUncompress(d2);

    //解压后的数据
#ifdef QT_DEBUG
    qDebug()<<"数据校正 用户类型:"<<usertype;
    qDebug()<<"correct in data unc:";
    qDebug()<<d_uncompress;
#endif
    Json::Reader jr;
    Json::Value jv;
    if(!jr.parse(d_uncompress.toStdString(),jv))
    {
        //qDebug()<<"no json";
        return -999;
    }

    addusernum=jv["update_id_items"].size()+jv["new_id_items"].size();
    thisnum=0;
    //qDebug()<<"add or update usernum from corr:"<<addusernum;

    Json::Value j_data;//
    //更新.
    for(int i=0;i<jv["update_id_items"].size();i++)
    {
        //获得ID.
        QString u_id;
        if(jv["update_id_items"][i].isInt())
            u_id=QString::number(jv["update_id_items"][i].asInt());
        else
            u_id=QString::fromStdString(jv["update_id_items"][i].asString());

        j_data.append(u_id.toStdString());
    }
    //添加.
    for(int i=0;i<jv["new_id_items"].size();i++)
    {
        //获得ID.
        QString u_id;
        if(jv["new_id_items"][i].isInt())
            u_id=QString::number(jv["new_id_items"][i].asInt());
        else
            u_id=QString::fromStdString(jv["new_id_items"][i].asString());

        j_data.append(u_id.toStdString());
    }
    Json::Value j_get_data;
//    qDebug()<<"json j_data size:"<<j_data.size();
    for(int i=0;i<j_data.size();i++)
    {
        j_get_data.append(j_data[i]);
        if(j_get_data.size()==4000)//服务器ID集合一次最多可接受4000个.
        {
            //qDebug()<<"add or updata id to userinfo--type:"<<usertype;
            id_json=QString::fromStdString(Json::FastWriter().write(j_get_data));
            if(http_replay_func()!=0)
                ret++;
            j_get_data.clear();
        }
    }
    if(j_get_data.size()>0)
    {
        //qDebug()<<"add or updata id to userinfo--type:"<<usertype;
        id_json=QString::fromStdString(Json::FastWriter().write(j_get_data));
        if(http_replay_func()!=0)
            ret++;
        j_get_data.clear();
    }

    subusernum=jv["remove_id_items"].size();
    {/* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mSchoolUserItems");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mSchoolUserItems"));
    if(db.tables().contains("SchoolUserItems"))
    {
        db.transaction();
        //删除.
        for(int i=0;i<jv["remove_id_items"].size();i++)
        {
            int rm_uid;
            if(jv["remove_id_items"][i].isInt())
                rm_uid=jv["remove_id_items"][i].asInt();
            else
                rm_uid=QString::fromStdString(jv["remove_id_items"][i].asString()).toInt();

            //删除用户
            query.prepare("Delete from SchoolUserItems where id=:id and type=:type");
            query.bindValue(":id",rm_uid);
            query.bindValue(":type",usertype);
            query.exec();
#ifdef QT_DEBUG
            qDebug()<<"det id:"<<rm_uid;
            qDebug()<<"det type:"<<usertype;
#endif

            string rmuid_str=to_string(rm_uid)+to_string(usertype);
            //清空AI人脸--具体参数看HCM_MSG_HEAD_S
            HCM_MSG_HEAD_S stMsgHead;
            HCM_SQLITE_OPERATE stOperate;
            memset(&stMsgHead, 0, sizeof(stMsgHead));
            memset(&stOperate, 0, sizeof(stOperate));
            stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules.
            stMsgHead.u32MsgType = OUT_SQLITE_PICTURE_DELONE;//
            //stOperate.stSqConfig.i_iWbmode = FACE_WHITELIST; //白名单.
            strcpy(stOperate.stSqConfig.i_sUserID,rmuid_str.c_str());//用户ID.
            //qDebug()<<"do delete item:";
            int p = glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, &stOperate, sizeof(HCM_SQLITE_OPERATE), NULL, 0);
            //qDebug()<<"Intf_SendModuleMsg over:"<<p;
        }
        db.commit();
    }
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSchoolUserItems");
    return ret;
}

int httpItemsByID::cleardb()
{
    //qDebug()<<"clear db * SchoolUserItems Synckey";
    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mSchoolUserItems");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mSchoolUserItems"));
    QString sqlstr;
    query.exec("DROP TABLE SchoolUserItems");
    query.exec("DROP TABLE Synckey");
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSchoolUserItems");

    userallnum=0;
    faceallnum=0;

    //清空AI人脸--具体参数看HCM_MSG_HEAD_S
    HCM_MSG_HEAD_S stMsgHead;
    HCM_SQLITE_OPERATE stOperate;
    memset(&stMsgHead, 0, sizeof(stMsgHead));
    memset(&stOperate, 0, sizeof(stOperate));
    stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules.
    stMsgHead.u32MsgType = OUT_SQLITE_PICTURE_DELLIST;//删除名单列表.
    stOperate.stSqConfig.i_iWbmode = FACE_WHITELIST; //白名单.
  //  qDebug()<<"do delete allusers:";
    int p = glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, &stOperate, sizeof(HCM_SQLITE_OPERATE), NULL, 0);
  //  qDebug()<<"Intf_SendModuleMsg over:"<<p;
    return 0;
}

void httpItemsByID::count_usernum()
{
  //  qDebug()<<"send SchoolUser num";
    userallnum=0;
    faceallnum=0;
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
     //   qDebug()<<"count----have SchoolUserItems";
        query.exec("SELECT COUNT(*) FROM SchoolUserItems");
        if (query.next())
        {
            userallnum =  query.value(0).toInt();
          //  qDebug()<<"user all num:"<<userallnum;
        }
        query.prepare("SELECT COUNT(*) FROM SchoolUserItems where face_url = :face_url");
        query.bindValue(":face_url","");
        query.exec();
        if (query.next())
        {
            faceallnum = userallnum - query.value(0).toInt();
         //   qDebug()<<"no face user:"<<query.value(0).toInt();
         //   qDebug()<<"face all num:"<<faceallnum;
        }
    }
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mSchoolUserItems");
}

