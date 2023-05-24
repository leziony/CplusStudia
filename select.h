#ifndef SELECT_H
#define SELECT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QChartView>

namespace Ui {
class Select;
}

class Select : public QWidget
{
    Q_OBJECT

public:
    explicit Select(QSqlDatabase& db,QWidget *parent = nullptr);
    ~Select();
private slots:
    void submit();
    void clear();
    void comboChange();
    void limitChange();

private:
    Ui::Select *ui;
    QChartView* chartView;
    std::vector< QPointF > datas;
    int comboV = 0;
    int limitV = 0;
};

#endif // SELECT_H
