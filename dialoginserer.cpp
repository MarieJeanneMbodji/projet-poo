#include "dialoginserer.h"
#include "ui_dialoginserer.h"
#include <QMessageBox>

DialogInserer::DialogInserer(Tablehachage *t, int hf, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInserer),
    table(t),
    hachfunction(hf)
{
    ui->setupUi(this);
    connect(ui->pushButtonInserer, &QPushButton::clicked, this, &DialogInserer::on_inserer_clicked);
}

DialogInserer::~DialogInserer()
{
    delete ui;
}

void DialogInserer::on_inserer_clicked()
{
    QString cle = ui->lineEditCle->text();
    QString type = ui->lineEditType->text();
    QString standing = ui->lineEditStanding->text();
    int nbChambre = ui->lineEditChambre->text().toInt();
    int nbToilette = ui->lineEditToilette->text().toInt();
    float superficie = ui->lineEditSuperficie->text().toFloat();
    float prix = ui->lineEditPrix->text().toFloat();
    QString description = ui->plainTextEditDescription->toPlainText();

    if (cle.isEmpty() || type.isEmpty() || standing.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Certains champs obligatoires sont vides !");
        return;
    }

    int result = table->inserer(
        cle, type, standing, nbChambre, nbToilette, superficie, prix, description, hachfunction
    );

    if (result) {
        QMessageBox::information(this, "Succès", "Insertion réussie !");
        this->close();
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'insertion !");
    }
}
