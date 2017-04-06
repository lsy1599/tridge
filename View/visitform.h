#ifndef VISITFORM_H
#define VISITFORM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QDate>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "View/visitdatetimeitem.h"

namespace Ui {
class VisitForm;
}

class VisitForm : public QWidget
{
    Q_OBJECT

public:
    VisitForm(const QJsonArray &dateArr,QWidget *parent = 0);
    QJsonArray toJson();

    ~VisitForm();

private:
    Ui::VisitForm *ui;
    QVBoxLayout *centralVBox;
    //QLabel *text;
    VisitDatetimeItem **dtitems;
    int N;



};

#endif // VISITFORM_H
