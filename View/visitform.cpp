#include "visitform.h"
#include "ui_visitform.h"

/// Описание визит формы
/// это форма
VisitForm::VisitForm(const QJsonArray &dateArr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VisitForm)
{
    ui->setupUi(this);
    N=0;
    dtitems=NULL;
    if(dateArr.count()>1){
        N=dateArr.count();
        dtitems=(VisitDatetimeItem**)malloc(N*sizeof(VisitDatetimeItem*));
        for(int i=0;i<N;i++){
            dtitems[i]=new VisitDatetimeItem(dateArr.at(i).toObject());
            if(dtitems[i]->is_valid()){
                layout()->addWidget(dtitems[i]);
            }
        }
    }
    layout()->addWidget(new QSplitter(Qt::Vertical));
    this->show();
}

QJsonArray VisitForm::toJson()
{
    QJsonArray mass;
    for(int i=0;i<N;i++){
        if(dtitems[i]->is_Checked()){
            mass.append(dtitems[i]->toJson());
        }
    }
    return mass;
    //конвертировать все даты в json и вернуть
}



VisitForm::~VisitForm()
{
    for(int i=0;i<N;i++){
        delete dtitems[i];
    }
    if(dtitems)
        free(dtitems);
    delete ui;
}
