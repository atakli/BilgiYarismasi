#include "mainwindownew.h"

#include <QApplication>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon{":/opened_book.ico"});

    const QString saveDir = QDir::home().filePath(".bilgiYarismasi");
    QDir d;
    if(!d.exists(saveDir))
    {
        if(!d.mkpath(saveDir))
        {
            qDebug() << "klasor olusturulamadi:" << saveDir;
            return -1;
        }
    }

    QDir::setCurrent(saveDir);
    MainWindowNew neww;
    neww.show();
    return app.exec();
}
