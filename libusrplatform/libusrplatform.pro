QT       +=  gui

TEMPLATE = lib
DEFINES += LIBUSRPLATFORM_LIBRARY

CONFIG += C++ 11

QT += core widgets network

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../blackboxcp/blackboxcp/usrplatform/hisi/hcm_usrplatform/hcm_usrplatform_lib.cpp \
    global_class/Global_Class.cpp \
    http_ser/http_scan.cpp \
    http_ser/http_usr.cpp \
    libusrplatform.cpp \
    ooo_ui/ooo_ui.cpp \

HEADERS += \
    ../blackboxcp/blackboxcp/usrplatform/hisi/com_inc/hciConfig.h \
    ../blackboxcp/blackboxcp/usrplatform/hisi/com_inc/hcmComInfo.h \
    ../blackboxcp/blackboxcp/usrplatform/hisi/hci_inc/hci_def.h \
    ../blackboxcp/blackboxcp/usrplatform/hisi/hci_inc/hci_intf.h \
    ../blackboxcp/blackboxcp/usrplatform/hisi/hci_inc/hci_struct.h \
    ../blackboxcp/blackboxcp/usrplatform/ui/ui_msg.h \
    global_class/Global_Class.h \
    http_ser/http_scan.h \
    http_ser/http_usr.h \
    libusrplatform.h \
    ooo_ui/ooo_ui.h \

INCLUDEPATH += \
    ../blackboxcp/blackboxcp/usrplatform/hisi/hci_inc/ \
    ../blackboxcp/blackboxcp/usrplatform/hisi/com_inc/ \



# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    ooo_ui/ooo_ui.ui
