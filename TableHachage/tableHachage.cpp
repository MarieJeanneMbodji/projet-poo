#include <iostream>
#include"tableHachage.h"
using namespace std;
TableHachage::TableHachage(int max){
    cout<<"appel du constructeur"<<endl;
    taille_max=max;
    nb_courant=0;
    tab=new noeud*[taille_max];
    if(!tab){
        cout<<"erreur d'allocation"<<endl;
        exit(0);
    }
    for(int i=0; i<taille_max;i++){
        tab[i]=NULL;
    }
}

 TableHachage:: ~TableHachage(){
    if(tab){
        for(int i=0; i<taille_max; i++){
            if(tab[i]!=NULL){
                while(tab[i]!=NULL){
                    noeud *temp=tab[i];
                    tab[i]=tab[i]->suiv;
                    delete temp;
                    temp=NULL;
                }
            }
        }
    }
 }  


int tableHachage::insersion(string cle,Maison& maison,int HashFunc){
    if(nb_courant>=taille_max){
        return -1;
    }
        int indexe;
        switch(HashFunc){
            case 1:
                indexe=Hach1(cle);
                break;
            case 2:
                indexe=Hach2(cle);
                break;
            case 3:
                indexe=Hach3(cle);
                break;
                default:
                return -1;
        }
        if(!contient(cle,indexe)){
                Noeud*nouv=new Noeud;
                nouv->Paire.cle=cle;
                nouv->Paire.maison=maison;
                nouv->suiv=tab[indexe];
                tab[indexe]=nouv;
                nb_courant++;
                return 1;
        }
        return 0;
 }

bool tableHachage::suppression(string cle,int HashFunc){
    if(!est_vide){
        switch(HashFunc){
            case 1:
                indice=Hach1(cle);
                break;
            case 2:
                indice=Hach2(cle);
                break;
            case 3:
                indice=Hach3(cle);
                break;
        }
        if(tab[indice]){
            if(tab[indice]->elmt.cle=cle){  //si l'elmt à supprimer se trouve à la tête de la liste
                new *temp=tab[indice];
                tab[indice]=tab[indice]->suiv;
                delete temp;
                if(tab[indexe]==NULL)
                nb_courant--;
            return true;
            }
        }
            else { //sinon on le cherche dans le reste de la liste
                Noeud *courant = tab[indice]->suiv, *precedent = tab[indice];
                while (courant != NULL && courant->element.cle != cle) {
                    precedent = courant;
                    courant = courant->suiv;
                }
                if (courant) {
                    precedent->suiv = courant->suiv;
                    delete courant;
                    return true;
                }
                return false;
            }
    }

 }

 bool tableHachage::contient(string cle,int HashFunc){
    int indice;
    switch(HashFunc){
        case 0:
                indice=0;
                break;
        case 1:
                indice=Hach1(cle);
                break;
         case 2:
                indice=Hach2(cle);
                break;
        case 3:
                indice=Hach3(cle);
                break;
    }
    noeud*nb_courant=tab[indice];
    while(courant!=NULL && courant->elmt.cle!=cle){
        courant=courant->suiv;
    }
    if(courant)
        return true;
    return false;
 }

 bool tableHachage::est_vide(){
    return nb_courant==0;
 }


 int tableHachage::size(){
    return nb_courant;
 }

int tableHachage:: hash1(string cle){
    int indexe=0;
    int hach=31;  //on prend un nbre premier pour réduire le risque de collision
    for(char c:cle){
        indexe=indexe*hach+c;
    }
    return indexe%taille_max;
}


int tableHachage::hash2(string cle){
    int hach=0;
    int poids=1;  //c'est la position des caractères
    for(char c:cle){
        hach=hach+c*poids; //on multiplie le code ascii par la position du caractere
        poids++;  //on incrémente pour le prochain caractere
    }
    return hach%taille_max;
}


int hash3(string cle){
    int hach=5381;
    for(char c:cle){
        hach=((hach<<5)+hach)+c;   // "<< 5"correspond à un décalage de 5 bits vers la gauche =2^5=32
    }
    return hach%taille_max;
}

