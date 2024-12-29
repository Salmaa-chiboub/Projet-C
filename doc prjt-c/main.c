#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <locale.h>
#include <conio.h>


#define MAX_USERS 100
#define NAME_LENGTH 30
#define SURNAME_LENGTH 30
#define USERNAME_LENGTH 30
#define PASSWORD_LENGTH 30
#define EMAIL_LENGTH 30
#define PHONE_LENGTH 15
#define MAX_USERNAME_LENGTH 30
#define MAX_RESERVATIONS 100
#define ID_LENGTH 10
#define FILEuser "users.bin"
#define FILEadmin "admin.bin"
#define FILEvoyageInterne "voyageInterne.bin"
#define FILEvoyageExterne "voyageExterne.bin"
#define FILENAME "compagnies.bin"




//****************************** */
// Flight information structures
typedef struct {
    int jour;
    int mois;
    int annee;
} Date;
typedef struct {

    int mois;
    int annee;
} Date2;

typedef struct {
    int heure;
    int minute;
} Heure;

// Structure for internal flights (unchanged)
typedef struct {
    char id[ID_LENGTH + 1];  // Unique ID for each voyage
    char aeroport_depart[30];
    char aeroport_arrive[30];
    Date date_voyage;
    Heure heure_depart;
    Heure heure_arrivee;
    int nb_place;
    char compagnie[30];
    int prix;
    Heure duree_vol;
} VoyageInterne;

// Structure for external flights (new)
typedef struct {
    char id[ID_LENGTH + 1];  // Unique ID for each voyage
    char aeroport_depart[30];
    char aeroport_arrive[30];
    char pays_depart[30];
    char pays_arrivee[30];
    Date date_voyage;
    Heure heure_depart;
    Heure heure_arrivee;
    int nb_place;
    char compagnie[30];
    int prix;
    char classe[20];
    Heure duree_vol;
} VoyageExterne;

// Structure to store user information
typedef struct {
    char nom[NAME_LENGTH];
    char prenom[SURNAME_LENGTH];
    char email[EMAIL_LENGTH];
    char telephone[PHONE_LENGTH];
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
} User;

typedef struct {
    char numero_carte[20];
    char titulaire_carte[50];
    Date2 date_expiration;
    char code_securite[5];
} InformationsPaiement;
typedef struct {
    char username[50];
    char voyage_id[30];
    int nb_places_reservees;
    Date date_reservation;
    float montant_paye;
} Reservation;

typedef struct {
    char nom[30];  // Nom de la compagnie
    char pays[30]; // Pays d'origine de la compagnie
    int annee_creation; // Année de création de la compagnie
    int nb_avions;  // Nombre d'avions dans la flotte
} Compagnie;
typedef struct {
    char id_contrat[20];  // Identifiant du contrat
    char date_debut[15];  // Date de début du contrat (format: dd/mm/yyyy)
    char date_fin[15];    // Date de fin du contrat (format: dd/mm/yyyy)
    float montant;        // Montant du contrat
} Contrat;

typedef struct {
    char nom[50];
    char prenom[50];
    char identifiant[30];  // Identifiant unique de l'employé
    char mot_de_passe[30];  // Mot de passe de l'employé
    char poste[50];  // Poste de l'employé dans l'agence
    char ville[50];  // Ville où travaille l'employé
    char email[100]; // Email de l'employé
    char telephone[20]; // Numéro de téléphone de l'employé
} Employe;








// Fonction de positionnement du curseur dans la console
void gotoXY(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position;
    position.X = x + 23;
    position.Y = y;
    SetConsoleCursorPosition(hConsole, position);
}

// Fonction pour changer la couleur du texte dans la console
void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Fonction pour afficher un trait horizontal (trait "gras") au-dessus et en dessous du titre
void drawBoldLine(int x_centre, int y_centre, int color) {
    int width = 50; // Longueur fixe du trait gras
    gotoXY(x_centre - width / 2, y_centre);
    SetColor(color);
    for (int i = 0; i < width; i++) printf("=");
    SetColor(7); // Retour à la couleur par défaut (blanc)
}

// Fonction pour afficher un titre avec des traits horizontaux au-dessus et en dessous
void drawTitleWithBoldLine(int x_centre, int y_centre, const char *title, int color) {
    int width = 50; // Longueur fixe du trait gras

    // Dessiner le trait au-dessus du titre
    drawBoldLine(x_centre, y_centre, color);

    // Afficher le titre en jaune
    gotoXY(x_centre - strlen(title) / 2, y_centre + 1);
    SetColor(14);  // Jaune pour le titre
    printf("%s", title);

    // Dessiner le trait en dessous du titre
    drawBoldLine(x_centre, y_centre + 2, color);

    SetColor(7);  // Retour à la couleur par défaut (blanc)
}

// Fonction pour centrer le message de pause
void centerSystemPause(int x_centre, int y_centre) {
    gotoXY(x_centre - 20, y_centre);
    system("pause");
}

// Fonction de choix dans le menu
int choose_item(char **items, char *title, int x_centre, int y_centre) {
    int selected_option = 0;
    int key;

    // Compter les éléments de la liste
    int item_count = 0;
    while (items[item_count] != NULL) {
        item_count++;
    }

    while (1) {
        system("cls");

        // Afficher le titre avec les traits
        drawTitleWithBoldLine(x_centre, y_centre, title, 14);  // Utilisation de la couleur jaune pour les traits

        // Afficher les options du menu avec la sélection mise en évidence
        for (int i = 0; i < item_count; i++) {
            gotoXY(x_centre, y_centre + 4 + i * 2);  // Ajouter un espacement entre les options
            if (i == selected_option) {
                SetColor(10);  // Vert pour l'option sélectionnée
                printf("> %s\n", items[i]);
                SetColor(7);  // Retour à la couleur par défaut
            } else {
                printf("%s\n", items[i]);
            }
        }

        // Obtenir l'entrée utilisateur pour la navigation
        key = _getch();
        if (key == 224) { // Touche de flèche (haut ou bas)
            key = _getch();
            if (key == 72 && selected_option > 0) { // Flèche haut
                selected_option--;
            } else if (key == 80 && selected_option < item_count - 1) { // Flèche bas
                selected_option++;
            }
        } else if (key == 13) { // Touche Entrée
            return selected_option;
        }
    }
}











//recuperer la date
// Fonction pour obtenir la date du jour
void obtenir_date_du_jour(Date *date) {
    time_t maintenant = time(NULL); // Obtenir le timestamp actuel
    struct tm *temps_local = localtime(&maintenant); // Convertir en temps local

    // Remplir la structure Date
    date->jour = temps_local->tm_mday;
    date->mois = temps_local->tm_mon + 1; // Les mois vont de 0 (janvier) à 11 (décembre)
    date->annee = temps_local->tm_year + 1900; // Année depuis 1900, donc on ajoute 1900
}



//partie utilisateur
void hide_input(char* input) {
    char ch;
    int i = 0;
    while (1) {
        ch = _getch();  // Lire un caractère sans l'afficher
        if (ch == 13) {  // Touche Enter
            break;
        } else if (ch == 8) {  // Touche Backspace
            if (i > 0) {
                i--;
                printf("\b \b");  // Effacer l'astérisque précédent
            }
        } else {
            input[i] = ch;
            i++;
            printf("*");  // Afficher un astérisque
        }
    }
    input[i] = '\0';  // Terminer la chaîne de caractères
}

// Signup function
char* signUp() {
    static char username[USERNAME_LENGTH];
    char confirmermotdepasse[30];
    User newUser;
    FILE *file = fopen(FILEuser, "ab+");

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return NULL;
    }

    printf("Inscription:\n");

    printf("Entrez votre nom: ");
    scanf("%s", newUser.nom);
    printf("Entrez votre prénom: ");
    scanf("%s", newUser.prenom);
    printf("Entrez votre email: ");
    scanf("%s", newUser.email);
    printf("Entrez votre numéro de téléphone: ");
    scanf("%s", newUser.telephone);
    printf("Entrez un nom d'utilisateur: ");
    scanf("%s", newUser.username);

    // Vérifier si le nom d'utilisateur existe déjà
    User tempUser;
    rewind(file);
    while (fread(&tempUser, sizeof(User), 1, file)) {
        if (strcmp(tempUser.username, newUser.username) == 0) {
            printf("Ce nom d'utilisateur est déjà pris.\n");
            fclose(file);
            return NULL;
        }
    }

    // Masquer la saisie du mot de passe
    printf("Entrez un mot de passe: ");
    hide_input(newUser.password);

    // Masquer la confirmation du mot de passe
    printf("\nConfirmer le mot de passe: ");
    hide_input(confirmermotdepasse);

    // Vérifier que les mots de passe correspondent
    while (strcmp(newUser.password, confirmermotdepasse) != 0) {
        printf("\nConfirmation de mot de passe incorrect, veuillez essayer !\n");
        printf("Entrez un mot de passe: ");
        hide_input(newUser.password);
        printf("\nConfirmer le mot de passe: ");
        hide_input(confirmermotdepasse);
    }

    // Écrire les informations de l'utilisateur dans le fichier
    fwrite(&newUser, sizeof(User), 1, file);
    fclose(file);

    strcpy(username, newUser.username);
    printf("\nInscription réussie ! Bienvenue %s %s\n", newUser.nom, newUser.prenom);

    return username;
}






char * login() {
    system("cls");
    char mot_de_passe[30];
    int i = 0;
    static char username[USERNAME_LENGTH];  // Déclaration statique pour pouvoir le retourner
    User user;
    FILE *file = fopen(FILEuser, "rb");

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return NULL;  // Retourner NULL si le fichier ne s'ouvre pas
    }

    // Affichage stylisé du titre
    int x_centre = 40;
    int y_centre = 5;
    drawTitleWithBoldLine(x_centre, y_centre, "Connexion User", 14);

     // Cadre pour le champ username
    
    gotoXY(x_centre - 22, y_centre + 3);
    SetColor(14);
    printf(" ------------------------------------------");
    gotoXY(x_centre - 22, y_centre + 4);
    printf(" | Username                               |");
    gotoXY(x_centre - 22, y_centre + 5);
    printf(" ------------------------------------------");


    // Demander l'identifiant
    gotoXY(x_centre - 7, y_centre + 4);
    SetColor(7);
    scanf("%29s", username);

    // Cadre pour le champ mot de passe
    SetColor(14);
    gotoXY(x_centre - 22, y_centre + 6);
    printf(" ------------------------------------------");
    gotoXY(x_centre - 22, y_centre + 7);
    printf(" | Password                               |");
    gotoXY(x_centre - 22, y_centre + 8);
    printf(" ------------------------------------------");


     // Demander le mot de passe
    gotoXY(x_centre - 7, y_centre + 7);
    SetColor(7);
    while (1) {
        char ch = _getch();  // Lire un caractère sans l'afficher
        if (ch == 13) {  // Si l'utilisateur appuie sur Enter (13 en ASCII)
            mot_de_passe[i] = '\0';  // Terminer la chaîne de caractères
            break;
        } else if (ch == 8 && i > 0) {  // Si c'est la touche Backspace
            printf("\b \b");  // Effacer le dernier caractère affiché
            i--;
        } else if (i < 20) {  // Limiter la saisie à 20 caractères
            mot_de_passe[i] = ch;  // Ajouter le caractère au mot de passe
            i++;
            printf("*");  // Afficher un astérisque pour masquer l'entrée
        }
    }
    printf("\n");

    // Ouvrir le fichier des users en mode lecture binaire
    FILE *fichier = fopen("users.bin", "rb");
    if (fichier == NULL) {
        gotoXY(x_centre - 20, y_centre + 10);
        SetColor(12);  // Rouge pour l'erreur
        printf("Erreur d'ouverture du fichier des users.\n");
        centerSystemPause(x_centre, y_centre + 11);
        SetColor(7);   // Retour à la couleur par défaut
        return NULL;
    }




    int trouve = 0;
    while (fread(&user, sizeof(User), 1, fichier)) {
        // Vérifier si l'identifiant et le mot de passe correspondent
        if (strcmp(user.username, username) == 0 && strcmp(user.password, mot_de_passe) == 0) {
            fclose(fichier);
            gotoXY(x_centre - 20, y_centre + 10);
            SetColor(10);  // Vert pour la réussite
            printf("Connexion réussie.\n");
            centerSystemPause(x_centre, y_centre + 11);
            SetColor(7);
            return username;  // Connexion réussie
        }
    }

    fclose(fichier);
    gotoXY(x_centre - 20, y_centre + 10);
    SetColor(12);  // Rouge pour l'erreur
    printf("Identifiant ou mot de passe incorrect.\n");
    centerSystemPause(x_centre, y_centre + 11);
    SetColor(7);   // Retour à la couleur par défaut
    return 0;  // Connexion échouée
}






// faire une reservation;
void saisirCriteresRecherche(char *depart, char *arrive, Date *date, int *nb_places) {
    printf("Entrez l'aéroport de départ : ");
    scanf("%s", depart);
    printf("Entrez l'aéroport d'arrivée : ");
    scanf("%s", arrive);
    printf("Entrez la date de voyage (jour mois année) : ");
    scanf("%d %d %d", &date->jour, &date->mois, &date->annee);
    printf("Entrez le nombre de places à réserver : ");
    scanf("%d", nb_places);
}

// Display available voyages with the updated structure
int afficherVoyagesDisponiblesInternes(FILE *fichier, const char *depart, const char *arrive, Date date, int nb_places) {
    VoyageInterne voyage;
    int index = 1;
    int voyage_trouve = 0;

    printf("\nVoyages disponibles :\n");
    while (fread(&voyage, sizeof(VoyageInterne), 1, fichier)) {
        if (strcmp(voyage.aeroport_depart, depart) == 0 &&
            strcmp(voyage.aeroport_arrive, arrive) == 0 &&
            voyage.date_voyage.jour == date.jour &&
            voyage.date_voyage.mois == date.mois &&
            voyage.date_voyage.annee == date.annee &&
            voyage.nb_place >= nb_places) {

            voyage_trouve = 1;
            printf("%d. ID: %s | Départ: %s | Arrivée: %s | Date: %02d/%02d/%04d | "
                   "Heure départ: %02d:%02d | Heure arrivée: %02d:%02d | Compagnie: %s | "
                   "Prix: %d | Places disponibles: %d | Durée du vol: %02d:%02d\n",
                   index, voyage.id, voyage.aeroport_depart, voyage.aeroport_arrive,
                   voyage.date_voyage.jour, voyage.date_voyage.mois, voyage.date_voyage.annee,
                   voyage.heure_depart.heure, voyage.heure_depart.minute,
                   voyage.heure_arrivee.heure, voyage.heure_arrivee.minute,
                   voyage.compagnie, voyage.prix, voyage.nb_place,
                   voyage.duree_vol.heure, voyage.duree_vol.minute);
            index++;
        }
    }

    return voyage_trouve ? index - 1 : 0;
}


int demanderChoixVoyage(int index_max) {
    int choix_voyage;
    printf("\nVoulez-vous selectionner un voyage pour la reservation ?\n");
    printf("Entez 0 pour Quitter\n");
    printf("Entrez le numéro du voyage pour Continuer\n");
    printf("Entez votre choix : ");
    scanf("%d", &choix_voyage);

    if (choix_voyage < 0 || choix_voyage > index_max) {
        printf("Choix invalide.\n");
        return -1;  // Annulation ou mauvais choix
    }

    return choix_voyage;
}

// Fonction pour traiter le paiement avec saisie et validation
int traiterPaiement(const char *username, float montant_total) {
    InformationsPaiement payement;

    printf("\n=== Processus de Paiement ===\n");
    printf("Utilisateur: %s\n", username);
    printf("Montant total à payer : %.2f\n", montant_total);

    // Demander les informations de paiement
    printf("Entrez le numéro de la carte de crédit (16 chiffres): ");
    scanf("%s", payement.numero_carte);
    printf("Entrez la date d'expiration de la carte (MM/AA) : ");
    scanf("%d %d", &payement.date_expiration.mois,&payement.date_expiration.annee);

    printf("Entrez le titulaire de la carte : ");
    scanf("%s", payement.titulaire_carte);
    printf("Code de sécurité (CVV, 3 chiffres) : ");
    scanf("%4s", payement.code_securite);



    if (strlen(payement.numero_carte) != 16 || strlen(payement.code_securite) != 3) {
        printf("Erreur : Les informations de paiement sont invalides.\n");
        return 0; // Paiement invalide
    }


    // Simuler un paiement réussi
    printf("\nPaiement de %.2f par la carte %s réussi !\n", montant_total, payement.numero_carte);
    printf("Merci pour votre confiance!\n");

    // Dans un système réel, vous pourriez également vérifier que le paiement est approuvé via un API ou un service bancaire.

    return 1;  // Paiement réussi
}

void enregistrerReservationInterne(const char *username, VoyageInterne voyage, int nb_places, float montant_paye) {
    FILE *reservation_file = fopen("reservationsInterne.bin", "ab");
    if (reservation_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier des réservations internes.\n");
        return;
    }

    Reservation reservation;
    strcpy(reservation.username, username);
    strcpy(reservation.voyage_id, voyage.id);
    reservation.nb_places_reservees = nb_places;
    reservation.montant_paye = montant_paye;  // Enregistrement du montant payé

    // Simule la date actuelle
    obtenir_date_du_jour(&reservation.date_reservation);

    fwrite(&reservation, sizeof(Reservation), 1, reservation_file);
    fclose(reservation_file);

    printf("Les informations de réservation et le montant payé ont été enregistrées pour le voyage interne.\n");
}


void confirmerReservationInterne(FILE *fichier, const char *username, int choix_voyage, int nb_places,
                                  char *aeroport_depart, char *aeroport_arrive, Date date) {
    VoyageInterne voyage;
    int index = 1;

    rewind(fichier);
    while (fread(&voyage, sizeof(VoyageInterne), 1, fichier)) {
        if (strcmp(voyage.aeroport_depart, aeroport_depart) == 0 &&
            strcmp(voyage.aeroport_arrive, aeroport_arrive) == 0 &&
            voyage.date_voyage.jour == date.jour &&
            voyage.date_voyage.mois == date.mois &&
            voyage.date_voyage.annee == date.annee &&
            voyage.nb_place >= nb_places) {

            if (index == choix_voyage) {
                // Étape 1 : Traitement du paiement
                float montant_total =  voyage.prix * nb_places ; // Calcul du montant total à payer

                int paiement_reussi = traiterPaiement(username, montant_total);
                if (!paiement_reussi) {
                    printf("Paiement annulé. Réservation non effectuée.\n");
                    return;
                }

                // Étape 2 : Mise à jour des places disponibles
                voyage.nb_place -= nb_places;
                fseek(fichier, -sizeof(VoyageInterne), SEEK_CUR);
                fwrite(&voyage, sizeof(VoyageInterne), 1, fichier);

                // Étape 3 : Enregistrement de la réservation avec le montant payé
                enregistrerReservationInterne(username, voyage, nb_places, montant_total);

                printf("Réservation confirmée pour %d place(s) pour le voyage %s.\n", nb_places, voyage.id);
                break;
            }
            index++;
        }
    }
}




void ReserverVoyageInterne(const char *username) {
    char depart[30], arrive[30];
    Date date;
    int nb_places;
    int index;
    int choix_voyage;

    FILE *fichier = fopen("voyageInterne.bin", "rb+");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier des voyages internes.\n");
        return;
    }

    // Saisie des critères de recherche
    saisirCriteresRecherche(depart, arrive, &date, &nb_places);

    // Affichage des voyages disponibles
    index = afficherVoyagesDisponiblesInternes(fichier, depart, arrive, date, nb_places);
    if (index == 0) {
        printf("Aucun voyage disponible ne correspond à vos critères.\n");
    } else {
        do {
            choix_voyage = demanderChoixVoyage(index);
            if (choix_voyage == 0) {
                printf("Réservation annulée.\n");
                fclose(fichier);
                return;
            }
        } while (choix_voyage == -1);

        // Confirmation et enregistrement de la réservation
        confirmerReservationInterne(fichier, username, choix_voyage, nb_places, depart, arrive, date);
    }

    fclose(fichier);
}






// Vyage Externes ;
// saisir des caracteres ;
void saisirCriteresRechercheExterne(char *pays_depart, char *pays_arrive, char *aeroport_depart, char *aeroport_arrive, Date *date, int *nb_places) {
    printf("Entrez le pays de départ : ");
    scanf("%s", pays_depart);
    printf("Entrez le pays d'arrivée : ");
    scanf("%s", pays_arrive);
    printf("Entrez l'aéroport de départ : ");
    scanf("%s", aeroport_depart);
    printf("Entrez l'aéroport d'arrivée : ");
    scanf("%s", aeroport_arrive);
    printf("Entrez la date de voyage (jour mois année) : ");
    scanf("%d %d %d", &date->jour, &date->mois, &date->annee);
    printf("Entrez le nombre de places à réserver : ");
    scanf("%d", nb_places);
}

//affichage des lignes disponibles

int afficherVoyagesDisponiblesExterne(FILE *fichier, const char *pays_depart, const char *pays_arrivee,
                                      const char *aeroport_depart, const char *aeroport_arrive, Date date, int nb_places) {
    VoyageExterne voyage;
    int index = 1;
    int voyage_trouve = 0;

    printf("\nVoyages disponibles :\n");
    while (fread(&voyage, sizeof(VoyageExterne), 1, fichier)) {
        if (strcmp(voyage.pays_depart, pays_depart) == 0 &&
            strcmp(voyage.pays_arrivee, pays_arrivee) == 0 &&
            strcmp(voyage.aeroport_depart, aeroport_depart) == 0 &&
            strcmp(voyage.aeroport_arrive, aeroport_arrive) == 0 &&
            voyage.date_voyage.jour == date.jour &&
            voyage.date_voyage.mois == date.mois &&
            voyage.date_voyage.annee == date.annee &&
            voyage.nb_place >= nb_places) {

            voyage_trouve = 1;
            printf("%d. ID: %s | Pays départ : %s | Pays arrivée : %s | Départ : %s | Arrivée : %s | "
                   "Date : %02d/%02d/%04d | Heure départ : %02d:%02d | Heure arrivée : %02d:%02d | "
                   "Compagnie : %s | Classe : %s | Prix : %d | Places disponibles : %d | Durée du vol : %02d:%02d\n",
                   index, voyage.id, voyage.pays_depart, voyage.pays_arrivee, voyage.aeroport_depart, voyage.aeroport_arrive,
                   voyage.date_voyage.jour, voyage.date_voyage.mois, voyage.date_voyage.annee,
                   voyage.heure_depart.heure, voyage.heure_depart.minute,
                   voyage.heure_arrivee.heure, voyage.heure_arrivee.minute,
                   voyage.compagnie, voyage.classe, voyage.prix, voyage.nb_place,
                   voyage.duree_vol.heure, voyage.duree_vol.minute);
            index++;
        }
    }

    return voyage_trouve ? index - 1 : 0;
}



// Fonction pour enregistrer la réservation avec informations de paiement
void enregistrerReservationExterne(const char *username, VoyageExterne voyage, int nb_places, float montant_paye) {
    FILE *reservation_file = fopen("reservationsExterne.bin", "ab");
    if (reservation_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier des réservations externes.\n");
        return;
    }

    Reservation reservation;
    strcpy(reservation.username, username);
    strcpy(reservation.voyage_id, voyage.id);
    reservation.nb_places_reservees = nb_places;
    reservation.montant_paye = montant_paye;  // Enregistrement du montant payé

    // Simule la date actuelle
    obtenir_date_du_jour(&reservation.date_reservation);

    fwrite(&reservation, sizeof(Reservation), 1, reservation_file);
    fclose(reservation_file);

    printf("Les informations de réservation et le montant payé ont été enregistrées pour le voyage externe.\n");
}


// confirmation de reservation

void confirmerReservationExterne(FILE *fichier, const char *username, int choix_voyage, int nb_places,
                                  char *pays_depart, char *pays_arrive, char *aeroport_depart, char *aeroport_arrive, Date date) {
    VoyageExterne voyage;
    int index = 1;

    rewind(fichier);
    while (fread(&voyage, sizeof(VoyageExterne), 1, fichier)) {
        if (strcmp(voyage.pays_depart, pays_depart) == 0 &&
            strcmp(voyage.pays_arrivee, pays_arrive) == 0 &&
            strcmp(voyage.aeroport_depart, aeroport_depart) == 0 &&
            strcmp(voyage.aeroport_arrive, aeroport_arrive) == 0 &&
            voyage.date_voyage.jour == date.jour &&
            voyage.date_voyage.mois == date.mois &&
            voyage.date_voyage.annee == date.annee &&
            voyage.nb_place >= nb_places) {

            if (index == choix_voyage) {
                // Étape 1 : Traitement du paiement
                float montant_total =  voyage.prix * nb_places ; // Calcul du montant total à payer
                printf("Le montant total à payer est : %.2f\n", montant_total);
                int paiement_reussi = traiterPaiement(username, montant_total);
                if (!paiement_reussi) {
                    printf("Paiement annulé. Réservation non effectuée.\n");
                    return;
                }

                // Étape 2 : Mise à jour des places disponibles
                voyage.nb_place -= nb_places;
                fseek(fichier, -sizeof(VoyageExterne), SEEK_CUR);
                fwrite(&voyage, sizeof(VoyageExterne), 1, fichier);

                // Étape 3 : Enregistrement de la réservation avec le montant payé
                enregistrerReservationExterne(username, voyage, nb_places, montant_total);

                printf("Réservation confirmée pour %d place(s) pour le voyage %s.\n", nb_places, voyage.id);
                break;
            }
            index++;
        }
    }
}



//reservetion de voyages externe ;
void ReserverVoyageExterne(const char *username) {
    char pays_depart[30], pays_arrive[30];
    char aeroport_depart[30], aeroport_arrive[30];
    Date date;
    int nb_places;
    int index;
    int choix_voyage;

    FILE *fichier = fopen("voyageExterne.bin", "rb+");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier des voyages externes.\n");
        return;
    }

    // Saisie des critères de recherche
    saisirCriteresRechercheExterne(pays_depart, pays_arrive, aeroport_depart, aeroport_arrive, &date, &nb_places);

    // Affichage des voyages disponibles
    index = afficherVoyagesDisponiblesExterne(fichier, pays_depart, pays_arrive, aeroport_depart, aeroport_arrive, date, nb_places);
    if (index == 0) {
        printf("Aucun voyage disponible ne correspond à vos critères.\n");
    } else {
        do {
            choix_voyage = demanderChoixVoyage(index);
            if (choix_voyage == 0) {
                printf("Réservation annulée.\n");
                fclose(fichier);
                return;
            }
        } while (choix_voyage == -1);

        // Confirmation et enregistrement de la réservation
        confirmerReservationExterne(fichier, username, choix_voyage, nb_places, pays_depart, pays_arrive, aeroport_depart, aeroport_arrive, date);
    }

    fclose(fichier);
}
// reservation de voyage;

void ReserverVoyage(char* username){
    int c;
    do{
        printf("voulez-vous reserver:\n");
        printf("1. Voyage interne\n");
        printf("2. Voyage Externe\n");
        printf("3. QUITTER\n");
        printf("Entrez votre choix : ");
        scanf("%d",&c);
        switch (c)
        {
        case 1:
            ReserverVoyageInterne(username);
            break;
        case 2:
            ReserverVoyageExterne(username);
            break;
        case 3:
            break;
        default:
            printf("Choix invalide!\n");
            break;
        }

    }while (c!=3);

}

void afficherReservationsInternes(const char* username) {
    FILE *reservation_file = fopen("reservationsInterne.bin", "rb");
    if (reservation_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier des réservations internes.\n");
        return;
    }

    Reservation reservation;

    printf("\nVos réservations internes :\n");
    printf("--------------------------\n");

    while (fread(&reservation, sizeof(Reservation), 1, reservation_file) == 1) {
        if (strcmp(reservation.username, username) == 0) {
            // Afficher les détails de la réservation
            printf("ID du voyage : %s\n", reservation.voyage_id);
            printf("Nombre de places réservées : %d\n", reservation.nb_places_reservees);
            printf("Montant payé : %.2f\n", reservation.montant_paye);
            printf("Date de réservation : %02d/%02d/%04d\n",
                   reservation.date_reservation.jour,
                   reservation.date_reservation.mois,
                   reservation.date_reservation.annee);
            printf("--------------------------\n");
        }
    }

    fclose(reservation_file);
}

void afficherReservationsExternes(const char* username) {
    FILE *reservation_file = fopen("reservationsExterne.bin", "rb");
    if (reservation_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier des réservations internes.\n");
        return;
    }

    Reservation reservation;

    printf("\nVos réservations Externes :\n");
    printf("--------------------------\n");

    while (fread(&reservation, sizeof(Reservation), 1, reservation_file) == 1) {
        if (strcmp(reservation.username, username) == 0) {
            // Afficher les détails de la réservation
            printf("ID du voyage : %s\n", reservation.voyage_id);
            printf("Nombre de places réservées : %d\n", reservation.nb_places_reservees);
            printf("Montant payé : %.2f\n", reservation.montant_paye);
            printf("Date de réservation : %02d/%02d/%04d\n",
                   reservation.date_reservation.jour,
                   reservation.date_reservation.mois,
                   reservation.date_reservation.annee);
            printf("--------------------------\n");
        }
    }

    fclose(reservation_file);
}


void afficherLignes(char* username) {
    int selected_option = 0;
    int key;
    int x_centre = 40;  // Largeur fixe
    int y_centre = 5;   // Hauteur fixe, ajustable selon l'espace
    char *options[] = {"Lignes internes", "Lignes externes", "Retour"};
    int num_options = 3;

    do {
        system("cls");

        // Affichage du titre avec les traits "gras"
        drawTitleWithBoldLine(x_centre, y_centre, "Affichage des Lignes", 14);

        // Affichage des options du menu avec navigation par flèches
        for (int i = 0; i < num_options; i++) {
            gotoXY(x_centre - 10, y_centre + 3 + i * 2);
            if (i == selected_option) {
                SetColor(10);  // Couleur verte pour l'option sélectionnée
                printf("> %s\n", options[i]);
            } else {
                SetColor(7);  // Couleur blanche par défaut
                printf("  %s\n", options[i]);
            }
        }

        // Obtenir l'entrée de l'utilisateur
        key = _getch();
        if (key == 224) {  // Touche flèche (haut ou bas)
            key = _getch();
            if (key == 72 && selected_option > 0) {  // Flèche haut
                selected_option--;
            } else if (key == 80 && selected_option < num_options - 1) {  // Flèche bas
                selected_option++;
            }
        } else if (key == 13) {  // Touche Entrée
            switch (selected_option) {
                case 0: {
                    system("cls");
                    FILE *file = fopen(FILEvoyageInterne, "rb");
                    if (!file) {
                        SetColor(12);
                        gotoXY(x_centre, y_centre + 2);
                        printf("\u26a0\ufe0f Erreur : Impossible d'ouvrir le fichier des lignes internes.\n");
                    } else {
                        VoyageInterne voyage;
                        int found = 0;
                        drawTitleWithBoldLine(x_centre, y_centre, "Lignes Internes Disponibles", 14);
                        
                        gotoXY(x_centre - 50, y_centre + 4);
                        printf("| %-10s | %-15s | %-15s | %-10s | %-8s MAD |\n", "ID", "Départ", "Arrivée", "Date", "Prix");
                        gotoXY(x_centre - 50, y_centre + 5);
                        printf("------------------------------------------------------------------------------\n");

                        while (fread(&voyage, sizeof(VoyageInterne), 1, file) == 1) {
                            found = 1;
                            gotoXY(x_centre - 50, y_centre + 6 + found);
                            printf("| %-10s | %-15s | %-15s | %02d/%02d/%04d | %-8d MAD |\n", 
                                voyage.id, voyage.aeroport_depart, voyage.aeroport_arrive, 
                                voyage.date_voyage.jour, voyage.date_voyage.mois, voyage.date_voyage.annee, 
                                voyage.prix);
                        }

                        if (!found) {
                            SetColor(12);
                            gotoXY(x_centre, y_centre + 2);
                            printf("\u26a0\ufe0f Aucun vol interne disponible pour le moment.\n");
                        }
                        fclose(file);
                    }
                    break;
                }
                case 1: {
                    system("cls");
                    FILE *file = fopen(FILEvoyageExterne, "rb");
                    if (!file) {
                        SetColor(12);
                        gotoXY(x_centre, y_centre + 2);
                        printf("\u26a0\ufe0f Erreur : Impossible d'ouvrir le fichier des lignes externes.\n");
                    } else {
                        VoyageExterne voyage;
                        int found = 0;
                        drawTitleWithBoldLine(x_centre, y_centre, "Lignes Externes Disponibles", 14);
                        
                        gotoXY(x_centre - 60, y_centre + 4);
                        printf("| %-10s | %-15s | %-15s | %-15s | %-15s | %-10s | %-10s MAD | %-10s |\n", 
                            "ID", "Pays Départ", "Pays Arrivée", "Aéroport Départ", "Aéroport Arrivée", "Date", "Prix", "Classe");
                        gotoXY(x_centre - 60, y_centre + 5);
                        printf("-------------------------------------------------------------------------------------------------------------------------------------\n");

                        while (fread(&voyage, sizeof(VoyageExterne), 1, file) == 1) {
                            found = 1;
                            gotoXY(x_centre - 60, y_centre + 6 + found);
                            printf("| %-10s | %-15s | %-15s | %-17s | %-18s | %02d/%02d/%04d | %-10d MAD | %-10s |\n", 
                                voyage.id, voyage.pays_depart, voyage.pays_arrivee, 
                                voyage.aeroport_depart, voyage.aeroport_arrive, 
                                voyage.date_voyage.jour, voyage.date_voyage.mois, voyage.date_voyage.annee, 
                                voyage.prix, voyage.classe);
                        }

                        if (!found) {
                            SetColor(12);
                            gotoXY(x_centre, y_centre + 2);
                            printf("\u26a0\ufe0f Aucun vol externe disponible pour le moment.\n");
                        }
                        fclose(file);
                    }
                    break;
                }
                case 2:
                    printf("\nRetour au menu principal.\n");
                    return;

                default:
                    system("cls");
                    gotoXY(x_centre, y_centre + 8);
                    SetColor(12);  // Rouge pour l'erreur de saisie
                    printf("Choix invalide !\n");
                    Sleep(1500);  // Pause de 1,5 secondes avant de réafficher le menu
                    break;
            }
        }
    } while (1);
}


// un menu pour choisir après le login ou le sign up
void menuPostConnexion(char* username) {
    int choix;
    do {
        printf("\nMenu principal :\n");
        printf("1. Afficher les lignes\n");
        printf("2. Chercher un voyage\n");
        printf("3. Afficher vos reservations internes\n");
        printf("4. Afficher vos reservations externes\n");
        printf("5. Quitter\n");       
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficherLignes( username);
                break;
            case 2:
              ReserverVoyage(username);
                break;
            case 3:// Nouveau cas pour afficher les réservations
              afficherReservationsInternes(username);
                break;
             case 4:// Nouveau cas pour afficher les réservations
              afficherReservationsExternes(username);
                break;               

            case 5: 
                 printf("au revoir !\n");
                 break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 5);
}






























//Partie administrateur;

//fonction generator Id
void generateUniqueID(char *id) {
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand(time(NULL)); // Seed the random number generator

    id[0] = alphabet[rand() % (sizeof(alphabet) - 1)]; // Generate a random alphabetic character

    for (int i = 1; i < ID_LENGTH; i++) { // Generate 7 random digits
        id[i] = '0' + rand() % 10;
    }
    id[ID_LENGTH] = '\0'; // Null-terminate the string
}


int loginAdmin() {
    User admin;
    FILE *file = fopen(FILEadmin, "rb"); // Ouvrir en mode binaire

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }

    printf("Connexion administrateur:\n");

    // Saisie de l'identifiant
    printf("Entrez votre Identifiant: ");
    scanf("%s", admin.username);

    // Saisie du mot de passe masqué
    printf("Entrez votre mot de passe: ");
    char ch;
    int index = 0;
    while ((ch = getch()) != '\r') { // '\r' pour Enter (sous Windows)
        if (ch == '\b' && index > 0) { // Gérer le retour arrière
            index--;
            printf("\b \b"); // Efface un caractère
        } else if (ch != '\b') {
            admin.password[index++] = ch;
            printf("*"); // Affiche une étoile pour chaque caractère
        }
    }
    admin.password[index] = '\0'; // Terminer la chaîne
    printf("\n");

    // Vérification des informations d'identification de l'administrateur
    User tempAdmin;
    while (fread(&tempAdmin, sizeof(User), 1, file) == 1) {
        if (strcmp(tempAdmin.username, admin.username) == 0 && strcmp(tempAdmin.password, admin.password) == 0) {
            printf("Connexion réussie ! Bienvenue, %s %s.\n", tempAdmin.nom, tempAdmin.prenom);
            fclose(file);
            return 1;
        }
    }

    printf("Identifiant ou mot de passe incorrect.\n");
    fclose(file);
    return 0;
}

void signUpAdmin() {
    User newAdmin;
    FILE *file = fopen(FILEadmin, "ab+"); // Open the file in binary append mode

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    printf("Inscription:\n");

    // Collect user information
    printf("Entrez votre nom: ");
    scanf("%s", newAdmin.nom);
    printf("Entrez votre prénom: ");
    scanf("%s", newAdmin.prenom);
    printf("Entrez votre email: ");
    scanf("%s", newAdmin.email);
    printf("Entrez votre numéro de téléphone: ");
    scanf("%s", newAdmin.telephone);
    printf("Entrez un nom d'utilisateur: ");
    scanf("%s", newAdmin.username);

    // Check if the username already exists
    User tempAdmin;
    rewind(file); // Rewind to the beginning of the file for checking
    while (fread(&tempAdmin, sizeof(User), 1, file)) {
        if (strcmp(tempAdmin.username, newAdmin.username) == 0) {
            printf("Ce nom d'utilisateur est déjà pris.\n");
            fclose(file);
            return;
        }
    }

    printf("Entrez un mot de passe: ");
    scanf("%s", newAdmin.password);

    // Write user information to the file
    fwrite(&newAdmin, sizeof(User), 1, file);
    fclose(file);

    printf("Inscription réussie !\n");
}


// Function to add an internal flight line
void ajouterLigneInterne(FILE *file) {
    VoyageInterne voyage;
    printf("Ajouter une ligne interne:\n");


    generateUniqueID(voyage.id);  // Generate a unique ID
    printf("ID du voyage: %s\n", voyage.id);


    printf("Entrez la ville de départ: ");
    scanf("%s", voyage.aeroport_depart);
    printf("Entrez la ville d'arrivée: ");
    scanf("%s", voyage.aeroport_arrive);

    printf("Entrez la date de voyage (jour mois année): ");
    scanf("%d %d %d", &voyage.date_voyage.jour, &voyage.date_voyage.mois, &voyage.date_voyage.annee);

    printf("Entrez l'heure de départ (heure minute): ");
    scanf("%d %d", &voyage.heure_depart.heure, &voyage.heure_depart.minute);

    printf("Entrez l'heure d'arrivée (heure minute): ");
    scanf("%d %d", &voyage.heure_arrivee.heure, &voyage.heure_arrivee.minute);

    printf("Entrez le nombre de places : ");
    scanf("%d", &voyage.nb_place);

    printf("Entrez le nom de la compagnie: ");
    scanf("%s", voyage.compagnie);

    printf("Entrez le prix du billet (MAD): ");
    scanf("%d", &voyage.prix);
    printf("Entrez la durée du vol (h:min): ");

    scanf("%d%d", &voyage.duree_vol.heure,&voyage.duree_vol.minute);

    // enregstrer les information de voyage
    fwrite(&voyage, sizeof(VoyageInterne), 1, file);

    printf("Ligne interne ajoutée avec succès.\n");
}






// Function to add an external flight line
void ajouterLigneExterne(FILE *file) {
    VoyageExterne voyage;
    printf("Ajouter une ligne externe:\n");

    generateUniqueID(voyage.id);  // Generate a unique ID
    printf("ID du voyage: %s\n", voyage.id);

    printf("Entrez le pays de départ: ");
    scanf("%s", voyage.pays_depart);
    printf("Entrez l'aéroport de départ: ");
    scanf("%s", voyage.aeroport_depart);

    printf("Entrez le pays d'arrivée: ");
    scanf("%s", voyage.pays_arrivee);
    printf("Entrez l'aéroport d'arrivée: ");
    scanf("%s", voyage.aeroport_arrive);

    printf("Entrez la date de voyage (jour mois année): ");
    scanf("%d %d %d", &voyage.date_voyage.jour, &voyage.date_voyage.mois, &voyage.date_voyage.annee);

    printf("Entrez l'heure de départ (heure minute): ");
    scanf("%d %d", &voyage.heure_depart.heure, &voyage.heure_depart.minute);

    printf("Entrez l'heure d'arrivée (heure minute): ");
    scanf("%d %d", &voyage.heure_arrivee.heure, &voyage.heure_arrivee.minute);

    printf("Entrez le nombre de places : ");
    scanf("%d", &voyage.nb_place);

    printf("Entrez le nom de la compagnie: ");
    scanf("%s", voyage.compagnie);

    printf("Entrez le prix du billet (MAD): ");
    scanf("%d", &voyage.prix);

    printf("Entrez la classe de vol (economy, business, first): ");
    scanf("%s", voyage.classe);

    printf("Entrez la durée du vol (h:min): ");
    scanf("%d%d", &voyage.duree_vol.heure, &voyage.duree_vol.minute);

    // Save the external flight information with the unique ID
     fwrite(&voyage, sizeof(VoyageExterne), 1, file);

    printf("Ligne externe ajoutée avec succès.\n");
}


void AjouterLigne() {
    int selected_option = 0;
    int key;
    int num_options = 3;
    char *menuOptions[] = {"Ajouter une ligne interne", "Ajouter une ligne externe", "Quitter", NULL};
    int choix;
    int x_centre = 40;  // Largeur fixe
    int y_centre = 5;   // Hauteur fixe, ajustable selon l'espace

    do {
        // Utilisation de la fonction choose_item pour afficher et choisir une option du menu
        system("cls");

        // Affichage du titre avec les traits "gras"
        drawTitleWithBoldLine(x_centre, y_centre, "Ajout des lignes", 14);

        // Affichage des options du menu avec navigation par flèches
        printf("\n");
        for (int i = 0; i < num_options; i++) {
            gotoXY(x_centre - 10, y_centre + 3 + i * 2);
            if (i == selected_option) {
                SetColor(10);  // Couleur verte pour l'option sélectionnée
                printf("> %s\n", menuOptions[i]);
            } else {
                SetColor(7);  // Couleur blanche par défaut
                printf("%s\n", menuOptions[i]);
            }
        }

        // Obtenir l'entrée de l'utilisateur
        key = _getch();
        if (key == 224) {  // Touche flèche (haut ou bas)
            key = _getch();
            if (key == 72 && selected_option > 0) {  // Flèche haut
                selected_option--;
            } else if (key == 80 && selected_option < num_options - 1) {  // Flèche bas
                selected_option++;
            }
        } else if (key == 13) {  // Touche Entrée
            switch (selected_option) {
            case 0: {  // Ajouter une ligne interne
                FILE *file = fopen(FILEvoyageInterne, "ab+");
                if (!file) {
                    SetColor(12);  // Couleur rouge pour l'erreur
                    printf("\n⚠️ Erreur lors de l'ouverture du fichier voyageInterne.bin\n");
                    return;
                }

                ajouterLigneInterne(file);  // Ajouter une ligne de vol interne
                fclose(file);
                break;
            }

            case 1: {  // Ajouter une ligne externe
                FILE *file = fopen(FILEvoyageExterne, "ab+");
                if (!file) {
                    SetColor(12);  // Couleur rouge pour l'erreur
                    printf("\n⚠️ Erreur lors de l'ouverture du fichier voyageExterne.bin\n");
                    return;
                }

                ajouterLigneExterne(file);  // Ajouter une ligne de vol externe
                fclose(file);
                break;
            }

            case 2: {  // Quitter
                system("cls");
                return;
            }

            default:
                SetColor(12);  // Couleur rouge pour un choix invalide
                printf("⚠️ Choix invalide! Veuillez réessayer.\n");
                break;
        }
        }
    } while (choix != 2);  // Continue jusqu'à ce que l'utilisateur choisisse de quitter
}


//supprimer une ligne Interne;
void supprimerLigneInterne() {
    char id[ID_LENGTH + 1];
    printf("Entrez l'ID de la ligne interne à supprimer : ");
    scanf("%s", id);

    FILE *file = fopen(FILEvoyageInterne, "rb");
    FILE *tempFile = fopen("tempInterne.bin", "wb");
    if (!file || !tempFile) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }

    VoyageInterne voyage;
    int found = 0;

    while (fread(&voyage, sizeof(VoyageInterne), 1, file)) {
        if (strcmp(voyage.id, id) != 0) {
            fwrite(&voyage, sizeof(VoyageInterne), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILEvoyageInterne);
    rename("tempInterne.bin", FILEvoyageInterne);

    if (found) {
        printf("Ligne interne supprimée avec succès.\n");
    } else {
        printf("Ligne interne non trouvée.\n");
    }
}
//suprimer ligne externe
void supprimerLigneExterne() {
    char id[ID_LENGTH + 1];
    printf("Entrez l'ID de la ligne externe à supprimer : ");
    scanf("%s", id);

    FILE *file = fopen(FILEvoyageExterne, "rb");
    FILE *tempFile = fopen("tempExterne.bin", "wb");
    if (!file || !tempFile) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }

    VoyageExterne voyage;
    int found = 0;

    while (fread(&voyage, sizeof(VoyageExterne), 1, file)) {
        if (strcmp(voyage.id, id) != 0) {
            fwrite(&voyage, sizeof(VoyageExterne), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILEvoyageExterne);
    rename("tempExterne.bin", FILEvoyageExterne);

    if (found) {
        printf("Ligne externe supprimée avec succès.\n");
    } else {
        printf("Ligne externe non trouvée.\n");
    }
}
// supprimer une ligne qlq
void supprimerLigne(){
    char c;
    do{
        printf("\nVoulez-vous Supprimer une ligne :\n");
        printf("1. Interne\n");
        printf("2. Externe\n");
        printf("3. Annuller\n");
        printf("Entrez votre choix: ");
        scanf("%d",&c);
        switch (c)
                {
                case 1:{
                    supprimerLigneInterne();
                    break;
                }
                case 2:{
                    supprimerLigneExterne();
                    break;
                }

               case 3:
                    break;

                default:
                     printf("Choix invalide! Veuillez réessayer.\n");
                     break;
                }



    }while(c!=3);
}
//modifier un voyage
void modifierLigneInterne() {
    FILE *file = fopen(FILEvoyageInterne, "rb+");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier de voyages internes.\n");
        return;
    }

    char id[ID_LENGTH + 1];
    printf("Entrez l'ID du voyage interne à modifier : ");
    scanf("%s", id);

    VoyageInterne voyage;
    int found = 0;

    while (fread(&voyage, sizeof(VoyageInterne), 1, file) == 1) {
        if (strcmp(voyage.id, id) == 0) {
            int choix;
            do{
            printf("Voyage interne trouvé. Choisissez l'information à modifier :\n");
            printf("1. Aéroport de départ\n");
            printf("2. Aéroport d'arrivée\n");
            printf("3. Date de voyage\n");
            printf("4. Heure de départ\n");
            printf("5. Heure d'arrivée\n");
            printf("6. Nombre de places\n");
            printf("7. Compagnie\n");
            printf("8. Prix\n");
            printf("9. Durée du vol\n");
            printf("0. Quitter la modification\n");
            printf("Entrez votre choix : ");

            scanf("%d", &choix);

            switch (choix) {
                case 1:
                    printf("Nouvel aéroport de départ : ");
                    scanf("%s", voyage.aeroport_depart);
                    break;
                case 2:
                    printf("Nouvel aéroport d'arrivée : ");
                    scanf("%s", voyage.aeroport_arrive);
                    break;
                case 3:
                    printf("Nouvelle date (jour mois année) : ");
                    scanf("%d %d %d", &voyage.date_voyage.jour, &voyage.date_voyage.mois, &voyage.date_voyage.annee);
                    break;
                case 4:
                    printf("Nouvelle heure de départ (heure minute) : ");
                    scanf("%d %d", &voyage.heure_depart.heure, &voyage.heure_depart.minute);
                    break;
                case 5:
                    printf("Nouvelle heure d'arrivée (heure minute) : ");
                    scanf("%d %d", &voyage.heure_arrivee.heure, &voyage.heure_arrivee.minute);
                    break;
                case 6:
                    printf("Nouveau nombre de places : ");
                    scanf("%d", &voyage.nb_place);
                    break;
                case 7:
                    printf("Nouvelle compagnie : ");
                    scanf("%s", voyage.compagnie);
                    break;
                case 8:
                    printf("Nouveau prix : ");
                    scanf("%d", &voyage.prix);
                    break;
                case 9:
                    printf("Nouvelle durée du vol (heure minute) : ");
                    scanf("%d %d", &voyage.duree_vol.heure, &voyage.duree_vol.minute);
                    break;
                case 0:
                    break;
                default:
                    printf("Choix invalide!\n");
                    break;
            }

            // Remet le curseur à la position de ce voyage dans le fichier
            fseek(file, -sizeof(VoyageInterne), SEEK_CUR);
            fwrite(&voyage, sizeof(VoyageInterne), 1, file);

            }while(choix!=0);
            found = 1;
            printf("Informations du voyage interne modifiées avec succès.\n");
            break;
        }
    }

    if (!found) {
        printf("Voyage interne avec l'ID %s non trouvé.\n", id);
    }

    fclose(file);
}

// Fonction pour modifier les informations spécifiques d'un voyage externe
void modifierLigneExterne() {
    FILE *file = fopen(FILEvoyageExterne, "rb+");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier de voyages externes.\n");
        return;
    }

    char id[ID_LENGTH + 1];
    printf("Entrez l'ID du voyage externe à modifier : ");
    scanf("%s", id);

    VoyageExterne voyage;
    int found = 0;

    while (fread(&voyage, sizeof(VoyageExterne), 1, file) == 1) {
        if (strcmp(voyage.id, id) == 0) {
            int choix;
            do{
            printf("Voyage externe trouvé. Choisissez l'information à modifier :\n");
            printf("1. Pays de départ\n");
            printf("2. Aéroport de départ\n");
            printf("3. Pays d'arrivée\n");
            printf("4. Aéroport d'arrivée\n");
            printf("5. Date de voyage\n");
            printf("6. Heure de départ\n");
            printf("7. Heure d'arrivée\n");
            printf("8. Nombre de places\n");
            printf("9. Compagnie\n");
            printf("10. Prix\n");
            printf("11. Classe de vol\n");
            printf("12. Durée du vol\n");
            printf("0. Quitter la modification\n");
            printf("Entrez votre choix : ");

            scanf("%d", &choix);

            switch (choix) {
                case 1:
                    printf("Nouveau pays de départ : ");
                    scanf("%s", voyage.pays_depart);
                    break;
                case 2:
                    printf("Nouvel aéroport de départ : ");
                    scanf("%s", voyage.aeroport_depart);
                    break;
                case 3:
                    printf("Nouveau pays d'arrivée : ");
                    scanf("%s", voyage.pays_arrivee);
                    break;
                case 4:
                    printf("Nouvel aéroport d'arrivée : ");
                    scanf("%s", voyage.aeroport_arrive);
                    break;
                case 5:
                    printf("Nouvelle date (jour mois année) : ");
                    scanf("%d %d %d", &voyage.date_voyage.jour, &voyage.date_voyage.mois, &voyage.date_voyage.annee);
                    break;
                case 6:
                    printf("Nouvelle heure de départ (heure minute) : ");
                    scanf("%d %d", &voyage.heure_depart.heure, &voyage.heure_depart.minute);
                    break;
                case 7:
                    printf("Nouvelle heure d'arrivée (heure minute) : ");
                    scanf("%d %d", &voyage.heure_arrivee.heure, &voyage.heure_arrivee.minute);
                    break;
                case 8:
                    printf("Nouveau nombre de places : ");
                    scanf("%d", &voyage.nb_place);
                    break;
                case 9:
                    printf("Nouvelle compagnie : ");
                    scanf("%s", voyage.compagnie);
                    break;
                case 10:
                    printf("Nouveau prix(MAD) : ");
                    scanf("%d", &voyage.prix);
                    break;
                case 11:
                    printf("Nouvelle classe de vol : ");
                    scanf("%s", voyage.classe);
                    break;
                case 12:
                    printf("Nouvelle durée du vol (heure minute) : ");
                    scanf("%d %d", &voyage.duree_vol.heure, &voyage.duree_vol.minute);
                    break;
                case 0:
                    break;
                default:
                    printf("Choix invalide!\n");
                    break;
            }

            // Remet le curseur à la position de ce voyage dans le fichier
            fseek(file, -sizeof(VoyageExterne), SEEK_CUR);
            fwrite(&voyage, sizeof(VoyageExterne), 1, file);

            }while (choix != 0);
            found = 1;
            printf("Informations du voyage externe modifiées avec succès.\n");
            break;
        }
    }

    if (!found) {
        printf("Voyage externe avec l'ID %s non trouvé.\n", id);
    }

    fclose(file);
}

// Fonction principale pour gérer la modification de voyage

void modifierVoyage(){
     char c;
    do{
        printf("\nLa ligne que vous voulez modifier:\n");
        printf("1. Interne\n");
        printf("2. Externe\n");
        printf("3. Annuller\n");
        printf("Entrez votre choix: ");
        scanf("%d",&c);
        switch (c)
                {
                case 1:{
                    modifierLigneInterne();
                    break;
                }
                case 2:{
                    modifierLigneExterne();
                    break;
                }

               case 3:
                    break;

                default:
                     printf("Choix invalide! Veuillez réessayer.\n");
                     break;
                }



    }while(c!=3);

}







FILE* Ouvrir_Fichier(const char *nom_fichier, const char *mode) {
    FILE *fichier = fopen(nom_fichier, mode);
    if (fichier == NULL) {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier %s en mode %s.\n", nom_fichier, mode);
        return NULL;  // Retourne NULL si l'ouverture échoue
    }
    return fichier;
}


void afficherReservations(FILE* fichier) {


    Reservation reservation;
    printf("\n=== Liste des Réservations Externes ===\n");
    printf("%-20s %-15s %-10s %-12s %-20s\n",
           "Nom d'utilisateur", "ID Voyage", "Places", "Date", "Paiement");
    printf("--------------------------------------------------------------\n");

    while (fread(&reservation, sizeof(Reservation), 1, fichier)) {
        printf("%-20s %-15s %-10d %02d/%02d/%04d       %.2f\n",
               reservation.username,
               reservation.voyage_id,
               reservation.nb_places_reservees,
               reservation.date_reservation.jour,
               reservation.date_reservation.mois,
               reservation.date_reservation.annee,
               reservation.montant_paye);
         printf("--------------------------------------------------------------\n");
    }

    fclose(fichier);
}

void ConsulterVoyages(){
    int c;
    do{
        printf("Voulez-vous consulter :\n");
        printf("              1. Reservations Internes ;\n");
        printf("              2. Reservations Externes ;\n");
        printf("              3. Quitter\n");
        printf("Entrer votre choix :");
        scanf("%d",&c);
        switch (c)
                {
                case 1:{
                    FILE *fichier =Ouvrir_Fichier("reservationsInterne.bin","rb");
                    afficherReservations(fichier);
                    break;
                }
                case 2:{
                    FILE *fichier =Ouvrir_Fichier("reservationsExterne.bin","rb");
                    afficherReservations(fichier);
                    break;
                }

               case 3:
                    break;
                }

    }while(c!=3);

}
void afficherLignesAdmin() {
    int selected_option = 0;
    int key;
    int x_centre = 40;  // Largeur fixe
    int y_centre = 5;   // Hauteur fixe, ajustable selon l'espace
    char *options[] = {"Lignes internes", "Lignes externes", "Retour"};
    int num_options = 3;

    do {
        system("cls");

        // Affichage du titre avec les traits "gras"
        drawTitleWithBoldLine(x_centre, y_centre, "Affichage des Lignes", 14);

        // Affichage des options du menu avec navigation par flèches
        for (int i = 0; i < num_options; i++) {
            gotoXY(x_centre - 10, y_centre + 3 + i * 2);
            if (i == selected_option) {
                SetColor(10);  // Couleur verte pour l'option sélectionnée
                printf("> %s\n", options[i]);
            } else {
                SetColor(7);  // Couleur blanche par défaut
                printf("%s\n", options[i]);
            }
        }

        key = _getch();
        if (key == 224) {  
            key = _getch();
            if (key == 72 && selected_option > 0) {  
                selected_option--;
            } else if (key == 80 && selected_option < num_options - 1) {  
                selected_option++;
            }
        } else if (key == 13) {  
            switch (selected_option) {
         case 0: {  // Lignes internes
                    system("cls");
                    FILE *file = fopen(FILEvoyageInterne, "rb");
                    if (!file) {
                        SetColor(12);
                        printf("\n⚠️ Erreur : Impossible d'ouvrir le fichier des lignes internes.\n");
                    } else {
                        VoyageInterne voyage;
                        int found = 0;

                        drawTitleWithBoldLine(x_centre, y_centre, "LIGNES INTERNES DISPONIBLES", 14);
                        gotoXY(x_centre - 50, y_centre + 4);
                        
                        // Affichage des en-têtes avec l'heure et la compagnie
                        printf("| %-10s | %-20s  | %-20s  | %-10s | %-10s | %-15s | %-10s MAD|\n", 
                               "ID", "Départ", "Arrivée", "Date", "Time", "Compagnie", "Prix");
                        gotoXY(x_centre - 50, y_centre + 5);
                        printf(" -----------------------------------------------------------------------------------------------------------------------\n");

                        int line = 6;
                        while (fread(&voyage, sizeof(VoyageInterne), 1, file) == 1) {
                            found = 1;
                            gotoXY(x_centre - 50, y_centre + line);
                            printf("| %-10s | %-20s | %-20s | %02d/%02d/%04d | %02dh%02d-%02dh%02d | %-15s | %-10d    |\n", 
                                   voyage.id, voyage.aeroport_depart, voyage.aeroport_arrive, 
                                   voyage.date_voyage.jour, voyage.date_voyage.mois, voyage.date_voyage.annee,
                                   voyage.heure_depart.heure, voyage.heure_depart.minute,
                                   voyage.heure_arrivee.heure, voyage.heure_arrivee.minute,
                                   voyage.compagnie, voyage.prix);
                            line++;
                        }

                        if (!found) {
                            SetColor(12);
                            gotoXY(x_centre - 20, y_centre + line + 2);
                            printf("\n⚠️ Aucun vol interne disponible pour le moment.\n");
                        }

                        gotoXY(x_centre - 50, y_centre + line);
                        printf(" =======================================================================================================================\n");
                        fclose(file);
                        centerSystemPause(x_centre, y_centre + line + 4);
                    }
                    break;
                }

              case 1: {
                    system("cls");
                    FILE *file = fopen(FILEvoyageExterne, "rb");
                    if (!file) {
                        SetColor(12);
                        printf("\n⚠️ Erreur : Impossible d'ouvrir le fichier des lignes externes.\n");
                    } else {
                        VoyageExterne voyage;
                        int found = 0;

                        drawTitleWithBoldLine(x_centre, y_centre, "VOLS INTERNATIONAUX DISPONIBLES", 14);

                        gotoXY(x_centre - 75, y_centre + 4);
                        printf("\n\n| %-10s | %-15s  | %-15s  | %-15s   | %-15s   | %-10s | %-10s MAD| %-10s |\n", 
                            "ID", "Pays Départ", "Pays Arrivée", "Aéroport Départ", "Aéroport Arrivée", 
                            "Date", "Prix", "Classe");
                        gotoXY(x_centre - 75, y_centre + 5);
                        printf("-------------------------------------------------------------------------------------------------------------------------------------\n");

                        int line = 6;
                        while (fread(&voyage, sizeof(VoyageExterne), 1, file) == 1) {
                            found = 1;
                            gotoXY(x_centre - 75, y_centre + line);
                            printf("| %-10s | %-15s | %-15s | %-17s | %-18s | %02d/%02d/%04d | %-10d MAD | %-10s|\n", 
                                voyage.id, voyage.pays_depart, voyage.pays_arrivee, 
                                voyage.aeroport_depart, voyage.aeroport_arrive, 
                                voyage.date_voyage.jour, voyage.date_voyage.mois, voyage.date_voyage.annee, 
                                voyage.prix, voyage.classe);
                            line++;
                        }

                        if (!found) {
                            SetColor(12);
                            gotoXY(x_centre - 20, y_centre + line + 2);
                            printf("\n⚠️ Aucun vol externe disponible pour le moment.\n");
                        }

                        gotoXY(x_centre - 75, y_centre + line + 2);
                        printf("=====================================================================================================================================\n");
                        fclose(file);
                        centerSystemPause(x_centre, y_centre + line + 4);
                    }
                    break;
                }
    case 2:
         SetColor(7);
         printf("\nRetour au menu principal.\n");
         return;
    default:
         SetColor(12);
         printf("\n⚠️ Choix invalide. Veuillez réessayer.\n");
            }
        }
    } while (selected_option != 2);
}



//menue de gestion des lignes;
void gestionlignes() {
    int selected_option = 0;
    int key;
    int x_centre = 40;  // Largeur fixe
    int y_centre = 5;   // Hauteur fixe, ajustable selon l'espace
    char *options[] = {"Afficher les lignes", "Ajouter une ligne", "Supprimer une ligne", "Modifier une ligne", "Quitter"};
    int num_options = 5;

    do {
        system("cls");

        // Affichage du titre avec les traits "gras"
        drawTitleWithBoldLine(x_centre, y_centre, "Gestion des Lignes", 14);

        // Affichage des options du menu avec navigation par flèches
        for (int i = 0; i < num_options; i++) {
            gotoXY(x_centre - 10, y_centre + 3 + i * 2);
            if (i == selected_option) {
                SetColor(10);  // Couleur verte pour l'option sélectionnée
                printf("> %s\n", options[i]);
            } else {
                SetColor(7);  // Couleur blanche par défaut
                printf("%s\n", options[i]);
            }
        }

        // Obtenir l'entrée de l'utilisateur
        key = _getch();
        if (key == 224) {  // Touche flèche (haut ou bas)
            key = _getch();
            if (key == 72 && selected_option > 0) {  // Flèche haut
                selected_option--;
            } else if (key == 80 && selected_option < num_options - 1) {  // Flèche bas
                selected_option++;
            }
        } else if (key == 13) {  // Touche Entrée
            switch (selected_option) {
                case 0:
                    system("cls");
                    afficherLignesAdmin();  // Afficher les lignes
                    break;

                case 1: {
                    system("cls");
                    AjouterLigne();  // Ajouter une ligne
                    break;
                }

                case 2: {
                    system("cls");
                    supprimerLigne();  // Supprimer une ligne
                    break;
                }

                case 3: {
                    system("cls");
                    modifierVoyage();  // Modifier une ligne
                    break;
                }

                case 4:
                    system("cls");
                    gotoXY(x_centre, y_centre + 8);
                    SetColor(7);  // Couleur blanche pour le message de fin
                    printf("Retour au menu principal...\n");
                    return;  // Quitter la fonction de gestion des lignes

                default:
                    system("cls");
                    gotoXY(x_centre, y_centre + 8);
                    SetColor(12);  // Rouge pour l'erreur de saisie
                    printf("Choix invalide !\n");
                    Sleep(1500);  // Pause de 1,5 secondes avant de réafficher le menu
                    break;
            }
        }
    } while (1);
}




//partie de statistiques


//statistique sur le nombre de reservation
// Fonction pour obtenir l'année et le mois actuel
void getCurrentYearAndMonth(int* year, int* month) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    *year = tm.tm_year + 1900; // tm_year commence à partir de 1900
    *month = tm.tm_mon + 1;    // tm_mon commence à partir de 0 (janvier)
}

// Fonction pour générer un rapport mensuel des réservations
char* generate_Monthly_Internal_reserv_Report() {
    int year, month;
    getCurrentYearAndMonth(&year, &month);

    // Ouvrir le fichier binaire des réservations
    FILE *binFile = fopen("reservationsInterne.bin", "rb");
    if (!binFile) {
        printf("Erreur : Impossible d'ouvrir reservationsInterne.bin.\n");
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Lire les réservations depuis le fichier binaire
    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    // Créer un nom de fichier basé sur l'année et le mois
    static char filename[256];  // Utiliser static pour conserver le nom du fichier après la fonction
    snprintf(filename, sizeof(filename), "reports/Interne/statistique_reservations/reports-mois/%d-%02d_reservations.txt", year, month);

    // Ouvrir le fichier texte pour écrire les résultats
    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) {
        printf("Erreur : Impossible de créer le fichier %s.\n", filename);
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Initialiser un tableau pour compter les réservations par jour (31 jours max)
    int reservationsPerDay[31] = {0};  // Tableau pour 31 jours

    // Comptabiliser les réservations pour chaque jour du mois
    for (int i = 0; i < numReservations; i++) {
        // Vérifier si la réservation est pour le mois et l'année actuels
        if (reservations[i].date_reservation.mois == month && reservations[i].date_reservation.annee == year) {
            reservationsPerDay[reservations[i].date_reservation.jour - 1] += reservations[i].nb_places_reservees;
        }
    }

    // Écrire les résultats dans le fichier texte (même pour les jours avec 0 réservation)
    for (int day = 0; day < 31; day++) {
        // Écrire le jour et le nombre de réservations, même s'il est à 0
        fprintf(txtFile, "%d %d\n", day + 1, reservationsPerDay[day]);
    }

    fclose(txtFile);
    printf("Le rapport des réservations pour %d-%02d a été généré dans %s\n", year, month, filename);

    return filename;  // Retourner le nom du fichier généré
}

//les reservations eterne
char* generate_Monthly_External_reserv_Report() {
    int year, month;
    getCurrentYearAndMonth(&year, &month);

    // Ouvrir le fichier binaire des réservations externes
    FILE *binFile = fopen("reservationsExterne.bin", "rb");
    if (!binFile) {
        printf("Erreur : Impossible d'ouvrir reservationsExterne.bin.\n");
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Lire les réservations depuis le fichier binaire
    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    // Créer un nom de fichier basé sur l'année et le mois
    static char filename[256];  // Utiliser static pour conserver le nom du fichier après la fonction
    snprintf(filename, sizeof(filename), "reports/Externe/statistique_reservations/reports-mois/%d-%02d_reservations_externe.txt", year, month);

    // Ouvrir le fichier texte pour écrire les résultats
    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) {
        printf("Erreur : Impossible de créer le fichier %s.\n", filename);
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Initialiser un tableau pour compter les réservations par jour (31 jours max)
    int reservationsPerDay[31] = {0};  // Tableau pour 31 jours

    // Comptabiliser les réservations pour chaque jour du mois
    for (int i = 0; i < numReservations; i++) {
        // Vérifier si la réservation est pour le mois et l'année actuels
        if (reservations[i].date_reservation.mois == month && reservations[i].date_reservation.annee == year) {
            reservationsPerDay[reservations[i].date_reservation.jour - 1] += reservations[i].nb_places_reservees;
        }
    }

    // Écrire les résultats dans le fichier texte (même pour les jours avec 0 réservation)
    for (int day = 0; day < 31; day++) {
        fprintf(txtFile, "%d %d\n", day + 1, reservationsPerDay[day]);  // Écrire le jour et le nombre de réservations
    }

    fclose(txtFile);
    printf("Le rapport des réservations externes pour %d-%02d a été généré dans %s\n", year, month, filename);

    return filename;  // Retourner le nom du fichier généré
}


void ReservationM() {
    // Générer le rapport des réservations internes
    char* filename_interne = generate_Monthly_Internal_reserv_Report();
    // Générer le rapport des réservations externes
    char* filename_externe = generate_Monthly_External_reserv_Report();

    if (filename_interne != NULL && filename_externe != NULL) {
        // Construire la commande pour exécuter le script Python avec les deux fichiers
        char command[512];
        snprintf(command, sizeof(command),
            "\"%s\" graphes/reservationsM.py %s %s",
            "C:\\Users\\DELL\\AppData\\Local\\Programs\\Python\\Python313\\pythonw.exe",
            filename_interne,
            filename_externe
        );

        // Appeler le script Python avec system()
        int result = system(command);

        if (result == 0) {
            printf("Le graphe a été tracé avec succès.\n");
        } else {
            printf("Erreur lors de l'exécution du script Python.\n");
        }
    } else {
        printf("Erreur lors de la génération des rapports ou du tracé.\n");
    }
}





// reservation annuelle
// Fonction pour obtenir l'année actuelle
void getCurrentYear(int* year) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    *year = tm.tm_year + 1900; // tm_year commence à partir de 1900
}

// Fonction pour générer un rapport annuel des réservations
char* generate_Annual_Internal_reserv_Report() {
    int year;
    getCurrentYear(&year);


    const char* folderName = "";

    // Ouvrir le fichier binaire des réservations
    FILE *binFile = fopen("reservationsInterne.bin", "rb");
    if (!binFile) {
        printf("Erreur : Impossible d'ouvrir reservationsInterne.bin.\n");
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Lire les réservations depuis le fichier binaire
    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    // Créer un nom de fichier basé sur l'année
    static char filename[256];  // Utiliser static pour conserver le nom du fichier après la fonction
    snprintf(filename, sizeof(filename), "reports/Interne/statistique_reservations/reports-anne/%d_annual_reservations.txt", year);

    // Ouvrir le fichier texte pour écrire les résultats
    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) {
        printf("Erreur : Impossible de créer le fichier %s.\n", filename);
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Initialiser un tableau pour compter les réservations par mois (12 mois)
    int reservationsPerMonth[12] = {0};  // Tableau pour 12 mois

    // Comptabiliser les réservations pour chaque mois de l'année
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.annee == year) {
            reservationsPerMonth[reservations[i].date_reservation.mois - 1] += reservations[i].nb_places_reservees;
        }
    }

    // Écrire les résultats dans le fichier texte (même pour les mois avec 0 réservation)
    for (int month = 0; month < 12; month++) {
        // Écrire le mois et le nombre de réservations, même s'il est à 0
        fprintf(txtFile, "%d %d\n", month + 1, reservationsPerMonth[month]);
    }

    fclose(txtFile);
    printf("Le rapport des réservations pour l'année %d a été généré dans %s\n", year, filename);

    return filename;  // Retourner le nom du fichier généré
}




char* generate_Annual_External_reserv_Report() {
    int year;
    getCurrentYear(&year); // Obtenir l'année actuelle

    // Ouvrir le fichier binaire des réservations externes
    FILE *binFile = fopen("reservationsExterne.bin", "rb");
    if (!binFile) {
        printf("Erreur : Impossible d'ouvrir reservationsExterne.bin.\n");
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Lire les réservations depuis le fichier binaire
    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    // Créer un nom de fichier basé sur l'année
    static char filename[256];  // Utiliser static pour conserver le nom du fichier après la fonction
    snprintf(filename, sizeof(filename), "reports/Externe/statistique_reservations/reports-anne/%d_annual_reservations_externe.txt", year);

    // Ouvrir le fichier texte pour écrire les résultats
    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) {
        printf("Erreur : Impossible de creer le fichier %s.\n", filename);
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Initialiser un tableau pour compter les réservations par mois (12 mois)
    int reservationsPerMonth[12] = {0};  // Tableau pour 12 mois

    // Comptabiliser les réservations pour chaque mois de l'année
    for (int i = 0; i < numReservations; i++) {
        // Vérifier si la réservation est pour l'année actuelle
        if (reservations[i].date_reservation.annee == year) {
            reservationsPerMonth[reservations[i].date_reservation.mois - 1] += reservations[i].nb_places_reservees;
        }
    }

    // Écrire les résultats dans le fichier texte (même pour les mois avec 0 réservation)
    for (int month = 0; month < 12; month++) {
        fprintf(txtFile, "%d %d\n", month + 1, reservationsPerMonth[month]);  // Écrire le mois et le nombre de réservations
    }

    fclose(txtFile);
    printf("Le rapport annuel des réservations externes pour l'année %d a été généré dans %s\n", year, filename);

    return filename;  // Retourner le nom du fichier généré
}


// Fonction principale qui appelle les deux fonctions : Génération du rapport et tracé avec Gnuplot
void ReservationA() {
    // Générer le rapport des réservations internes
    char* filename_interne = generate_Annual_Internal_reserv_Report();
    // Générer le rapport des réservations externes
    char* filename_externe = generate_Annual_External_reserv_Report();

    if (filename_interne != NULL && filename_externe != NULL) {
        // Construire la commande pour exécuter le script Python avec les deux fichiers
        char command[512];
        snprintf(command, sizeof(command),
                 "\"C:\\Users\\DELL\\AppData\\Local\\Programs\\Python\\Python313\\pythonw.exe\" graphes/reservationsA.py %s %s",
                 filename_interne, filename_externe);

        // Appeler le script Python avec system()
        int result = system(command);

        if (result == 0) {
            printf("Le graphe annuel a été tracé avec succès.\n");
        } else {
            printf("Erreur lors de l'exécution du script Python.\n");
        }
    } else {
        printf("Erreur lors de la génération des rapports ou du tracé.\n");
    }
}





void statistiqueReservation(){
    int c;
    do{
            printf("\nReservations:\n");
            printf("            1.Monssuelle \n");
            printf("            2.Annuelle\n");
            printf("Entrez votre choix: ");
            scanf("%d",&c);
            switch (c)
                {
                case 1:
                     ReservationM();
                     break;
                case 2:{
                     ReservationA();

                    break;
                }
               default:
                     printf("Choix invalide! Veuillez réessayer.\n");
                     break;
         }



    }while(c!=2);


}









//stistique sur le revenue
// Fonction pour générer le rapport des revenus mensuels
char* generate_Monthly_Internal_Revenue_Report() {
    int year, month;
    getCurrentYearAndMonth(&year, &month);  // Utilisez getCurrentYearAndMonth() pour obtenir l'année et le mois actuels

    // Ouvrir le fichier binaire des réservations
    FILE *binFile = fopen("reservationsInterne.bin", "rb");
    if (!binFile) {
        printf("Erreur : Impossible d'ouvrir reservationsInterne.bin.\n");
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Lire les réservations depuis le fichier binaire
    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    // Créer un nom de fichier basé sur l'année et le mois
    static char filename[256];  // Utiliser static pour conserver le nom du fichier après la fonction
    snprintf(filename, sizeof(filename), "reports/Interne/statistique_revenue/reports-mois/%d-%02d_revenue.txt", year, month);

    // Ouvrir le fichier texte pour écrire les résultats
    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) {
        printf("Erreur : Impossible de créer le fichier %s.\n", filename);
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Initialiser un tableau pour compter les revenus par jour (31 jours max)
    float revenuePerDay[31] = {0.0};  // Tableau pour 31 jours

    // Comptabiliser les revenus pour chaque jour du mois
    for (int i = 0; i < numReservations; i++) {
        // Vérifier si la réservation est pour le mois et l'année actuels
        if (reservations[i].date_reservation.mois == month && reservations[i].date_reservation.annee == year) {
            revenuePerDay[reservations[i].date_reservation.jour - 1] += reservations[i].montant_paye;  // Ajouter le montant payé
        }
    }

    // Écrire les résultats dans le fichier texte (même pour les jours avec 0 revenu)
    for (int day = 0; day < 31; day++) {
        // Écrire le jour et le revenu, même s'il est à 0
        fprintf(txtFile, "%d %.2f\n", day + 1, revenuePerDay[day]);
    }

    fclose(txtFile);
    printf("Le rapport des revenus pour %d-%02d a été généré dans %s\n", year, month, filename);

    return filename;  // Retourner le nom du fichier généré
}


char* generate_Monthly_External_Revenue_Report() {
    int year, month;
    getCurrentYearAndMonth(&year, &month);  // Récupérer l'année et le mois actuels

    // Ouvrir le fichier binaire des réservations externes
    FILE *binFile = fopen("reservationsExterne.bin", "rb");
    if (!binFile) {
        printf("Erreur : Impossible d'ouvrir reservationsExterne.bin.\n");
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Lire les réservations depuis le fichier binaire
    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    // Créer un nom de fichier basé sur l'année et le mois
    static char filename[256];  // Utiliser static pour conserver le nom du fichier après la fonction
    snprintf(filename, sizeof(filename), "reports/Externe/statistique_revenue/reports-mois/%d-%02d_revenue_externe.txt", year, month);

    // Ouvrir le fichier texte pour écrire les résultats
    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) {
        printf("Erreur : Impossible de créer le fichier %s.\n", filename);
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Initialiser un tableau pour compter les revenus par jour (31 jours max)
    float revenuePerDay[31] = {0.0};  // Tableau pour 31 jours

    // Comptabiliser les revenus pour chaque jour du mois
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.mois == month && reservations[i].date_reservation.annee == year) {
            revenuePerDay[reservations[i].date_reservation.jour - 1] += reservations[i].montant_paye;  // Ajouter le montant payé
        }
    }

    // Écrire les résultats dans le fichier texte (même pour les jours avec 0 revenu)
    for (int day = 0; day < 31; day++) {
        fprintf(txtFile, "%d %.2f\n", day + 1, revenuePerDay[day]);
    }

    fclose(txtFile);
    printf("Le rapport des revenus externes pour %d-%02d a été généré dans %s\n", year, month, filename);

    return filename;  // Retourner le nom du fichier généré
}

void RevenueM() {
    int year, month;
    getCurrentYearAndMonth(&year, &month);  // Récupérer l'année et le mois actuels

    // Générer le rapport des revenus mensuels internes
    char* reportFileInterne = generate_Monthly_Internal_Revenue_Report();
    // Générer le rapport des revenus mensuels externes
    char* reportFileExterne = generate_Monthly_External_Revenue_Report();

    if (reportFileInterne != NULL && reportFileExterne != NULL) {
        // Construire la commande pour exécuter le script Python avec les deux fichiers
        char command[512];
        snprintf(command, sizeof(command), "C:\\Users\\DELL\\AppData\\Local\\Programs\\Python\\Python313\\pythonw.exe graphes\\revenueM.py %s %s", reportFileInterne, reportFileExterne);

        // Exécuter le script Python
        int result = system(command);

        if (result == 0) {
            printf("Le graphe mensuel des revenus a été tracé avec succès.\n");
        } else {
            printf("Erreur lors de l'exécution du script Python.\n");
        }
    } else {
        printf("Erreur lors de la génération des rapports des revenus mensuels.\n");
    }
}




// Fonction pour générer le rapport des revenus annuels
char* generate_Annual_Internal_Revenue_Report() {
    int year;
    getCurrentYear(&year);  // Utilisez getCurrentYear() pour obtenir l'année actuelle

    // Ouvrir le fichier binaire des réservations
    FILE *binFile = fopen("reservationsInterne.bin", "rb");
    if (!binFile) {
        printf("Erreur : Impossible d'ouvrir reservationsInterne.bin.\n");
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Lire les réservations depuis le fichier binaire
    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    // Créer un nom de fichier basé sur l'année
    static char filename[256];  // Utiliser static pour conserver le nom du fichier après la fonction
    snprintf(filename, sizeof(filename), "reports/Interne/statistique_revenue/reports-anne/%d_annual_revenue.txt", year);

    // Ouvrir le fichier texte pour écrire les résultats
    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) {
        printf("Erreur : Impossible de créer le fichier %s.\n", filename);
        return NULL;  // Retourner NULL en cas d'erreur
    }

    // Initialiser un tableau pour compter les revenus par mois (12 mois)
    float revenuePerMonth[12] = {0.0};  // Tableau pour 12 mois

    // Comptabiliser les revenus pour chaque mois de l'année
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.annee == year) {
            revenuePerMonth[reservations[i].date_reservation.mois - 1] += reservations[i].montant_paye;  // Ajouter le montant payé
        }
    }

    // Écrire les résultats dans le fichier texte (mois et revenu)
    for (int month = 0; month < 12; month++) {
        // Écrire le mois et le revenu, même s'il est à 0
        fprintf(txtFile, "%d %.2f\n", month + 1, revenuePerMonth[month]);
    }

    fclose(txtFile);
    printf("Le rapport des revenus pour l'année %d a été généré dans %s\n", year, filename);

    return filename;  // Retourner le nom du fichier généré
}


// Fonction pour générer le rapport des revenus annuels externes
char* generate_Annual_External_Revenue_Report() {
    int year;
    getCurrentYear(&year);  // Obtenez l'année actuelle

    // Ouvrir le fichier binaire des réservations externes
    FILE *binFile = fopen("reservationsExterne.bin", "rb");
    if (!binFile) {
        printf("Erreur : Impossible d'ouvrir reservationsExterne.bin.\n");
        return NULL;
    }

    // Lire les réservations depuis le fichier binaire
    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    // Créer un nom de fichier basé sur l'année
    static char filename[256];  // Utiliser static pour conserver le nom du fichier
    snprintf(filename, sizeof(filename), "reports/Externe/statistique_revenue/reports-anne/%d_annual_revenue_externe.txt", year);

    // Ouvrir le fichier texte
    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) {
        printf("Erreur : Impossible de créer le fichier %s.\n", filename);
        return NULL;
    }

    // Initialiser les revenus par mois (12 mois)
    float revenuePerMonth[12] = {0.0};

    // Comptabiliser les revenus pour chaque mois de l'année
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.annee == year) {
            revenuePerMonth[reservations[i].date_reservation.mois - 1] += reservations[i].montant_paye;
        }
    }

    // Écrire les résultats dans le fichier
    for (int month = 0; month < 12; month++) {
        fprintf(txtFile, "%d %.2f\n", month + 1, revenuePerMonth[month]);
    }

    fclose(txtFile);
    printf("Le rapport des revenus externes pour l'année %d a été généré dans %s\n", year, filename);

    return filename;
}


void RevenueA() {
    int year;
    getCurrentYear(&year);  // Récupérer l'année actuelle

    // Générer les rapports annuels
    char* reportFileInterne = generate_Annual_Internal_Revenue_Report();
    char* reportFileExterne = generate_Annual_External_Revenue_Report();

    if (reportFileInterne != NULL && reportFileExterne != NULL) {
        // Construire la commande pour exécuter le script Python avec les deux fichiers
        char command[512];
        snprintf(command, sizeof(command), "C:\\Users\\DELL\\AppData\\Local\\Programs\\Python\\Python313\\pythonw.exe graphes\\revenueA.py %s %s", reportFileInterne, reportFileExterne);

        // Exécuter le script Python
        int result = system(command);

        if (result == 0) {
            printf("Le graphe annuel des revenus a été tracé avec succès.\n");
        } else {
            printf("Erreur lors de l'exécution du script Python.\n");
        }
    } else {
        printf("Erreur lors de la génération des rapports des revenus annuels.\n");
    }
}






void statistiqueRevenue(){
    int c;
    do{
            printf("\nRevenue:\n");
            printf("            1.Monssuelle \n");
            printf("            2.Annuelle\n");
            printf("Entrez votre choix: ");
            scanf("%d",&c);
            switch (c)
                {
                case 1:
                     RevenueM();
                     break;
                case 2:{
                     RevenueA();
                    break;
                }
               default:
                     printf("Choix invalide! Veuillez réessayer.\n");
                     break;
         }



    }while(c!=2);


}







// menue de statistique ;
void ConsulterStatistique(){
    int c ;
    do{
        printf("voulez-vous voir :\n");
        printf("            1.Le nombre de reservation \n");
        printf("            2.Le revenue \n");
        printf("Entrez votre choix: ");
        scanf("%d",&c);
        switch (c)
                {
                case 1:
                     statistiqueReservation();
                     break;
                case 2:{
                     statistiqueRevenue();
                     break;
                }
               default:
                     printf("Choix invalide! Veuillez réessayer.\n");
                     break;
         }
    }while(c!=2);
}


// Fonction pour ajouter une compagnie
void ajouterCompagnie() {
    Compagnie nouvelleCompagnie;
    Contrat nouveauContrat;

    // Encodage UTF-8 pour afficher correctement les caractères spéciaux
    SetConsoleOutputCP(CP_UTF8);

    // Saisie des informations sur la compagnie
    printf("Entrez le nom de la compagnie: ");
    scanf("%29s", nouvelleCompagnie.nom);

    printf("Entrez le pays d'origine de la compagnie: ");
    scanf("%29s", nouvelleCompagnie.pays);

    printf("Entrez l'année de création de la compagnie: ");
    scanf("%d", &nouvelleCompagnie.annee_creation);

    printf("Entrez le nombre d'avions dans la flotte: ");
    scanf("%d", &nouvelleCompagnie.nb_avions);

    // Saisie des informations sur le contrat
    printf("\nEntrez l'identifiant du contrat: ");
    scanf("%19s", nouveauContrat.id_contrat);

    printf("Entrez la date de début du contrat (dd/mm/yyyy): ");
    scanf("%14s", nouveauContrat.date_debut);

    printf("Entrez la date de fin du contrat (dd/mm/yyyy): ");
    scanf("%14s", nouveauContrat.date_fin);

    printf("Entrez le montant du contrat: ");
    scanf("%f", &nouveauContrat.montant);

    // Ouverture du fichier en mode ajout binaire
    FILE *fichier = fopen(FILENAME, "ab");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // Écriture des données dans le fichier
    fwrite(&nouvelleCompagnie, sizeof(Compagnie), 1, fichier);
    fwrite(&nouveauContrat, sizeof(Contrat), 1, fichier);

    fclose(fichier);
    printf("Compagnie et contrat ajoutés avec succès.\n");
}

// Fonction pour supprimer une compagnie
void supprimerCompagnie() {
    char nomCompagnie[30];

    // Saisie du nom de la compagnie à supprimer
    printf("Entrez le nom de la compagnie à supprimer: ");
    scanf("%29s", nomCompagnie);

    FILE *fichier = fopen(FILENAME, "rb");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // Calcul de la taille du fichier
    fseek(fichier, 0, SEEK_END);
    long taille_fichier = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);

    long nb_compagnies = taille_fichier / (sizeof(Compagnie) + sizeof(Contrat));
    Compagnie compagnie;
    Contrat contrat;
    Compagnie compagnies[100];
    Contrat contrats[100];
    long i = 0;
   // Allouer dynamiquement de la mémoire
    Compagnie *compagnies = (Compagnie*)malloc(nb_compagnies * sizeof(Compagnie));
    Contrat *contrats = (Contrat*)malloc(nb_compagnies * sizeof(Contrat));
    // Lecture des données dans des tableaux temporaires
    while (fread(&compagnie, sizeof(Compagnie), 1, fichier)) {
        fread(&contrat, sizeof(Contrat), 1, fichier);
        compagnies[i] = compagnie;
        contrats[i] = contrat;
        i++;
    }
      // Libérer la mémoire allouée
    free(compagnies);
    free(contrats);

    fclose(fichier);

    // Réouverture du fichier en mode écriture
    fichier = fopen(FILENAME, "wb");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier en mode écriture.\n");
        return;
    }

    // Réécriture des données en ignorant la compagnie à supprimer
    int trouve = 0;
    for (i = 0; i < nb_compagnies; i++) {
        if (strcmp(compagnies[i].nom, nomCompagnie) != 0) {
            fwrite(&compagnies[i], sizeof(Compagnie), 1, fichier);
            fwrite(&contrats[i], sizeof(Contrat), 1, fichier);
        } else {
            trouve = 1;
        }
    }

    fclose(fichier);

    if (trouve) {
        printf("La compagnie '%s' a été supprimée.\n", nomCompagnie);
    } else {
        printf("La compagnie '%s' n'a pas été trouvée.\n", nomCompagnie);
    }
}


void afficherCompagnies() {
    FILE *fichier = fopen(FILENAME, "rb");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    Compagnie compagnie;
    Contrat contrat;
    int compteur = 0;

    printf("\nListe des compagnies enregistrées :\n");
    printf("----------------------------------------------------------\n");

    // Lire et afficher chaque compagnie et son contrat
    while (fread(&compagnie, sizeof(Compagnie), 1, fichier)) {
        fread(&contrat, sizeof(Contrat), 1, fichier); // Lire le contrat correspondant
        compteur++;
        printf("Compagnie %d :\n", compteur);
        printf("  Nom              : %s\n", compagnie.nom);
        printf("  Pays d'origine   : %s\n", compagnie.pays);
        printf("  Année de création: %d\n", compagnie.annee_creation);
        printf("  Nombre d'avions  : %d\n", compagnie.nb_avions);
        printf("  Contrat associé :\n");
        printf("    ID Contrat     : %s\n", contrat.id_contrat);
        printf("    Date de début  : %s\n", contrat.date_debut);
        printf("    Date de fin    : %s\n", contrat.date_fin);
        printf("    Montant        : %.2f\n", contrat.montant);
        printf("----------------------------------------------------------\n");
    }

    fclose(fichier);

    if (compteur == 0) {
        printf("Aucune compagnie enregistrée.\n");
    }
}


void informationCompagnie(){
    int c ;
    do{
        printf("\nCompagnie:\n");
        printf("1/ Ajouter Compagnie\n");
        printf("2/ Supprime Compagnie\n");
        printf("3/ affichier listes des compagnies\n");

        scanf("%d",&c);
                switch (c)
                {
                case 1:
                     ajouterCompagnie();
                     break;
                case 2:{
                    supprimerCompagnie();
                    break;
                }
                case 3:
                     afficherCompagnies();
                     break;
                case 4:
                     printf("Retour au programme principal.\n");
                     break;

               default:
                     printf("Choix invalide! Veuillez réessayer.\n");
                     break;

    }
}while(c!=4);
}



//gestion des employes 
// Fonction pour vérifier si l'identifiant existe déjà dans le fichier
int identifiantExiste(const char *nom_fichier, const char *identifiant) {
    FILE *fichier = fopen(nom_fichier, "rb");
    if (fichier == NULL) {
        return 0;  // Le fichier n'existe pas encore, donc aucun employé n'est enregistré
    }

    Employe employe;
    while (fread(&employe, sizeof(Employe), 1, fichier)) {
        if (strcmp(employe.identifiant, identifiant) == 0) {
            fclose(fichier);
            return 1;  // L'identifiant existe déjà
        }
    }

    fclose(fichier);
    return 0;  // L'identifiant est unique
}

// Fonction pour ajouter un employé au système et stocker ses informations dans un fichier
void ajouterEmploye() {
    Employe employe;
    const char *nom_fichier = "employe.bin"; // Le fichier dans lequel les employés sont enregistrés
    
    // Demander les informations à entrer pour l'employé
    printf("Nom de l'employé: ");
    scanf("%49s", employe.nom);

    printf("Prénom de l'employé: ");
    scanf("%49s", employe.prenom);

    printf("Poste de l'employé: ");
    scanf("%49s", employe.poste);

    printf("Ville où travaille l'employé: ");
    scanf("%49s", employe.ville);

    printf("Email de l'employé: ");
    scanf("%99s", employe.email);

    printf("Numéro de téléphone de l'employé: ");
    scanf("%19s", employe.telephone);

    // Vérifier que l'identifiant est unique
    do {
        printf("Identifiant de connexion (unique): ");
        scanf("%29s", employe.identifiant);
        if (identifiantExiste(nom_fichier, employe.identifiant)) {
            printf("Erreur: cet identifiant existe déjà. Veuillez en saisir un autre.\n");
        }
    } while (identifiantExiste(nom_fichier, employe.identifiant));

    printf("Mot de passe: ");
    scanf("%29s", employe.mot_de_passe);

    // Ouvrir le fichier en mode ajout binaire
    FILE *fichier = fopen(nom_fichier, "ab");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // Écrire les informations de l'employé dans le fichier
    fwrite(&employe, sizeof(Employe), 1, fichier);

    // Fermer le fichier
    fclose(fichier);

    printf("L'employé a été ajouté avec succès.\n");
}

// Fonction pour supprimer un employé du fichier
void supprimerEmploye() {
    char identifiant[30];
    
    // Demander à l'administrateur de saisir l'identifiant de l'employé à supprimer
    printf("Entrez l'identifiant de l'employé à supprimer: ");
    scanf("%29s", identifiant);

    FILE *fichier = fopen("employe.bin", "rb");
    if (fichier == NULL) {
        printf("Le fichier n'existe pas.\n");
        return;
    }

    FILE *tempFile = fopen("temp.bin", "wb");
    if (tempFile == NULL) {
        printf("Erreur lors de la création du fichier temporaire.\n");
        fclose(fichier);
        return;
    }

    Employe employe;
    int trouve = 0;

    // Parcourir tous les employés et copier dans le fichier temporaire sauf l'employé à supprimer
    while (fread(&employe, sizeof(Employe), 1, fichier)) {
        if (strcmp(employe.identifiant, identifiant) != 0) {
            fwrite(&employe, sizeof(Employe), 1, tempFile);  // Copier l'employé si l'identifiant ne correspond pas
        } else {
            trouve = 1;  // L'employé a été trouvé et supprimé
        }
    }

    fclose(fichier);
    fclose(tempFile);

    if (!trouve) {
        printf("Aucun employé trouvé avec cet identifiant.\n");
        remove("temp.bin");  // Supprimer le fichier temporaire si aucun employé n'a été trouvé
        return;
    }

    // Remplacer l'ancien fichier par le nouveau fichier sans l'employé supprimé
    remove("employe.bin");  // Supprimer l'ancien fichier
    rename("temp.bin", "employe.bin");  // Renommer le fichier temporaire en fichier original

    printf("L'employé a été supprimé avec succès.\n");
}

void afficherEmployes() {
    FILE *fichier = fopen("employe.bin", "rb");
    if (fichier == NULL) {
        printf("Le fichier n'existe pas.\n");
        return;
    }

    Employe employe;
    int compteur = 0;

    printf("Liste des employés :\n");

    // Parcourir le fichier et afficher les informations de chaque employé
    while (fread(&employe, sizeof(Employe), 1, fichier)) {
        printf("\nEmployé %d:\n", ++compteur);
        printf("Nom: %s\n", employe.nom);
        printf("Prénom: %s\n", employe.prenom);
        printf("Identifiant: %s\n", employe.identifiant);
        printf("Poste: %s\n", employe.poste);
        printf("Ville: %s\n", employe.ville);
        printf("Email: %s\n", employe.email);
        printf("Téléphone: %s\n", employe.telephone);
    }

    fclose(fichier);

    if (compteur == 0) {
        printf("Aucun employé enregistré.\n");
    }
}


// Fonction pour supprimer une réservation dans le fichier
void supprimerReservation(const char *nom_fichier) {
    char username[50], voyage_id[30];
    
    // Demander l'ID de la réservation et le nom d'utilisateur
    printf("Entrez le nom d'utilisateur de la réservation à supprimer : ");
    scanf("%49s", username);
    
    printf("Entrez l'ID de la réservation à supprimer : ");
    scanf("%29s", voyage_id);
    
    // Ouvrir le fichier des réservations en mode lecture binaire
    FILE *fichier = fopen(nom_fichier, "rb");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier des réservations.\n");
        return;
    }
    
    // Ouvrir un fichier temporaire pour écrire les réservations restantes
    FILE *temp_file = fopen("temp.bin", "wb");
    if (temp_file == NULL) {
        printf("Erreur lors de la création du fichier temporaire.\n");
        fclose(fichier);
        return;
    }

    Reservation reservation;
    int found = 0;

    // Parcourir le fichier et écrire les réservations restantes dans le fichier temporaire
    while (fread(&reservation, sizeof(Reservation), 1, fichier)) {
        // Vérifier si l'utilisateur et l'ID de la réservation correspondent
        if (strcmp(reservation.username, username) == 0 && strcmp(reservation.voyage_id, voyage_id) == 0) {
            printf("Réservation trouvée et supprimée.\n");
            found = 1;  // Marquer que la réservation a été trouvée et supprimée
        } else {
            // Si la réservation ne correspond pas, l'écrire dans le fichier temporaire
            fwrite(&reservation, sizeof(Reservation), 1, temp_file);
        }
    }

    // Fermer les fichiers
    fclose(fichier);
    fclose(temp_file);

    // Vérifier si la réservation a été trouvée et supprimée
    if (!found) {
        printf("Aucune réservation trouvée avec cet identifiant et nom d'utilisateur.\n");
        // Supprimer le fichier temporaire s'il n'y a pas eu de suppression
        remove("temp.bin");
        return;
    }

    // Supprimer le fichier original et renommer le fichier temporaire
    remove(nom_fichier);
    rename("temp.bin", nom_fichier);  // Le fichier temporaire devient le fichier principal

    printf("La réservation a été supprimée avec succès.\n");
}


void gestionEmploye(){
    int choix;
    do {
        printf("\nMenu de gestion des employés\n");
        printf("1. Ajouter un employé\n");
        printf("2. Supprimer un employé\n");
        printf("3. Afficher les employés\n");
        printf("4. Quitter\n");
        printf("Choisissez une option: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouterEmploye();
                break;
            case 2:
                supprimerEmploye();
                break;
            case 3:
                afficherEmployes();
                break;
            case 4:
                printf("Au revoir!\n");
                break;
            default:
                printf("Choix invalide, essayez encore.\n");
        }
    } while (choix != 4);

}

void SupprimerReservations(){
    int c;
    char* nom_fichier;
    do{
        printf("1. Supprimer une réservation Interne\n");
        printf("2. Supprimer une réservation Externe\n");
        printf("3. Quitter\n");
        printf("Choisissez une option: ");
        scanf("%d", &c);

        switch (c) {
            case 1:
                // Fichier pour les réservations internes
                nom_fichier = "reservationsInterne.bin";
                supprimerReservation(nom_fichier);
                break;
            case 2:
                // Fichier pour les réservations externes
                nom_fichier = "reservationsExterne.bin";
                supprimerReservation(nom_fichier);
                break;
            case 3:
                printf("Retour au menu précédent.\n");
                break;
            default:
                printf("Choix invalide, essayez encore.\n");
        }
    } while (c != 3);  // Quitter si c'est 3
}




// Fonction pour afficher le menu de gestion des réservations
void GestionReservations() {
    int choix;

    do {
        printf("\nMenu de Gestion des Réservations\n");
        printf("1. Supprimer une réservation\n");
        printf("2. Afficher toutes les réservations\n");
        printf("3. Quitter\n");
        printf("Choisissez une option: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                SupprimerReservations();
                break;
            case 2:
                ConsulterVoyages();
                break;
            case 3:
                printf("Au revoir!\n");
                break;
            default:
                printf("Choix invalide, essayez encore.\n");
        }
    } while (choix != 3);
}



//Menu d'administrateur
void MenuAdministrateur(){
    int c;
    do{

                printf("\nMenu Administrateur:\n");
                printf("1/ Gestion de Lignes \n");
                printf("2/ Gestion des reservations \n");
                printf("3/ Consulter les Statistiques \n");
                printf("4/ Gestion des Compagnies \n");
                printf("5/ Gestion des Employes \n");
                printf("6/ QUITTER\n");
                printf("Donner votre choix : ");
                scanf("%d",&c);
                switch (c)
                {
                case 1:
                     gestionlignes();
                     break;
                case 2:
                     GestionReservations();
                     break;
                case 3:
                     ConsulterStatistique();
                     break;
                case 4:
                     informationCompagnie();
                     break;
                case 5:
                     gestionEmploye();
                     break;
                case 6:

                     printf("Retour au programme principal.\n");
                     break;

               default:
                     printf("Choix invalide! Veuillez réessayer.\n");
                     break;
         }

            }while(c != 6);

}






















//// partie employee


// Fonction pour vérifier la connexion d'un employé
int loginEmploye() {
    system("cls");
    char identifiant[30];
    char mot_de_passe[30];
    int i = 0;

    // Affichage stylisé du titre
    int x_centre = 40;
    int y_centre = 5;
    drawTitleWithBoldLine(x_centre, y_centre, "Connexion Employé", 14);

    // Cadre pour le champ identifiant
    
    gotoXY(x_centre - 22, y_centre + 3);
    SetColor(14);
    printf(" ------------------------------------------");
    gotoXY(x_centre - 22, y_centre + 4);
    printf("| Identifiant                              |");
    gotoXY(x_centre - 22, y_centre + 5);
    printf(" ------------------------------------------");

    // Demander l'identifiant
    gotoXY(x_centre - 7, y_centre + 4);
    SetColor(7);
    scanf("%29s", identifiant);

    // Limiter la saisie de l'utilisateur à 20 caractères
    identifiant[20] = '\0';

    // Cadre pour le champ mot de passe
    SetColor(14);
    gotoXY(x_centre - 22, y_centre + 6);
    printf(" ------------------------------------------");
    gotoXY(x_centre - 22, y_centre + 7);
    printf("| Mot de passe                             |");
    gotoXY(x_centre - 22, y_centre + 8);
    printf(" ------------------------------------------");

    // Demander le mot de passe
    gotoXY(x_centre - 7, y_centre + 7);
    SetColor(7);
    while (1) {
        char ch = _getch();  // Lire un caractère sans l'afficher
        if (ch == 13) {  // Si l'utilisateur appuie sur Enter (13 en ASCII)
            mot_de_passe[i] = '\0';  // Terminer la chaîne de caractères
            break;
        } else if (ch == 8 && i > 0) {  // Si c'est la touche Backspace
            printf("\b \b");  // Effacer le dernier caractère affiché
            i--;
        } else if (i < 20) {  // Limiter la saisie à 20 caractères
            mot_de_passe[i] = ch;  // Ajouter le caractère au mot de passe
            i++;
            printf("*");  // Afficher un astérisque pour masquer l'entrée
        }
    }
    printf("\n");

    // Ouvrir le fichier des employés en mode lecture binaire
    FILE *fichier = fopen("employe.bin", "rb");
    if (fichier == NULL) {
        gotoXY(x_centre - 20, y_centre + 10);
        SetColor(12);  // Rouge pour l'erreur
        printf("Erreur d'ouverture du fichier des employés.\n");
        centerSystemPause(x_centre, y_centre + 11);
        SetColor(7);   // Retour à la couleur par défaut
        return 0;
    }

    Employe employe;
    int trouve = 0;
    while (fread(&employe, sizeof(Employe), 1, fichier)) {
        // Vérifier si l'identifiant et le mot de passe correspondent
        if (strcmp(employe.identifiant, identifiant) == 0 && strcmp(employe.mot_de_passe, mot_de_passe) == 0) {
            fclose(fichier);
            gotoXY(x_centre - 20, y_centre + 10);
            SetColor(10);  // Vert pour la réussite
            printf("Connexion réussie.\n");
            centerSystemPause(x_centre, y_centre + 11);
            SetColor(7);
            return 1;  // Connexion réussie
        }
    }

    fclose(fichier);
    gotoXY(x_centre - 20, y_centre + 10);
    SetColor(12);  // Rouge pour l'erreur
    printf("Identifiant ou mot de passe incorrect.\n");
    centerSystemPause(x_centre, y_centre + 11);
    SetColor(7);   // Retour à la couleur par défaut
    return 0;  // Connexion échouée
}

void ReserverVoyageHorsLineInterne(char *username) {
    FILE *fichierVoyages = fopen("voyageInterne.bin", "rb");
    FILE *fichierReservations = fopen("reservationsInterne.bin", "ab"); // Fichier des réservations (ajout binaire)
    if (fichierVoyages == NULL || fichierReservations == NULL) {
        printf("Erreur d'ouverture des fichiers.");
        return;
    }

    char depart[30], arrive[30];
    Date date;
    int nb_places;
    saisirCriteresRecherche(depart, arrive, &date, &nb_places);

    int nb_voyages_disponibles = afficherVoyagesDisponiblesInternes(fichierVoyages, depart, arrive, date, nb_places);
    if (nb_voyages_disponibles == 0) {
        printf("Aucun voyage disponible ne correspond à vos critères.");
        fclose(fichierVoyages);
        fclose(fichierReservations);
        return;
    }

    int choix_voyage = demanderChoixVoyage(nb_voyages_disponibles);
    if (choix_voyage == 0) {
        printf("Réservation annulée.");
        fclose(fichierVoyages);
        fclose(fichierReservations);
        return;
    }

    // Repositionner le fichier à la première position
    rewind(fichierVoyages);
    
    // Récupérer le voyage choisi
    VoyageInterne voyage;
    int index = 1;
    while (fread(&voyage, sizeof(VoyageInterne), 1, fichierVoyages)) {
        if (strcmp(voyage.aeroport_depart, depart) == 0 &&
            strcmp(voyage.aeroport_arrive, arrive) == 0 &&
            voyage.date_voyage.jour == date.jour &&
            voyage.date_voyage.mois == date.mois &&
            voyage.date_voyage.annee == date.annee &&
            voyage.nb_place >= nb_places) {

            if (index == choix_voyage) {
                float montant_totale=voyage.prix * nb_places;
                printf("\nVous avez choisi le voyage : %s\n", voyage.id);
                printf("\nMontant total à payer: %.2f MAD\n", montant_totale);
                
                // Demander la confirmation de la réservation
                char confirmation;
                printf("Confirmez-vous la réservation ? (O/N): ");
                scanf(" %c", &confirmation);

                if (confirmation == 'O' || confirmation == 'o') {
                    // Mettre à jour le nombre de places disponibles
                    voyage.nb_place -= nb_places;
                    
                    // Créer une structure de réservation
                    Reservation nouvelleReservation;
                    strcpy(nouvelleReservation.username, username);
                    strcpy(nouvelleReservation.voyage_id, voyage.id);
                    nouvelleReservation.nb_places_reservees = nb_places;
                    obtenir_date_du_jour(&nouvelleReservation.date_reservation);
                    nouvelleReservation.montant_paye = voyage.prix * nb_places;
                    
                    // Écrire la réservation dans le fichier des réservations
                    fwrite(&nouvelleReservation, sizeof(Reservation), 1, fichierReservations);
                    printf("\nRéservation effectuée avec succès !\n");
                    
                    // Écrire les modifications dans le fichier des voyages
                    FILE *fichierTemp = fopen("temp.bin", "wb");
                    if (fichierTemp == NULL) {
                        printf("Erreur lors de la mise à jour du fichier des voyages.");
                        fclose(fichierVoyages);
                        fclose(fichierReservations);
                        return;
                    }
                    
                    rewind(fichierVoyages);
                    VoyageInterne tempVoyage;
                    while (fread(&tempVoyage, sizeof(VoyageInterne), 1, fichierVoyages)) {
                        if (strcmp(tempVoyage.id, voyage.id) == 0) {
                            fwrite(&voyage, sizeof(VoyageInterne), 1, fichierTemp); // Ecrire le voyage mis à jour
                        } else {
                            fwrite(&tempVoyage, sizeof(VoyageInterne), 1, fichierTemp); // Ecrire les autres voyages
                        }
                    }
                    
                    fclose(fichierTemp);
                    fclose(fichierVoyages);
                    remove("voyagesInternes.bin");
                    rename("temp.bin", "voyagesInternes.bin");
                    fclose(fichierReservations);
                    return;
                } else {
                    printf("\nRéservation annulée.\n");
                    fclose(fichierVoyages);
                    fclose(fichierReservations);
                    return;
                }
            }
            index++;
        }
    }

    printf("Erreur : voyage non trouvé.");
    fclose(fichierVoyages);
    fclose(fichierReservations);
}

// Fonction pour réserver un voyage hors ligne pour les voyages externes
void ReserverVoyageExterneHorsLine(char *username) {
    FILE *fichierVoyages = fopen("voyageExterne.bin", "rb");
    FILE *fichierReservations = fopen("reservationsExterne.bin", "ab"); // Fichier des réservations externes (ajout binaire)
    if (fichierVoyages == NULL || fichierReservations == NULL) {
        printf("Erreur d'ouverture des fichiers.");
        return;
    }

    char pays_depart[30], pays_arrive[30];
    char aeroport_depart[30], aeroport_arrive[30];
    Date date;
    int nb_places;
    
    saisirCriteresRechercheExterne(pays_depart, pays_arrive, aeroport_depart, aeroport_arrive, &date, &nb_places);

    int nb_voyages_disponibles = afficherVoyagesDisponiblesExterne(fichierVoyages, pays_depart, pays_arrive, aeroport_depart, aeroport_arrive, date, nb_places);
    if (nb_voyages_disponibles == 0) {
        printf("Aucun voyage disponible ne correspond à vos critères.");
        fclose(fichierVoyages);
        fclose(fichierReservations);
        return;
    }

    int choix_voyage = demanderChoixVoyage(nb_voyages_disponibles);
    if (choix_voyage == 0) {
        printf("Réservation annulée.");
        fclose(fichierVoyages);
        fclose(fichierReservations);
        return;
    }

    // Repositionner le fichier à la première position
    rewind(fichierVoyages);
    
    // Récupérer le voyage choisi
    VoyageExterne voyage;
    int index = 1;
    while (fread(&voyage, sizeof(VoyageExterne), 1, fichierVoyages)) {
        if (strcmp(voyage.aeroport_depart, aeroport_depart) == 0 &&
            strcmp(voyage.aeroport_arrive, aeroport_arrive) == 0 &&
            strcmp(voyage.pays_depart, pays_depart) == 0 &&
            strcmp(voyage.pays_arrivee, pays_arrive) == 0 &&
            voyage.date_voyage.jour == date.jour &&
            voyage.date_voyage.mois == date.mois &&
            voyage.date_voyage.annee == date.annee &&
            voyage.nb_place >= nb_places) {

            if (index == choix_voyage) {
                printf("\nVous avez choisi le voyage : %s\n", voyage.id);
                printf("\nMontant total à payer: %.2f MAD\n", voyage.prix * nb_places);
                
                // Demander la confirmation de la réservation
                char confirmation;
                printf("Confirmez-vous la réservation ? (O/N): ");
                scanf(" %c", &confirmation);

                if (confirmation == 'O' || confirmation == 'o') {
                    // Mettre à jour le nombre de places disponibles
                    voyage.nb_place -= nb_places;
                    
                    // Créer une structure de réservation
                    Reservation nouvelleReservation;
                    strcpy(nouvelleReservation.username, username);
                    strcpy(nouvelleReservation.voyage_id, voyage.id);
                    nouvelleReservation.nb_places_reservees = nb_places;
                    nouvelleReservation.date_reservation = date;
                    nouvelleReservation.montant_paye = voyage.prix * nb_places;
                    
                    // Écrire la réservation dans le fichier des réservations
                    fwrite(&nouvelleReservation, sizeof(Reservation), 1, fichierReservations);
                    printf("\nRéservation effectuée avec succès !\n");
                    
                    // Écrire les modifications dans le fichier des voyages
                    FILE *fichierTemp = fopen("temp.bin", "wb");
                    if (fichierTemp == NULL) {
                        printf("Erreur lors de la mise à jour du fichier des voyages.");
                        fclose(fichierVoyages);
                        fclose(fichierReservations);
                        return;
                    }
                    
                    rewind(fichierVoyages);
                    VoyageExterne tempVoyage;
                    while (fread(&tempVoyage, sizeof(VoyageExterne), 1, fichierVoyages)) {
                        if (strcmp(tempVoyage.id, voyage.id) == 0) {
                            fwrite(&voyage, sizeof(VoyageExterne), 1, fichierTemp); // Ecrire le voyage mis à jour
                        } else {
                            fwrite(&tempVoyage, sizeof(VoyageExterne), 1, fichierTemp); // Ecrire les autres voyages
                        }
                    }
                    
                    fclose(fichierTemp);
                    fclose(fichierVoyages);
                    remove("voyagesExternes.bin");
                    rename("temp.bin", "voyagesExternes.bin");
                    fclose(fichierReservations);
                    return;
                } else {
                    printf("\nRéservation annulée.\n");
                    fclose(fichierVoyages);
                    fclose(fichierReservations);
                    return;
                }
            }
            index++;
        }
    }

    printf("Erreur : voyage non trouvé.");
    fclose(fichierVoyages);
    fclose(fichierReservations);
}

void ReserverVoyageHorsLine(char *username) {
    int choix;
    do {
        printf("\n===== Menu Principal =====\n");
        printf("1. Réserver un voyage interne\n");
        printf("2. Réserver un voyage externe\n");
        printf("0. Quitter\n");
        printf("Votre choix: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ReserverVoyageHorsLineInterne(username);
                break;
            case 2:
                ReserverVoyageExterneHorsLine(username);
                break;
            case 0:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
                break;
        }
    } while (choix != 0);
}

void MenueEmploye() {
    int selected_option = 0;
    int key;
    int x_centre = 40;  // Largeur fixe
    int y_centre = 5;   // Hauteur fixe, ajustable selon l'espace
    char *options[] = {"Gestion de Lignes", "Effectuer Reservation(s)", "Consulter Reservation(s)", "Deconnecter"};
    int num_options = 4;

    do {
        system("cls");

        // Affichage du titre avec les traits "gras"
        drawTitleWithBoldLine(x_centre, y_centre, "Menu Employé", 14);
        
        // Affichage des options du menu avec navigation par flèches
        for (int i = 0; i < num_options; i++) {
            gotoXY(x_centre -10, y_centre + 3 + i * 2);
            if (i == selected_option) {
                SetColor(10);  // Couleur verte pour l'option sélectionnée
                printf("> %s\n", options[i]);
            } else {
                SetColor(7);  // Couleur blanche par défaut
                printf("%s\n", options[i]);
            }
        }

        // Obtenir l'entrée de l'utilisateur
        key = _getch();
        if (key == 224) { // Touche flèche (haut ou bas)
            key = _getch();
            if (key == 72 && selected_option > 0) { // Flèche haut
                selected_option--;
            } else if (key == 80 && selected_option < num_options - 1) { // Flèche bas
                selected_option++;
            }
        } else if (key == 13) { // Touche Entrée
            switch (selected_option) {
                case 0:
                    system("cls");
                    gestionlignes();
                    break;

                case 1: {
                    system("cls");
                    char *username;
                    username = signUp();
                    ReserverVoyageHorsLine(username);
                    break;
                }

                case 2: {
                    system("cls");
                    ConsulterVoyages();
                    break;
                }

                case 3:
                    system("cls");
                    gotoXY(x_centre, y_centre + 8);
                    SetColor(7);  // Couleur blanche pour le message de déconnexion
                    printf("Au revoir !\n");
                    return; // Quitter la fonction directement

                default:
                    system("cls");
                    gotoXY(x_centre, y_centre + 8);
                    SetColor(12);  // Rouge pour l'erreur de saisie
                    printf("Choix invalide !\n");
                    Sleep(1500);  // Pause de 1,5 secondes avant de réafficher le menu
                    break;
            }
        }
    } while (1);
}



















void MenuePrincipal() {
    char *mainMenuOptions[] = {"User", "Admin", "Employe", "Exit", NULL};
    char *userMenuOptions[] = {"Sign Up", "Log In", "Exit", NULL};
    char *adminMenuOptions[] = {"Log In", "Exit", NULL};
    char *employeMenuOptions[] = {"Log In", "Exit", NULL};

    int choix;
    int choice;
    char *username;  // Indicateur de connexion réussie
    int loggedIn;

    int x_centre = 40;  // Largeur fixe
    int y_centre = 5;   // Hauteur fixe, ajustable selon l'espace

    do {
        choix = choose_item(mainMenuOptions, "  Etes-vous", x_centre, y_centre);

        switch (choix) {
            case 0: { // User
                system("cls");
                do {
                    choice = choose_item(userMenuOptions, "Menu Utilisateur", x_centre, y_centre);

                    switch (choice) {
                        case 0:
                            username = signUp();
                            if (username != NULL) {
                                menuPostConnexion(username);
                            }
                            break;
                        case 1:
                            username = login();
                            if (username != NULL) {
                                menuPostConnexion(username);
                            }
                            break;
                        case 2:
                            system("cls");
                            gotoXY(x_centre, y_centre + 8);
                            printf("Au revoir !\n");
                            break;
                    }
                } while (choice != 2);
                break;
            }

            case 1: { // Admin
                system("cls");
                do {
                    choice = choose_item(adminMenuOptions, "Menu Administrateur", x_centre, y_centre);

                    switch (choice) {
                        case 0:
                            loggedIn = loginAdmin();
                            if (loggedIn) {
                                MenuAdministrateur();
                            }
                            break;
                        case 1:
                            system("cls");
                            gotoXY(x_centre, y_centre + 8);
                            printf("Au revoir !\n");
                            break;
                    }
                } while (choice != 1);
                break;
            }

            case 2: { // Employe
                system("cls");
                do {
                    choice = choose_item(employeMenuOptions, "Menu Employé", x_centre, y_centre);

                    switch (choice) {
                        case 0:
                            loggedIn = loginEmploye();
                            if (loggedIn) {
                                MenueEmploye();
                            }
                            break;
                        case 1:
                            system("cls");
                            gotoXY(x_centre, y_centre + 8);
                            printf("Au revoir !\n");
                            break;
                    }
                } while (choice != 1);
                break;
            }

            case 3: // Exit
                system("cls");
                gotoXY(x_centre, y_centre + 8);
                printf("Au revoir !\n");
                exit(0);
        }
    } while (choix != 3);
}







int main() {
    system("chcp 850 > nul");
    SetConsoleOutputCP(CP_UTF8); // UTF-8 pour le support des accents
    MenuePrincipal();
    return 0;
}
