#include "qaWindow.h"
#include "mainwindownew.h"

#include <QApplication>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
//    Q_INIT_RESOURCE(masterdetail);

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

//    if (!createConnection())
//        return EXIT_FAILURE;
//    QFile albumDetails("albumdetails.xml");
//    MainWindow window("artists", "albums", &albumDetails);
//    window.show();

    return app.exec();
}
