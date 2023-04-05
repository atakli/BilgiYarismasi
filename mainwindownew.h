#ifndef MAINWINDOWNEW_H
#define MAINWINDOWNEW_H

#include <QStackedWidget>
#include <QTimer>

class QMediaPlayer;
class QAudioOutput;

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
    void (MainWindowNew::*slot)() = nullptr;
    bool isChangeNextQuestionImmediately, isWaitForNextQuestion = true;
    bool isFirstQuestion = true;
    QTimer *timer_question = nullptr;
    void startQuestion();
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
private slots:
    void onClicked_continuePushButton(bool flag);
    void finishCompetition();
    void startCompetition();
    void nextQuestion();
    void browseMusic();
    void addQA();
};

#endif // MAINWINDOWNEW_H
