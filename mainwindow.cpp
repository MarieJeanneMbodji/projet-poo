#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include<QDebug>
#include <QUuid>
#include "dialoginserer.h"
#include "dialogsupprimer.h"
#include "dialogrechercher.h"
#include "dialogafficher.h"
#include "pageaide.h"
#include <QDateTime>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    table = new Tablehachage(10001);

    if (ui->Hashtohash) { // Vérifiez si l'objet existe avant de connecter
           connect(ui->Hashtohash, &QAction::triggered, this, &MainWindow::on_Hashtohash_triggered);
       } else {
           qDebug() << "Erreur: QAction 'hashtohash' non trouvé dans l'UI de MainWindow.";
       }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Fonction qui lit les données depuis une base SQLite
// et insère chaque maison + ses images dans la table de hachage
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileDialog>

void MainWindow::chargerDepuisSqlite(QString cheminBD) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // on crée un objet "db" de type base de donnée QTlite dans QT
    db.setDatabaseName(cheminBD);  // on donne à "db" le chemin d'accés vres notre fichier ou se trouve notre base

    if (!db.open()) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir la base de données !");
        return;
    }

    QSqlQuery query("SELECT * FROM maison");  // requête sql por sélectionner toute les lignes de la table maison
    int nbInserees = 0;

    while (query.next()) { // on parcours chaque ligne en réccupérant chaque champs et on les stocke dans la structure maison
        //on insére les maisons dans la table de hachage
        QString cle = query.value("cle").toString();  // on peux aussi faire hash dans la base
        QString type = query.value("type").toString();
        QString standing = query.value("standing").toString();
        int chambres = query.value("nb_chambres").toInt();
        int toilettes = query.value("nb_toilettes").toInt();
        float superficie = query.value("superficie").toFloat();
        int prix = query.value("prix").toInt();
        QString description = query.value("description").toString();

        table->inserer(cle, type, standing, chambres, toilettes, superficie, prix, description,  hachfunction);
         {

            nbInserees++;
        }
    }

    db.close();

    QMessageBox::information(this, "Succès",
        QString("%1 maisons chargées dans la table.").arg(nbInserees));
}


void MainWindow::on_Charger_triggered()
{
    QString chemin = QFileDialog::getOpenFileName(this, "Choisir une base", "", "Base SQLite (*.db *.bd)");  // boite de dialogue pour réccupérer le chemin d'accés de la base
    if (!chemin.isEmpty()) {
        chargerDepuisSqlite(chemin);
    }
}


void MainWindow::on_Taille_triggered()
{
    int taille = table->size(); // Appelle la fonction dans Tablehachage
        QMessageBox::information(this, "Taille de la table",
            "Nombre d'éléments dans la table : " + QString::number(taille));
}

void MainWindow::on_fonction_1_triggered()
{
    hachfunction = 1;
        QMessageBox::information(this, "Choix", "Fonction de hachage 1 sélectionnée.");
}


void MainWindow::on_fonction_2_triggered()
{
    hachfunction = 2;
        QMessageBox::information(this, "Choix", "Fonction de hachage 2 sélectionnée.");
}

void MainWindow::on_fonction_3_triggered()
{
    hachfunction = 3;
        QMessageBox::information(this, "Choix", "Fonction de hachage 3 sélectionnée.");
}


void MainWindow::on_sans_hach_triggered()
{
    hachfunction = 0;
    QMessageBox::information(this, "Choix", "Pas de fonction de hachage sélectionnée.");
}
/*
void MainWindow::on_Afficher_triggered()
{
    if (table == nullptr || table->size() == 0) { // On vérifie aussi si 'table' n'est pas null
        QMessageBox::warning(this, "Chargement nécessaire", "Veuillez d'abord charger la base de données via le menu 'Charger' avant d'afficher la table.");
        return; // Sort de la fonction si la table est vide ou non chargée
    }
   afficherDansTableView();
}

*/

void MainWindow::on_Vider_triggered()
{
    if (table == nullptr || table->size() == 0) { // On vérifie aussi si 'table' n'est pas null
        QMessageBox::warning(this, "Chargement nécessaire", "Veuillez d'abord charger la base de données via le menu 'Charger' avant de vider la table .");
        return; // Sort de la fonction si la table est vide ou non chargée
    }
    table->vider_table();
    QMessageBox::information(this, "Vidage", "Table vidée avec succès.");
}



void MainWindow::on_actionInserer_triggered()
{
    if (table == nullptr || table->size() == 0) { // On vérifie aussi si 'table' n'est pas null
        QMessageBox::warning(this, "Chargement nécessaire", "Veuillez d'abord charger la base de données via le menu 'Charger' avant d'insérer un éléments.");
        return; // Sort de la fonction si la table est vide ou non chargée
    }
    DialogInserer d(table, hachfunction, this);

    d.exec();
}

void MainWindow::on_actionSupprimer_triggered()
{
    if (table == nullptr || table->size() == 0) { // On vérifie aussi si 'table' n'est pas null
        QMessageBox::warning(this, "Chargement nécessaire", "Veuillez d'abord charger la base de données via le menu 'Charger' avant de supprimer un éléments.");
        return; // Sort de la fonction si la table est vide ou non chargée
    }
    DialogSupprimer d(table, hachfunction, this);
    d.exec();
}



void MainWindow::Sauvegarder(QString cheminBD) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "sauvegarde_connexion");
    db.setDatabaseName(cheminBD);

    if (!db.open()) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir la base pour sauvegarde.");
        return;
    }

    QSqlQuery query(db);

    // Supprimer toutes les anciennes données
    if (!query.exec("DELETE FROM maison")) {
        QMessageBox::warning(this, "Erreur", "Erreur lors du vidage de la base.");
        db.close();
        return;
    }

    // Parcourir toutes les cases de la table
    for (int i = 0; i < table->taille_max; ++i) {
        noeud* courant = table->tab[i];
        while (courant != nullptr) {
            const Maison& m = courant->element.maison;
            QString cle = courant->element.cle;

            query.prepare("INSERT INTO maison (cle, type, standing, nb_chambres, nb_toilettes, superficie, prix, description) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
            query.addBindValue(cle);
            query.addBindValue(m.type);
            query.addBindValue(m.standing);
            query.addBindValue(m.nb_chambre);
            query.addBindValue(m.nb_toilettes);
            query.addBindValue(m.superficie);
            query.addBindValue(m.prix);
            query.addBindValue(m.description);

            if (!query.exec()) {
                qDebug() << "Erreur d’insertion SQL :" << query.lastError();
            }

            courant = courant->suivant;
        }
    }

    db.close();
    QMessageBox::information(this, "Sauvegarde", "Table sauvegardée dans la base avec succès.");
}




void MainWindow::on_Sauvegarder_triggered()
{
    if (table == nullptr || table->size() == 0) { // On vérifie aussi si 'table' n'est pas null
        QMessageBox::warning(this, "Chargement nécessaire", "Veuillez d'abord charger la base de données via le menu 'Charger' avant d'insérer des éléments.");
        return; // Sort de la fonction si la table est vide ou non chargée
    }
    QString chemin = "C:/Users/HP/Desktop/HouseSeek/houseseek.db";
         Sauvegarder(chemin);
}

void MainWindow::on_actionRechercher_triggered()
{
    if (table == nullptr || table->size() == 0) { // On vérifie aussi si 'table' n'est pas null
        QMessageBox::warning(this, "Chargement nécessaire", "Veuillez d'abord charger la base de données via le menu 'Charger' avant d'insérer des éléments.");
        return; // Sort de la fonction si la table est vide ou non chargée
    }
    Dialogrechercher d(table, hachfunction,this);  // Crée une instance de la fenêtre
        d.exec();
}

void MainWindow::on_Hashtohash_triggered()
{
    QString hardcodedDatabasePath = "C:/Users/HP/Desktop/HouseSeek/houseseek.db";

    if (!QFile::exists(hardcodedDatabasePath)) {
        QMessageBox::warning(this, "Erreur de base de données", "Le fichier de base de données spécifié est introuvable : " + hardcodedDatabasePath);
        return; // Ne pas ouvrir la fenêtre si le fichier n'existe pas
    }

    ComparehashWindow *compareWindow = new ComparehashWindow(this);
    compareWindow->setDatabasePath(hardcodedDatabasePath); // Assurez-vous que databasePath est bien initialisé
    compareWindow->setAttribute(Qt::WA_DeleteOnClose);
    compareWindow->show();
}


void MainWindow::on_Afficher_triggered()
{
    // Avant d'ouvrir le dialogue, assurez-vous que 'table' est bien initialisé et non vide.
    if (table == nullptr || table->size() == 0) {
        QMessageBox::warning(this, "Erreur", "La table n'est pas initialisée. Veuillez charger la base de données d'abord.");
        return;
    }

    // Si votre table a une méthode 'isEmpty()' ou 'size()'
    // if (table->isEmpty()) { // Ou table->size() == 0 si vous avez une méthode size()
    //     QMessageBox::information(this, "Table Vide", "La table de hachage est vide. Chargez des données.");
    //     // return; // Décidez si vous voulez empêcher l'ouverture du dialogue si la table est vide
    // }

    // Crée une instance de votre Dialogafficher, en lui passant le pointeur vers votre table
    Dialogafficher *dialogAfficher = new Dialogafficher(table, this);
    // Assurez-vous que le dialogue sera supprimé automatiquement après avoir été fermé
    dialogAfficher->setAttribute(Qt::WA_DeleteOnClose);
    dialogAfficher->exec(); // Affiche le dialogue de manière modale
}
void MainWindow::on_tableMaison_clicked(const QModelIndex &index)
{
    // C'est ici que vous mettrez le code qui doit s'exécuter
    // lorsque l'utilisateur clique sur une cellule de votre QTableView ou QTableWidget.
    // Pour l'instant, vous pouvez laisser ce bloc vide, mais il doit exister.
    // Exemple (pour voir si ça fonctionne) :
    // QMessageBox::information(this, "Clic sur Table",
    //                          QString("Vous avez cliqué sur la ligne %1, colonne %2.")
    //                          .arg(index.row()).arg(index.column()));
}

// Dans mainwindow.cpp
#include "pageaide.h" // Assurez-vous que cette ligne est présente.
#include <QDebug>     // Ajoutez cette ligne si ce n'est pas déjà fait pour le débogage.

void MainWindow::on_Pageaide_triggered()
{


        qDebug() << "Slot on_pageaide_triggered() appelé. Tentative d'affichage de PageAide."; // Message de débogage

        // Crée une instance de votre page d'aide
        pageaide *helpPage = new pageaide(this); // 'this' définit MainWindow comme parent

        // Indique à Qt de supprimer cet objet lorsque la fenêtre est fermée.
        helpPage->setAttribute(Qt::WA_DeleteOnClose);

        // Affiche la fenêtre d'aide (non modale)
        helpPage->show();

        qDebug() << "PageAide::show() appelée. Vérifiez l'affichage."; // Autre message de débogage
}
