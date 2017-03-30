#ifndef TRIDGE_H
#define TRIDGE_H

#include <QMainWindow>
#include "View/mainview.h"
#include "settingsform.h"
#include "printerscontroller.h"

namespace Ui {
  class Tridge;
}

class Tridge : public QMainWindow
{
  Q_OBJECT

public:
  explicit Tridge(QWidget *parent = 0);
  ~Tridge();

private:
  Ui::Tridge *ui;
  MainView* mainview;
  SettingsForm* settingsform;
  PrintersController* controller;
private slots:
  void loadSettingsForm();
  void settingUpdatedSlot();
};

#endif // TRIDGE_H
