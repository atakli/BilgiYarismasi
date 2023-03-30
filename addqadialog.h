#ifndef ADDQADIALOG_H
#define ADDQADIALOG_H

#include <QDialog>

class QAbstractButton;

namespace Ui { class AddQaDialog; }

class AddQaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddQaDialog(QWidget *parent = nullptr);
    ~AddQaDialog();
    void closeEvent(QCloseEvent* event) override;
private:
    Ui::AddQaDialog *ui;
private slots:
    void handleButtonClicked(QAbstractButton *button);
};

#endif // ADDQADIALOG_H
