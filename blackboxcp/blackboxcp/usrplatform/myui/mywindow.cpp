#include "mywindow.h"
#include "ui_mywindow.h"

#include <iostream>
#include <unistd.h>

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
#ifdef QT_DEBUG
    qDebug()<<"main winodw setupui"<<QThread::currentThread();
#endif

    QWSServer::setCursorVisible(false);

    QTextCodec* locale_codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(locale_codec);
    QTextCodec::setCodecForCStrings(locale_codec);
    QTextCodec::setCodecForTr(locale_codec);

    ui->setupUi(this);

    //透明
    setAutoFillBackground(false);  //这个不设置的话就背景变黑
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowState(Qt::WindowMaximized);//最大化

    networkport_flag=0;
    netbaidu=0;
    login_flag=0;
    //800*1280-8寸  720*1280-5寸  600*1024-7寸
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    screen_width = screenRect.width();
    screen_height = screenRect.height();

    no_net_last=1;
    tem_flag=0;
    nowtem=0;
    temdev_ok=0;
    upgrade_type=0;

    hidefacelabel();
    hideinfolabel();
}

MyWindow::~MyWindow()
{
    if(timer_showtime->isActive())
        timer_showtime->deleteLater();
    timer_showtime=NULL;

    if(showfacetime->isActive())
        showfacetime->deleteLater();
    showfacetime=NULL;

    if(showinfotime->isActive())
        showinfotime->deleteLater();
    showinfotime=NULL;

    delete ui;
}

void MyWindow::InitWindow()
{
    //隐藏下面文本提示框--出货的不显示
    //ui->widget_2->hide();

#ifdef QT_DEBUG
    qDebug()<<"init window"<<QThread::currentThread();
#endif

    glo=Global_class::GetInstance();

    timer_showtime = new QTimer();
    connect(timer_showtime, SIGNAL(timeout()), this, SLOT(ShowTime()));
    timer_showtime->start(15000);

    showfacetime=new QTimer();
    showfacetime->setSingleShot(1);
    connect(showfacetime,SIGNAL(timeout()),this,SLOT(showface_over()));

    showinfotime=new QTimer();
    showinfotime->setSingleShot(1);
    connect(showinfotime,SIGNAL(timeout()),this,SLOT(hideinfolabel()));

    wifi_level=1;

    QFont isfont;//("DejaVu Sans",20);//("DejaVu Sans",20,QFont::Normal)
    isfont.setPointSize(25);
    ui->label_info->setFont(isfont);
    //人脸框
    QSize fs(440,440);
    ui->label_faceimg->setMaximumSize(fs);
    ui->label_faceimg->setMinimumSize(fs);
    QSize fw(450,450);
    ui->widget_face->setMaximumSize(fw);
    ui->widget_face->setMinimumSize(fw);
    //用户信息框
    QSize us(450,200);
    ui->widget_user->setMaximumSize(us);
    ui->widget_user->setMinimumSize(us);
    QFont usfont;//("wenquanyi_160_75",160);
    usfont.setPointSize(40);
    ui->label_username->setFont(usfont);
    ui->label_userclass->setFont(usfont);
    //

    //门禁框
    usfont.setPointSize(30);
    ui->label_mj->setFont(usfont);

    //设置普通lable的字体，大小，高低等
    QFont ptfont;
    //ptfont=QFont("DejaVu Sans",20);
    ptfont.setPointSize(20);
    int labelh=40;
    //学校
    ui->label_school->setFont(ptfont);
    ui->label_school->setMinimumHeight(labelh);
    ui->label_school->setMaximumHeight(labelh);
//    ui->label_school->setText("学校信息");
    //时间
    ui->label_time->setFont(ptfont);
    ui->label_time->setMinimumHeight(labelh);
    ui->label_time->setMaximumHeight(labelh);
//    ui->label_time->setText("时间");
    //ptfont=QFont("DejaVu Sans",15);
    ptfont.setPointSize(15);
    //同步中
    ui->label_insync->setFont(ptfont);
    ui->label_insync->setMinimumHeight(labelh);
    ui->label_insync->setMaximumHeight(labelh);
//    ui->label_insync->setText("同步中:0");
    //已同步
    ui->label_allsync->setFont(ptfont);
    ui->label_allsync->setMinimumHeight(labelh);
    ui->label_allsync->setMaximumHeight(labelh);
//    ui->label_allsync->setText("已同步:0");
    //版本
    ui->label_version->setFont(ptfont);
    ui->label_version->setMinimumHeight(labelh);
    ui->label_version->setMaximumHeight(labelh);
//    ui->label_version->setText("版本:0");
    //网络
    ui->label_network->setFont(ptfont);
    ui->label_network->setMinimumHeight(labelh);
    ui->label_network->setMaximumHeight(labelh);
//    ui->label_network->setText("网络:-");
    //人脸数
    ui->label_facenum->setFont(ptfont);
    ui->label_facenum->setMinimumHeight(labelh);
    ui->label_facenum->setMaximumHeight(labelh);
//    ui->label_facenum->setText("人脸数:0");
    //用户数
    ui->label_usernum->setFont(ptfont);
    ui->label_usernum->setMinimumHeight(labelh);
    ui->label_usernum->setMaximumHeight(labelh);
//    ui->label_usernum->setText("用户数:");
    //设备
    ui->label_device->setFont(ptfont);
    ui->label_device->setMinimumHeight(labelh);
    ui->label_device->setMaximumHeight(labelh);
//    ui->label_device->setText("设备:");
    ui->label_number->setFont(ptfont);
    ui->label_number->setMinimumHeight(labelh);
    ui->label_number->setMaximumHeight(labelh);

    userfacebj=QImage("/usrappfs/face-pic/user-img.png");
    userinfobj=QImage("/usrappfs/face-pic/user-info.png");
    mjz_w=QImage("/usrappfs/face-pic/tl-white.png");
    mjy_w=QImage("/usrappfs/face-pic/tr-white.png");
    mjt_w=QImage("/usrappfs/face-pic/tc-white.png");
    mjz_r=QImage("/usrappfs/face-pic/tl-red.png");
    mjy_r=QImage("/usrappfs/face-pic/tr-red.png");
    mjt_r=QImage("/usrappfs/face-pic/tc-red.png");
    t28=QImage("/usrappfs/face-pic/t28.png");

    QImage fb=userfacebj.scaled(ui->widget_face->size(),Qt::KeepAspectRatio);
    QPalette pface;
    pface.setBrush(QPalette::Background, QBrush(QPixmap::fromImage(fb)));
    ui->widget_face->setAutoFillBackground(1);
    ui->widget_face->setPalette(pface);

    QImage fu=userinfobj.scaled(ui->widget_user->size(),Qt::IgnoreAspectRatio);
    QPalette puser;
    puser.setBrush(QPalette::Background, QBrush(QPixmap::fromImage(fu)));
    ui->widget_user->setAutoFillBackground(1);
    ui->widget_user->setPalette(puser);

    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::white);//31,229,255
    ui->label_username->setPalette(pe);
    ui->label_userclass->setPalette(pe);

    //背景frame
    ui->frame_bj->setAutoFillBackground(1);
    palette_bjtm=ui->frame_bj->palette();
    palette_bjtm.setBrush(QPalette::Window,QColor(0,0,0,0));
    ui->frame_bj->setPalette(palette_bjtm);

    palette_bjjb=ui->frame_bj->palette();

    //信息框
    //ui->widget_info->setStyleSheet("border-radius:10px;");
    ui->widget_info->setAutoFillBackground(1);
    palette_info=ui->widget_info->palette();

    QImage tt=t28.scaled(ui->label_t->size(),Qt::KeepAspectRatio);
    ui->label_t->setPixmap(QPixmap::fromImage(tt));

    hide_mj();

#ifdef QT_DEBUG
    qDebug()<<"设置字体结束";
#endif
    ShowTime();
}

QPixmap MyWindow::PixmapToRound(QPixmap src, int radius)
{
    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    //painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), radius, radius);

    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

void MyWindow::showfacelabel()
{
    ui->widget_face->show();
    ui->widget_user->show();
}

void MyWindow::hidefacelabel()
{
    ui->frame_bj->setPalette(palette_bjtm);
    ui->widget_face->hide();
    ui->widget_user->hide();
}

void MyWindow::showinfolabel(int t)
{
    if(t==1)
    {
        int minw=ui->widget_info->width()*0.7;
        ui->label_info->setMinimumWidth(minw);
        ui->label_info->setMaximumWidth(minw+10);
        ui->label_info->setSizeIncrement(ui->widget_info->height(),minw+5);
    }
    else
    {
        ui->label_info->setMinimumWidth(0);
        ui->label_info->adjustSize();
    }
    ui->label_info->setMinimumHeight(ui->widget_info->height());
    ui->widget_info->show();
}

void MyWindow::hideinfolabel()
{
    if(upgrade_type==2||upgrade_type==3)
    {
        QString utext=QString::fromLocal8Bit("即将更新,请勿重启或断电");
        if(upgrade_type==3)
            utext=QString::fromLocal8Bit("更新成功,即将重启,请勿断电");
        ui->label_info->setText(utext);
    }
    else
    {
        ui->label_info->setText("");
        ui->widget_info->hide();
    }
}

void MyWindow::show_mj()
{
    ui->label_mt->show();
    ui->label_z->show();
    ui->label_y->show();
    ui->label_mj->show();
}

void MyWindow::hide_mj()
{
    ui->label_mt->hide();
    ui->label_z->hide();
    ui->label_y->hide();
    ui->label_mj->hide();
}

void MyWindow::showface_over()
{
    hideinfolabel();
    hidefacelabel();
    hide_mj();
    showtem();
}

void MyWindow::show_user(HCP_DETECT_INFO pInfo,int temthis)
{
    #ifdef QT_DEBUG
    qDebug()<<"进入显示新人脸函数:"<<QDateTime::currentMSecsSinceEpoch();
    #endif
    static int fff=0;
    if(fff==0)
    {
        QRect sf=ui->frame_bj->rect();
        QLinearGradient jbs(sf.x(), sf.y(), sf.x(), (sf.y()+sf.height()));

        jbs.setColorAt(0,QColor(51,153,102,70));
        jbs.setColorAt(0.5,QColor(51,153,102,250));
        jbs.setColorAt(1.0,QColor(51,153,102,70));

        auto role = QPalette::Window;
        palette_bjjb_ok.setBrush(role,QBrush(jbs));

        jbs.setColorAt(0,QColor(255,0,0,70));
        jbs.setColorAt(0.5,QColor(255,0,0,240));
        jbs.setColorAt(1.0,QColor(255,0,0,70));
        palette_bjjb_ng.setBrush(role,QBrush(jbs));

        jbs.setColorAt(0,QColor(0,0,0,10));
        jbs.setColorAt(0.5,QColor(0,0,0,10));
        jbs.setColorAt(1.0,QColor(0,0,0,10));
        palette_bjjb_face.setBrush(role,QBrush(jbs));

        fff=1;
    }

    QString st=ui->label_info->text();
    if(st.contains(QString::fromLocal8Bit("过高")))
    {
        hideinfolabel();
    }

    if(showfacetime->isActive())
        showfacetime->stop();
    //获取用户信息
    HCP_DETECT_INFO n_di;
    memset(&n_di, 0, sizeof(HCP_DETECT_INFO));
    n_di.iKnow = pInfo.iKnow;
    qstrcpy(n_di.szPath, pInfo.szPath);
    qstrcpy(n_di.szName, pInfo.szName);
    qstrcpy(n_di.szInfo, pInfo.szInfo);
    QString userName = QString::fromLocal8Bit(n_di.szName);
    QString userInfo = QString::fromLocal8Bit(n_di.szInfo);

    hide_mj();
    //ui->widget_tem->setContentsMargins(40,0,40,0);//
    ui->frame_bj->setPalette(palette_bjjb_face);
    showfacelabel();

    ui->label_username->setText(userName);
    ui->label_userclass->setText(userInfo);

    QImage pixmap_user(n_di.szPath);//QPixmap读取图片耗时比qimage多
    QSize sf;
    sf.setWidth(ui->label_faceimg->width()*0.8);
    sf.setHeight(ui->label_faceimg->height()*0.8);
    QImage showuserImg=pixmap_user.scaled(sf,Qt::IgnoreAspectRatio);//Qt::KeepAspectRatio
    ui->label_faceimg->setPixmap(QPixmap::fromImage(showuserImg));

    //如果需要测温
    if(temthis==1)
    {
        if(temdev_ok)
        {
            palette_info.setBrush(QPalette::Window,QColor(31,229,255,175));
            ui->label_info->setText(QString::fromLocal8Bit("请测温"));
        }
        else
        {
            palette_info.setBrush(QPalette::Window,QColor(255,0,0,175));
            ui->label_info->setText(QString::fromLocal8Bit("请连接测温插件并重启设备"));
        }
        ui->widget_info->setPalette(palette_info);
        if(showinfotime->isActive())
            showinfotime->stop();
        showinfolabel();

        ui->label_tem->show();
        showfacetime->start(tem_timeout*1000+1000);//加1秒，用于适应测温超时,但可以通行的用户
    }
    #ifdef QT_DEBUG
    qDebug()<<"人脸显示完毕:"<<QDateTime::currentMSecsSinceEpoch();
    #endif
}

void MyWindow::show_door(int open_type,float tw)
{
#ifdef QT_DEBUG
    qDebug()<<"UI进入显示通行结果函数"<<open_type<<QDateTime::currentMSecsSinceEpoch();
#endif

    if(showfacetime->isActive())
        showfacetime->stop();

    QImage st;
    QImage sz;
    QImage sy;
    QPalette pe;
    hideinfolabel();
    if(open_type==0)
    {
        //hide_mj();//在刷人脸时已经隐藏
        ui->label_tem->hide();

        ui->frame_bj->setPalette(palette_bjjb_ok);
        showfacetime->start(3000);
    #ifdef QT_DEBUG
        qDebug()<<"通行结果显示完毕"<<QDateTime::currentMSecsSinceEpoch();
    #endif
        return;
    }
    else if(open_type==1||open_type==11||open_type==21)
    {
        st=mjt_r.scaled(ui->label_mt->size(),Qt::KeepAspectRatio);
        sz=mjz_r.scaledToWidth(ui->label_z->width(),Qt::SmoothTransformation);
        sy=mjy_r.scaledToWidth(ui->label_y->width(),Qt::SmoothTransformation);
        pe.setColor(QPalette::WindowText, Qt::red);
        ui->label_mj->setText(QString::fromLocal8Bit("您当前处于门禁时段"));
        ui->frame_bj->setPalette(palette_bjjb_ng);
    }
    else if(open_type==2 || open_type==10 || open_type==12)
    {
        st=mjt_w.scaled(ui->label_mt->size(),Qt::KeepAspectRatio);
        sz=mjz_w.scaledToWidth(ui->label_z->width(),Qt::SmoothTransformation);
        sy=mjy_w.scaledToWidth(ui->label_y->width(),Qt::SmoothTransformation);
        pe.setColor(QPalette::WindowText, Qt::white);
        ui->frame_bj->setPalette(palette_bjjb_ok);
        if(open_type==2)
        {
            ui->label_mj->setText(QString::fromLocal8Bit("您当前处于请假通行时段"));
        }
        else
        {
            ui->label_mj->setText(QString::fromLocal8Bit("体温正常：")+QString::number(tw-0.05,'f',1)+"°C");
            if(open_type==12)
            {
                //上方显示请假通行阶段
                palette_info.setBrush(QPalette::Window,QColor(31,229,255,175));
                ui->widget_info->setPalette(palette_info);
                ui->label_info->setText(QString::fromLocal8Bit("您当前处于请假通行时段"));
                if(showinfotime->isActive())
                    showinfotime->stop();
                showinfolabel();
            }
        }
    }
    else if(open_type==13 || open_type==20 || open_type==22)
    {
        st=mjt_r.scaled(ui->label_mt->size(),Qt::KeepAspectRatio);
        sz=mjz_r.scaledToWidth(ui->label_z->width(),Qt::SmoothTransformation);
        sy=mjy_r.scaledToWidth(ui->label_y->width(),Qt::SmoothTransformation);
        pe.setColor(QPalette::WindowText, Qt::red);
        ui->label_mj->setText(QString::fromLocal8Bit("体温异常：")+QString::number(tw-0.05,'f',1)+"°C");
        if(open_type==13)//异常不通行
        {
            ui->frame_bj->setPalette(palette_bjjb_ng);
        }
        else//异常通行
        {
            ui->frame_bj->setPalette(palette_bjjb_ok);
            if(open_type==22)
            {
                //上方显示请假通行阶段
                palette_info.setBrush(QPalette::Window,QColor(31,229,255,175));
                ui->widget_info->setPalette(palette_info);
                ui->label_info->setText(QString::fromLocal8Bit("您当前处于请假通行时段"));
                if(showinfotime->isActive())
                    showinfotime->stop();
                showinfolabel();
            }
        }
    }
    ui->label_tem->hide();
    ui->label_mt->setPixmap(QPixmap::fromImage(st));
    ui->label_z->setPixmap(QPixmap::fromImage(sz));
    ui->label_y->setPixmap(QPixmap::fromImage(sy));
    ui->label_mj->setPalette(pe);
    show_mj();
    showfacetime->start(3000);
#ifdef QT_DEBUG
    qDebug()<<"通行结果显示完毕"<<QDateTime::currentMSecsSinceEpoch();
#endif
}

void MyWindow::show_MAC_V(QString mac, QString vis,QString schoolname)
{
    ui->label_device->setText(QString::fromLocal8Bit("设备:")+mac);
    ui->label_version->setText(QString::fromLocal8Bit("版本:")+vis);
    ui->label_school->setText("  "+schoolname);
}

void MyWindow::show_network(bool flag)
{
    xbull_net=flag;
    show_network_ui();
}

void MyWindow::show_network_ui()
{
    //没找到活跃的网卡
    if(networkport_flag==0)
    {
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::red);
        ui->label_network->setPalette(pe);
        ui->label_network->setText(QString::fromLocal8Bit("网络:未连接"));
        no_net_last=1;
        return;
    }

    QPalette pe;
    if(wifi_flag==0)//有线网络
    {
        QString net_stat;
        if(dhcp_ip==0)//静态IP
            net_stat=QString::fromLocal8Bit("网络:")+n_ip;
        else//动态IP
            net_stat=QString::fromLocal8Bit("网络:有线网络");

        if(netbaidu==0)
        {
            pe.setColor(QPalette::WindowText, Qt::red);
            ui->label_network->setPalette(pe);
            if(no_net_last)
            {
                ui->label_network->setText(net_stat+QString::fromLocal8Bit(" ~ 检测中"));
                no_net_last=0;
            }
            else
            {
                ui->label_network->setText(net_stat+QString::fromLocal8Bit(" ~ 不可用"));
            }
            return;
        }

        if(xbull_net==0)
        {
            pe.setColor(QPalette::WindowText, Qt::red);
            ui->label_network->setPalette(pe);
            ui->label_network->setText(net_stat+QString::fromLocal8Bit(" ~ 等待心跳包"));
            return;
        }

        pe.setColor(QPalette::WindowText, Qt::white);
        ui->label_network->setPalette(pe);
        ui->label_network->setText(net_stat+QString::fromLocal8Bit(" ~ 可用"));
    }
    else//wifi默认为动态IP
    {
        QString net_stat="WIFI: "+wifi_name;
        QString wfl="";
        if(wifi_level!=0)
            wfl=QString::number(wifi_level);

        if(netbaidu==0)
        {
            pe.setColor(QPalette::WindowText, Qt::red);
            ui->label_network->setPalette(pe);

            if(no_net_last)
            {
                wfl=wfl+QString::fromLocal8Bit(" ~ 检测中");
                no_net_last=0;
            }
            else
            {
                wfl=wfl+QString::fromLocal8Bit("-不可用");
            }
            QString info = "<html><head/><body><p>"+net_stat+"-<img src='/usrappfs/face-pic/wifi-red.png'/>"+wfl+"</p></body></html>";
            ui->label_network->setText(info);
            return;
        }
        if(xbull_net==0)
        {
            pe.setColor(QPalette::WindowText, Qt::red);
            ui->label_network->setPalette(pe);
            wfl=wfl+QString::fromLocal8Bit("-等待心跳包");
            QString info = "<html><head/><body><p>"+net_stat+"-<img src='/usrappfs/face-pic/wifi-red.png'/>"+wfl+"</p></body></html>";
            ui->label_network->setText(info);
            return;
        }

        pe.setColor(QPalette::WindowText, Qt::white);
        ui->label_network->setPalette(pe);
        wfl=wfl+QString::fromLocal8Bit("-可用");
        QString infoo = "<html><head/><body><p>"+net_stat+"-<img src='/usrappfs/face-pic/wifi-white.png'/>"+wfl+"</p></body></html>";
        ui->label_network->setText(infoo);
    }
}

void MyWindow::show_syning(int syn_ing)
{
    ui->label_insync->setText(QString::fromLocal8Bit("  同步中:")+QString::number(syn_ing));
}

void MyWindow::show_synall(int syn_all)
{
    ui->label_allsync->setText(QString::fromLocal8Bit("已同步:")+QString::number(syn_all));
}

void MyWindow::show_face_user(int usernum, int facenum)
{
    ui->label_facenum->setText(QString::fromLocal8Bit("  人脸数:")+QString::number(facenum));
    ui->label_usernum->setText(QString::fromLocal8Bit("用户数:")+QString::number(usernum));
}

void MyWindow::ShowTime()
{
    //当前时间
    QDateTime dati = QDateTime::currentDateTime();
    QString str = dati.toString("yyyy/MM/dd hh:mm");
    ui->label_time->setText(str+"  ");

    //判断是否要测温
    nowtem=tem_flag;
    if(nowtem==1)
    {
        //获取当天的秒时间
        int nows=QTime::currentTime().hour()*3600+QTime::currentTime().minute()*60+QTime::currentTime().second();
        //判断是否在不测温时段内
        for(int i=0;i<notem.size();i++)
        {
            if(notem.at(i).at(0).toInt()<=nows && notem.at(i).at(1).toInt()>=nows)
            {
                nowtem=0;
                break;
            }
        }
    }

    showtem();

    //获得网卡信息
    networkport_flag=0;
    n_ip="";
    //获得网卡信息状态
    int i1=0;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface netInterface, list)
    {
        i1++;
        if (!netInterface.isValid())
            continue;

        QNetworkInterface::InterfaceFlags flags = netInterface.flags();
        // 网络接口处于活动状态
        if (flags.testFlag(QNetworkInterface::IsRunning)
        && !flags.testFlag(QNetworkInterface::IsLoopBack))
        {
            /*qDebug() << "noooooooo-"<<i1;
            qDebug() << "Device : " << netInterface.name();  // 设备名
            qDebug() << "HardwareAddress : " << netInterface.hardwareAddress();  // 硬件地址
            qDebug() << "Human Readable Name : " << netInterface.humanReadableName();  // 人类可读的名字*/

            //在4.8中，没有type来区分wifi和有线

            // 遍历每一个IP地址
            QList<QNetworkAddressEntry> entryList = netInterface.addressEntries();
            foreach(QNetworkAddressEntry entry, entryList)
            {
               /* qDebug() << "IP Address:" << entry.ip().toString();  // IP地址
                qDebug() << "Netmask:" << entry.netmask().toString();  // 子网掩码
                qDebug() << "Broadcast:" << entry.broadcast().toString();  // 广播地址
                */
                n_ip=entry.ip().toString();
                //广播地址不为空，才认为网线正常连接
                /*
                if(entry.broadcast().toString()!="")
                {
                    networkport_flag=1;
                    n_ip=entry.ip().toString();
                }*/
            }
            //获取到第一个活跃的跳出
            if(n_ip!="")
            {
                networkport_flag=1;
                QString dev_name=netInterface.humanReadableName();
                //检查设备名上是否有wlan或者wifi的字符，有就为wifi模块 Qt::CaseInsensitive--不分大小写
                if(dev_name.contains("wlan",Qt::CaseInsensitive) || dev_name.contains("wifi",Qt::CaseInsensitive))
                    wifi_flag=1;
                else
                    wifi_flag=0;

                break;
            }
        }
    }

    get_wifiname();
    if(networkport_flag==0)
    {
        show_network_ui();
        return;
    }

    //qDebug()<<"ping百度看能否上外网";
    QHostInfo::lookupHost("www.baidu.com",this,SLOT(onLookupHost(QHostInfo)));
    QHostInfo::lookupHost("www.x-bull.com",this,SLOT(onLookupHost_xbull(QHostInfo)));
}

void MyWindow::devnofound(int s, QString mac)
{
#ifdef QT_DEBUG
    qDebug()<<"显示设备在平台的注册情况[仅未注册才显示]";
#endif

    if(s==1)
    {
        palette_info.setBrush(QPalette::Window,QColor(255,0,0,175));
        ui->widget_info->setPalette(palette_info);
        ui->label_info->setText(QString::fromLocal8Bit("设备:")+mac+QString::fromLocal8Bit(" 还未注册"));
        if(showinfotime->isActive())
            showinfotime->stop();
        showinfolabel();
        login_flag=1;
    }
    else
    {
        if(login_flag)
            hideinfolabel();
        login_flag=0;
    }
}

void MyWindow::update_p(int loadtype,int utype,float s,int all)
{
    if(all<=0)//下载为0不显示--如果有多个增量任务堆积，第一个就会更新到最新，后面的拿到的数据都是空的，可以不显示
        return;

    QString typestr;
    if(loadtype==0)//此为一般情况下的下载数据
        typestr=QString::fromLocal8Bit("正在下载用户数据[")+QString::number(all)+QString::fromLocal8Bit("],已完成:");
    else
    {
        //此为上报状态后，有数据不对进行数据校正
        if(utype==0)
            typestr=QString::fromLocal8Bit("正在更新学生用户数据[")+QString::number(all)+QString::fromLocal8Bit("],已完成:");
        else if(utype==1)
            typestr=QString::fromLocal8Bit("正在更新家长用户数据[")+QString::number(all)+QString::fromLocal8Bit("],已完成:");
        else if(utype==2)
            typestr=QString::fromLocal8Bit("正在更新职工用户数据[")+QString::number(all)+QString::fromLocal8Bit("],已完成:");
        else
            typestr=QString::fromLocal8Bit("正在更新访客用户数据[")+QString::number(all)+QString::fromLocal8Bit("],已完成:");
    }

    double s100=s*100;
    ui->label_info->setText(typestr+QString::number(s100,'f',2)+"%,请稍候再识别~");
    palette_info.setBrush(QPalette::Window,QColor(31,229,255,175));
    ui->widget_info->setPalette(palette_info);
    if(showinfotime->isActive())
        showinfotime->stop();
    showinfolabel();
}

void MyWindow::all_update(int alluser)
{
    palette_info.setBrush(QPalette::Window,QColor(31,229,255,175));
    ui->widget_info->setPalette(palette_info);
    ui->label_info->setText(QString::fromLocal8Bit("更新完成,已重新录入,当前共有用户")+QString::number(alluser)+QString::fromLocal8Bit("位"));
    if(showinfotime->isActive())
        showinfotime->stop();
    showinfolabel(1);
    showinfotime->start(5000);
}

void MyWindow::add_update(int adduser, int subuser, int alluser)
{
    if((adduser+subuser)<=0)//增量变化的用户为0，不显示--如果有多个增量任务堆积，第一个就会更新到最新，后面的拿到的数据都是空的，可以不显示
        return;

    palette_info.setBrush(QPalette::Window,QColor(31,229,255,175));
    ui->widget_info->setPalette(palette_info);
    ui->label_info->setText(QString::fromLocal8Bit("更新完成,新增用户")+QString::number(adduser)+
                            QString::fromLocal8Bit("位,删除用户")+QString::number(subuser)+
                            QString::fromLocal8Bit("位,当前共有用户")+QString::number(alluser)+
                            QString::fromLocal8Bit("位"));
    if(showinfotime->isActive())
        showinfotime->stop();
    showinfolabel();
    showinfotime->start(5000);
}

void MyWindow::showui_debug(int s)
{
    if(s==1)
        ui->widget_2->show();
    else
        ui->widget_2->hide();
    showdebug=s;
}

void MyWindow::lsdata(int type, int htime,int wetime,int taskover)
{
    if(showdebug==0)
        return;

    if(type==0)
    {
        ui->lineEdit_img->setText(QString::fromLocal8Bit("抓拍到进入函数的耗时[ms]:")+QString::number(htime));
        //这个基本在10ms内，不再显示，该label留给界面耗时使用
        //ui->lineEdit_wefunc->setText(QString::fromLocal8Bit("处理函数耗时[ms]:")+QString::number(wetime));
    }
    else if(type==1)
    {
        static int rwnum=0;
        rwnum++;
        QString oo;
        if(taskover==1)
            oo=QString::fromLocal8Bit("成功");
        else
            oo=QString::fromLocal8Bit("失败");
        ui->lineEdit_taks->setText(oo+"|"+QString::number(wetime)+QString::fromLocal8Bit("-本次任务执行耗时:")+QString::number(htime)+"ms | "+QString::number(rwnum));
    }
}

void MyWindow::shouhj(int hj)
{
    if(hj==1)
        ui->lineEdit_hj->setText(QString::fromLocal8Bit("测试版本"));
    else if(hj==2)
        ui->lineEdit_hj->setText(QString::fromLocal8Bit("灰度版本"));
    else
        ui->lineEdit_hj->setText(QString::fromLocal8Bit("正式版本"));
}

void MyWindow::updatesta(int utype,QString ustr)
{
    if(showdebug==1)
    {
        ui->lineEdit_up->setText(ustr);
    }

    upgrade_type=utype;
    if(utype==1)
    {
        palette_info.setBrush(QPalette::Window,QColor(31,229,255,175));
        ui->widget_info->setPalette(palette_info);
        QString gxtime=QString::fromLocal8Bit("设备即将在")+ustr+QString::fromLocal8Bit("更新");
        ui->label_info->setText(gxtime);
    }
    else if(utype==2 || utype==3)
    {
        palette_info.setBrush(QPalette::Window,QColor(31,229,255,175));
        ui->widget_info->setPalette(palette_info);
        ui->label_info->setText(ustr);
    }
    else if(utype==4 || utype==5 || utype==6)
    {
        palette_info.setBrush(QPalette::Window,QColor(255,0,0,175));
        ui->widget_info->setPalette(palette_info);
        ui->label_info->setText(ustr);
    }

    if(showinfotime->isActive())
        showinfotime->stop();
    showinfolabel();
    if(utype!=2 && utype!=3)
       showinfotime->start(5000);
}

void MyWindow::upfacedata(int us)
{
    if(showdebug==0)
        return;

    if(us==0)
        ui->lineEdit_upimg->setText(QString::fromLocal8Bit("上传识别数据成功"));
    else
        ui->lineEdit_upimg->setText(QString::fromLocal8Bit("上传识别数据失败:")+QString::number(us));
}

void MyWindow::tcpsta(QString sta)
{
    if(showdebug==0)
        return;
    ui->lineEdit_upimg->setText(sta);
}

void MyWindow::onLookupHost(QHostInfo host)
{
    //qDebug()<<"ping百度结束";
    if (host.error() != QHostInfo::NoError)
    {
        netbaidu=0;
    }
    else
    {
        netbaidu=1;
    }
    show_network_ui();
}

void MyWindow::onLookupHost_xbull(QHostInfo host)
{
    if (host.error() != QHostInfo::NoError)
    {
        xbull_net=0;
    }
    else
    {
        xbull_net=1;
    }
    show_network_ui();
}

void MyWindow::get_wifiname()
{
    if(wifi_flag==0)
    {
#ifdef QT_DEBUG
        qDebug()<<"有线连接，显示时不搜索WiFi名称";
#endif
        dhcp_ip=0;
        HCI_NETBASE_S stHNS;
        memset( &stHNS, 0, sizeof( stHNS ) );
        int netret=glo->g_HciFunc->Intf_GetParam( HCI_CFG_NET, 0, &stHNS, sizeof(stHNS));
        //qDebug()<<"临时打印,请求状态:"<<netret;
        if(netret==0)
        {
            if(stHNS.ibDhcp==0)//未开启
                dhcp_ip=0;
            else//开启
                dhcp_ip=1;
            //qDebug()<<"临时打印,DHCP状态:"<<dhcp_ip;
        }
    }
    else
    {
#ifdef QT_DEBUG
            qDebug()<<"无线连接,搜索WiFi名称";
#endif
        wifi_name="";
        wifi_level=0;
        //wifi
        HCM_WIFISTATUS_S stwifi;
        memset( &stwifi, 0, sizeof(stwifi));
        int nRet;
        //获取wifi的一些信息
        //关于wifi-第二个参数: 通道设置为0时为获取WIFI配置信息，设置为1时是获取wifi搜索列表，设置为2时是获取状态
        nRet = glo->g_HciFunc->Intf_GetParam( HCI_CFG_WIFI, 2, &stwifi, sizeof(stwifi) );
        if(nRet==0)
        {
#ifdef QT_DEBUG
            qDebug()<<"wifi状态:"<<stwifi.enWpa_State;
#endif
            if(stwifi.enWpa_State==2)//wifi连接成功
            {
                wifi_name=QString::fromStdString(stwifi.bySSID);
                QString okwifi_bssid16="";
                for(int i=0;i<6;i++)
                {
                    int s;
                    s=stwifi.byBSSID[i];// & 0xff;
                    QString h16=QString::number(s, 16);
                    okwifi_bssid16=okwifi_bssid16+h16;
                }
#ifdef QT_DEBUG
                qDebug()<<"已连接wifi的ssid:"<<stwifi.bySSID;
                qDebug()<<"已连接wifi的bssid:"<<stwifi.byBSSID;
                qDebug()<<"已连接wifi的bssid--16:"<<okwifi_bssid16;

#endif
                //获得wifi列表，用于获取wifi的信号强度
                HCM_WIFILIST_S all_wifi;
                memset( &all_wifi, 0, sizeof(all_wifi));
                nRet = glo->g_HciFunc->Intf_GetParam( HCI_CFG_WIFI, 1, &all_wifi, sizeof(all_wifi));
                if(nRet==0)
                {
                    int wifinum=all_wifi.byWifiNum;
#ifdef QT_DEBUG
                    qDebug()<<"索引到的wifi总数:"<<wifinum;
#endif
                    for(int i=0;i<wifinum;i++)
                    {
                        //SSID可能有重复的，这里使用BSSID来判断是否要找的WiFi
                        QString sywifi_bssid16="";
                        for(int x=0;x<6;x++)
                        {
                            int s;
                            s=all_wifi.stList[i].byBSSID[x];// & 0xff;
                            QString h16=QString::number(s, 16);
                            sywifi_bssid16=sywifi_bssid16+h16;
                        }
                        if(sywifi_bssid16=="000000")
                            continue;

#ifdef QT_DEBUG
                        qDebug()<<"索引到的wifi no:"<<i;
                        qDebug()<<"索引到的wifi的ssid:"<<all_wifi.stList[i].bySSID;
                        qDebug()<<"索引到的wifi的bssid:"<<all_wifi.stList[i].byBSSID;
                        qDebug()<<"索引到wifi的bssid--16:"<<sywifi_bssid16;
                        qDebug()<<"";
#endif
                        //int xsd=strcmp(okwifi_bssid16.toStdString().c_str(), sywifi_bssid16.toStdString().c_str());
                        if(okwifi_bssid16==sywifi_bssid16)//是否连接的WiFi
                        {

#ifdef QT_DEBUG
                            qDebug()<<"wifi 找到相同!!!!!!!!!!!!!!!!!!!!!!!!!";
                            qDebug()<<"wifi level:"<<all_wifi.stList[i].dwSignal;
#endif
                            //dwSignal这个值减100差不多就是db值了
                            wifi_level=all_wifi.stList[i].dwSignal-100;
                            if(wifi_level > -1)
                                wifi_level=-1;
                            if(wifi_level < -100)
                                wifi_level=-100;

                            break;
                        }
                    }
                }
                else
                {
                    //获取wifi的搜索列表失败，可能是没插无线网卡，也可能是无线网卡坏了
                    //qDebug()<<"get wifi list info fail:"<<nRet;
                }
            }
        }
        else
        {
            //获取wifi的状态失败，可能是没插无线网卡，也可能是无线网卡坏了
            //qDebug()<<"get wifi info fail:"<<nRet;
        }
    }
}

void MyWindow::set_temp(int tf,int tt,QList<QStringList> donttem)
{
    #ifdef QT_DEBUG
    qDebug()<<"界面收到测温的相关参数:"<<tf<<tt<<donttem;
    #endif
    tem_flag=tf;
    tem_timeout=tt;
    notem.clear();
    for(int i=0;i<donttem.size();i++)
    {
        notem.append(donttem.at(i));
    }
}

void MyWindow::temtoohigh(float temvalue)
{
    palette_info.setBrush(QPalette::Window,QColor(31,229,255,175));
    ui->widget_info->setPalette(palette_info);
    ui->label_info->setText(QString::number(temvalue-0.05,'f',1)+QString::fromLocal8Bit("°C 体温过高，请重测"));
    if(showinfotime->isActive())
        showinfotime->stop();
    showinfolabel();
    ui->label_tem->show();
    if(isvoied==0)
        glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/temaglen.aac");
}

void MyWindow::temdev_sta_func(bool flag)
{
    temdev_ok=flag;
    showtem();
}

void MyWindow::get_vi_ui(int f)
{
    isvoied=f;
}

void MyWindow::showtem()
{
    if(nowtem==1)
    {
        QString temtext=ui->label_info->text();
        if(temdev_ok)//测温插件正常
        {
            ui->label_tem->show();
            if(temtext.contains((QString::fromLocal8Bit("插件"))))//有显示 未连接插件 就先隐藏提示框
            {
                hideinfolabel();
            }
        }
        else
        {
            ui->label_tem->hide();
            if(!temtext.contains((QString::fromLocal8Bit("用户"))))//如果当前正在下载用户信息，就不显示
            {
                palette_info.setBrush(QPalette::Window,QColor(255,0,0,175));
                ui->widget_info->setPalette(palette_info);
                ui->label_info->setText(QString::fromLocal8Bit("请连接测温插件并重启设备"));
                if(showinfotime->isActive())
                    showinfotime->stop();
                showinfolabel();
            }
        }
    }
    else
    {
        QString temtext=ui->label_info->text();
        if(temtext.contains((QString::fromLocal8Bit("插件"))))
        {
            hideinfolabel();
        }
        ui->label_tem->hide();
    }
}

void MyWindow::showtemnum(int errort,float readtem, float overtem,float pcz)
{
    if(showdebug==0)
        return;
    if(errort>=0)
        ui->lineEdit_wefunc->setText(QString::fromLocal8Bit("测温状态:")+QString::number(errort)
                                     +QString::fromLocal8Bit(",原始温度:")+QString::number(readtem-0.05,'f',2)+
                                     QString::fromLocal8Bit(",补偿结果:")+QString::number(overtem-0.05,'f',2)+
                                     "["+QString::number(pcz,'f',1)+"]");
    else
        ui->lineEdit_wefunc->setText(QString::fromLocal8Bit("空气温度-不处理"));
}

void MyWindow::showtemdootD(int temt, int doort)
{
    if(showdebug==0)
        return;

    ui->lineEdit_up->setText(QString::fromLocal8Bit("测温耗时:")+QString::number(temt)+
                             QString::fromLocal8Bit(",门禁耗时:")+QString::number(doort));
}

void MyWindow::cardisoff(QString cardinfo)
{
    if(showinfotime->isActive())
        showinfotime->stop();

    if(ui->label_info->text().contains(QString::fromLocal8Bit("注册")))
    {
        return;
    }

    if(cardinfo.contains(QString::fromLocal8Bit("开启")))
    {
        if(isvoied==0)
            glo->g_HciFunc->Intf_AudioPlayFile(0, "/usrappfs/audio/cardoff.aac");
    }

    palette_info.setBrush(QPalette::Window,QColor(255,0,0,175));
    ui->widget_info->setPalette(palette_info);
    ui->label_info->setText(cardinfo);
    showinfolabel();
    showinfotime->start(5000);
}

void MyWindow::show_cha_num(int cha, int num)
{
    ui->label_number->setText(QString::fromLocal8Bit("编号")+QString::number(num)+
                              QString::fromLocal8Bit("频道")+QString::number(cha));
}
