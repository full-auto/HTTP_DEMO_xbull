#include "httptaskpolling.h"

HttpTaskPolling::HttpTaskPolling()
{

}

HttpTaskPolling::~HttpTaskPolling()
{

}

void HttpTaskPolling::init()
{
//    qDebug()<<"轮询任务线程 init t: "<<QThread::currentThread();

    h_taskReserve=new HttptaskReserve();
    t_task=new QTimer();
    update_flag=0;
    laststatus=0;
    //t_task->setInterval(1000);
    t_task->setSingleShot(true);
    connect(t_task,SIGNAL(timeout()),this,SLOT(taskreplay()));

    //等待n秒，等其它线程资源都准备好
    QTimer dd;
    dd.setSingleShot(1);
    QEventLoop dde;
    connect(&dd,SIGNAL(timeout()),&dde,SLOT(quit()));
    dd.start(5000);
    dde.exec();

    //taskreplay();//刚启动，上报一次板子状态
    t_task->start(5000);
    //    qDebug()<<"轮询任务线程 ok";
}

void HttpTaskPolling::uninit()
{
    if(t_task->isActive())
       t_task->stop();
    delete t_task;
    t_task=NULL;

    /*
    t_task->deleteLater();
    h_taskReserve->deleteLater();
    */
    delete h_taskReserve;
    h_taskReserve=NULL;
}

void HttpTaskPolling::taskreplay()
{
#ifdef QT_DEBUG
    qDebug()<<"平台任务轮询: "<<QThread::currentThreadId();
#endif
 //   qDebug()<<"平台任务轮询: "<<"task_flag"<<h_taskReserve->task_flag<<"update_flag"<<update_flag<<QThread::currentThreadId();
    if(h_taskReserve->task_flag==0)
    {
        if(update_flag==0)
        {
            if(status_flag()==1)
            {
                h_taskReserve->task_flag=1;//设为执行任务状态
               // qDebug()<<"上报设备的一些信息到平台";
                emit ptaskid(0,0,0);
            }
            else
            {
                h_taskReserve->http_replay_func();
                if(h_taskReserve->task_flag==1)//有任务发送信号
                {
                    //qDebug()<<"收到平台任务，将发送信号给任务线程进行执行";
                    emit ptaskid(h_taskReserve->t_id,h_taskReserve->t_resource_id,h_taskReserve->t_action_id);
                }
                else//无任务，1分钟后再询问
                {
                    t_task->start(60000);
                }

                //qDebug()<<"即将发送信号，让界面显示设备的在平台上的注册状态";
                if(h_taskReserve->sta==-2)
                {
                    //qDebug()<<"发送信号，设备未注册";
                    emit dev_login_flag(1,h_taskReserve->net_MAC);
                }
                else
                {
                    //qDebug()<<"发送信号，设备已注册";
                    emit dev_login_flag(0,"");
                }
            }
        }
        else
        {
            //qDebug()<<"无任务正在执行，发送更新信号";
            emit notask();
            t_task->stop();//不再轮询任务
        }
    }
}

void HttpTaskPolling::set_task_flag()
{
#ifdef QT_DEBUG
    qDebug()<<"任务执行完毕";
#endif
    h_taskReserve->task_flag=0;
    t_task->start(5000);//执行完任务，马上再询问一次
}

void HttpTaskPolling::have_update_func(bool flag)
{
    /*
    if(flag)
        qDebug()<<"收到更新请求，等待轮询是否有任务正在执行";
    else
        qDebug()<<"收到恢复请求，重新开始任务轮询";
*/
    update_flag=flag;
    if(update_flag==0)
    {
        t_task->start(5000);
    }
}

int HttpTaskPolling::status_flag()
{
    //距离上次上传是否已经超过一小时
    int thistime=QDateTime::currentMSecsSinceEpoch()/1000;
    if(thistime-laststatus<3600)
        return 0;

    laststatus=thistime;
    QTime current_time =QTime::currentTime();
    int hour=current_time.hour();//当前的小时
    int min=current_time.minute();//当前的分钟
    //转换成17.5点的格式，如17:29为17，17.30为17.5，方便用于判断是否在高峰期时段
    double hm=hour;
    if(min>30)
        hm=hm+0.5;

    /* 以下时段不上传
    06:30~08:30
    11:30~14:30
    16:00~20:00
    21:30~22:30
    */

    if(hm>=6.5 && hm<8.5)
        return 0;

    if(hm>=11.5 && hm<14.5)
        return 0;

    if(hm>=16 && hm<20)
        return 0;

    if(hm>=21.5 && hm<22.5)
        return 0;

#ifdef QT_DEBUG
    qDebug()<<"开机后,以及每隔一小时,都自动上报一次板子状态,高峰期除外";
#endif

    return 1;
}
