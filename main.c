#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Tache
{
    char description[150];
    int priorite;
    char statut[30];
}Tache;
typedef struct Noeud{
    Tache data;
    struct Noeud* Next;
    struct Noeud* Prev;
}Noeud;
typedef struct Noeud* list;

typedef struct File {
    list Tete;
    list Queue;
    int taille;
} File;

typedef File* file;

typedef struct Pile {
    Tache tache_supprimee;
    struct Pile* Next;
}Pile;
typedef struct Pile* pile;
list newTache(Tache T)
{
    list NewT = (list)malloc(sizeof(Noeud));
    if(NewT == NULL){
        printf("Erreur d'allocation.\n");
        return NULL;
    }
    else{
        NewT->data = T;
        NewT->Next=NULL;
        NewT->Prev=NULL;

    }
    return NewT;
}
list AjouTache(list head, Tache T)
{
    list NewT = newTache(T);
    if (NewT == NULL)
        return head;

    if (head == NULL)
        return NewT;

         if (T.priorite < head->data.priorite) {
        NewT->Next = head;
        head->Prev = NewT;
        return NewT;
         }
     list temp = head;
     while(temp->Next!=NULL && temp->Next->data.priorite < T.priorite)
        temp=temp->Next;
     NewT->Next=temp->Next;
     NewT->Prev=temp;
     if(temp->Next!=NULL)
        temp->Next->Prev=NewT;
     temp->Next=NewT;
     return head;
}
pile Empiler(pile P, Tache T)
{
    pile NewE = (pile)malloc(sizeof(struct Pile));
    if (NewE == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        return P;
    }


    NewE->tache_supprimee = T;
    NewE->Next = P;

    return NewE;
}

Tache Depiler(pile *P)
{
    Tache tache_vide = {"", -1, ""};
     if(*P==NULL)
       {
            printf("la pile est vide ");
                return tache_vide;
       }

     Tache T = (*P)->tache_supprimee;
     pile temp = *P;
     *P = (*P)->Next;
     free(temp);

     return T;
}
list Supprim(list head, char descr[], pile *P)
{
    if(head==NULL)
        return NULL;
     list ptr = head;
    if(strcmp(head->data.description,descr)==0){
        *P=Empiler(*P,head->data);
        head=head->Next;
        if (head != NULL)
            head->Prev = NULL;
        free(ptr);
        return head;
    }
    list temp=head;
    while(temp != NULL && strcmp(temp->Next->data.description,descr)!=0)
        temp=temp->Next;
    if(temp->Next == NULL)
        return head;

         *P=Empiler(*P,temp->Next->data);

    list tmp = temp->Next;
    temp->Next = tmp->Next;
    if (tmp->Next != NULL)
        tmp->Next->Prev = temp;

    free(tmp);


         return head;
}
list Modifier(list head, char descr[], Tache T)
{
    if(head == NULL)
        return NULL;

    list temp = head;
    while(temp != NULL && strcmp(temp->data.description, descr) != 0)
        temp = temp->Next;

    if (temp == NULL)
        return head;

    Tache t_modifie = T;


    if(temp->Prev != NULL)
        temp->Prev->Next = temp->Next;
    else
        head = temp->Next;

    if(temp->Next != NULL)
        temp->Next->Prev = temp->Prev;

    free(temp);


    head = AjouTache(head, t_modifie);

    return head;
}

Tache Recherche(list head, char descr[])
{
   Tache tache_vide = {"", -1, ""};
    list temp = head;
    while(temp!=NULL && strcmp(temp->data.description,descr)!= 0)
        temp = temp->Next;
    if(temp == NULL){
       printf("Tache introuvable!\n");
       return tache_vide;
    }
    return temp->data;
}
list annulerSuppr(list head, pile* P) {
    if (*P == NULL) {
        printf("Aucune tâche à annuler (pile vide).\n");
        return head;
    }

    Tache t = Depiler(P);

    head = AjouTache(head, t);

    return head;
}

void afficherListe(list head) {
    list l = head;
    if (l == NULL) {
        printf("Liste vide.\n");
        return;
    }

    printf("Liste des taches :\n");
    while (l != NULL) {
        printf("  - Description : %s\n", l->data.description);
        printf("    Priorite    : %d\n", l->data.priorite);
        printf("    Statut      : %s\n\n", l->data.statut);
        l = l->Next;
    }
}
file creerFile() {
    file f = (file)malloc(sizeof(File));
    f->Tete = f->Queue = NULL;
    f->taille = 0;
    return f;
}
void Enfiler(file f, Tache T) {
    list n = (list)malloc(sizeof(Noeud));
    n->data = T;
    n->Next = NULL;
    n->Prev = NULL;

    if (f->Queue == NULL) {
        f->Tete = f->Queue = n;
    } else {
        f->Queue->Next = n;
        n->Prev = f->Queue;
        f->Queue = n;
    }
    f->taille++;
}
Tache Defiler(file f) {
    Tache tache_vide = {"", -1, ""};

    if (f->Tete == NULL)
        return tache_vide;

    list temp = f->Tete;
    Tache t = temp->data;

    f->Tete = f->Tete->Next;
    if (f->Tete)
        f->Tete->Prev = NULL;
    else
        f->Queue = NULL;

    free(temp);
    f->taille--;

    return t;
}
file trierParPriorite(file f) {
    if (f == NULL || f->taille <= 1)
        return f;

    file f_triee = creerFile();

    while (f->taille > 0) {
        Tache t = Defiler(f);


        list n = (list)malloc(sizeof(Noeud));
        n->data = t;
        n->Next = NULL;
        n->Prev = NULL;

        if (f_triee->Tete == NULL) {
            f_triee->Tete = f_triee->Queue = n;
        } else {
            list current = f_triee->Tete;
            while (current != NULL && current->data.priorite <= t.priorite)
                current = current->Next;

            if (current == f_triee->Tete) {

                n->Next = f_triee->Tete;
                f_triee->Tete->Prev = n;
                f_triee->Tete = n;
            } else if (current == NULL) {

                n->Prev = f_triee->Queue;
                f_triee->Queue->Next = n;
                f_triee->Queue = n;
            } else {

                n->Next = current;
                n->Prev = current->Prev;
                current->Prev->Next = n;
                current->Prev = n;
            }
        }
        f_triee->taille++;
    }

    free(f);
    return f_triee;
}

void libererListe(list l) {
    list temp;
    while (l != NULL) {
        temp = l;
        l = l->Next;
        free(temp);
    }
}
void libererPile(pile p) {
    pile temp;
    while (p != NULL) {
        temp = p;
        p = p->Next;
        free(temp);
    }
}
void libererFile(file f) {
    if (f == NULL)
        return;

    list temp;
    while (f->Tete != NULL) {
        temp = f->Tete;
        f->Tete = f->Tete->Next;
        free(temp);
    }

    free(f);
}


int main() {
    list maListe = NULL;
    pile P = NULL;
    file fileTaches = creerFile();

    int choix;
    Tache t;
    char description[150];

    do {
        printf("\n*** MENU ***\n");
        printf("1. Ajouter une tache\n");
        printf("2. Supprimer une tache\n");
        printf("3. Modifier une tache\n");
        printf("4. Rechercher une tache\n");
        printf("5. Annuler la derniere suppression\n");
        printf("6. Afficher la liste des taches\n");
        printf("7. Trier les taches par priorite (file)\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("Description : ");
                scanf(" %[^\n]s", t.description);

                printf("Priorite : ");
                scanf("%d", &t.priorite);

                printf("Statut : ");
                scanf(" %[^\n]s", t.statut);

                maListe = AjouTache(maListe, t);
                printf("Tache ajoutee.\n");
                break;

            case 2:
                printf("Description de la tache a supprimer : ");
                scanf(" %[^\n]s", description);

                maListe = Supprim(maListe, description, &P);
                printf("Tache supprimee (si trouvee).\n");
                break;

            case 3:
                printf("Description de la tache a modifier : ");
                scanf(" %[^\n]s", description);

                printf("Nouvelle description : ");
                scanf(" %[^\n]s", t.description);

                printf("Nouvelle priorite : ");
                scanf("%d", &t.priorite);

                printf("Nouveau statut : ");
                scanf("%s", t.statut);

                maListe = Modifier(maListe, description, t);
                printf("Tache modifiee.\n");
                break;

            case 4: {
                printf("Description de la tache a rechercher : ");
                scanf(" %[^\n]s", description);

                Tache trouvee = Recherche(maListe, description);
                if (trouvee.priorite != -1) {
                    printf("Tâche trouvee :\n");
                    printf("  - Description : %s\n", trouvee.description);
                    printf("  - Priorite    : %d\n", trouvee.priorite);
                    printf("  - Statut      : %s\n", trouvee.statut);
                }
                break;
            }

            case 5:
                maListe = annulerSuppr(maListe, &P);
                break;

            case 6:
                afficherListe(maListe);
                break;

            case 7: {
                fileTaches = creerFile();
                list temp = maListe;
                while (temp != NULL) {
                    Enfiler(fileTaches, temp->data);
                    temp = temp->Next;
                }
                fileTaches = trierParPriorite(fileTaches);
                printf("Tâches triees par priorité :\n");
                afficherListe(fileTaches->Tete);
                libererFile(fileTaches);
                break;
            }

            case 0:
                printf("Programme fermé.\n");
                break;

            default:
                printf("Choix invalide.\n");
        }

    } while (choix != 0);

    libererListe(maListe);
    libererPile(P);

    return 0;
}

