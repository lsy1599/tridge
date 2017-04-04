#include "visitform.h"
#include "ui_visitform.h"
#include <QSplitter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

VisitForm::VisitForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VisitForm)
{
    ui->setupUi(this);
    //centralVBox=new QVBoxLayout;
    int N=10;
    //QJsonDocument doc;
    //QJsonObject obj;
    //obj.insert("date",);

    //qDebug()<<obj;
    VisitDatetimeItem** dtitems=(VisitDatetimeItem**)malloc(N*sizeof(VisitDatetimeItem*));
    for(int i=0;i<N;i++){
        dtitems[i]=new VisitDatetimeItem(QDate::currentDate().addDays(i),QTime(10+i%3,i%2*30),QTime(17+i%4,(i+3)%2*30));
        layout()->addWidget(dtitems[i]);

    }
    //QSplitter* split=new QSplitter(Qt::Vertical);

    layout()->addWidget(new QSplitter(Qt::Vertical));

    this->show();
}

VisitForm::~VisitForm()
{
    delete ui;
}
