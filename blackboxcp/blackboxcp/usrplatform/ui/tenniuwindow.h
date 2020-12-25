#ifndef TENNIUWINDOW_H
#define TENNIUWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include "ui_msg.h"

#include <QEventLoop>

namespace Ui {
class MainWindow;
}

class TenNiuWindow :public QMainWindow
{
    Q_OBJECT

public:
    explicit TenNiuWindow(QWidget *parent = 0);
    ~TenNiuWindow();
    void InitWindow();
    void ShowWelcome();
    void hidewelcome();

public slots:
    /* 显示识别人脸截图 */
    void ShowFace(HCP_DETECT_INFO pInfo);
    /* 显示设备MAC和版本号 */
    void show_MAC_V(QString mac,QString vis);
    /* 显示网络状态 */
    void show_network(bool flag);
    /* 显示同步中 */
    void show_syning(int syn_ing);
    /* 显示已同步 */
    void show_synall(int syn_all);
    /* 显示人脸数和用户数 */
    void show_face_user(int usernum,int facenum);

    /* 隐藏识别人脸的图片以及数据，用于应对连续识别显示卡顿的情况 */
    void hide_showface();

private:
    void LoadStartImg();
    void HideLabel();
    void ShowLabel();
    void ShowTime();
	QPixmap PixmapToRound(QPixmap &src, int radius);
    QTimer *timer_showtime;
    QTimer *showfacetime;

public slots:
    void OnTimer_showtime();

private:
	
	Ui::MainWindow *ui;
	int screen_width;
	int screen_height;
	HCP_PLAT_TYPE_E m_platType;
	HCP_RESOLUTION_E m_resType;

};

#endif // TENNIUWINDOW_H
