#-------------------------------------------------
#
# Project created by QtCreator 2015-05-20T09:23:49
#
#-------------------------------------------------

QT       += core gui quick quickwidgets enginio

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DWEngineLight
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hitrolldialog.cpp \
    armourbypassdialog.cpp \
    combatroundwidget.cpp \
    combatroundcontroller.cpp \
    combatroundmodel.cpp

HEADERS  += mainwindow.h \
    player.h \
    hitrolldialog.h \
    armourbypassdialog.h \
    combatroundwidget.h \
    combatroundcontroller.h \
    combatroundmodel.h

FORMS    += mainwindow.ui \
    hitrolldialog.ui \
    armourbypassdialog.ui \
    combatroundwidget.ui

DISTFILES += \
    CombatRoundQuickWidget.qml

RESOURCES += \
    ressources.qrc
