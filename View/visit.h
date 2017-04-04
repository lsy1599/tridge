#ifndef VISIT_H
#define VISIT_H

#include <QWidget>
#include "visitform.h"

namespace Ui {
class Visit;
}

class Visit : public QWidget
{
    Q_OBJECT

public:
    explicit Visit(QWidget *parent = 0);
    ~Visit();

private:
    Ui::Visit *ui;
};

#endif // VISIT_H
