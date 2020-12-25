#include "ooo_ui.h"
#include "ui_ooo_ui.h"

ooo_ui::ooo_ui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ooo_ui)
{
    ui->setupUi(this);
    QWSServer::setCursorVisible(false);

    // 设置tr函数，本地QString 对本地文件系统读写时候的默认编码格式为utf-8   统一编码
    QTextCodec* locale_codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(locale_codec);
    QTextCodec::setCodecForCStrings(locale_codec);
    QTextCodec::setCodecForTr(locale_codec);

    // 设置显示识别的用户名的框框
    ui->label_usr->hide();
    QPalette palette_bjtm=ui->horizontalFrame->palette();
    palette_bjtm.setBrush(QPalette::Window,QColor(0,0,0,0));
    ui->horizontalFrame->setPalette(palette_bjtm);

    // 用定时器实时刷新时间
    QObject::connect(&tim_date,SIGNAL(timeout()),this,SLOT(current_time()));
    tim_date.start(1000); // 1000ms 刷新一次时间

    //透明
    setAutoFillBackground(false);  //这个不设置的话就背景变黑
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);

}

ooo_ui::~ooo_ui()
{
    delete ui;
}


/*******************************************************************************
 * 函数功能：定时器触发的擦函数，实时更新界面的时间信息
 * 参数说明：
 *   空
 * 返回值：
 * 　空
 * ******************************************************************************/
void ooo_ui::current_time(void)
{
    //当前时间
    QDateTime dati  = QDateTime::currentDateTime();
    QString str     = dati.toString("yyyy/MM/dd hh:mm:ss");
    ui->label_current_time->setText(str+" ");
}


/*******************************************************************************
 * 函数功能：把传进来的字符串显示到ui的textBrowser文本展示框上
 * 参数说明：
 *   空
 * ******************************************************************************/
void ooo_ui::set_text(QString msg)
{
    ui->textBrowser->append(msg);
}


/*******************************************************************************
 * 函数功能：控制左侧两个标签显示的内容
 * 参数说明：
 *   int flag　：1是下面的，2是上面的
 * 　QString msg　：　需要显示的内容
 * ******************************************************************************/
void ooo_ui::set_label(int flag,QString msg)
{
    if(flag == 1)
       ui->label->setText(msg);
    else
        ui->label_2->setText(msg);
}


/*******************************************************************************
 * 函数功能：显示摄像头发来的数据
 * 参数说明：
 *   QString id　：设备ID
 * 　QString date　：　识别日期
 * ******************************************************************************/
void ooo_ui::set_info(QString id,QString date)
{
    ui->label_id->setText(id);
    ui->label_time->setText(date);
}


/*******************************************************************************
 * 函数功能：显示链接的用户
 * 参数说明：
 *   QString ip　：用户ip
 * ******************************************************************************/
void ooo_ui::set_title(QString ip)
{

    ui->label_client->setText(QString("Client  [%1]").arg(ip) );
}


/*******************************************************************************
 * 函数功能：显示识别到的用户
 * 参数说明：
 *   QString name　：用户name
 * ******************************************************************************/
void ooo_ui::show_usr(QString name)
{
    ui->label_usr->show();
    ui->label_usr->setText(name + "OK");
}


/*******************************************************************************
 * 函数功能：真正显示图片的在这儿！
 * 参数说明：
 *   QString file_name　：需要显示的图片名(绝对路径)
 * ******************************************************************************/
void ooo_ui::show_pic(QString file_name)
{
#ifndef BUG_ooo
    qDebug() << err_func_show << "***所有步骤完成！显示识别到的图片**";
    qDebug() << "***********************************************************************************************************";
#endif

    QPixmap pic;
    // 使用QPixmap
    pic.load(file_name);
    pic = pic.scaled(ui->label_5->size());
    ui->label_5->setPixmap(pic);
    ui->textBrowser->append("显示完成!");
    ui->label_2->setText("芜湖！666");
}

