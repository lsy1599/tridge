#include "tridge.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QApplication::setOrganizationName("SCRIPT");
  QApplication::setApplicationName("tridge");
  Tridge w;
  w.show();

  return a.exec();
}
