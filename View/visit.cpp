#include "visit.h"
#include "ui_visit.h"


Visit::Visit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Visit)
{
    ui->setupUi(this);
    QJsonObject date1;
    date1.insert("date","12.10.2017");
    date1.insert("t1","10:00");
    date1.insert("t2","18:00");
    QJsonObject date2;
    date2.insert("date","13.10.2017");
    date2.insert("t1","11:30");
    date2.insert("t2","16:00");
    QJsonObject date3;
    date3.insert("date","14.10.2017");
    date3.insert("t1","12:00");
    date3.insert("t2","17:00");

    QJsonArray dateArr;
    dateArr.insert(0,date1);
    dateArr.insert(1,date2);
    dateArr.insert(2,date3);

    QJsonObject obj;
    obj.insert("comment","Hello, please check dates");
    obj.insert("dates",dateArr);


    setMinimumWidth(640);
    setMinimumHeight(480);
    form=new VisitForm(dateArr);


    ui->label->setText(obj.value("comment").toString());
    ui->scrollArea->setWidget(form);
}

Visit::~Visit()
{
    delete ui;
}

void Visit::on_pushButton_clicked()
{
    QJsonObject final;
    final.insert("soglas",form->toJson());
    final.insert("comment",ui->textEdit->document()->toPlainText());
    qDebug()<<final;
}
