#include "addqadialog.h"
#include "ui_addqadialog.h"

#include <QMessageBox>
#include <QPushButton>

extern QString appName;

AddQaDialog::AddQaDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddQaDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Save)->setText("Kaydet");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("İptal");

    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &AddQaDialog::handleButtonClicked);
}

AddQaDialog::~AddQaDialog()
{
    delete ui;
}
void AddQaDialog::closeEvent(QCloseEvent* event)
{
    if (!ui->questionEdit->toPlainText().isEmpty() && !ui->answerEdit->toPlainText().isEmpty())
    {
        if (QMessageBox(QMessageBox::Question, appName, "Soru cevap kaydedilsin mi?", QMessageBox::Yes | QMessageBox::No).exec() == QMessageBox::Yes)
            ui->sa
        else
            model->revertAll();
    }
    close();
}
void AddQaDialog::handleButtonClicked(QAbstractButton *button)
{
    if (button == buttonBox->button(QDialogButtonBox::Save)) {
        accept();
    } else {
        reject();
    }
}
