HEADERS   = dialog.h \
#            addqadialog.h \
#            database.h \
            mainwindownew.h \
#            stackedwidget.h \
            qaWindow.h \
            qaform.h

RESOURCES = masterdetail.qrc

SOURCES   = dialog.cpp \
#            addqadialog.cpp \
#            database.cpp \
            main.cpp \
            mainwindownew.cpp \
#            stackedwidget.cpp \
            qaWindow.cpp \
            qaform.cpp

unix{
    QMAKE_CXXFLAGS += -std=c++2a
}
win32{
#    QMAKE_CXXFLAGS_WARN_ON += /std:c++latest
    CONFIG += c++2b
}

win32:RC_ICONS += opened_book.ico  # Icon by Hilmy Abiyyu Asad on freeicons.io

QT += sql xml widgets multimedia
requires(qtConfig(tableview))

EXAMPLE_FILES = albumdetails.xml

FORMS += \
#    addqadialog.ui \
    qaform.ui \
    stackedwidget.ui
