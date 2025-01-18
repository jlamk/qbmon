QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
VERSION = 1.1.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    dialogmonitors.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    about.h \
    dialogmonitors.h \
    mainwindow.h

FORMS += \
    about.ui \
    dialogmonitors.ui \
    mainwindow.ui

TRANSLATIONS += \
    qbmon_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -lXrandr
LIBS += -lX11

RESOURCES += \
    res.qrc
