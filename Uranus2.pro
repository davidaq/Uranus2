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
    UAlgTags/ucommenttag.cpp \
    UAlgTags/ustatictag.cpp \
    UAlgTags/ualgtag.cpp \
    UAlgTags/ucalltag.cpp \
    UAlgTags/uassigntag.cpp \
    UAlgTags/ulooptag.cpp \
    UAlgTags/utraversetag.cpp \
    UAlgTags/uconditiontag.cpp \
    UAlgTags/ubranchtag.cpp \
    UAlgTags/ulisttag.cpp \
    UAlgTags/ulisttagitem.cpp \
    UAlgTags/ucalltagselectdlg.cpp

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
    UAlgTags/ucommenttag.h \
    UAlgTags/ustatictag.h \
    UAlgTags/ucalltag.h \
    UAlgTags/uassigntag.h \
    UAlgTags/ulooptag.h \
    UAlgTags/utraversetag.h \
    UAlgTags/uconditiontag.h \
    UAlgTags/ubranchtag.h \
    UAlgTags/ulisttag.h \
    UAlgTags/ulisttagitem.h \
    UAlgTags/ucalltagselectdlg.h

FORMS    += mainwindow.ui \
    console.ui \
    configdlg.ui \
    ualgorithmeditor.ui \
    UAlgTags/ucalltagselectdlg.ui

RESOURCES += \
    res.qrc
