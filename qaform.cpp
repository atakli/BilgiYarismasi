#include "qaform.h"
#include "ui_qaform.h"

//#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCheckBox>

extern QString appName;

QaForm::QaForm(QWidget *parent) : QWidget(parent), ui(new Ui::AddqaForm)
{
    ui->setupUi(this);

//    static QSqlDatabase db{QSqlDatabase::addDatabase("QSQLITE")}; // static omurlu olmali veya dynamically create etmeliyim cunku ctor'in scope'u bitince bunun omru bitecek. ama QSqlTableModel nesnesi araciligiyla kullanimda olmaya devam edecek. ama bi problem cikmadi. it is what ub is!
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bilgiyarismasi.db");
    if (!db.open())
    {
        qCritical() << "Failed to open database:" << db.lastError().text();
        return;												// TODO: constructor'in icinde return etmek dogru mu?
    }

    QSqlQuery query;// = model->query(); // yorum satiri yapmadan once db hic yokken query.exec calismiyordu, "Driver not loaded Driver not loaded" hatasi veriyordu nedense. baktım QSqlTableModel'deki ctor, db'yi const olarak aliyor. dedim acaba o yuzden mi table olusuramiyor. ama sonra veri girebiliyorum? database'in yapisina bakmak lazim, belki de veri girmekle table olusturmak farkli seylerdir.

    if (!query.exec("CREATE TABLE IF NOT EXISTS yarismalar (yarisma_ismi TEXT, sorular BLOB)") ||
            !query.exec("CREATE TABLE IF NOT EXISTS sorular (soru TEXT NOT NULL, cevap TEXT, süre integer NOT NULL)") ||
            !query.exec("CREATE TABLE IF NOT EXISTS muzikler (yol TEXT NOT NULL, isim TEXT NOT NULL)") ||
            !query.exec("CREATE TABLE IF NOT EXISTS yarisma (takim text, dogru integer, yanlis integer, sira integer)"))    // 10-15 satirlik kod verip burdaki yanlislik nedir diye sormustum, chatgpt arasindaki virgulu unuttugumu farketti
    {
        qCritical() << "Failed to create table:" << query.lastError().text();
        return;
    }

    model = new QSqlTableModel(this, db);
    model->setTable("sorular");     // sonradan farkettim: bu daha once exec'lerden onceydi. daha bu table yokken set ediyodum.
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!model->select())   // ya bunun geri donus degeri varmis. kontrol etmek lazim. bak etseydim daha erken farkedebilirdim sorunu. edit: ama qt'nin kendi verdigi ornekte kontrol etmemis. aklimda olmali demek ki
        qDebug() << "model->select(): false";

    model->setHeaderData(Soru, Qt::Horizontal, tr("Soru"));
    model->setHeaderData(Cevap, Qt::Horizontal, tr("Cevap"));
    model->setHeaderData(Sure, Qt::Horizontal, tr("Süre"));

    ui->tableView->setWordWrap(false);
    ui->tableView->setModel(model);
    model->insertColumn(CheckBox);
    model->setHeaderData(CheckBox, Qt::Horizontal, tr(""));     // basina column ekledigim halde tableview datalari 1. columndan baslayarak yerlestirecegini nerden biliyor?
    ui->tableView->hideColumn(Cevap);

    for (int row = 0; row < model->rowCount(); ++row)
    {
        ui->tableView->setIndexWidget(model->index(row, CheckBox), new QCheckBox(ui->tableView));
    }
    ui->tableView->resizeColumnToContents(CheckBox);

    connect(ui->addqaButton, &QPushButton::clicked, this, &QaForm::addRow);
    connect(ui->submitButton, &QPushButton::clicked, this, &QaForm::submit);
    connect(ui->removePushButton, &QPushButton::clicked, this, &QaForm::removeRows);
    connect(ui->revertButton, &QPushButton::clicked,  model, &QSqlTableModel::revertAll);
    connect(ui->showAnswersPushButton, &QPushButton::clicked, this, &QaForm::showHideAnswers);

//    connect(ui->addqaButton, &QPushButton::clicked, this, std::mem_fn(&AddQaDialog::show));
//    connect(addQaDialog, &QDialog::finished, this, &QaForm::handleDialogResult);
}

QaForm::~QaForm()
{
    delete ui;
}

QSqlTableModel *QaForm::getModel() const
{
    return model;
}
void QaForm::submit()
{
    qDebug() << "is transaction successful:" << model->database().transaction();
    if (model->submitAll())
    {
        ui->tableView->showColumn(Cevap);   // bir hata vardi, degisiklikleri submit edince goruntude problem oluyordu, cevap columnunu show edip en son tekrar hide edince duzeldi nasilsa.
        qDebug() << "is commit successful:" << model->database().commit();
        qDebug() << "is insertColumn successful:" << model->insertColumn(CheckBox);
        ui->tableView->hideColumn(Cevap);
        for (int i = 0; i < model->rowCount(); ++i)
        {
            ui->tableView->setIndexWidget(model->index(i, CheckBox), new QCheckBox(ui->tableView));
        }
        ui->tableView->resizeColumnToContents(CheckBox);
        ui->tableView->hideColumn(Cevap);
    }
    else
    {
        qDebug() << "is rollback successful:" << model->database().rollback();
        QMessageBox::warning(this, tr("Cached Table"), tr("Veritabanı hatası: %1").arg(model->lastError().text()));
    }
}
void QaForm::addRow()
{
    if(!model->insertRow(model->rowCount()))
        QMessageBox::warning(this, appName, "Satır eklenemedi!");
}

void QaForm::removeRows()
{
    const int rows = model->rowCount();
    qDebug() << "removeRows fonksiyonuna girdiginde" << rows << "adet row var.";
    if (rows == 0)
    {
        QMessageBox msgBox(QMessageBox::Warning, appName, "Hiçbir satırı seçmediniz.", QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok, "Tamam");
        msgBox.exec();
    }
    for (int row = 0; row < rows; ++row)
    {
        QCheckBox* checkBox = static_cast<QCheckBox*>(ui->tableView->indexWidget(model->index(row, CheckBox)));
        if (checkBox->isChecked())
            model->removeRow(row);
    }
}
void QaForm::closeEvent(QCloseEvent*)
{
    if (model->isDirty())
    {
        QMessageBox msgBox(QMessageBox::Question, appName, "Değişiklikler kaydedilsin mi?", QMessageBox::Yes | QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes, "Evet kaydet");
        msgBox.setButtonText(QMessageBox::No, "Hayır değişiklikleri geri al");
        if (msgBox.exec() == QMessageBox::Yes)
            submit();
        else
            model->revertAll();
    }
    close();
}
void QaForm::handleDialogResult(int result)
{
    if (result == QDialog::Accepted) {
        submit();
    }
    else {
        model->revertAll();
    }
}

void QaForm::showHideAnswers()
{
    if (ui->tableView->isColumnHidden(Cevap))
    {
        ui->tableView->showColumn(Cevap);
        ui->showAnswersPushButton->setText("Cevapları Gizle");
    }
    else
    {
        ui->tableView->hideColumn(Cevap);
        ui->showAnswersPushButton->setText("Cevapları Göster");
    }
}
