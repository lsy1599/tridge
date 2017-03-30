#include "printerscontroller.h"
#include <QDebug>

PrintersController::PrintersController(QObject *parent): QObject(parent)
{
  settings=new QSettings;//load settings
  monitors=NULL;
  printers=NULL;
  monitorIdInDB=NULL;
  db=NULL;
  client=NULL;
  //Иницилизация мониторов в соотвтествии с настройками

  initMonitors();
  //Подключимся к базе данных
  initDB();
  //client
  initClient();
////


}

PrintersController::~PrintersController()
{

}

void PrintersController::initMonitors()
{
  if(!monitors){
      //если id мониторов есть - удаляем
    if(monitorIdInDB)
      free(monitorIdInDB);
    //если принтеров нет - создаем
    if(!printers)
      printers=new Printers();
    settings->beginGroup("Printers");
    monitorsCount=0;
    for ( int i = 0; i < printers->size(); i++ ){
        QString value=settings->
            value(QString::fromWCharArray(printers->atname(i)),"-1")
            .toString();
        if(value=="-1")
          settings->setValue(QString::fromWCharArray(printers->atname(i)),"0");
        else if(value!=""&&value!="0"){
            monitorsCount++;
            monitors=(PrinterMonitor**)realloc(monitors,monitorsCount*sizeof(PrinterMonitor*));
            syskey<<value;
            monitors[monitorsCount-1]=new PrinterMonitor(printers->atname(i),i);
            connect( monitors[monitorsCount-1], SIGNAL(PrintedPages(int,int,QDateTime&)),
                     this,                      SLOT(monitorDetectPrintedJob(int,int,QDateTime&)));
          }
      }
    settings->endGroup();
    }

}

void PrintersController::initDB()
{
  db=new DbMonitor();
  //connect(db,SIGNAL(totalPage(int,int)),this,SLOT(totalPageSlot(int,int)));
  //connect(db,SIGNAL(lastPage(int,int)),this,SLOT(lastPageSlot(int,int)));
  //добавить данные принтер в БАЗУ ДАННЫХ И положить id в monitorIdInDB
  monitorIdInDB=(int*)malloc(monitorsCount*sizeof(int));

  for(int i=0;i<monitorsCount;i++){
      monitorIdInDB[i]=db->appendPrinter(monitors[i]->atName());
    }
}


void PrintersController::initClient()
{
  host=settings->value("Server","error").toString();
  if(host=="error"){
    settings->setValue("Server","tridge.hldns.ru");
    host="tridge.hldns.ru";
  }
  port=settings->value("Port","-1").toString();
  if(port=="-1"){
    settings->setValue("Port","7468");
    port="7468";
    }

  client=new printerClient(host,port.toInt());
  client->connectToHost();
  connect(client,SIGNAL(signalSay(QDateTime,QString)),this,SLOT(serverSay(QDateTime,QString)));
  connect(client,SIGNAL(signalConnected()),this,SLOT(connectToServer()));
  /*
  connect(client,SIGNAL(signalDisconnected()),this,SLOT(disconnectFromServer()));
  connect(client,SIGNAL(signalError(QString)),this,SLOT(connectToServerError(QString)));
  //connect(this,SIGNAL(sendToServer(QString)),client,SLOT(SendToServer(QString)));
  */
}

void PrintersController::sync()
{

  //client->connectToHost();
      for ( int i = 0; i < monitorsCount; i++ ){
          if(client->isConnect()){
          //syskey.at(i);
          //monitorIdInDB[i];
          if(syskey.at(i)!="1"){
              client->
                  SendToServer(
                    QString("#sync#%1#get#")
                    .arg(syskey.at(i)));
            }
        }
    }
}

void PrintersController::sendTasksToServer()
{
  for ( int i = 0; i < monitorsCount; i++ ){
      if(client->isConnect()&&syskey.at(i)!="1"){
              QList<PrinterTask> tasks=db->getTasks(monitorIdInDB[i]);
              //#newjob#<syskey>#<id>#<count:int>#<time:int>#
              for(int j=0;j<tasks.size();j++){
                  client->SendToServer(
                        QString("#newjob#%1#%2#%3#%4#")
                        .arg(syskey.at(i),
                             QString::number(tasks.at(j).id),
                             QString::number(tasks.at(j).printedPages),
                             QString::number(tasks.at(j).time)
                             ));
                }
            }
    }
}


void PrintersController::monitorDetectPrintedJob(int id, int count, QDateTime &time)
{
  //напечатано новое задание, что будем делать?
  //отправим в базу данных
  if(db)
    db->appendTask(monitorIdInDB[id],count,time);
  sendTasksToServer();
}

void PrintersController::serverSay(QDateTime time, QString sey)
{
  //qDebug()<<sey;
  QStringList query=sey.split("#");
  //qDebug()<<query;
  if(query.size()>3){
      int idPrinter=-1;
      for(int i=0;i<monitorsCount;i++){
          //query.at(2)-<syskey:QString>
          if(query.at(2)==syskey.at(i)){
              idPrinter=monitorIdInDB[i];
              break;
            }
        }
      if(idPrinter!=-1){
          if(query.at(1)=="sync"){
                  //так как перезаправок много рефил первый
                  if(query.at(3)=="refill"){
                      //прием данных о перезаправке
                      //query.at(4)-<idrefill:int> уникальный номер заправки
                      //query.at(5)-<time:int> дата заправки
                      //query.at(6)-<count:int> количество отпечатаных страниц
                      //query.at(7)-<price:double> стоимость заправки
                      db->updateRefill(QString(query.at(4)).toInt(),
                                       idPrinter,
                                       QString(query.at(6)).toInt(),
                                       QString(query.at(7)).toDouble(),
                                       QString(query.at(5)).toInt()
                                       );
                    }
                  else if(query.at(3)=="overall"){
                      //прием общих данных

                      //query.at(4)-<bulk:int>
                      db->overall(idPrinter,QString(query.at(4)).toInt());

                    }
                  else if(query.at(3)=="end"){
                      //конец приема данных
                      //аргументов нет
                      client->SendToServer(
                            QString("#sync#%1#accept#")
                            .arg(query.at(2)));
                      sendTasksToServer();
                    }
            }
          else if(query.at(1)=="jobaccept"){
                  //query.at(3)-<id>
                  //query.at(4)-<time>
              //подтвердить печать
              db->deleteTask(QString(query.at(3)).toInt());
              client->SendToServer(
                    QString("#newjob#%1#delete#%2#")
                    .arg(query.at(2),query.at(4)));
            }
        }
  }
}

void PrintersController::connectToServer()
{
  sync();
}
