#ifndef VISIT_H
#define VISIT_H

#include <QWidget>
#include "visitform.h"
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class Visit;
}

class Visit : public QWidget
{
    Q_OBJECT

public:
    explicit Visit(QWidget *parent = 0);
    ~Visit();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Visit *ui;
    VisitForm* form;
};

#endif // VISIT_H
