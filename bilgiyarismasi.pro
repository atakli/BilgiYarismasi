HEADERS   = database.h \
    addqaform.h \
            dialog.h \
            mainwindownew.h \
#            stackedwidget.h \
            qaWindow.h
RESOURCES = masterdetail.qrc
SOURCES   = dialog.cpp \
    addqaform.cpp \
            database.cpp \
            main.cpp \
            mainwindownew.cpp \
#            stackedwidget.cpp \
            qaWindow.cpp

unix{
    QMAKE_CXXFLAGS += -std=c++2a
}
win32{
    QMAKE_CXXFLAGS_WARN_ON += /std:c++latest
}

QT += sql widgets
QT += xml widgets
requires(qtConfig(tableview))

EXAMPLE_FILES = albumdetails.xml

FORMS += \
    addqaform.ui \
    stackedwidget.ui
