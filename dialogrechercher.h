#ifndef DIALOGRECHERCHER_H
#define DIALOGRECHERCHER_H

#include <QDialog>
#include "tablehachage.h"


namespace Ui {
class Dialogrechercher;
}

class Dialogrechercher : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialogrechercher(Tablehachage* t, int hf,QWidget *parent = 0);
    ~Dialogrechercher();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialogrechercher *ui;
    Tablehachage *table;
    int hachfunction;
};

#endif // DIALOGRECHERCHER_H
