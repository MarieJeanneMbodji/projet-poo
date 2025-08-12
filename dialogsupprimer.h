#ifndef DIALOGSUPPRIMER_H
#define DIALOGSUPPRIMER_H

#include <QDialog>
#include "tablehachage.h"

namespace Ui {
class DialogSupprimer;
}

class DialogSupprimer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSupprimer(Tablehachage *t, int hf, QWidget *parent = nullptr);
    ~DialogSupprimer();

private slots:
    void on_supprimer_clicked();

private:
    Ui::DialogSupprimer *ui;
    Tablehachage *table;
    int hachfunction;
};

#endif // DIALOGSUPPRIMER_H
