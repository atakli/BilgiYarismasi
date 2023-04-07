#ifndef QAFORM_H
#define QAFORM_H

#include <QSqlTableModel>
#include <QWidget>

//class QSqlRelationalTableModel;

namespace Ui { class AddqaForm; }

enum Columns {CheckBox = 0, Soru = 1, Cevap = 2, Sure = 3};

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
    Ui::AddqaForm *ui;
    int rowIndex = 0;
    void submit();
    void closeEvent(QCloseEvent*) override;
//    QSqlRelationalTableModel *tableModel;
private slots:
    void addRow();
    void removeRows();
    void showHideAnswers();
    void handleDialogResult(int result);
};

#endif // QAFORM_H
