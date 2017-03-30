#ifndef DBMONITOR_H
#define DBMONITOR_H

#include <QObject>
#include <QtSql>
#include <QDateTime>
#include <QList>

struct PrinterTask{
  int id;
  int idPrinter;
  int printedPages;
  int time;
};
class DbMonitor: public QObject
{
  Q_OBJECT
public:
  DbMonitor();
  ~DbMonitor();
  ///добавить принтер и вернуть номер в БД
  int appendPrinter(QString printerName);
  ///Добавить задание
  bool appendTask(int idPrinter, int printedPages, QDateTime time);
  QList<PrinterTask> getTasks(int idPrinter);
  bool deleteTask(int idTask);
  //описать обновление заправки
  bool updateRefill(int idRefill, int idPrinter, int count, double price, int time);
  bool overall(int idPrinter,int bulk);
private:
  QSqlDatabase db;
  bool open;
  QSqlQuery query;
signals:
  void openError();

public slots:
};

#endif // DBMONITOR_H
