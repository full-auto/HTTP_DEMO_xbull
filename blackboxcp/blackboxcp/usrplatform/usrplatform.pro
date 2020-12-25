#-------------------------------------------------
#
# Project created by QtCreator 2020-08-29T14:39:43
#
#-------------------------------------------------

QT       += widgets network sql core gui multimedia

TARGET = usrplatform
TEMPLATE = lib

DEFINES += USRPLATFORM_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        usrplatform.cpp \
    hisi/hcm_usrplatform/hcm_usrplatform_lib.cpp \
    hisi/global_class.cpp \
    http/httpreplay/httpfacegateswipecard.cpp \
    http/httpreplay/httpgetload.cpp \
    http/httpreplay/httpgetservertimet.cpp \
    http/httpreplay/httpgettoken.cpp \
    http/httpreplay/httpheartbeat.cpp \
    http/httpreplay/httpperson.cpp \
    http/httpreplay/httpschooluseritems.cpp \
    http/httpreplay/httpselfstudyfacedevice.cpp \
    http/httpreplay/httpselfstudyfacegateconfig.cpp \
    http/httpreplay/httptaskrelease.cpp \
    http/httpreplay/httptaskreserve.cpp \
    http/httpfacerecordupdata.cpp \
    http/httptask.cpp \
    http/httptaskpolling.cpp \
    json/json_reader.cpp \
    json/json_value.cpp \
    json/json_valueiterator.inl \
    json/json_writer.cpp \
    ui/tenniuwindow.cpp \
    http/httpreplay/httpincrementuseritems.cpp \
    http/httpreplay/httpitemsbyid.cpp \
    http/httpreplay/httpupdatainfo.cpp \
    myui/mywindow.cpp \
    http/httpreplay/httpreportstatus.cpp \
    http/httpreplay/httpcorrection.cpp \
    http/httpreplay/httpschoolleaveuser.cpp \
    http/httpreplay/httptemperature.cpp \
    http/httpreplay/httpswipecard.cpp \
    http/httptastreturn.cpp

HEADERS += \
        usrplatform.h \
        usrplatform_global.h \ 
    hisi/com_inc/hcmComInfo.h \
    hisi/hci_inc/hci_def.h \
    hisi/hci_inc/hci_intf.h \
    hisi/hci_inc/hci_struct.h \
    hisi/global_class.h \
    http/httpreplay/httpfacegateswipecard.h \
    http/httpreplay/httpgetload.h \
    http/httpreplay/httpgetservertimet.h \
    http/httpreplay/httpgettoken.h \
    http/httpreplay/httpheartbeat.h \
    http/httpreplay/httpperson.h \
    http/httpreplay/httpschooluseritems.h \
    http/httpreplay/httpselfstudyfacedevice.h \
    http/httpreplay/httpselfstudyfacegateconfig.h \
    http/httpreplay/httptaskrelease.h \
    http/httpreplay/httptaskreserve.h \
    http/httpreplay/httpurl.h \
    http/httpfacerecordupdata.h \
    http/httptask.h \
    http/httptaskpolling.h \
    json/allocator.h \
    json/assertions.h \
    json/config.h \
    json/forwards.h \
    json/json.h \
    json/json_features.h \
    json/json_tool.h \
    json/reader.h \
    json/value.h \
    json/version.h \
    json/writer.h \
    ui/tenniuwindow.h \
    ui/ui_mainwindow.h \
    ui/ui_msg.h \
    http/httpreplay/httpincrementuseritems.h \
    http/httpreplay/httpitemsbyid.h \
    http/httpreplay/httpupdatainfo.h \
    myui/mywindow.h \
    http/httpreplay/httpreportstatus.h \
    http/httpreplay/httpcorrection.h \
    http/httpreplay/httpschoolleaveuser.h \
    http/httpreplay/httptemperature.h \
    http/httpreplay/httpswipecard.h \
    http/httptastreturn.h

INCLUDEPATH += \
        ui \
        hisi \
        hisi/com_inc \
        hisi/hci_inc \
        http \
        http/httpreplay \
        json

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix:!macx: LIBS += -L$$PWD/../../../../../opt/hisi-aliyun-oss/openssl/lib/ -lcrypto

INCLUDEPATH += $$PWD/../../../../../opt/hisi-aliyun-oss/openssl/include
DEPENDPATH += $$PWD/../../../../../opt/hisi-aliyun-oss/openssl/include

unix:!macx: LIBS += -L$$PWD/../../../../../opt/hisi-aliyun-oss/openssl/lib/ -lssl

INCLUDEPATH += $$PWD/../../../../../opt/hisi-aliyun-oss/openssl/include
DEPENDPATH += $$PWD/../../../../../opt/hisi-aliyun-oss/openssl/include

unix:!macx: LIBS += -L$$PWD/../../../../../opt/hisi-aliyun-oss/curl/lib/ -lcurl

INCLUDEPATH += $$PWD/../../../../../opt/hisi-aliyun-oss/curl/include
DEPENDPATH += $$PWD/../../../../../opt/hisi-aliyun-oss/curl/include

unix:!macx: LIBS += -L$$PWD/../../../../../opt/hisi-aliyun-oss/oss/lib/ -lalibabacloud-oss-cpp-sdk

INCLUDEPATH += $$PWD/../../../../../opt/hisi-aliyun-oss/oss/include
DEPENDPATH += $$PWD/../../../../../opt/hisi-aliyun-oss/oss/include



FORMS += \
    myui/mywindow.ui

unix:!macx: LIBS += -L$$PWD/../../../../../opt/hisi200/apr/lib/ -lapr-1

INCLUDEPATH += $$PWD/../../../../../opt/hisi200/apr/include
DEPENDPATH += $$PWD/../../../../../opt/hisi200/apr/include
INCLUDEPATH += $$PWD/../../../../../opt/hisi200/apr/include/apr-1

unix:!macx: LIBS += -L$$PWD/../../../../../opt/hisi200/apr-util/lib/ -laprutil-1

INCLUDEPATH += $$PWD/../../../../../opt/hisi200/apr-util/include
DEPENDPATH += $$PWD/../../../../../opt/hisi200/apr-util/include
INCLUDEPATH += $$PWD/../../../../../opt/hisi200/apr-util/include/apr-1

unix:!macx: LIBS += -L$$PWD/../../../../../opt/hisi200/aliyun-log-sls/lib/ -llog_c_sdk

INCLUDEPATH += $$PWD/../../../../../opt/hisi200/aliyun-log-sls/include
DEPENDPATH += $$PWD/../../../../../opt/hisi200/aliyun-log-sls/include


#QMAKE_CXXFLAGS += -g
