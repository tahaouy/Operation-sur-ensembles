#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENSEMBLES 10
#define MAX_NOM 20

typedef struct Noeud {
    int valeur;
    struct Noeud* suivant;
} Noeud;

typedef struct Ensemble {
    char nom[MAX_NOM];
    Noeud* tete;
    int taille;
} Ensemble;

void clearScreen() {
    system("cls");
}

Ensemble* creerEnsemble(char nom[MAX_NOM]) {
    Ensemble* nouvelEnsemble = (Ensemble*)malloc(sizeof(Ensemble));
    if (nouvelEnsemble == NULL) {
        printf("Erreur d'allocation de memoire\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nouvelEnsemble->nom, nom);
    nouvelEnsemble->tete = NULL;
    nouvelEnsemble->taille = 0;
    return nouvelEnsemble;
}

void ajouterElement(Ensemble* ensemble, int element) {
    Noeud* nouveauNoeud = (Noeud*)malloc(sizeof(Noeud));
    if (nouveauNoeud == NULL) {
        printf("Erreur d'allocation de memoire\n");
        exit(EXIT_FAILURE);
    }
    nouveauNoeud->valeur = element;
    nouveauNoeud->suivant = NULL;

    if (ensemble->tete == NULL) {
        ensemble->tete = nouveauNoeud;
    } else {
        Noeud* temp = ensemble->tete;
        while (temp->suivant != NULL) {
            temp = temp->suivant;
        }
        temp->suivant = nouveauNoeud;
    }
    ensemble->taille++;
}

void afficherEnsemble(Ensemble* ensemble) {
    printf("Ensemble %s : ", ensemble->nom);
    Noeud* temp = ensemble->tete;
    while (temp != NULL) {
        printf("%d ", temp->valeur);
        temp = temp->suivant;
    }
    printf("\n");
}

Ensemble* clonerEnsemble(Ensemble* ensemble) {
    Ensemble* copie = creerEnsemble(ensemble->nom);
    Noeud* temp = ensemble->tete;
    while (temp != NULL) {
        ajouterElement(copie, temp->valeur);
        temp = temp->suivant;
    }
    return copie;
}

int estDansEnsemble(Ensemble* ensemble, int element) {
    Noeud* temp = ensemble->tete;
    while (temp != NULL) {
        if (temp->valeur == element) {
            return 1;
        }
        temp = temp->suivant;
    }
    return 0;
}

Ensemble* unionEnsembles(Ensemble* ensemble1, Ensemble* ensemble2, int* nombreUnions) {
    char nomEnsemble[MAX_NOM];
    sprintf(nomEnsemble, "U%d", (*nombreUnions)++);
    Ensemble* resultat = creerEnsemble(nomEnsemble);

    Noeud* temp = ensemble1->tete;
    while (temp != NULL) {
        ajouterElement(resultat, temp->valeur);
        temp = temp->suivant;
    }

    temp = ensemble2->tete;
    while (temp != NULL) {
        if (!estDansEnsemble(resultat, temp->valeur)) {
            ajouterElement(resultat, temp->valeur);
        }
        temp = temp->suivant;
    }

    return resultat;
}

Ensemble* intersectionEnsembles(Ensemble* ensemble1, Ensemble* ensemble2, int* nombreIntersections) {
    char nomEnsemble[MAX_NOM];
    sprintf(nomEnsemble, "I%d", (*nombreIntersections)++);
    Ensemble* resultat = creerEnsemble(nomEnsemble);

    Noeud* temp = ensemble1->tete;
    while (temp != NULL) {
        if (estDansEnsemble(ensemble2, temp->valeur)) {
            ajouterElement(resultat, temp->valeur);
        }
        temp = temp->suivant;
    }

    return resultat;
}

Ensemble* differenceEnsembles(Ensemble* ensemble1, Ensemble* ensemble2, int* nombreDifferences) {
    char nomEnsemble[MAX_NOM];
    sprintf(nomEnsemble, "D%d", (*nombreDifferences)++);
    Ensemble* resultat = creerEnsemble(nomEnsemble);

    Noeud* temp = ensemble1->tete;
    while (temp != NULL) {
        if (!estDansEnsemble(ensemble2, temp->valeur)) {
            ajouterElement(resultat, temp->valeur);
        }
        temp = temp->suivant;
    }

    return resultat;
}

Ensemble* differenceSymetrique(Ensemble* ensemble1, Ensemble* ensemble2, int* nombreDifferencesSymetriques) {
    char nomEnsemble[MAX_NOM];
    sprintf(nomEnsemble, "S%d", (*nombreDifferencesSymetriques)++);
    Ensemble* resultat = creerEnsemble(nomEnsemble);

    Ensemble* diff1 = differenceEnsembles(ensemble1, ensemble2, nombreDifferencesSymetriques);
    Ensemble* diff2 = differenceEnsembles(ensemble2, ensemble1, nombreDifferencesSymetriques);

    Noeud* temp = diff1->tete;
    while (temp != NULL) {
        ajouterElement(resultat, temp->valeur);
        temp = temp->suivant;
    }

    temp = diff2->tete;
    while (temp != NULL) {
        if (!estDansEnsemble(resultat, temp->valeur)) {
            ajouterElement(resultat, temp->valeur);
        }
        temp = temp->suivant;
    }

    
    free(diff1);
    free(diff2);

    return resultat;
}

int main() {
    Ensemble* ensembles[MAX_ENSEMBLES];
    int nbEnsembles = 0;
    int nombreUnions = 1, nombreIntersections = 1, nombreDifferences = 1, nombreDifferencesSymetriques = 1;

    printf("===== Tableau de Bord des Operations d'Ensemble =====\n");
    int choix;
    do {
        printf("\nMenu :\n");
        printf("1. Creer un nouvel ensemble\n");
        printf("2. Afficher les ensembles existants\n");
        printf("3. Calculer une operation d'ensemble\n");
        printf("4. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                clearScreen();
                if (nbEnsembles >= MAX_ENSEMBLES) {
                    printf("Nombre maximal d'ensembles atteint.\n");
                    break;
                }
                printf("Entrez le nom de l'ensemble (max %d caracteres) : ", MAX_NOM - 1);
                char nom[MAX_NOM];
                scanf("%s", nom);
                ensembles[nbEnsembles] = creerEnsemble(nom);
                printf("Entrez le nombre de valeurs � ajouter � l'ensemble : ");
                int nombreValeurs;
                scanf("%d", &nombreValeurs);
                printf("Entrez les %d �l�ments de l'ensemble separes par des espaces : ", nombreValeurs);
                for (int i = 0; i < nombreValeurs; i++) {
                    int valeur;
                    scanf("%d", &valeur);
                    ajouterElement(ensembles[nbEnsembles], valeur);
                }
                nbEnsembles++;
                printf("nouvel ensemble cree \n");
                break;
            }
            case 2: {
                clearScreen();
                printf("Ensembles existants :\n");
                for (int i = 0; i < nbEnsembles; i++) {
                    afficherEnsemble(ensembles[i]);
                }
                break;
            }
            case 3: {
                clearScreen();
                if (nbEnsembles < 2) {
                    printf("Il faut au moins deux ensembles pour effectuer une op�ration.\n");
                    break;
                }
                printf("Choisissez le premier ensemble :\n");
                for (int i = 0; i < nbEnsembles; i++) {
                    printf("%d. Ensemble %s\n", i + 1, ensembles[i]->nom);
                }
                int choix1;
                scanf("%d", &choix1);
                printf("Choisissez le deuxieme ensemble :\n");
                for (int i = 0; i < nbEnsembles; i++) {
                    if (i + 1 != choix1) {
                        printf("%d. Ensemble %s\n", i + 1, ensembles[i]->nom);
                    }
                }
                int choix2;
                scanf("%d", &choix2);
                Ensemble* resultat;
                printf("Choisissez l'operation � effectuer :\n");
                printf("1. Union\n");
                printf("2. Intersection\n");
                printf("3. Difference\n");
                printf("4. Diff�rence sym�trique\n");
                int operation;
                scanf("%d", &operation);
                switch (operation) {
                    case 1:
                        resultat = unionEnsembles(ensembles[choix1 - 1], ensembles[choix2 - 1], &nombreUnions);
                        break;
                    case 2:
                        resultat = intersectionEnsembles(ensembles[choix1 - 1], ensembles[choix2 - 1], &nombreIntersections);
                        break;
                    case 3:
                        resultat = differenceEnsembles(ensembles[choix1 - 1], ensembles[choix2 - 1], &nombreDifferences);
                        break;
                    case 4:
                        resultat = differenceSymetrique(ensembles[choix1 - 1], ensembles[choix2 - 1], &nombreDifferencesSymetriques);
                        break;
                    default:
                        printf("Op�ration non valide.\n");
                        continue;
                }
                printf("R�sultat de l'op�ration : ");
                afficherEnsemble(resultat);
                ensembles[nbEnsembles++] = clonerEnsemble(resultat);
                break;
            }
            case 4:
                clearScreen();
                printf("Au revoir !\n");
                break;
            default:
                clearScreen();
                printf("Choix invalide. Veuillez saisir une option valide.\n");
                break;
        }
    } while (choix != 4);

    
    for (int i = 0; i < nbEnsembles; i++) {
        Noeud* temp = ensembles[i]->tete;
        while (temp != NULL) {
            Noeud* suivant = temp->suivant;
            free(temp);
            temp = suivant;
        }
        free(ensembles[i]);
    }

    return 0;
}

