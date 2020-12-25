#ifndef GLOBAL_CLASS_H
#define GLOBAL_CLASS_H

#include <httpurl.h>
#include <ui_msg.h>

/* 公共类 */
class Global_class:public QObject
{
    Q_OBJECT

public:
    Global_class();
    ~Global_class();

signals:
    /* 提供给人脸识别返回函数发送信号，关联记录表的写入
     * FACE_DATA id 阈值等信息
     * HCP_DETECT_INFO 识别回来的名字，班级图片路径等信息
     */
    void haveface(FACE_DATA,HCP_DETECT_INFO);

    /* 退出信号 */
    void appquit();

    /* 临时，在界面显示耗时 */
    void send_tt(int,int,int,int);

public:
    /* 获得本类指针 */
    static Global_class* GetInstance();

    /* 发送有识别时的信号 */
    void emit_haveface();

    /* 发送有识别时的信号 */
    void emit_haveface_in(FACE_DATA ifd,HCP_DETECT_INFO ihdi);

    /* 发送退出信号 */
    void emit_appquit();

    /* 临时，在界面显示耗时 */
    void emit_send_tt(int tt,int wet);

public:
    static Global_class* instance;
    /* 主程序函数指针 */
    HCI_FUNC *g_HciFunc;
    /* 主程序识别结果数据,用于记录写入使用 */
    FACE_DATA g_fb;
    /* 给界面显示时的数据 */
    HCP_DETECT_INFO g_uidata;

    QMutex mutexall;
};

#endif // GLOBAL_CLASS_H
