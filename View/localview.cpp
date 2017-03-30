#include "localview.h"
#include "ui_localview.h"

LocalView::LocalView(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LocalView)
{
  ui->setupUi(this);
  ui->label_5->setVisible(false);
  ui->price->setVisible(false);

}

LocalView::~LocalView()
{
  delete ui;
}

void LocalView::setName(QString str)
{
  ui->printerName->setText(str);
}

void LocalView::setSysId(QString str)
{
  ui->sysId->setText(str);
}

void LocalView::setFollow(int value)
{
  if(value==LOCAL_FOLLOW){
      ui->trackingType->setText("Локальное наблюдение");
      ui->changeSysId->setEnabled(false);
      ui->changeSysId->setVisible(false);
    }
  else if(value==NETWORK_FOLLOW){
      ui->trackingType->setText("Сетевое наблюдение");
      ui->changeSysId->setEnabled(true);
      ui->changeSysId->setVisible(true);
    }
}

void LocalView::setPrice(double value)
{
  ui->label_5->setVisible(true);
  ui->price->setText(QString("%1 руб.").arg(value));
  ui->price->setVisible(true);
}

void LocalView::setBulk(int value)
{
  if(value>=0&&value<100)
    ui->bulk->setValue(value);
}

void LocalView::on_changeSysId_clicked()
{
    emit chengeSysId();
    ui->changeSysId->setEnabled(false);
}

void LocalView::on_addPage_clicked()
{
  emit addPages(ui->countAddP->value());
  ui->countAddP->setValue(0);
}

void LocalView::on_pushButton_4_clicked()
{
    emit cartridgeChenged();
  ui->pushButton_4->setEnabled(false);
}

void LocalView::on_pushButton_3_clicked()
{
    emit problemSignal();
  ui->pushButton_3->setEnabled(false);
}
