#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>
#include "localview.h"
#include <QToolBox>

namespace Ui {
  class MainView;
}

class MainView : public QWidget
{
  Q_OBJECT

public:
  explicit MainView(QWidget *parent = 0);
  ~MainView();
  void addview(QString printerName);
  LocalView** views;
  int countView() const;
private slots:
  void on_pushButton_clicked();

private:
  QToolBox* toolbox;
  int count;
  Ui::MainView *ui;
signals:
  void LoadSettingsForm();
};

#endif // MAINVIEW_H
