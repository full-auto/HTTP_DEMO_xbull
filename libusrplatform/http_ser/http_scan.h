#ifndef HTTP_SCAN_H
#define HTTP_SCAN_H

#include <global_class/Global_Class.h>
#include <hcmComInfo.h>

// 让本类在线程执行
class http_scan : public QObject
{
    Q_OBJECT

public:
    http_scan();
    ~http_scan();

    // 全局类的指针
    Global_Class *glo = NULL;

    // 轮询全局图片列表，有就去识别，一次提取一张。没有则return
    QTimer tim_pic_scan;

public slots:
    // 把初始化丢到这里让定时器去执行
    void init(void);

    // 用来轮询的槽函数
    void pic_scaning(void);

signals:
    // 发送识别的文本结果给ui
    void emit_usr(QString);

    // 发送识别完成的图片，让ui显示
    void emit_show_pic(QString);
};

#endif // HTTP_SCAN_H
