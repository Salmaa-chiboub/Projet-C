#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USERS 100
#define NAME_LENGTH 30
#define SURNAME_LENGTH 30
#define USERNAME_LENGTH 30
#define PASSWORD_LENGTH 30
#define EMAIL_LENGTH 30
#define PHONE_LENGTH 15
#define MAX_USERNAME_LENGTH 30
#define MAX_RESERVATIONS 100
#define ID_LENGTH 8
#define FILEuser "users.bin"
#define FILEadmin "admin.bin"
#define FILEvoyageInterne "voyageInterne.bin"
#define FILEvoyageExterne "voyageExterne.bin"





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

// Signup function
char* signUp() {
    static char username[USERNAME_LENGTH];  // Déclaration statique pour pouvoir le retourner
    User newUser;
    FILE *file = fopen(FILEuser, "ab+");  // Open the file in binary append mode

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return NULL;  // Retourner NULL en cas d'erreur d'ouverture
    }

    printf("Inscription:\n");

    // Collect user information
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

    // Check if the username already exists
    User tempUser;
    rewind(file);  // Rewind to the beginning of the file for checking
    while (fread(&tempUser, sizeof(User), 1, file)) {
        if (strcmp(tempUser.username, newUser.username) == 0) {
            printf("Ce nom d'utilisateur est déjà pris.\n");
            fclose(file);
            return NULL;  // Retourner NULL si le nom d'utilisateur existe déjà
        }
    }

    printf("Entrez un mot de passe: ");
    scanf("%s", newUser.password);

    // Write user information to the file
    fwrite(&newUser, sizeof(User), 1, file);
    fclose(file);

    // Store the username in the static variable
    strcpy(username, newUser.username);
    printf("Inscription réussie ! Bienvenue %s %s\n", newUser.nom, newUser.prenom);

    return username;  // Retourner le nom d'utilisateur après l'inscription
}




char* login() {
    static char username[USERNAME_LENGTH];
    User user;
    FILE *file = fopen(FILEuser, "rb");

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return NULL;
    }

    printf("Connexion:\n");
    printf("Entrez votre nom d'utilisateur: ");
    scanf("%s", user.username);
    printf("Entrez votre mot de passe: ");
    scanf("%s", user.password);


    User tempUser;
    while (fread(&tempUser, sizeof(User), 1, file)) {
        if (strcmp(tempUser.username, user.username) == 0 && strcmp(tempUser.password, user.password) == 0) {

            strcpy(username, tempUser.username);
            printf("Connexion réussie ! Bienvenue, %s %s.\n", tempUser.nom, tempUser.prenom);
            fclose(file);  // Fermer le fichier
            return username;  // Connexion réussie, retourner le username
        }
    }

    printf("Nom d'utilisateur ou mot de passe incorrect.\n");
    fclose(file);  // Fermer le fichier si l'authentification échoue
    return NULL;  // Connexion échouée
}


// Fonction pour afficher les lignes existantes
void afficherLignes() {
    int choix;

    do {
        printf("\n===========================\n");
        printf("  AFFICHAGE DES LIGNES\n");
        printf("===========================\n");
        printf("1. Lignes internes\n");
        printf("2. Lignes externes\n");
        printf("3. Retour\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                FILE *file = fopen(FILEvoyageInterne, "rb");
                if (!file) {
                    printf("\n⚠️ Erreur : Impossible d'ouvrir le fichier des lignes internes.\n");
                } else {
                    VoyageInterne voyage;
                    int found = 0;
                    printf("\n==========================================================================\n");
                    printf("           LIGNES INTERNES DISPONIBLES\n");
                    printf("==========================================================================\n");
                    printf("| %-10s | %-15s | %-15s | %-10s | %-8s |\n",
                           "ID", "Départ", "Arrivée", "Date", "Prix ($)");
                    printf("--------------------------------------------------------------------------\n");

                    while (fread(&voyage, sizeof(VoyageInterne), 1, file) == 1) {
                        found = 1;
                        printf("| %-10s | %-15s | %-15s | %02d/%02d/%04d | %-8d |\n",
                               voyage.id, voyage.aeroport_depart, voyage.aeroport_arrive,
                               voyage.date_voyage.jour, voyage.date_voyage.mois, voyage.date_voyage.annee,
                               voyage.prix);
                    }

                    if (!found) {
                        printf("\n⚠️ Aucun vol interne disponible pour le moment.\n");
                    }
                    printf("\n==========================================================================\n");
                    fclose(file);
                }
                break;
            }
            case 2: {
                FILE *file = fopen(FILEvoyageExterne, "rb");
                if (!file) {
                    printf("\n⚠️ Erreur : Impossible d'ouvrir le fichier des lignes externes.\n");
                } else {
                    VoyageExterne voyage;
                    int found = 0;
                    printf("\n==========================================================================\n");
                    printf("           LIGNES EXTERNES DISPONIBLES\n");
                    printf("==========================================================================\n");
                    printf("| %-10s | %-15s | %-15s | %-10s | %-8s |\n",
                           "ID", "Départ", "Arrivée", "Date", "Prix ($)");
                    printf("--------------------------------------------------------------------------\n");

                    while (fread(&voyage, sizeof(VoyageExterne), 1, file) == 1) {
                        found = 1;
                        printf("| %-10s | %-15s | %-15s | %02d/%02d/%04d | %-8d |\n",
                               voyage.id, voyage.aeroport_depart, voyage.aeroport_arrive,
                               voyage.date_voyage.jour, voyage.date_voyage.mois, voyage.date_voyage.annee,
                               voyage.prix);
                    }

                    if (!found) {
                        printf("\n⚠️ Aucun vol externe disponible pour le moment.\n");
                    }
                    printf("\n==========================================================================\n");
                    fclose(file);
                }
                break;
            }
            case 3:
                printf("\nRetour au menu principal.\n");
                break;
            default:
                printf("\n⚠️ Choix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 3);
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
    scanf(" %[^\n]", payement.titulaire_carte);// Pour accepter des espaces dans le nom du titulaire
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




// un menu pour choisir après le login ou le sign up
void menuPostConnexion(char* username) {
    int choix;
    do {
        printf("\nMenu principal :\n");
        printf("1. Afficher les lignes\n");
        printf("2. Chercher un voyage\n");
        printf("3. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficherLignes();
                break;
            case 2:
              ReserverVoyage(username);
                break;
            case 3:
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 3);
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

//fonction login admin
int loginAdmin() {
    User admin;
    FILE *file = fopen(FILEadmin, "rb"); // Ouvrir en mode binaire

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }

    printf("Connexion administrateur:\n");
    printf("Entrez votre Identifiant: ");
    scanf("%s", admin.username);
    printf("Entrez votre mot de passe: ");
    scanf("%s", admin.password);

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

    printf("Entrez le prix du billet ($): ");
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

    printf("Entrez le prix du billet ($): ");
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
    int choix;

    do {
        printf("\n1. Ajouter une ligne interne\n");
        printf("2. Ajouter une ligne externe \n");
        printf("3. Quitter\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                // Ouvre le fichier binaire en mode ajout ("ab")
                FILE *file = fopen(FILEvoyageInterne, "ab");
                if (!file) {
                    printf("Erreur lors de l'ouverture du fichier voyageInterne.bin\n");
                    return;
                }

                ajouterLigneInterne(file);  // Ajouter une ligne de vol interne
                fclose(file);
                break;
            }





            case 2: {
                FILE *file = fopen(FILEvoyageExterne, "a+");

                if (!file) {
                    printf("Erreur lors de l'ouverture du fichier voyageExterne.bin\n");
                    return;
                }

                ajouterLigneExterne(file);  // Ajouter une ligne de vol externe
                fclose(file);
                break;
            }

            case 3:
                printf("Retour au menu principal.\n");
                break;

            default:
                printf("Choix invalide! Veuillez réessayer.\n");
                break;
        }
    } while (choix != 3);
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
                    printf("Nouveau prix : ");
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


//Menu d'administrateur
void MenuAdministrateur(){
    int c;
    do{

                printf("\nMenu Administrateur:\n");
                printf("1/ Affichier les lignes\n");
                printf("2/ Ajout d'une ligne\n");
                printf("3/ Supprimer d'une ligne\n");
                printf("4/ Modifier les information de voyage \n");
                printf("5/ Consulter les reservations \n");
                printf("6/ QUITTER\n");
                printf("Donner votre choix : ");
                scanf("%d",&c);
                switch (c)
                {
                case 1:
                     afficherLignes();
                     break;
                case 2:{
                    //ajouter une ligne;
                    AjouterLigne();
                    break;
                }
                case 3:{
                    //supprimer une ligne
                    supprimerLigne();
                     break;
                }

                case 4:
                     //modifier un voyage;
                     modifierVoyage();
                     break;
                case 5:
                     ConsulterVoyages();
                     break;
                case 6:
                    //quitter
                     printf("Retour au programme principal.\n");
                     break;

               default:
                     printf("Choix invalide! Veuillez réessayer.\n");
                     break;
         }

            }while(c != 6);

}


















int main() {
    int choix;
    int choice;
    char* username;  // Indicateur de connexion réussie
     int loggedIn;

    do {
        printf("Etes-vous :\n");
        printf("              1. User\n");
        printf("              2. Admin\n");
        printf("              3. Exit\n");
        printf("Veuillez entrer votre choix: ");
        scanf("%d", &choix);


    switch (choix) {
        case 1:{
        // utilisateur
        printf("Bienvenue utilisateur!\n");

        do {
            printf("\nMenu:\n");
            printf("1. Sign Up\n");
            printf("2. Log In\n");
            printf("3. Exit\n");
            printf("Veuillez entrer votre choix: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    username = signUp();
                    // Après l'inscription, on affiche un menu spécifique
                    if(username!=NULL){
                        menuPostConnexion(username);
                    }

                    break;
                case 2:
                    username = login();
                    if(username!=NULL){
                        menuPostConnexion(username);
                    }

                    break;
                case 3:
                    printf("Au revoir !\n");
                    break;
                default:
                    printf("Choix invalide. Veuillez réessayer.\n");
            }
            } while (choice != 3);
            break;
        }

        case 2:{
            // administrateur
            printf("Bienvenue Administrateur!\n");
            do {
            printf("\nMenu:\n");
            printf("1. Log In\n");
            printf("2. Exit\n");
            printf("Veuillez entrer votre choix: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    loggedIn = loginAdmin();
                    if (loggedIn) {
                        MenuAdministrateur(); // Après la connexion réussie
                    }
                    break;
                case 2:

                    printf("Au revoir !\n");
                    break;
                default:
                    printf("Choix invalide. Veuillez réessayer.\n");
            }
            } while (choice != 2);

            break;
        }
        case 3:
            // quitter
            printf("Au revoir !\n");
            exit(0);

    }
} while (choix != 1 && choix != 2 && choix != 3);

    return 0;
}
