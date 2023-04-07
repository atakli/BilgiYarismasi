HEADERS   =  \
            mainwindow.h \
            qaform.h

RESOURCES = \
    resources.qrc

SOURCES   =  \
            main.cpp \
            mainwindow.cpp \
            qaform.cpp

unix:QMAKE_CXXFLAGS += -std=c++2a
win32:CONFIG += c++2b

CONFIG += console

# windeployqt.exe bilgiyarismasi.exe -no-translations --no-opengl-sw --no-system-d3d-compiler

win32{
INCLUDEPATH += "C:\\Users\\Emre ATAKLI\\Documents\\qt\\UpdateController\\include"
CONFIG(release, debug|release): LIBS += -L"C:\\Users\\Emre ATAKLI\\Documents\\qt\\UpdateController\\build-UpdateController-Desktop_Qt_6_4_1_MSVC2019_64bit-Release\\release\\static" -lupdatecontroller
else:CONFIG(debug, debug|release): LIBS += -L"C:\\Users\\Emre ATAKLI\\Documents\\qt\\UpdateController\\build-UpdateController-Desktop_Qt_6_4_1_MSVC2019_64bit-Debug\\debug\\static" -lupdatecontroller
}
unix{
INCLUDEPATH += /home/emre/qtprojects/UpdateController/include
CONFIG(debug, debug|release): LIBS += -L/home/emre/qtprojects/UpdateController/build-UpdateController-Desktop_Qt_6_4_3_GCC_64bit-Debug/debug/static -lUpdateController
else:CONFIG(release, debug|release): LIBS += -L/home/b720/qtprojects/UpdateController/build-UpdateController-Desktop_Qt_6_4_3_GCC_64bit-Release/release/static -lUpdateController
}

win32:RC_ICONS += opened_book.ico  # Icon by Hilmy Abiyyu Asad on freeicons.io

QT += sql xml widgets multimedia

FORMS += \
    qaform.ui \
    stackedwidget.ui
