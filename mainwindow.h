#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "comparehash.h"
#include "TableHachage.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void chargerDepuisSqlite(QString cheminBD= "C:Users/HP/Desktop/HouseSeek/houseseek.db");
    void Sauvegarder(QString cheminBD= "C:Users/HP/Desktop/HouseSeek/houseseek.db");
    void afficherDansTableView();
    void afficherImagesDepuisBase(QString cleMaison);

    
private slots:
    void on_Charger_triggered();
    void on_Taille_triggered();
    void on_fonction_1_triggered();
    void on_fonction_2_triggered();
    void on_fonction_3_triggered();
    void on_sans_hach_triggered();
    void on_Afficher_triggered();
    void on_Vider_triggered();
    void on_actionInserer_triggered();
    void on_actionSupprimer_triggered();


    void on_Sauvegarder_triggered();

    void on_actionRechercher_triggered();

    //void on_hashtohash_triggered(); // Le slot qui sera déclenché pour ouvrir la fenêtre de comparaison


    void on_Hashtohash_triggered();



    void on_tableMaison_clicked(const QModelIndex &index);

    void on_Pageaide_triggered();

private:
    Ui::MainWindow *ui;
    int hachfunction = 0; // 0 = aucune, 1 = hach1, 2 = hach2, 3 = hach3
    Tablehachage *table; //table logique pour stocker les maisons
};

#endif // MAINWINDOW_H
