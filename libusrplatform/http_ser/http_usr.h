#ifndef HTTP_USR_H
#define HTTP_USR_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include <QThread>

#include <global_class/Global_Class.h>


#define save_path    "/home/libusrplatform/face_pic"  // 正如其名

class http_usr : public QObject
{
    Q_OBJECT
public:
    http_usr(QObject *parent = nullptr);
    ~http_usr();

    void choreme_view(QString msg);   // 接收浏览器请求
    void Parsing_GET_request(QStringList &msg);  // 处理GET请求
    void Parsing_POST_request(QStringList &msg); // 处理POST请求

    Global_Class *glo; // 用来保存全局类的地址

public slots:
    void init(void); // 把初始化丢到这里让定时器去执行
    void new_client_connect(void); // 有新用户链接
    void new_client_message(void); //　接收到新消息

signals:
    void emit_err_string(QString);   //　发送错误给界面类
    void emit_client(QString);   // 发送新用户ip给界面类
    void emit_info(QString,QString); // 发送识别的设备id和时间

    void emit_scan_pic(QString); // 发送图片文件名，让http_scan去识别

private:
    QTcpServer * ser;
    QTcpSocket * client;
    QPixmap *pic;

    QByteArray data;    // 用来保存图片数据的缓存区
    QString file_name;   // 保存识别的图片名称
    int file_size;   // 将下载的大小

};

#endif // HTTP_USR_H
