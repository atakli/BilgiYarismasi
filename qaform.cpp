#include "qaform.h"
//#include "addqadialog.h"
#include "ui_qaform.h"

//#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCheckBox>

extern QString appName;

QaForm::QaForm(QWidget *parent) : QWidget(parent), ui(new Ui::AddqaForm)//, addQaDialog(new AddQaDialog)
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

//    tableModel = new QSqlRelationalTableModel(this);
//    tableModel->setTable("sorular");
//    tableModel->setRelation(2, QSqlRelation("yarismalar", "id", "yarisma_ismi"));
//    tableModel->select();

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
    const bool ret = model->select(); // ya bunun geri donus degeri varmis. kontrol etmek lazim. bak etseydim daha erken farkedebilirdim sorunu. edit: ama qt'nin kendi verdigi ornekte kontrol etmemis. aklimda olmali demek ki
    if(!ret)
        qDebug() << "model->select():" << ret;

    model->setHeaderData(0, Qt::Horizontal, tr("Soru"));
    model->setHeaderData(1, Qt::Horizontal, tr("Cevap"));
    model->setHeaderData(2, Qt::Horizontal, tr("Süre"));

    ui->tableView->setWordWrap(false);
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(1);

//    ui->tableView->QTableView::setModel(model);
//    ui->tableView->resizeColumnsToContents();
//    ui->tableView->setItemDelegateForColumn(
//    qDebug() << "header:" << ui->tableView->verticalHeader()->visualIndexAt(0);
//    qDebug() << "header:" << ui->tableView->selectAll();

//    ui->tableView->selectAll();
//    qDebug() << "list:" << ui->tableView->selectionModel()->selectedIndexes();

    for (int i = 0; i < 0; ++i)
    {
//        ui->tableView->setCellWidget(i, 0, new QCheckBox);
//        ui->tableView->setIndexWidget(model->index(i, 0), new QCheckBox);
//        ui->tableView->addScrollBarWidget();
    }

    connect(ui->submitButton, &QPushButton::clicked, this, &QaForm::submit);
    connect(ui->revertButton, &QPushButton::clicked,  model, &QSqlTableModel::revertAll);
    connect(ui->addqaButton, &QPushButton::clicked, this, &QaForm::addRow);
    connect(ui->showAnswersPushButton, &QPushButton::clicked, this, &QaForm::showHideAnswers);

//    connect(ui->addqaButton, &QPushButton::clicked, this, std::mem_fn(&AddQaDialog::show));
//    connect(addQaDialog, &QDialog::finished, this, &QaForm::handleDialogResult);
}

QaForm::~QaForm()
{
    delete ui;
//    delete tableModel;
    //    delete addQaDialog;
}

QSqlTableModel *QaForm::getModel() const
{
    return model;
}
void QaForm::submit()
{
    model->database().transaction();
    if (model->submitAll())
    {
//        if ()                                                 // column'lardan biri bos birakilirsa uyar. farkindaysa kaydet
        model->database().commit();
        ui->tableView->resizeColumnsToContents();
    }
    else
    {
        model->database().rollback();
        QMessageBox::warning(this, tr("Cached Table"), tr("Veritabanı hatası: %1").arg(model->lastError().text()));
    }
}
void QaForm::addRow()
{
    if(!model->insertRow(model->rowCount()))
        QMessageBox::warning(this, appName, "Satır eklenemedi!");

    //        model->removeRow()
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
    if (ui->tableView->isColumnHidden(1))
    {
        ui->tableView->showColumn(1);
        ui->showAnswersPushButton->setText("Cevapları Gizle");
    }
    else
    {
        ui->tableView->hideColumn(1);
        ui->showAnswersPushButton->setText("Cevapları Göster");
    }
}
