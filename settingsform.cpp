#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SettingsForm)
{
  ui->setupUi(this);
  printers=new Printers(this);
  settings=new QSettings(this);
  load();
}

SettingsForm::~SettingsForm()
{
  delete ui;
}

bool SettingsForm::load()
{
  for(int i=0;i<printers->size();i++){
      QListWidgetItem *item=new QListWidgetItem(
            QString::fromWCharArray(printers->atname(i)));

      settings->beginGroup("Printers");
      QString value=settings->value(
            QString::fromWCharArray(printers->atname(i)),"-1").toString();

      if (value=="-1"||value==""||value=="0") {
          settings->setValue(QString::fromWCharArray(printers->atname(i)),"0");
          item->setCheckState(Qt::Unchecked);
          ui->printerList->addItem(item);
        }
      else if(value=="1"){
          item->setCheckState(Qt::Checked);
          ui->printerList->addItem(item);
        }
      else{
          item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
          ui->autorize->addItem(item);
      }
      settings->endGroup();

      ui->printerList->addItem(item);
    }
  return false;
}

void SettingsForm::on_saveButton_clicked()
{
  settings->beginGroup("Printers");
  for(int i=0;i<ui->printerList->count();i++)
  {
    QString value=settings->value(ui->printerList->item(i)->text(),"fail").toString();
    if(value=="0"||value=="1"||value=="fail")
    {
      if(ui->printerList->item(i)->checkState()==Qt::Checked)
          settings->setValue(ui->printerList->item(i)->text(),"1");
      else
        settings->setValue(ui->printerList->item(i)->text(),"0");
    }
  }
  settings->endGroup();
  emit setingsrechange();
  close();
}
