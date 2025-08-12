#ifndef DIALOGINSERER_H
#define DIALOGINSERER_H

#include <QDialog>
#include "tablehachage.h"

namespace Ui {
class DialogInserer;
}

class DialogInserer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInserer(Tablehachage *t, int hf, QWidget *parent = nullptr);
    ~DialogInserer();

private slots:
    void on_inserer_clicked();

private:
    Ui::DialogInserer *ui;
    Tablehachage *table;
    int hachfunction;
};

#endif // DIALOGINSERER_H
