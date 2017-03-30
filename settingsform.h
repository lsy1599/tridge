#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>
#include "Module/printers.h"
#include <QSettings>
#include <QListWidgetItem>
#include <QTreeWidget>

namespace Ui {
  class SettingsForm;
}

class SettingsForm : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsForm(QWidget *parent = 0);
  ~SettingsForm();

private:
  Printers* printers;

  Ui::SettingsForm *ui;
  QSettings *settings;
  bool load();
  bool save();

private slots:
  void on_saveButton_clicked();
signals:
  void setingsrechange();

};

#endif // SETTINGSFORM_H
