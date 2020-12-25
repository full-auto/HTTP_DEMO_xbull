#include <QApplication>
#include <qdebug.h>
#include <QWSServer>
#include <qtextcodec.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qdebug.h>
#include <qstring.h>
#include <string>
#include <unistd.h>
#include <qbitmap.h>
#include <qpainter.h>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>
#include <qdesktopwidget.h>

#include "tenniuwindow.h"
#include "ui_mainwindow.h"

TenNiuWindow::TenNiuWindow(QWidget *parent):
    QMainWindow(parent),ui(new Ui::MainWindow), screen_width(800), screen_height(1300)
{
    qDebug()<<"main winodw setupui"<<QThread::currentThread();
    QWSServer::setCursorVisible(false);

    QTextCodec* locale_codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(locale_codec);
    QTextCodec::setCodecForCStrings(locale_codec);
    QTextCodec::setCodecForTr(locale_codec);


    ui->setupUi(this);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    screen_width = screenRect.width();
    screen_height = screenRect.height();

    qDebug() << screen_width << "*" << screen_height;

    if( screen_width == 800 && screen_height == 1280 )
    {
        m_resType = E_RESOLUTION_800_1280;
        qDebug()<<"m_resType 800_1280";
    }
    else if( screen_width == 720 && screen_height == 1280 )
    {
        m_resType = E_RESOLUTION_720_1280;
        qDebug()<<"m_resType 720_1280";
    }
    else if ( screen_width == 600 && screen_height == 1024 )
    {
        m_resType = E_RESOLUTION_600_1024;
        qDebug()<<"m_resType 600_1024";
    }

    if( screen_height == 1280 )
        screen_height = 1300;
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    this->resize(screen_width, screen_height);

//    int w = 600;
//    int y = screen_height / 4 * 3;
//    ui->ProgressBar->setGeometry( (800-w)/2, y, w, 40 );
//    ui->ProgressBar->setMinimum(0);
//    ui->ProgressBar->setMaximum(0);

//    timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
//    timer->start(500);

    HideLabel();
    LoadStartImg();

    timer_showtime = new QTimer(this);
    connect(timer_showtime, SIGNAL(timeout()), this, SLOT(OnTimer_showtime()));
    timer_showtime->start(60000);

    showfacetime=new QTimer(this);
    showfacetime->setInterval(5000);
    showfacetime->setSingleShot(1);
    connect(showfacetime,SIGNAL(timeout()),this,SLOT(hide_showface()));

}

TenNiuWindow::~TenNiuWindow()
{
    delete ui;
}

void TenNiuWindow::HideLabel()
{
    ui->LabelLogo->hide();
    ui->LabelTime->hide();
    ui->LabelTime2->hide();
    ui->LabelTimeBK->hide();
    ui->LabelIpAddress->hide();
    ui->LabelVersion->hide();
    ui->LabelEnvirTemp->hide();
    ui->LabelUserImage->hide();
    ui->hideButton->setStyleSheet("QPushButton{background:yellow}");
    ui->hideButton->hide();
    ui->ProgressBar->hide();

    ui->LabelTemp->hide();
    ui->LabelFace->hide();
    ui->LabelUser->hide();
    ui->LabelUserName->hide();
    ui->LabelUserInfo->hide();
    ui->LabelBottom->hide();
    ui->statusBar->hide();
    ui->LabelStatus->hide();

    ui->LabelInfo1->hide();
    ui->LabelInfo2->hide();
    ui->LabelInfo3->hide();
    ui->LabelInfo4->hide();
    ui->LabelInfo5->hide();
    ui->LabelInfo6->hide();
    ui->LabelInfo7->hide();
    ui->LabelInfo8->hide();
}

void TenNiuWindow::ShowLabel()
{
    ui->LabelStart->hide();
    ui->LabelLogo->show();
    ui->LabelTime->show();
    ui->LabelUser->show();
    ui->LabelUserName->show();
    ui->LabelUserInfo->show();
    ui->LabelBottom->show();
    ui->LabelUserInfo->raise();
    ui->LabelLogo->raise();
    ui->LabelTime->raise();
    ui->LabelUser->raise();
    ui->LabelUserName->raise();
    ui->LabelBottom->raise();
    ui->LabelTimeBK->raise();
    ui->LabelUserImage->raise();

    ui->LabelInfo1->show();
    ui->LabelInfo2->show();
    ui->LabelInfo3->show();
    ui->LabelInfo4->show();
    ui->LabelInfo5->show();
    ui->LabelInfo6->show();
    ui->LabelInfo7->show();
}

void TenNiuWindow::LoadStartImg()
{
    qDebug()<<"show start img"<<QThread::currentThread();
    QImage image("/usrappfs/face-pic/init.png");
    ui->LabelStart->move(0,0);
    ui->LabelStart->resize(screen_width, screen_height);
    ui->LabelStart->setPixmap(QPixmap::fromImage(image));
    ui->LabelStart->setScaledContents(true);
}

void TenNiuWindow::OnTimer_showtime()
{
    ShowTime();
}

void TenNiuWindow::ShowTime()
{
    QDateTime dati = QDateTime::currentDateTime();
    QString str = dati.toString("yyyy/MM/dd hh:mm");
    ui->LabelTime->setText(str+"  ");
}

void TenNiuWindow::InitWindow()
{
    qDebug()<<"init window"<<QThread::currentThread();
    ShowLabel();

//    if( m_resType == E_RESOLUTION_800_1280 )
    {
        qDebug()<<"show label";
        // top
        int w = screen_width;
        int h = 46;
        int x = 0;
        int y = 0;
        ui->LabelLogo->move(x, y);
        ui->LabelLogo->resize(w, h);
        ui->LabelLogo->setStyleSheet("QLabel{background:rgba(194, 194, 194, 50%);color:rgb(255,255,255);}");
        ui->LabelLogo->setFont(QFont("DejaVu Sans",25,QFont::Normal));
        ui->LabelLogo->setAlignment(Qt::AlignVCenter);
        ui->LabelLogo->setText(QString::fromLocal8Bit("十牛信息幼儿园"));

        // time
        w = screen_width/2;
        h = 30;
        x = screen_width-w;
        ui->LabelTime->setAlignment(Qt::AlignRight);
        ui->LabelTime->setFont(QFont("DejaVu Sans",25,QFont::Light));
        ui->LabelTime->setStyleSheet("color:rgb(255,255,255);");
        ui->LabelTime->setGeometry(x, y+10, w, h);
        ui->LabelTime->setScaledContents(true);

        //user background
        w = screen_width - 140;
        h = 200;
        x = (screen_width-w)/2;
        y = screen_height - 100 - 120 - h;
        ui->LabelUser->move(x, y);
        ui->LabelUser->resize(w, h);
        ui->LabelUser->setStyleSheet("border-image:url(/usrappfs/face-pic/user-info.png);");
        ui->LabelUser->setScaledContents(true);

        // user name
        ui->LabelUserName->move(x+10, y+20);
        ui->LabelUserName->resize(w-20, 70);
        ui->LabelUserName->setFont(QFont("DejaVu Sans",65,QFont::Bold));
        ui->LabelUserName->setStyleSheet("color:rgb(31,229,255);");
        ui->LabelUserName->setAlignment(Qt::AlignCenter);

        // user info
        ui->LabelUserInfo->move(x+10, y+110);
        ui->LabelUserInfo->resize(w-20, 70);
        ui->LabelUserInfo->setFont(QFont("DejaVu Sans",65,QFont::Normal));
        ui->LabelUserInfo->setStyleSheet("color:rgb(31,229,255);");
        ui->LabelUserInfo->setAlignment(Qt::AlignCenter);
        //ui->LabelUserInfo->setText("三年级(2)班");

        //
        w = screen_width - 100;
        h = w;
        x = (screen_width-w)/2;
        y = screen_height - 100 - 120 - 200 - 30 - h;
        ui->LabelTimeBK->setStyleSheet("border-image:url(/usrappfs/face-pic/tenniu/user-img.png)");
        ui->LabelTimeBK->move(x, y);
        ui->LabelTimeBK->resize(w, h);
        ui->LabelTimeBK->setScaledContents(true);

        // user img
        w = 550;
        h = 550;
        x = (screen_width-w)/2;
        y = screen_height - 100 - 120 - 200 - 105 - h;
        ui->LabelUserImage->move(x, y);
        ui->LabelUserImage->resize(w, h);
        ui->LabelUserImage->setAlignment(Qt::AlignCenter);


        // bottom
        w = screen_width;
        h = 120;
        x = 0;
        y = screen_height - h;
        ui->LabelBottom->move(x, y);
        ui->LabelBottom->resize(w, h);
        ui->LabelBottom->setStyleSheet("QLabel{background:rgba(194, 194, 194, 50%);color:rgb(255,255,255);}");

        QString sInfo1 = " 同步中:0";
        w = screen_width / 4;
        h = 60;
        x = 0;
        ui->LabelInfo1->move(x, y);
        ui->LabelInfo1->resize(w, h);
        ui->LabelInfo1->setStyleSheet("color:rgb(255,255,255);");
        ui->LabelInfo1->setFont(QFont("DejaVu Sans",25,QFont::Normal));
        ui->LabelInfo1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->LabelInfo1->setText(sInfo1);

        QString sInfo2 = "已同步:0";
        x = w;
        ui->LabelInfo2->move(x, y);
        ui->LabelInfo2->resize(w, h);
        ui->LabelInfo2->setStyleSheet("color:rgb(255,255,255);");
        ui->LabelInfo2->setFont(QFont("DejaVu Sans",25,QFont::Normal));
        ui->LabelInfo2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->LabelInfo2->setText(sInfo2);

        QString sInfo4 = "版本:-";
        //x = w*3 + 50;
        x = w*2;
        ui->LabelInfo4->move(x, y);
        ui->LabelInfo4->resize(w+50, h);
        ui->LabelInfo4->setStyleSheet("color:rgb(255,255,255);");
        ui->LabelInfo4->setFont(QFont("DejaVu Sans",25,QFont::Normal));
        ui->LabelInfo4->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->LabelInfo4->setText(sInfo4);

        QString sInfo7 = "无网络连接";
        x = w*3;
        ui->LabelInfo7->move(x, y);
        ui->LabelInfo7->resize(w+150, h);
        ui->LabelInfo7->setStyleSheet("color:rgb(255,255,255);");
        ui->LabelInfo7->setFont(QFont("DejaVu Sans",25,QFont::Normal));
        ui->LabelInfo7->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        ui->LabelInfo7->setText(sInfo7);

        QString sInfo5 = " 人脸数:0";
        x = 0;
        y = y+45;
        ui->LabelInfo5->move(x, y);
        ui->LabelInfo5->resize(w, h);
        ui->LabelInfo5->setStyleSheet("color:rgb(255,255,255);");
        ui->LabelInfo5->setFont(QFont("DejaVu Sans",25,QFont::Normal));
        ui->LabelInfo5->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        ui->LabelInfo5->setText(sInfo5);

        QString sInfo6 = "用户数:0";
        x = w;
        ui->LabelInfo6->move(x, y);
        ui->LabelInfo6->resize(w, h);
        ui->LabelInfo6->setStyleSheet("color:rgb(255,255,255);");
        ui->LabelInfo6->setFont(QFont("DejaVu Sans",25,QFont::Normal));
        ui->LabelInfo6->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        ui->LabelInfo6->setText(sInfo6);

        QString sInfo3 = "设备:-";
        x = w*2;
        ui->LabelInfo3->move(x, y);
        ui->LabelInfo3->resize(w+150, h);
        ui->LabelInfo3->setStyleSheet("color:rgb(255,255,255);");
        ui->LabelInfo3->setFont(QFont("DejaVu Sans",25,QFont::Normal));
        ui->LabelInfo3->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        ui->LabelInfo3->setText(sInfo3);

        // background
        w = screen_width;
        h = screen_height;
        x = 0;
        y = 0;
        ui->LabelTime2->move(x, y);
        ui->LabelTime2->resize(w, h);
        ui->LabelTime2->setStyleSheet("border-image:url(/disk/ui_img/welcome.png)");

        ShowTime();

        qDebug()<<"---------------------------------------";
        qDebug()<<"show label ok--------------------------";
        qDebug()<<"---------------------------------------";
    }
}

void TenNiuWindow::ShowWelcome()
{
    qDebug()<<"show welcome"<<QThread::currentThread();
    ui->LabelTimeBK->hide();
    ui->LabelUserImage->hide();
//    ui->LabelTime2->hide();
    ui->LabelUser->hide();
    ui->LabelUserName->hide();
    ui->LabelUserInfo->hide();
    ui->LabelTemp->hide();

    ui->LabelTime2->show();
}

void TenNiuWindow::hidewelcome()
{
    ui->LabelTime2->hide();
}

QPixmap TenNiuWindow::PixmapToRound(QPixmap &src, int radius)
{
    qDebug()<<"show face img"<<QThread::currentThread();
    if (src.isNull()) {
        return QPixmap();
    }

    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), radius, radius);

    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

void TenNiuWindow::ShowFace(HCP_DETECT_INFO pInfo)
{
    qDebug()<<"show face data and img"<<QThread::currentThread();

    if(showfacetime->isActive())
        showfacetime->stop();
    hide_showface();

    QString userName;
    QString userInfo;

    HCP_DETECT_INFO n_di=pInfo;

    ui->LabelUserImage->show();
//    ui->LabelTime2->show();
    ui->LabelTimeBK->show();
    ui->LabelUser->show();
    ui->LabelUserName->show();
    ui->LabelUserInfo->show();

    if(n_di.iKnow == 0)
    {
        userName = "陌生人";
        userInfo = "未匹配到相似人脸";
        ui->LabelUser->setStyleSheet("border-image:url(/usrappfs/face-pic/tenniu/user-info-ex.png);");
        ui->LabelUserName->setStyleSheet("color:rgb(255,139,51);");
        ui->LabelUserName->setText( userName );
        ui->LabelUserInfo->setStyleSheet("color:rgb(255,139,51);");
        ui->LabelUserInfo->setText( userInfo );
        ui->LabelTimeBK->setStyleSheet("border-image:url(/usrappfs/face-pic/tenniu/user-img-ex.png)");
    }
    else
    {
        userName = n_di.szName;
        userInfo = n_di.szInfo;//"三年级(2)班";
        int len = userName.length();
        ui->LabelUser->setStyleSheet("border-image:url(/usrappfs/user-info.png);");
        ui->LabelUserName->setStyleSheet("color:rgb(31,229,255);");
        ui->LabelUserName->setText( userName );
        ui->LabelUserInfo->setStyleSheet("color:rgb(31,229,255);");
        ui->LabelUserInfo->setText( userInfo );
        ui->LabelTimeBK->setStyleSheet("border-image:url(/usrappfs/user-img.png)");
    }

    QPixmap pixmap_user(n_di.szPath);
    QPixmap userImg;
/*
    if( m_resType == E_RESOLUTION_800_1280 )
        userImg = PixmapToRound(pixmap_user,268);
    else if( m_resType == E_RESOLUTION_720_1280 )
        userImg = PixmapToRound(pixmap_user,237);
*/

    userImg = PixmapToRound(pixmap_user,500);

    ui->LabelUserImage->setPixmap(userImg);
    ui->LabelUserImage->setScaledContents(true);

    //识别结果图显示一会就隐藏.
    showfacetime->start();
}

void TenNiuWindow::show_MAC_V(QString mac, QString vis)
{
    ui->LabelInfo3->setText(QString::fromLocal8Bit("设备号:")+mac);
    ui->LabelInfo4->setText(QString::fromLocal8Bit("版本号:")+vis);
}

void TenNiuWindow::show_network(bool flag)
{
    if(flag)
    {
        ui->LabelInfo7->setStyleSheet("color:rgb(255,255,255)");
        ui->LabelInfo7->setText(QString::fromLocal8Bit("网络:已连接"));
    }
    else
    {
        ui->LabelInfo7->setStyleSheet("color:rgb(255,0,0)");
        ui->LabelInfo7->setText(QString::fromLocal8Bit("网络:无网络"));
    }
}

void TenNiuWindow::show_syning(int syn_ing)
{
    ui->LabelInfo1->setText(QString::fromLocal8Bit("同步中:")+QString::number(syn_ing));
}

void TenNiuWindow::show_synall(int syn_all)
{
    ui->LabelInfo2->setText(QString::fromLocal8Bit("已同步:")+QString::number(syn_all));
}

void TenNiuWindow::show_face_user(int usernum,int facenum)
{
    ui->LabelInfo5->setText(QString::fromLocal8Bit("人脸数:")+QString::number(facenum));
    ui->LabelInfo6->setText(QString::fromLocal8Bit("用户数:")+QString::number(usernum));
}

void TenNiuWindow::hide_showface()
{
    ui->LabelUserImage->hide();
//    ui->LabelTime2->hide();
    ui->LabelTimeBK->hide();
    ui->LabelUser->hide();
    ui->LabelUserName->hide();
    ui->LabelUserInfo->hide();
}
