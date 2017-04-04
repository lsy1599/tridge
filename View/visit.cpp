#include "visit.h"
#include "ui_visit.h"
#include <QPushButton>

Visit::Visit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Visit)
{
    ui->setupUi(this);
    VisitForm* form=new VisitForm;
    //form->show();
    //ui->scrollArea->setMinimumWidth(form->minimumWidth());
    setMinimumWidth(640);
    setMinimumHeight(480);
    ui->scrollArea->setWidget(form);
}

Visit::~Visit()
{
    delete ui;
}
