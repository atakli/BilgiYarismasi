#include "mainwindownew.h"
#include "ui_mainwindow.h"

#include <QTime>

MainWindowNew::MainWindowNew(QMainWindow *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindowNew::~MainWindowNew()
{
	delete ui;
}

void MainWindowNew::startTimer()
{
	if (timer != nullptr)
		delete timer;
	auto showTime = [this]
	{
		QTime time = QTime::currentTime();
		const QString text = time.second() % 2 == 0 ? time.toString("hh mm ss") : time.toString("hh:mm:ss");
		ui->lcdNumber->display(text);
	};
	ui->lcdNumber->setDigitCount(8);
	showTime();
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, showTime);
	timer->start(1000);
}
