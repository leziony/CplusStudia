#include "mainwindow.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <iostream>
int main(int argc, char *argv[])
{
    std::cout << "start" << std::endl;
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("46.242.240.123");
    db.setPort(3306);
    db.setDatabaseName("35216066_first");
    db.setUserName("35216066_first");
    db.setPassword("asdf1234#");
    if(!db.open())
    {
       QMessageBox warn;
       warn.setIcon(QMessageBox::Warning);
       warn.setText(db.lastError().text() + "\n Funkcje moga byc ograniczone.");
       warn.exec();

    }
    else
    {
        QSqlQuery q;
        q.prepare("SELECT * FROM wyniki LIMIT 1");
        q.exec();
        while(q.next())
        {
            std::cout << q.value(0).toInt() << std::endl;
            std::cout << q.value(1).toString().toStdString() << std::endl;
        }

    }
    MainWindow w(db);
    w.show();
    return a.exec();
}

