#include "TableHachage.h"
using namespace std;


Tablehachage::Tablehachage(int max){
    taille_max=max;
    nb_courant=0;
    tab=new noeud*[taille_max];
    if(!tab){
        exit(0);
    }
    for(int i=0;i < taille_max;i++){
        tab[i]=NULL;
    }
}
Tablehachage::~Tablehachage(){
    if(tab){
        for(int i=0;i < taille_max;i++){
            if(tab[i]!=NULL){
                while(tab[i]!=NULL){
                    noeud*temp=tab[i];
                    tab[i]=tab[i]->suivant;
                    delete temp;
                    temp=NULL;
                }
            }
        }
        delete [] tab;
    }
}
void Tablehachage::detruire_table(){
    for(int i = 0; i < taille_max; i++){
        if(tab[i] != NULL){
            while(tab[i] != NULL){
                noeud *temp = tab[i];
                tab[i] = tab[i]->suivant;
                delete temp;
                temp = NULL;
            }
        }
    }
}
int Tablehachage::inserer(QString cle, QString type,QString standing,int nb_chambre,int nb_toilettes,
                          float superficie,int prix,QString description, int Hashfunction){
    if(nb_courant >= taille_max){
        return -1;
    }
    int indexe;
    qDebug() << "Valeur reçue dans Hashfunction =" << Hashfunction;
    switch(Hashfunction){
        case 1:
            indexe=hach1(cle);
            break;
        case 2:
            indexe=hach2(cle);
            break;
        case 3:
            indexe=hach3(cle);
            break;
    case 0: indexe = 0; break; //  ajout du mode "sans hachage"
        default:
            qDebug() << "Valeur de fonction de hachage invalide : " << Hashfunction;
            return -1;
    }
    if(contient(cle, Hashfunction)){
        return 0; }
    noeud* nouv = new noeud;
    if (!nouv) {
        return -1;
    }
    nouv->element.cle = cle;
    nouv->element.maison.type=type;
    nouv->element.maison.standing=standing;
    nouv->element.maison.nb_chambre=nb_chambre;
    nouv->element.maison.nb_toilettes=nb_toilettes;
    nouv->element.maison.superficie=superficie;
    nouv->element.maison.prix=prix;
    nouv->element.maison.description=description;
    nouv->suivant = NULL;
    if (indexe < 0 || indexe >= taille_max) {
        qDebug() << "Erreur : index hors limites =" << indexe;
        return -1;
    }
    if(tab[indexe] == NULL){
        tab[indexe] = nouv;
    } else {
        noeud* courant = tab[indexe];
        while(courant->suivant != NULL){
            courant = courant->suivant;
        }
        courant->suivant = nouv;
    }
    nb_courant++;
    return 1;
}

bool Tablehachage::suppression(QString cle, int hachfunction) {
    if (nb_courant <= 0)
        return false;

    int indice = 0;
    switch (hachfunction) {
        case 1: indice = hach1(cle); break;
        case 2: indice = hach2(cle); break;
        case 3: indice = hach3(cle); break;
        default: return false; // cas non prévu
    }

    noeud* courant = tab[indice];
    noeud* precedent = nullptr;

    while (courant != nullptr) {
        if (courant->element.cle == cle) {
            if (precedent == nullptr) {
                // Suppression en tête de liste
                tab[indice] = courant->suivant;
            } else {
                // Suppression au milieu ou à la fin
                precedent->suivant = courant->suivant;
            }

            delete courant;
            nb_courant--;
            return true;
        }

        precedent = courant;
        courant = courant->suivant;
    }

    // clé non trouvée
    return false;
}

Maison Tablehachage::Get(QString cle,int hachfunction){
    int indexe;
    switch(hachfunction){
    case 1:
        indexe=hach1(cle);
        break;
    case 2:
        indexe=hach2(cle);
        break;
    case 3:
        indexe=hach3(cle);
        break;
    }
    noeud*courant=tab[indexe];
    while(courant!=NULL && courant->element.cle!=cle ){
            courant= courant->suivant;
    }
    Maison M;
    if(courant){
        M.type = courant->element.maison.type;
        M.standing = courant->element.maison.standing;
        M.nb_chambre = courant->element.maison.nb_chambre;
        M.nb_toilettes = courant->element.maison.nb_toilettes;
        M.superficie = courant->element.maison.superficie;
        M.prix = courant->element.maison.prix;
        M.description = courant->element.maison.description;
    }
    else{
        M.type = "undefined key";
        M.standing = "undefined key";
        M.nb_chambre = -1;
        M.nb_toilettes = -1;
        M.standing = -1;
        M.superficie = -1;
        M.prix = -1;
        M.description = "undefined key";
    }
    return M;
}

bool Tablehachage::contient(QString cle,int hachfunction){
    int indice;
        switch(hachfunction){
            case 1:
                    indice=hach1(cle);
                    break;
             case 2:
                    indice=hach2(cle);
                    break;
            case 3:
                    indice=hach3(cle);
                    break;
        default:
                    qDebug() << "Erreur: hachfunction invalide =" << hachfunction;
                    return false;
        }
        if (indice < 0 || indice >= taille_max) {
               qDebug() << "Erreur: index hors limite =" << indice;
               return false;
           }
        noeud*courant=tab[indice];
        while(courant!=NULL && courant->element.cle!=cle){
            courant=courant->suivant;
        }
        if(courant)
            return true;
        return false;
}
bool Tablehachage::est_vide(){
    return nb_courant==0;
}
int Tablehachage::size(){
    return nb_courant;
}
void Tablehachage::afficher(){

    for (int i = 0; i < taille_max; ++i) {
        qDebug() << "[" << i << "]";
        noeud* courant = tab[i];
        if (courant == NULL) {
            qDebug()<<"-> NULL";
        } else {
            while (courant != NULL) {
                qDebug()<<" -> (Cle: " << courant->element.cle
                     << ", Type: " << courant->element.maison.type
                     << ", Standing: " << courant->element.maison.standing
                     << ", Chambres: " << courant->element.maison.nb_chambre
                     << ", Toilettes: " << courant->element.maison.nb_toilettes
                     << ", Superficie: " << courant->element.maison.superficie << "m2"
                     << ", Prix: " << courant->element.maison.prix <<"frc"
                     << ", Description: " << courant->element.maison.description << ")";
                courant = courant->suivant;
            }
            qDebug()<<" -> NULL";
        }
    }
}




int Tablehachage::hach1(QString cle){
    int indexe=0;
        unsigned int hach=7;  //on prend un nbre premier pour réduire le risque de collision
        for(int i=1; i<=cle.length();i++){
            indexe=indexe*hach+(int)cle.at(i-1).toLatin1();
        }
        return (indexe % taille_max);
}

int Tablehachage::hach2(const QString &cle){
    int hach=0;
    int poids=1;  //c'est la position des caractères
     for(int i=1; i<=cle.length();i++){
        hach=hach+(int)cle.at(i-1).toLatin1()*poids;
        poids++;
    }
    return (hach % taille_max);
}

int Tablehachage::hach3(QString cle){
    unsigned int hach=5381;
    for(int i=1; i<=cle.length();i++){
        hach=((hach << 5) + hach) + (unsigned char)cle.at(i-1).toLatin1();
     }
        return (hach % taille_max);

}

void Tablehachage::vider_table() {
    for (int i = 0; i < taille_max; ++i) {
        noeud* courant = tab[i];
        while (courant != NULL) {
            noeud* temp = courant;
            courant = courant->suivant;
            delete temp;
        }
        tab[i] = NULL;
    }
    nb_courant = 0;
    qDebug() << "Table vidée avec succès.";
}

