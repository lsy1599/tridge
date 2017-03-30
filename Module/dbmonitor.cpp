#include "dbmonitor.h"

DbMonitor::DbMonitor()
{
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("localdb.sqlite");
  open=true;
  if (!db.open()){
      //qDebug()<<"faIL!!";
      open=false;
      emit openError();
  }
  else{
      query=QSqlQuery(db);
      query.exec("CREATE TABLE IF NOT EXISTS Printer ("
                 "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
                 "name TEXT NOT NULL,"
                 "bulk INTEGER);");
      query.exec("CREATE TABLE IF NOT EXISTS Job ("
                 "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                 "printer INTEGER NOT NULL, "
                 "value INTEGER NOT NULL,"
                 "time INTEGER NOT NULL);");
      query.exec("CREATE TABLE IF NOT EXISTS Refill ("
                 "id INTEGER NOT NULL PRIMARY KEY,"
                 "printer INTEGER NOT NULL,"
                 "count INTEGER NOT NULL,"
                 "price REAL NOT NULL,"
                 "time INTEGER NOT NULL);");
    }
}

DbMonitor::~DbMonitor()
{

}

int DbMonitor::appendPrinter(QString printerName)
{
  query.exec(QString("Select id From Printer WHERE name='%1'")
             .arg(printerName));
  if(query.next()){
      return query.value(0).toInt();
    }
  else{
      query.exec(QString("INSERT INTO Printer (name) VALUES('%1')")
                 .arg(printerName));

      query.exec(QString("Select id From Printer WHERE name='%1'")
                 .arg(printerName));
      if(query.next()){
          return query.value(0).toInt();
        }
      else
        return -1;
    }
}

bool DbMonitor::appendTask(int idPrinter, int printedPages, QDateTime time)
{
  return query.exec(QString("INSERT INTO Job (printer, value, time)"
                    "VALUES (%1,%2,%3)")
                    .arg(QString::number(idPrinter),
                         QString::number(printedPages),
                         QString::number(time.toTime_t())));
}

QList<PrinterTask> DbMonitor::getTasks(int idPrinter)
{
  QList <PrinterTask> tasks;
  query.exec(QString("SELECT id,value,time FROM Job WHERE printer=%1")
             .arg(QString::number(idPrinter)));
  while(query.next()){
      PrinterTask task;
      task.id=query.value(0).toInt();
      task.idPrinter=idPrinter;
      task.printedPages=query.value(1).toInt();
      task.time=query.value(2).toInt();
      tasks<<task;
    }
  return tasks;
}

bool DbMonitor::deleteTask(int idTask)
{
  query.exec(QString("Select printer, value From Job WHERE id=%1").arg(QString::number(idTask)));
  if(query.next()){
      int idPrinter=query.value(0).toInt();
      int value=query.value(1).toInt();
      query.exec(QString("Select id, count FROM Refill "
                          "WHERE id="
                            "(SELECT MAX(id) FROM "
                              "(SELECT * FROM Refill WHERE printer=%1)"
                            ")")
                 .arg(QString::number(idPrinter)));
      if(query.next()){
          query.exec(
                QString("UPDATE Refill SET count=%1 WHERE id=%2")
                .arg(
                  QString::number(query.value(1).toInt()+value),
                  QString::number(query.value(0).toInt())
                  )
                );
        }

    }
  return query.exec(QString("DELETE FROM Job WHERE id=%1")
                    .arg(QString::number(idTask)));
}

bool DbMonitor::updateRefill(int idRefill, int idPrinter, int count, double price, int time)
{
  //тестировать могут быть проблемы с обновлением данных
  return query.exec(QString("INSERT INTO Refill (id, printer, count, price, time)"
                            "VALUES ('%1','%2','%3','%4','%5')")
                    .arg(QString::number(idRefill),
                         QString::number(idPrinter),
                         QString::number(count),
                         QString::number(price),
                         QString::number(time)
                         )
                    );
}

bool DbMonitor::overall(int idPrinter, int bulk)
{
  return query.exec(QString("UPDATE Printer SET bulk=%1 WHERE id=%2")
                      .arg(QString::number(bulk),
                           QString::number(idPrinter)
                           )
                      );
}
