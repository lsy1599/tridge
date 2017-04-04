#include "timeedititem.h"
#include "ui_timeedititem.h"
#include <QComboBox>
#include <QDebug>
#include <QPalette>

TimeEditItem::TimeEditItem(int _index, QWidget *parent):
    QWidget(parent),
    ui(new Ui::TimeEditItem)
{
    ui->setupUi(this);
    index=_index;
}

TimeEditItem::~TimeEditItem()
{
    delete ui;
}

void TimeEditItem::setList1(QTime l1min, const QTime &l1max)
{
    QStringList list;
    while(l1min<=l1max){
        list<<l1min.toString("hh:mm");
        l1min=l1min.addSecs(60*30);
    }
    list1=list;
    ui->t1->clear();
    ui->t1->addItems(list);
}

void TimeEditItem::setList2(QTime l2min, const QTime &l2max)
{
    QStringList list;
    QTime cT1=QTime::fromString(ui->t1->currentText(),"hh:mm");
    while(l2min<=l2max){
        QString str;

        int shiftHour, shiftMinute;

        QTime t;
        if(l2min<cT1){
            //отрицательное смещение
            str+="-";
            t=QTime::fromMSecsSinceStartOfDay(cT1.msecsSinceStartOfDay()-l2min.msecsSinceStartOfDay());
        }
        else{
            //положительное смещение
            str+="+";
            t=QTime::fromMSecsSinceStartOfDay(l2min.msecsSinceStartOfDay()-cT1.msecsSinceStartOfDay());
        }
        shiftHour=t.hour();
        shiftMinute=t.minute();

        str+=QString::number(shiftHour)+"."+
                    QString::number((shiftMinute==30?5:0));

        list<<l2min.toString("hh:mm")+" ("+str+" ч.)";
        l2min=l2min.addSecs(60*30);
    }
    list2=list;
    ui->t2->clear();
    ui->t2->addItems(list);
}

int TimeEditItem::getindex()
{
    return index;
}

QTime TimeEditItem::getT1()
{
    return QTime::fromString(ui->t1->currentText(),"hh:mm");
}

QTime TimeEditItem::getT2()
{
    return QTime::fromString(ui->t2->currentText().split(" ").at(0),"hh:mm");
}

void TimeEditItem::setT1(QTime t)
{
    bool flag=false;
    int i;
    for(i=0;i<list1.size();i++){
        if(list1.at(i)==t.toString("hh:mm")){
            flag=true;
            break;
        }
    }
    if(flag)
        ui->t1->setCurrentIndex(i);
    else
        ui->t1->setCurrentIndex(list1.size()-1);
}

void TimeEditItem::setT2(QTime t)
{
    bool flag=false;
    int i;
    for(i=0;i<list2.size();i++){
        QString str=list2.at(i);
        if(str.split(" ").at(0)==t.toString("hh:mm")){
            flag=true;
            break;
        }
    }
    if(flag)
        ui->t2->setCurrentIndex(i);
    else
        ui->t2->setCurrentIndex(list2.size()-1);
}

void TimeEditItem::emitchng1()
{
    emit chng1(getT1(),index);
}

void TimeEditItem::on_t1_currentIndexChanged(const QString &arg1)
{
    QTime t=QTime::fromString(arg1,"hh:mm");
    emit chng1(t,index);
}

void TimeEditItem::on_t2_currentIndexChanged(const QString &arg1)
{
    QTime t=QTime::fromString(arg1.split(" ").at(0),"hh:mm");
    emit chng2(t,index);
}
