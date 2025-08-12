#ifndef DIALOGAFFICHER_H
#define DIALOGAFFICHER_H

#include <QDialog>
#include "TableHachage.h"


namespace Ui {
class Dialogafficher;
}

class Dialogafficher : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialogafficher(Tablehachage*t,QWidget *parent);
    ~Dialogafficher();
    void afficherDansTableView();
    void afficherImagesDepuisBase(QString cleMaison);

    
private slots:
    void on_tableMaison_clicked(const QModelIndex &index);

private:
    Ui::Dialogafficher *ui;
    Tablehachage *table;
};

#endif // DIALOGAFFICHER_H
