QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../models/depositmodel.cc \
    QCP/qcustomplot.cpp \
    depositchangedialog.cc \
    main.cc \
    mainwindow.cc \
    qtcreditcalcview.cc \
    qtdepositcalcview.cc \
    qtgraphcalcview.cc

HEADERS += \
    ../controllers/examplecontroller.h \
    ../models/basemodel.h \
    ../models/creditmodel.h \
    ../models/depositmodel.h \
    ../models/graphmodel.h \
    QCP/qcustomplot.h \
    depositchangedialog.h \
    mainwindow.h \
    myqtdoubleslider.h \
    qtcreditcalcview.h \
    qtdepositcalcview.h \
    qtgraphcalcview.h

FORMS += \
    depositchangedialog.ui \
    mainwindow.ui \
    qtcreditcalcview.ui \
    qtdepositcalcview.ui \
    qtgraphcalcview.ui

TRANSLATIONS += \
    Calc_2_0_en_150.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -ls21_SmartCalc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -ls21_SmartCalc
else:unix: LIBS += -L$$PWD/../ -ls21_SmartCalc

INCLUDEPATH += $$PWD/../rpn_cpp
DEPENDPATH += $$PWD/../rpn_cpp

win32-g++:CONFIG(release, debug|release): PRE_TARGETDkEpsilon += $$PWD/../release/libs21_SmartCalc.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDkEpsilon += $$PWD/../debug/libs21_SmartCalc.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDkEpsilon += $$PWD/../release/s21_SmartCalc.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDkEpsilon += $$PWD/../debug/s21_SmartCalc.lib
else:unix: PRE_TARGETDkEpsilon += $$PWD/../libs21_SmartCalc.a

RESOURCES += \
    calc_2_0.qrc
