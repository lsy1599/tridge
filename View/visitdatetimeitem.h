#ifndef VISITDATETIMEITEM_H
#define VISITDATETIMEITEM_H

#include <QWidget>
#include <QDate>
#include "View/timeedititem.h"

namespace Ui {
class VisitDatetimeItem;
}

class VisitDatetimeItem : public QWidget
{
    Q_OBJECT

public:
    VisitDatetimeItem(QDate date, QTime t1=QTime(10,0), QTime t2=QTime(18,0), QWidget *parent = 0);
    VisitDatetimeItem(QWidget *parent = 0);

    ~VisitDatetimeItem();


private slots:

    void on_addtime_clicked();
    void on_deltime_clicked();
    void chng1(QTime t,int i);
    void chng2(QTime t,int i);
    void on_checkBox_clicked(bool checked);

private:
    void init(QDate date, const QTime &_t1=QTime(10,0), const QTime &_t2=QTime(18,0));
    QDate d;
    QTime t1;
    QTime t2;
    TimeEditItem **tei;
    int max;
    int count;
    Ui::VisitDatetimeItem *ui;
    void teiBS(bool value);//заглушить сигналы
};

#endif // VISITDATETIMEITEM_H
