#include "mainview.h"
#include "ui_mainview.h"

MainView::MainView(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MainView)
{
  ui->setupUi(this);
  ui->label->setText("Добавьте принтеры для слежения");
  count =0;
  views=NULL;
  toolbox=new QToolBox(this);
  ui->verticalLayout->addWidget(toolbox);
  toolbox->setVisible(false);
}

MainView::~MainView()
{
  for(int i=0;i<count;i++){
      delete views[i];
    }
  if(views)
    free(views);
  delete ui;
}

void MainView::addview(QString printerName)
{
  if(count==0){
    ui->label->setText("Активные мониторы: ");
    toolbox->setVisible(true);
  }
  count++;
  views=(LocalView**)realloc(views,count*sizeof(LocalView*));
  views[count-1]=new LocalView();
  toolbox->addItem(views[count-1],printerName);
}

int MainView::countView() const
{
  return count;
}

void MainView::on_pushButton_clicked()
{
    emit LoadSettingsForm();
}
