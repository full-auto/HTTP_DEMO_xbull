#include "httptask.h"

HttpTask::HttpTask()
{

}

HttpTask::~HttpTask()
{

}

void HttpTask::init()
{
//    qDebug()<<"执行任务线程 init t: "<<QThread::currentThread();
    h_selfStudyFaceGateConfig=new HttpselfStudyFaceGateConfig();
    h_schoolUserItems=new HttpschoolUserItems();
    h_IncrementUserItems=new httpIncrementUserItems();
    h_ItemsByID=new httpItemsByID();
    h_reportStatus=new httpreportStatus();
    h_correction=new httpcorrection();
    h_httpschoolleaveuser=new httpSchoolLeaveUser();

    /*
    pro_times=new QTimer();
    pro_times->setInterval(1000);
    connect(pro_times,SIGNAL(timeout()),this,SLOT(update_progress()));
    */
    //界面提示百分比，不再使用1秒更新一次，因为是同一个线程会相互争资源
    connect(h_ItemsByID,SIGNAL(update100()),this,SLOT(update_progress()));

    /* 开机先获得设备信息 */
    //h_selfStudyFaceGateConfig->http_replay_func();
    h_selfStudyFaceGateConfig->getdecinfo();
    dev_set_func();

    h_ItemsByID->count_usernum();
    emit user_num(h_ItemsByID->userallnum,h_ItemsByID->faceallnum);
//    qDebug()<<"执行任务线程 ok";

    //判断版本是否有变化
    v_times=new QTimer();
    v_times->setInterval(3000);
    connect(v_times,SIGNAL(timeout()),this,SLOT(change_vis()));
    v_times->start();//开始版本判断
}

void HttpTask::uninit()
{
    if(pro_times->isActive())
        pro_times->stop();
    pro_times->deleteLater();
    pro_times=NULL;
    /*
    h_selfStudyFaceGateConfig->deleteLater();
    h_schoolUserItems->deleteLater();
    h_taskRelease->deleteLater();
    h_IncrementUserItems->deleteLater();
    h_ItemsByID->deleteLater();
    h_reportStatus->deleteLater();
    h_correction->deleteLater();
    h_httpschoolleaveuser->deleteLater();
    */
    delete h_selfStudyFaceGateConfig;
    delete h_schoolUserItems;
    delete h_IncrementUserItems;
    delete h_ItemsByID;
    delete h_reportStatus;
    delete h_correction;
    delete h_httpschoolleaveuser;
    h_selfStudyFaceGateConfig=NULL;
    h_schoolUserItems=NULL;
    h_IncrementUserItems=NULL;
    h_ItemsByID=NULL;
    h_reportStatus=NULL;
    h_correction=NULL;
    h_httpschoolleaveuser=NULL;
}

void HttpTask::task_info(int id1, int id2, int id3)
{
    QTime do_time;
    do_time.start();

#ifdef QT_DEBUG
    qDebug()<<"任务执行线程获取到平台任务";
    qDebug()<<"任务ID:"<<id3<<" | task do t: "<<QThread::currentThreadId();
#endif

    id=id1;
    resource_id=id2;
    action_id=id3;
    user_change=0;

    int task_flag=1;//记录任务是否成功
    bool nofunc=0;

    h_ItemsByID->failstr.clear();

    switch(action_id)
    {
    case 0://自动上报状态，非请求获得的任务
    {
        status_correcting();
        break;
    }
    case 1://清除所有数据.
    {
        tasks1();
        break;
    }
    case 2://更新所有数据.
    {
        user_change=1;
        //设备
        if(h_selfStudyFaceGateConfig->http_replay_func()!=0)
            task_flag++;
        //学校用户
        if(h_schoolUserItems->http_replay_func()!=0)
            task_flag++;
        emit user_num(0,0);//全量更新时，界面用户数量先设0
        if(h_schoolUserItems->version.toInt()>68)//大于68版本才做此操作.
        {
            //pro_times->start();
            loaduser_tpye=0;
            h_ItemsByID->comp_data=h_schoolUserItems->comperr_json_alldata;
            //增量更新的全量处理接口
            if(h_ItemsByID->all_updata_func()!=0)
                task_flag++;
            //pro_times->stop();
        }
        dev_set_func();

        h_ItemsByID->count_usernum();
        emit user_num(h_ItemsByID->userallnum,h_ItemsByID->faceallnum);
        emit allupdate(h_ItemsByID->userallnum);
        //请假用户
        h_httpschoolleaveuser->cleandb();
        if(h_httpschoolleaveuser->http_replay_func()!=0)
            task_flag++;
        emit send_update_lrave();

        break;
    }
    case 3://更新设备信息.
    {
        if(h_selfStudyFaceGateConfig->http_replay_func()!=0)
            task_flag++;
        dev_set_func();
        break;
    }
    case 4://删除学生.
    {
        nofunc=1;
        break;
    }
    case 5://更新学生信息.
    {
        nofunc=1;
        break;
    }
    case 6://更新学校用户列表.
    {
        user_change=1;
        if(h_schoolUserItems->http_replay_func()!=0)
            task_flag++;
        emit user_num(0,0);
        if(h_schoolUserItems->version.toInt()>68)//大于68版本才做此操作.
        {
            //pro_times->start();
            loaduser_tpye=0;
            h_ItemsByID->comp_data=h_schoolUserItems->comperr_json_alldata;
            if(h_ItemsByID->all_updata_func()!=0)
                task_flag++;
            //pro_times->stop();
        }
        h_ItemsByID->count_usernum();
        emit user_num(h_ItemsByID->userallnum,h_ItemsByID->faceallnum);
        emit allupdate(h_ItemsByID->userallnum);
        break;
    }
    case 7://新增学生.
    {
        nofunc=1;
        break;
    }
    case 8://新增职工.
    {
        nofunc=1;
        break;
    }
    case 9://更新职工.
    {
        nofunc=1;
        break;
    }
    case 10://移除职工.
    {
        nofunc=1;
        break;
    }
    case 11://新增家长.
    {
        nofunc=1;
        break;
    }
    case 12://更新家长.
    {
        nofunc=1;
        break;
    }
    case 13://移除家长.
    {
        nofunc=1;
        break;
    }
    case 14://新增访客.
    {
        nofunc=1;
        break;
    }
    case 15://更新访客.
    {
        nofunc=1;
        break;
    }
    case 16://删除访客.
    {
        nofunc=1;
        break;
    }
    case 17://数据校正.
    {
        if(status_correcting()!=0)
            task_flag++;
        break;
    }
    case 18://增量更新.
    {
        user_change=1;
        if(h_schoolUserItems->version.toInt()>68)//大于68版本才做此操作.
        {
            //pro_times->start();
            loaduser_tpye=0;
            if(h_IncrementUserItems->http_replay_func()!=0)
                task_flag++;
            h_ItemsByID->comp_data=h_IncrementUserItems->comp_inc_data;
            if(h_ItemsByID->inc_updata_func()!=0)
                task_flag++;
            //pro_times->stop();
        }
        h_ItemsByID->count_usernum();
        emit user_num(h_ItemsByID->userallnum,h_ItemsByID->faceallnum);
        emit addupdate(h_ItemsByID->addusernum,h_ItemsByID->subusernum,h_ItemsByID->userallnum);

        break;
    }
    case 19://增量更新-请假数据
    {
        if(h_httpschoolleaveuser->http_replay_func()!=0)
            task_flag++;
        emit send_update_lrave();
        break;
    }
    default:
        break;
    }

    if(task_flag!=1)
        task_flag=0;

    if(action_id!=0)//0自动上报不是请求到的任务，不用返回
    {
        send_tastover(id,task_flag);
    }

#ifdef QT_DEBUG
    qDebug()<<"task func times is :"<<do_time.elapsed()<<" ms";
#endif

    int ttt=do_time.elapsed();
    emit sendtime(1,ttt,action_id,task_flag);//界面显示任务耗时信号

    if(user_change)
        emit send_update_user();//门禁线程更新用户信息信号

    if(nofunc)
    {
#ifdef QT_DEBUG
        qDebug()<<"获取到了无对应处理函数的任务:"<<action_id;
#endif
    }
#ifdef QT_DEBUG
    qDebug()<<"如果执行人脸下发任务，失败的有[load为下载失败，add为注册到AI失败，空的话为没收到人脸下发任务或者注册都正常]:";
    qDebug()<<h_ItemsByID->failstr;
#endif
//    qDebug()<<"任务执行结束，发送信号到任务轮询线程";
    emit task_end();//任务结束信号
}

void HttpTask::update_progress()
{
#ifdef QT_DEBUG
    qDebug()<<"当注册人脸时，发送信号,把进度百分比到界面显示";
#endif
    int nownum=h_ItemsByID->thisnum;
    float t=nownum;
    int allnum=h_ItemsByID->addusernum;
    float a=allnum;
    int user_t=h_ItemsByID->usertype;
    if(a<1)
        emit updates(loaduser_tpye,user_t,0,allnum);
    else
    {
        float p=t/a;
        emit updates(loaduser_tpye,user_t,p,allnum);
    }
}

void HttpTask::tasks1()
{
    //清空数据库.
    //非本线程的发送信号让实际操作线程操作.
    emit cleardata();
    //本线程的直接操作.
    h_selfStudyFaceGateConfig->cleardb();
    dev_set_func();
    h_ItemsByID->cleardb();
    emit user_num(h_ItemsByID->userallnum,h_ItemsByID->faceallnum);
    h_httpschoolleaveuser->cleandb();
}

int HttpTask::change_vis()
{
#ifdef QT_DEBUG
    qDebug()<<"版本对比处理函数";
#endif
    int old_v=0;
    { /* 限制作用域， QSqlDatabase::removeDatabase 只能在作用域外断开数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","vis");
    QString dbfilename=DB_PATH;
    dbfilename=dbfilename+"Xbull";
    db.setDatabaseName(dbfilename);
    QSqlQuery query(QSqlDatabase::database("vis"));
    //获得数据表版本号
    if(!db.tables().contains("version"))
    {
        query.exec("create table version(version int)");
    }
    else
    {
        query.exec("SELECT count(*) from version");
        query.next();
        if(query.value(0).toInt()!=0)//记录表内有数据.
        {
            query.exec("SELECT * from version limit 1"); //获得第一条记录.
            query.seek(0);
            QSqlRecord rdata = query.record();
            old_v=rdata.value("version").toInt();
        }
    }
    //对比是否有变化，有就请求设备数据
#ifdef QT_DEBUG
        qDebug()<<"对比版本:"<<old_v<<" | "<<h_selfStudyFaceGateConfig->version.toInt();
#endif
    if(old_v!=h_selfStudyFaceGateConfig->version.toInt())
    {
        h_selfStudyFaceGateConfig->http_timeout->setInterval(3000);//此时的超时改为3秒，避免没网时阻塞其它正常操作
        if(h_selfStudyFaceGateConfig->http_replay_func()==0)
        {
            old_v=h_selfStudyFaceGateConfig->version.toInt();
            dev_set_func();
            //更新成功，把最新的版本号保存入数据表中
            query.exec("delete from version");
            query.exec("update sqlite_sequence SET seq = 0 where name ='version'");
            query.prepare("INSERT INTO version(version)VALUES(:version)");
            query.bindValue(":version",old_v);
            query.exec();
            v_times->stop();//停止版本判断
#ifdef QT_DEBUG
            qDebug()<<"版本不同，设备已自动更新设置配置，停止对比";
#endif
        }
        h_selfStudyFaceGateConfig->http_timeout->setInterval(60000*3);//结束设回正常
    }
    else
    {
        v_times->stop();//停止版本判断
#ifdef QT_DEBUG
         qDebug()<<"版本相同，设备已自动更新设置配置，停止对比";
#endif
    }

    query.clear();
    db.close();
    }
    QSqlDatabase::removeDatabase("vis");
}

void HttpTask::dev_set_func()
{
    //界面 设备信息
    emit macvis(h_selfStudyFaceGateConfig->devname_user,
                h_selfStudyFaceGateConfig->version,
                h_selfStudyFaceGateConfig->school_user);
    //界面 下方提示框
    emit send_show_debug(h_selfStudyFaceGateConfig->debug_showui);

    //识别的一些函数
    emit send_facethr(h_selfStudyFaceGateConfig->face_thr,
                      h_selfStudyFaceGateConfig->leaceflag,
                      h_selfStudyFaceGateConfig->start_leave,
                      h_selfStudyFaceGateConfig->end_leave,
                      h_selfStudyFaceGateConfig->voice_f);
    //门禁规则参数
    emit send_rule2(h_selfStudyFaceGateConfig->dopenflag,
                    h_selfStudyFaceGateConfig->rule2_strlist);
    //测温参数
    emit send_tem_vulan(h_selfStudyFaceGateConfig->tem_flag,
                        h_selfStudyFaceGateConfig->tem_thr,
                        h_selfStudyFaceGateConfig->tem_type,
                        h_selfStudyFaceGateConfig->tem_pc_flag,
                        h_selfStudyFaceGateConfig->tem_pc,
                        h_selfStudyFaceGateConfig->tem_wait,
                        h_selfStudyFaceGateConfig->tem_ptime,
                        h_selfStudyFaceGateConfig->tem_nousertype,
                        h_selfStudyFaceGateConfig->tem_usertype_timeout);
    //静音刷卡 音箱
    emit send_ismute(h_selfStudyFaceGateConfig->voice_f);

    //刷卡 音箱
    emit send_cardopen(h_selfStudyFaceGateConfig->card_open);
    emit send_boxopen(h_selfStudyFaceGateConfig->hbox_open,
                      h_selfStudyFaceGateConfig->hbox_angin,
                      h_selfStudyFaceGateConfig->hbox_notype);
    emit send_ser_num(h_selfStudyFaceGateConfig->hbox_number);
    emit send_channl_num(h_selfStudyFaceGateConfig->hbox_cha,h_selfStudyFaceGateConfig->hbox_number);
}

int HttpTask::status_correcting()
{
    int ret=1;
    if(h_reportStatus->http_replay_func()==0)
    {
        ret=0;
//        qDebug()<<"status corr";
        bool user_cor=0;//记录是否有进行配置或用户的数据修正
        //设备数据不对
        if(h_reportStatus->config_status==0)
        {
            user_cor=1;
            if(h_selfStudyFaceGateConfig->http_replay_func()!=0)
                ret++;
            dev_set_func();
        }

        for(int i=0;i<4;i++)
        {
            if(h_reportStatus->usertype_status[i]==0)
            {
                user_change=1;
                user_cor=1;
                //pro_times->start();
                loaduser_tpye=1;
                if(h_correction->correction_func(i)!=0)//
                    ret++;
                //根据反馈要校正的ID，更新用户数据
                h_ItemsByID->comp_data=h_correction->correc_date;
                if(h_ItemsByID->correct_func(i)!=0)//
                    ret++;
                //pro_times->stop();
                emit addupdate(h_ItemsByID->addusernum,h_ItemsByID->subusernum,h_ItemsByID->userallnum);
            }
        }

        if(user_cor)
        {
            h_ItemsByID->count_usernum();
            emit user_num(h_ItemsByID->userallnum,h_ItemsByID->faceallnum);
            //如果有进行修复，修复完再上报一次,但本次上报返回即使有数据依然异常也不再修复，等待下一次进来
            //这里不管是否成功
            h_reportStatus->http_replay_func();
        }
    }
    return ret;
}
