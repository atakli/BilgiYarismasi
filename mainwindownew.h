#ifndef MAINWINDOWNEW_H
#define MAINWINDOWNEW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui { class MainWindow; }

class MainWindowNew : public QMainWindow
{
public:
	MainWindowNew(QMainWindow* parent = nullptr);
	~MainWindowNew();
	Ui::MainWindow* ui;
	void startTimer();
private:
	QTimer *timer = nullptr;
};

#endif // MAINWINDOWNEW_H
