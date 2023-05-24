#include "select.h"
#include "ui_select.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QChart>
#include <QLineSeries>

Select::Select(QSqlDatabase& db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Select)
{
    ui->setupUi(this);
    connect(ui->submitb,&QPushButton::clicked,this,&Select::submit);
    connect(ui->clearb,&QPushButton::clicked,this,&Select::clear);
    connect(ui->value,&QComboBox::currentIndexChanged,this,&Select::comboChange);
    connect(ui->limit,&QSpinBox::valueChanged,this,&Select::limitChange);

    chartView = new QChartView( this );
    chartView->move( QPoint( 615, 10 ) );
    chartView->resize( QSize( 631, 391 ) );
    chartView->show();

    QChart* chart = chartView->chart();

    chart->setTitle( "Wyniki" );



}

void Select::submit()
{
    ui->results->clear();
    datas.clear();
    QChart* chart = chartView->chart();
    QLineSeries* lineSeries = new QLineSeries( chart );
    chart->removeAllSeries();
    QSqlQuery q;
    switch(comboV)
    {
    case 0:
        q.prepare("SELECT id,WBC FROM wyniki ORDER BY id LIMIT ? ");
        ui->results->append("ID | WBCValue");
        break;
    case 1:
        q.prepare("SELECT id,HGB FROM wyniki ORDER BY id LIMIT ? ");
        ui->results->append("ID | HGBValue");
        break;
    case 2:
        q.prepare("SELECT id,PLT FROM wyniki ORDER BY id LIMIT ? ");
        ui->results->append("ID | PLTValue");
        break;
    case 3:
        q.prepare("SELECT id,TG FROM wyniki ORDER BY id LIMIT ? ");
        ui->results->append("ID | TGValue");
        break;
    case 4:
        q.prepare("SELECT id,HDL FROM wyniki ORDER BY id LIMIT ? ");
        ui->results->append("ID | HDLValue");
        break;
    case 5:
        q.prepare("SELECT id,HGB FROM wyniki ORDER BY id LIMIT ? ");
        ui->results->append("ID | LDLValue");
        break;
    default:
        QMessageBox::warning(this,tr("Fail"),"ERROR at ComboBox");
        return;
    }
    q.bindValue(0,limitV);
    if(!q.exec())
    {
        QMessageBox::warning(this,tr("Fail"),q.lastError().text());

    }
    else
    {
       int i = 1;
      while(q.next())
      {
          datas.emplace_back(i,q.value(1).toDouble());
          ui->results->append(q.value(0).toString() + "|" + q.value(1).toString());
          i++;
      }
      for( const QPointF& dataPoint : datas )
          {
              lineSeries->append( dataPoint );
          }
          chart->addSeries( lineSeries );
          chart->createDefaultAxes();



    }
}
void Select::comboChange()
{
    comboV = ui->value->currentIndex();
}
void Select::limitChange()
{
    limitV = ui->limit->value();
}
void Select::clear()
{
    ui->results->clear();
    QChart* chart = chartView->chart();
    chart->removeAllSeries();


}

Select::~Select()
{
    delete ui;
}
