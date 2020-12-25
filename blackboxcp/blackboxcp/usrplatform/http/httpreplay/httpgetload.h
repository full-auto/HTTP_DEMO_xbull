#ifndef HTTPGETLOAD_H
#define HTTPGETLOAD_H
#include <httpurl.h>

/* 单独的下载线程类(get)，创建并赋值url后，执行start()在子线程开始下载 */
/* 也可直接调用run()，但此时不会创建子线程，只会在调用函数所在的线程执行 */
/* 主要在人脸下载时，如果每个都开线程，以开发板的硬件大概率卡死，此处将是下载完一个用户图片，进行录入，再进行下一步的返回数据处理 */
/* 当前未作任何错误处理 */
/*
 * 在使用新建线程时绑定该信号，线程结束时由QT释放资源
 * connect(HttpGetLoad,SIGNAL(finished()),HttpGetLoad,SLOT(deleteLater()));
*/
/* 这个主要用于下载人脸图片 */
class HttpGetLoad:public QThread
{
    Q_OBJECT

public:
    HttpGetLoad();
    ~HttpGetLoad();

private slots:
    void tDownloadProgress(qint64 bytesSent, qint64 bytesTotal);/* 下载进度 当前字节数 总字节数 */

//private:
public:
    virtual void run();/* 任务处理线程 */

public:
    QString urladdr;/* 下载链接 */
    QString imgnamepath;/* 下载后的人脸图片路径，用于在开发板注册人脸 */
    bool load_flag;/* 下载结果，0失败 1成功 */
};

#endif // HTTPGETLOAD_H
