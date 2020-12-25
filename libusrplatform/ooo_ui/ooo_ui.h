#ifndef OOO_UI_H
#define OOO_UI_H

#include <QMainWindow>
#include <QWSServer>
#include <QSize>
#include <QTextCodec>
#include <QDebug>
#include <QFile>
#include <stdlib.h>

#include <global_class/Global_Class.h>  // 全局类


namespace Ui {
class ooo_ui;
}

class ooo_ui : public QMainWindow
{
    Q_OBJECT

public:
    explicit ooo_ui(QWidget *parent = nullptr);
    ~ooo_ui();

    QTimer tim_date; // 用定时器来更新时间
    Global_Class *glo = NULL; // 保存全局静态类

signals :
    //void emit_scan_pic(void); // 发送信号，让http_scan去识别


public slots:
    // 写入textBrower的槽函数
    void set_text(QString msg);

    // 设置两个标签，flag =1 为老铁没毛病，flag=2 是奥利给
    void set_label(int flag,QString msg);

    // 显示id 和日期
    void set_info(QString id,QString date);

    // 显示实时时间
    void current_time(void);

    // 显示客户ip
    void set_title(QString ip);

    // 显示识别到的人脸信息
    void show_usr(QString name);

    // 真正显示图片的在这儿！
    void show_pic(QString file_name);

private:
    Ui::ooo_ui *ui;
};

#endif // OOO_UI_H
