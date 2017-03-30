#ifndef TRACKINGSETTINGSVIEW_H
#define TRACKINGSETTINGSVIEW_H

#include <QWidget>

namespace Ui {
  class TrackingSettingsView;
}

class TrackingSettingsView : public QWidget
{
  Q_OBJECT

public:
  explicit TrackingSettingsView(QWidget *parent = 0);
  ~TrackingSettingsView();

private:
  Ui::TrackingSettingsView *ui;
};

#endif // TRACKINGSETTINGSVIEW_H
