/******************************************************************************

  Copyright (C), 2020-2025, HiFace Tech. Co., Ltd.

  HiFace Camera Interface  (HCI)
  HiFace Camera Module  (HCM)

以下四个函数接口必须存在，以主应用先关联
  HCM_USRPLATFORM_Init
  HCM_USRPLATFORM_UnInit
  HCM_USRPLATFORM_DealAiNalyseData
  HCM_USRPLATFORM_GetInfo
  
******************************************************************************/
	
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

#include "hci_def.h"
#include "hci_intf.h"
#include "hci_struct.h"
#include "hcmComInfo.h"

#include <http_ser/http_usr.h>
#include <http_ser/http_scan.h>
#include <ooo_ui/ooo_ui.h>
#include <global_class/Global_Class.h>

#include <QCoreApplication>
#include <qtextcodec.h>
#include <unistd.h>
//#include "tenniuwindow.h"
#include <QSplashScreen>
/////////////////////////////////////
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <math.h>
#include <sys/ioctl.h>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QEventLoop>

////////////////////////////////////////////////////


ooo_ui *my_ui  = NULL;
QTimer *tim    = NULL;

// http服务器类、　自定应用类
http_usr *http_ser   = NULL;
QThread  *T_http_ser = NULL;
Global_Class *glo    = NULL;
/////////////////////////////////////////////////////

extern "C"{
INT HCM_USRPLATFORM_Init(HCI_FUNC *pstHcFunc);
INT HCM_USRPLATFORM_UnInit(VOID);
INT HCM_USRPLATFORM_DealAiNalyseData(UINT nCh, INT nAnalyseType,  VOID* pResultData, UINT nResultLen);
CHAR* HCM_USRPLATFORM_GetInfo(VOID);
VOID * UsrPlatform_Task(VOID *arg);
}

CHAR *pVersion = "usrplatform v1.0.0";

INT gusrplatformStart = 0;
INT gHCMGetAiStartStatus = 0;
bool functhread = false;
//INT DHCP_F=0;
//INT correcting_time=0;

///////////////////////////////////////////////


/*******************************************************************************
 * 函数功能：初始化各资源。包括HTTP主处理类、人脸识别类、ui界面类
 * 参数说明：
 *   空
 * 返回值：
 * 　空
 * ******************************************************************************/
VOID * UsrPlatform_Task(VOID *arg)
{
    qDebug() << err_func_show<< "now come in===============================================";
    int argc = 0;
    char *argv = "Lib_test2";
    QApplication a(argc, &argv, QApplication::GuiServer);

    //　显示启动界面
    QPixmap pixmap("/home/my_logo.png");//face-pic  ui_img
    QSplashScreen screen(pixmap);
    //screen.showMessage("", Qt::AlignCenter, Qt::white);
    screen.show();


    // 空等待，直到AI模块初始化完成----为什么AI模块加载完再加载我的资源会错误？
    //while(gHCMGetAiStartStatus != AI_START_SUCESS_E)sleep(5);
    sleep(7);

    qDebug() << err_func_show<<"============================================";
#if 1
    // 发送控制-不灭屏幕
    AI_FACE_CTL_S stFACECTL;
    memset( &stFACECTL, 0, sizeof( stFACECTL ) );
    int nRet = glo->g_HciFunc->Intf_GetParam( HCI_CFG_FACECTL, 0, &stFACECTL, sizeof(stFACECTL) );
    if(nRet==0)
    {
        //把识别对比开关设为1，防止出现不识别的情况
        stFACECTL.ucCmpEnable      = 1;
        stFACECTL.ucLcdDisplayMode = 1;//没人后黑屏0  常亮1
        stFACECTL.sLcdCloseTime    = 600;//没人脸后黑屏时间 0~600
        stFACECTL.ucWLedMode       = WLED_LINK_MOTION ;//led联动移动超时侦测
        stFACECTL.sLedMotionTime   = 600;//led 600s无物体就关闭
        nRet = glo->g_HciFunc->Intf_SetParam( HCI_CFG_FACECTL, 0, &stFACECTL, sizeof(AI_FACE_CTL_S) );
    }
#endif

    // 初始化ui类,和用来结束事件循环的定时器
    my_ui     = new ooo_ui;
    tim       = new QTimer;
    my_ui->set_text("now come in UsrPlatform_Task");
    tim->setInterval(100);
    tim->setSingleShot(true);
    QEventLoop event_sleep;
    QObject::connect(tim,SIGNAL(timeout()),&event_sleep,SLOT(quit())); //绑定信号:100ms 后结束事件循环


    // 线程初始化，用线程跑服务器
    http_ser   = new http_usr;
    T_http_ser = new QThread();
    QObject::connect(tim,SIGNAL(timeout()),http_ser,SLOT(init()));//100ms 后 初始化这个HTTP类
    QObject::connect(http_ser,SIGNAL(emit_err_string(QString)),my_ui,SLOT(set_text(QString))); // 链接服务器与ui,服务器想打印信息就能直接显示在ui上
    QObject::connect(http_ser,SIGNAL(emit_client(QString)),my_ui,SLOT(set_title(QString)));
    QObject::connect(http_ser,SIGNAL(emit_info(QString,QString)),my_ui,SLOT(set_info(QString,QString)));
    http_ser->moveToThread(T_http_ser);
    T_http_ser->start();
    tim->start();  // 100ms超时后，触发类初始化和事件循环的退出
    event_sleep.exec();
    QObject::disconnect(tim,SIGNAL(timeout()),http_ser,SLOT(init()));

#ifndef BUG_ooo
    qDebug() << err_func_show << "[ui]和[HTTP服务器]初始化成功！*******************";
#endif

    // 线程跑人脸识别
    http_scan http_serch;
    QThread   T_http_serch;
    QObject::connect(tim,SIGNAL(timeout()),&http_serch,SLOT(init()));//100ms 后 初始化这个HTTP类
    QObject::connect(&http_serch,SIGNAL(emit_usr(QString)),my_ui,SLOT(show_usr(QString)) );
    QObject::connect(http_ser,SIGNAL(emit_scan_pic()),&http_serch,SLOT(pic_scaning()));
    QObject::connect(&http_serch,SIGNAL(emit_show_pic(QString)),my_ui,SLOT(show_pic(QString)) );
    http_serch.moveToThread(&T_http_serch);
    T_http_serch.start();
    tim->start();  // 100ms超时后，触发新线程里类的初始化和事件循环的退出
    event_sleep.exec();
    QObject::disconnect(tim,SIGNAL(timeout()),&http_serch,SLOT(init()));



    qDebug() << err_func_show<<"init success!==================================";

    screen.hide();
    my_ui->show();
    functhread = true;
    a.exec();
    return NULL;
}


//主进程起来会去调用 HCM_USRPLATFORM_Init.
//注意此函数不能阻塞, 用户可以开启线程方式来初始化.可以通过pstHcFunc 来获取设备相关信息接口.
/*******************************************************************************
 * 函数功能：初始化全局类，创建主初始化线程
 * 参数说明：
 *  HCI_FUNC *pstHcFunc 主程序回传的全局函数指针
 * 返回值：
 * 　空
 * ******************************************************************************/
INT HCM_USRPLATFORM_Init(HCI_FUNC *pstHcFunc)
{
    qDebug() << err_func_show<< "come in===============================================";
    pthread_t thread_id = 0;


    // 在这里第一次调用静态全局类指针的初始化， 并把主线程的函数指针之地址　赋值给全局类
    glo = Global_Class::GetGlobal_class();
    glo->g_HciFunc = pstHcFunc;
    gusrplatformStart = 1;
    if(glo->g_HciFunc!=NULL)
            qDebug()<<err_func_show<<"HCI_FUNC OK*****************************************";
        else
            qDebug()<<err_func_show<<"HCI_FUNC NG*****************************************";
    gusrplatformStart = 1;


    //开启线程，作为我们界面线程以及其他业务线程的依托
    pthread_create(&thread_id, NULL, UsrPlatform_Task, (void *)NULL);

    qDebug() << err_func_show<< "exit!===============================================";
    return 0 ;
}


/*******************************************************************************
 * 函数功能：发送信号，让UsrPlatform_Task卸载相关资源
 * 参数说明：
 *   空
 * 返回值：
 * 　空
 * ******************************************************************************/
INT HCM_USRPLATFORM_UnInit(VOID)
{
    qDebug() << err_func_show<< "now come in===============================================";
#ifdef QT_DEBUG
	printf("HCM_USRPLATFORM_UnInit...\n");
#endif
    tim->start();
	return 0;
}

//func : from ai module  HCI_AI_SendAnalyse api
//ai模块信息回传 启动包括人脸识别成功失败相关信息、ai启动状态、 等人脸结果信息返回
/*******************************************************************************
 * 函数功能：获取AI模块的处理信息
 * 参数说明：
 * 　UINT nCh : ?
 * 　INT nAnalyseType  ：返回的状态识别码
 * 　VOID* pResultData　：返回的处理结果码、结构体
 * 　UINT nResultLen　　：　返回处理结果的数据长度
 * 返回值：
 * 　空
 * ******************************************************************************/
INT HCM_USRPLATFORM_DealAiNalyseData(UINT nCh, INT nAnalyseType,  VOID* pResultData, UINT nResultLen)
{
    qDebug() << err_func_show<< "come in===============================================";
    AI_COMPARE_REC_INFO *pCmpRecInfo;
    if(nAnalyseType == ANALYSE_START_E) //ai 模块启动状态.
    {
        if(*(AI_STARTSTATUS_TYPE_E *)pResultData == AI_START_SUCESS_E) // ai 模块启动成功.
        {
            printf("AI MODULES START  SUCESS !!!!\n");
            gHCMGetAiStartStatus = AI_START_SUCESS_E;

            qDebug() << err_func_show<< "open-success!================================================";
            my_ui->set_label(1,"启动成功");
            my_ui->set_label(2,"等待人脸!");

        }
        else if(*(AI_STARTSTATUS_TYPE_E *)pResultData == AI_START_FAIL_E)
        {
            printf("AI MODULES START  FAIL !!!!\n");
            gHCMGetAiStartStatus = AI_START_FAIL_E;

            qDebug() << err_func_show<< "falil !================================================";
            my_ui->set_label(1,"启动失败");
            my_ui->set_label(2,"不是吧!");

        }
    }
    else if(nAnalyseType==ANALYSE_FACEINFO_E || nAnalyseType==ANALYSE_CMPFAIL_E)//识别成功 或者 失败
    {
        if(!functhread)//libusrplatform.so的相关资源还未全部初始化
        {
            qDebug() << err_func_show<< "libusrplatform.so 初始化未完成=======================================";
            my_ui->set_label(2,"初始化未完成!");
            my_ui->set_label(1,"不是吧哥哥！");
            return 0; // 退出本次调用
        }

        qDebug() << err_func_show<< "face is success!================================================";
        if(nAnalyseType==ANALYSE_FACEINFO_E)
        {
            pCmpRecInfo = (AI_COMPARE_REC_INFO *)pResultData;
            if(/*pCmpRecInfo->usrid=="0" || */pCmpRecInfo->result<10) // 比对之后，相似值小于10% 退出
                return 0;

            my_ui->set_label(2,"识别成功了!");
            my_ui->set_label(1,QString("ID 为[%1]").arg(pCmpRecInfo->usrid));
            my_ui->show_usr(QString("%1 -匹配率为 [%2]").arg(pCmpRecInfo->name).arg(pCmpRecInfo->result) );
            my_ui->set_text("匹配率为 " +QString::number(pCmpRecInfo->result));
        }
        else if (nAnalyseType==ANALYSE_CMPFAIL_E)
        {
            my_ui->set_label(2,"识别失败了!");
            my_ui->set_label(1,QString("nAnalyseType = [%1]").arg(nAnalyseType));
            my_ui->set_text("now is " +QString::number(nAnalyseType));
        }
    }


    qDebug() << err_func_show<< "exit!===============================================";
    return 0;
}


/*******************************************************************************
 * 函数功能：空
 * 参数说明：
 *   空
 * 返回值：
 * 　空
 * ******************************************************************************/
CHAR* HCM_USRPLATFORM_GetInfo(VOID)
{
    qDebug() << err_func_show<< "now come in===============================================";
	return pVersion;
}
