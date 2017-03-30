#include "printers.h"


Printers::Printers(QObject *parent): QObject(parent)
{
  count = 0;
  sz = 0;
  Level=4;
  EnumPrinters( PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, Level, NULL, 0, &sz, &count );
  if ( (list = (PRINTER_INFO_4*) malloc( sz )) == 0 )
      emit error();
  else if (!EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, Level, (LPBYTE)list, sz, &sz, &count))
  {
      free( list );
      emit error();
  }

}

LPWSTR Printers::atname(const int i)
{
  if(i>=0&&i<(int)count)
    return list[i].pPrinterName;
  else
    return NULL;
}

QStringList Printers::namesList()
{
  QStringList names;
  for(int i=0;i<(int)count;i++)
    names<<QString::fromWCharArray(list[i].pPrinterName);
  return names;
}

int Printers::size()
{
  return count;
}
