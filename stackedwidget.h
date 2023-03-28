#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include <QStackedWidget>

namespace Ui { class StackedWidget; }

class StackedWidget : public QStackedWidget
{
	Q_OBJECT

public:
	explicit StackedWidget(QWidget *parent = nullptr);
	~StackedWidget();

private:
	Ui::StackedWidget *ui;
};

#endif // STACKEDWIDGET_H
