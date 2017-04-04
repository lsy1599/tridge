#include "visitdatetimeitem.h"
#include "ui_visitdatetimeitem.h"
#include <QDebug>

VisitDatetimeItem::VisitDatetimeItem(QDate date,QTime _t1,QTime _t2, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VisitDatetimeItem)
{
    ui->setupUi(this);
    init(date,_t1,_t2);
}

VisitDatetimeItem::VisitDatetimeItem(QWidget *parent):
    QWidget(parent),
    ui(new Ui::VisitDatetimeItem)
{
    ui->setupUi(this);

}

VisitDatetimeItem::~VisitDatetimeItem()
{
    //delete tei;
    for(int i=0;i<count;i++){
        delete tei[i];
    }
    free(tei);
    delete ui;
}


void VisitDatetimeItem::init(QDate date, const QTime &_t1, const QTime &_t2)
{
    d=date;
    ui->datel->setText(d.toString(" dd.MM.yyyy"));

    t1=_t1; t2=_t2;
    if(_t2<_t1){
        t1=_t2;
        t2=_t1;
    }

    max=(t2.addMSecs(-t1.msecsSinceStartOfDay()).msecsSinceStartOfDay()/1000/60/60);
    count=1;

    tei=(TimeEditItem**)malloc(sizeof(TimeEditItem*));
    tei[0]=new TimeEditItem(0);
    tei[0]->setList1(t1,t2.addSecs(-60*60));
    tei[0]->setList2(t1.addSecs(60*60),t2);
    tei[0]->setT2(t2);
    connect(tei[0],SIGNAL(chng1(QTime,int)),this,SLOT(chng1(QTime,int)));
    connect(tei[0],SIGNAL(chng2(QTime,int)),this,SLOT(chng2(QTime,int)));

    ui->gridLayout->addWidget(tei[0],0,2);

    ui->deltime->setEnabled(false);
    //ui->deltime->setEnabled();
    this->setMinimumHeight(35*count);
}

void VisitDatetimeItem::teiBS(bool value)
{
    for(int i=0;i<count;i++){
        tei[i]->blockSignals(value);
    }
}

void VisitDatetimeItem::on_addtime_clicked()
{

    //что то проверить можно ли добавлять новый
    if(count<max){
        ui->deltime->setEnabled(true);
        count++;
        this->setMinimumHeight(35*count);
        tei=(TimeEditItem**)realloc(tei,count*sizeof(TimeEditItem*));
        tei[count-1]=new TimeEditItem(count-1);
        tei[count-1]->setList1(t1.addSecs((count-1)*60*60),t2.addSecs(-1*60*60));
        tei[count-1]->setList2(t1.addSecs(count*60*60),t2);
        connect(tei[count-1],SIGNAL(chng1(QTime,int)),this,SLOT(chng1(QTime,int)));
        connect(tei[count-1],SIGNAL(chng2(QTime,int)),this,SLOT(chng2(QTime,int)));
        //обновить у всех предыдущих первый список от минимального(i*1час) до максимального (count-(i+1)*1час
        teiBS(true);

        for(int i=0;i<count;i++){
            QTime tempT1=tei[i]->getT1();
            //qDebug()<<tempT1;
            tei[i]->setList1(t1.addSecs(i*60*60),t2.addSecs(-(count-i)*60*60));
            tei[i]->setT1(tempT1);

            QTime tempT2=tei[i]->getT2();
            //qDebug()<<tempT2;
            tei[i]->setList2(t1.addSecs((i+1)*60*60),t2.addSecs(-(count-i-1)*60*60));
            tei[i]->setT2(tempT2);
        }
        tei[count-1]->setT1(tei[count-2]->getT2());
        tei[count-1]->setList2(t1.addSecs(count*60*60),t2);
        tei[count-1]->setT2(QTime(24,59,59));

        teiBS(false);
        ui->gridLayout->addWidget(tei[count-1],count-1,2);
        if(count==max)
            ui->addtime->setEnabled(false);
    }
}


void VisitDatetimeItem::on_deltime_clicked()
{
    delete tei[count-1];

    count--;
    //изменилось
    tei=(TimeEditItem**)realloc(tei,count*sizeof(TimeEditItem*));
    teiBS(true);
    for(int i=0;i<count;i++){
        QTime tempT1=tei[i]->getT1();
        //qDebug()<<tempT1;
        tei[i]->setList1(t1.addSecs(i*60*60),t2.addSecs(-(count-i)*60*60));
        tei[i]->setT1(tempT1);

        QTime tempT2=tei[i]->getT2();
        //qDebug()<<tempT2;
        tei[i]->setList2(t1.addSecs((i+1)*60*60),t2.addSecs(-(count-i-1)*60*60));
        tei[i]->setT2(tempT2);
    }
    teiBS(false);

    if(count==1)
        ui->deltime->setEnabled(false);
    if(count+1==max)
        ui->addtime->setEnabled(true);
    this->setMinimumHeight(35*count);
}

void VisitDatetimeItem::chng1(QTime t, int i)
{
    teiBS(true);
    QTime tempT2=tei[i]->getT2();
    tei[i]->setList2(t1.addSecs((i+1)*60*60),t2.addSecs(-(count-i-1)*60*60));
    if(t.addSecs(60*60)>=tempT2){
        tei[i]->setT2(t.addSecs(60*60));
    }
    else{
        tei[i]->setT2(tempT2);
    }

    teiBS(false);
    if(i>0){
        if(t<tei[i-1]->getT2())
            tei[i-1]->setT2(t);
    }
    if(i<count-1){
        if(tei[i]->getT2()>tei[i+1]->getT1())
            tei[i+1]->setT1(tei[i]->getT2());
    }
}

void VisitDatetimeItem::chng2(QTime t, int i)
{
    teiBS(true);

    QTime tempT1=tei[i]->getT1();
    tei[i]->setList1(t1.addSecs(i*60*60),t2.addSecs(-(count-i)*60*60));
    if(t.addSecs(-60*60)<=tempT1){
        tei[i]->setT1(t.addSecs(-60*60));
    }
    else{
        tei[i]->setT1(tempT1);
    }

    QTime tempT2=tei[i]->getT2();
    tei[i]->setList2(t1.addSecs((i+1)*60*60),t2.addSecs(-(count-i-1)*60*60));
    tei[i]->setT2(tempT2);
    teiBS(false);

    //tempT1=getT1();
    tei[i]->emitchng1();
    //tei[i]->setT1(tei[i]->getT1());
    //if()
}



void VisitDatetimeItem::on_checkBox_clicked(bool checked)
{
    if(checked==false){
        for(int i=1;i<count;i++){
            delete tei[i];
        }
        count=1;
        tei=(TimeEditItem**)realloc(tei,sizeof(TimeEditItem*));
        tei[0]->setList1(t1,t2.addSecs(-60*60));
        tei[0]->setList2(t1.addSecs(60*60),t2);
        tei[0]->setT1(t1);
        tei[0]->setT2(t2);
        ui->deltime->setEnabled(false);
        ui->addtime->setEnabled(true);
        this->setMinimumHeight(35*count);
    }
}


