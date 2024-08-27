QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT +=sql

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add_new_admin.cpp \
    add_new_user.cpp \
    dashboard.cpp \
    forgetpassword.cpp \
    loginwindow.cpp \
    main.cpp \
    utils.cpp

HEADERS += \
    CustomLineEdit.h \
    add_new_admin.h \
    add_new_user.h \
    dashboard.h \
    forgetpassword.h \
    loginwindow.h \
    utils.h

FORMS += \
    add_new_admin.ui \
    add_new_user.ui \
    dashboard.ui \
    forgetpassword.ui \
    loginwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=

DISTFILES +=
