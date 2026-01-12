QT       += core gui
QT +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    balancewindow.cpp \
    datamanager.cpp \
    inactivitytimer.cpp \
    environment.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    startwindow.cpp \
    windowmanager.cpp\
    transactionwindow.cpp\
    withdrawwindow.cpp\
    withdrawend.cpp\
    withdrawothersum.cpp

HEADERS += \
    balancewindow.h \
    customlineedit.h \
    inactivitytimer.h \
    environment.h \
    login.h \
    mainwindow.h \
    startwindow.h \
    datamanager.h \
    windowmanager.h\
    transactionwindow.h\
    withdrawend.h\
    withdrawwindow.h\
    withdrawothersum.h


FORMS += \
    balancewindow.ui \
    login.ui \
    mainwindow.ui \
    startwindow.ui \
    transactionwindow.ui\
    withdrawwindow.ui\
    withdrawend.ui\
    withdrawothersum.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
