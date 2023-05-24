#include "form.h"
#include "select.h"
#include "ui_form.h"
#include "mainwindow.h"
#include <QFileDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

Form::Form(QSqlDatabase& db,MainWindow *login, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),
    db(db),
    login(login),
    select(new Select(db))
{
    ui->setupUi(this);
    connect(ui->saveb,&QPushButton::clicked,this,&Form::save);
    connect(ui->loadb,&QPushButton::clicked,this,&Form::load);
    connect(ui->databaseb,&QPushButton::clicked,this,&Form::send);
    connect(ui->lastb,&QPushButton::clicked,this,&Form::results);
    connect(ui->recallb,&QPushButton::clicked,this,&Form::recall);
    connect(ui->editb,&QPushButton::clicked,this,&Form::edit);
    connect(ui->logoutb,&QPushButton::clicked,this,&Form::logout);
    connect(ui->Dane,&QLineEdit::textChanged,this,&Form::NameChange);
    connect(ui->Plec,&QComboBox::currentIndexChanged,this,&Form::GenderChange);
    connect(ui->id,&QSpinBox::valueChanged,this,&Form::idChange);
    connect(ui->Wiek,&QSpinBox::valueChanged,this,&Form::AgeChange);
    connect(ui->WBC,&QDoubleSpinBox::valueChanged,this,&Form::WBCChange);
    connect(ui->HGB,&QDoubleSpinBox::valueChanged,this,&Form::HGBChange);
    connect(ui->PLT,&QDoubleSpinBox::valueChanged,this,&Form::PLTChange);
    connect(ui->TG,&QDoubleSpinBox::valueChanged,this,&Form::TGChange);
    connect(ui->HDL,&QDoubleSpinBox::valueChanged,this,&Form::HDLChange);
    connect(ui->LDL,&QDoubleSpinBox::valueChanged,this,&Form::LDLChange);




}
void Form::save()
{
     QString namefile = QFileDialog::getSaveFileName(this,"Zapisz Dane","","Text File(*.txt);;All Files(*)");
     if (namefile.isEmpty())
         return;
     else
     {
         QFile file(namefile);
         if(!file.open(QIODevice::WriteOnly))
         {
             QMessageBox::warning(this,"Failure",file.errorString());
             return;
         }
         file.write(nameValue.toLocal8Bit() + '\n');
         file.write(QByteArray::number(static_cast<int>(gender))+'\n');
         file.write(QByteArray::number(ageValue)+'\n');
         file.write(QByteArray::number(WBCValue)+'\n');
         file.write(QByteArray::number(HGBValue)+'\n');
         file.write(QByteArray::number(PLTValue)+'\n');
         file.write(QByteArray::number(TGValue)+'\n');
         file.write(QByteArray::number(HDLValue)+'\n');
         file.write(QByteArray::number(LDLValue)+'\n');

     }
    }
void Form::load()
{
    QString namefile = QFileDialog::getOpenFileName(this,"Zapisz Dane","","Text File(*.txt);;All Files(*)");
    if (namefile.isEmpty())
        return;
    else
    {
        QFile file(namefile);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,"Failure",file.errorString());
            return;
        }
        nameValue = file.readLine();
        gender = static_cast<Gender>(file.readLine().toInt());
        ageValue = file.readLine().toInt();
        WBCValue = file.readLine().toDouble();
        HGBValue = file.readLine().toDouble();
        PLTValue = file.readLine().toDouble();
        TGValue = file.readLine().toDouble();
        HDLValue = file.readLine().toDouble();
        LDLValue = file.readLine().toDouble();
        ui->Dane->setText(nameValue);
        ui->Plec->setCurrentIndex(static_cast<int>(gender));
        ui->Wiek->setValue(ageValue);
        ui->WBC->setValue(WBCValue);
        ui->HGB->setValue(HGBValue);
        ui->PLT->setValue(PLTValue);
        ui->TG->setValue(TGValue);
        ui->HDL->setValue(HDLValue);
        ui->LDL->setValue(LDLValue);
    }
}
void Form::send()
{
    QSqlQuery q;
    q.prepare("INSERT INTO wyniki (imie,plec,wiek,WBC,HGB,PLT,TG,HDL,LDL) values(?,?,?,?,?,?,?,?,?)");
    q.bindValue(0,nameValue);
    if(static_cast<int>(gender)==0)
    {
        q.bindValue(1,"M");
    }
    else if(static_cast<int>(gender)==1)
    {
        q.bindValue(1,"F");
    }
    else
    {
        q.bindValue(1,"-");
    }
    q.bindValue(2,ageValue);
    q.bindValue(3,WBCValue);
    q.bindValue(4,HGBValue);
    q.bindValue(5,PLTValue);
    q.bindValue(6,TGValue);
    q.bindValue(7,HDLValue);
    q.bindValue(8,LDLValue);
    if(!q.exec())
    {
        QMessageBox::warning(this,tr("Fail"),q.lastError().text());
    }
    else
    {
        QMessageBox::information(this,tr("Completed"),"Dane zostaly wyslane");
    }

}
void Form::results()
{
    select->show();
}
void Form::recall()
{
    QSqlQuery q;
    q.prepare("SELECT imie,plec,wiek,WBC,HGB,PLT,TG,HDL,LDL FROM wyniki WHERE id=?");
    q.bindValue(0,idv);
    if(!q.exec())
    {
        QMessageBox::warning(this,tr("Fail"),q.lastError().text());
    }
    else
    {
        q.next();
        if (!q.isValid())
        {
            QMessageBox::warning(this,tr("Fail"),"Dane nie zostały pobrane-zly ID");
            return;
        }

        if(q.value(1).toString() == "M")
        {
            ui->Plec->setCurrentIndex(0);
            gender = static_cast<Gender>(ui->Plec->currentIndex());
        }
        else if (q.value(1).toString() == "F")
        {
            ui->Plec->setCurrentIndex(1);
            gender = static_cast<Gender>(ui->Plec->currentIndex());
        }
        else
        {
            QMessageBox::warning(this,tr("Fail"),"Nieprawidlowa plec");
            return;
        }
       nameValue = q.value(0).toString();
       ageValue=q.value(2).toInt();
       WBCValue = q.value(3).toDouble();
       HGBValue= q.value(4).toDouble();
       PLTValue = q.value(5).toDouble();
       TGValue = q.value(6).toDouble();
       HDLValue = q.value(7).toDouble();
       LDLValue = q.value(8).toDouble();
       ui->Dane->setText(nameValue);
       ui->Wiek->setValue(ageValue);
       ui->WBC->setValue(WBCValue);
       ui->HGB->setValue(HGBValue);
       ui->PLT->setValue(PLTValue);
       ui->TG->setValue(TGValue);
       ui->HDL->setValue(HDLValue);
       ui->LDL->setValue(LDLValue);

        QMessageBox::information(this,tr("Completed"),"Dane zostaly wczytane");
    }

}
void Form::edit()
{
    QSqlQuery q;
    q.prepare("UPDATE wyniki SET imie = ?, plec = ?, wiek = ?, WBC = ?, HGB = ?, PLT = ?, TG = ?, HDL = ?, LDL = ? WHERE id = ?");
    q.bindValue(0,nameValue);
    if(static_cast<int>(gender)==0)
    {
        q.bindValue(1,"M");
    }
    else if(static_cast<int>(gender)==1)
    {
        q.bindValue(1,"F");
    }
    else
    {
        q.bindValue(1,"-");
    }
    q.bindValue(2,ageValue);
    q.bindValue(3,WBCValue);
    q.bindValue(4,HGBValue);
    q.bindValue(5,PLTValue);
    q.bindValue(6,TGValue);
    q.bindValue(7,HDLValue);
    q.bindValue(8,LDLValue);
    q.bindValue (9,idv);
    if(!q.exec())
    {
        QMessageBox::warning(this,tr("Fail"),q.lastError().text());
    }
    else
    {
        QMessageBox::information(this,tr("Completed"),"Dane zostaly edytowane");
    }

}
void Form::logout()
{
    login->show();
    this->close();
}
void Form::NameChange()
{
    nameValue = ui->Dane->text();

}
void Form::GenderChange()
{
    gender = static_cast<Gender>(ui->Plec->currentIndex());
}
void Form::AgeChange()
{
    ageValue = ui->Wiek->value();
}
void Form::idChange()
{
    idv = ui->id->value();
}
void Form::WBCChange()
{
    WBCValue = ui->WBC->value();
    if(WBCValue<3.90)
    {
        ui->WBCNominal->setText("L");
    }
    else if (WBCValue>10.20)
    {
        ui->WBCNominal->setText("H");
    }
    else
    {
        ui->WBCNominal->setText("-");
    }

}
void Form::HGBChange()
{
    HGBValue = ui->HGB->value();
    if(HGBValue<13.50)
    {
        ui->HGBNominal->setText("L");
    }
    else if (HGBValue>17.20)
    {
        ui->HGBNominal->setText("H");
    }
    else
    {
        ui->HGBNominal->setText("-");
    }

}
void Form::PLTChange()
{
    PLTValue = ui->PLT->value();
    if(PLTValue<150.0)
    {
        ui->PLTNominal->setText("L");
    }
    else if (PLTValue>370.0)
    {
        ui->PLTNominal->setText("H");
    }
    else
    {
        ui->PLTNominal->setText("-");
    }

}
void Form::TGChange()
{
    TGValue = ui->TG->value();
    if(TGValue>150.0)
    {
        ui->TGNominal->setText("H");
    }
    else
    {
        ui->TGNominal->setText("-");
    }
}
void Form::HDLChange()
{
    HDLValue = ui->HDL->value();
    if(HDLValue>40.0)
    {
        ui->HDLNominal->setText("H");
    }
    else
    {
        ui->HDLNominal->setText("-");
    }
}
void Form::LDLChange()
{
    LDLValue = ui->LDL->value();
    if(LDLValue>115.0)
    {
        ui->LDLNominal->setText("H");
    }
    else
    {
        ui->LDLNominal->setText("-");
    }
}
Form::~Form()
{
    delete ui;
}
