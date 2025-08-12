/*#include "comparehash.h"
#include "ui_comparehash.h" // Généré par Qt
#include <QElapsedTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QFont> // Inclus pour QFont (pour setLabelFont)
#include <QMessageBox> // Pour afficher des messages d'erreur si besoin
#include "TableHachage.h" // Assurez-vous que le chemin est correct
#include <QSharedPointer>
//#include <QCPAxisTickerText>
#include <QDateTime> // Assurez-vous d'inclure QDateTime
#include <Qt>
#include <QtCore/QtGlobal>
#include <QFlags>
#include <algorithm> // Déjà ajouté, mais assurez-vous qu'il est présent
#include <limits>    // AJOUTÉ : Pour std::numeric_limits, pour trouver min/max robustement




ComparehashWindow::ComparehashWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::comparehash)
{
    ui->setupUi(this);

    // Initialisation des QComboBox (à faire si vous avez ajouté les QComboBox dans l'UI)
    ui->comboBox->addItem("Hachage 1");
    ui->comboBox->addItem("Hachage 2");
    ui->comboBox->addItem("Hachage 3");
    ui->comboBox2->addItem("Hachage 1");
    ui->comboBox2->addItem("Hachage 2");
    ui->comboBox2->addItem("Hachage 3");

    // Connexion du bouton "Comparer" à son slot
    // Utilisez le nom de votre bouton défini dans Qt Designer, par exemple "pushButton_comparer"
    connect(ui->pushButton_comparer, &QPushButton::clicked, this, &ComparehashWindow::on_pushButton_comparer_clicked);

    // Initialisation de QCustomPlot (peut être fait ici ou dans le slot du bouton)
    // C'est mieux de laisser la logique de tracé dans comparerTempsChargement
    // et d'appeler comparerTempsChargement via le bouton "Comparer"
}

ComparehashWindow::~ComparehashWindow()
{
    delete ui;
}

void ComparehashWindow::setDatabasePath(const QString &path)
{
    databasePath = path;
}

QString ComparehashWindow::getHashFunctionName(int id) {
    switch (id) {
        case 1: return "Hachage 1";
        case 2: return "Hachage 2";
        case 3: return "Hachage 3";
        default: return "Inconnu";
    }
}

#include <QDateTime> // Assurez-vous que cette ligne est présente en haut de votre .cpp
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QElapsedTimer>
// #include <QCoreApplication> // Non utilisé dans cette version, pas besoin de l'inclure.

// ... (vos autres includes)

void ComparehashWindow::chargerAvecFonction(int hashFunction, qint64 &temps)
{
    // Crée une nouvelle table de hachage à chaque appel pour assurer un état propre
    Tablehachage table(10001);

    QElapsedTimer timer;
    timer.start();

    // --- Gestion de la connexion à la base de données ---
    // Utilisez un nom de connexion FIXE pour cette fonction.
    // Cela évite de créer/détruire de multiples connexions qui posent problème.
    // IMPORTANT : Assurez-vous que ce nom ("HashComparisonConnection") n'est PAS utilisé par d'autres
    // parties de votre application (comme dans MainWindow) simultanément.
    QString connectionName = "HashComparisonConnection";

    QSqlDatabase db;
    // Vérifier si la connexion existe déjà dans le pool de Qt
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
        if (db.isOpen()) {
            db.close(); // Fermez-la si elle est encore ouverte suite à une exécution précédente
            // Pour des versions Qt très anciennes, un petit délai ou processEvents pourrait aider ici,
            // mais évitons-le pour l'instant car cela peut introduire d'autres problèmes (réentrance).
        }
    } else {
        // La connexion n'existe pas encore, ajoutez-la
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    }

    db.setDatabaseName(databasePath); // Chemin vers votre fichier .db

    // Tenter d'ouvrir la base de données
    if (!db.open()) {
        qDebug() << "Erreur ouverture DB:" << db.lastError().text();
        QMessageBox::critical(this, "Erreur BD", "Impossible d'ouvrir la base de données: " + db.lastError().text());
        temps = -1;
        // Pas besoin de removeDatabase ici, car la connexion n'a pas été établie avec succès
        return;
    }

    // --- Exécution de la requête SQL et insertion dans la table de hachage ---
    QSqlQuery query(db); // Associe la requête à la connexion 'db'
    // ATTENTION : Corrigez ici si votre table s'appelle 'maison' et non 'maisons'
    if (!query.exec("SELECT cle, type, standing, nb_chambres, nb_toilettes, superficie, prix, description FROM maison")) {
        qDebug() << "Erreur SELECT:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur BD", "Erreur lors de la lecture des données: " + query.lastError().text());
        temps = -1;
        db.close(); // Ferme la base de données avant de quitter
        // Ne PAS appeler removeDatabase ici, car nous voulons réutiliser cette connexion nommée
        return;
    }

    int nbInserees = 0;
    while (query.next()) {
        QString cle = query.value("cle").toString();
        QString type = query.value("type").toString();
        QString standing = query.value("standing").toString();
        int chambres = query.value("nb_chambres").toInt();
        int toilettes = query.value("nb_toilettes").toInt();
        float superficie = query.value("superficie").toFloat();
        int prix = query.value("prix").toInt();
        QString description = query.value("description").toString();

        int resultatInsertion = table.inserer(cle, type, standing, chambres, toilettes, superficie, prix, description, hashFunction);
        if (resultatInsertion == 1) {
            nbInserees++;
        } else if (resultatInsertion == -1) {
            qDebug() << "Erreur d'insertion pour la clé" << cle << "avec la fonction" << hashFunction;
        }
    }

    temps = timer.elapsed(); // Enregistre le temps écoulé
    db.close(); // Ferme la base de données pour la libérer
    // Ne PAS appeler removeDatabase ici. La connexion nommée sera réutilisée la prochaine fois.
}



// ... (vos autres includes)

void ComparehashWindow::comparerTempsChargement(int func1_id, int func2_id)
{
    QCustomPlot *plot = ui->customPlot;

    // Effacer tous les graphes existants pour un nouveau tracé
    plot->clearGraphs();

    // --- Données pour la Fonction de Hachage 1 ---
    QVector<double> x1_trials = {1, 2, 3}; // Les points X (par exemple, Essai 1, Essai 2, Essai 3)
    QVector<double> y1_times;             // Les temps de chargement pour la fonction 1

    // Exécuter 3 essais pour la Fonction de Hachage 1
    for (int i = 0; i < 3; ++i) {
        qint64 temp_t;
        chargerAvecFonction(func1_id, temp_t);
        y1_times << (double)temp_t; // Convertir en double pour QVector<double>
    }

    // --- Données pour la Fonction de Hachage 2 ---
    QVector<double> x2_trials = {1, 2, 3}; // Les points X (Essai 1, Essai 2, Essai 3)
    QVector<double> y2_times;             // Les temps de chargement pour la fonction 2

    // Exécuter 3 essais pour la Fonction de Hachage 2
    for (int i = 0; i < 3; ++i) {
        qint64 temp_t;
        chargerAvecFonction(func2_id, temp_t);
        y2_times << (double)temp_t; // Convertir en double pour QVector<double>
    }

    // --- Configuration du Graphe 1 (Fonction de Hachage 1) ---
    plot->addGraph(); // Ajoute un nouveau graphe (sera graph(0))
    plot->graph(0)->setData(x1_trials, y1_times);
    plot->graph(0)->setName(getHashFunctionName(func1_id)); // Nom pour la légende
    plot->graph(0)->setPen(QPen(Qt::blue, 2));              // Ligne bleue épaisse
    plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle); // Points en cercles

    // --- Configuration du Graphe 2 (Fonction de Hachage 2) ---
    plot->addGraph(); // Ajoute un nouveau graphe (sera graph(1))
    plot->graph(1)->setData(x2_trials, y2_times);
    plot->graph(1)->setName(getHashFunctionName(func2_id)); // Nom pour la légende
    plot->graph(1)->setPen(QPen(Qt::red, 2));               // Ligne rouge épaisse
    plot->graph(1)->setScatterStyle(QCPScatterStyle::ssSquare); // Points en carrés

    // --- Configuration des Axes ---
    plot->xAxis->setLabel("Numéro de l'Essai");
    plot->yAxis->setLabel("Temps de Chargement (ms)");

    // Définir la plage de l'axe X
    plot->xAxis->setRange(0.5, 3.5); // Pour inclure l'espace autour des points 1, 2, 3

    // Définir la plage de l'axe Y dynamiquement (solution pour QVector::append)
    double min_val = std::numeric_limits<double>::max();
    double max_val = std::numeric_limits<double>::min();

    // Parcourir y1_times
    for (double val : y1_times) {
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }
    // Parcourir y2_times
    for (double val : y2_times) {
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }

    if (max_val > min_val) {
        plot->yAxis->setRange(min_val * 0.8, max_val * 1.2); // Marge de 20%
    } else if (max_val != std::numeric_limits<double>::min()) { // Cas où min_val == max_val (toutes les valeurs sont les mêmes)
        plot->yAxis->setRange(max_val * 0.8, max_val * 1.2);
    } else {
        plot->yAxis->setRange(0, 10); // Plage par défaut si aucune donnée
    }

    // --- Configuration de la Légende (AUCUN CONTRÔLE DE POSITIONNEMENT PROGRAMMATIQUE) ---
    // Toutes les méthodes de positionnement ont été signalées comme manquantes.
    // La légende sera visible, mais son emplacement sera celui par défaut de votre version de QCustomPlot.
    plot->legend->setVisible(true);
    plot->legend->setBrush(QBrush(QColor(255, 255, 255, 200))); // Fond semi-transparent blanc
    plot->legend->setBorderPen(QPen(Qt::black, 1));             // Bordure noire
    plot->legend->setTextColor(Qt::black);                      // Couleur du texte en noir

    // Styles des labels des axes
    plot->xAxis->setLabelFont(QFont(font().family(), 10));
    plot->yAxis->setLabelFont(QFont(font().family(), 10));

    // Mettre à jour le graphique pour afficher les changements
    plot->replot();
}

// Slot appelé lorsque le bouton "Comparer" est cliqué
void ComparehashWindow::on_pushButton_comparer_clicked()
{
    // Récupérer les index sélectionnés dans les QComboBox (les index commencent à 0)
    // Si vos IDs de fonction sont 1, 2, 3, ajoutez 1 à l'index
    int func1_id = ui->comboBox->currentIndex() + 1;
    int func2_id = ui->comboBox2->currentIndex() + 1;

    // Vérifier que les fonctions sélectionnées sont différentes (ou non, selon votre logique)
    if (func1_id == func2_id) {
        QMessageBox::warning(this, "Sélection Invalide", "Veuillez sélectionner deux fonctions de hachage différentes.");
        return;
    }

    // Appeler la fonction de comparaison avec les IDs sélectionnés
    comparerTempsChargement(func1_id, func2_id);
}

void ComparehashWindow::on_comparehash_destroyed()
{

}
*/

#include "comparehash.h"
#include "ui_comparehash.h" // Généré par Qt
#include <QElapsedTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QFont> // Inclus pour QFont (pour setLabelFont)
#include <QMessageBox> // Pour afficher des messages d'erreur si besoin
#include "TableHachage.h" // Assurez-vous que le chemin est correct
#include <QSharedPointer>
//#include <QCPAxisTickerText>
#include <QDateTime> // Assurez-vous d'inclure QDateTime
#include <Qt>
#include <QtCore/QtGlobal>
#include <QFlags>
#include <algorithm> // Déjà ajouté, mais assurez-vous qu'il est présent
#include <limits>    // AJOUTÉ : Pour std::numeric_limits, pour trouver min/max robustement




ComparehashWindow::ComparehashWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::comparehash)
{
    ui->setupUi(this);

    // Initialisation des QComboBox (à faire si vous avez ajouté les QComboBox dans l'UI)
    ui->comboBox->addItem("Hachage 1");
    ui->comboBox->addItem("Hachage 2");
    ui->comboBox->addItem("Hachage 3");
    ui->comboBox->addItem("sans hachage");
    ui->comboBox2->addItem("Hachage 1");
    ui->comboBox2->addItem("Hachage 2");
    ui->comboBox2->addItem("Hachage 3");
    ui->comboBox2->addItem("sans hachage");


    // Connexion du bouton "Comparer" à son slot
    // Utilisez le nom de votre bouton défini dans Qt Designer, par exemple "pushButton_comparer"
    connect(ui->pushButton_comparer, &QPushButton::clicked, this, &ComparehashWindow::on_pushButton_comparer_clicked);

    // Initialisation de QCustomPlot (peut être fait ici ou dans le slot du bouton)
    // C'est mieux de laisser la logique de tracé dans comparerTempsChargement
    // et d'appeler comparerTempsChargement via le bouton "Comparer"
}

ComparehashWindow::~ComparehashWindow()
{
    delete ui;
}

void ComparehashWindow::setDatabasePath(const QString &path)
{
    databasePath = path;
}

// Fonction utilitaire pour obtenir le nom de la fonction de hachage
// Dans comparehash.cpp

QString ComparehashWindow::getHashFunctionName(int id)
{
    switch (id) {
        case 1: return "Fonction Hachage 1";
        case 2: return "Fonction Hachage 2";
        case 3: return "Fonction Hachage 3";
        case 4: return "Sans Hachage"; // AJOUTEZ CETTE LIGNE
        default: return "Inconnu";
    }
}

#include <QDateTime> // Assurez-vous que cette ligne est présente en haut de votre .cpp
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QElapsedTimer>
// #include <QCoreApplication> // Non utilisé dans cette version, pas besoin de l'inclure.

// ... (vos autres includes)


// Dans comparehash.cpp

void ComparehashWindow::chargerAvecFonction(int func_id, qint64 &temps)
{
    QElapsedTimer timer;
    timer.start();

    // Gestion de la connexion à la base de données (gardez ce code comme il est)
    QString connectionName = "HashComparisonConnection";

    QSqlDatabase db;
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
        if (db.isOpen()) {
            db.close();
        }
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    }

    db.setDatabaseName(databasePath);

    if (!db.open()) {
        qDebug() << "Erreur ouverture DB:" << db.lastError().text();
        QMessageBox::critical(this, "Erreur BD", "Impossible d'ouvrir la base de données: " + db.lastError().text());
        temps = -1;
        return;
    }

    QSqlQuery query(db);
    // TRÈS IMPORTANT : Vérifiez que le nom de la table est bien 'maison' (au singulier)
    if (!query.exec("SELECT cle, type, standing, nb_chambres, nb_toilettes, superficie, prix, description FROM maison")) {
        qDebug() << "Erreur SELECT:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur BD", "Erreur lors de la lecture des données: " + query.lastError().text());
        temps = -1;
        db.close();
        return;
    }

    // --- Logique d'insertion en fonction du type de fonction ---
    if (func_id >= 1 && func_id <= 3) { // Pour les fonctions de hachage 1, 2, 3
        Tablehachage table(10001); // Crée une nouvelle table de hachage pour chaque test

        while (query.next()) {
            QString cle = query.value("cle").toString();
            QString type = query.value("type").toString();
            QString standing = query.value("standing").toString();
            int chambres = query.value("nb_chambres").toInt();
            int toilettes = query.value("nb_toilettes").toInt();
            float superficie = query.value("superficie").toFloat();
            int prix = query.value("prix").toInt();
            QString description = query.value("description").toString();

            table.inserer(cle, type, standing, chambres, toilettes, superficie, prix, description, func_id);
        }
    }
    // CE BLOC EST CELUI POUR "SANS HACHAGE" - À VÉRIFIER OU AJOUTER
    else if (func_id == 4) { // Cas : "Sans Hachage"
        QList<MaisonData> maisonList; // Utilise une QList pour stocker les maisons

        while (query.next()) {
            MaisonData m; // Crée une nouvelle instance de la structure pour chaque ligne
            m.cle = query.value("cle").toString();
            m.type = query.value("type").toString();
            m.standing = query.value("standing").toString();
            m.nb_chambres = query.value("nb_chambres").toInt();
            m.nb_toilettes = query.value("nb_toilettes").toInt();
            m.superficie = query.value("superficie").toFloat();
            m.prix = query.value("prix").toInt();
            m.description = query.value("description").toString();
            maisonList.append(m); // Ajoute simplement à la fin de la liste
        }
        // Pour "Sans Hachage", le temps est mesuré par le chargement de la DB dans la QList.
        // Aucune opération de hachage n'est effectuée ici.
    } else { // Si l'ID de fonction n'est pas reconnu (comme si func_id était 0, par exemple)
        qDebug() << "ID de fonction non reconnu:" << func_id;
        QMessageBox::warning(this, "Erreur Fonction", "ID de fonction de hachage non reconnu.");
        temps = -1; // Indique une erreur ou un ID non géré
        db.close();
        return;
    }

    temps = timer.elapsed(); // Enregistre le temps écoulé
    db.close(); // Ferme la base de données
}



void ComparehashWindow::comparerTempsChargement(int func1_id, int func2_id)
{
    QCustomPlot *plot = ui->customPlot;

    // Effacer tous les graphes existants pour un nouveau tracé
    plot->clearGraphs();

    // --- Données pour la Fonction de Hachage 1 ---
    QVector<double> x1_trials = {1, 2, 3}; // Les points X (par exemple, Essai 1, Essai 2, Essai 3)
    QVector<double> y1_times;             // Les temps de chargement pour la fonction 1

    // Exécuter 3 essais pour la Fonction de Hachage 1
    for (int i = 0; i < 3; ++i) {
        qint64 temp_t;
        chargerAvecFonction(func1_id, temp_t);
        y1_times << (double)temp_t; // Convertir en double pour QVector<double>
    }

    // --- Données pour la Fonction de Hachage 2 ---
    QVector<double> x2_trials = {1, 2, 3}; // Les points X (Essai 1, Essai 2, Essai 3)
    QVector<double> y2_times;             // Les temps de chargement pour la fonction 2

    // Exécuter 3 essais pour la Fonction de Hachage 2
    for (int i = 0; i < 3; ++i) {
        qint64 temp_t;
        chargerAvecFonction(func2_id, temp_t);
        y2_times << (double)temp_t; // Convertir en double pour QVector<double>
    }

    // --- Configuration du Graphe 1 (Fonction de Hachage 1) ---
    plot->addGraph(); // Ajoute un nouveau graphe (sera graph(0))
    plot->graph(0)->setData(x1_trials, y1_times);
    plot->graph(0)->setName(getHashFunctionName(func1_id)); // Nom pour la légende
    plot->graph(0)->setPen(QPen(Qt::blue, 2));              // Ligne bleue épaisse
    plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle); // Points en cercles

    // --- Configuration du Graphe 2 (Fonction de Hachage 2) ---
    plot->addGraph(); // Ajoute un nouveau graphe (sera graph(1))
    plot->graph(1)->setData(x2_trials, y2_times);
    plot->graph(1)->setName(getHashFunctionName(func2_id)); // Nom pour la légende
    plot->graph(1)->setPen(QPen(Qt::red, 2));               // Ligne rouge épaisse
    plot->graph(1)->setScatterStyle(QCPScatterStyle::ssSquare); // Points en carrés

    // --- Configuration des Axes ---
    plot->xAxis->setLabel("Numéro de l'Essai");
    plot->yAxis->setLabel("Temps de Chargement (ms)");

    // Définir la plage de l'axe X
    plot->xAxis->setRange(0.5, 3.5); // Pour inclure l'espace autour des points 1, 2, 3

    // Définir la plage de l'axe Y dynamiquement (solution pour QVector::append)
    double min_val = std::numeric_limits<double>::max();
    double max_val = std::numeric_limits<double>::min();

    // Parcourir y1_times
    for (double val : y1_times) {
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }
    // Parcourir y2_times
    for (double val : y2_times) {
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }

    if (max_val > min_val) {
        plot->yAxis->setRange(min_val * 0.8, max_val * 1.2); // Marge de 20%
    } else if (max_val != std::numeric_limits<double>::min()) { // Cas où min_val == max_val (toutes les valeurs sont les mêmes)
        plot->yAxis->setRange(max_val * 0.8, max_val * 1.2);
    } else {
        plot->yAxis->setRange(0, 10); // Plage par défaut si aucune donnée
    }

    // --- Configuration de la Légende (AUCUN CONTRÔLE DE POSITIONNEMENT PROGRAMMATIQUE) ---
    // Toutes les méthodes de positionnement ont été signalées comme manquantes.
    // La légende sera visible, mais son emplacement sera celui par défaut de votre version de QCustomPlot.
    plot->legend->setVisible(true);
    plot->legend->setBrush(QBrush(QColor(255, 255, 255, 200))); // Fond semi-transparent blanc
    plot->legend->setBorderPen(QPen(Qt::black, 1));             // Bordure noire
    plot->legend->setTextColor(Qt::black);                      // Couleur du texte en noir

    // Styles des labels des axes
    plot->xAxis->setLabelFont(QFont(font().family(), 10));
    plot->yAxis->setLabelFont(QFont(font().family(), 10));

    // Mettre à jour le graphique pour afficher les changements
    plot->replot();
}

// Slot appelé lorsque le bouton "Comparer" est cliqué
void ComparehashWindow::on_pushButton_comparer_clicked()
{
    // Récupérer les index sélectionnés dans les QComboBox (les index commencent à 0)
    // Si vos IDs de fonction sont 1, 2, 3, ajoutez 1 à l'index
    int func1_id = ui->comboBox->currentIndex() + 1;
    int func2_id = ui->comboBox2->currentIndex() + 1;

    // Vérifier que les fonctions sélectionnées sont différentes (ou non, selon votre logique)
    if (func1_id == func2_id) {
        QMessageBox::warning(this, "Sélection Invalide", "Veuillez sélectionner deux fonctions de hachage différentes.");
        return;
    }

    // Appeler la fonction de comparaison avec les IDs sélectionnés
    comparerTempsChargement(func1_id, func2_id);
}

void ComparehashWindow::on_comparehash_destroyed()
{

}
