#ifndef TABLEHACHAGE_H
#define TABLEHACHAGE_H
#include <QString>
#include <QDebug>
#include <cmath>
using namespace std;
struct Maison{
    QString type;
    QString standing;
    int nb_chambre;
    int nb_toilettes;
    float superficie;
    double prix;
    QString description;
};

struct paire{
    QString cle;  //valeure de hachage fournit par notre fonction de hachage
    Maison maison;
};
struct noeud{
    paire element;
    struct noeud*suivant;
};

class Tablehachage{
public:
    int taille_max;
    int nb_courant;
    noeud**tab;
public:
    Tablehachage(int);
    ~Tablehachage();
    int inserer(QString ,QString , QString ,int ,int , float ,int ,QString , int );
    bool suppression(QString,int);
    Maison Get(QString,int);
    bool contient(QString,int);
    int size();
    bool est_vide();
    int hach1(QString);
    int hach2(const QString&);
    int hach3(QString);
    void detruire_table();
    void afficher();
    void vider_table();


};


#endif // TABLEHACHAGE_H
