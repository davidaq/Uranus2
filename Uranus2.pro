#-------------------------------------------------
#
# Project created by QtCreator 2012-08-06T18:42:37
#
#-------------------------------------------------

QT       += core gui

TARGET = Uranus2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    console.cpp \
    cfg.cpp \
    mainwindow_benchMenuActions.cpp \
    configdlg.cpp \
    udocument.cpp \
    ualgorithmeditor.cpp \
    funclistitem.cpp \
    UAlgTags/ufunctiontag.cpp \
    UAlgTags/uarghook.cpp \
    UAlgTags/ucontainertag.cpp \
    UAlgTags/ucommenttag.cpp

HEADERS  += mainwindow.h \
    console.h \
    cfg.h \
    configdlg.h \
    udocument.h \
    ualgorithmeditor.h \
    funclistitem.h \
    UAlgTags/ualgtag.h \
    UAlgTags/ufunctiontag.h \
    UAlgTags/uarghook.h \
    UAlgTags/ucontainertag.h \
    UAlgTags/ucommenttag.h

FORMS    += mainwindow.ui \
    console.ui \
    configdlg.ui \
    ualgorithmeditor.ui

RESOURCES += \
    res.qrc
