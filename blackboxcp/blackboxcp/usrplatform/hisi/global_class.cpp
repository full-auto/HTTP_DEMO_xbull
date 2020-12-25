#include "global_class.h"

Global_class* Global_class::instance=NULL;

Global_class::Global_class()
{
    g_HciFunc=NULL;
}

Global_class::~Global_class()
{
#ifdef QT_DEBUG
    qDebug()<<"delete global";
#endif
    if(instance!=NULL)
    {
        delete instance;
        instance=NULL;
    }
}

Global_class *Global_class::GetInstance()
{
    QMutex g_mutex;
    g_mutex.lock();
    if(instance==NULL)
    {
        //qDebug()<<"ggc is null!!!!!!";
        instance=new Global_class();
    }
    else
    {
       //qDebug()<<"ggc is had--------";
    }
    g_mutex.unlock();
    //qDebug()<<"gcc ok";
    return instance;
}

void Global_class::emit_haveface()
{
    //qDebug()<<"global emit haveface"<<QThread::currentThread();
    emit haveface(g_fb,g_uidata);
}

void Global_class::emit_haveface_in(FACE_DATA ifd, HCP_DETECT_INFO ihdi)
{
    //qDebug()<<"global emit haveface_in"<<QThread::currentThread();
    emit haveface(ifd,ihdi);
}

void Global_class::emit_appquit()
{
    //qDebug()<<"global emit quit"<<QThread::currentThread();
    emit appquit();
}

void Global_class::emit_send_tt(int tt,int wet)
{
    //qDebug()<<"global emit func time"<<QThread::currentThread();
    emit send_tt(0,tt,wet,0);
}
