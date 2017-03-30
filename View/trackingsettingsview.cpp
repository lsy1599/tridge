#include "trackingsettingsview.h"
#include "ui_trackingsettingsview.h"

TrackingSettingsView::TrackingSettingsView(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TrackingSettingsView)
{
  ui->setupUi(this);
}

TrackingSettingsView::~TrackingSettingsView()
{
  delete ui;
}
