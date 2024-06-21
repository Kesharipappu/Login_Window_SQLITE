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
    main.cpp \
    mainwindow.cpp \
    newuser.cpp \
    reset_password.cpp

HEADERS += \
    add_new_admin.h \
    add_new_user.h \
    dashboard.h \
    mainwindow.h \
    newuser.h \
    reset_password.h

FORMS += \
    add_new_admin.ui \
    add_new_user.ui \
    dashboard.ui \
    mainwindow.ui \
    newuser.ui \
    reset_password.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
