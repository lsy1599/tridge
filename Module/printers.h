#ifndef PRINTERS_H
#define PRINTERS_H

#include <QObject>
#include <windows.h>
#include <winspool.h>
#include <QStringList>

class Printers: public QObject
{
  Q_OBJECT
public:
  Printers(QObject *parent = 0);
  LPWSTR atname(const int);
  QStringList namesList();

  int size();
private:
  DWORD count;
  DWORD sz;
  DWORD Level;
  PRINTER_INFO_4* list;
signals:
  void error();
};

#endif // PRINTERS_H
