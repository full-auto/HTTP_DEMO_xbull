#ifndef GLOBAL_CLASS_H
#define GLOBAL_CLASS_H

#include <hci_intf.h>
#include <QObject>
#include <QMutex>
#include <QDebug>
#include <QList>
#include <QPixmap>
#include <QImage>
#include <QThread>
#include <QTimer>


#define err_func_show    QThread::currentThread() << __FUNCTION__<<__LINE__<<": "

//#define BUG_ooo

class Global_Class : public QObject
{
    Q_OBJECT

public:
    Global_Class();
    ~Global_Class();

    // 静态全局类初始化。其他文件的类不需要new, 只需要调用这个函数接口即可获取同一个全局类
    static Global_Class* GetGlobal_class();

public:
    /* 主程序的函数指针 通过xx_init的参数来进行调用 */
    HCI_FUNC *g_HciFunc;

    /*给HTTP_scan线程去轮询的缓冲图片链表*/
    QList<QByteArray> pic_list;

    // 本类的指针，用来给其他头文件引用
    static Global_Class *instance;

};




#endif // GLOBAL_CLASS_H
