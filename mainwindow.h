#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

class Form;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QSqlDatabase& db,QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void showtext();
    void close();
private:
    Ui::MainWindow *ui;
    Form* f = nullptr;



};
#endif // MAINWINDOW_H
