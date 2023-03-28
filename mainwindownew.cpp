#include "mainwindownew.h"
#include "ui_stackedwidget.h"

#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <QTime>

enum {SettingsPage = 0, CompetitionPage = 1};
QString appName = "Bilgi Yarışması";

MainWindowNew::MainWindowNew(QStackedWidget *parent) : QStackedWidget(parent), ui(new Ui::StackedWidget)
{
	ui->setupUi(this);
    setCurrentIndex(CompetitionPage);

    connect(ui->settingsPushButton, &QPushButton::clicked, [this]{setCurrentIndex(SettingsPage);});
    connect(ui->startCompetitionPushButton, &QPushButton::clicked, this, &MainWindowNew::startCompetition);
    connect(ui->finishCompetitionPushButton, &QPushButton::clicked, this, &MainWindowNew::finishCompetition);
    connect(ui->addQAButton, &QPushButton::clicked, this, &MainWindowNew::addQA);

    db.createTable("yarismalar (yarisma_ismi TEXT, sorular TEXT)");
    db.createTable("sorular (soru TEXT, cevap TEXT, sure integer)");
    db.createTable("yarisma (takim TEXT, dogru integer yanlis integer sira integer)");

//    db.insertValue("sorular", "sdgfg afsddddddd", "afdwsfs afas afds", 60);
//    QSqlQuery query;
//    if (!query.exec("INSERT INTO sorular VALUES ('sdgfg afsddddddd', 'afdwsfs afas afds', 60)"))
//    {
//        qCritical() << "Failed to insert data into table:" << query.lastError().text();
//    }
}

MainWindowNew::~MainWindowNew()
{
	delete ui;
}

void MainWindowNew::startTimer(int duration_as_sec)
{
    auto doubleToMinSec = [](int number)
    {
        const int seconds = number % 60;
        const int minutes = (number - seconds) / 60;
        return QTime{minutes, seconds}.toString("HH:mm");
    };

    auto showTime = [doubleToMinSec, duration_as_sec, this]()mutable
	{
//        QTime time = doubleToMinSec(duration--);
//        const QString text = time.second() % 2 == 0 ? time.toString("mm ss") : time.toString("mm:ss");
        ui->lcdNumber->display(doubleToMinSec(duration_as_sec--));
	};
    ui->lcdNumber->setDigitCount(5);
	showTime();

    delete timer;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, showTime);
    timer->start(1000);

    delete timer_whole_competition;
    timer_whole_competition = new QTimer(this);
    connect(timer_whole_competition, &QTimer::timeout, this, &MainWindowNew::finishCompetition);
    timer_whole_competition->start(duration_as_sec * 1000);
}

void MainWindowNew::startCompetition()
{
    QMessageBox msgBox(QMessageBox::Question, appName, "Yarışmayı başlatıyorum...", QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Devam");
    msgBox.setButtonText(QMessageBox::No, "Hayır");
    if (msgBox.exec() == QMessageBox::Yes)
    {
        setCurrentIndex(CompetitionPage);
    }
    else
        return;
    ui->startCompetitionPushButton->setText("Yarışmaya Dön");
//    QtConcurrent::run(this, &MainWindowNew::loopOverQuestions);
    loopOverQuestions();
}

void MainWindowNew::finishCompetition()
{
    ui->startCompetitionPushButton->setText("Yarışmayı Başlat");
//    ui->lcdNumber->setDigitCount(5);
//    ui->lcdNumber->display("");
    startTimer(0);
}

void MainWindowNew::loopOverQuestions()
{
    startTimer(10);
    const QString table = "yarisma1";
    QSqlQuery query("SELECT * from " + table);
    while (query.next())
    {
        const QString question = query.value(0).toString();
        const int duration = query.value(1).toInt();
    }
}

void MainWindowNew::addQA()
{
    addqaForm.show();
}
