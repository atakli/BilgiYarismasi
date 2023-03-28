#include "database.h"
#include "addqaform.h"
#include "ui_addqaform.h"

#include <QCheckBox>

class CustomSqlTableModel : public QSqlTableModel
{
public:
    CustomSqlTableModel()
    {

    }
};

AddqaForm::AddqaForm(DataBase& db, QWidget *parent) : QWidget(parent), ui(new Ui::AddqaForm)
{
    ui->setupUi(this);

    model = new QSqlTableModel(this, db.getDataBase());
    model->setTable("sorular");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(1, Qt::Horizontal, tr("Soru"));
    model->setHeaderData(2, Qt::Horizontal, tr("Cevap"));
    model->setHeaderData(3, Qt::Horizontal, tr("Süre"));

//    QTableView *view = new QTableView;
    ui->tableView->setModel(model);
//    ui->tableView->QTableView::setModel(model);
    ui->tableView->resizeColumnsToContents();
//    ui->tableView->setItemDelegateForColumn(

    int cellCount = 0;
    const auto id = db.calculate_current_id_of_table("sorular");
    if (id.has_value())
        cellCount = id.value();
    else
        return;

    for (int i = 0; i < cellCount; ++i)
    {
//        ui->tableView->setCellWidget(i, 0, new QCheckBox);
//        ui->tableView->setIndexWidget(model->index(i, 0), new QCheckBox);
//        ui->tableView->addScrollBarWidget();
    }

    connect(ui->addqaButton, &QPushButton::clicked, this, &AddqaForm::addRow);
}

AddqaForm::~AddqaForm()
{
    delete ui;
}
void AddqaForm::addRow()
{
//    ui->tableWidget->insertRow(rowIndex);

//    ui->tableWidget->setCellWidget(rowIndex, 0, new QCheckBox);
//    ui->tableWidget->setItem(rowIndex, 1, new QTableWidgetItem());
//    ui->tableWidget->setItem(rowIndex, 2, new QTableWidgetItem());
//    ui->tableWidget->setItem(rowIndex, 3, new QTableWidgetItem());
//    ui->tableWidget->resizeColumnsToContents();
//    ++rowIndex;
}
