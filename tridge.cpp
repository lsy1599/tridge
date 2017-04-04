#include "tridge.h"
#include "ui_tridge.h"
#include "view/visit.h"

Tridge::Tridge(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Tridge)
{
  ui->setupUi(this);

  //загрузить настройки

  //создать контролер (PrinterController)
  //инициализировать мониторы для контроллера из настроек
  //настроить подключение с сервером у контроллера из настроек
  controller=new PrintersController(this);



  //создать представление (mainview)
  mainview=new MainView(this);
  connect(mainview,SIGNAL(LoadSettingsForm()),this,SLOT(loadSettingsForm()));
  ui->verticalLayout_2->addWidget(mainview);

  mainview->addview("Printer1");
  mainview->addview("Printer2");
  //соединить представление (mainview) с контролером (PrinterController)
  settingsform=NULL;

  Visit *visit=new Visit();
  visit->show();
}

Tridge::~Tridge()
{
  delete ui;
}

void Tridge::loadSettingsForm()
{
  if(!settingsform){
      settingsform=new SettingsForm(0);\
      connect(settingsform,SIGNAL(setingsrechange()),this,SLOT(settingUpdatedSlot()));
      settingsform->show();
    }
  else{
      disconnect(settingsform,SIGNAL(setingsrechange()),this,SLOT(settingUpdatedSlot()));
      settingsform->close();
      delete settingsform;
      settingsform=new SettingsForm(0);
      connect(settingsform,SIGNAL(setingsrechange()),this,SLOT(settingUpdatedSlot()));
      settingsform->show();
    }
}

void Tridge::settingUpdatedSlot()
{
  //настройки были обновлены
}
