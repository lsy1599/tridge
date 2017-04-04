#include "timeedititem.h"
#include "ui_timeedititem.h"

TimeEditItem::TimeEditItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeEditItem)
{
    ui->setupUi(this);
}

TimeEditItem::~TimeEditItem()
{
    delete ui;
}
