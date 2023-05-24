#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qmessagebox.h"
#include "form.h"

MainWindow::MainWindow(QSqlDatabase& db,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , f( new Form(db,this) )

{
    ui->setupUi(this);
    connect(ui->submit,&QPushButton::clicked,this,&MainWindow::showtext);
    connect(ui->closeb,&QPushButton::clicked,this,&MainWindow::close);


}
void MainWindow::showtext()
{
       QString texter = ui->test->text();
        if(texter.isEmpty())
        {
            QMessageBox::warning(this,tr("No Name"),tr("There is no name!"));
        }
        else if(texter!="Test")
        {
            QMessageBox::warning(this,tr("Testing"),tr("Please write Test!"));
        }
        else
        {
            //QMessageBox::information(this,tr("Printout!"),QString("Printed info is: \"%1\"").arg(texter));
            f->show();
            this->hide();

       }



}
void MainWindow::close()
{
    ui->test->clear();
    QApplication::quit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

