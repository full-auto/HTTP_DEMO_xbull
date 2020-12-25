/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include <QDebug>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *LabelStart;
    QPushButton *hideButton;
    QLabel *LabelLogo;
    QLabel *LabelTemp;
    QLabel *LabelFace;
    QLabel *LabelUser;
    QLabel *LabelUserImage;
    QLabel *LabelUserName;
    QLabel *LabelUserInfo;
    QLabel *LabelIpAddress;
    QLabel *LabelVersion;
    QLabel *LabelTime;
    QLabel *LabelTime2;
    QLabel *LabelTimeBK;
    QLabel *LabelBottom;
    QLabel *LabelEnvirTemp;
    QLabel *LabelStatus;
    QProgressBar *ProgressBar;
    QStatusBar *statusBar;

    QLabel *LabelInfo1;
    QLabel *LabelInfo2;
    QLabel *LabelInfo3;
    QLabel *LabelInfo4;
    QLabel *LabelInfo5;
    QLabel *LabelInfo6;
    QLabel *LabelInfo7;
    QLabel *LabelInfo8;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->setEnabled(true);
        MainWindow->resize(750, 1224);
        MainWindow->setAutoFillBackground(true);
        MainWindow->setAnimated(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        LabelStart = new QLabel(centralWidget);
        LabelStart->setObjectName(QString::fromUtf8("LabelStart"));
        LabelStart->setGeometry(QRect(90, 730, 121, 71));
        hideButton = new QPushButton(centralWidget);
        hideButton->setObjectName(QString::fromUtf8("hideButton"));
        hideButton->setGeometry(QRect(340, 710, 271, 41));
        LabelTemp = new QLabel(centralWidget);
        LabelTemp->setObjectName(QString::fromUtf8("LabelTemp"));
        LabelTemp->setGeometry(QRect(200, 90, 241, 71));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial Black"));
        font.setPointSize(36);
        font.setBold(true);
        font.setWeight(75);
        LabelLogo = new QLabel(centralWidget);
        LabelLogo->setObjectName(QString::fromUtf8("LabelLogo"));
        LabelLogo->setGeometry(QRect(200, 0, 200, 40));
        LabelTemp->setFont(font);
        LabelFace = new QLabel(centralWidget);
        LabelFace->setObjectName(QString::fromUtf8("LabelFace"));
        LabelFace->setGeometry(QRect(140, 110, 431, 341));
        LabelUser = new QLabel(centralWidget);
        LabelUser->setObjectName(QString::fromUtf8("LabelUser"));
        LabelUser->setGeometry(QRect(180, 790, 221, 81));
        LabelUserImage = new QLabel(centralWidget);
        LabelUserImage->setObjectName(QString::fromUtf8("LabelUserImage"));
        LabelUserImage->setGeometry(QRect(610, 970, 101, 81));
        LabelUserName = new QLabel(centralWidget);
        LabelUserName->setObjectName(QString::fromUtf8("LabelUserName"));
        LabelUserName->setGeometry(QRect(90, 960, 151, 31));
        LabelUserInfo = new QLabel(centralWidget);
        LabelUserInfo->setObjectName(QString::fromUtf8("LabelUserInfo"));
        LabelUserInfo->setGeometry(QRect(90, 1010, 141, 31));
        LabelIpAddress = new QLabel(centralWidget);
        LabelIpAddress->setObjectName(QString::fromUtf8("LabelIpAddress"));
        LabelIpAddress->setGeometry(QRect(630, 1090, 161, 51));
        LabelVersion = new QLabel(centralWidget);
        LabelVersion->setObjectName(QString::fromUtf8("LabelVersion"));
        LabelVersion->setGeometry(QRect(30, 1090, 161, 51));
        LabelTime = new QLabel(centralWidget);
        LabelTime->setObjectName(QString::fromUtf8("LabelTime"));
        LabelTime->setGeometry(QRect(220, 590, 121, 71));
        LabelTime2 = new QLabel(centralWidget);
        LabelTime2->setObjectName(QString::fromUtf8("LabelTime2"));
        LabelTime2->setGeometry(QRect(220, 590, 121, 71));
        LabelTimeBK = new QLabel(centralWidget);
        LabelTimeBK->setObjectName(QString::fromUtf8("LabelTimeBK"));
        LabelTimeBK->setGeometry(QRect(220, 590, 121, 71));
        LabelBottom = new QLabel(centralWidget);
        LabelBottom->setObjectName(QString::fromUtf8("LabelBottom"));
        LabelBottom->setGeometry(QRect(260, 800, 121, 71));
        LabelEnvirTemp = new QLabel(centralWidget);
        LabelEnvirTemp->setObjectName(QString::fromUtf8("LabelEnvirTemp"));
        LabelEnvirTemp->setGeometry(QRect(400, 840, 161, 51));
        LabelStatus = new QLabel(centralWidget);
        LabelStatus->setObjectName(QString::fromUtf8("LabelStatus"));
        LabelStatus->setGeometry(QRect(220, 590, 121, 71));
        ProgressBar = new QProgressBar(centralWidget);
        ProgressBar->setObjectName(QString::fromUtf8("ProgressBar"));
        ProgressBar->setGeometry(QRect(130, 440, 401, 16));
        ProgressBar->setValue(24);
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);
        LabelInfo1 = new QLabel(centralWidget);
        LabelInfo2 = new QLabel(centralWidget);
        LabelInfo3 = new QLabel(centralWidget);
        LabelInfo4 = new QLabel(centralWidget);
        LabelInfo5 = new QLabel(centralWidget);
        LabelInfo6 = new QLabel(centralWidget);
        LabelInfo7 = new QLabel(centralWidget);
        LabelInfo8 = new QLabel(centralWidget);
        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        MainWindow->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        LabelStart->setText(QApplication::translate("MainWindow", "Label-Start", 0, QApplication::UnicodeUTF8));
        hideButton->setText(QApplication::translate("MainWindow", "PushButton", 0, QApplication::UnicodeUTF8));
        LabelTemp->setText(QString());
        LabelFace->setText(QString());
        //LabelFace->setText(QApplication::translate("MainWindow", "Face", 0, QApplication::UnicodeUTF8));
        LabelUser->setText(QString());
        LabelUserImage->setText(QApplication::translate("MainWindow", "image", 0, QApplication::UnicodeUTF8));
        LabelUserName->setText(QApplication::translate("MainWindow", "Label-user", 0, QApplication::UnicodeUTF8));
        LabelUserInfo->setText(QString());
        LabelIpAddress->setText(QApplication::translate("MainWindow", "ip", 0, QApplication::UnicodeUTF8));
        LabelVersion->setText(QApplication::translate("MainWindow", "ver", 0, QApplication::UnicodeUTF8));
        LabelTime->setText(QApplication::translate("MainWindow", "", 0, QApplication::UnicodeUTF8));
        LabelTime2->setText(QApplication::translate("MainWindow", "", 0, QApplication::UnicodeUTF8));
        LabelBottom->setText(QString());
        LabelEnvirTemp->setText(QApplication::translate("MainWindow", "envir", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
