#include "dialogsupprimer.h"
#include "ui_dialogsupprimer.h"
#include <QMessageBox>

DialogSupprimer::DialogSupprimer(Tablehachage *t, int hf, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSupprimer),
    table(t),
    hachfunction(hf)
{
    ui->setupUi(this);
    connect(ui->pushButtonSupprimer, &QPushButton::clicked, this, &DialogSupprimer::on_supprimer_clicked);
}

DialogSupprimer::~DialogSupprimer()
{
    delete ui;
}

void DialogSupprimer::on_supprimer_clicked()
{
    QString cle = ui->lineEditCle->text();

    if (cle.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une clé !");
        return;
    }

    bool result = table->suppression(cle, hachfunction);

    if (result) {
        QMessageBox::information(this, "Succès", "Suppression réussie !");
        this->close();
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune structure trouvée avec cette clé.");
    }
}


