#include "dialogrechercher.h"
#include "ui_dialogrechercher.h"

#include <QMessageBox>

Dialogrechercher::Dialogrechercher(Tablehachage *t, int hf, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogrechercher),
    table(t),
    hachfunction(hf)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &Dialogrechercher::on_pushButton_clicked);
}

Dialogrechercher::~Dialogrechercher()
{
    delete ui;
}


void Dialogrechercher::on_pushButton_clicked()
{
    QString cle = ui->lineEditCle->text();

    if (cle.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une clé !");
        return;
    }

    // On récupère la maison à partir de la clé
    Maison m = table->Get(cle, hachfunction);

    // Si la clé est introuvable, on vérifie un champ indicatif
    if (m.type == "undefined key") {
        QMessageBox::warning(this, "Introuvable", "Aucune maison avec cette clé !");
        return;
    }

    // Si la maison est trouvée, on affiche ses informations
    QString infos = QString(
        "Type : %1\n"
        "Standing : %2\n"
        "Nombre de chambres : %3\n"
        "Nombre de toilettes : %4\n"
        "Superficie : %5 m²\n"
        "Prix : %6 FCFA\n"
        "Description : %7")
        .arg(m.type)
        .arg(m.standing)
        .arg(m.nb_chambre)
        .arg(m.nb_toilettes)
        .arg(m.superficie)
        .arg(m.prix)
        .arg(m.description);

    QMessageBox::information(this, "Maison trouvée", infos);
}

