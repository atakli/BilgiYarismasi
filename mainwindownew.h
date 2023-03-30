#ifndef MAINWINDOWNEW_H
#define MAINWINDOWNEW_H

#include <QStackedWidget>
#include <QTimer>

#include "qaform.h"

namespace Ui { class StackedWidget; }

extern QString dbName;

bool createConnection();

struct Question
{
    QString question;
    int duration_min, duration_sec;
};

class MainWindowNew : public QStackedWidget
{
    Q_OBJECT
public:
    MainWindowNew(QStackedWidget* parent = nullptr);
	~MainWindowNew();
    Ui::StackedWidget* ui;
    void startTimer(int duration_as_sec);
private:
    int recordIndex = 0;
    QaForm qaForm;
    QTimer *timer_clock = nullptr;
    void (MainWindowNew::*slot)();
    bool isChangeNextQuestionImmediately;
    QTimer *timer_question = nullptr;
private slots:
    void finishCompetition();
    void startCompetition();
    void nextQuestion();
    void browseMusic();
    void addQA();
};

#endif // MAINWINDOWNEW_H
