#ifndef PRINTERMONITOR_H
#define PRINTERMONITOR_H

#include <QObject>

#include <windows.h>
#include <winspool.h>
#include <QVector>
#include <QThread>
#include <QTime>
#include <QDateTime>
#include <QTimer>
#include <QString>

class JobsNotification : public QThread {
Q_OBJECT

public:
    void setprinter(HANDLE &Printer);
    void run();
    void stop();
private:
    bool status;
    HANDLE hPrinter;
signals:
  void started();
  void add_job(int id);
  void total(int id,int total);
  void printed(int id,int printed);
  void job_delete(int id);
};


struct currentJob{
  int id;
  int total;
  int printed;
  QDateTime changeTime;
};

class PrinterMonitor : public QObject
{
    Q_OBJECT
public:
    PrinterMonitor(LPTSTR szPrinterName,int num, QObject *parent = 0);

    ~PrinterMonitor();
    QString atName();
    int id;
private:
    HANDLE hPrinter;
    LPTSTR PrinterName;
    JobsNotification jobs;
    QVector <currentJob*> query;
    QTimer* upTmr;
signals:
    void openError();
    void PrintedPages(int id, int count,QDateTime& time);
    void newjobsignal(int id, QDateTime& time);
    //void determJob(int id,);

public slots:

private slots:
    void updateQuery();
    void add_job_slot(int id);
    void total_slot(int id,int total);
    void printed_slot(int id,int printed);
    void job_delete_slot(int id);
};


#endif // PRINTERMONITOR_H
