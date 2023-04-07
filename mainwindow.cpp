#include "mainwindow.h"
#include "ui_stackedwidget.h"

//#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFontMetrics>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QScreen>
#include <QTime>


enum {SettingsPage = 0, CompetitionPage = 1};
QString appName = "Bilgi Yarışması";

MainWindow::~MainWindow()
{
    delete ui;
}
MainWindow::MainWindow(QStackedWidget *parent) : QStackedWidget(parent), ui(new Ui::StackedWidget)
{
	ui->setupUi(this);

    const int height = this->size().height();
    const int width = this->size().width();

    auto locate = [this]                        // dikkat: chatgpt ctor'da this'i lambda'larda kullanma, ub olabilir cunku object henuz fully initialized olmamis olabilir dedi
    {
        const QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
        const int x = (screenGeometry.width() - this->width()) / 2;
        const int y = (screenGeometry.height() - this->height()) / 2;
        this->move(x, y);
    };

    auto settingsSizeConf = [this]{setCurrentIndex(SettingsPage); this->setFixedHeight(200); this->setFixedWidth(550);};
    settingsSizeConf();

    connect(ui->nextQuestionPushButton, &QPushButton::clicked, this, [this]{ui->nextQuestionPushButton->setEnabled(false);});
//    connect(ui->nextQuestionCheckBox, &QCheckBox::clicked, this, &MainWindow::onClicked_nextQuestionCheckBox);
    connect(ui->nextQuestionCheckBox, &QCheckBox::clicked, this, [this](bool flag){isWaitForNextQuestion = flag;});
    // oncesinde onClicked_nextQuestionCheckBox isminde membeer function ile yapmistim. ama cok ilginc: bunun ismi on ile clicked'in arasinda bosluk varken hata vermisti
    connect(ui->returnToMainPagePushButton, &QPushButton::clicked, this, [this, height, width, locate]{setCurrentIndex(CompetitionPage); this->setFixedSize(width, height); locate();});
    connect(ui->finishCompetitionPushButton, &QPushButton::clicked, this, &MainWindow::startCompetition);
    connect(ui->nextQuestionPushButton, &QPushButton::clicked, this, &MainWindow::startQuestion);
    connect(ui->settingsPushButton, &QPushButton::clicked, this, settingsSizeConf);
    connect(ui->musicSelectButton, &QPushButton::clicked, this, &MainWindow::browseMusic);
    connect(ui->addQAButton, &QPushButton::clicked, &qaForm, &QaForm::show);
    connect(ui->checkUpdatesButton, &QPushButton::clicked, this, [this]{update.isNewVersionAvailable(true);});
//    connect(ui->musicGroupBox, &QPushButton::clicked, this, [this](bool isActivated){if (isActivated) });

    update.setParameters("https://api.github.com/repos/atakli/BilgiYarismasi/releases/latest", appName, "bilgiyarismasi.exe");
    update.isNewVersionAvailable();

    player = std::make_unique<QMediaPlayer>();
    audioOutput = std::make_unique<QAudioOutput>();
    player->setAudioOutput(audioOutput.get());
    connect(player.get(), &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status)
    {
        if (status == QMediaPlayer::EndOfMedia && ui->musicGroupBox->isChecked())
        {
            player->setSource(QUrl::fromLocalFile(ui->musicLineEdit->text()));
            player->play();
        }
    });
//    audioOutput->setVolume(1);
}

void MainWindow::startTimer(int duration_as_sec)
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

    slot = (recordIndex++ == qaForm.getModel()->rowCount() - 1) ? &MainWindow::finishCompetition : &MainWindow::nextQuestion;
    connect(timer_question, &QTimer::timeout, this, [this]{if (isWaitForNextQuestion) ui->nextQuestionPushButton->setEnabled(true); player->pause(); (this->*slot)();});
    timer_question->start(duration_as_sec * 1000);
}

void MainWindow::startCompetition()
{
    QMessageBox msgBox(QMessageBox::Question, appName, "Yarışmayı başlatıyorum...", QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Devam");
    msgBox.setButtonText(QMessageBox::No, "Hayır");
    if (msgBox.exec() != QMessageBox::Yes)
        return;
    ui->finishCompetitionPushButton->setText("Yarışmayı Bitir");
    ui->finishCompetitionPushButton->disconnect();
    connect(ui->finishCompetitionPushButton, &QPushButton::clicked, this, &MainWindow::finishCompetition);
    nextQuestion();
}

void MainWindow::finishCompetition()
{
    isFirstQuestion = true;
    player->stop();
    ui->finishCompetitionPushButton->setText("Yarışmayı Başlat");
    ui->finishCompetitionPushButton->disconnect();
    connect(ui->finishCompetitionPushButton, &QPushButton::clicked, this, &MainWindow::startCompetition);
//    ui->lcdNumber->setDigitCount(5);
    ui->lcdNumber->display("");
    ui->questionLabel->setText("Yarışma Bitti");
    recordIndex = 0;
    if (slot != nullptr)
        disconnect(timer_question, &QTimer::timeout, this, slot);
    delete timer_clock;
    timer_clock = nullptr;
    delete timer_question;      // bunları create delete etmek yerine otomatik omurlu olsun, timer->stop ile yapayim
    timer_question = nullptr;
    ui->nextQuestionPushButton->setEnabled(false);
}

void MainWindow::startQuestion()
{
    const QSqlRecord record = qaForm.getModel()->record(recordIndex);
    const QString question = record.value(Soru).toString();
    const int duration = record.value(Sure).toInt();
    ui->questionLabel->setText(question);
    if (ui->musicGroupBox->isEnabled())
        player->play();
    startTimer(duration);
}
void MainWindow::nextQuestion()
{
    if (isFirstQuestion)
    {
        isFirstQuestion = false;
        player->setSource(QUrl::fromLocalFile(ui->musicLineEdit->text()));
        startQuestion();
    }
    else if (!isWaitForNextQuestion)
    {
        startQuestion();
    }
    else
        timer_question->disconnect();
}

void MainWindow::browseMusic()
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
