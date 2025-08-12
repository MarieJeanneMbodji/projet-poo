// Dans dialogafficher.cpp

#include "dialogafficher.h"
#include "ui_dialogafficher.h"
#include "mainwindow.h" // Assurez-vous que c'est inclus si vous en avez besoin
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QUuid>
// Inclure les entêtes spécifiques pour QStandardItemModel et QTableView
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView> // Assurez-vous d'inclure QTableView
#include "TableHachage.h" // Votre classe TableHachage

// Constructeur de Dialogafficher
Dialogafficher::Dialogafficher(Tablehachage* t, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogafficher),
    table(t) // Initialise le membre 'table' avec le pointeur passé
{
    ui->setupUi(this);

    // Initialisez votre QTableView si ce n'est pas déjà fait dans le Designer
    // Par exemple, si votre QTableView est nommé 'tableView' dans votre .ui
    // et que vous voulez sélectionner des lignes entières
    if (ui->tableView) { // Vérifier que le pointeur n'est pas nul
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        // Connecter le signal clicked de tableMaison (assurez-vous que c'est le bon nom dans votre UI)
        // à votre slot on_tableMaison_clicked
        connect(ui->tableView, &QTableView::clicked, this, &Dialogafficher::on_tableMaison_clicked);
    } else {
        qDebug() << "Erreur: QTableView 'tableView' non trouvé dans l'UI de Dialogafficher.";
        QMessageBox::critical(this, "Erreur d'UI", "Le composant TableView est introuvable.");
    }


    // Appel de la fonction pour afficher les données dès l'ouverture du dialogue
    // Vérifiez que 'table' n'est pas nullptr avant d'appeler afficherDansTableView
    if (table) {
        afficherDansTableView();
    } else {
        QMessageBox::critical(this, "Erreur de données", "La table de hachage n'a pas été initialisée correctement.");
        qDebug() << "Erreur: Pointeur TableHachage nul dans Dialogafficher.";
    }
}

Dialogafficher::~Dialogafficher()
{
    delete ui;
}

void Dialogafficher::afficherDansTableView() {
    // Créer un nouveau modèle à chaque fois pour s'assurer qu'il est propre
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"Clé", "Type", "Standing", "Chambres", "Toilettes", "Superficie", "Prix", "Description"});

    int row = 0;

    // Assurez-vous que 'table' est valide ici
    if (!table) {
        qDebug() << "Erreur: table est nul dans afficherDansTableView().";
        return;
    }

    // Parcourir votre TableHachage pour remplir le modèle
    // (Cette logique dépend de l'implémentation de votre TableHachage)
    // L'exemple ci-dessous suppose que votre TableHachage a une structure similaire à celle-ci
    // avec un tableau 'tab' et des nœuds 'noeud'
    for (int i = 0; i < table->taille_max; ++i) { // Utilisez 'taille_max' ou la capacité totale de votre table
        noeud* courant = table->tab[i]; // Supposons que 'tab' est le tableau interne de TableHachage
        while (courant) {
            model->setItem(row, 0, new QStandardItem(courant->element.cle));
            model->setItem(row, 1, new QStandardItem(courant->element.maison.type));
            model->setItem(row, 2, new QStandardItem(courant->element.maison.standing));
            model->setItem(row, 3, new QStandardItem(QString::number(courant->element.maison.nb_chambre)));
            model->setItem(row, 4, new QStandardItem(QString::number(courant->element.maison.nb_toilettes)));
            model->setItem(row, 5, new QStandardItem(QString::number(courant->element.maison.superficie)));
            model->setItem(row, 6, new QStandardItem(QString::number(courant->element.maison.prix)));
            model->setItem(row, 7, new QStandardItem(courant->element.maison.description));
            row++;
            courant = courant->suivant;
        }
    }

    // ÉTAPE CRUCIALE : Attacher le modèle à votre QTableView dans l'UI
    // Remplacez 'tableView' par l'objectName de votre QTableView dans dialogafficher.ui
    if (ui->tableView) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents(); // Ajuste automatiquement la largeur des colonnes
    } else {
        // Devrait être géré par le check dans le constructeur, mais un fallback ne nuit pas
        qDebug() << "Erreur: QTableView 'tableView' est nul lors de la définition du modèle.";
    }
}

// Implémentation du slot on_tableMaison_clicked
// Ce slot sera appelé lorsque l'utilisateur clique sur une cellule du QTableView.
void Dialogafficher::on_tableMaison_clicked(const QModelIndex &index)
{
    // Récupérer la clé de la maison de la ligne cliquée (colonne 0)
    QString cleMaison = index.sibling(index.row(), 0).data().toString();
    qDebug() << "Maison cliquée avec la clé :" << cleMaison;

    // Appeler la fonction pour afficher les images liées à cette clé
    afficherImagesDepuisBase(cleMaison);

    // Si vous voulez également afficher d'autres détails de la maison cliquée, vous pouvez les récupérer ici
    // QString type = index.sibling(index.row(), 1).data().toString();
    // QMessageBox::information(this, "Détails Maison", "Clé: " + cleMaison + "\nType: " + type);
}

// Votre fonction existante pour afficher les images (sans modification)
void Dialogafficher::afficherImagesDepuisBase(QString cleMaison) {
    // ... (votre code existant pour charger et afficher les images)
    QSqlDatabase db = QSqlDatabase::database(); // Utilise la connexion existante
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Erreur", "La base de données n'est pas ouverte !");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT image_blob FROM image WHERE maison_cle = ?");
    query.addBindValue(cleMaison);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible de charger les images :\\n" + query.lastError().text());
        return;
    }

    for (int i = 1; i <= 6; ++i) {
        QLabel* cible = findChild<QLabel*>("img" + QString::number(i));
        if (cible) cible->clear();
    }

    int num = 1;
    while (query.next() && num <= 6) {
        QByteArray blob = query.value(0).toByteArray();
        QPixmap pix;
        pix.loadFromData(blob);

        if (!pix.isNull()) {
            QLabel* cible = findChild<QLabel*>("img" + QString::number(num));
            if (cible) {
                // Échelle l'image pour s'adapter au QLabel, en gardant le ratio
                cible->setPixmap(pix.scaled(cible->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
        num++;
    }
}
