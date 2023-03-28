#ifndef ADDQAFORM_H
#define ADDQAFORM_H

#include <QSqlTableModel>
#include <QWidget>

class DataBase;

namespace Ui { class AddqaForm; }

class AddqaForm : public QWidget
{
    Q_OBJECT
public:
    explicit AddqaForm(DataBase& db, QWidget *parent = nullptr);
    ~AddqaForm();

private:
    Ui::AddqaForm *ui;
    int rowIndex = 0;
    QSqlTableModel *model;
private slots:
    void addRow();
};

#endif // ADDQAFORM_H
