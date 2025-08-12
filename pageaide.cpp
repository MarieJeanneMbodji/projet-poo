#include "pageaide.h"
#include "ui_pageaide.h"

pageaide::pageaide(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pageaide)
{
    ui->setupUi(this);
}

pageaide::~pageaide()
{
    delete ui;
}
