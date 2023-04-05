#ifndef QAFORM_H
#define QAFORM_H

#include <QSqlTableModel>
#include <QWidget>

//class QSqlRelationalTableModel;
class AddQaDialog;

namespace Ui { class AddqaForm; }

class QaForm : public QWidget
{
    Q_OBJECT
public:
    explicit QaForm(QWidget *parent = nullptr);
    ~QaForm();
    QSqlTableModel* getModel() const;

private:
    QSqlTableModel *model;
    QSqlDatabase db;
//    AddQaDialog* addQaDialog;
    Ui::AddqaForm *ui;
    int rowIndex = 0;
    void submit();
    void closeEvent(QCloseEvent*) override;
//    QSqlRelationalTableModel *tableModel;
private slots:
    void addRow();
    void handleDialogResult(int result);
    void showHideAnswers();
};

#endif // QAFORM_H
