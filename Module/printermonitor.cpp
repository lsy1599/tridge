#include "printermonitor.h"

#include <QDebug>

PrinterMonitor::PrinterMonitor(LPTSTR szPrinterName, int num, QObject *parent) : QObject(parent)
{
  id=num;
    PrinterName=szPrinterName;
    if( ! OpenPrinter( PrinterName, &hPrinter, NULL ))
      emit openError();
    else{
        jobs.setprinter(hPrinter);
        connect(&jobs,SIGNAL(add_job(int)),this,SLOT(add_job_slot(int)));
        connect(&jobs,SIGNAL(total(int,int)),this,SLOT(total_slot(int,int)));
        connect(&jobs,SIGNAL(printed(int,int)),this,SLOT(printed_slot(int,int)));
        connect(&jobs,SIGNAL(job_delete(int)),this,SLOT(job_delete_slot(int)));
        jobs.start();
        upTmr=new QTimer(this);
        connect(upTmr, SIGNAL(timeout()), SLOT(updateQuery()));
        upTmr->start(5*60*1000);
      }
}

PrinterMonitor::~PrinterMonitor()
{
  disconnect(&jobs,SIGNAL(add_job(int)),this,SLOT(add_job_slot(int)));
  disconnect(&jobs,SIGNAL(total(int,int)),this,SLOT(total_slot(int,int)));
  disconnect(&jobs,SIGNAL(printed(int,int)),this,SLOT(printed_slot(int,int)));
  disconnect(&jobs,SIGNAL(job_delete(int)),this,SLOT(job_delete_slot(int)));
  jobs.stop();
  ClosePrinter( hPrinter );
}

QString PrinterMonitor::atName()
{
  return QString::fromWCharArray(PrinterName);
}

void PrinterMonitor::updateQuery()
{
  QDateTime current;
  current=QDateTime::currentDateTime();
  for(int i=0;i<query.size();i++){
      if(query.at(i)->changeTime.addSecs(20*60)<current)
        {
        currentJob* deljob=query.at(i);
        query.remove(i);
        emit PrintedPages(id,deljob->printed,current);
        delete deljob;
        break;
        }
    }
}

void PrinterMonitor::add_job_slot(int id)
{
  bool flag=true;
  for(int i=0;i<query.size();i++)
    if(query.at(i)->id==id){
      flag=false;
      break;
      }
  if(flag)
  {
      currentJob* newjob=new currentJob;
      newjob->id=id;
      newjob->printed=0;
      newjob->total=0;
      newjob->changeTime=QDateTime::currentDateTime();
      emit newjobsignal(id,newjob->changeTime);
    query.append(newjob);
  }
}

void PrinterMonitor::total_slot(int id, int total)
{
  for(int i=0;i<query.size();i++)
    if(query.at(i)->id==id){
        query.at(i)->total=total;
        query.at(i)->changeTime=QDateTime::currentDateTime();
        break;
      }
}

void PrinterMonitor::printed_slot(int id, int printed)
{
  for(int i=0;i<query.size();i++)
    if(query.at(i)->id==id){
        query.at(i)->changeTime=QDateTime::currentDateTime();
        query.at(i)->printed=printed;
        break;
      }
}


void PrinterMonitor::job_delete_slot(int id)
{
  for(int i=0;i<query.size();i++)
    {
      if(query.at(i)->id==id){
        currentJob* deljob=query.at(i);
        query.remove(i);
        QDateTime time=QDateTime::currentDateTime();
        emit PrintedPages(deljob->id,deljob->total,time);
        delete deljob;
        break;
      }
    }
}


void JobsNotification::run()
{
  status=true;
      HANDLE  hPrinterCN;
      DWORD   dwChanged;


      unsigned short      usPrinterJobInfo[4];
      usPrinterJobInfo[0]     =   JOB_NOTIFY_FIELD_PRINTER_NAME;//содержащую имя принтера
      usPrinterJobInfo[1]     =   JOB_NOTIFY_FIELD_DOCUMENT;//"MS-WORD: Review.doc ").
      usPrinterJobInfo[2]     =   JOB_NOTIFY_FIELD_TOTAL_PAGES;//определяет размер, в страницах, задания.
      usPrinterJobInfo[3]     =   JOB_NOTIFY_FIELD_PAGES_PRINTED;//указывает число страниц, напечатанных.

      PRINTER_NOTIFY_OPTIONS_TYPE         stgPrinterNotifyOptionsType[1];
      stgPrinterNotifyOptionsType[0].Type     =   JOB_NOTIFY_TYPE;
      stgPrinterNotifyOptionsType[0].Count    =   4;
      stgPrinterNotifyOptionsType[0].pFields  =   usPrinterJobInfo;

      PRINTER_NOTIFY_OPTIONS      stgPrinterOptions;
      stgPrinterOptions.Version   =   2;
      stgPrinterOptions.Flags     =   PRINTER_NOTIFY_OPTIONS_REFRESH ;
      stgPrinterOptions.Count     =   1;
      stgPrinterOptions.pTypes    =   stgPrinterNotifyOptionsType;

      PPRINTER_NOTIFY_INFO pPrinterInfo;

      // PrinterHandle initialisieren

          emit started();
          // Drucküberwachung anstossen ...
          hPrinterCN = FindFirstPrinterChangeNotification( hPrinter, PRINTER_CHANGE_JOB, NULL, &stgPrinterOptions );

          // ... hat funktioniert?
          if( INVALID_HANDLE_VALUE != hPrinterCN )
          {
              while( status )
              {
                  // Darauf warten, dass sich etwas bei den Druckaufträgen tut
                  WaitForSingleObject( hPrinterCN, INFINITE );

                  // Informationen auslesen
                  if( TRUE == FindNextPrinterChangeNotification( hPrinterCN, &dwChanged, NULL, (LPVOID *) &pPrinterInfo ) )
                  {

                      // Wenn ein Druckauftrag hinzugefügt wurde
                      if( PRINTER_CHANGE_ADD_JOB & dwChanged && pPrinterInfo)
                        {
                          /*
                          // пауза печати для контроля
                          SetJob( hPrinter, pPrinterInfo->aData[0].Id, 0, NULL, JOB_CONTROL_PAUSE );
                          */
                          emit add_job(pPrinterInfo->aData[0].Id);
                        }
                      if(PRINTER_CHANGE_DELETE_JOB & dwChanged && pPrinterInfo)
                        {
                          //qDebug()<<"delete!!!"<<pPrinterInfo->aData;
                          emit job_delete(pPrinterInfo->aData[0].Id);
                        }
                      // Wenn der Druckauftrag geschrieben wird
                      if( PRINTER_CHANGE_WRITE_JOB & dwChanged && pPrinterInfo)
                      {
                          //qDebug()<<"WRITE_JOB gefunden";
                          for( unsigned int i = 0; i < pPrinterInfo->Count; i++ )
                          {
                              //qDebug()<<i;
                              // Wenn es eine Druckauftragsänderung ist
                              if( JOB_NOTIFY_TYPE == pPrinterInfo->aData[i].Type )
                              {
                                  switch (pPrinterInfo->aData[i].Field) {
                                    case JOB_NOTIFY_FIELD_TOTAL_PAGES:
                                      emit total(pPrinterInfo->aData[i].Id,pPrinterInfo->aData[i].NotifyData.adwData[0]);
                                      break;
                                    case JOB_NOTIFY_FIELD_PAGES_PRINTED:
                                      emit printed(pPrinterInfo->aData[i].Id,pPrinterInfo->aData[i].NotifyData.adwData[0]);
                                      break;
                                    }
                              }
                          }
                      }


                  }
              }
            }
}

void JobsNotification::stop()
{
  status=false;
}

void JobsNotification::setprinter(HANDLE &Printer)
{
  hPrinter=Printer;
}

