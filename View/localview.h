#ifndef LOCALVIEW_H
#define LOCALVIEW_H

#include <QWidget>
#define LOCAL_FOLLOW 0
#define NETWORK_FOLLOW 1
namespace Ui {
  class LocalView;
}

class LocalView : public QWidget
{
  Q_OBJECT

public:
  explicit LocalView(QWidget *parent = 0);
  ~LocalView();
  void setName(QString str);
  void setSysId(QString str);
  void setFollow(int value);
  void setPrice(double value);
  void setBulk(int value);
private:
  Ui::LocalView *ui;
signals:
  void chengeSysId();
  void addPages(int count);
  void cartridgeChenged();
  void problemSignal();
private slots:
  void on_changeSysId_clicked();
  void on_addPage_clicked();
  void on_pushButton_4_clicked();
  void on_pushButton_3_clicked();
};

#endif // LOCALVIEW_H
