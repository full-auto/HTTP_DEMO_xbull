#include "httpfacerecordupdata.h"

httpFaceRecordUpdata::httpFaceRecordUpdata()
{

}

httpFaceRecordUpdata::~httpFaceRecordUpdata()
{
  //  qDebug()<<"~httpFaceRecordUpdata";
    if(up_time->isActive())
        up_time->stop();
    up_time->deleteLater();
    up_time=NULL;
}

void httpFaceRecordUpdata::init()
{
//    qDebug()<<"识别记录线程 init t: "<<QThread::currentThread();
    send_data_flag=0;
    send_img_flag=0;
    send_tem_flag=0;
    send_card_flag=0;
    synall=0;
    old_id="";
    old_captime=0;
    thr=80;
    old_aifunc=1;
    tem_flag=0;
    nospeak=0;
    vbox_open=1;
    angin_Broad=0;
    no_Broadtype="";

    //上传数据轮询
    up_time=new QTimer();
    up_time->setInterval(500);//
    //up_time->setSingleShot(true);
    connect(up_time,SIGNAL(timeout()),this,SLOT(send_updata_all()));
    up_time->start();

    glo=Global_class::GetInstance();

    int sql1=1;
    int sql2=1;

    { // 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","opendoor");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("opendoor"));

    if(!db.tables().contains("openuserinfo"))
        sql1=1;
    else
    {
        query.exec("select * from sqlite_master where name='openuserinfo' and sql like '%broadcast_name%'");
        if (!query.next())
            sql1=1;
    }
    if(!db.tables().contains("openleaveuser"))
        sql2=1;

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("opendoor");

    if(sql1==1)
        update_sql();
    if(sql2==1)
        update_leavesql();

    //关门
    closedoor_time=new QTimer();
    closedoor_time->setInterval(300);
    closedoor_time->setSingleShot(true);
    connect(closedoor_time,SIGNAL(timeout()),this,SLOT(closedoor_func()));

    tem_time=new QTimer();
    tem_time->setSingleShot(1);
    tem_eventloop=new QEventLoop();
    connect(tem_time,SIGNAL(timeout()),tem_eventloop, SLOT(quit()));
    connect(this,SIGNAL(quit_tem_event()),tem_eventloop, SLOT(quit()));

    //qDebug()<<"识别记录线程 ok ";
}

void httpFaceRecordUpdata::uninit()
{
    if(up_time->isActive())
      up_time->stop();
    up_time->deleteLater();
    up_time=NULL;

    if(closedoor_time->isActive())
      closedoor_time->stop();
    closedoor_time->deleteLater();
    closedoor_time=NULL;

    if(tem_time->isActive())
      tem_time->stop();
    tem_time->deleteLater();
    tem_time=NULL;

    if(tem_eventloop->isRunning())
        tem_eventloop->quit();
    delete tem_eventloop;
    tem_eventloop=NULL;
}

void httpFaceRecordUpdata::senddataquit(int sta)
{
    if(sta==0)//
    {
#ifdef QT_DEBUG
        qDebug()<<"发送数据到平台成功，将删除这条记录";
#endif

        { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mRecordSheet");
        QString dbfilename=DB_PATH;
        dbfilename=dbfilename+"Xbull";
        db.setDatabaseName(dbfilename);
        QSqlQuery query(QSqlDatabase::database("mRecordSheet"));;

        query.prepare("Delete from RecordSheet where imgpath=:imgpath");//
        query.bindValue(":imgpath",senddata_path);
        query.exec();

        query.clear();
        db.close();
#ifdef QT_DEBUG
        qDebug()<<"本次结果记录的对应路径[仅删除记录表内容，不对实际文件操作]:"<<senddata_path;
#endif
        }
        QSqlDatabase::removeDatabase("mRecordSheet");
        synall++;
        emit emit_synall(synall);
    }
    else if(sta==1)//上传时，参数不对--最大可能时用户ID在平台上没有，此时查询一次板子是否有该用户ID。没有则删除该记录
    {
        { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mRecordSheet");
        QString dbfilename=DB_PATH;
        dbfilename=dbfilename+"Xbull";
        db.setDatabaseName(dbfilename);
        QSqlQuery query(QSqlDatabase::database("mRecordSheet"));;

        query.prepare("SELECT count(*) from openuserinfo where id=:id and type=:type");
        int id=up_id;
        int type=up_type-1;
        query.bindValue(":id",id);
        query.bindValue(":type",type);
        query.exec();
        query.next();

        if(query.value(0).toInt()==0)//记录表无这个数据,直接把和这个用户id相关的都删了
        {
            query.prepare("Delete from RecordSheet where user_id=:user_id and type=:type");//
            query.bindValue(":user_id",id);
            query.bindValue(":type",type);
            query.exec();
        }
        //需要上传到日记记录

        query.clear();
        db.close();
#ifdef QT_DEBUG
        qDebug()<<"本次结果记录的对应路径[仅删除记录表内容，不对实际文件操作]:"<<senddata_path;
#endif
        }
        QSqlDatabase::removeDatabase("mRecordSheet");
    }
    else//其它，暂时不做处理，继续下一次轮询.
    {
#ifdef QT_DEBUG
        qDebug()<<"上传数据失败，等待下一次轮询再次上传";
#endif
    }

    emit emit_updata_flag(sta);//临时显示用
    send_data_flag=0;
}

void httpFaceRecordUpdata::sendimgquit(int sta)
{
    if(sta==0||sta==1)//0-OK  1-找不到该图片
    {
        { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mRecordSheet");
        QString dbfilename=DB_PATH;
        dbfilename=dbfilename+"Xbull";
        db.setDatabaseName(dbfilename);
        QSqlQuery query(QSqlDatabase::database("mRecordSheet"));;

        query.prepare("Delete from RecordImg where imgpath=:imgpath");
        query.bindValue(":imgpath",sendimg_path);
        query.exec();

        query.clear();
        db.close();
        //qDebug()<<"本次图片记录的对应路径[仅删除记录表内容，不对实际文件操作]::"<<sendimg_path;
        }
        QSqlDatabase::removeDatabase("mRecordSheet");
    }
    else//其它，暂时不做处理，继续下一次轮询.
    {
        //qDebug()<<"上传图片失败[已经找到图片]，等待下一次轮询";
    }
    send_img_flag=0;
}

void httpFaceRecordUpdata::sendtemquit(int sta)
{
    if(sta==0 || sta==-1)//0--ok   -1--找不到用户信息，上传温度失败
    {
        { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mRecordSheet");
        QString dbfilename=DB_PATH;
        dbfilename=dbfilename+"Xbull";
        db.setDatabaseName(dbfilename);
        QSqlQuery query(QSqlDatabase::database("mRecordSheet"));;

        query.prepare("Delete from TemSheet where temperature_at=:temperature_at");
        query.bindValue(":temperature_at",tem_uptime);
        query.exec();

        query.clear();
        db.close();
        }
        QSqlDatabase::removeDatabase("mRecordSheet");
    }
    else//其它，暂时不做处理，继续下一次轮询.
    {
        //qDebug()<<"暂时不做处理，继续下一次轮询";
    }

    send_tem_flag=0;
}

void httpFaceRecordUpdata::sendcardquit(int sta)
{
    if(sta != -999)//0--ok   12已存在这条记录
    {
        { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mRecordSheet");
        QString dbfilename=DB_PATH;
        dbfilename=dbfilename+"Xbull";
        db.setDatabaseName(dbfilename);
        QSqlQuery query(QSqlDatabase::database("mRecordSheet"));;

        query.prepare("Delete from CardSheet where time=:time");
        query.bindValue(":time",card_timep);
        query.exec();

        query.clear();
        db.close();
        }
        QSqlDatabase::removeDatabase("mRecordSheet");

        if(sta==0)//成功后同步结果加1
        {
            synall++;
            emit emit_synall(synall);
        }
    }
    else//其它，暂时不做处理，继续下一次轮询.
    {
        //qDebug()<<"暂时不做处理，继续下一次轮询";
    }
    send_card_flag=0;
}

void httpFaceRecordUpdata::send_updata_all()
{
    //qDebug()<<"轮询要上传的信息";
    up_time->stop();
    //识别记录的，无论是否上次上传结束，后获取总数在界面更新同步中
    int tbz=0;

    { // 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mRecordSheet");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mRecordSheet"));;

    // 检测是否识别记录表
    if(db.tables().contains("RecordSheet"))
    {
        query.exec("SELECT count(*) from RecordSheet");
        query.next();
        if(query.value(0).toInt()!=0)//记录表有数据.
        {
            tbz=tbz+query.value(0).toInt();
            if(send_data_flag==0)//是否在上传中,不是才获取数据上传
            {
                query.exec("SELECT * from RecordSheet limit 1"); //获得第一条记录.
                query.seek(0);
                QSqlRecord rdata = query.record();
                FACE_POST fp;
                // 图片路径
                fp.picture_path=rdata.value("imgpath").toString();
                senddata_path=rdata.value("imgpath").toString();
                // 抓拍时间
                fp.capture_time=rdata.value("capture_time").toInt();
                // 用户 id
                fp.user_id=rdata.value("user_id").toInt();
                // 用户类型:1=学生, 2=家长, 3=职工, 4=访客
                fp.user_type=rdata.value("type").toInt()+1;//接收处的类型编号比数据库的多1.
                // 识别状态:1=已经识别 2=未识别; version > 15, 此字段为必填
                fp.identify_status=rdata.value("identify_status").toInt();
                // 人脸相似度
                fp.rate=rdata.value("rate").toFloat();
                if(senddata_path!="")
                {
                    up_id=fp.user_id;
                    up_type=fp.user_type;
                    send_data_flag=1;
    #ifdef QT_DEBUG
                    qDebug()<<"记录表有识别记录，发送信号给数据上传线程，本次上传数据对应的图片路径:"<<fp.picture_path;
    #endif
                    emit emit_sendface(fp);
                }
                else//发现空数据，清空空数据
                {
                    //qDebug()<<"发现空数据，清空空数据";
                    query.exec("Delete from RecordSheet where imgpath is null");//
                }
            }
        }
    }

    //图片
    if(send_img_flag==0)//是否在上传中,不是才获取数据上传
    {
        // 检测是否图片记录表
        if(db.tables().contains("RecordImg"))
        {
            query.exec("SELECT count(*) from RecordImg");
            query.next();
            if(query.value(0).toInt()!=0)//记录表有数据.
            {
                query.exec("SELECT * from RecordImg limit 1"); //获得第一条记录.
                query.seek(0);
                QSqlRecord idata = query.record();
                QString db_imgpath=idata.value("imgpath").toString();
                if(db_imgpath!="")
                {
                    sendimg_path=db_imgpath;
                    send_img_flag=1;
#ifdef QT_DEBUG
                    qDebug()<<"图片记录表有数据，发送信号给OSS线程进行上传，本次图片路径是:"<<db_imgpath;
#endif
                    emit emit_sendimg(db_imgpath);
                }
                else//发现空数据，清空空数据
                {
                    //qDebug()<<"发现空数据，清空空数据";
                    query.exec("Delete from RecordImg where imgpath is null");//
                }
            }
        }
    }

    //体温
    if(send_tem_flag==0)//是否在上传中,不是才获取数据上传
    {
        // 检测是否体温记录表
        if(db.tables().contains("TemSheet"))
        {
            query.exec("SELECT count(*) from TemSheet");
            query.next();
            if(query.value(0).toInt()!=0)//记录表有数据.
            {
                query.exec("SELECT * from TemSheet limit 1"); //获得第一条记录.
                query.seek(0);
                QSqlRecord idata = query.record();

                int t_id=idata.value("user_id").toInt();
                int t_type=idata.value("user_type").toInt();
                float t_tw=idata.value("temperature").toFloat();
                tem_uptime=idata.value("temperature_at").toInt();
#ifdef QT_DEBUG
                qDebug()<<"测温记录表有数据，发送信号给进行上传，本次时间戳是:"<<tem_uptime;

#endif
                //emit 发送上传测温
                emit emit_sendtem(t_id,t_type,t_tw,tem_uptime);
                send_tem_flag=1;
            }
        }
    }

    //刷卡--和刷脸一样，为了统计同步中，不过是否在上传都获取一次数量
    // 检测是否刷卡记录表
    if(db.tables().contains("CardSheet"))
    {
        query.exec("SELECT count(*) from CardSheet");
        query.next();
        if(query.value(0).toInt()!=0)//记录表有数据.
        {
            tbz=tbz+query.value(0).toInt();
            if(send_card_flag==0)
            {
            query.exec("SELECT * from CardSheet limit 1"); //获得第一条记录.
            query.seek(0);
            QSqlRecord idata = query.record();

            QString card_num=idata.value("card").toString();
            card_timep=idata.value("time").toInt();
            QString imgpath=idata.value("picture_path").toString();
#ifdef QT_DEBUG
            qDebug()<<"刷卡记录表有数据，发送信号给进行上传，本次时间戳是:"<<tem_uptime;
#endif
            emit emit_sendcard(card_num,card_timep,imgpath);
            send_card_flag=1;
            }
        }
    }


    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mRecordSheet");
    emit emit_syning(tbz);//界面更新--同步中,刷脸和刷卡的一起计数
    up_time->start();
}

void httpFaceRecordUpdata::haveAIfacefunc(FACE_DATA tfd, HCP_DETECT_INFO thdi)
{
    #ifdef QT_DEBUG
    qDebug()<<"进入新的处理函数"<<QDateTime::currentMSecsSinceEpoch();
    #endif
    if(old_aifunc)//上一次已经处理完毕
    {
#ifdef QT_DEBUG
        qDebug()<<"如果与上一个通过用户相同，且间隔在两秒内，则不再做重复处理--本次跳出";
        qDebug()<<"AI上次用户id:"<<QString::fromStdString(old_id)<<" | 上次时间:"<<old_captime;
        qDebug()<<"AI本次用户id:"<<QString::fromStdString(tfd.user_id)<<" | 本次时间:"<<tfd.capture_time;
#endif
        int new_captime=QDateTime::currentMSecsSinceEpoch()/1000;
        if(tfd.user_id==old_id && std::abs(new_captime-old_captime)<nospeak)
        {
            #ifdef QT_DEBUG
            qDebug()<<"与上一个通过用户相同，且间隔在固定秒数内，本次跳出";
            #endif
            return;
        }

        up_time->stop();
        old_aifunc=0;//在此处才设为处理中
        cometype=1;
        update_type=0;//每次开始清零，要记录体温+10，记录人脸+1

        temDuration=0;
        doorDuration=0;

        this_fd=tfd;
        //人脸库上的用户id为平台上id+type，在此处进行拆分
        int id_type=QString::fromStdString(this_fd.user_id).toInt();
        int user_id=id_type/10;
        int type=id_type%10;

        if(this_fd.identify_status==2)//未识别的，未识别记录上传，测温门禁等其它不再处理
        {
            #ifdef QT_DEBUG
            qDebug()<<"未识别的，未识别记录上传，测温门禁等其它不再处理";
            #endif
            update_type=update_type+1;
            aifunc_ok();
            return;
        }
        else//识别成功，将开始测温-门禁的处理
        {
            //在界面显示识别的结果
            HCP_DETECT_INFO nthd;
            memset(&nthd, 0, sizeof(HCP_DETECT_INFO));
            nthd.iKnow = thdi.iKnow;
            qstrcpy(nthd.szPath, thdi.szPath);
            qstrcpy(nthd.szName, thdi.szName);
            qstrcpy(nthd.szInfo, thdi.szInfo);

            //测温判断
            int tem_this=tem_func(type);
            //界面先显示用户人脸和信息
            emit show_faceimg(nthd,tem_this);
            if(tem_this==1)//需要测温
            {
                nospeak=3;
                if(is_voi==0)
                    glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/tem.aac");//播报
                tem_contrast(type);
            }
            else//不用测温,进入门禁判断函数
            {
                nospeak=1;
                door_control_func(0);
            }
        }
    }
    else
    {
        #ifdef QT_DEBUG
        qDebug()<<"上一次未处理完"<<QDateTime::currentMSecsSinceEpoch();
        #endif
    }
}

int httpFaceRecordUpdata::tem_func(int t)
{
    #ifdef QT_DEBUG
    qDebug()<<"是否测温判断函数"<<QDateTime::currentMSecsSinceEpoch();
    #endif
    tw_v=0;
    int tem_now=tem_flag;//记录这次是否需要测温
    if(tem_now==1)//是否测温
    {
        //获取当天的秒时间
        int nows=QTime::currentTime().hour()*3600+QTime::currentTime().minute()*60+QTime::currentTime().second();
        //判断是否在不测温时段内
        for(int i=0;i<tem_ptime.size();i++)
        {
            if(tem_ptime.at(i).at(0).toInt()<=nows && tem_ptime.at(i).at(1).toInt()>=nows)
            {
                tem_now=0;
                break;
            }
        }

        if(tem_now==1)//在测温时段内，判断是否为不用测温用户
        {
            if(tem_nousertype.contains(QString::number(t)))//该用户类型不用测温
            {
                tem_now=0;
            }
        }
    }
    #ifdef QT_DEBUG
    qDebug()<<"是否测温结果判断:"<<tem_now<<QDateTime::currentMSecsSinceEpoch();
    #endif
    return tem_now;
}

void httpFaceRecordUpdata::tem_contrast(int t)
{
#ifdef QT_DEBUG
    qDebug()<<"进入测温函数:"<<QDateTime::currentMSecsSinceEpoch();
#endif
    long long intemfunct=QDateTime::currentMSecsSinceEpoch();

    //emit 发送测温信号
    if(tem_pc_flag==1)
        emit get_tem(tem_thr,tem_pc);
    else
        emit get_tem(tem_thr,0);

    tem_time->start(tem_wait*1000);
    tem_eventloop->exec();

    //发送信号停止测温
    emit stop_tem();
    long long oktemfunct=QDateTime::currentMSecsSinceEpoch();
    temDuration=std::abs(oktemfunct-intemfunct);

    if(!tem_time->isActive())//测温超时,则判断是否超时后可以直接通过的用户类型
    {
        #ifdef QT_DEBUG
        qDebug()<<"测温超时:"<<QDateTime::currentMSecsSinceEpoch();
        #endif
        if(tem_usertype_timeout.contains(QString::number(t)))
        {
            #ifdef QT_DEBUG
            qDebug()<<"用户类型可以不测温，继续判断门禁"<<QDateTime::currentMSecsSinceEpoch();
            #endif
            door_control_func(0);
        }
        else//该用户需要测温才能通行，本次结束
        {
            #ifdef QT_DEBUG
            qDebug()<<"用户类型必须测温，结束"<<QDateTime::currentMSecsSinceEpoch();
            #endif
            //界面此时等待隐藏，不用刷新文字提示
            aifunc_ok();//用户需要测温才能通行,到此本次处理结束
        }
    }
    else
    {
        tem_time->stop();
        if(tem_over==1)//测温不通过，判断当前测温模式
        {
            //测温类型:
            //0-默认，发烧不放行+上传数据;
            //1-发烧不放行+不上传数据
            //2-发烧发行+上传数据;
            //3-发烧放行+不上传数据
            if(tem_type==0 || tem_type==1)
            {
                emit show_door(13,tw_v);//显示体温异常，属于不通行
                if(is_voi==0)
                    glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/temng.aac");//播报

                if(tem_type==0)
                    update_type=update_type+10;
                aifunc_ok();//用户发烧,到此本次处理结束
            }
            else//发烧-也可以通行
            {
                if(tem_type==2)
                {
                    update_type=update_type+10;
                }
                door_control_func(2);
            }
        }
        else//测温正常
        {
            update_type=update_type+10;
            door_control_func(1);
        }
    }
}

void httpFaceRecordUpdata::door_control_func(int temover)
{
    long long indoorfunct=QDateTime::currentMSecsSinceEpoch();
    int open_ok=-1;//0正常通行  1门禁时段  2请假通行
    //人脸库上的用户id为平台上id+type，在此处进行拆分
    int id_type=QString::fromStdString(this_fd.user_id).toInt();
    int user_id=id_type/10;
    int type=id_type%10;

    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    //qDebug()<<"数据库录入，记录识别结果用于上传";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mRecordSheet");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mRecordSheet"));

#ifdef QT_DEBUG
    qDebug()<<"判断开门:"<<QDateTime::currentMSecsSinceEpoch();
#endif

    query.prepare("select attendance_label,attendance_label_arr,broadcast_position_items,broadcast_name from openuserinfo where id=:id and type=:type");
    query.bindValue(":id",user_id);
    query.bindValue(":type",type);
    query.exec();
    if(query.next())
    {
        QString attendance_label=query.value(0).toString();
        if(attendance_label=="")//如果为空，改为默认的标签  否则出现空的情况，会100%命中门禁规则
        {
            if(type==0)
                attendance_label="学生";
            else if(type==1)
                attendance_label="家长";
            else if(type==2)
                attendance_label="职工";
            else
                attendance_label="访客";
        }
        QString attendance_label_arr=query.value(1).toString();//此为json，需要转换
        QString broadcast_position_items=query.value(2).toString();//此为json,直接发送，由处理处在转换
        QString broadcast_name=query.value(3).toString();
        //职工默认0号音箱
        broadcast_name="[{\"broadcast_name\":\"" + broadcast_name + "\",\"speaker_num\":\"0\"}]";

        //访客不触发门禁，默认都通行
        if(type==3)//访客默认通行
        {
#ifdef QT_DEBUG
            qDebug()<<"访客默认通行:"<<QDateTime::currentMSecsSinceEpoch();
#endif
            open_ok=0;
        }

        //检测请假用户
        if(open_ok==-1)
        {
#ifdef QT_DEBUG
            qDebug()<<"判断请假:"<<QDateTime::currentMSecsSinceEpoch();
#endif
            query.prepare("SELECT start_time,end_time from openleaveuser where user_id=:user_id and user_type=:user_type");
            query.bindValue(":user_id",user_id);
            query.bindValue(":user_type",type);
            query.exec();
            QList<QStringList> userfff;
            while(query.next())
            {
                int start_time=query.value(0).toInt();
                int end_time=query.value(1).toInt();
                QStringList ss;
                ss.append(QString::number(start_time));
                ss.append(QString::number(end_time));
                userfff.append(ss);
        #ifdef QT_DEBUG
                qDebug()<<"发现对应用户的请假时间:"<<start_time<<"~"<<end_time;
        #endif
            }
            open_ok=leave_flag(userfff);
        }

        //检测门禁规则
        if(open_ok==-1)
        {
#ifdef QT_DEBUG
            qDebug()<<"判断门禁:"<<QDateTime::currentMSecsSinceEpoch();
#endif
            QStringList attendance_label_list;
            attendance_label_list.append(attendance_label);
            if(type==2)//职工才有多个标签
            {
                Json::Reader jr;
                Json::Value jv;
                jr.parse(attendance_label_arr.toStdString(),jv);
                for(int j=0;j<jv.size();j++)
                {
                    // 前后加上"|",用于隔开各个标签。避免出现主任，班主任两个标签时，班主任判定为主任
                    QString slabel=QString::fromStdString(jv[j].asString());
                    if(slabel=="")
                        slabel="-";
                    slabel="|"+slabel+"|";
                    attendance_label_list.append(slabel);
                }
            }
            open_ok=open_flag(type,attendance_label_list);
        }
        long long okdoorfunct=QDateTime::currentMSecsSinceEpoch();
        doorDuration=std::abs(indoorfunct-okdoorfunct);
#ifdef QT_DEBUG
        qDebug()<<"判断结果:"<<open_ok<<":"<<QDateTime::currentMSecsSinceEpoch();
#endif
        if(open_ok==1)//门禁阶段 禁止通行
        {
            int mjw=temover*10+open_ok;
            if(is_voi==0)
                glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/ismj.aac");//播报
            emit show_door(mjw,tw_v);//门禁阶段不用显示体温，1，11，21都一样
        }
        else//允许通行
        {
            if(closedoor_time->isActive())
                closedoor_time->stop();

            int mjw=temover*10+open_ok;
            emit show_door(mjw,tw_v);

#ifdef QT_DEBUG
            qDebug()<<"准备播报,是否静音:"<<is_voi<<",体温结果"<<temover;
            qDebug()<<"音箱是否播报:"<<vbox_open;
#endif

            if(vbox_open==1 && type!=3)//开启音箱播报 且 不是访客[访客不播报]
            {
                broad_func(user_id, type,broadcast_name,broadcast_position_items);
            }

            if(is_voi==0)
            {
                if(temover==0 || temover==2)
                {
                    glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/facepass.aac");//播报
                }
                else
                {
                    glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/temok.aac");
                }
            }

            INT nParam = 1;//拉高
            glo->g_HciFunc->Intf_LocalIoCtl(IOCTL_SET_ALARMOUT1, &nParam, sizeof(nParam));//开门
            closedoor_time->start();
        }
    }

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mRecordSheet");
    if(open_ok!=1)//当为1时，处于门禁不上传，0正常通行，2请假通行
    {
        update_type=update_type+1;
    }
    old_id=this_fd.user_id;
    old_captime=QDateTime::currentMSecsSinceEpoch()/1000;
    aifunc_ok();
}

void httpFaceRecordUpdata::cleardb()
{
#ifdef QT_DEBUG
    qDebug()<<"从平台收到清空记录表的任务，将删除识别记录表和OSS图片记录表[仅对数据表操作]";
#endif

    {/* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mRecordSheet");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("mRecordSheet"));
    //直接删除表格
    query.exec("DROP TABLE RecordSheet");
    query.exec("DROP TABLE RecordImg");
    query.exec("DROP TABLE TemSheet");
    query.exec("DROP TABLE CardSheet");

    query.exec("DROP TABLE openleaveuser");
    query.exec("DROP TABLE openuserinfo");
    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("mRecordSheet");
 //   qDebug()<<"删除成功，将把界面上的 同步中 已同步 都设为0";
    emit emit_syning(0);
    emit emit_synall(0);
}

void httpFaceRecordUpdata::set_thr(float t,int lf,int ls,int le,int vl)
{
    thr=t;
    leave_ok=lf;
    leave_s=ls;
    leave_e=le;
    is_voi=vl;
#ifdef QT_DEBUG
    qDebug()<<"设置二次判断的阈值:"<<thr;
    qDebug()<<"leave_ok:"<<leave_ok;
    qDebug()<<"leave_s:"<<leave_s;
    qDebug()<<"leave_e:"<<leave_e;
    qDebug()<<"is_voi:"<<is_voi;
#endif
}

void httpFaceRecordUpdata::set_tem_num(int t_flag, float t_thr, int t_type, int t_pc_f,float t_pc, int t_wait,QString t_ptime,QString t_no,QString t_out)
{
    #ifdef QT_DEBUG
    qDebug()<<"收到测温参数";
    qDebug()<<"是否测温:"<<t_flag;
    qDebug()<<"发烧阈值:"<<t_thr;
    qDebug()<<"模式：:"<<t_type;
    qDebug()<<"是否补偿:"<<t_pc_f;
    qDebug()<<"补偿值:"<<t_pc;
    qDebug()<<"超时:"<<t_wait;
    qDebug()<<"不测温时段:"<<t_ptime;
    qDebug()<<"不测温用户:"<<t_no;
    qDebug()<<"超时可通行用户:"<<t_out;
    #endif
    tem_flag=t_flag;
    tem_thr=t_thr;
    tem_type=t_type;
    tem_pc_flag=t_pc_f;
    tem_pc=t_pc;
    tem_wait=t_wait;
    tem_nousertype=t_no;
    tem_usertype_timeout=t_out;
    //json
    tem_ptime.clear();
    Json::Reader jr;
    Json::Value jv;
    if(jr.parse(t_ptime.toStdString(),jv))
    {
        for(int i=0;i<jv.size();i++)
        {
            Json::Value js=jv[i];
            //获取开始小时数--再转成秒数
            QString stop_at=QString::fromStdString(js[0].asString());
            int mint=stop_at.indexOf(":");
            QString h_at=stop_at.mid(0,mint);
            int h_num=h_at.toInt()*3600;
            //获取开始分钟数--再转成秒数
            QString m_at=stop_at.mid(mint+1,2);
            int m_num=m_at.toInt()*60;
            //合计当天秒数
            QString all_at=QString::number(h_num+m_num);

            //获取结束时间，操作同上
            QString stop_end=QString::fromStdString(js[1].asString());
            mint=stop_end.indexOf(":");
            QString h_end=stop_end.mid(0,mint);
            h_num=h_end.toInt()*3600;
            QString m_end=stop_end.mid(mint+1,2);
            m_num=m_end.toInt()*60;
            QString all_end=QString::number(h_num+m_num);

            #ifdef QT_DEBUG
            qDebug()<<"开始时间:"<<stop_at<<"|"<<all_at;
            qDebug()<<"结束时间:"<<stop_end<<"|"<<all_end;
            #endif

            QStringList ss;
            ss.append(all_at);
            ss.append(all_end);
            tem_ptime.append(ss);
        }
    }
    //发送一些参数给界面，用于判断显示
    emit tem_ui(tem_flag,tem_wait,tem_ptime);
}

void httpFaceRecordUpdata::closedoor_func()
{
    INT nParam = 0;//拉低
    glo->g_HciFunc->Intf_LocalIoCtl(IOCTL_SET_ALARMOUT1, &nParam, sizeof(nParam));//关门
}

void httpFaceRecordUpdata::set_rule2(int mr, QList<QStringList> r_list)
{
    dopen_flag=mr;
    rule2_list.clear();
    for(int i=0;i<r_list.size();i++)
    {
        rule2_list.append(r_list.at(i));
    }
}

void httpFaceRecordUpdata::update_sql()
{
    //更新都是删除重新复制
    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","opendoor");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("opendoor"));

    query.exec("DROP TABLE openuserinfo");
    query.exec("create table openuserinfo(id int,\
                                        type int,\
                                        name string,\
                                        class_name string,\
                                        attendance_label string,\
                                        attendance_label_arr string,\
                                        broadcast_name string,\
                                        broadcast_position_items string,\
                                        card_items string)");

    query.exec("insert into openuserinfo(id,type,name,class_name,attendance_label,attendance_label_arr,broadcast_name,broadcast_position_items,card_items) select\
                                        id,type,name,class_name,attendance_label,attendance_label_arr,broadcast_name,broadcast_position_items,card_items from SchoolUserItems");

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("opendoor");
}

void httpFaceRecordUpdata::update_leavesql()
{
    //更新都是删除重新复制
    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","opendoor");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("opendoor"));

    query.exec("DROP TABLE openleaveuser");
    query.exec("create table openleaveuser AS SELECT * FROM LeaveUser");

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("opendoor");
}

void httpFaceRecordUpdata::have_tem_over(bool fever, float tw)
{
    //qDebug()<<"收到测温结果"<<fever<<"|"<<tw;
    tem_over=fever;
    tw_v=tw;
    emit quit_tem_event();
}

void httpFaceRecordUpdata::get_cardbox(int bopen,int anginopen,QString notype)
{
    vbox_open=bopen;
    angin_Broad=anginopen;
    no_Broadtype=notype;
}

void httpFaceRecordUpdata::have_carduser(QString inc_id)
{
    if(old_aifunc)//上一次已经处理完毕  和刷脸共用
    {
        //qDebug()<<"进入刷卡函数"<<QDateTime::currentMSecsSinceEpoch();
        up_time->stop();
        old_aifunc=0;//在此处才设为处理中
        cometype=2;
        update_type=0;//每次开始清零，要记录体温+10，记录刷卡+1

        QString card_low=inc_id.toLower();//大写转小写
        int card_sta=-1;//0=未激活，1=使用中，2=挂失，3=禁用，4=已删除   -1未找到
        int c_id=0;
        int c_type=0;
        QString c_name;
        QString c_class;
        QString c_card;

        { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","opendoor");
        QString dbfilename=DB_PATH;
        dbfilename=dbfilename+"Xbull";
        db.setDatabaseName(dbfilename);
        QSqlQuery query(QSqlDatabase::database("opendoor"));

        //查找包含卡信息的用户
        QString sel_info="select id,type,name,class_name,card_items from openuserinfo where(card_items like'%"+card_low+"%')";
        query.exec(sel_info);
        if(query.next())
        {
            c_id=query.value(0).toInt();
            c_type=query.value(1).toInt();
            c_name=query.value(2).toString();
            c_class=query.value(3).toString();
            c_card=query.value(4).toString();
            //获得卡状态
            Json::Reader jr;
            Json::Value js;
            if(jr.parse(c_card.toStdString(),js))
            {
                for(int i=0;i<js.size();i++)
                {
                    string cidstr=js[i]["card_id"].asString();
                    if(cidstr==card_low.toStdString())
                    {
                        if(js[i]["status"].isInt())
                            card_sta=js[i]["status"].asInt();
                        else
                            card_sta=QString::fromStdString(js[i]["status"].asString()).toInt();
                        break;
                    }
                }
            }
        }
        query.clear();
        db.close();
        }
        QSqlDatabase::removeDatabase("opendoor");

        if(card_sta==1)//卡状态使用中
        {
            this_fd.capture_time=QDateTime::currentMSecsSinceEpoch()/1000-3600*8;
            QString idtype=QString::number(c_id)+QString::number(c_type);
            this_fd.user_id=idtype.toStdString();//用于后续测温，门禁判断使用
            card_number=card_low;
            //在界面显示刷卡的结果
            HCP_DETECT_INFO nthd;
            memset(&nthd, 0, sizeof(HCP_DETECT_INFO));
            nthd.iKnow = 1;
            qstrcpy(nthd.szPath, "/usrappfs/face-pic/cardimg.png");//显示固定的图片
            qstrcpy(nthd.szName, c_name.toStdString().data());
            qstrcpy(nthd.szInfo, c_class.toStdString().data());

            //测温判断
            int tem_this=tem_func(c_type);
            //界面先显示用户人脸和信息
            emit show_faceimg(nthd,tem_this);
            if(tem_this==1)//需要测温
            {
                if(is_voi==0)
                    glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/tem.aac");//播报
                tem_contrast(c_type);
            }
            else//不用测温,进入门禁判断函数
            {
                door_control_func(0);
            }
        }
        else
        {
            long long int card10=inc_id.toLongLong(0,16);
            QString carderror;
            if(card_sta==-1)
            {
                carderror=QString::fromLocal8Bit("卡未绑定用户或被禁用：")+QString::number(card10);
                if(is_voi==0)
                    glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/cardnouserorban.aac");//播报
            }
            else if(card_sta==0)
            {
                carderror=QString::fromLocal8Bit("卡未激活：")+QString::number(card10);
                if(is_voi==0)
                    glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/cardnoactivate.aac");//播报
            }
            else if(card_sta==2)
            {
                carderror=QString::fromLocal8Bit("卡挂失：")+QString::number(card10);
                if(is_voi==0)
                    glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/cardloss.aac");//播报
            }
            else if(card_sta==3)
            {
                carderror=QString::fromLocal8Bit("卡禁用：")+QString::number(card10);
                if(is_voi==0)
                    glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/cardban.aac");//播报
            }
            else if(card_sta==4)
            {
                carderror=QString::fromLocal8Bit("卡已删除：")+QString::number(card10);
                if(is_voi==0)
                    glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/carddelete.aac");//播报
            }
            emit card_error_info(carderror);
            aifunc_ok();
        }
    }
    else
    {
        #ifdef QT_DEBUG
        qDebug()<<"上一次未处理完"<<QDateTime::currentMSecsSinceEpoch();
        #endif
    }
}

void httpFaceRecordUpdata::broad_func(int uid, int utype,QString broadname,QString broadnameitems)
{
    //获取当前秒级时间戳
    int nowtime=QDateTime::currentMSecsSinceEpoch()/1000;
    //移除超过10秒的记录，从后面算起
    for(int x=last_usedid_time.size()-1; x>=0; x--)
    {
        int reg_time=last_usedid_time.at(x).at(1).toInt();
        if(nowtime-reg_time>10)
        {
            //qDebug()<<"以下记录都是超过10秒的，将进行清除";
            //按照先进的排序，所有在此之前的都可以移除
            for(int j=x; j>=0; j--)
            {
                last_usedid_time.removeAt(j);
            }
            break;//在此退出
        }
    }

    //判断剩下的，是否有在10秒内播报过
    bool noreg=1;
    for(int x=0;x<last_usedid_time.size();x++)
    {
        if(last_usedid_time.at(x).contains(QString::number(uid*10+utype)))
        {
            #ifdef QT_DEBUG
            qDebug()<<"该id在10秒内有播报过"<<last_usedid_time.at(x);
            #endif
            noreg=0;
            break;
        }
    }

    if(!no_Broadtype.contains(QString::number(utype)) && noreg)//是否是不播报类型,并且10秒内没播过
    {
        //把这个播报的用户记录添加进去
        QStringList thisbroad;
        thisbroad.append(QString::number(uid*10+utype));
        thisbroad.append(QString::number(nowtime));
        last_usedid_time.append(thisbroad);

        QString broadjson=broadnameitems;
        if(broadnameitems.size()<10)//broadnameitems没有播报信息
        {
            broadjson=broadname;
        }

        emit send_speakdate(broadjson);
        if(angin_Broad==1)//重复发送一次
        {
            #ifdef QT_DEBUG
            qDebug()<<"播报两次";
            #endif
            emit send_speakdate(broadjson);
        }
    }
}

int httpFaceRecordUpdata::leave_flag(QList<QStringList> start_end)
{
#ifdef QT_DEBUG
    qDebug()<<"进入请假判断函数:"<<QDateTime::currentMSecsSinceEpoch();
    qDebug()<<"请假通行标识符:"<<leave_ok;
#endif
    int ret=-1;

    if(leave_ok==0)//是否支持请假通行
    {
        //计算日期时，按照板子的来
        QDateTime bj_DateTime = QDateTime(QDateTime::currentDateTime());
        QString taday_at=bj_DateTime.toString("yyyy-MM-dd");
        //计算当前处于当天的秒数
        int now_s=QTime::currentTime().hour()*3600+QTime::currentTime().minute()*60+QTime::currentTime().second();

#ifdef QT_DEBUG
        qDebug()<<"当前时间:"<<now_s;
        qDebug()<<"支持请假开始时间:"<<leave_s;
        qDebug()<<"支持请假结束时间[0就设为86400]:"<<leave_e;
#endif

        if(leave_e==0)
            leave_e=86400;

        if(now_s<leave_s || now_s>leave_e)//是否在请假通行时间段内
        {
#ifdef QT_DEBUG
            qDebug()<<"不在允许请假通行时间段内";
#endif
            return ret;
        }
    }
    else
    {
#ifdef QT_DEBUG
        qDebug()<<"不允许请假通行";
#endif
        return ret;
    }

    //时间戳快8个小时
    int now_s=(QDateTime::currentMSecsSinceEpoch()/1000)-(3600*8);

#ifdef QT_DEBUG
    qDebug()<<"找到的用户请假对应时段数量："<<start_end.size();
#endif

    for(int i=0;i<start_end.size();i++)
    {
        int start_time=start_end.at(i).at(0).toInt();
        int end_time=start_end.at(i).at(1).toInt();
#ifdef QT_DEBUG
        qDebug()<<"请假用户判断时段:"<<i;
        qDebug()<<"当前时间:"<<now_s;
        qDebug()<<"请假开始时间:"<<start_time;
        qDebug()<<"请假结束时间:"<<end_time;
#endif
        if(now_s>=start_time && now_s<=end_time)
        {
#ifdef QT_DEBUG
            qDebug()<<"在请假时间段内";
#endif
            ret=2;
            break;
        }
        else
        {
#ifdef QT_DEBUG
            qDebug()<<"不在请假时间段内";
#endif
        }
    }
#ifdef QT_DEBUG
    qDebug()<<"结束请假判断函数:"<<QDateTime::currentMSecsSinceEpoch();
#endif
    return ret;
}

int httpFaceRecordUpdata::open_flag(int type, QStringList user_tag)
{
#ifdef QT_DEBUG
    qDebug()<<"进入门禁判断函数:"<<QDateTime::currentMSecsSinceEpoch();
    qDebug()<<"用户所有标签:"<<user_tag.size();
    for(int ut=0;ut<user_tag.size();ut++)
    {
        qDebug()<<ut<<"用户标签:"<<user_tag.at(ut);
    }
#endif
    /*门禁规则的链表--用于槽函数发送到开门线程使用,链表排序规则已经固定，请查看插入方式
     *  0开始日期--时间戳
     *  1是否永久
     *  2结束日期--时间戳
     *  3不生效日期--yyyy-mm-dd
     *  4学生标签--全部-住宿-走读...
     *  5家长标签
     *  6职工标签
     *  7详细规则--json数组[week:生效周几，start_time:开始时间--当天秒数，end_time:结束时间--当天秒数，access_type_id:进出类型 0禁止进出、1允许进出、2只进不出、3只出不进]
     */
    //触发门禁规则的按照门禁规则来，没触发的则允许通行
    int opendoor_flag=dopen_flag;
    for(int i=0;i<rule2_list.size();i++)
    {
        int start_at                =rule2_list.at(i).at(0).toInt();
        int is_eternal              =rule2_list.at(i).at(1).toInt();
        int end_at                  =rule2_list.at(i).at(2).toInt();
        QString invalid_at_items    =rule2_list.at(i).at(3);
        QString student_tag_items   =rule2_list.at(i).at(4);
        QString parent_tag_items    =rule2_list.at(i).at(5);
        QString staff_tag_items     =rule2_list.at(i).at(6);
        QString access_items_json   =rule2_list.at(i).at(7);
#ifdef QT_DEBUG
        qDebug()<<"-------------";
        qDebug()<<"门禁规则:"<<i;
        qDebug()<<"默认设置 0通行 1禁止:"<<dopen_flag;
        qDebug()<<"开始日期:"<<start_at;
        qDebug()<<"是否永久:"<<is_eternal;
        qDebug()<<"结束日期:"<<end_at;
        qDebug()<<"不生效日期:"<<invalid_at_items;
        qDebug()<<"学生标签:"<<student_tag_items;
        qDebug()<<"家长标签:"<<parent_tag_items;
        qDebug()<<"职工标签:"<<staff_tag_items;
        qDebug()<<"详细规则:"<<access_items_json;
#endif

        //获取当前时间，因为板子的时区为0时区，比北京时间块8小时，这里需要先做换算处理
        //计算秒级时间戳，板子时间戳-8小时=北京时间戳
        int bjts=(QDateTime::currentMSecsSinceEpoch()/1000)-(3600*8);
        //获得当前日期--只有用于判断日期是否在生效时间内,不生效的年月日按照板子的来
        QDateTime bj_DateTime = QDateTime(QDateTime::currentDateTime());//QDateTime::fromTime_t(bjts);

        //判断是否大于开始日期
        if((bjts-start_at)<0)//当前日期小于开始日期，此规则不生效
            continue;

        if(is_eternal==0)//非永久生效，判断是否在结束日期前
        {
            if((bjts-end_at)>0)//当前日期大于结束日期，此规则不生效
                continue;
        }

        //获取今日日期,直接判断今日yyyy-mm-dd是否包含在不生效的字符串中
        QString taday_at=bj_DateTime.toString("yyyy-MM-dd");
        if(invalid_at_items.contains(taday_at))//今日是否处于不生效日期
            continue;

#ifdef QT_DEBUG
        qDebug()<<""<<bjts;
        qDebug()<<"标签判断:";
#endif

        //判断识别到的用户标签是否包含在规则内
        bool tag_flag=0;
        if(type==0)
        {
#ifdef QT_DEBUG
            qDebug()<<"判断学生标签,门禁的为:"<<student_tag_items;
            qDebug()<<"用户标签为:"<<user_tag.at(0);
#endif
            QString student_all=QString::fromLocal8Bit("全部学生");
            //当前学生只能设置一个标签----走读生，住宿生...
            if(student_tag_items.contains(student_all)||student_tag_items.contains(user_tag.at(0)))
            {
                tag_flag=1;
            }
        }
        else if(type==1)
        {
#ifdef QT_DEBUG
            qDebug()<<"判断家长标签,门禁的为[家长当前只有 全部家长 的选择]:"<<parent_tag_items;
            qDebug()<<"用户标签为:"<<user_tag.at(0);
#endif
            QString parent_all=QString::fromLocal8Bit("全部家长");
            //当前家长的标签只有家长
            if(parent_tag_items.contains(parent_all)/*||parent_tag_items.contains(user_tag.at(0))*/)
            {
                tag_flag=1;
            }
        }
        else if(type==2)
        {
#ifdef QT_DEBUG
            qDebug()<<"判断职工标签,门禁的为:"<<staff_tag_items;
#endif
            QString staff_all=QString::fromLocal8Bit("全部职工");
            //职工可以设置多个标签--语文老师，班主任...
            if(staff_tag_items.contains(staff_all))
            {
                tag_flag=1;
            }
            else
            {
#ifdef QT_DEBUG
                qDebug()<<"没选择全部职工，职工可能有多个标签，将逐一判断";
                qDebug()<<"该职工拥有的标签数量:"<<user_tag.size();
#endif
                for(int j=0;j<user_tag.size();j++)
                {
#ifdef QT_DEBUG
                    qDebug()<<"该职工标签:"<<j<<"|"<<user_tag.at(j);
#endif
                    if(staff_tag_items.contains(user_tag.at(j)))
                    {
                        tag_flag=1;
                    }
                }
            }
        }

        if(tag_flag==0)//该用户标签没有触发规则对应的标签
        {
#ifdef QT_DEBUG
            qDebug()<<"没有用户标签触发该门禁，将判断下一条门禁";
#endif
            continue;
        }

        //计算当前处于当天的秒数 h*3600+m*60+s
        int now_s=QTime::currentTime().hour()*3600+QTime::currentTime().minute()*60+QTime::currentTime().second();

        //把今日周几转为数字
        QString now_week=bj_DateTime.toString("ddd");
#ifdef QT_DEBUG
        qDebug()<<"时间戳:"<<bjts;
        qDebug()<<"周几:"<<now_week;
#endif
        if(QString("Monday").contains(now_week,Qt::CaseInsensitive))
            now_week="1";
        else if(QString("Tuesday").contains(now_week,Qt::CaseInsensitive))
            now_week="2";
        else if(QString("Wednesday").contains(now_week,Qt::CaseInsensitive))
            now_week="3";
        else if(QString("Thursday").contains(now_week,Qt::CaseInsensitive))
            now_week="4";
        else if(QString("Friday").contains(now_week,Qt::CaseInsensitive))
            now_week="5";
        else if(QString("Saturday").contains(now_week,Qt::CaseInsensitive))
            now_week="6";
        else
            now_week="7";
        //解析门禁时间段的信息
        Json::Reader jr;
        Json::Value jv;
        jr.parse(access_items_json.toStdString(),jv);
        for(int j=0;j<jv.size();j++)
        {
            QString weeks=QString::fromStdString(Json::FastWriter().write(jv[j]["week"]));
            if(weeks.contains(now_week))//在生效的周几内
            {
                //开始时间
                int start_time;
                if(jv[j]["start_time"].isInt())
                    start_time=jv[j]["start_time"].asInt();
                else
                    start_time=QString::fromStdString(jv[j]["start_time"].asString()).toInt();
                //结束时间
                int end_time;
                if(jv[j]["end_time"].isInt())
                    end_time=jv[j]["end_time"].asInt();
                else
                    end_time=QString::fromStdString(jv[j]["end_time"].asString()).toInt();

#ifdef QT_DEBUG
                qDebug()<<"在生效的周几内";
                qDebug()<<"当前时间:"<<now_s;
                qDebug()<<"开始时间:"<<start_time;
                qDebug()<<"结束时间:"<<end_time;
#endif
                //判断是否在生效时间段内
                if(now_s>=start_time && now_s<=end_time)//在生效时间段内
                {
                    int access_type_id;
                    if(jv[j]["access_type_id"].isInt())
                        access_type_id=jv[j]["access_type_id"].asInt();
                    else
                        access_type_id=QString::fromStdString(jv[j]["access_type_id"].asString()).toInt();
                    //0禁止进出、1允许进出、2只进不出、3只出不进  0 1和默认相反
#ifdef QT_DEBUG
                    qDebug()<<"在生效的时间内,对应的通行标识符[0禁止进出、1允许进出、2只进不出、3只出不进]:"<<access_type_id;
#endif
                    if(access_type_id==0)
                        opendoor_flag=1;
                    else if(access_type_id==1)
                        opendoor_flag=0;
                    else//只进不出，只出不进的，暂不处理，按照该规则默认的来
                        opendoor_flag=dopen_flag;

                    return opendoor_flag;//返回通行状态，其它规则不再判断
                }
            }
        }
    }
#ifdef QT_DEBUG
    qDebug()<<"结束门禁判断函数:"<<opendoor_flag<<" | "<<QDateTime::currentMSecsSinceEpoch();
#endif
    //没触发门禁，默认开门
    return opendoor_flag;
}

void httpFaceRecordUpdata::aifunc_ok()
{
    #ifdef QT_DEBUG
    qDebug()<<"处理结束，记录结果:"<<update_type;
    #endif
    if(update_type!=0)
    {
        //人脸库上的用户id为平台上id+type，在此处进行拆分
        int id_type=QString::fromStdString(this_fd.user_id).toInt();
        int user_id=id_type/10;
        int type=id_type%10;

        { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
        //qDebug()<<"数据库录入，记录识别结果用于上传";
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","mRecordSheet");
        QString dbfilename=DB_PATH;
        dbfilename=dbfilename+"Xbull";
        db.setDatabaseName(dbfilename);
        QSqlQuery query(QSqlDatabase::database("mRecordSheet"));

        /* 检测是否识别记录表，没有则创建 */
        if(!db.tables().contains("RecordSheet"))
        {
            query.exec("create table RecordSheet(capture_time int,\
                                                user_id int,\
                                                type int,\
                                                identify_status int,\
                                                rate float,\
                                                imgpath string)");
        }
        /* 检测是否刷卡记录表，没有则创建 */
        if(!db.tables().contains("CardSheet"))
        {
            query.exec("create table CardSheet(card varchar,\
                                                time int,\
                                                picture_path string)");
        }
        /* 检测是否图片记录表，没有则创建 */
        if(!db.tables().contains("RecordImg"))
        {
            query.exec("create table RecordImg(imgpath string)");
        }
        /* 检测是否体温记录表，没有则创建 */
        if(!db.tables().contains("TemSheet"))
        {
            query.exec("create table TemSheet(user_id int,\
                                                user_type int,\
                                                temperature float,\
                                                temperature_at int)");
        }

        if(update_type==1||update_type==11)
        {
            if(cometype==1)
            {
                //qDebug()<<"开始记录人脸信息，用于上传平台";
                query.prepare("INSERT INTO RecordSheet(capture_time,\
                                                        user_id,\
                                                        type,\
                                                        identify_status,\
                                                        rate,\
                                                        imgpath)\
                                                VALUES(:capture_time,\
                                                        :user_id,\
                                                        :type,\
                                                        :identify_status,\
                                                        :rate,\
                                                        :imgpath)");
                int capture_time=this_fd.capture_time;//pCmpRecInfo->stSnapUsecTime
                int identify_status=this_fd.identify_status;
                float rate=this_fd.rate;
                QString imgpath=QString::fromStdString(this_fd.imgpath);
                //上传数据-识别记录表
                query.bindValue(":capture_time",capture_time);
                query.bindValue(":user_id",user_id);
                query.bindValue(":type",type);
                query.bindValue(":identify_status",identify_status);
                query.bindValue(":rate",rate);
                query.bindValue(":imgpath",imgpath);
                query.exec();
                //上传图片--路径记录表
                query.prepare("INSERT INTO RecordImg(imgpath)VALUES(:imgpath)");
                query.bindValue(":imgpath",imgpath);
                query.exec();
            }
            else
            {
                //获取抓拍的路径
                char *szSnapPath;
                HCM_MSG_HEAD_S stMsgHead;
                memset(&stMsgHead, 0, sizeof(stMsgHead));
                stMsgHead.u32DestModule = HCM_AIMODULE_E; //发送给ai modules
                stMsgHead.u32MsgType = IN_SNAPCUR_PIC_PATH;
                UINT nCount;
                int nret=-1;
                nret = glo->g_HciFunc->Intf_SendModuleMsg(&stMsgHead, NULL, 0, (VOID **)&szSnapPath, &nCount);
                QString photopath=szSnapPath;

                //用主程序接口获取路径
                //...
                query.prepare("INSERT INTO CardSheet(card,\
                                                    time,\
                                                    picture_path)\
                                                VALUES(:card,\
                                                    :time,\
                                                    :picture_path)");
                //上传数据-刷卡记录表
                query.bindValue(":card",card_number);
                query.bindValue(":time",this_fd.capture_time);
                query.bindValue(":picture_path",photopath);
                query.exec();
                //上传图片--路径记录表
                if(!photopath.isEmpty())
                {
                    query.prepare("INSERT INTO RecordImg(imgpath)VALUES(:imgpath)");
                    query.bindValue(":imgpath",photopath);
                    query.exec();
                }
            }
        }

        if(update_type==10||update_type==11)
        {
            if(type!=3)//访客的体温不上传
            {
                //qDebug()<<"开始记录体温，用于上传平台";
                query.prepare("INSERT INTO TemSheet(user_id,\
                                                        user_type,\
                                                        temperature,\
                                                        temperature_at)\
                                                VALUES(:user_id,\
                                                        :user_type,\
                                                        :temperature,\
                                                        :temperature_at)");
                int thist=QDateTime::currentMSecsSinceEpoch()/1000-3600*8;
                //上传数据-体温记录表
                query.bindValue(":user_id",user_id);
                query.bindValue(":user_type",type);
                query.bindValue(":temperature",tw_v-0.05);//避免因为四舍五入，37.46变成37.5，37.46就按照36.4来处理
                query.bindValue(":temperature_at",thist);
                query.exec();
            }
        }

        query.clear();
        db.close();
        }
        QSqlDatabase::removeDatabase("mRecordSheet");
    }
    emit temdoorDuration(temDuration,doorDuration);
    //处理标识设回空闲，重启上传计时器
    old_aifunc=1;
    up_time->start();
}


