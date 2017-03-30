#ifndef PRINTERSCONTROLLER_H
#define PRINTERSCONTROLLER_H

#include <QObject>
#include "Module/printermonitor.h"
#include "Module/printers.h"
#include "Module/dbmonitor.h"
#include <QSettings>
#include <QDateTime>
#include "Module/printerclient.h"
#include <QStringList>
#include <QList>
class PrintersController : public QObject
{
  Q_OBJECT
public:
  explicit PrintersController(QObject *parent = 0);
  ~PrintersController();
private:
  //monitors
  PrinterMonitor** monitors;
  int monitorsCount;
  QStringList syskey;
  void initMonitors();
  //database
  DbMonitor* db;
  int*monitorIdInDB;
  void initDB();
  //clitent
  printerClient* client;
  QString host;
  QString port;
  void initClient();
  void sync();
  Printers* printers;
  QSettings* settings;
  void sendTasksToServer();

signals:

public slots:

private slots:
  void monitorDetectPrintedJob(int id, int count,QDateTime& time);
  void serverSay(QDateTime time, QString sey);
  void connectToServer();
};

#endif // PRINTERSCONTROLLER_H
