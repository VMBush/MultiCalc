QT       += core gui
QT       += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
QMAKE_CXXFLAGS += -Wa,-mbig-obj
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
# uic -o ui_mainwindow.h ./mainwindow.ui
# uic -o ui_diagram.h ./diagram.ui
SOURCES += \
    c_funcs/calculations.c \
    c_funcs/helpers.c \
    c_funcs/validators.c \
    credit.cpp \
    deposit.cpp \
    diagram.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    c_funcs/calculations.h \
    diagram.h \
    mainwindow.h \
    qcustomplot.h \
    smartcalc.h

FORMS += \
    diagram.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
