#include "mainwindownew.h"
#include "ui_stackedwidget.h"

#include <QtConcurrent/QtConcurrent>
//#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFontMetrics>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlRecord>

#include <QTime>

#include <QSqlQuery>

enum {SettingsPage = 0, CompetitionPage = 1};
QString appName = "Bilgi Yarışması";

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"
                        "This example needs SQLite support. Please read "
                        "the Qt SQL driver documentation for information how "
                        "to build it.\n\n"
                        "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query;

    query.exec("create table artists (id int primary key, artist varchar(40), albumcount int)");

    query.exec("insert into artists values(0, '<all>', 0)");
    query.exec("insert into artists values(1, 'Ane Brun', 2)");
    query.exec("insert into artists values(2, 'Thomas Dybdahl', 3)");
    query.exec("insert into artists values(3, 'Kaizers Orchestra', 3)");

    query.exec("create table albums (albumid int primary key, title varchar(50), artistid int, year int)");

    query.exec("insert into albums values(1, 'Spending Time With Morgan', 1, 2003)");
    query.exec("insert into albums values(2, 'A Temporary Dive', 1, 2005)");
    query.exec("insert into albums values(3, '...The Great October Sound', 2, 2002)");
    query.exec("insert into albums values(4, 'Stray Dogs', 2, 2003)");
    query.exec("insert into albums values(5, 'One day you`ll dance for me, New York City', 2, 2004)");
    query.exec("insert into albums values(6, 'Ompa Til Du D\xc3\xb8r', 3, 2001)");
    query.exec("insert into albums values(7, 'Evig Pint', 3, 2002)");
    query.exec("insert into albums values(8, 'Maestro', 3, 2005)");

    return true;
}

MainWindowNew::MainWindowNew(QStackedWidget *parent) : QStackedWidget(parent), ui(new Ui::StackedWidget)
{
	ui->setupUi(this);

    const int height = this->size().height();
    const int width = this->size().width();

    auto settingsSizeConf = [this]{setCurrentIndex(SettingsPage); this->setFixedHeight(200); this->setFixedWidth(550);};
    settingsSizeConf();

    connect(ui->nextQuestionPushButton, &QPushButton::clicked, this, [this]{ui->nextQuestionPushButton->setEnabled(false);});
    connect(ui->nextQuestionCheckBox, &QCheckBox::clicked, this, &MainWindowNew::onClicked_nextQuestionCheckBox);
    connect(ui->returnToMainPagePushButton, &QPushButton::clicked, this, [this, height, width]{setCurrentIndex(CompetitionPage); this->setFixedSize(width, height);});
    connect(ui->finishCompetitionPushButton, &QPushButton::clicked, this, &MainWindowNew::startCompetition);
    connect(ui->nextQuestionPushButton, &QPushButton::clicked, this, &MainWindowNew::startQuestion);
    connect(ui->settingsPushButton, &QPushButton::clicked, this, settingsSizeConf);
    connect(ui->musicSelectButton, &QPushButton::clicked, this, &MainWindowNew::browseMusic);
    connect(ui->addQAButton, &QPushButton::clicked, this, &MainWindowNew::addQA);
    connect(ui->checkUpdatesButton, &QPushButton::clicked, this, [this]{update.isNewVersionAvailable();});
//    connect(ui->musicGroupBox, &QPushButton::clicked, this, [this](bool isActivated){if (isActivated) });

    update.setParameters("https://api.github.com/repos/atakli/BilgiYarismasi/releases/latest", appName, "bilgiyarismasi.zip");
    update.isNewVersionAvailable();

    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status)
    {
        if (status == QMediaPlayer::EndOfMedia && ui->musicGroupBox->isChecked())
        {
            player->setSource(QUrl::fromLocalFile(ui->musicLineEdit->text()));
            player->play();
        }
    });
//    audioOutput->setVolume(1);
}

MainWindowNew::~MainWindowNew()
{
	delete ui;
    delete player;
    delete audioOutput;
}

void MainWindowNew::startTimer(int duration_as_sec)
{
    auto showTime = [duration_as_sec, this]()mutable
	{
        const int number = duration_as_sec--;
        const int seconds = number % 60;
        const int minutes = (number - seconds) / 60;
        ui->lcdNumber->display(QTime{minutes, seconds}.toString("HH:mm"));
	};
    ui->lcdNumber->setDigitCount(5);
	showTime();

    delete timer_clock;
    timer_clock = new QTimer(this);
    connect(timer_clock, &QTimer::timeout, this, showTime);
    timer_clock->start(1000);

    delete timer_question;
    timer_question = new QTimer(this);

    slot = (recordIndex++ == qaForm.getModel()->rowCount() - 1) ? &MainWindowNew::finishCompetition : &MainWindowNew::nextQuestion;
    connect(timer_question, &QTimer::timeout, this, [this]{if (isWaitForNextQuestion) ui->nextQuestionPushButton->setEnabled(true); player->pause(); (this->*slot)();});
    timer_question->start(duration_as_sec * 1000);
}

void MainWindowNew::startCompetition()
{
    QMessageBox msgBox(QMessageBox::Question, appName, "Yarışmayı başlatıyorum...", QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Devam");
    msgBox.setButtonText(QMessageBox::No, "Hayır");
    if (msgBox.exec() != QMessageBox::Yes)
        return;
    ui->finishCompetitionPushButton->setText("Yarışmayı Bitir");
    ui->finishCompetitionPushButton->disconnect();
    connect(ui->finishCompetitionPushButton, &QPushButton::clicked, this, &MainWindowNew::finishCompetition);
    nextQuestion();
}

void MainWindowNew::finishCompetition()
{
    isFirstQuestion = true;
    player->stop();
    ui->finishCompetitionPushButton->setText("Yarışmayı Başlat");
    ui->finishCompetitionPushButton->disconnect();
    connect(ui->finishCompetitionPushButton, &QPushButton::clicked, this, &MainWindowNew::startCompetition);
//    ui->lcdNumber->setDigitCount(5);
    ui->lcdNumber->display("");
    ui->questionLabel->clear();
    recordIndex = 0;
    if (slot != nullptr)
        disconnect(timer_question, &QTimer::timeout, this, slot);
    delete timer_clock;
    timer_clock = nullptr;
    delete timer_question;      // bunları create delete etmek yerine otomatik omurlu olsun, timer->stop ile yapayim
    timer_question = nullptr;
    ui->nextQuestionPushButton->setEnabled(false);
}
void MainWindowNew::onClicked_nextQuestionCheckBox(bool flag) // cok ilginc: bunun ismi on_clicked_continuePushButton iken hata verdi
{
    isWaitForNextQuestion = flag;
}
void MainWindowNew::startQuestion()
{
    const QSqlRecord record = qaForm.getModel()->record(recordIndex);
    const QString question = record.value(0).toString();
    const int duration = record.value(2).toInt();
    ui->questionLabel->setText(question);
    if (ui->musicGroupBox->isEnabled())
        player->play();
    startTimer(duration);
}
void MainWindowNew::nextQuestion()
{
    if (isFirstQuestion)
    {
        isFirstQuestion = false;
        player->setSource(QUrl::fromLocalFile(ui->musicLineEdit->text()));      // TODO: yarismanin ortasinda music aktiflestirilirse de yap. basa almak zorunda kalinmasin
        startQuestion();
    }
    else if (!isWaitForNextQuestion)
    {
        startQuestion();
    }
    else
        timer_question->disconnect();
}

void MainWindowNew::browseMusic()
{
    const QString musicFile = QFileDialog::getOpenFileName(this, tr("Müzik seç"));
//    const qsizetype length = musicFile.length();
    ui->musicLineEdit->setText(musicFile);
    QFontMetrics m = ui->musicLineEdit->fontMetrics();
    const int length = m.horizontalAdvance(musicFile);
//    ui->musicLineEdit->setMinimumWidth(ui->musicLineEdit->sizeHint().width());
    ui->musicLineEdit->setMinimumWidth(length);
//    ui->musicLineEdit->setMinimumWidth(1000);
    ui->musicGroupBox->resize(length + 30, ui->musicGroupBox->height());        // 30 diye sabit vermek hos olmamis olabilir
}

void MainWindowNew::addQA()
{
    qaForm.show();
}
