#ifndef TABLEHACHAGE_H
#define TABLEHACHAGE_H
#define N 1051 ;
#include<iostream>
#include<string>
using namespace std;

struct Maison{
    string type;
    string standing;
    int nb_chambres;
    int nb_toilettes;
    int nb_etages;
    float superficie;
    string description;
   // string <photo>;  //lien vers la photo 
};


struct Paire{
    string cle;  //valeure fournie par la fonction de hachage 
    Maison maison;
};

struct Noeud{   //pour mettre les elmts dans une liste chainée en cas de colision
    Paire elmt;
    Noeud*suiv;
};


class TableHachage{
    int taille_max;
    int nb_courant;
    Noeud**tab;
    public:
    TableHachage(int);  //constructeur
    ~TableHachage();    //destructeur
    insersion(string cle,Maison& maison,int HachFunc);  //permet d'ajouter une paire cle-valeur
    Maison get(string,int);     //retourne la valeure associée à une cle ie-une maison
    bool suppression(string,int);  //suprime une paire
    bool contient(string,int);  //retourne vrai si la cle se trouve dans la table, faux sinon. sachant que la valeure depend de la fonction de hachage
    int size();     //retourne le nmbre de paires entrée dans la tabble 
    bool est_vide();  //retourne vrai si la table est vide, faux sinon
    int Hach1(string);
    int Hach2(string);
    int Hach3(string);

};

#endif //TABLEHACHAGE
