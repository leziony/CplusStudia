#ifndef FORM_H
#define FORM_H

#include <QSqlDatabase>
#include <QWidget>

enum class Gender
{
    Male = 0,
    Female = 1
};

class MainWindow;
class Select;
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QSqlDatabase& db,MainWindow* login,QWidget *parent = nullptr);
    ~Form();
private slots:
    void save();
    void load();
    void send();
    void results();
    void recall();
    void edit();
    void logout();
    void NameChange();
    void GenderChange();
    void AgeChange();
    void idChange();
    void WBCChange();
    void HGBChange();
    void PLTChange();
    void TGChange();
    void HDLChange();
    void LDLChange();

private:
    Ui::Form *ui;
    QSqlDatabase& db;
    MainWindow* login = nullptr;
    Select* select = nullptr;
    QString nameValue = "Żółw";
    Gender gender = Gender::Male;
    int ageValue = 13;
    int idv = 1;
    double WBCValue = 0.0;
    double HGBValue = 0.0;
    double PLTValue = 0.0;
    double TGValue = 0.0;
    double HDLValue = 0.0;
    double LDLValue = 0.0;

};

#endif // FORM_H
