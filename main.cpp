#include "mainwindownew.h"

#include <QApplication>
#include <QFile>
#include <QDir>

#include <iostream>

#ifdef WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
#ifndef _DEBUG  // COK HOS
if (argc != 1 && argc != 2)
    {
        std::cerr << "No more arguments than one!" << std::endl;
        return -1;
    }
    if (argc == 1)
    {
        FreeConsole();
    }
    else if(strcmp(argv[1], "--enable-logging"))
    {
        std::cerr << "The given argument is wrong! Only the argument --enable-logging is accepted." << std::endl;
        return -2;
    }
#endif

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
    const QString exePath = QFileInfo{argv[0]}.absolutePath();  // burda sorun yasadim, setCurrent argv'yi set ediyor zannettim, sorunun sebebi de bu sandım. meger o set etmiyormus, programfiles'a yaptigim kurulumda klasorun isminde turkce karakterler oldugu icinmis. bir iki saatim gitti ama cozdum. edit: hayir set ediyormus gercekten :(((( ama turkce karakterde sorun var yine de muhtemelen
    qDebug() << "setCurrent islemi basarili:" << QDir::setCurrent(saveDir);
    const QString versionTxtPath = QDir{exePath}.filePath("version.txt");
    qDebug() << "version.txt path:" << versionTxtPath;
    if (!QFileInfo{"version.txt"}.exists())
        qDebug() << "version.txt yok!";
    if (QFileInfo{"version.txt"}.exists())
        qDebug() << "version.txt silme islemi basarili:" << QFile::remove("version.txt");
    qDebug() << "version.txt kopyalama islemi basarili:" << QFile::copy(versionTxtPath, "version.txt");

    MainWindowNew neww;
    neww.show();
    return app.exec();
}
