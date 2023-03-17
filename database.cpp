#include "database.h"

DataBase::DataBase() : db(QSqlDatabase::addDatabase("QSQLITE"))
{
    db.setDatabaseName("bilgiYarismasi.db");
    if (!db.open())
    {
        qCritical() << "Failed to open database:" << db.lastError().text();
        return;												// TODO: constructor'in icinde return etmek dogru mu?
    }
}

DataBase::~DataBase()
{
    db.close();
}

QSqlDatabase& DataBase::getDataBase()
{
    return db;
}

ReturnSuccess DataBase::createTable(const QString& table)
{
    if (!db.tables().contains(table))
    {
        QSqlQuery query(db);
        const QString table_creation_query = "CREATE TABLE " + table + " (question TEXT PRIMARY KEY, answer TEXT)";
        if (!query.exec(table_creation_query))
        {
            qCritical() << "Failed to create table:" << query.lastError().text();
            return BadCreateTable;
        }
    }
    else
        qDebug() << "Table is already present";
    return GoodCreateTable;
}

ReturnSuccess DataBase::insertValue(const QString& table, const std::string& question, const std::string& answer)
{
    if (!db.tables().contains(table))
    {
        qCritical() << "No such table!";
        return NoSuchTable;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO " + table + " (question, answer) VALUES (:question, :answer)");
    query.bindValue(":question", question.c_str());
    query.bindValue(":answer", answer.c_str());

    if (!query.exec())
    {
        qCritical() << "Failed to insert data into table:" << query.lastError().text();
        return BadInsertion;
    }
    return GoodInsertion;
}
