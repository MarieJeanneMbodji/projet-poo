/*#ifndef COMPAREHASH_H
#define COMPAREHASH_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QComboBox> // Inclure pour QComboBox si vous les utilisez dans l'UI et y accédez dans le .h
#include "TableHachage.h"
#include "qcustomplot.h"

namespace Ui {
class comparehash;
}

class ComparehashWindow : public QDialog
{
    Q_OBJECT // IMPORTANT : Macro Q_OBJECT DOIT ÊTRE PRÉSENT ET NON COMMENTÉ

public:
    explicit ComparehashWindow(QWidget *parent = nullptr);
    ~ComparehashWindow();

    void setDatabasePath(const QString &path); // Pour passer le chemin de la base de données
    void comparerTempsChargement(int func1_id, int func2_id); // Modifié pour prendre les IDs des fonctions

private slots:
    // Slot pour le bouton "Comparer"
    void on_pushButton_comparer_clicked();

    void on_comparehash_destroyed();

private:
    Ui::comparehash *ui;
    QString databasePath; // Pour stocker le chemin de la base de données

    void chargerAvecFonction(int hashFunction, qint64 &temps);
    QString getHashFunctionName(int id); // Fonction utilitaire pour obtenir le nom de la fonction
};

#endif // COMPAREHASH_H*/

#ifndef COMPAREHASH_H
#define COMPAREHASH_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QComboBox>
#include "TableHachage.h"
#include "qcustomplot.h"

// Dans comparehash.h

struct MaisonData {
    QString cle;
    QString type;
    QString standing;
    int nb_chambres;
    int nb_toilettes;
    float superficie;
    int prix;
    QString description;
};

// Optionnel, mais recommandé si vous utilisez MaisonData dans des QVariant ailleurs :
// Q_DECLARE_METATYPE(MaisonData)

namespace Ui {
class comparehash;
}

class ComparehashWindow : public QDialog
{
    Q_OBJECT // IMPORTANT : Macro Q_OBJECT DOIT ÊTRE PRÉSENT ET NON COMMENTÉ

public:
    explicit ComparehashWindow(QWidget *parent = nullptr);
    ~ComparehashWindow();

    void setDatabasePath(const QString &path);
    void comparerTempsChargement(int func1_id, int func2_id);

private slots:
    // Slot pour le bouton "Comparer"
    void on_pushButton_comparer_clicked();

    void on_comparehash_destroyed();

private:
    Ui::comparehash *ui;
    QString databasePath; // Pour stocker le chemin de la base de données

    void chargerAvecFonction(int hashFunction, qint64 &temps);
    QString getHashFunctionName(int id); // Fonction utilitaire pour obtenir le nom de la fonction
};

#endif // COMPAREHASH_H
