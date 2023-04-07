#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>
#include <QTimer>

class QMediaPlayer;
class QAudioOutput;

#include "updatecontroller.h"
#include "qaform.h"

namespace Ui { class StackedWidget; }

class MainWindow : public QStackedWidget
{
    Q_OBJECT
public:
    MainWindow(QStackedWidget* parent = nullptr);
    ~MainWindow();
    void startTimer(int duration_as_sec);
private:
    int recordIndex = 0;
    QaForm qaForm;
    QTimer *timer_clock = nullptr;
    void (MainWindow::*slot)() = nullptr;
    bool isChangeNextQuestionImmediately;
    bool isWaitForNextQuestion = true;
    bool isFirstQuestion = true;
    QTimer *timer_question = nullptr;
    std::unique_ptr<QMediaPlayer> player;
    Ui::StackedWidget* ui;
    std::unique_ptr<QAudioOutput> audioOutput;
    UpdateController update;
    void startQuestion();
private slots:
    void finishCompetition();
    void startCompetition();
    void nextQuestion();
    void browseMusic();
};

#endif // MAINWINDOW_H
