HEADERS   = database.h \
            dialog.h \
            mainwindownew.h \
            qaWindow.h \
            stackedwidget.h
RESOURCES = masterdetail.qrc
SOURCES   = dialog.cpp \
            database.cpp \
            main.cpp \
            mainwindownew.cpp \
            qaWindow.cpp \
            stackedwidget.cpp

QT += sql widgets
QT += xml widgets
requires(qtConfig(tableview))

EXAMPLE_FILES = albumdetails.xml

FORMS += \
    stackedwidget.ui
