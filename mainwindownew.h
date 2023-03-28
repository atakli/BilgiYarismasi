#ifndef MAINWINDOWNEW_H
#define MAINWINDOWNEW_H

#include <QStackedWidget>
#include <QTimer>

#include "database.h"
#include "addqaform.h"

namespace Ui { class StackedWidget; }

extern QString dbName;

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
    DataBase db{dbName};
    AddqaForm addqaForm{db};
    QTimer *timer = nullptr;
    QTimer *timer_whole_competition = nullptr;
    void finishCompetition();
    void loopOverQuestions();
private slots:
    void startCompetition();
    void addQA();
};

#endif // MAINWINDOWNEW_H
