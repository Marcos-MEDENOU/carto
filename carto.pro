QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Cette ligne permet d'inclure le module de base de données
QT += sql

#module maps


SOURCES += \
    Equipe.cpp \
    PointRepere.cpp \
    main.cpp \
    mainwindow.cpp \
    PointRepere.cpp \
    Equipe.cpp

HEADERS += \
    Equipe.h \
    PointRepere.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    carto_fr_BJ.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
