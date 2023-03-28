#include "database.h"
#include "qaWindow.h"
#include "mainwindownew.h"

#include <QApplication>
#include <QFile>
#include <QDir>

#include <stdlib.h>

QString dbName = ".bilgiyarismasi.db";

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(masterdetail);

    QApplication app(argc, argv);

    QDir::setCurrent(QDir::home().absolutePath());
    MainWindowNew neww;
    neww.show();

//    if (!createConnection())
//        return EXIT_FAILURE;
//    QFile albumDetails("albumdetails.xml");
//    MainWindow window("artists", "albums", &albumDetails);
//    window.show();

    return app.exec();
}
