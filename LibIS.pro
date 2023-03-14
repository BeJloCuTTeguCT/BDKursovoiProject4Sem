QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/authorization.cpp \
    Sources/editrow.cpp \
    Sources/main.cpp \
    Sources/mfiltermodel.cpp \
    Sources/search.cpp \
    Sources/setconnectdb.cpp \
    Sources/stylesheet.cpp \
    Sources/widget.cpp

HEADERS += \
    Include/authorization.h \
    Include/editrow.h \
    Include/mfiltermodel.h \
    Include/myenums.h \
    Include/search.h \
    Include/setconnectdb.h \
    Include/stylesheet.h \
    Include/widget.h

FORMS += \
    Forms/authorization.ui \
    Forms/editrow.ui \
#    Forms/mywidget.ui \
    Forms/search.ui \
    Forms/setconnectdb.ui \
    Forms/simple_search.ui \
    Forms/widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/resources.qrc

INCLUDEPATH += .\Include/
