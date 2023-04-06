HEADERS   =  \
#            dialog.h
#            addqadialog.h \
#            database.h \
            mainwindownew.h \
#            stackedwidget.h \
#            qaWindow.h \
            qaform.h

RESOURCES = masterdetail.qrc \
    icon.qrc

SOURCES   =  \
#            dialog.cpp
#            addqadialog.cpp \
#            database.cpp \
            main.cpp \
            mainwindownew.cpp \
#            stackedwidget.cpp \
#            qaWindow.cpp \
            qaform.cpp

unix{
    QMAKE_CXXFLAGS += -std=c++2a
}
win32{
#    QMAKE_CXXFLAGS_WARN_ON += /std:c++latest
    CONFIG += c++2b
}

#windeployqt.exe bilgiyarismasi.exe -no-translations --no-opengl-sw --no-system-d3d-compiler

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
requires(qtConfig(tableview))

EXAMPLE_FILES = albumdetails.xml

FORMS += \
#    addqadialog.ui \
    qaform.ui \
    stackedwidget.ui
