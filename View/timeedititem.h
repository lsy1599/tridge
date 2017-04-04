#ifndef TIMEEDITITEM_H
#define TIMEEDITITEM_H

#include <QWidget>
#include <QTime>
#include <QStringList>

namespace Ui {
class TimeEditItem;
}

class TimeEditItem : public QWidget
{
    Q_OBJECT

public:
    TimeEditItem(int _index, QWidget *parent = 0);
    ~TimeEditItem();

    void setList1(QTime l1min, const QTime &l1max);
    void setList2(QTime l2min,const QTime &l2max);
    int getindex();
    QTime getT1();
    QTime getT2();
    void setT1(QTime t);
    void setT2(QTime t);
    void emitchng1();
private slots:
    void on_t1_currentIndexChanged(const QString &arg1);
    void on_t2_currentIndexChanged(const QString &arg1);

private:
    QStringList list1;
    QStringList list2;
    int index;
    Ui::TimeEditItem *ui;

signals:
    void chng1(QTime, int);
    void chng2(QTime, int);

public slots:

};

#endif // TIMEEDITITEM_H
