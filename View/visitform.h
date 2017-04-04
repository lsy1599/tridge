#ifndef VISITFORM_H
#define VISITFORM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "View/visitdatetimeitem.h"
#include <QDate>
namespace Ui {
class VisitForm;
}

class VisitForm : public QWidget
{
    Q_OBJECT

public:
    explicit VisitForm(QWidget *parent = 0);
    ~VisitForm();

private:
    Ui::VisitForm *ui;
    QVBoxLayout *centralVBox;
    //QLabel *text;
    VisitDatetimeItem **dtitems;



};

#endif // VISITFORM_H
