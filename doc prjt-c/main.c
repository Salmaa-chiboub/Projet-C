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
#define MAX_OFFRES 100 // Ajustez cette valeur selon vos besoins
#define MAX_TITRE 100
#define MAX_DESCRIPTION 500
#define MAX_NOM 50
#define MAX_EMAIL 100
#define MAX_CV 200
#define MAX_LETTRE 200







//styler
// Définition des couleurs
#define COLOR_RESET 7
#define COLOR_HIGHLIGHT 14  // Jaune
#define COLOR_BORDER 11    // Cyan clair
#define COLOR_TITLE 10     // Vert clair
#define COLOR_LOGO 13      // Magenta clair
#define COLOR_PRIMARY 9     // Bleu clair - couleur principale
#define COLOR_SECONDARY 3   // Cyan - couleur secondaire
#define COLOR_ACCENT 15     // Blanc brillant - pour les éléments sélectionnés
#define COLOR_INACTIVE 8    // Gris - pour les éléments non sélectionnés
#define COLOR_BORDER 9      // Bleu clair - pour les bordures
#define COLOR_TITLE 15      // Blanc brillant - pour les titres
#define COLOR_SEPARATOR 9


#define MAX_LENGTH 20
#define MIN_LENGTH 3
// Structure pour les options du menu
typedef struct {
    char* text;
    void (*action)();
} MenuItem;

// Fonction pour positionner le curseur
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Fonction pour changer la couleur du texte
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Fonction pour dessiner le séparateur
void drawSeparator(int startX, int width, int y) {
    setColor(COLOR_SEPARATOR);
    gotoxy(startX, y);
    printf("╠");
    for(int i = 1; i < width - 1; i++) {
        printf("═");
    }
    printf("═╣");
    setColor(COLOR_RESET);
}

// Fonction pour dessiner le logo
void drawLogo() {
    setColor(COLOR_PRIMARY);
    gotoxy(35, 3);
    printf("    █████╗ ███╗   ███╗ █████╗ ███╗   ██╗██╗     ██╗███╗   ██╗███████╗███████╗");
    gotoxy(35, 4);
    printf("   ██╔══██╗████╗ ████║██╔══██╗████╗  ██║██║     ██║████╗  ██║██╔════╝██╔════╝");
    gotoxy(35, 5);
    printf("   ███████║██╔████╔██║███████║██╔██╗ ██║██║     ██║██╔██╗ ██║█████╗  ███████╗");
    gotoxy(35, 6);
    printf("   ██╔══██║██║╚██╔╝██║██╔══██║██║╚██╗██║██║     ██║██║╚██╗██║██╔══╝  ╚════██║");
    gotoxy(35, 7);
    printf("   ██║  ██║██║ ╚═╝ ██║██║  ██║██║ ╚████║███████╗██║██║ ╚████║███████╗███████║");
    gotoxy(35, 8);
    printf("   ╚═╝  ╚═╝╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝╚═╝╚═╝  ╚═══╝╚══════╝╚══════╝");
    setColor(COLOR_RESET);
}


// Fonction pour dessiner le cadre
void drawFrame(int startX, int startY, int width, int height) {
    setColor(COLOR_BORDER);

    // Coins
    gotoxy(startX, startY); printf("╔");
    gotoxy(startX + width, startY); printf("╗");
    gotoxy(startX, startY + height); printf("╚");
    gotoxy(startX + width, startY + height); printf("╝");

    // Lignes horizontales
    for(int i = 1; i < width; i++) {
        gotoxy(startX + i, startY); printf("═");
        gotoxy(startX + i, startY + height); printf("═");
    }

    // Lignes verticales
    for(int i = 1; i < height; i++) {
        gotoxy(startX, startY + i); printf("║");
        gotoxy(startX + width, startY + i); printf("║");
    }

    setColor(COLOR_RESET);
}

// Fonction pour afficher une option du menu
void drawMenuItem(const char* text, int x, int y, int isSelected) {
    gotoxy(x, y);
    if(isSelected) {
        setColor(COLOR_ACCENT);
        printf("  ►  %s", text);
    } else {
        setColor(COLOR_INACTIVE);
        printf("     %s", text);
    }
    setColor(COLOR_RESET);
}
// Fonction pour afficher la ligne d'attente animée
void drawWaitingAnimation(int x, int y, const char* message) {
    const char* dots[] = {".  ", ".. ", "..."};
    setColor(14);  // Jaune pour le message
    
    for(int i = 0; i < 6; i++) {  // 6 cycles d'animation
        gotoxy(x, y);
        printf("%s%s", message, dots[i % 3]);
        Sleep(200);  // Délai entre chaque point
        
        // Effacer la ligne
        gotoxy(x, y);
        printf("                                                  ");
    }
}


// Fonction pour dessiner un champ de saisie
void drawInputField(const char* label, int y, char* buffer, int isPassword) {
    // Label
    setColor(9);
    gotoxy(30, y+1);
    printf("► %s", label);

    // Champ de saisie avec un cadre simple
    setColor(COLOR_BORDER);
    gotoxy(50, y );
    printf("╭────────────────────────────────────────╮");
    gotoxy(50, y + 1);
    printf("│                                        │");
    gotoxy(50, y + 2);
    printf("╰────────────────────────────────────────╯");

    // Position du curseur pour la saisie
    gotoxy(53, y + 1);
    setColor(COLOR_ACCENT);

    if (isPassword) {
        hide_input(buffer);
    } else {
        scanf("%s", buffer);
    }
    setColor(COLOR_RESET);
}



// Fonction pour vérifier si une chaîne ne contient que des lettres
int isAlphaString(const char* str) {
    for(int i = 0; str[i]; i++) {
        if (!isalpha(str[i]) && str[i] != ' ' && str[i] != '-') {
            return 0;
        }
    }
    return 1;
}

// Fonction pour vérifier le format de l'email
int isValidEmail(const char* email) {
    int atFound = 0;
    int dotFound = 0;
    int atPosition = -1;
    int length = strlen(email);

    if(length < 5) return 0; // a@b.c minimum

    for(int i = 0; i < length; i++) {
        if(email[i] == '@') {
            if(atFound || i == 0) return 0; // @ déjà trouvé ou au début
            atFound = 1;
            atPosition = i;
        }
        else if(email[i] == '.') {
            if(atPosition != -1 && i > atPosition) dotFound = 1;
        }
    }

    return atFound && dotFound;
}

void showError(const char* message, int y) {
    setColor(12);
    gotoxy(45, y);  // Position à droite du champ de saisie
    printf("* %s", message);
    setColor(COLOR_RESET);
}

// Fonction pour effacer un message d'erreur
void clearError(int y) {
    gotoxy(45, y);
    printf("%-50s", "      "); // Efface le message
}



// Fonctions de validation
int estDateValide(int jour, int mois, int annee) {
    // Vérifier l'année (supposons entre 2024 et 2030)
    if (annee < 2024 || annee > 2030) return 0;
    
    // Vérifier le mois
    if (mois < 1 || mois > 12) return 0;
    
    // Tableau des jours par mois (0 = non utilisé)
    int joursParMois[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Ajuster février pour les années bissextiles
    if (annee % 4 == 0 && (annee % 100 != 0 || annee % 400 == 0))
        joursParMois[2] = 29;
    
    // Vérifier le jour
    if (jour < 1 || jour > joursParMois[mois]) return 0;
    
    return 1;
}

int estHeureValide(int heure, int minute) {
    return (heure >= 0 && heure < 24 && minute >= 0 && minute < 60);
}

int estPrixValide(float prix) {
    return (prix > 0 && prix < 100000); // Prix maximum raisonnable
}

int estPlacesValide(int places) {
    return (places > 0 && places <= 500); // Maximum raisonnable de places
}

void afficherMessageErreur(const char* message, int CENTER_X) {
    setColor(12);
    gotoxy(CENTER_X - (strlen(message) / 2), 25);
    printf("%s", message);
    Sleep(2000);
}


// Fonction utilitaire pour effacer la zone de saisie
void clearInputArea(int CENTER_X, int startY, int endY) {
    for(int i = startY; i <= endY; i++) {
        gotoxy(CENTER_X - 25, i);
        printf("                                                  ");
    }
}

// Fonction pour dessiner un cadre de saisie spécial pour date/heure
void drawDateInputField(const char* label, int y, int CENTER_X, int* jour, int* mois, int* annee) {
    int startX = CENTER_X - 15;
    char j[3] = {0}, m[3] = {0}, a[5] = {0};
    
    setColor(13);
    // Cadre principal
    gotoxy(startX, y);
    printf("┌─────────────────────────────────────────────┐");
    gotoxy(startX, y + 1);
    printf("│  Date de voyage                             │");
    gotoxy(startX, y + 2);
    printf("│  ┌──────┐   ┌──────┐   ┌──────────┐         │");
    gotoxy(startX, y + 3);
    printf("│  │      │   │      │   │          │         │");
    gotoxy(startX, y + 4);
    printf("│  └──────┘   └──────┘   └──────────┘         │");
    gotoxy(startX, y + 5);
    printf("│   JOUR       MOIS       ANNÉE               │");
    gotoxy(startX, y + 6);
    printf("└─────────────────────────────────────────────┘");

    // Saisie du jour
    setColor(15);
    gotoxy(startX + 5, y + 3);
    scanf("%2s", j);
    *jour = atoi(j);

    // Saisie du mois
    gotoxy(startX + 16, y + 3);
    scanf("%2s", m);
    *mois = atoi(m);

    // Saisie de l'année
    gotoxy(startX + 27, y + 3);
    scanf("%4s", a);
    *annee = atoi(a);
}

void drawTimeInputField(const char* label, int y, int CENTER_X, int* heure, int* minute) {
    int startX = CENTER_X - 15;
    char h[3] = {0}, m[3] = {0};
    
    setColor(13);
    // Cadre principal
    gotoxy(startX, y);
    printf("┌─────────────────────────────────────────────┐");
    gotoxy(startX, y + 1);
    printf("│%-46s│",label);
    gotoxy(startX, y + 2);
    printf("│  ┌──────┐     ┌──────┐                      │");
    gotoxy(startX, y + 3);
    printf("│  │      │  :  │      │                      │");
    gotoxy(startX, y + 4);
    printf("│  └──────┘     └──────┘                      │");
    gotoxy(startX, y + 5);
    printf("│   HEURES     MINUTES                        │");
    gotoxy(startX, y + 6);
    printf("└─────────────────────────────────────────────┘");

    // Saisie de l'heure
    setColor(15);
    gotoxy(startX + 5, y + 3);
    scanf("%2s", h);
    *heure = atoi(h);

    // Saisie des minutes
    gotoxy(startX + 18, y + 3);
    scanf("%2s", m);
    *minute = atoi(m);
}

// Fonction pour afficher un message d'aide
void afficherAideDate(int CENTER_X, int y) {
    setColor(8);  // Gris pour le texte d'aide
    gotoxy(CENTER_X - 20, y + 7);
    printf("Format: JJ/MM/AAAA  (Ex: 25/12/2024)");
}

void afficherAideHeure(int CENTER_X, int y) {
    setColor(8);  // Gris pour le texte d'aide
    gotoxy(CENTER_X - 20, y + 7);
    printf("Format: HH:MM  (Ex: 14:30)");
}

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

// Structure pour le contrat
typedef struct {
    char id_contrat[20];        // Identifiant unique du contrat
    char date_debut[15];        // Format: JJ/MM/AAAA
    char date_fin[15];          // Format: JJ/MM/AAAA
    float montant;              // Montant du contrat
    int statut;                 // 1 = actif, 0 = terminé
} Contrat;

// Structure pour la compagnie aérienne
typedef struct {
    char nom[30];              // Nom de la compagnie
    char pays[30];             // Pays d'origine
    int annee_creation;        // Année de création
    int nb_avions;             // Nombre d'avions dans la flotte
    char logo_path[100];       // Chemin vers le logo de la compagnie
    char email_contact[50];    // Email de contact
    char telephone[20];        // Numéro de téléphone
    int statut;               // 1 = actif, 0 = inactif
    Contrat contrat;          // Contrat associé à la compagnie
} Compagnie;

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
// structure pour les offres existants
typedef struct {
    char id[20]; // Identifiant unique de l'offre
    char titre[MAX_TITRE];
    char description[MAX_DESCRIPTION];
    char entreprise[MAX_TITRE];
    char localisation[MAX_TITRE];
    char type_contrat[20];
    int salaire;

} OffreEmploi;

// structure pour les condidatures
typedef struct {
    char offre_id[20];
    char nom[50];
    char prenom[50];
    char email[100];
    char telephone[20];
    char niveau_etude[50];    // Licence, Master, Doctorat, etc.
    char specialite[100];     // Domaine d'étude
    char experience[500];     // Description de l'expérience professionnelle
    char competences[500];    // Liste des compétences
    char langues[200];        // Langues maîtrisées
    char date_candidature[20];
    char statut[20];         // "En attente", "Acceptée", "Refusée"
    char cv_path[200];       // Chemin vers le fichier CV
} Candidature;




void rechercherEmploi(char* username) {
    // Afficher d'abord les offres disponibles
    afficherOffres();
    
    // Demander à l'utilisateur s'il veut postuler
    int currentChoice = 0;
    char choix;
    const char* options[] = {"Oui", "Non"};
    
    do {
        system("cls");
        drawFrame(20, 1, 100, 30);
        drawLogo();
        drawSeparator(20, 100, 10);

        setColor(9);
        gotoxy(20, 12);
        printf("╠═════════════════════════════════════════ POSTULER À UNE OFFRE ════════════════════════════════════════╣");
        
        // Afficher la question en plus grand et centré
        setColor(13);
        gotoxy(45, 15);
        printf("╔══════════════════════════════════════════════════╗");
        gotoxy(45, 16);
        printf("║                                                  ║");
        gotoxy(45, 17);
        printf("║          Voulez-vous postuler à une offre ?      ║");
        gotoxy(45, 18);
        printf("║                                                  ║");
        gotoxy(45, 19);
        printf("╚══════════════════════════════════════════════════╝");

        // Afficher les options centrées
        for(int i = 0; i < 2; i++) {
            gotoxy(55 + i*20, 21);
            if(i == currentChoice) {
                setColor(11);
                printf("►  %s  ◄", options[i]);
            } else {
                setColor(15);
                printf("   %s   ", options[i]);
            }
        }

        // Navigation
        choix = _getch();
        if(choix == 75 && currentChoice > 0) { // Flèche gauche
            currentChoice--;
        }
        else if(choix == 77 && currentChoice < 1) { // Flèche droite
            currentChoice++;
        }
        else if(choix == 13) { // Entrée
            if(currentChoice == 1) { // Non
                return;
            }
            break; // Oui
        }
    } while(1);

    int pageFormulaire = 0;
    Candidature candidature;
    
    // Initialiser les champs
    memset(&candidature, 0, sizeof(Candidature));

    do {
        system("cls");
        drawFrame(20, 1, 100, 30);
        drawLogo();
        drawSeparator(20, 100, 10);

        setColor(9);
        gotoxy(20, 12);
        if (pageFormulaire == 0) {
            printf("╠═══════════════════════════════════ INFORMATIONS PERSONNELLES ═══════════════════════════════╣");
            
            setColor(15);
            // Afficher les labels
            gotoxy(25, 15);
            printf("👤 Nom:");
            gotoxy(37, 15);
            printf("%-40s", candidature.nom);

            gotoxy(25, 18);
            printf("👤 Prénom:");
            gotoxy(37, 18);
            printf("%-40s", candidature.prenom);

            gotoxy(25, 21);
            printf("📧 Email:");
            gotoxy(37, 21);
            printf("%-40s", candidature.email);

            gotoxy(25, 24);
            printf("📞 Téléphone:");
            gotoxy(37, 24);
            printf("%-40s", candidature.telephone);

            // Afficher les messages d'erreur en rouge
            setColor(12);
            if(strlen(candidature.nom) > 0 && (strlen(candidature.nom) < 2 || !isAlphaString(candidature.nom))) {
                gotoxy(37, 16);
                printf("❌ Le nom doit contenir au moins 2 caractères alphabétiques");
            }
            if(strlen(candidature.prenom) > 0 && (strlen(candidature.prenom) < 2 || !isAlphaString(candidature.prenom))) {
                gotoxy(37, 19);
                printf("❌ Le prénom doit contenir au moins 2 caractères alphabétiques");
            }
            if(strlen(candidature.email) > 0 && !isValidEmail(candidature.email)) {
                gotoxy(37, 22);
                printf("❌ Format d'email invalide");
            }
            if(strlen(candidature.telephone) > 0 && strlen(candidature.telephone) < 8) {
                gotoxy(37, 25);
                printf("❌ Numéro de téléphone invalide");
            }

        } else {
            printf("╠═══════════════════════════════════ FORMATION ET EXPÉRIENCE ════════════════════════════════╣");
            
            setColor(15);
            gotoxy(25, 15);
            printf("🎓 Niveau d'études:");
            gotoxy(45, 15);
            printf("%-50s", candidature.niveau_etude);

            gotoxy(25, 18);
            printf("📚 Spécialité:");
            gotoxy(45, 18);
            printf("%-50s", candidature.specialite);

            gotoxy(25, 21);
            printf("💼 Expérience:");
            gotoxy(45, 21);
            printf("%-50s", candidature.experience);

            gotoxy(25, 24);
            printf("🔧 Compétences:");
            gotoxy(45, 24);
            printf("%-50s", candidature.competences);
        }

        // Barre de navigation
        setColor(13);
        gotoxy(20, 27);
        printf("╠════════════════════════════════════════════════════════════════════════════════════════╣");
        gotoxy(25, 28);
        if (pageFormulaire == 0) {
            printf("← (ESC) Retour | (→) Page Suivante | (TAB) Champ suivant | (ENTRÉE) Modifier");
        } else {
            printf("(←) Page Précédente | (ENTRÉE) Soumettre | (ESC) Retour | (TAB) Champ suivant");
        }

        choix = _getch();
        if (choix == 27) { // ESC
            return;
        } else if (choix == 77 && pageFormulaire == 0) { // Flèche droite
            if(strlen(candidature.nom) >= 2 && isAlphaString(candidature.nom) &&
               strlen(candidature.prenom) >= 2 && isAlphaString(candidature.prenom) &&
               isValidEmail(candidature.email) && strlen(candidature.telephone) >= 8) {
                pageFormulaire = 1;
            }
        } else if (choix == 75 && pageFormulaire == 1) { // Flèche gauche
            pageFormulaire = 0;
        } else if (choix == 13) { // Entrée
            char buffer[500];
            if (pageFormulaire == 0) {
                // Mode édition pour la première page
                gotoxy(37, 15);
                printf("%-40s", "");
                gotoxy(37, 15);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if(strlen(buffer) > 0) strcpy(candidature.nom, buffer);

                gotoxy(37, 18);
                printf("%-40s", "");
                gotoxy(37, 18);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if(strlen(buffer) > 0) strcpy(candidature.prenom, buffer);

                gotoxy(37, 21);
                printf("%-40s", "");
                gotoxy(37, 21);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if(strlen(buffer) > 0) strcpy(candidature.email, buffer);

                gotoxy(37, 24);
                printf("%-40s", "");
                gotoxy(37, 24);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if(strlen(buffer) > 0) strcpy(candidature.telephone, buffer);
            } else {
                // Mode édition pour la deuxième page
                gotoxy(45, 15);
                printf("%-50s", "");
                gotoxy(45, 15);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if(strlen(buffer) > 0) strcpy(candidature.niveau_etude, buffer);

                gotoxy(45, 18);
                printf("%-50s", "");
                gotoxy(45, 18);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if(strlen(buffer) > 0) strcpy(candidature.specialite, buffer);

                gotoxy(45, 21);
                printf("%-50s", "");
                gotoxy(45, 21);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if(strlen(buffer) > 0) strcpy(candidature.experience, buffer);

                gotoxy(45, 24);
                printf("%-50s", "");
                gotoxy(45, 24);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                if(strlen(buffer) > 0) strcpy(candidature.competences, buffer);

                // Vérification et soumission du formulaire
                if(strlen(candidature.niveau_etude) >= 2 && strlen(candidature.specialite) >= 2 && 
                   strlen(candidature.experience) >= 2 && strlen(candidature.competences) >= 2) {
                    
                    // Préparation de la candidature
                    time_t now = time(NULL);
                    struct tm *tm = localtime(&now);
                    strftime(candidature.date_candidature, sizeof(candidature.date_candidature), "%Y-%m-%d", tm);
                    strcpy(candidature.statut, "En attente");
                    
                    // Tentative d'ajout de la candidature
                    if (ajouterCandidature(&candidature)) {
                        // Écran de confirmation
                        system("cls");
                        drawFrame(20, 1, 100, 30);
                        drawLogo();
                        drawSeparator(20, 100, 10);

                        setColor(9);
                        gotoxy(20, 12);
                        printf("╠═══════════════════════════════════ CONFIRMATION ═══════════════════════════════════════╣");
                        
                        setColor(10);
                        gotoxy(35, 15);
                        printf("✅ Votre candidature a été enregistrée avec succès!");
                        
                        setColor(15);
                        gotoxy(35, 17);
                        printf("📋 Détails de la candidature:");
                        
                        gotoxy(35, 19);
                        printf("👤 Nom: %s %s", candidature.nom, candidature.prenom);
                        
                        gotoxy(35, 20);
                        printf("📧 Email: %s", candidature.email);
                        
                        gotoxy(35, 21);
                        printf("📞 Téléphone: %s", candidature.telephone);
                        
                        gotoxy(35, 22);
                        printf("🎓 Formation: %s en %s", candidature.niveau_etude, candidature.specialite);
                        
                        gotoxy(35, 23);
                        printf("📅 Date: %s", candidature.date_candidature);
                        
                        gotoxy(35, 24);
                        printf("📌 Statut: %s", candidature.statut);

                        setColor(13);
                        gotoxy(30, 26);
                        printf("Votre candidature a été enregistrée et sera examinée prochainement.");
                        
                        setColor(15);
                        gotoxy(35, 28);
                        printf("Appuyez sur une touche pour continuer...");
                        _getch();
                        return;
                    } else {
                        setColor(12);
                        gotoxy(35, 26);
                        printf("❌ Erreur lors de l'enregistrement de la candidature");
                        gotoxy(35, 27);
                        printf("Veuillez réessayer plus tard.");
                        Sleep(2000);
                        return;
                    }
                } else {
                    setColor(12);
                    gotoxy(35, 26);
                    printf("❌ Veuillez remplir tous les champs correctement");
                    Sleep(2000);
                }
            }
        }
    } while(1);
}













void drawNavigationBar(int left, int right, int y, const char* message) {
    const int width = right - left;
    const int messageLen = strlen(message);
    const int startX = left + (width - messageLen) / 2;

    setColor(13);
    gotoxy(left, y);
    printf("╠");
    for(int i = left + 1; i < right; i++) printf("═");
    printf("╣");

    gotoxy(left, y + 1);
    printf("║");
    setColor(15);
    printf("%*s%s%*s", (startX - left - 1), "", message, 
           (right - startX - messageLen - 1), "");
    setColor(13);
    gotoxy(right, y + 1);
    printf("║");

    gotoxy(left, y + 2);
    printf("╚");
    for(int i = left + 1; i < right; i++) printf("═");
    printf("╝");
}

int ajouterCandidature(Candidature *candidature) {
    FILE *file = fopen("candidatures.bin", "ab");
    if (file == NULL) {
        return 0;
    }

    int success = fwrite(candidature, sizeof(Candidature), 1, file) == 1;
    fclose(file);
    return success;
}

void afficherConfirmationCandidature(Candidature *candidature) {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);

    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═══════════════════════════════════ CONFIRMATION ═══════════════════════════════════════╣");
    
    // Cadre de confirmation
    setColor(11);
    gotoxy(CENTER_X - 30, 14);
    printf("┌────────────────────────────────────────────────────┐");
    gotoxy(CENTER_X - 30, 15);
    printf("│          Candidature enregistrée avec succès       │");
    gotoxy(CENTER_X - 30, 16);
    printf("└────────────────────────────────────────────────────┘");

    // Détails de la candidature
    setColor(15);
    gotoxy(CENTER_X - 25, 18);
    printf("Informations personnelles:");
    gotoxy(CENTER_X - 25, 19);
    printf("Nom complet  : %s %s", candidature->nom, candidature->prenom);
    gotoxy(CENTER_X - 25, 20);
    printf("Email        : %s", candidature->email);
    gotoxy(CENTER_X - 25, 21);
    printf("Téléphone    : %s", candidature->telephone);

    gotoxy(CENTER_X - 25, 23);
    printf("Formation et expérience:");
    gotoxy(CENTER_X - 25, 24);
    printf("Niveau       : %s en %s", candidature->niveau_etude, candidature->specialite);
    gotoxy(CENTER_X - 25, 25);
    printf("Date         : %s", candidature->date_candidature);
    gotoxy(CENTER_X - 25, 26);
    printf("Statut       : %s", candidature->statut);

    // Barre de navigation
    drawNavigationBar(FRAME_LEFT, FRAME_RIGHT, 28, "Appuyez sur une touche pour continuer...");
    _getch();
}

void afficherErreurCandidature() {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);

    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═══════════════════════════════════ ERREUR ═══════════════════════════════════════╣");

    // Message d'erreur
    setColor(12);
    gotoxy(CENTER_X - 30, 15);
    printf("┌────────────────────────────────────────────────────┐");
    gotoxy(CENTER_X - 30, 16);
    printf("│     Erreur lors de l'enregistrement               │");
    gotoxy(CENTER_X - 30, 17);
    printf("│     de la candidature                             │");
    gotoxy(CENTER_X - 30, 18);
    printf("└────────────────────────────────────────────────────┘");

    setColor(15);
    gotoxy(CENTER_X - 25, 20);
    printf("Causes possibles:");
    gotoxy(CENTER_X - 25, 21);
    printf("- Problème d'accès au fichier");
    gotoxy(CENTER_X - 25, 22);
    printf("- Espace disque insuffisant");
    gotoxy(CENTER_X - 25, 23);
    printf("- Erreur système");

    setColor(14);
    gotoxy(CENTER_X - 25, 25);
    printf("Veuillez réessayer plus tard ou contacter le support.");

    // Barre de navigation
    drawNavigationBar(FRAME_LEFT, FRAME_RIGHT, 28, "Appuyez sur une touche pour continuer...");
    _getch();
}


void remplirFormulaireCandidature(Candidature *candidature) {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    int pageFormulaire = 0;
    char buffer[500];

    do {
        system("cls");
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        if (pageFormulaire == 0) {
            printf("╠═══════════════════════════════════ INFORMATIONS PERSONNELLES ═══════════════════════════════╣");
            
            setColor(15);
            drawInputField("Nom", 15, candidature->nom, 0);
            drawInputField("Prénom", 18, candidature->prenom, 0);
            drawInputField("Email", 21, candidature->email, 0);
            drawInputField("Téléphone", 24, candidature->telephone, 0);

            // Afficher les messages d'erreur en rouge
            setColor(12);
            if(strlen(candidature->nom) > 0 && (strlen(candidature->nom) < 2 || !isAlphaString(candidature->nom))) {
                gotoxy(CENTER_X - 20, 16);
                printf("Le nom doit contenir au moins 2 caractères alphabétiques");
            }
            if(strlen(candidature->prenom) > 0 && (strlen(candidature->prenom) < 2 || !isAlphaString(candidature->prenom))) {
                gotoxy(CENTER_X - 20, 19);
                printf("Le prénom doit contenir au moins 2 caractères alphabétiques");
            }
            if(strlen(candidature->email) > 0 && !isValidEmail(candidature->email)) {
                gotoxy(CENTER_X - 20, 22);
                printf("Format d'email invalide");
            }
            if(strlen(candidature->telephone) > 0 && strlen(candidature->telephone) < 8) {
                gotoxy(CENTER_X - 20, 25);
                printf("Numéro de téléphone invalide");
            }

        } else {
            printf("╠═══════════════════════════════════ FORMATION ET EXPÉRIENCE ════════════════════════════════╣");
            
            setColor(15);
            drawInputField("Niveau d'études", 15, candidature->niveau_etude, 0);
            drawInputField("Spécialité", 18, candidature->specialite, 0);
            drawInputField("Expérience", 21, candidature->experience, 0);
            drawInputField("Compétences", 24, candidature->competences, 0);
        }

        // Barre de navigation
        setColor(11);
        drawNavigationBar(FRAME_LEFT, FRAME_RIGHT, 27, 
            pageFormulaire == 0 ? "ESC: Retour | →: Suivant | ENTRÉE: Modifier" :
                                "←: Précédent | ENTRÉE: Soumettre | ESC: Retour");

        char choix = _getch();
        if (choix == 27) { // ESC
            return;
        } else if (choix == 77 && pageFormulaire == 0) { // Flèche droite
            if(strlen(candidature->nom) >= 2 && isAlphaString(candidature->nom) &&
               strlen(candidature->prenom) >= 2 && isAlphaString(candidature->prenom) &&
               isValidEmail(candidature->email) && strlen(candidature->telephone) >= 8) {
                pageFormulaire = 1;
            }
        } else if (choix == 75 && pageFormulaire == 1) { // Flèche gauche
            pageFormulaire = 0;
        } else if (choix == 13) { // Entrée
            if (pageFormulaire == 1 &&
                strlen(candidature->niveau_etude) >= 2 && 
                strlen(candidature->specialite) >= 2 && 
                strlen(candidature->experience) >= 2 && 
                strlen(candidature->competences) >= 2) {
                return;
            }
        }
    } while(1);
}






FILE* Ouvrir_Fichier(const char *nom_fichier, const char *mode) {
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    
    FILE *fichier = fopen(nom_fichier, mode);
    if (fichier == NULL) {
        // Affichage du message d'erreur stylisé
        system("cls");
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═══════════════════════════════════════ ERREUR SYSTÈME ════════════════════════════════════════════╣");

        setColor(12); // Rouge pour l'erreur
        gotoxy(CENTER_X - 25, 16);
        printf("✗ Impossible d'ouvrir le fichier : %s", nom_fichier);
        gotoxy(CENTER_X - 25, 17);
        printf("Mode d'ouverture : %s", mode);

        // Message d'instruction
        setColor(13);
        const char* error_msg = "Appuyez sur une touche pour continuer...";
        gotoxy(CENTER_X - (strlen(error_msg) / 2) + 8, 26);
        printf("%s", error_msg);

        getch(); // Attendre une touche
        system("cls");
        return NULL;
    }
    return fichier;
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

    // Configuration initiale
    system("cls");

    // Dessiner le cadre principal
    drawFrame(20, 1, 100, 30);

    // Dessiner le logo
    drawLogo();

    // Dessiner le séparateur
    drawSeparator(20, 100, 10);

    // Titre
    setColor(9);
    gotoxy(20,13);
    printf("╠══════════════════════════════════════════ INSCRIPTION ════════════════════════════════════════════╣");

    // Saisie et validation du nom
    do {
        clearError(14);
        drawInputField("Nom", 14, newUser.nom, 0);
        if(strlen(newUser.nom) > MAX_LENGTH) {
            showError("Le nom ne doit pas dépasser 20 caractères", 17);
            continue;
        }
        if(!isAlphaString(newUser.nom)) {
            showError("Le nom ne doit contenir que des lettres", 17);
            continue;
        }
        break;
    } while(1);
    clearError(17);

    // Saisie et validation du prénom
    do {
        clearError(18);
        drawInputField("Prénom", 18, newUser.prenom, 0);
        if(strlen(newUser.prenom) > MAX_LENGTH) {
            showError("Le prénom ne doit pas dépasser 20 caractères", 21);
            continue;
        }
        if(!isAlphaString(newUser.prenom)) {
            showError("Le prénom ne doit contenir que des lettres", 21);
            continue;
        }
        break;
    } while(1);
    clearError(21);

    // Saisie et validation de l'email
    do {
        clearError(22);
        drawInputField("Email", 22, newUser.email, 0);
        if(strlen(newUser.email) > MAX_LENGTH * 2) {
            showError("L'email est trop long       ", 25);
            continue;
        }
        if(!isValidEmail(newUser.email)) {
            showError("Format d'email invalide    ", 25);
            continue;
        }
        break;
    } while(1);
    clearError(25);

    // Saisie et validation du nom d'utilisateur
    do {
        clearError(26);
        drawInputField("username ", 26, newUser.username, 0);
        if(strlen(newUser.username) > MAX_LENGTH) {
            showError("Le nom d'utilisateur ne doit pas dépasser 20 caractères", 29);
            continue;
        }
        if(strlen(newUser.username) < MIN_LENGTH) {
            showError("Le nom d'utilisateur doit avoir au moins 3 caractères", 29);
            continue;
        }

        // Vérifier si le nom d'utilisateur existe déjà
        User tempUser;
        rewind(file);
        int exists = 0;
        while (fread(&tempUser, sizeof(User), 1, file)) {
            if (strcmp(tempUser.username, newUser.username) == 0) {
                showError("Ce nom d'utilisateur est déjà pris", 29);
                exists = 1;
                break;
            }
        }
        if (exists) continue;
        break;
    } while(1);
    clearError(29);

    // Configuration initiale pour la page du mot de passe
    system("cls");
    drawFrame(20, 1, 100, 30);
    drawLogo();
     drawSeparator(20, 100, 10);

    // Titre
    setColor(9);
    gotoxy(20,12);
    printf("╠══════════════════════════════════════════ INSCRIPTION ════════════════════════════════════════════╣");

    // Saisie et validation du mot de passe
    do {
        clearError(16);

        drawInputField("pasworld ", 16, newUser.password, 1);
        if(strlen(newUser.password) > MAX_LENGTH) {
            showError("Le mot de passe ne doit pas dépasser 20 caractères", 19);
            continue;
        }
        if(strlen(newUser.password) < MIN_LENGTH) {
            showError("Le mot de passe doit avoir au moins 3 caractères", 19);
            continue;
        }
        clearError(19);

        clearError(20);
        drawInputField("Confirmation passworld", 20, confirmermotdepasse, 1);
        if(strcmp(newUser.password, confirmermotdepasse) != 0) {
            showError("Les mots de passe ne correspondent pas", 23);
            continue;
        }
        break;
    } while(1);
    clearError(23);
    clearError(19);


    // Écrire les informations dans le fichier
    fwrite(&newUser, sizeof(User), 1, file);
    fclose(file);

    strcpy(username, newUser.username);

    // Message de succès

    setColor(10);
    gotoxy(45, 26);
    printf("Inscription réussie ! Bienvenue %s %s", newUser.nom, newUser.prenom);
    Sleep(3000);
    system("cls");

    return username;
}


char* login() {
    static char username[USERNAME_LENGTH];
    User user;
    FILE *file = fopen(FILEuser, "rb");

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return NULL;
    }

    // Configuration initiale
    system("cls");

    // Dessiner le cadre principal
    drawFrame(20, 1, 100, 30);

    // Dessiner le logo
    drawLogo();

    // Dessiner le séparateur
    drawSeparator(20, 100, 10);

    // Titre
    setColor(9);
    gotoxy(20, 13);
    printf("╠═════════════════════════════════════════ CONNEXION ═══════════════════════════════════════════════╣");

    // Saisie du nom d'utilisateur
    drawInputField("username", 16, user.username, 0);

    // Saisie du mot de passe
    drawInputField("password", 20, user.password, 1);

    User tempUser;
    int loginSuccess = 0;
    while (fread(&tempUser, sizeof(User), 1, file)) {
        if (strcmp(tempUser.username, user.username) == 0 && strcmp(tempUser.password, user.password) == 0) {
            strcpy(username, tempUser.username);
            setColor(10);
            gotoxy(45, 24);
            printf("Connexion réussie ! Bienvenue, %s %s", tempUser.nom, tempUser.prenom);
            loginSuccess = 1;
            break;
        }
    }

    if (!loginSuccess) {
        setColor(12);
        gotoxy(45, 24);
        printf("Nom d'utilisateur ou mot de passe incorrect");
        Sleep(2000);
    } else {
        Sleep(2000);
    }

    fclose(file);
    system("cls");
    return loginSuccess ? username : NULL;
}





// Fonction pour afficher un billet de manière formatée












void saisirNombrePlaces(int *nb_places, int max_places) {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);

    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═══════════════════════════════════ SÉLECTION DES PLACES ══════════════════════════════════╣");

    // Affichage du cadre d'information
    setColor(11);
    gotoxy(CENTER_X - 25, 14);
    printf("┌─────────────────────────────────────────────┐");
    gotoxy(CENTER_X - 25, 15);
    printf("│            INFORMATIONS PLACES              │");
    gotoxy(CENTER_X - 25, 16);
    printf("├─────────────────────────────────────────────┤");
    gotoxy(CENTER_X - 25, 17);
    printf("│ Places disponibles: %-24d │", max_places);
    gotoxy(CENTER_X - 25, 18);
    printf("│ Minimum: 1                                  │");
    gotoxy(CENTER_X - 25, 19);
    printf("│ Maximum: %-35d │", max_places);
    gotoxy(CENTER_X - 25, 20);
    printf("└─────────────────────────────────────────────┘");

    char places_str[5] = {0};
    int places_valides = 0;

    while (!places_valides) {
        // Effacer les messages précédents
        for(int i = 22; i <= 25; i++) {
            gotoxy(CENTER_X - 25, i);
            printf("                                                  ");
        }

        setColor(15);
        gotoxy(CENTER_X - 20, 22);
        printf("Entrez le nombre de places souhaité: ");
        scanf("%s", places_str);
        *nb_places = atoi(places_str);

        if (*nb_places <= 0 || *nb_places > max_places) {
            setColor(12);
            gotoxy(CENTER_X - 25, 24);
            printf("✗ Nombre de places invalide! (1-%d places)", max_places);
            Sleep(1500);
            continue;
        }

        // Animation de validation
        drawWaitingAnimation(CENTER_X - 12, 24, "Validation");
        
        setColor(10);
        gotoxy(CENTER_X - 20, 24);
        printf("✓ Nombre de places validé: %d", *nb_places);
        places_valides = 1;
        Sleep(1500);
    }
}





void afficherBillet(const VoyageInterne *voyage, const Reservation *reservation) {
    int page_courante = 1;
    const int TOTAL_PAGES = 2;
    char touche;

    do {
        // Configuration initiale
        system("cls");
        drawFrame(20, 1, 100, 30);
        drawLogo();
        drawSeparator(20, 100, 10);
        
        setColor(9);
        gotoxy(20, 12);
        printf("╠═══════════════════════════════════════ BILLET ELECTRONIQUE ═══════════════════════════════════════╣");

        if (page_courante == 1) {
            // Page 1 : Informations de réservation
            setColor(11);
            gotoxy(30, 14);
            printf("╔═══════════════════ INFORMATIONS DE RESERVATION ═══════════════════╗");
            
            setColor(15);
            gotoxy(32, 16);
            printf("Numéro de réservation  : %s", reservation->voyage_id);
            gotoxy(32, 17);
            printf("Nom du passager        : %s", reservation->username);
            gotoxy(32, 18);
            printf("Nombre de places       : %d", reservation->nb_places_reservees);
            gotoxy(32, 19);
            printf("Montant payé           : %.2f MAD", reservation->montant_paye);
            
            setColor(11);
            gotoxy(30, 21);
            printf("╚════════════════════════════════════════════════════════════════╝");
        }
        else if (page_courante == 2) {
            // Page 2 : Détails du vol
            setColor(11);
            gotoxy(30, 14);
            printf("╔════════════════════════ DETAILS DU VOL ════════════════════════╗");
            
            setColor(15);
            gotoxy(32, 16);
            printf("Compagnie aérienne     : %s", voyage->compagnie);
            gotoxy(32, 17);
            printf("Aéroport de départ     : %s", voyage->aeroport_depart);
            gotoxy(32, 18);
            printf("Aéroport d'arrivée     : %s", voyage->aeroport_arrive);
            gotoxy(32, 19);
            printf("Date du départ         : %02d/%02d/%d", 
                   voyage->date_voyage.jour, voyage->date_voyage.mois, voyage->date_voyage.annee);
            gotoxy(32, 20);
            printf("Heure de départ        : %02d:%02d", 
                   voyage->heure_depart.heure, voyage->heure_depart.minute);
            gotoxy(32, 21);
            printf("Heure d'arrivée        : %02d:%02d", 
                   voyage->heure_arrivee.heure, voyage->heure_arrivee.minute);
            gotoxy(32, 22);
            printf("Durée du vol           : %02dh%02dm", 
                   voyage->duree_vol.heure, voyage->duree_vol.minute);
            
            setColor(11);
            gotoxy(30, 24);
            printf("╚════════════════════════════════════════════════════════════════╝");
        }

        // Barre de navigation
        setColor(13);
        gotoxy(20, 26);
        printf("╠═══════════════════════════════════════════ NAVIGATION ════════════════════════════════════════════╣");
        
        setColor(15);
        gotoxy(30, 27);
        printf("◄  Page précédente  ←  |  Page %d/%d  |  Page suivante  →  ►", page_courante, TOTAL_PAGES);
        gotoxy(40, 28);
        printf("Appuyez sur [Entrée] pour quitter");

        // Gestion de la navigation
        touche = _getch();
        if (touche == 75 && page_courante > 1) { // Flèche gauche
            page_courante--;
        }
        else if (touche == 77 && page_courante < TOTAL_PAGES) { // Flèche droite
            page_courante++;
        }
        else if (touche == 13) { // Entrée
            break;
        }
    } while (1);
}

void afficherBilletApresPaiement(const char *username) {
    // Configuration initiale
    system("cls");
    drawFrame(20, 1, 100, 30);
    drawLogo();
    drawSeparator(20, 100, 10);
    
    setColor(9);
    gotoxy(20, 12);
    printf("╠═══════════════════════════════════ BILLET DE RESERVATION ════════════════════════════════════╣");

    // Ouvrir le fichier des réservations
    FILE *fp_reservations = fopen("reservationsInterne.bin", "rb");
    if (fp_reservations == NULL) {
        setColor(12);
        gotoxy(35, 15);
        printf("Erreur lors de l'ouverture du fichier des réservations");
        gotoxy(35, 16);
        printf("Appuyez sur une touche pour continuer...");
        _getch();
        return;
    }

    // Ouvrir le fichier des voyages
    FILE *fp_voyages = fopen("voyageInterne.bin", "rb");
    if (fp_voyages == NULL) {
        setColor(12);
        gotoxy(35, 15);
        printf("Erreur lors de l'ouverture du fichier des voyages");
        gotoxy(35, 16);
        printf("Appuyez sur une touche pour continuer...");
        _getch();
        fclose(fp_reservations);
        return;
    }

    Reservation reservation;
    VoyageInterne voyage;
    Reservation derniereReservation;
    VoyageInterne dernierVoyage;
    int reservationTrouvee = 0;

    // Animation de recherche
    setColor(15);
    gotoxy(35, 15);
    printf("Recherche de votre réservation");
    for(int i = 0; i < 3; i++) {
        printf(".");
        Sleep(300);
    }

    // Se positionner à la fin du fichier pour lire la dernière réservation
    fseek(fp_reservations, -sizeof(Reservation), SEEK_END);
    
    if (fread(&reservation, sizeof(Reservation), 1, fp_reservations) == 1) {
        if (strcmp(reservation.username, username) == 0) {
            // Chercher le voyage correspondant
            rewind(fp_voyages);
            while (fread(&voyage, sizeof(VoyageInterne), 1, fp_voyages) == 1) {
                if (strcmp(reservation.voyage_id, voyage.id) == 0) {
                    derniereReservation = reservation;
                    dernierVoyage = voyage;
                    reservationTrouvee = 1;
                    break;
                }
            }
        }
    }

    system("cls");
    drawFrame(20, 1, 100, 30);
    drawLogo();
    drawSeparator(20, 100, 10);
    
    setColor(9);
    gotoxy(20, 12);
    printf("╠═══════════════════════════════════ BILLET DE RESERVATION ════════════════════════════════════╣");

    if (reservationTrouvee) {
        setColor(10); // Vert pour le succès
        gotoxy(35, 14);
        printf("Réservation trouvée pour : %s", username);
        gotoxy(35, 15);
        printf("ID du voyage : %s", derniereReservation.voyage_id);
        
        afficherBillet(&dernierVoyage, &derniereReservation);
    } else {
        setColor(12); // Rouge pour l'erreur
        gotoxy(35, 15);
        printf("Aucune réservation trouvée pour l'utilisateur %s", username);
        gotoxy(35, 17);
        system("cls");
        printf("Appuyez sur une touche pour continuer...");
        _getch();
    }

    fclose(fp_reservations);
    fclose(fp_voyages);
}

void saisirCriteresRecherche(char *depart, char *arrive, Date *date, int *nb_places) {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);

    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═══════════════════════════════════ CRITÈRES DE RECHERCHE ══════════════════════════════════╣");

    // Saisie des informations
    drawInputField("Aéroport de départ", 15, depart, 0);
    drawInputField("Aéroport d'arrivée", 18, arrive, 0);

    // Saisie de la date
    setColor(11);
    gotoxy(CENTER_X - 20, 21);
    printf("Date du voyage:");
    drawDateInputField("Date", 22, CENTER_X, &date->jour, &date->mois, &date->annee);

    // Utiliser la nouvelle fonction pour la saisie du nombre de places
    saisirNombrePlaces(nb_places, 50); // 50 est un exemple de maximum, ajustez selon vos besoins
}

int afficherVoyagesDisponiblesInternes(FILE *fichier, const char *depart, const char *arrive, Date date, int nb_places) {
    VoyageInterne voyage;
    int index = 1;
    int voyage_trouve = 0;
    int ligne_actuelle = 14; // Position de départ après le titre

    // Afficher l'en-tête stylisé
    setColor(11); // Cyan clair pour l'en-tête
    gotoxy(22, ligne_actuelle);
    printf("╔════════════════════════════════════════ VOLS DISPONIBLES ══════════════════════════════════════╗");
    ligne_actuelle += 2;

    // En-tête des colonnes
    setColor(13); // Blanc pour les détails
    gotoxy(25, ligne_actuelle);
    printf("Critères de recherche :");
    setColor(7);
    gotoxy(25, ligne_actuelle + 1);
    printf("De: %-15s  Vers: %-15s  Date: %02d/%02d/%04d  Places: %d", 
           depart, arrive, date.jour, date.mois, date.annee, nb_places);
    ligne_actuelle += 3;

    while (fread(&voyage, sizeof(VoyageInterne), 1, fichier)) {
        if (strcmp(voyage.aeroport_depart, depart) == 0 &&
            strcmp(voyage.aeroport_arrive, arrive) == 0 &&
            voyage.date_voyage.jour == date.jour &&
            voyage.date_voyage.mois == date.mois &&
            voyage.date_voyage.annee == date.annee &&
            voyage.nb_place >= nb_places) {

            voyage_trouve = 1;

            // Cadre pour chaque vol
            setColor(9); // Bleu pour le cadre
            gotoxy(25, ligne_actuelle);
            printf("┌─────────────────────────────────── Vol N°%d ────────────────────────────────────┐", index);
            
            // Informations du vol en deux colonnes
            setColor(index % 2 == 0 ? 15 : 7); // Alterner blanc et gris clair
            
            // Colonne gauche
            gotoxy(27, ligne_actuelle + 1);
            printf("ID Vol: %-10s              Compagnie: %-20s", voyage.id, voyage.compagnie);
            
            gotoxy(27, ligne_actuelle + 2);
            printf("Départ: %-10s              Arrivée: %-20s", voyage.aeroport_depart, voyage.aeroport_arrive);
            
            gotoxy(27, ligne_actuelle + 3);
            printf("Date: %02d/%02d/%04d               Prix: %d MAD", 
                   voyage.date_voyage.jour, voyage.date_voyage.mois, 
                   voyage.date_voyage.annee, voyage.prix);
            
            gotoxy(27, ligne_actuelle + 4);
            printf("Heure départ: %02d:%02d           Heure arrivée: %02d:%02d", 
                   voyage.heure_depart.heure, voyage.heure_depart.minute,
                   voyage.heure_arrivee.heure, voyage.heure_arrivee.minute);
            
            gotoxy(27, ligne_actuelle + 5);
            printf("Durée du vol: %02dh%02dm          Places disponibles: %d", 
                   voyage.duree_vol.heure, voyage.duree_vol.minute, voyage.nb_place);

            // Ligne de fermeture du cadre
            setColor(9);
            gotoxy(25, ligne_actuelle + 6);
            printf("└──────────────────────────────────────────────────────────────────────────────┘");

            ligne_actuelle += 8; // Espace entre les vols
            index++;

            // Vérifier si on atteint la limite du cadre
            if (ligne_actuelle > 25) {
                setColor(14); // Jaune pour le message
                gotoxy(25, 26);
                printf("↑↓ Utilisez les flèches pour voir plus de vols...");
                break;
            }
        }
    }

    if (!voyage_trouve) {
        setColor(12); // Rouge pour message d'erreur
        gotoxy(30, ligne_actuelle);
        printf("Aucun vol ne correspond à vos critères de recherche.");
    }

    setColor(15); // Retour à la couleur par défaut
    return voyage_trouve ? index - 1 : 0;
}
int demanderChoixVoyage(int index_max) {
    int choix_voyage;
    
    // Nettoyer l'écran et dessiner le cadre complet
    system("cls");
    drawFrame(20, 1, 100, 30);
    drawLogo();
    drawSeparator(20, 100, 10);

    // Titre
    setColor(9);
    gotoxy(20, 12);
    printf("╠══════════════════════════════════════ SÉLECTION DU VOL ═══════════════════════════════════════╣");

    // Afficher les options
    setColor(15); // Blanc pour le texte
    gotoxy(35, 15);
    printf("▶ Pour annuler la réservation, entrez [0]");
    
    gotoxy(35, 17);
    printf("▶ Pour sélectionner un vol, entrez un numéro entre [1-%d]", index_max);

    // Zone de saisie avec style
    setColor(11); // Cyan clair pour la saisie
    gotoxy(35, 19);
    printf("Votre choix ➤ ");
    
    scanf("%d", &choix_voyage);

    // Gestion des messages de retour
    if (choix_voyage < 0 || choix_voyage > index_max) {
        setColor(12); // Rouge pour l'erreur
        gotoxy(48, 19);
        printf("⚠ Choix invalide !");
        Sleep(1500);
        return -1;
    }

    // Message de confirmation
    if (choix_voyage > 0) {
        system("cls");
        setColor(10); // Vert pour la confirmation
        gotoxy(48, 19);
        printf("✓ Vol %d sélectionné !", choix_voyage);
    } else {
        setColor(14); // Jaune pour l'annulation
        gotoxy(48, 19);
        printf("○ Réservation annulée");
    }
    Sleep(1000);

    setColor(15); // Retour à la couleur par défaut
    return choix_voyage;
}

void enregistrerReservationInterne(const char *username, VoyageInterne voyage, int nb_places, float montant_paye) {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int CENTER_X = FRAME_LEFT + ((FRAME_RIGHT - FRAME_LEFT) / 2);

    FILE *reservation_file = Ouvrir_Fichier("reservationsInterne.bin", "ab");
    if (reservation_file == NULL) {
        setColor(12);
        gotoxy(CENTER_X - 25, 25);
        printf("✗ Erreur lors de l'enregistrement de la réservation!");
        Sleep(2000);
        return;
    }

    // Animation d'enregistrement
    drawWaitingAnimation(CENTER_X - 12, 25, "Enregistrement en cours");

    Reservation reservation;
    strcpy(reservation.username, username);
    strcpy(reservation.voyage_id, voyage.id);
    reservation.nb_places_reservees = nb_places;
    reservation.montant_paye = montant_paye;
    obtenir_date_du_jour(&reservation.date_reservation);

    fwrite(&reservation, sizeof(Reservation), 1, reservation_file);
    fclose(reservation_file);

    setColor(10);
    gotoxy(CENTER_X - 25, 27);
    printf("✓ Réservation enregistrée avec succès!");
    Sleep(2000);
}

int traiterPaiement(const char *username, float montant_total) {
    InformationsPaiement payement;

    // Configuration initiale
    system("cls");
    drawFrame(20, 1, 100, 30);
    drawLogo();
    drawSeparator(20, 100, 10);
    
    setColor(9);
    gotoxy(20, 12);
    printf("╠═════════════════════════════════════ PROCESSUS DE PAIEMENT ═══════════════════════════════════════╣");

    // Informations de base
    setColor(11);
    gotoxy(30, 14);
    printf("Client: %-20s", username);
    gotoxy(30, 15);
    printf("Montant total à payer: %.2f MAD", montant_total);

    setColor(15);
    gotoxy(30, 17);
    printf("╔══════════════════ INFORMATIONS DE PAIEMENT ══════════════════╗");

    // Numéro de carte
    gotoxy(31, 18);
    printf("Numéro de carte (16 chiffres) : ");
    gotoxy(65, 18);
    if (scanf("%16s", payement.numero_carte) != 1) {
        setColor(12);
        gotoxy(35, 24);
        printf("Erreur de lecture du numéro de carte.");
        return 0;
    }
    while (getchar() != '\n');

    // Date d'expiration
    gotoxy(32, 19);
    printf("Date d'expiration (MM AA)     : ");
    gotoxy(65, 19);
    if (scanf("%d %d", &payement.date_expiration.mois, &payement.date_expiration.annee) != 2) {
        setColor(12);
        gotoxy(35, 24);
        printf("Erreur de lecture de la date d'expiration.");
        return 0;
    }
    while (getchar() != '\n');

    // Titulaire
    gotoxy(32, 20);
    printf("Titulaire de la carte         : ");
    gotoxy(65, 20);
    if (fgets(payement.titulaire_carte, sizeof(payement.titulaire_carte), stdin) == NULL) {
        setColor(12);
        gotoxy(35, 24);
        printf("Erreur de lecture du nom du titulaire.");
        return 0;
    }
    payement.titulaire_carte[strcspn(payement.titulaire_carte, "\n")] = '\0';

    // CVV
    gotoxy(32, 21);
    printf("Code de sécurité (CVV)        : ");
    gotoxy(65, 21);
    if (scanf("%3s", payement.code_securite) != 1) {
        setColor(12);
        gotoxy(35, 24);
        printf("Erreur de lecture du CVV.");
        return 0;
    }
    while (getchar() != '\n');

    gotoxy(30, 22);
    printf("╚═════════════════════════════════════════════════════════════╝");

    // Validation
    if (strlen(payement.numero_carte) != 16) {
        setColor(12);
        gotoxy(35, 24);
        printf("Le numéro de carte doit contenir exactement 16 chiffres.");
        return 0;
    }

    if (strlen(payement.code_securite) != 3) {
        setColor(12);
        gotoxy(35, 24);
        printf("Le CVV doit contenir exactement 3 chiffres.");
        return 0;
    }

    if (payement.date_expiration.mois < 1 || payement.date_expiration.mois > 12) {
        setColor(12);
        gotoxy(35, 24);
        printf("Mois d'expiration invalide.");
        return 0;
    }

    // Message de succès
    setColor(10);
    gotoxy(35, 24);
    printf("Paiement de %.2f MAD effectué avec succès!", montant_total);
    gotoxy(35, 25);
    printf("Merci pour votre confiance!");
    Sleep(2000);

    return 1;
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
                afficherBilletApresPaiement(username);
                break;
            }
            index++;
        }
    }
 
}




//reserver voyage interne

void ReserverVoyageInterne(const char *username) {
    char depart[30], arrive[30];
    Date date;
    int nb_places;
    int index;
    int choix_voyage;

    // Configuration initiale
    system("cls");
    drawFrame(20, 1, 100, 30);
    drawLogo();
    drawSeparator(20, 100, 10);
    
    setColor(13);
    gotoxy(20, 12);
    printf("╠═══════════════════════════════════ RESERVATION VOYAGE INTERNE ════════════════════════════════════╣");

    FILE *fichier = fopen("voyageInterne.bin", "rb+");
    if (fichier == NULL) {
        setColor(12); // Rouge pour l'erreur
        gotoxy(35, 15);
        printf("Erreur lors de l'ouverture du fichier des voyages internes.");
        _getch();
        return;
    }

    // Saisie des critères de recherche
    setColor(15); // Blanc pour le texte normal
    gotoxy(30, 14);
    printf("Entrez l'aéroport de départ : ");
    gotoxy(61, 14);
    scanf("%s", depart);

    gotoxy(30, 15);
    printf("Entrez l'aéroport d'arrivée : ");
    gotoxy(61, 15);
    scanf("%s", arrive);

    gotoxy(30, 16);
    printf("Date de voyage (jour mois année) : ");
    gotoxy(64, 16);
    scanf("%d %d %d", &date.jour, &date.mois, &date.annee);

    gotoxy(30, 17);
    printf("Nombre de places à réserver : ");
    gotoxy(60, 17);
    scanf("%d", &nb_places);

    // Affichage des voyages disponibles
    system("cls");
    drawFrame(20, 1, 100, 30);
    drawLogo();
    drawSeparator(20, 100, 10);
    
    setColor(9);
    gotoxy(20, 12);
    printf("╠═════════════════════════════════════ VOYAGES DISPONIBLES ═════════════════════════════════════════╣");

    index = afficherVoyagesDisponiblesInternes(fichier, depart, arrive, date, nb_places);
    
    if (index == 0) {
        system("cls");
        setColor(12);
        gotoxy(35, 15);
        printf("Aucun voyage disponible ne correspond à vos critères.");
        gotoxy(35, 17);
        printf("Appuyez sur une touche pour revenir au menu...");
        _getch();
    } else {
        // Attendre que l'utilisateur voie les vols disponibles
        setColor(15);
        gotoxy(30, 30);
        printf("Appuyez sur une touche pour continuer la sélection...");
        _getch();
        
        // Demander le choix du vol
        choix_voyage = demanderChoixVoyage(index);
            
        if (choix_voyage == 0) {
            setColor(14);
            gotoxy(35, 27);
            printf("Réservation annulée.");
            Sleep(1500);
            fclose(fichier);
            return;
        } else if (choix_voyage != -1) {
            // Confirmation et enregistrement de la réservation
            system("cls");
            drawFrame(20, 1, 100, 30);
            drawLogo();
            drawSeparator(20, 100, 10);
            
            setColor(9);
            gotoxy(20, 12);
            printf("╠═════════════════════════════════ CONFIRMATION DE RESERVATION ══════════════════════════════════╣");

            confirmerReservationInterne(fichier, username, choix_voyage, nb_places, depart, arrive, date);
            
            setColor(15);
            gotoxy(30, 27);
            system("cls");
            printf("Appuyez sur une touche pour revenir au menu...");
            _getch();
        }
    }

    fclose(fichier);
}





// Vyage Externes ;
void afficherBilletE(const VoyageExterne *voyage, const Reservation *reservation) {
    int page_courante = 1;
    const int TOTAL_PAGES = 2;
    char touche;

    do {
        // Configuration initiale
        system("cls");
        drawFrame(20, 1, 100, 30);
        drawLogo();
        drawSeparator(20, 100, 10);
        
        setColor(9);
        gotoxy(20, 12);
        printf("╠═══════════════════════════════ BILLET ELECTRONIQUE INTERNATIONAL ══════════════════════════════╣");

        if (page_courante == 1) {
            // Page 1 : Informations de réservation
            setColor(11);
            gotoxy(30, 14);
            printf("╔═══════════════════ INFORMATIONS DE RESERVATION ═══════════════════╗");
            
            setColor(15);
            gotoxy(32, 16);
            printf("Numéro de réservation  : %s", reservation->voyage_id);
            gotoxy(32, 17);
            printf("Nom du passager        : %s", reservation->username);
            gotoxy(32, 18);
            printf("Nombre de places       : %d", reservation->nb_places_reservees);
            gotoxy(32, 19);
            printf("Montant payé           : %.2f MAD", reservation->montant_paye);
            
            setColor(11);
            gotoxy(30, 21);
            printf("╚════════════════════════════════════════════════════════════════╝");
        }
        else if (page_courante == 2) {
            // Page 2 : Détails du vol international
            setColor(11);
            gotoxy(30, 14);
            printf("╔════════════════════ DETAILS DU VOL INTERNATIONAL ═══════════════╗");
            
            setColor(15);
            gotoxy(32, 16);
            printf("Compagnie aérienne     : %s", voyage->compagnie);
            gotoxy(32, 17);
            printf("Classe de voyage       : %s", voyage->classe);
            gotoxy(32, 18);
            printf("Pays de départ         : %s", voyage->pays_depart);
            gotoxy(32, 19);
            printf("Aéroport de départ     : %s", voyage->aeroport_depart);
            gotoxy(32, 20);
            printf("Pays d'arrivée         : %s", voyage->pays_arrivee);
            gotoxy(32, 21);
            printf("Aéroport d'arrivée     : %s", voyage->aeroport_arrive);
            gotoxy(32, 22);
            printf("Date de départ         : %02d/%02d/%d", 
                   voyage->date_voyage.jour, voyage->date_voyage.mois, voyage->date_voyage.annee);
            gotoxy(32, 23);
            printf("Heure de départ        : %02d:%02d", 
                   voyage->heure_depart.heure, voyage->heure_depart.minute);
            gotoxy(32, 24);
            printf("Heure d'arrivée        : %02d:%02d", 
                   voyage->heure_arrivee.heure, voyage->heure_arrivee.minute);
            gotoxy(32, 25);
            printf("Durée du vol           : %02dh%02dm", 
                   voyage->duree_vol.heure, voyage->duree_vol.minute);
            
            setColor(11);
            gotoxy(30, 26);
            printf("╚════════════════════════════════════════════════════════════════╝");
        }

        // Barre de navigation
        setColor(13);
        gotoxy(20, 27);
        printf("╠═══════════════════════════════════ NAVIGATION ═══════════════════════════════════╣");
        
        setColor(15);
        gotoxy(30, 28);
        printf("◄ Page précédente (←)  |  Page %d/%d  |  Page suivante (→) ►", page_courante, TOTAL_PAGES);
        gotoxy(30, 29);
        printf("Appuyez sur [Entrée] pour quitter");

        // Gestion de la navigation
        touche = _getch();
        if (touche == 75 && page_courante > 1) { // Flèche gauche
            page_courante--;
        }
        else if (touche == 77 && page_courante < TOTAL_PAGES) { // Flèche droite
            page_courante++;
        }
        else if (touche == 13) { // Entrée
            break;
        }
    } while (1);
}


void afficherBilletApresPaiementE(const char *username) {
    // Configuration initiale
    system("cls");
    drawFrame(20, 1, 100, 30);
    drawLogo();
    drawSeparator(20, 100, 10);
    
    setColor(9);
    gotoxy(20, 12);
    printf("╠═══════════════════════════════ BILLET DE RESERVATION INTERNATIONALE ══════════════════════════════╣");

    // Ouvrir le fichier des réservations
    FILE *fp_reservations = fopen("reservationsExterne.bin", "rb");
    if (fp_reservations == NULL) {
        setColor(12);
        gotoxy(35, 15);
        printf("Erreur lors de l'ouverture du fichier des réservations externes");
        gotoxy(35, 16);
        printf("Appuyez sur une touche pour continuer...");
        _getch();
        return;
    }

    // Ouvrir le fichier des voyages
    FILE *fp_voyages = fopen("voyageExterne.bin", "rb");
    if (fp_voyages == NULL) {
        setColor(12);
        gotoxy(35, 15);
        printf("Erreur lors de l'ouverture du fichier des voyages externes");
        gotoxy(35, 16);
        printf("Appuyez sur une touche pour continuer...");
        _getch();
        fclose(fp_reservations);
        return;
    }

    Reservation reservation;
    VoyageExterne voyage;
    Reservation derniereReservation;
    VoyageExterne dernierVoyage;
    int reservationTrouvee = 0;

    // Animation de recherche
    setColor(15);
    gotoxy(35, 15);
    printf("Recherche de votre réservation internationale");
    for(int i = 0; i < 3; i++) {
        printf(".");
        Sleep(300);
    }

    // Se positionner à la fin du fichier pour lire la dernière réservation
    fseek(fp_reservations, -sizeof(Reservation), SEEK_END);
    
    if (fread(&reservation, sizeof(Reservation), 1, fp_reservations) == 1) {
        if (strcmp(reservation.username, username) == 0) {
            // Chercher le voyage correspondant
            rewind(fp_voyages);
            while (fread(&voyage, sizeof(VoyageExterne), 1, fp_voyages) == 1) {
                if (strcmp(reservation.voyage_id, voyage.id) == 0) {
                    derniereReservation = reservation;
                    dernierVoyage = voyage;
                    reservationTrouvee = 1;
                    break;
                }
            }
        }
    }

    system("cls");
    drawFrame(20, 1, 100, 30);
    drawLogo();
    drawSeparator(20, 100, 10);
    
    setColor(9);
    gotoxy(20, 12);
    printf("╠═══════════════════════════════ BILLET DE RESERVATION INTERNATIONALE ══════════════════════════════╣");

    if (reservationTrouvee) {
        setColor(10); // Vert pour le succès
        gotoxy(35, 14);
        printf("Réservation internationale trouvée pour : %s", username);
        gotoxy(35, 15);
        printf("ID du voyage : %s", derniereReservation.voyage_id);
        
        afficherBilletE(&dernierVoyage, &derniereReservation);
    } else {
        setColor(12); // Rouge pour l'erreur
        gotoxy(35, 15);
        printf("Aucune réservation internationale trouvée pour l'utilisateur %s", username);
        gotoxy(35, 17);
        printf("Appuyez sur une touche pour continuer...");
        _getch();
    }

    fclose(fp_reservations);
    fclose(fp_voyages);
}
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
int afficherVoyagesDisponiblesExterne(FILE *fichier, const char *depart, const char *arrive, 
                                     const char *pays_depart, const char *pays_arrivee, 
                                     Date date, int nb_places) {
    VoyageExterne voyage;
    int index = 1;
    int voyage_trouve = 0;
    int ligne_actuelle = 14; // Position de départ après le titre

    // Afficher l'en-tête stylisé
    setColor(11);
    gotoxy(22, ligne_actuelle);
    printf("╔════════════════════════════════ VOLS INTERNATIONAUX DISPONIBLES ═══════════════════════════════╗");
    ligne_actuelle += 2;

    // En-tête des critères de recherche
    setColor(13);
    gotoxy(25, ligne_actuelle);
    printf("Critères de recherche :");
    setColor(7);
    gotoxy(25, ligne_actuelle + 1);
    printf("De: %s (%s)", depart, pays_depart);
    gotoxy(25, ligne_actuelle + 2);
    printf("Vers: %s (%s)", arrive, pays_arrivee);
    gotoxy(25, ligne_actuelle + 3);
    printf("Date: %02d/%02d/%04d  Places: %d", date.jour, date.mois, date.annee, nb_places);
    ligne_actuelle += 5;

    while (fread(&voyage, sizeof(VoyageExterne), 1, fichier)) {
        if (strcmp(voyage.aeroport_depart, depart) == 0 &&
            strcmp(voyage.aeroport_arrive, arrive) == 0 &&
            strcmp(voyage.pays_depart, pays_depart) == 0 &&
            strcmp(voyage.pays_arrivee, pays_arrivee) == 0 &&
            voyage.date_voyage.jour == date.jour &&
            voyage.date_voyage.mois == date.mois &&
            voyage.date_voyage.annee == date.annee &&
            voyage.nb_place >= nb_places) {

            voyage_trouve = 1;

            // Cadre pour chaque vol
            setColor(9);
            gotoxy(25, ligne_actuelle);
            printf("┌─────────────────────────────────── Vol N°%d ────────────────────────────────────┐", index);
            
            // Informations du vol avec alternance de couleurs
            setColor(index % 2 == 0 ? 15 : 7);
            
            // Première ligne
            gotoxy(27, ligne_actuelle + 1);
            printf("ID Vol: %-10s              Compagnie: %-20s", voyage.id, voyage.compagnie);
            
            // Deuxième ligne
            gotoxy(27, ligne_actuelle + 2);
            printf("Départ: %-10s (%s)        Arrivée: %-10s (%s)", 
                   voyage.aeroport_depart, voyage.pays_depart,
                   voyage.aeroport_arrive, voyage.pays_arrivee);
            
            // Troisième ligne
            gotoxy(27, ligne_actuelle + 3);
            printf("Date: %02d/%02d/%04d               Classe: %s", 
                   voyage.date_voyage.jour, voyage.date_voyage.mois, 
                   voyage.date_voyage.annee, voyage.classe);
            
            // Quatrième ligne
            gotoxy(27, ligne_actuelle + 4);
            printf("Heure départ: %02d:%02d           Heure arrivée: %02d:%02d", 
                   voyage.heure_depart.heure, voyage.heure_depart.minute,
                   voyage.heure_arrivee.heure, voyage.heure_arrivee.minute);
            
            // Cinquième ligne
            gotoxy(27, ligne_actuelle + 5);
            printf("Durée: %02dh%02dm                 Prix: %d MAD", 
                   voyage.duree_vol.heure, voyage.duree_vol.minute, voyage.prix);

            // Sixième ligne
            gotoxy(27, ligne_actuelle + 6);
            printf("Places disponibles: %d", voyage.nb_place);

            // Ligne de fermeture du cadre
            setColor(9);
            gotoxy(25, ligne_actuelle + 7);
            printf("└──────────────────────────────────────────────────────────────────────────────┘");

            ligne_actuelle += 9; // Espace entre les vols
            index++;

            // Vérifier si on atteint la limite du cadre
            if (ligne_actuelle > 25) {
                setColor(14);
                gotoxy(25, 29);
                printf("↑↓ Utilisez les flèches pour voir plus de vols...");
                break;
            }
        }
    }

    if (!voyage_trouve) {
        setColor(12);
        gotoxy(30, ligne_actuelle);
        printf("Aucun vol international ne correspond à vos critères de recherche.");
    }

    setColor(15); // Retour à la couleur par défaut
    return voyage_trouve ? index - 1 : 0;
}


int traiterPaiementE(const char *username, float montant_total) {
    InformationsPaiement payement;
    
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2) + 8; // Ajouté +3 pour un meilleur centrage
    const int FORM_LEFT = CENTER_X - 15;

    // Affichage de l'écran de paiement
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 30);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);
    
    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═════════════════════════════════════ PROCESSUS DE PAIEMENT ═══════════════════════════════════════╣");

    // Informations de paiement
    setColor(15);
    char info[100];
    sprintf(info, "Utilisateur : %s", username);
    gotoxy(CENTER_X - (strlen(info) / 2), 14);
    printf("%s", info);

    setColor(14); // Jaune pour le montant
    sprintf(info, "Montant total à payer : %.2f MAD", montant_total);
    gotoxy(CENTER_X - (strlen(info) / 2), 15);
    printf("%s", info);

    // Formulaire de paiement
    setColor(15);
    gotoxy(FORM_LEFT, 17);
    printf("Numéro de carte (16 chiffres) : ");
    scanf("%s", payement.numero_carte);
    
    gotoxy(FORM_LEFT, 18);
    printf("Date d'expiration (MM AA)     : ");
    scanf("%d %d", &payement.date_expiration.mois, &payement.date_expiration.annee);
    
    // Vider le buffer d'entrée
    while (getchar() != '\n');
    
    gotoxy(FORM_LEFT, 19);
    printf("Titulaire de la carte         : ");
    fgets(payement.titulaire_carte, sizeof(payement.titulaire_carte), stdin);
    payement.titulaire_carte[strcspn(payement.titulaire_carte, "\n")] = '\0';
    
    gotoxy(FORM_LEFT, 20);
    printf("Code de sécurité (CVV)        : ");
    scanf("%s", payement.code_securite);

    // Validation des informations
    if (strlen(payement.numero_carte) != 16 || strlen(payement.code_securite) != 3) {
        setColor(12); // Rouge pour l'erreur
        const char* error_msg = "⚠ Erreur : Les informations de paiement sont invalides.";
        gotoxy(CENTER_X - (strlen(error_msg) / 2), 22);
        printf("%s", error_msg);
        Sleep(2000);
        return 0;
    }

    // Animation de traitement
    setColor(13);
    const char* processing_msg = "Traitement du paiement en cours...";
    gotoxy(CENTER_X - (strlen(processing_msg) / 2), 22);
    printf("%s", processing_msg);
    Sleep(1500);

    // Message de succès
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 30);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);
    
    setColor(13); // Changé en mauve (13)
    gotoxy(FRAME_LEFT, 12);
    printf("╠═══════════════════════════════════════ PAIEMENT RÉUSSI ═══════════════════════════════════════════╣");

    setColor(15);
    sprintf(info, "Paiement de %.2f MAD effectué avec succès!", montant_total);
    gotoxy(CENTER_X - (strlen(info) / 2), 15);
    printf("%s", info);

    sprintf(info, "Carte utilisée : **** **** **** %s", &payement.numero_carte[12]);
    gotoxy(CENTER_X - (strlen(info) / 2), 16);
    printf("%s", info);

    setColor(13);
    const char* thanks_msg = "Merci pour votre confiance!";
    gotoxy(CENTER_X - (strlen(thanks_msg) / 2), 18);
    printf("%s", thanks_msg);

    const char* continue_msg = "Appuyez sur une touche pour voir votre billet...";
    gotoxy(CENTER_X - (strlen(continue_msg) / 2), 20);
    printf("%s", continue_msg);
    _getch();

   // Afficher le billet
    afficherBilletApresPaiementE(username);
    return 1;
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
    reservation.montant_paye = montant_paye;

    // Simule la date actuelle
    obtenir_date_du_jour(&reservation.date_reservation);

    fwrite(&reservation, sizeof(Reservation), 1, reservation_file);
    fclose(reservation_file);

    printf("Les informations de réservation et le montant payé ont été enregistrés pour le voyage international.\n");
}


// confirmation de reservation

void confirmerReservationExterne(FILE *fichier, const char *username, int choix_voyage, int nb_places,
                               const char *depart, const char *arrive, const char *pays_depart, 
                               const char *pays_arrivee, Date date) {
    VoyageExterne voyage;
    Reservation nouvelle_reservation;
    int position = (choix_voyage - 1) * sizeof(VoyageExterne);
    
    // Positionner le curseur au bon voyage
    fseek(fichier, position, SEEK_SET);
    fread(&voyage, sizeof(VoyageExterne), 1, fichier);

    // Calculer le montant total
    float montant_total = voyage.prix * nb_places;

    // Traiter le paiement
    if (traiterPaiement(username, montant_total)) {
        // Préparer la nouvelle réservation
        strcpy(nouvelle_reservation.voyage_id, voyage.id);
        strcpy(nouvelle_reservation.username, username);
        nouvelle_reservation.nb_places_reservees = nb_places;
        nouvelle_reservation.montant_paye = montant_total;  // Stocker le montant calculé

        // Mettre à jour le nombre de places disponibles
        voyage.nb_place -= nb_places;
        fseek(fichier, position, SEEK_SET);
        fwrite(&voyage, sizeof(VoyageExterne), 1, fichier);

        // Enregistrer la réservation
        FILE *fichier_reservations = fopen("reservationsExterne.bin", "ab");
        if (fichier_reservations != NULL) {
            fwrite(&nouvelle_reservation, sizeof(Reservation), 1, fichier_reservations);
            fclose(fichier_reservations);

            // Afficher le billet
            afficherBilletApresPaiementE(username);
        } else {
            setColor(12);
            gotoxy(35, 20);
            printf("Erreur lors de l'enregistrement de la réservation.");
            Sleep(2000);
        }
    }
}




void ReserverVoyageExterne(const char *username) {
    char depart[30], arrive[30], pays_depart[30], pays_arrivee[30];
    Date date;
    int nb_places;
    int index;
    int choix_voyage;

    // Configuration initiale
    system("cls");
    drawFrame(20, 1, 100, 30);
    drawLogo();
    drawSeparator(20, 100, 10);
    
    setColor(13);
    gotoxy(20, 12);
    printf("╠═══════════════════════════════════ RESERVATION VOYAGE EXTERNE ════════════════════════════════════╣");

    FILE *fichier = fopen("voyageExterne.bin", "rb+");
    if (fichier == NULL) {
        setColor(12); // Rouge pour l'erreur
        gotoxy(35, 15);
        printf("Erreur lors de l'ouverture du fichier des voyages externes.");

        _getch();
        return;
    }

    // Saisie des critères de recherche
    setColor(15); // Blanc pour le texte normal
    gotoxy(30, 14);
    printf("Entrez l'aéroport de départ : ");
    gotoxy(61, 14);
    scanf("%s", depart);

    gotoxy(30, 15);
    printf("Entrez le pays de départ    : ");
    gotoxy(61, 15);
    scanf("%s", pays_depart);

    gotoxy(30, 16);
    printf("Entrez l'aéroport d'arrivée : ");
    gotoxy(61, 16);
    scanf("%s", arrive);

    gotoxy(30, 17);
    printf("Entrez le pays d'arrivée    : ");
    gotoxy(61, 17);
    scanf("%s", pays_arrivee);

    gotoxy(30, 18);
    printf("Date de voyage (jour mois année) : ");
    gotoxy(64, 18);
    scanf("%d %d %d", &date.jour, &date.mois, &date.annee);

    gotoxy(30, 19);
    printf("Nombre de places à réserver : ");
    gotoxy(60, 19);
    scanf("%d", &nb_places);

    // Affichage des voyages disponibles
    system("cls");
    drawFrame(20, 1, 100, 30);
    drawLogo();
    drawSeparator(20, 100, 10);
    
    setColor(9);
    gotoxy(20, 12);
    printf("╠═════════════════════════════════════ VOYAGES DISPONIBLES ═════════════════════════════════════════╣");

    index = afficherVoyagesDisponiblesExterne(fichier, depart, arrive, pays_depart, pays_arrivee, date, nb_places);
    
    if (index == 0) {
        setColor(12);
        gotoxy(35, 15);
        printf("Aucun voyage disponible ne correspond à vos critères.");
        gotoxy(35, 17);
        printf("Appuyez sur une touche pour revenir au menu...");
        _getch();
    } else {
        // Attendre que l'utilisateur voie les vols disponibles
        setColor(15);
        gotoxy(30, 30);
        printf("Appuyez sur une touche pour continuer la sélection...");
        _getch();

        // Demander le choix du vol
        choix_voyage = demanderChoixVoyage(index);
            
        if (choix_voyage == 0) {
            setColor(14);
            gotoxy(35, 27);
            printf("Réservation annulée.");
            Sleep(1500);
            fclose(fichier);
            return;
        } else if (choix_voyage != -1) {
            // Confirmation et enregistrement de la réservation
            system("cls");
            drawFrame(20, 1, 100, 30);
            drawLogo();
            drawSeparator(20, 100, 10);
            
            setColor(9);
            gotoxy(20, 12);
            printf("╠═════════════════════════════════ CONFIRMATION DE RESERVATION ══════════════════════════════════╣");

            confirmerReservationExterne(fichier, username, choix_voyage, nb_places, depart, arrive, pays_depart, pays_arrivee, date);
            
            setColor(15);

            _getch();
        }
    }

    fclose(fichier);
}
// reservation de voyage;

// reservation de voyage;

void ReserverVoyage(char* username) {
    system("cls");
    int currentChoice = 0;
    const int NUM_OPTIONS = 3;
    char* options[] = {
        "Voyage interne",
        "Voyage externe",
        "Retour"
    };

    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 15;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 30);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);
        
        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠══════════════════════════════════════ RÉSERVATION DE VOYAGE ══════════════════════════════════════╣");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 5;

            if(i == currentChoice) {
                setColor(13); // Mauve pour la sélection
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→ %s", options[i]);
            } else {
                setColor(15); // Blanc pour les autres options
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation (centrées)
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 5, 26);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");
            
            switch(currentChoice) {
                case 0: {
                    // Réservation voyage interne
                    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 30);
                    drawLogo();
                    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);
                    
                    setColor(9);
                    gotoxy(FRAME_LEFT, 12);
                    printf("╠═════════════════════════════════ VOYAGE INTERNE ══════════════════════════════════╣");
                    
                    ReserverVoyageInterne(username);
                    
                    // Attendre que l'utilisateur soit prêt à revenir au menu
                    setColor(15);
                    printf("\n\nAppuyez sur une touche pour revenir au menu principal...");
                    _getch();
                    while(kbhit()) _getch(); // Vider le buffer clavier
                    break;
                }
                case 1: {
                    // Réservation voyage externe
                    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 30);
                    drawLogo();
                    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);
                    
                    setColor(9);
                    gotoxy(FRAME_LEFT, 12);
                    printf("╠═════════════════════════════════ VOYAGE EXTERNE ═════════════════════════════════════╣");
                    
                    ReserverVoyageExterne(username);
                    
                    // Attendre que l'utilisateur soit prêt à revenir au menu
                    setColor(15);
                    printf("\n\nAppuyez sur une touche pour revenir au menu principal...");
                    _getch();
                    while(kbhit()) _getch(); // Vider le buffer clavier
                    break;
                }
                case 2: {
                    return; // Retour au menu précédent
                }
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            system("cls");
            return;
        }
        system("cls");
    }
}

void afficherReservationsInternes(const char* username) {
    system("cls");
    int page_actuelle = 1;
    int reservations_par_page = 2;
    int total_reservations = 0;
    int total_pages;
    char key;
    
    // Compter d'abord le nombre total de réservations
    FILE *count_file = fopen("reservationsInterne.bin", "rb");
    if (count_file != NULL) {
        Reservation temp;
        while (fread(&temp, sizeof(Reservation), 1, count_file) == 1) {
            if (strcmp(temp.username, username) == 0) {
                total_reservations++;
            }
        }
        fclose(count_file);
    }
    
    total_pages = (total_reservations + reservations_par_page - 1) / reservations_par_page;
    if (total_pages == 0) total_pages = 1;

    do {
        system("cls");
        // Dessiner le cadre principal et les éléments de base
        drawFrame(20, 1, 100, 30);
        drawLogo();
        drawSeparator(20, 100, 10);
        
        // Titre du menu
        setColor(13);
        gotoxy(20, 12);
        printf("╠═══════════════════════════════════ VOS RESERVATIONS INTERNES ═════════════════════════════════════╣");
        
        FILE *reservation_file = fopen("reservationsInterne.bin", "rb");
        if (reservation_file == NULL) {
            setColor(12);
            gotoxy(40, 15);
            printf("Erreur lors de l'ouverture du fichier des réservations.");
            gotoxy(40, 17);
            printf("Appuyez sur une touche pour continuer...");
            _getch();
            return;
        }

        Reservation reservation;
        int y_pos = 14;
        int reservations_trouvees = 0;
        int reservations_affichees = 0;
        int skip_count = (page_actuelle - 1) * reservations_par_page;

        while (fread(&reservation, sizeof(Reservation), 1, reservation_file) == 1) {
            if (strcmp(reservation.username, username) == 0) {
                if (skip_count > 0) {
                    skip_count--;
                    continue;
                }
                
                if (reservations_affichees >= reservations_par_page) {
                    break;
                }

                reservations_trouvees = 1;
                
                // Cadre pour chaque réservation
                setColor(9);
                gotoxy(30, y_pos);
                printf("╔════════════════════════════════════════════════════╗");
                gotoxy(30, y_pos + 1);
                printf("║");
                gotoxy(83, y_pos + 1);
                printf("║");
                gotoxy(30, y_pos + 2);
                printf("║");
                gotoxy(83, y_pos + 2);
                printf("║");
                gotoxy(30, y_pos + 3);
                printf("║");
                gotoxy(83, y_pos + 3);
                printf("║");
                gotoxy(30, y_pos + 4);
                printf("║");
                gotoxy(83, y_pos + 4);
                printf("║");
                gotoxy(30, y_pos + 5);
                printf("╚════════════════════════════════════════════════════╝");

                // Affichage des informations
                setColor(15);
                gotoxy(32, y_pos + 1);
                printf("ID du voyage : %s", reservation.voyage_id);
                gotoxy(32, y_pos + 2);
                printf("Nombre de places : %d", reservation.nb_places_reservees);
                gotoxy(32, y_pos + 3);
                printf("Montant payé : %.2f DH", reservation.montant_paye);
                gotoxy(32, y_pos + 4);
                printf("Date : %02d/%02d/%04d", 
                       reservation.date_reservation.jour,
                       reservation.date_reservation.mois,
                       reservation.date_reservation.annee);

                y_pos += 7;
                reservations_affichees++;
            }
        }

        fclose(reservation_file);

        if (!reservations_trouvees) {
            setColor(14);
            gotoxy(40, 15);
            printf("Vous n'avez aucune réservation interne.");
        }

        // Barre de navigation modifiée
        setColor(13);
        gotoxy(20, 28);
        printf("╠═══════════════════════════════════════════════════════════════════════════════════════════════════╣");
        gotoxy(25, 29);
        printf(" Page %d/%d   ← Précédent | Suivant → | (ESC) Retour au menu précédent", page_actuelle, total_pages);

        // Attendre l'entrée utilisateur
        key = _getch();
        if (key == 75 && page_actuelle > 1) { // Flèche gauche
            page_actuelle--;
        }
        else if (key == 77 && page_actuelle < total_pages) { // Flèche droite
            page_actuelle++;
        }
        else if (key == 27) { // Échap (ASCII 27)
            break;
        }

    } while (1);
}




void afficherReservationsExternes(const char* username) {
    system("cls");
    int page_actuelle = 1;
    int reservations_par_page = 2;
    int total_reservations = 0;
    int total_pages;
    char key;
    
    // Compter le nombre total de réservations
    FILE *count_file = fopen("reservationsExterne.bin", "rb");
    if (count_file != NULL) {
        Reservation temp;
        while (fread(&temp, sizeof(Reservation), 1, count_file) == 1) {
            if (strcmp(temp.username, username) == 0) {
                total_reservations++;
            }
        }
        fclose(count_file);
    }
    
    total_pages = (total_reservations + reservations_par_page - 1) / reservations_par_page;
    if (total_pages == 0) total_pages = 1;

    do {
        system("cls");
        drawFrame(20, 1, 100, 30);
        drawLogo();
        drawSeparator(20, 100, 10);
        
        // Titre du menu
        setColor(13);
        gotoxy(20, 12);
        printf("╠═══════════════════════════════════ VOS RESERVATIONS EXTERNES ═════════════════════════════════════╣");
        
        FILE *reservation_file = fopen("reservationsExterne.bin", "rb");
        if (reservation_file == NULL) {
            setColor(12);
            gotoxy(40, 15);
            printf("Erreur lors de l'ouverture du fichier des réservations.");
            gotoxy(40, 17);
            printf("Appuyez sur une touche pour continuer...");
            _getch();
            return;
        }

        Reservation reservation;
        int y_pos = 15;
        int reservations_trouvees = 0;
        int reservations_affichees = 0;
        int skip_count = (page_actuelle - 1) * reservations_par_page;

        while (fread(&reservation, sizeof(Reservation), 1, reservation_file) == 1) {
            if (strcmp(reservation.username, username) == 0) {
                if (skip_count > 0) {
                    skip_count--;
                    continue;
                }
                
                if (reservations_affichees >= reservations_par_page) {
                    break;
                }

                reservations_trouvees = 1;
                
                // Cadre pour chaque réservation
                setColor(9);
                gotoxy(30, y_pos);
                printf("╔════════════════════════════════════════════════════╗");
                gotoxy(30, y_pos + 1);
                printf("║");
                gotoxy(83, y_pos + 1);
                printf("║");
                gotoxy(30, y_pos + 2);
                printf("║");
                gotoxy(83, y_pos + 2);
                printf("║");
                gotoxy(30, y_pos + 3);
                printf("║");
                gotoxy(83, y_pos + 3);
                printf("║");
                gotoxy(30, y_pos + 4);
                printf("║");
                gotoxy(83, y_pos + 4);
                printf("║");
                gotoxy(30, y_pos + 5);
                printf("╚════════════════════════════════════════════════════╝");

                // Affichage des informations
                setColor(15);
                gotoxy(32, y_pos + 1);
                printf("ID du voyage : %s", reservation.voyage_id);
                gotoxy(32, y_pos + 2);
                printf("Nombre de places : %d", reservation.nb_places_reservees);
                gotoxy(32, y_pos + 3);
                printf("Montant payé : %.2f DH", reservation.montant_paye);
                gotoxy(32, y_pos + 4);
                printf("Date : %02d/%02d/%04d", 
                       reservation.date_reservation.jour,
                       reservation.date_reservation.mois,
                       reservation.date_reservation.annee);

                y_pos += 7;
                reservations_affichees++;
            }
        }

        fclose(reservation_file);

        if (!reservations_trouvees) {
            setColor(14);
            gotoxy(40, 15);
            printf("Vous n'avez aucune réservation externe.");
        }

        // Barre de navigation modifiée
        setColor(13);
        gotoxy(20, 28);
        printf("╠═══════════════════════════════════════════════════════════════════════════════════════════════════╣");
        gotoxy(25, 29);
        printf("  Page %d/%d   ← Précédent   | Suivant →  | (ESC) Retour au menu précédent ", page_actuelle, total_pages);

        // Attendre l'entrée utilisateur
        key = _getch();
        if (key == 75 && page_actuelle > 1) { // Flèche gauche
            page_actuelle--;
        }
        else if (key == 77 && page_actuelle < total_pages) { // Flèche droite
            page_actuelle++;
        }
        else if (key == 27) { // Échap (ASCII 27)
            break;
        }

    } while (1);
}





void afficherLignes() {
    system("cls");
    int choix;
    int currentChoice = 0;
    const int NUM_OPTIONS = 3;
    char* options[] = {
        "Lignes internes",
        "Lignes externes",
        "Retour"
    };

    do {
        // Affichage initial
        drawFrame(20, 1, 100, 30);
        drawLogo();
        drawSeparator(20, 100, 10);
        
        setColor(9); // Bleu pour le titre principal
        gotoxy(20, 12);
        printf("╠══════════════════════════════════════ AFFICHAGE DES LIGNES ═════════════════════════════════════╣");

        // Afficher les options
        for(int i = 0; i < NUM_OPTIONS; i++) {
            if(i == currentChoice) {
                setColor(13); // Mauve pour la sélection
                gotoxy(60, 15 + i * 2);
                printf("→ %s", options[i]);
            } else {
                setColor(15); // Blanc pour les autres options
                gotoxy(60, 15 + i * 2);
                printf("  %s", options[i]);
            }
        }

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");
            
            switch(currentChoice) {
                case 0: { // Lignes internes
                    FILE *file = fopen(FILEvoyageInterne, "rb");
                    if (!file) {
                        drawFrame(20, 1, 100, 30);
                        drawLogo();
                        drawSeparator(20, 100, 10);
                        
                        setColor(12); // Rouge pour l'erreur
                        gotoxy(40, 15);
                        printf("⚠ Erreur : Impossible d'ouvrir le fichier des lignes internes.");
                        Sleep(2000);
                    } else {
                        VoyageInterne voyage;
                        int found = 0;
                        int pageCourante = 0;
                        const int LIGNES_PAR_PAGE = 8;
                        const int DEBUT_LISTE = 16;
                        const int FIN_LISTE = 24;

                        do {
                            system("cls");
                            drawFrame(20, 1, 100, 30);
                            drawLogo();
                            drawSeparator(20, 100, 10);
                            
                            setColor(9);
                            gotoxy(20, 12);
                            printf("╠═════════════════════════════════ LIGNES INTERNES DISPONIBLES ═════════════════════════════════╣");
                            
                            setColor(13);
                            gotoxy(25, 14);
                            printf("%-10s %-15s %-15s %-10s %-12s", "ID", "Départ", "Arrivée", "Date", "Prix");
                            
                            setColor(15);
                            int ligne = DEBUT_LISTE;
                            fseek(file, pageCourante * LIGNES_PAR_PAGE * sizeof(VoyageInterne), SEEK_SET);
                            int compteur = 0;

                            while (fread(&voyage, sizeof(VoyageInterne), 1, file) == 1 && compteur < LIGNES_PAR_PAGE) {
                                found = 1;
                                gotoxy(25, ligne);
                                printf("%-10s %-15s %-15s %02d/%02d/%04d %-8d MAD",
                                    voyage.id, voyage.aeroport_depart, voyage.aeroport_arrive,
                                    voyage.date_voyage.jour, voyage.date_voyage.mois, 
                                    voyage.date_voyage.annee, voyage.prix);
                                ligne++;
                                compteur++;
                            }

                            if (found) {
                                setColor(13);
                                gotoxy(30, 26);
                                printf("← Utilisez les flèches pour naviguer entre les pages →");
                                gotoxy(45, 27);
                                printf("ÉCHAP: Retour");

                                char key = _getch();
                                if (key == -32 || key == 0) {
                                    key = _getch();
                                    if (key == 75) { // Gauche
                                        if (pageCourante > 0) pageCourante--;
                                    }
                                    else if (key == 77) { // Droite
                                        VoyageInterne nextVoyage;
                                        long currentPos = ftell(file);
                                        if (fread(&nextVoyage, sizeof(VoyageInterne), 1, file) == 1) {
                                            pageCourante++;
                                        }
                                        fseek(file, currentPos, SEEK_SET);
                                    }
                                }
                                else if (key == 27) { // Échap
                                    break;
                                }
                            } else {
                                setColor(12);
                                gotoxy(40, 16);
                                printf("⚠ Aucun vol interne disponible pour le moment.");
                                Sleep(2000);
                                break;
                            }
                        } while (1);
                        fclose(file);
                    }
                    break;
                }
                
                case 1: { // Lignes externes
                    FILE *file = fopen(FILEvoyageExterne, "rb");
                    if (!file) {
                        drawFrame(20, 1, 100, 30);
                        drawLogo();
                        drawSeparator(20, 100, 10);
                        
                        setColor(12);
                        gotoxy(40, 15);
                        printf("⚠ Erreur : Impossible d'ouvrir le fichier des lignes externes.");
                        Sleep(2000);
                    } else {
                        VoyageExterne voyage;
                        int found = 0;
                        int pageCourante = 0;
                        const int LIGNES_PAR_PAGE = 8;
                        const int DEBUT_LISTE = 16;
                        const int FIN_LISTE = 24;

                        do {
                            system("cls");
                            drawFrame(20, 1, 100, 30);
                            drawLogo();
                            drawSeparator(20, 100, 10);
                            
                            setColor(9);
                            gotoxy(20, 12);
                            printf("╠════════════════════════════════ VOLS INTERNATIONAUX DISPONIBLES ═════════════════════════════╣");
                            
                            setColor(13);
                            gotoxy(22, 14);
                            printf("%-10s %-12s %-12s %-15s %-15s %-10s %-10s %-8s",
                                "ID", "Départ", "Arrivée", "Aéro. Dép.", "Aéro. Arr.",
                                "Date", "Prix", "Classe");
                            
                            setColor(15);
                            int ligne = DEBUT_LISTE;
                            fseek(file, pageCourante * LIGNES_PAR_PAGE * sizeof(VoyageExterne), SEEK_SET);
                            int compteur = 0;

                            while (fread(&voyage, sizeof(VoyageExterne), 1, file) == 1 && compteur < LIGNES_PAR_PAGE) {
                                found = 1;
                                gotoxy(22, ligne);
                                printf("%-10s %-12s %-12s %-15s %-15s %02d/%02d/%04d %-8d %-8s",
                                    voyage.id, voyage.pays_depart, voyage.pays_arrivee,
                                    voyage.aeroport_depart, voyage.aeroport_arrive,
                                    voyage.date_voyage.jour, voyage.date_voyage.mois,
                                    voyage.date_voyage.annee, voyage.prix, voyage.classe);
                                ligne++;
                                compteur++;
                            }

                            if (found) {
                                setColor(13);
                                gotoxy(30, 26);
                                printf("← Utilisez les flèches pour naviguer entre les pages →");
                                gotoxy(45, 27);
                                printf("ÉCHAP: Retour");

                                char key = _getch();
                                if (key == -32 || key == 0) {
                                    key = _getch();
                                    if (key == 75) { // Gauche
                                        if (pageCourante > 0) pageCourante--;
                                    }
                                    else if (key == 77) { // Droite
                                        VoyageExterne nextVoyage;
                                        long currentPos = ftell(file);
                                        if (fread(&nextVoyage, sizeof(VoyageExterne), 1, file) == 1) {
                                            pageCourante++;
                                        }
                                        fseek(file, currentPos, SEEK_SET);
                                    }
                                }
                                else if (key == 27) { // Échap
                                    break;
                                }
                            } else {
                                setColor(12);
                                gotoxy(40, 16);
                                printf("⚠ Aucun vol externe disponible pour le moment.");
                                Sleep(2000);
                                break;
                            }
                        } while (1);
                        fclose(file);
                    }
                    break;
                }
                
                case 2: // Retour
                    return;
            }
            system("cls");
        }
        system("cls");
}while(1);
}





    void menuPostConnexion(char* username) {
        system("cls");
        int currentChoice = 0;
        const int NUM_OPTIONS = 5;
        char* options[] = {
            "Chercher un voyage",
            "Afficher vos réservations internes",
            "Afficher vos réservations externes",
            "Voir nos offres",
            "Retour"
        };

        // Constantes pour le centrage
        const int FRAME_LEFT = 20;
        const int FRAME_RIGHT = 100;
        const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
        const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
        const int MENU_START_Y = 15; // Position Y de départ pour le menu

        while(1) {
            // Affichage initial
            drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 30);
            drawLogo();
            drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

            // Titre avec le nom d'utilisateur
            setColor(9);
            gotoxy(FRAME_LEFT, 12);
            printf("╠══════════════════════════════════ BIENVENUE %s ═══════════════════════════════════════╣", username);

            // Afficher les options (centrées)
            for(int i = 0; i < NUM_OPTIONS; i++) {
                int optionLength = strlen(options[i]);
                // Ajustement du centrage : +5 pour déplacer légèrement vers la droite
                int startX = CENTER_X - (optionLength / 2) + 5;

                if(i == currentChoice) {
                    setColor(13); // Mauve pour la sélection
                    gotoxy(startX - 2, MENU_START_Y + i * 2); // -2 pour la flèche
                    printf("→ %s", options[i]);
                } else {
                    setColor(15); // Blanc pour les autres options
                    gotoxy(startX, MENU_START_Y + i * 2);
                    printf("%s", options[i]);
                }
            }

            // Instructions de navigation (centrées)
            setColor(13);
            const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
            // Ajustement du centrage des instructions : +5 pour déplacer légèrement vers la droite
            gotoxy(CENTER_X - (strlen(instructions) / 2) + 5, 26);
            printf("%s", instructions);

            // Navigation
            char key = _getch();
            if(key == 72) { // Flèche haut
                currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
            }
            else if(key == 80) { // Flèche bas
                currentChoice = (currentChoice + 1) % NUM_OPTIONS;
            }
            else if(key == 13) { // Entrée
                system("cls");

                switch(currentChoice) {
                    case 0: {
                        
                        ReserverVoyage(username);
                        break;
                    }
                    case 1: {
                        
                        afficherReservationsInternes(username);
                        break;
                    }
                    case 2: {
                        
                                
                        afficherReservationsExternes(username);
                        break;
                    }
                    case 3: {
                        

                        rechercherEmploi(username);
                        break;
                }
                case 4:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





























//Partie administrateur;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


//login et sign up
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int loginAdmin() {
    User admin;
    FILE *file = fopen(FILEadmin, "rb");

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }

    // Configuration initiale
    system("cls");
    
    // Dessiner le cadre principal
    drawFrame(20, 1, 100, 30);
    
    // Dessiner le logo
    drawLogo();
    
    // Dessiner le séparateur
    drawSeparator(20, 100, 10);

    // Titre
    setColor(9);
    gotoxy(20, 13);
    printf("╠═══════════════════════════════════════ CONNEXION ADMIN ═══════════════════════════════════════════╣");

    // Saisie de l'identifiant
    drawInputField("Identifiant    ", 16, admin.username, 0);
    
    // Saisie du mot de passe
    drawInputField("Password   ", 20, admin.password, 1);

    User tempAdmin;
    int loginSuccess = 0;
    while (fread(&tempAdmin, sizeof(User), 1, file) == 1) {
        if (strcmp(tempAdmin.username, admin.username) == 0 && strcmp(tempAdmin.password, admin.password) == 0) {
            setColor(10);
            gotoxy(45, 24);
            printf("Connexion réussie ! Bienvenue, %s %s", tempAdmin.nom, tempAdmin.prenom);
            loginSuccess = 1;
            break;
        }
    }

    if (!loginSuccess) {
        setColor(12);
        gotoxy(45, 24);
        printf("Identifiant ou mot de passe administrateur incorrect");
        Sleep(2000);
    } else {
        Sleep(2000);
    }

    fclose(file);
    system("cls");
    return loginSuccess;
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


//gestion des ligne 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to add an internal flight line
void ajouterLigneInterne(FILE *file) {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int INPUT_START_Y = 14;
    const int INPUT_SPACING = 3;
    VoyageInterne voyage;
    char buffer[100];
    int currentPage = 0;
    const int PAGES = 4;

    generateUniqueID(voyage.id);

    while(currentPage < PAGES) {
        system("cls");
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═══════════════════════════════════════ AJOUT LIGNE INTERNE ═══════════════════════════════════════╣");

        // ID Box
        setColor(11);
        gotoxy(CENTER_X - 8, 13);
        printf("┌────────────────────────────────────┐");
        gotoxy(CENTER_X - 8, 14);
        printf("│    ID du voyage: %-17s │", voyage.id);
        gotoxy(CENTER_X - 8, 15);
        printf("└────────────────────────────────────┘");

        if(currentPage == 0) {
            int y = INPUT_START_Y + 3;
            
            drawInputField("Ville de départ", y, voyage.aeroport_depart, 0);
            drawInputField("Ville d'arrivée", y + INPUT_SPACING, voyage.aeroport_arrive, 0);
            drawInputField("Compagnie", y + INPUT_SPACING * 2, voyage.compagnie, 0);

            setColor(13);
            
            gotoxy(CENTER_X - 25, 30);
            printf("Page 1/4 -               ENTRÉE: Continuer,                      ÉCHAP: Retour");

            char key = _getch();
            if(key == 27) return;
            if(key == 13) currentPage++;
        }
        else if(currentPage == 1) {
            int y = INPUT_START_Y +2;
            
            // Date du voyage
            drawDateInputField("Date du voyage", y, CENTER_X, 
                             &voyage.date_voyage.jour, 
                             &voyage.date_voyage.mois, 
                             &voyage.date_voyage.annee);

            setColor(13);
            
            gotoxy(CENTER_X - 25, 30);
            printf("Page 2/4 -               ENTRÉE: Continuer,                      ÉCHAP: Retour");

            char key = _getch();
            if(key == 27) currentPage--;
            if(key == 13) currentPage++;
        }
        else if(currentPage == 2) {
            int y = INPUT_START_Y+2 ;

            // Heure de départ
            drawTimeInputField("Heure de départ", y, CENTER_X,
                             &voyage.heure_depart.heure,
                             &voyage.heure_depart.minute);

            // Heure d'arrivée
            drawTimeInputField("Heure d'arrivée", y + 7, CENTER_X,
                             &voyage.heure_arrivee.heure,
                             &voyage.heure_arrivee.minute);

            setColor(13);
            
            gotoxy(CENTER_X - 25, 30);
            printf("Page 4/4 -               ENTRÉE: Continuer,                      ÉCHAP: Retour");

            char key = _getch();
            if(key == 27) currentPage--;
            if(key == 13) currentPage++;
        }
        else {
            int y = INPUT_START_Y + 3;

            drawInputField("Prix (MAD)", y, buffer, 0);
            if(strlen(buffer) > 0) sscanf(buffer, "%d", &voyage.prix);
            
            drawInputField("Nombre de places", y + INPUT_SPACING, buffer, 0);
            if(strlen(buffer) > 0) sscanf(buffer, "%d", &voyage.nb_place);

            setColor(13);
            
            gotoxy(CENTER_X - 25, 30);
            printf("Page 4/4 -               ENTRÉE: Continuer,                      ÉCHAP: Retour");

            char key = _getch();
            if(key == 27) currentPage--;
            if(key == 13) {
                fwrite(&voyage, sizeof(VoyageInterne), 1, file);
                
                system("cls");
                drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
                drawLogo();
                drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

                setColor(10);
                gotoxy(CENTER_X - 15, 17);
                printf("Ligne interne ajoutée avec succès!");

                setColor(15);
                gotoxy(CENTER_X - 20, 19);
                printf("Appuyez sur une touche pour continuer...");

                _getch();
                currentPage++;
            }
        }
    }
}

void ajouterLigneExterne(FILE *file) {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int INPUT_START_Y = 14;
    const int INPUT_SPACING = 3;
    VoyageExterne voyage;
    char buffer[100];
    int currentPage = 0;
    const int PAGES = 5;

    generateUniqueID(voyage.id);

    while(currentPage < PAGES) {
        system("cls");
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═══════════════════════════════════════ AJOUT LIGNE EXTERNE ═══════════════════════════════════════╣");

        // ID Box
        setColor(11);
        gotoxy(CENTER_X - 8, 13);
        printf("┌────────────────────────────────────┐");
        gotoxy(CENTER_X -8, 14);
        printf("│    ID du voyage: %-17s │", voyage.id);
        gotoxy(CENTER_X - 8, 15);
        printf("└────────────────────────────────────┘");

        if(currentPage == 0) {
            int y = INPUT_START_Y + 3;
            
            drawInputField("Pays de départ", y, voyage.pays_depart, 0);
            drawInputField("Aéroport de départ", y + INPUT_SPACING, voyage.aeroport_depart, 0);

            setColor(13);
            
            gotoxy(CENTER_X - 25, 30);
            printf("Page 1/5 -               ENTRÉE: Continuer,                      ÉCHAP: Retour");

            char key = _getch();
            if(key == 27) return;
            if(key == 13) currentPage++;
        }
        else if(currentPage == 1) {
            int y = INPUT_START_Y + 3;
            
            drawInputField("Pays d'arrivée", y, voyage.pays_arrivee, 0);
            drawInputField("Aéroport d'arrivée", y + INPUT_SPACING, voyage.aeroport_arrive, 0);

            setColor(13);
            
            gotoxy(CENTER_X - 25, 30);
            printf("Page 2/5 -               ENTRÉE: Continuer,                      ÉCHAP: Retour");

            char key = _getch();
            if(key == 27) currentPage--;
            if(key == 13) currentPage++;
        }
        else if(currentPage == 2) {
            int y = INPUT_START_Y + 2;
            
            // Date du voyage
            drawDateInputField("Date du voyage", y, CENTER_X, 
                             &voyage.date_voyage.jour, 
                             &voyage.date_voyage.mois, 
                             &voyage.date_voyage.annee);

            setColor(13);
            
            gotoxy(CENTER_X - 25, 30);
            printf("Page 3/5 -               ENTRÉE: Continuer,                      ÉCHAP: Retour");

            char key = _getch();
            if(key == 27) currentPage--;
            if(key == 13) currentPage++;
        }
        else if(currentPage == 3) {
            int y = INPUT_START_Y +2;

            // Heures de départ et d'arrivée
            drawTimeInputField("Heure de départ", y, CENTER_X,
                             &voyage.heure_depart.heure,
                             &voyage.heure_depart.minute);

            drawTimeInputField("Heure d'arrivée", y + 7, CENTER_X,
                             &voyage.heure_arrivee.heure,
                             &voyage.heure_arrivee.minute);

            setColor(13);
            
            gotoxy(CENTER_X - 25, 30);
            printf("Page 4/5 -               ENTRÉE: Continuer,                      ÉCHAP: Retour");

            char key = _getch();
            if(key == 27) currentPage--;
            if(key == 13) currentPage++;
        }
        else {
            int y = INPUT_START_Y +2 ;

            drawInputField("Compagnie", y, voyage.compagnie, 0);
            drawInputField("Classe (eco/bus/first)", y + INPUT_SPACING, voyage.classe, 0);
            drawInputField("Prix (MAD)", y + INPUT_SPACING * 2, buffer, 0);
            if(strlen(buffer) > 0) sscanf(buffer, "%d", &voyage.prix);
            drawInputField("Nombre de places", y + INPUT_SPACING * 3, buffer, 0);
            if(strlen(buffer) > 0) sscanf(buffer, "%d", &voyage.nb_place);

            setColor(13);
            
            gotoxy(CENTER_X - 25, 30);
            printf("Page 5/5 -               ENTRÉE: Continuer,                      ÉCHAP: Retour");

            char key = _getch();
            if(key == 27) currentPage--;
            if(key == 13) {
                fwrite(&voyage, sizeof(VoyageExterne), 1, file);
                
                system("cls");
                drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
                drawLogo();
                drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

                setColor(10);
                gotoxy(CENTER_X - 15, 17);
                printf("Ligne externe ajoutée avec succès!");

                setColor(15);
                gotoxy(CENTER_X - 20, 19);
                printf("Appuyez sur une touche pour continuer...");

                _getch();
                currentPage++;
            }
        }
    }
}


void AjouterLigne() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 3;
    char* options[] = {
        "Ajouter Ligne Interne",
        "Ajouter Ligne Externe",
        "Retour"
    };
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 16;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═══════════════════════════════════════ AJOUT DES LIGNES ══════════════════════════════════════════╣");
                
        // Sous-titre
        setColor(11);
        gotoxy(CENTER_X - 15, 14);
        printf("          ► Sélection du Type de Ligne");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");
            switch(currentChoice) {
                case 0: {
                    FILE *file = fopen(FILEvoyageInterne, "ab+");
                    if (!file) {
                        system("cls");
                        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
                        drawLogo();
                        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

                        setColor(12);
                        gotoxy(CENTER_X - 25, 15);
                        printf("╔══════════════════════════════════════════════════╗");
                        gotoxy(CENTER_X - 25, 16);
                        printf("║                                                  ║");
                        gotoxy(CENTER_X - 25, 17);
                        printf("║     Erreur d'ouverture du fichier interne       ║");
                        gotoxy(CENTER_X - 25, 18);
                        printf("║                                                  ║");
                        gotoxy(CENTER_X - 25, 19);
                        printf("╚══════════════════════════════════════════════════╝");
                        _getch();
                    } else {
                        ajouterLigneInterne(file);
                        fclose(file);
                    }
                    break;
                }
                case 1: {
                    FILE *file = fopen(FILEvoyageExterne, "ab+");
                    if (!file) {
                        system("cls");
                        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
                        drawLogo();
                        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

                        setColor(12);
                        gotoxy(CENTER_X - 25, 15);
                        printf("╔══════════════════════════════════════════════════╗");
                        gotoxy(CENTER_X - 25, 16);
                        printf("║                                                  ║");
                        gotoxy(CENTER_X - 25, 17);
                        printf("║     Erreur d'ouverture du fichier externe       ║");
                        gotoxy(CENTER_X - 25, 18);
                        printf("║                                                  ║");
                        gotoxy(CENTER_X - 25, 19);
                        printf("╚══════════════════════════════════════════════════╝");
                        _getch();
                    } else {
                        ajouterLigneExterne(file);
                        fclose(file);
                    }
                    break;
                }
                case 2:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}

//supprimer une ligne Interne;
void supprimerLigneInterne() {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    char id[ID_LENGTH + 1] = {0};

    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═════════════════════════════════ SUPPRESSION LIGNE INTERNE ═══════════════════════════════════════╣");
            
    // Afficher un message de confirmation
    setColor(14);  // Jaune pour l'avertissement
    gotoxy(CENTER_X - 25, 14);
    printf("⚠ Attention: Cette action est irréversible!");
    
    setColor(11);
    gotoxy(CENTER_X - 15, 16);
    printf("Entrez l'ID de la ligne à supprimer");

    drawInputField("ID", 18, id, 0);

    // Demander confirmation
    setColor(15);
    gotoxy(CENTER_X - 25, 21);
    printf("Êtes-vous sûr de vouloir supprimer cette ligne ? (O/N): ");
    char confirmation = _getch();
    if (confirmation != 'O' && confirmation != 'o') {
        setColor(14);
        gotoxy(CENTER_X - 15, 23);
        printf("Suppression annulée!");
        Sleep(1500);
        return;
    }

    FILE *file = Ouvrir_Fichier(FILEvoyageInterne, "rb");
    FILE *tempFile = Ouvrir_Fichier("tempInterne.bin", "wb");
    if (!file || !tempFile) {
        setColor(12);
        gotoxy(CENTER_X - 20, 23);
        printf("✗ Erreur lors de l'ouverture des fichiers!");
        Sleep(1500);
        return;
    }

    VoyageInterne voyage;
    int found = 0;

    drawWaitingAnimation(CENTER_X - 12, 23, "Suppression en cours");

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
        setColor(10);
        gotoxy(CENTER_X - 15, 25);
        printf("✓ Ligne supprimée avec succès!");
    } else {
        setColor(12);
        gotoxy(CENTER_X - 15, 25);
        printf("✗ Ligne non trouvée!");
    }
    Sleep(2000);
}

void supprimerLigneExterne() {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    char id[ID_LENGTH + 1] = {0};

    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═════════════════════════════════ SUPPRESSION LIGNE EXTERNE ═════════════════════════════════════╣");
            
    // Afficher un message de confirmation
    setColor(14);  // Jaune pour l'avertissement
    gotoxy(CENTER_X - 25, 14);
    printf("⚠ Attention: Cette action est irréversible!");
    
    setColor(11);
    gotoxy(CENTER_X - 15, 16);
    printf("Entrez l'ID de la ligne à supprimer");

    drawInputField("ID", 18, id, 0);

    // Demander confirmation
    setColor(15);
    gotoxy(CENTER_X - 25, 21);
    printf("Êtes-vous sûr de vouloir supprimer cette ligne ? (O/N): ");
    char confirmation = _getch();
    if (confirmation != 'O' && confirmation != 'o') {
        setColor(14);
        gotoxy(CENTER_X - 15, 23);
        printf("Suppression annulée!");
        Sleep(1500);
        return;
    }

    FILE *file = Ouvrir_Fichier(FILEvoyageExterne, "rb");
    FILE *tempFile = Ouvrir_Fichier("tempExterne.bin", "wb");
    if (!file || !tempFile) {
        setColor(12);
        gotoxy(CENTER_X - 20, 23);
        printf("✗ Erreur lors de l'ouverture des fichiers!");
        Sleep(1500);
        return;
    }

    VoyageExterne voyage;
    int found = 0;

    drawWaitingAnimation(CENTER_X - 12, 23, "Suppression en cours");

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
        setColor(10);
        gotoxy(CENTER_X - 15, 25);
        printf("✓ Ligne supprimée avec succès!");
    } else {
        setColor(12);
        gotoxy(CENTER_X - 15, 25);
        printf("✗ Ligne non trouvée!");
    }
    Sleep(2000);
}


void supprimerLigne() {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    
    int currentChoice = 0;
    const int NUM_OPTIONS = 3;
    char* options[] = {
        "Supprimer une ligne Interne",
        "Supprimer une ligne Externe",
        "Retour au menu principal"
    };

    while(1) {
        system("cls");
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═════════════════════════════════ SUPPRESSION D'UNE LIGNE ═════════════════════════════════════╣");

        setColor(11);
        gotoxy(CENTER_X - 15, 14);
        printf("Choisissez le type de ligne à supprimer :");

        // Affichage des options
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, 17 + i);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, 17 + i);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Gestion des touches
        char key = _getch();
        if(key == 72) currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        else if(key == 80) currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        else if(key == 13) {
            switch(currentChoice) {
                case 0: {
                    supprimerLigneInterne();
                    break;
                }
                case 1: {
                    supprimerLigneExterne();
                    break;
                }
                case 2: {
                    return;
                }
            }
        }
        else if(key == 27) {
            return;
        }
    }
}




//modifier un voyage
void modifierLigneInterne() {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    char id[ID_LENGTH + 1] = {0};

    // Première partie : Recherche du voyage
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠════════════════════════════════ MODIFICATION VOYAGE INTERNE ═════════════════════════════════╣");

    setColor(11);
    gotoxy(CENTER_X - 15, 14);
    printf("             Recherche du Voyage");

    drawInputField("ID du voyage", 17, id, 0);

    FILE *file = Ouvrir_Fichier(FILEvoyageInterne, "rb+");
    if (!file) return;

    VoyageInterne voyage;
    int found = 0;

    drawWaitingAnimation(CENTER_X - 12, 20, "Recherche en cours");

    while (fread(&voyage, sizeof(VoyageInterne), 1, file) == 1) {
        if (strcmp(voyage.id, id) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        setColor(12);
        const char* error_msg = "✗ Voyage non trouvé!";
        gotoxy(CENTER_X - (strlen(error_msg) / 2) + 8, 22);
        printf("%s", error_msg);
        Sleep(2000);
        fclose(file);
        return;
    }

    // Deuxième partie : Menu de modification
    int currentChoice = 0;
    const int NUM_OPTIONS = 10;
    char* options[] = {
        "Aéroport de départ",
        "Aéroport d'arrivée",
        "Date de voyage",
        "Heure de départ",
        "Heure d'arrivée",
        "Nombre de places",
        "Compagnie",
        "Prix",
        "Durée du vol",
        "Terminer la modification"
    };

    while(1) {
        system("cls");
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═════════════════════════════════ MODIFICATION VOYAGE INTERNE ═════════════════════════════════════╣");

        setColor(11);
        gotoxy(CENTER_X - 10, 14);
        printf("Modification du voyage ID: %s", voyage.id);

        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, 16 + i);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, 16 + i);
                printf("%s", options[i]);
            }
        }

        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Modifier    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        char key = _getch();
        if(key == 72) currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        else if(key == 80) currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        else if(key == 13) {
            system("cls");
            drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
            drawLogo();
            drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

            setColor(11);
            gotoxy(CENTER_X - 10, 14);
            printf("  Modification de : %s", options[currentChoice]);

            int valide = 0;
            do {
                switch(currentChoice) {
                    case 0: // Aéroport départ
                        drawInputField("aéroport de départ", 17, voyage.aeroport_depart, 0);
                        valide = strlen(voyage.aeroport_depart) > 0;
                        if (!valide) {
                            afficherMessageErreur("L'aéroport ne peut pas être vide!", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;

                    case 1: // Aéroport arrivée
                        drawInputField("aéroport d'arrivée", 17, voyage.aeroport_arrive, 0);
                        valide = strlen(voyage.aeroport_arrive) > 0;
                        if (!valide) {
                            afficherMessageErreur("L'aéroport ne peut pas être vide!", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;

                    case 2: { // Date voyage
                        int jour, mois, annee;
                        drawDateInputField("Date du voyage", 17, CENTER_X, &jour, &mois, &annee);
                        
                        if (estDateValide(jour, mois, annee)) {
                            voyage.date_voyage.jour = jour;
                            voyage.date_voyage.mois = mois;
                            voyage.date_voyage.annee = annee;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Date invalide! (2024-2030)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 22);
                            Sleep(1500);
                        }
                        break;
                    }

                    case 3: { // Heure départ
                        int heure, minute;
                        drawTimeInputField("Heure de départ", 17, CENTER_X, &heure, &minute);
                        
                        if (estHeureValide(heure, minute)) {
                            voyage.heure_depart.heure = heure;
                            voyage.heure_depart.minute = minute;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Heure invalide! (00-23:00-59)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 22);
                            Sleep(1500);
                        }
                        break;
                    }

                    case 4: { // Heure arrivée
                        int heure, minute;
                        drawTimeInputField("Heure d'arrivée", 17, CENTER_X, &heure, &minute);
                        
                        if (estHeureValide(heure, minute)) {
                            voyage.heure_arrivee.heure = heure;
                            voyage.heure_arrivee.minute = minute;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Heure invalide! (00-23:00-59)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 22);
                            Sleep(1500);
                        }
                        break;
                    }

                    case 5: { // Nombre places
                        char places_str[10] = {0};
                        drawInputField("nombre de places", 17, places_str, 0);
                        int places = atoi(places_str);
                        
                        if (estPlacesValide(places)) {
                            voyage.nb_place = places;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Nombre de places invalide! (1-500)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;
                    }

                    case 6: // Compagnie
                        drawInputField("compagnie", 17, voyage.compagnie, 0);
                        valide = strlen(voyage.compagnie) > 0;
                        if (!valide) {
                            afficherMessageErreur("La compagnie ne peut pas être vide!", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;

                    case 7: { // Prix
                        char prix_str[15] = {0};
                        drawInputField("prix (MAD)", 17, prix_str, 0);
                        int prix = atoi(prix_str);
                        
                        if (estPrixValide(prix)) {
                            voyage.prix = prix;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Prix invalide! (0-100000 MAD)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;
                    }

                    case 8: { // Durée vol
                        int heure, minute;
                        drawTimeInputField("Durée du vol", 17, CENTER_X, &heure, &minute);
                        
                        if (estHeureValide(heure, minute)) {
                            voyage.duree_vol.heure = heure;
                            voyage.duree_vol.minute = minute;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Durée invalide! (00-23:00-59)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 22);
                            Sleep(1500);
                        }
                        break;
                    }

                    case 9: // Terminer
                        if(fseek(file, -sizeof(VoyageInterne), SEEK_CUR) == 0) {
                            fwrite(&voyage, sizeof(VoyageInterne), 1, file);
                            setColor(10);
                            gotoxy(CENTER_X - 10, 25);
                            printf("✓ Modifications enregistrées avec succès!");
                            Sleep(1500);
                            fclose(file);
                            return;
                        }
                        break;
                }
            } while (!valide);

            if (currentChoice != 9) {
                drawWaitingAnimation(CENTER_X - 10, 25, "Sauvegarde en cours");
                fseek(file, -sizeof(VoyageInterne), SEEK_CUR);
                fwrite(&voyage, sizeof(VoyageInterne), 1, file);
                
                setColor(10);
                gotoxy(CENTER_X - 15, 27);
                printf("✓ Modification enregistrée!");
                Sleep(1500);
            }
        }
        else if(key == 27) {
            fclose(file);
            return;
        }
        system("cls");
    }
}

void modifierLigneExterne() {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    char id[ID_LENGTH + 1] = {0};

    // Première partie : Recherche du voyage
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠════════════════════════════════ MODIFICATION VOYAGE EXTERNE ═════════════════════════════════╣");

    setColor(11);
    gotoxy(CENTER_X - 15, 14);
    printf("             Recherche du Voyage");

    drawInputField("ID du voyage", 17, id, 0);

    FILE *file = Ouvrir_Fichier(FILEvoyageExterne, "rb+");
    if (!file) return;

    VoyageExterne voyage;
    int found = 0;

    drawWaitingAnimation(CENTER_X - 12, 20, "Recherche en cours");

    while (fread(&voyage, sizeof(VoyageExterne), 1, file) == 1) {
        if (strcmp(voyage.id, id) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        setColor(12);
        const char* error_msg = "✗ Voyage non trouvé!";
        gotoxy(CENTER_X - (strlen(error_msg) / 2) + 8, 22);
        printf("%s", error_msg);
        Sleep(2000);
        fclose(file);
        return;
    }

    // Deuxième partie : Menu de modification
    int currentChoice = 0;
    const int NUM_OPTIONS = 13;
    char* options[] = {
        "Pays de départ",
        "Aéroport de départ",
        "Pays d'arrivée",
        "Aéroport d'arrivée",
        "Date de voyage",
        "Heure de départ",
        "Heure d'arrivée",
        "Nombre de places",
        "Compagnie",
        "Prix",
        "Classe de vol",
        "Durée du vol",
        "Terminer la modification"
    };

    while(1) {
        system("cls");
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═════════════════════════════════ MODIFICATION VOYAGE EXTERNE ═════════════════════════════════════╣");

        setColor(11);
        gotoxy(CENTER_X - 10, 14);
        printf("Modification du voyage ID: %s", voyage.id);

        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, 16 + i);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, 16 + i);
                printf("%s", options[i]);
            }
        }

        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Modifier    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        char key = _getch();
        if(key == 72) currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        else if(key == 80) currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        else if(key == 13) {
            system("cls");
            drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
            drawLogo();
            drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

            setColor(11);
            gotoxy(CENTER_X - 10, 14);
            printf("  Modification de : %s", options[currentChoice]);

            int valide = 0;
            do {
                switch(currentChoice) {
                    case 0: // Pays départ
                        drawInputField("pays de départ", 17, voyage.pays_depart, 0);
                        valide = strlen(voyage.pays_depart) > 0;
                        if (!valide) {
                            afficherMessageErreur("Le pays ne peut pas être vide!", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;

                    case 1: // Aéroport départ
                        drawInputField("aéroport de départ", 17, voyage.aeroport_depart, 0);
                        valide = strlen(voyage.aeroport_depart) > 0;
                        if (!valide) {
                            afficherMessageErreur("L'aéroport ne peut pas être vide!", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;

                    case 2: // Pays arrivée
                        drawInputField("pays d'arrivée", 17, voyage.pays_arrivee, 0);
                        valide = strlen(voyage.pays_arrivee) > 0;
                        if (!valide) {
                            afficherMessageErreur("Le pays ne peut pas être vide!", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;

                    case 3: // Aéroport arrivée
                        drawInputField("Naéroport d'arrivée", 17, voyage.aeroport_arrive, 0);
                        valide = strlen(voyage.aeroport_arrive) > 0;
                        if (!valide) {
                            afficherMessageErreur("L'aéroport ne peut pas être vide!", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;

                    case 4: { // Date voyage
                        int jour, mois, annee;
                        drawDateInputField("Date du voyage", 17, CENTER_X, &jour, &mois, &annee);
                        
                        if (estDateValide(jour, mois, annee)) {
                            voyage.date_voyage.jour = jour;
                            voyage.date_voyage.mois = mois;
                            voyage.date_voyage.annee = annee;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Date invalide! (2024-2030)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 22);
                            Sleep(1500);
                        }
                        break;
                    }

                    case 5: { // Heure départ
                        int heure, minute;
                        drawTimeInputField("Heure de départ", 17, CENTER_X, &heure, &minute);
                        
                        if (estHeureValide(heure, minute)) {
                            voyage.heure_depart.heure = heure;
                            voyage.heure_depart.minute = minute;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Heure invalide! (00-23:00-59)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 22);
                            Sleep(1500);
                        }
                        break;
                    }

                    case 6: { // Heure arrivée
                        int heure, minute;
                        drawTimeInputField("Heure d'arrivée", 17, CENTER_X, &heure, &minute);
                        
                        if (estHeureValide(heure, minute)) {
                            voyage.heure_arrivee.heure = heure;
                            voyage.heure_arrivee.minute = minute;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Heure invalide! (00-23:00-59)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 22);
                            Sleep(1500);
                        }
                        break;
                    }

                    case 7:{ // Nombre places
                        char places_str[10] = {0};
                        drawInputField("nombre de places", 17, places_str, 0);
                        int places = atoi(places_str);
                        
                        if (estPlacesValide(places)) {
                            voyage.nb_place = places;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Nombre de places invalide! (1-500)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;
                    }
                    case 8: // Compagnie
                        drawInputField("compagnie", 17, voyage.compagnie, 0);
                        valide = strlen(voyage.compagnie) > 0;
                        if (!valide) {
                            afficherMessageErreur("La compagnie ne peut pas être vide!", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;

                    case 9:{// Prix
                        char prix_str[15] = {0};
                        drawInputField("Nouveau prix (MAD)", 17, prix_str, 0);
                        float prix = atof(prix_str);
                        
                        if (estPrixValide(prix)) {
                            voyage.prix = prix;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Prix invalide! (0-100000 MAD)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;
                    }
                    case 10: // Classe
                        drawInputField("classe de vol", 17, voyage.classe, 0);
                        valide = strlen(voyage.classe) > 0;
                        if (!valide) {
                            afficherMessageErreur("La classe ne peut pas être vide!", CENTER_X);
                            clearInputArea(CENTER_X, 17, 19);
                            Sleep(1500);
                        }
                        break;

                    case 11: { // Durée vol
                        int heure, minute;
                        drawTimeInputField("Durée du vol", 17, CENTER_X, &heure, &minute);
                        
                        if (estHeureValide(heure, minute)) {
                            voyage.duree_vol.heure = heure;
                            voyage.duree_vol.minute = minute;
                            valide = 1;
                        } else {
                            afficherMessageErreur("Durée invalide! (00-23:00-59)", CENTER_X);
                            clearInputArea(CENTER_X, 17, 22);
                            Sleep(1500);
                        }
                        break;
                    }

                    case 12: // Terminer
                        if(fseek(file, -sizeof(VoyageExterne), SEEK_CUR) == 0) {
                            fwrite(&voyage, sizeof(VoyageExterne), 1, file);
                            setColor(10);
                            gotoxy(CENTER_X - 10, 25);
                            printf("✓ Modifications enregistrées avec succès!");
                            Sleep(1500);
                            fclose(file);
                            return;
                        }
                        break;
                }
            } while (!valide);

            if (currentChoice != 12) {
                drawWaitingAnimation(CENTER_X - 10, 25, "Sauvegarde en cours");
                fseek(file, -sizeof(VoyageExterne), SEEK_CUR);
                fwrite(&voyage, sizeof(VoyageExterne), 1, file);
                
                setColor(10);
                gotoxy(CENTER_X - 15, 27);
                printf("✓ Modification enregistrée!");
                Sleep(1500);
            }
        }
        else if(key == 27) {
            fclose(file);
            return;
        }
        system("cls");
    }
}

void modifierVoyage() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 3;
    char* options[] = {
        "Modifier Ligne Interne",
        "Modifier Ligne Externe",
        "Retour"
    };
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 16;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠════════════════════════════════════ MODIFICATION DES LIGNES ════════════════════════════════════╣");

        // Sous-titre
        setColor(11);
        gotoxy(CENTER_X - 15, 14);
        printf("          ► Sélection du Type de Ligne");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");
            switch(currentChoice) {
                case 0:
                    modifierLigneInterne();
                    break;
                case 1:
                    modifierLigneExterne();
                    break;
                case 2:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}



void gestionlignes() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 5;
    char* options[] = {
        "Afficher les lignes",
        "Ajouter une ligne",
        "Supprimer une ligne",
        "Modifier une ligne",
        "Retour"
    };
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 16;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═════════════════════════════════════ GESTION DES LIGNES ══════════════════════════════════════════╣");
                
        

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");
            switch(currentChoice) {
                case 0:
                    afficherLignes();
                    break;
                case 1:
                    AjouterLigne();
                    break;
                case 2:
                    supprimerLigne();
                    break;
                case 3:
                    modifierVoyage();
                    break;
                case 4:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////






//partie des reservations
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void afficherReservations(FILE* fichier) {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int ITEMS_PER_PAGE = 8;
    const int TABLE_LEFT = FRAME_LEFT + 5; // Décalage à droite du tableau
    
    Reservation reservation;
    int currentPage = 0;
    int totalReservations = 0;
    
    while (fread(&reservation, sizeof(Reservation), 1, fichier)) {
        totalReservations++;
    }
    rewind(fichier);
    
    int totalPages = (totalReservations + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    
    while(1) {
        system("cls");
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═════════════════════════════════ LISTE DES RESERVATIONS ══════════════════════════════════════════╣");

        // Style du tableau
        setColor(11);
        gotoxy(TABLE_LEFT+5, 14);
        printf("┌────────────────────┬───────────────┬──────────┬────────────┬─────────────┐");
        gotoxy(TABLE_LEFT+5, 15);
        printf("│  Nom d'utilisateur │   ID Voyage   │  Places  │    Date    │   Paiement  │");
        gotoxy(TABLE_LEFT+5, 16);
        printf("├────────────────────┼───────────────┼──────────┼────────────┼─────────────┤");

        // Afficher les réservations de la page courante
        setColor(15);
        fseek(fichier, currentPage * ITEMS_PER_PAGE * sizeof(Reservation), SEEK_SET);
        int itemsOnPage = 0;
        
        for(int i = 0; i < ITEMS_PER_PAGE && fread(&reservation, sizeof(Reservation), 1, fichier); i++) {
            gotoxy(TABLE_LEFT+5, 17 + i * 2);
            printf("│ %-18s │ %-13s │    %-5d │ %02d/%02d/%04d │  %8.2f   │",
                   reservation.username,
                   reservation.voyage_id,
                   reservation.nb_places_reservees,
                   reservation.date_reservation.jour,
                   reservation.date_reservation.mois,
                   reservation.date_reservation.annee,
                   reservation.montant_paye);
            
            // Ligne de séparation entre les entrées
            if (i < ITEMS_PER_PAGE - 1) {
                gotoxy(TABLE_LEFT+5, 18 + i * 2);
                printf("├────────────────────┼───────────────┼──────────┼────────────┼─────────────┤");
            }
            itemsOnPage++;
        }

        // Fermeture du tableau
        gotoxy(TABLE_LEFT+5, 17 + (itemsOnPage * 2));
        printf("└────────────────────┴───────────────┴──────────┴────────────┴─────────────┘");

        
        setColor(13);
        // Instructions de navigation
        const char* instructions = "←→: Navigation                                      ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 8, 31);
        printf("%s", instructions);
         // Information de pagination
        char pageInfo[50];
        sprintf(pageInfo, "Page %d/%d\n\n", currentPage + 1, totalPages);
        gotoxy(CENTER_X - (strlen(pageInfo) / 2) + 8, 31);
        printf("%s", pageInfo);

        // Navigation
        char key = _getch();
        if(key == 75 && currentPage > 0) { // Flèche gauche
            currentPage--;
        }
        else if(key == 77 && currentPage < totalPages - 1) { // Flèche droite
            currentPage++;
        }
        else if(key == 27) { // Échap
            break;
        }
    }
}
void ConsulterVoyages() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 3;
    char* options[] = {
        "Réservations Internes",
        "Réservations Externes",
        "Retour"
    };
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 16;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═════════════════════════════════ CONSULTATION DES RESERVATIONS ═════════════════════════════════╣");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");
            FILE *fichier = NULL;
            switch(currentChoice) {
                case 0:
                    fichier = Ouvrir_Fichier("reservationsInterne.bin", "rb");
                    if(fichier != NULL) {
                        afficherReservations(fichier);
                    }
                    break;
                case 1:
                    fichier = Ouvrir_Fichier("reservationsExterne.bin", "rb");
                    if(fichier != NULL) {
                        afficherReservations(fichier);
                    }
                    break;
                case 2:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}


void supprimerReservation(const char *nom_fichier) {
    char username[50] = {0};
    char voyage_id[30] = {0};
    
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    
    // Première section - Saisie des informations
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠══════════════════════════════════ SUPPRIMER UNE RESERVATION ══════════════════════════════════════╣");

    setColor(11);
    gotoxy(CENTER_X - 15, 14);
    printf("          ► Recherche de Réservation");

    // Saisie des informations
    drawInputField("Nom d'utilisateur", 17, username, 0);
    drawInputField("ID du voyage", 20, voyage_id, 0);

    // Message pour confirmer
    setColor(13);
    const char* confirm_msg = "Appuyez sur ENTRÉE pour confirmer la suppression...";
    gotoxy(CENTER_X - (strlen(confirm_msg) / 2)+8, 26);
    printf("%s", confirm_msg);
    while(getch() != 13); // Attendre ENTRÉE

    // Deuxième section - Confirmation et traitement
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═════════════════════════════════ SUPPRIMER UNE RESERVATION ═══════════════════════════════════════╣");

    setColor(11);
    gotoxy(CENTER_X - 15, 14);
    printf("         ► Confirmation de Suppression");

    // Afficher les détails de la réservation
    setColor(15);
    gotoxy(CENTER_X - 20, 16);
    printf("Vous allez supprimer la réservation de : %s", username);
    gotoxy(CENTER_X - 20, 17);
    printf("ID du voyage : %s", voyage_id);

    FILE *fichier = fopen(nom_fichier, "rb");
    if (fichier != NULL) {
        // Animation de recherche
        drawWaitingAnimation(CENTER_X - 5, 20, "Recherche en cours");

        FILE *temp_file = fopen("temp.bin", "wb");
        if (temp_file != NULL) {
            Reservation reservation;
            int found = 0;

            while (fread(&reservation, sizeof(Reservation), 1, fichier)) {
                if (strcmp(reservation.username, username) == 0 && 
                    strcmp(reservation.voyage_id, voyage_id) == 0) {
                    found = 1;
                } else {
                    fwrite(&reservation, sizeof(Reservation), 1, temp_file);
                }
            }

            fclose(fichier);
            fclose(temp_file);

            // Effacer la ligne de l'animation
            gotoxy(CENTER_X - 5, 20);
            printf("                                                  ");

            if (found) {
                // Animation de suppression
                drawWaitingAnimation(CENTER_X - 5, 20, "Suppression en cours");
                remove(nom_fichier);
                rename("temp.bin", nom_fichier);

                setColor(10);
                const char* success_msg = "✓ Réservation supprimée avec succès!";
                gotoxy(CENTER_X - (strlen(success_msg) / 2)+8, 20);
                printf("%s", success_msg);
            } else {
                remove("temp.bin");
                setColor(12);
                const char* error_msg = "✗ Réservation non trouvée!";
                gotoxy(CENTER_X - (strlen(error_msg) / 2)+8, 20);
                printf("%s", error_msg);
            }
        }
    } else {
        setColor(12);
        const char* error_msg = "✗ Erreur d'ouverture du fichier!";
        gotoxy(CENTER_X - (strlen(error_msg) / 2), 29);
        printf("%s", error_msg);
    }

    Sleep(2000);
}



void SupprimerReservations() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 3;
    char* options[] = {
        "Supprimer une réservation Interne",
        "Supprimer une réservation Externe",
        "Retour"
    };
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 16;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠════════════════════════════════ SUPPRESSION DES RESERVATIONS ═════════════════════════════════════╣");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");
            char* nom_fichier;
            switch(currentChoice) {
                case 0:
                    nom_fichier = "reservationsInterne.bin";
                    supprimerReservation(nom_fichier);
                    break;
                case 1:
                    nom_fichier = "reservationsExterne.bin";
                    supprimerReservation(nom_fichier);
                    break;
                case 2:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}



// Fonction pour afficher le menu de gestion des réservations
void GestionReservations() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 3;
    char* options[] = {
        "Supprimer une réservation",
        "Afficher toutes les réservations",
        "Retour"
    };
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 16;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═══════════════════════════════════ GESTION DES RESERVATIONS ══════════════════════════════════════╣");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");
            switch(currentChoice) {
                case 0:
                    SupprimerReservations();
                    break;
                case 1:
                    ConsulterVoyages();
                    break;
                case 2:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//partie de statistiques

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    FILE *binFile = fopen("reservationsInterne.bin", "rb");
    if (!binFile) return NULL;

    _mkdir("reports");
    _mkdir("reports/Interne");
    _mkdir("reports/Interne/statistique_reservations");
    _mkdir("reports/Interne/statistique_reservations/reports-mois");

    static char filename[256];
    snprintf(filename, sizeof(filename), "reports/Interne/statistique_reservations/reports-mois/%d-%02d_reservations.txt", year, month);

    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    int reservationsPerDay[31] = {0};
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.mois == month && 
            reservations[i].date_reservation.annee == year) {
            reservationsPerDay[reservations[i].date_reservation.jour - 1] += reservations[i].nb_places_reservees;
        }
    }

    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) return NULL;

    for (int day = 0; day < 31; day++) {
        fprintf(txtFile, "%d %d\n", day + 1, reservationsPerDay[day]);
    }
    fclose(txtFile);
    return filename;
}

char* generate_Monthly_External_reserv_Report() {
    int year, month;
    getCurrentYearAndMonth(&year, &month);

    FILE *binFile = fopen("reservationsExterne.bin", "rb");
    if (!binFile) return NULL;

    _mkdir("reports");
    _mkdir("reports/Externe");
    _mkdir("reports/Externe/statistique_reservations");
    _mkdir("reports/Externe/statistique_reservations/reports-mois");

    static char filename[256];
    snprintf(filename, sizeof(filename), "reports/Externe/statistique_reservations/reports-mois/%d-%02d_reservations_externe.txt", year, month);

    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    int reservationsPerDay[31] = {0};
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.mois == month && 
            reservations[i].date_reservation.annee == year) {
            reservationsPerDay[reservations[i].date_reservation.jour - 1] += reservations[i].nb_places_reservees;
        }
    }

    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) return NULL;

    for (int day = 0; day < 31; day++) {
        fprintf(txtFile, "%d %d\n", day + 1, reservationsPerDay[day]);
    }
    fclose(txtFile);
    return filename;
}
void ReservationM() {
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + 10 + (FRAME_WIDTH / 2);
    
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═══════════════════════════════ STATISTIQUES RESERVATIONS MENSUELLES ══════════════════════════════╣");

    int year, month;
    getCurrentYearAndMonth(&year, &month);

    setColor(11);
    gotoxy(CENTER_X - 18, 14);
    printf("► Génération du Rapport pour %02d/%d", month, year);

    // Animation de génération
    drawWaitingAnimation(CENTER_X - 10, 16, "Génération des rapports");

    // Générer les rapports
    char* reportFileInterne = generate_Monthly_Internal_reserv_Report();
    char* reportFileExterne = generate_Monthly_External_reserv_Report();

    if (reportFileInterne != NULL && reportFileExterne != NULL) {
        // Animation de traitement
        drawWaitingAnimation(CENTER_X - 10, 16, "Traitement des données");

        // Construire la commande Python
        char command[512];
        snprintf(command, sizeof(command), 
                "C:\\Users\\DELL\\AppData\\Local\\Programs\\Python\\Python313\\pythonw.exe graphes\\reservationsM.py %s %s", 
                reportFileInterne, reportFileExterne);

        // Exécuter le script Python
        int result = system(command);

        if (result == 0) {
            setColor(10);
            const char* success_msg = "✓ Le graphe mensuel des réservations a été généré avec succès!";
            gotoxy(CENTER_X - (strlen(success_msg) / 2), 16);
            printf("%s", success_msg);
        } else {
            setColor(12);
            const char* error_msg = "✗ Erreur lors de la génération du graphe";
            gotoxy(CENTER_X - (strlen(error_msg) / 2), 16);
            printf("%s", error_msg);
        }
    } else {
        setColor(12);
        const char* error_msg = "✗ Erreur lors de la génération des rapports";
        gotoxy(CENTER_X - (strlen(error_msg) / 2), 16);
        printf("%s", error_msg);
    }
    Sleep(2000);
}





// reservation annuelle
// Fonction pour obtenir l'année actuelle
void getCurrentYear(int* year) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    *year = tm.tm_year + 1900; // tm_year commence à partir de 1900
}


char* generate_Annual_Internal_reserv_Report() {
    int year;
    getCurrentYear(&year);

    FILE *binFile = fopen("reservationsInterne.bin", "rb");
    if (!binFile) return NULL;

    _mkdir("reports");
    _mkdir("reports/Interne");
    _mkdir("reports/Interne/statistique_reservations");
    _mkdir("reports/Interne/statistique_reservations/reports-anne");

    static char filename[256];
    snprintf(filename, sizeof(filename), "reports/Interne/statistique_reservations/reports-anne/%d_annual_reservations.txt", year);

    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    int reservationsPerMonth[12] = {0};
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.annee == year) {
            reservationsPerMonth[reservations[i].date_reservation.mois - 1] += reservations[i].nb_places_reservees;
        }
    }

    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) return NULL;

    for (int month = 0; month < 12; month++) {
        fprintf(txtFile, "%d %d\n", month + 1, reservationsPerMonth[month]);
    }
    fclose(txtFile);
    return filename;
}




char* generate_Annual_External_reserv_Report() {
    int year;
    getCurrentYear(&year);

    FILE *binFile = fopen("reservationsExterne.bin", "rb");
    if (!binFile) return NULL;

    _mkdir("reports");
    _mkdir("reports/Externe");
    _mkdir("reports/Externe/statistique_reservations");
    _mkdir("reports/Externe/statistique_reservations/reports-anne");

    static char filename[256];
    snprintf(filename, sizeof(filename), "reports/Externe/statistique_reservations/reports-anne/%d_annual_reservations_externe.txt", year);

    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    int reservationsPerMonth[12] = {0};
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.annee == year) {
            reservationsPerMonth[reservations[i].date_reservation.mois - 1] += reservations[i].nb_places_reservees;
        }
    }

    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) return NULL;

    for (int month = 0; month < 12; month++) {
        fprintf(txtFile, "%d %d\n", month + 1, reservationsPerMonth[month]);
    }
    fclose(txtFile);
    return filename;
}



// Fonction principale qui appelle les deux fonctions : Génération du rapport et tracé avec Gnuplot
void ReservationA() {
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + 10 + (FRAME_WIDTH / 2);
    
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠════════════════════════════════ STATISTIQUES RESERVATIONS ANNUELLES ══════════════════════════════╣");

    int year;
    getCurrentYear(&year);

    setColor(11);
    gotoxy(CENTER_X - 18, 14);
    printf("► Génération du Rapport pour l'année %d", year);

    // Animation de génération
    drawWaitingAnimation(CENTER_X - 10, 16, "Génération des rapports");

    // Générer les rapports
    char* reportFileInterne = generate_Annual_Internal_reserv_Report();
    char* reportFileExterne = generate_Annual_External_reserv_Report();

    if (reportFileInterne != NULL && reportFileExterne != NULL) {
        // Animation de traitement
        drawWaitingAnimation(CENTER_X - 10, 16, "Traitement des données");

        // Construire la commande Python
        char command[512];
        snprintf(command, sizeof(command), 
                "C:\\Users\\DELL\\AppData\\Local\\Programs\\Python\\Python313\\pythonw.exe graphes\\reservationsA.py %s %s", 
                reportFileInterne, reportFileExterne);

        // Exécuter le script Python
        int result = system(command);

        if (result == 0) {
            setColor(10);
            const char* success_msg = "✓ Le graphe annuel des réservations a été généré avec succès!";
            gotoxy(CENTER_X - (strlen(success_msg) / 2), 16);
            printf("%s", success_msg);
        } else {
            setColor(12);
            const char* error_msg = "✗ Erreur lors de la génération du graphe";
            gotoxy(CENTER_X - (strlen(error_msg) / 2), 16);
            printf("%s", error_msg);
        }
    } else {
        setColor(12);
        const char* error_msg = "✗ Erreur lors de la génération des rapports";
        gotoxy(CENTER_X - (strlen(error_msg) / 2), 16);
        printf("%s", error_msg);
    }
    Sleep(2000);
}







void statistiqueReservation() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 2;
    char* options[] = {
        "Mensuelle",
        "Annuelle"
    };
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 16;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠══════════════════════════════════ STATISTIQUES RESERVATIONS ══════════════════════════════════════╣");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf(" → %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");
            switch(currentChoice) {
                case 0:
                    ReservationM();
                    break;
                case 1:
                    ReservationA();
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}









//stistique sur le revenue
// Fonction pour générer le rapport des revenus mensuels

char* generate_Monthly_Internal_Revenue_Report() {
    int year, month;
    getCurrentYearAndMonth(&year, &month);

    FILE *binFile = fopen("reservationsInterne.bin", "rb");
    if (!binFile) return NULL;

    _mkdir("reports");
    _mkdir("reports/Interne");
    _mkdir("reports/Interne/statistique_revenue");
    _mkdir("reports/Interne/statistique_revenue/reports-mois");

    static char filename[256];
    snprintf(filename, sizeof(filename), "reports/Interne/statistique_revenue/reports-mois/%d-%02d_revenue.txt", year, month);

    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    float revenuePerDay[31] = {0.0};
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.mois == month && 
            reservations[i].date_reservation.annee == year) {
            revenuePerDay[reservations[i].date_reservation.jour - 1] += reservations[i].montant_paye;
        }
    }

    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) return NULL;

    for (int day = 0; day < 31; day++) {
        fprintf(txtFile, "%d %.2f\n", day + 1, revenuePerDay[day]);
    }
    fclose(txtFile);
    return filename;
}



char* generate_Monthly_External_Revenue_Report() {
    int year, month;
    getCurrentYearAndMonth(&year, &month);

    FILE *binFile = fopen("reservationsExterne.bin", "rb");
    if (!binFile) return NULL;

    _mkdir("reports");
    _mkdir("reports/Externe");
    _mkdir("reports/Externe/statistique_revenue");
    _mkdir("reports/Externe/statistique_revenue/reports-mois");

    static char filename[256];
    snprintf(filename, sizeof(filename), "reports/Externe/statistique_revenue/reports-mois/%d-%02d_revenue_externe.txt", year, month);

    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    float revenuePerDay[31] = {0.0};
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.mois == month && 
            reservations[i].date_reservation.annee == year) {
            revenuePerDay[reservations[i].date_reservation.jour - 1] += reservations[i].montant_paye;
        }
    }

    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) return NULL;

    for (int day = 0; day < 31; day++) {
        fprintf(txtFile, "%d %.2f\n", day + 1, revenuePerDay[day]);
    }
    fclose(txtFile);
    return filename;
}
void RevenueM() {
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT +10+ (FRAME_WIDTH / 2);
    
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠══════════════════════════════════════ REVENUS MENSUELS ═══════════════════════════════════════════╣");

    int year, month;
    getCurrentYearAndMonth(&year, &month);

    setColor(11);
    gotoxy(CENTER_X - 18 ,14);
    printf("► Génération du Rapport pour %02d/%d", month, year);

    // Animation de génération
    drawWaitingAnimation(CENTER_X - 10, 16, "Génération des rapports");

    // Générer les rapports
    char* reportFileInterne = generate_Monthly_Internal_Revenue_Report();
    char* reportFileExterne = generate_Monthly_External_Revenue_Report();

    if (reportFileInterne != NULL && reportFileExterne != NULL) {
        // Animation de traitement
        drawWaitingAnimation(CENTER_X - 10, 16, "Traitement des données");

        // Construire la commande Python
        char command[512];
        snprintf(command, sizeof(command), 
                "C:\\Users\\DELL\\AppData\\Local\\Programs\\Python\\Python313\\pythonw.exe graphes\\revenueM.py %s %s", 
                reportFileInterne, reportFileExterne);

        // Exécuter le script Python
        int result = system(command);

        if (result == 0) {
            setColor(10);
            const char* success_msg = "✓ Le graphe mensuel des revenus a été généré avec succès!";
            gotoxy(CENTER_X - (strlen(success_msg) / 2), 16);
            printf("%s", success_msg);
        } else {
            setColor(12);
            const char* error_msg = "✗ Erreur lors de la génération du graphe";
            gotoxy(CENTER_X - (strlen(error_msg) / 2), 16);
            printf("%s", error_msg);
        }
    } else {
        setColor(12);
        const char* error_msg = "✗ Erreur lors de la génération des rapports";
        gotoxy(CENTER_X - (strlen(error_msg) / 2), 16);
        printf("%s", error_msg);
    }
    Sleep(2000);
}





// Fonction pour générer le rapport des revenus annuels
char* generate_Annual_Internal_Revenue_Report() {
    int year;
    getCurrentYear(&year);

    FILE *binFile = fopen("reservationsInterne.bin", "rb");
    if (!binFile) return NULL;

    _mkdir("reports");
    _mkdir("reports/Interne");
    _mkdir("reports/Interne/statistique_revenue");
    _mkdir("reports/Interne/statistique_revenue/reports-anne");

    static char filename[256];
    snprintf(filename, sizeof(filename), "reports/Interne/statistique_revenue/reports-anne/%d_annual_revenue.txt", year);

    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    float revenuePerMonth[12] = {0.0};
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.annee == year) {
            revenuePerMonth[reservations[i].date_reservation.mois - 1] += reservations[i].montant_paye;
        }
    }

    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) return NULL;

    for (int month = 0; month < 12; month++) {
        fprintf(txtFile, "%d %.2f\n", month + 1, revenuePerMonth[month]);
    }
    fclose(txtFile);
    return filename;
}


// Fonction pour générer le rapport des revenus annuels externes
char* generate_Annual_External_Revenue_Report() {
    int year;
    getCurrentYear(&year);

    FILE *binFile = fopen("reservationsExterne.bin", "rb");
    if (!binFile) return NULL;

    _mkdir("reports");
    _mkdir("reports/Externe");
    _mkdir("reports/Externe/statistique_revenue");
    _mkdir("reports/Externe/statistique_revenue/reports-anne");

    static char filename[256];
    snprintf(filename, sizeof(filename), "reports/Externe/statistique_revenue/reports-anne/%d_annual_revenue_externe.txt", year);

    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, binFile);
    fclose(binFile);

    float revenuePerMonth[12] = {0.0};
    for (int i = 0; i < numReservations; i++) {
        if (reservations[i].date_reservation.annee == year) {
            revenuePerMonth[reservations[i].date_reservation.mois - 1] += reservations[i].montant_paye;
        }
    }

    FILE *txtFile = fopen(filename, "w");
    if (!txtFile) return NULL;

    for (int month = 0; month < 12; month++) {
        fprintf(txtFile, "%d %.2f\n", month + 1, revenuePerMonth[month]);
    }
    fclose(txtFile);
    return filename;
}



void RevenueA() {
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT +10+ (FRAME_WIDTH / 2);
    
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═══════════════════════════════════════ REVENUS ANNUELS ═══════════════════════════════════════════╣");
             
    int year;
    getCurrentYear(&year);

    setColor(11);
    gotoxy(CENTER_X - 18, 14);
    printf("► Génération du Rapport pour l'année %d", year);

    // Animation de génération
    drawWaitingAnimation(CENTER_X - 10, 16, "Génération des rapports");

    // Générer les rapports
    char* reportFileInterne = generate_Annual_Internal_Revenue_Report();
    char* reportFileExterne = generate_Annual_External_Revenue_Report();

    if (reportFileInterne != NULL && reportFileExterne != NULL) {
        // Animation de traitement
        drawWaitingAnimation(CENTER_X - 10, 16, "Traitement des données");

        // Construire la commande Python
        char command[512];
        snprintf(command, sizeof(command), 
                "C:\\Users\\DELL\\AppData\\Local\\Programs\\Python\\Python313\\pythonw.exe graphes\\revenueA.py %s %s", 
                reportFileInterne, reportFileExterne);

        // Exécuter le script Python
        int result = system(command);

        if (result == 0) {
            setColor(10);
            const char* success_msg = "✓ Le graphe annuel des revenus a été généré avec succès!";
            gotoxy(CENTER_X - (strlen(success_msg) / 2)+5, 16);
            printf("%s", success_msg);
        } else {
            setColor(12);
            const char* error_msg = "✗ Erreur lors de la génération du graphe";
            gotoxy(CENTER_X - (strlen(error_msg) / 2)+5, 16);
            printf("%s", error_msg);
        }
    } else {
        setColor(12);
        const char* error_msg = "✗ Erreur lors de la génération des rapports";
        gotoxy(CENTER_X - (strlen(error_msg) / 2)+5, 16);
        printf("%s", error_msg);
    }
    Sleep(2000);
}




void statistiqueRevenue() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 3;
    char* options[] = {
        "Revenus Mensuels",
        "Revenus Annuels",
        "Retour"
    };
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 16;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═══════════════════════════════════ STATISTIQUES DES REVENUS ══════════════════════════════════════╣");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");
            switch(currentChoice) {
                case 0:
                    RevenueM();
                    break;
                case 1:
                    RevenueA();
                    break;
                case 2:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}

// menue de statistique ;

void ConsulterStatistique() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 3;
    char* options[] = {
        " Réservations",
        "Revenus",
        "Retour"
    };
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 16;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═════════════════════════════════════════ STATISTIQUES ════════════════════════════════════════════╣");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");
            switch(currentChoice) {
                case 0:
                    statistiqueReservation();
                    break;
                case 1:
                    statistiqueRevenue();
                    break;
                case 2:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////










// Fonction pour ajouter une compagnie
void ajouterCompagnie() {
    Compagnie nouvelleCompagnie;
    char annee_str[5] = {0};
    char avions_str[5] = {0};
    char montant_str[20] = {0};
    
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    
    // Première section - Informations de la compagnie
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═════════════════════════════════════════ AJOUTER UNE COMPAGNIE ═══════════════════════════════════╣");

    setColor(11);
    gotoxy(CENTER_X - 30, 16);
    printf("► Informations de la Compagnie");

    // Premier groupe de champs
    drawInputField("Nom", 17, nouvelleCompagnie.nom, 0);
    drawInputField("Pays", 20, nouvelleCompagnie.pays, 0);
    drawInputField("Année de création", 23, annee_str, 0);
    drawInputField("Nombre d'avions", 26, avions_str, 0);

    // Message pour continuer
    setColor(13);
    const char* next_msg = "Appuyez sur ENTRÉE pour continuer...";
    gotoxy(CENTER_X - (strlen(next_msg) / 2)+5, 29);
    printf("%s", next_msg);
    while(getch() != 13); // Attendre ENTRÉE

    // Deuxième section - Informations du contrat
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═════════════════════════════════════ AJOUTER UNE COMPAGNIE ═══════════════════════════════════════╣");

    setColor(11);
    gotoxy(CENTER_X - 30, 16);
    printf("► Informations du Contrat");

    // Deuxième groupe de champs
    drawInputField("ID Contrat", 17, nouvelleCompagnie.contrat.id_contrat, 0);
    drawInputField("Date début", 20, nouvelleCompagnie.contrat.date_debut, 0);
    drawInputField("Date fin", 23, nouvelleCompagnie.contrat.date_fin, 0);
    drawInputField("Montant", 26, montant_str, 0);
    
    // Conversion des valeurs
    nouvelleCompagnie.annee_creation = atoi(annee_str);
    nouvelleCompagnie.nb_avions = atoi(avions_str);
    nouvelleCompagnie.contrat.montant = atof(montant_str);

    // Sauvegarde dans le fichier
    FILE *fichier = fopen(FILENAME, "ab");
    if (fichier != NULL) {
        fwrite(&nouvelleCompagnie, sizeof(Compagnie), 1, fichier);
        fclose(fichier);
        
        // Message de succès
        setColor(10);
        const char* success_msg = "✓ Compagnie ajoutée avec succès!";
        gotoxy(CENTER_X - (strlen(success_msg) / 2)+5, 29);
        printf("%s", success_msg);
        Sleep(2000);
    } else {
        // Message d'erreur
        setColor(12);
        const char* error_msg = "✗ Erreur lors de l'enregistrement!";
        gotoxy(CENTER_X - (strlen(error_msg) / 2)+5, 29);
        printf("%s", error_msg);
        Sleep(2000);
    }
}

void supprimerCompagnie() {
    char nomCompagnie[30] = {0};
    
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    
    // Première section - Saisie du nom
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠════════════════════════════════════ SUPPRIMER UNE COMPAGNIE ══════════════════════════════════════╣");

    setColor(11);
    gotoxy(CENTER_X - 5, 15);
    printf("Recherche de Compagnie");

    drawInputField("Nom Compagne", 17, nomCompagnie, 0);

    // Message pour confirmer
    setColor(13);
    const char* confirm_msg = "Appuyez sur ENTRÉE pour confirmer la suppression...";
    gotoxy(CENTER_X - (strlen(confirm_msg) / 2)+5, 20);
    printf("%s", confirm_msg);
    while(getch() != 13); // Attendre ENTRÉE

    // Deuxième section - Confirmation et traitement
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠════════════════════════════════════ SUPPRIMER UNE COMPAGNIE ══════════════════════════════════════╣");

    setColor(11);
    gotoxy(CENTER_X - 5, 15);
    printf("Confirmation de Suppression");

    // Afficher les détails de la compagnie
    setColor(15);
    gotoxy(CENTER_X - 15, 17);
    printf("Vous allez supprimer la compagnie : %s", nomCompagnie);

    FILE *fichier = fopen(FILENAME, "rb");
    if (fichier == NULL) {
        setColor(12);
        const char* error_msg = "✗ Erreur d'accès au fichier!";
        gotoxy(CENTER_X - (strlen(error_msg) / 2)+5, 20);
        printf("%s", error_msg);
        Sleep(2000);
        return;
    }

    // Animation de recherche
    drawWaitingAnimation(CENTER_X - 10, 20, "Recherche en cours");

    // Lecture et stockage temporaire
    Compagnie *compagnies = NULL;
    int nbCompagnies = 0;
    Compagnie temp;

    while (fread(&temp, sizeof(Compagnie), 1, fichier)) {
        nbCompagnies++;
        compagnies = realloc(compagnies, nbCompagnies * sizeof(Compagnie));
        compagnies[nbCompagnies - 1] = temp;
    }
    fclose(fichier);

    // Animation de suppression
    drawWaitingAnimation(CENTER_X - 10, 20, "Suppression en cours");

    // Réécriture sans la compagnie à supprimer
    fichier = fopen(FILENAME, "wb");
    if (fichier != NULL) {
        int trouve = 0;
        for (int i = 0; i < nbCompagnies; i++) {
            if (strcmp(compagnies[i].nom, nomCompagnie) != 0) {
                fwrite(&compagnies[i], sizeof(Compagnie), 1, fichier);
            } else {
                trouve = 1;
            }
        }
        fclose(fichier);

        // Effacer la ligne de l'animation
        gotoxy(CENTER_X - 25, 20);
        printf("                                                  ");

        if (trouve) {
            setColor(10);
            const char* success_msg = "✓ Compagnie supprimée avec succès!";
            gotoxy(CENTER_X - (strlen(success_msg) / 2)+5, 20);
            printf("%s", success_msg);
        } else {
            setColor(12);
            const char* error_msg = "✗ Compagnie non trouvée!";
            gotoxy(CENTER_X - (strlen(error_msg) / 2)+5, 20);
            printf("%s", error_msg);
        }
    }

    free(compagnies);
    Sleep(2000);
}

void afficherCompagnies() {
    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠══════════════════════════════════════ LISTE DES COMPAGNIES ═══════════════════════════════════════╣");
            
    FILE *file = fopen("compagnies.bin", "rb");
    if (file != NULL) {
        Compagnie comp;
        int totalCompagnies = 0;
        int currentPage = 1;
        int itemsPerPage = 4;
        
        // Compter le nombre total de compagnies
        while (fread(&comp, sizeof(Compagnie), 1, file) == 1) {
            totalCompagnies++;
        }
        rewind(file);

        int totalPages = (totalCompagnies + itemsPerPage - 1) / itemsPerPage;
        
        while(1) {
            system("cls");
            drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
            drawLogo();
            drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

            setColor(9);
            gotoxy(FRAME_LEFT, 12);
            printf("╠════════════════════════════════════ LISTE DES COMPAGNIES ═════════════════════════════════════════╣");
                   
            // Numéro de page
            setColor(13);
            gotoxy(FRAME_RIGHT -5, 30);
            printf("Page %d/%d", currentPage, totalPages);

            int startIndex = (currentPage - 1) * itemsPerPage;
            int y = 14;
            int count = 0;

            fseek(file, startIndex * sizeof(Compagnie), SEEK_SET);

            while (count < itemsPerPage && fread(&comp, sizeof(Compagnie), 1, file) == 1) {
                // Titre et informations principales
                setColor(11);
                gotoxy(CENTER_X - 35, y);
                printf("► %s", comp.nom);
                
                setColor(15);
                gotoxy(CENTER_X - 30, y + 1);
                printf("Pays: %-15s | Avions: %d", comp.pays, comp.nb_avions);
                
                // Informations du contrat
                setColor(8);
                gotoxy(CENTER_X - 30, y + 2);
                printf("Contrat %s: du %s au %s", 
                       comp.contrat.id_contrat, 
                       comp.contrat.date_debut, 
                       comp.contrat.date_fin);
                gotoxy(CENTER_X - 30, y + 3);
                printf("Montant: %.2f MAD", comp.contrat.montant);

                y += 4;
                count++;
            }

            // Total des compagnies
            setColor(11);
            gotoxy(CENTER_X - 35, 28);
            printf("Total: %d compagnie%s", totalCompagnies, totalCompagnies > 1 ? "s" : "");

            // Instructions de navigation
            setColor(13);
            const char* instructions = "←→: Pages         ENTRÉE: Détails         ÉCHAP: Retour\n\n";
            gotoxy(CENTER_X - (strlen(instructions) / 2), 30);
            printf("%s", instructions);
            
            char key = _getch();
            if (key == 27) {
                break;
            } else if (key == 75 && currentPage > 1) {
                currentPage--;
            } else if (key == 77 && currentPage < totalPages) {
                currentPage++;
            }
        }
        fclose(file);
    } else {
        setColor(12);
        const char* error_msg = "✗ Aucune compagnie enregistrée";
        gotoxy(CENTER_X - (strlen(error_msg) / 2), 20);
        printf("%s", error_msg);
        Sleep(2000);
    }
}

void gestionCompagnies() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 4;
    char* options[] = {
        "Ajouter une Compagnie",
        "Supprimer une Compagnie",
        "Afficher les Compagnies",
        "Retour au Menu Principal"
    };

    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 16;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠════════════════════════════════════ GESTION DES COMPAGNIES ═══════════════════════════════════════╣");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→  %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");

            switch(currentChoice) {
                case 0:
                    ajouterCompagnie();
                    break;
                case 1:
                    supprimerCompagnie();
                    break;
                case 2:
                    afficherCompagnies();
                    break;
                case 3:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////





//gestion des employes
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void ajouterEmploye() {
    Employe employe;
    const char *nom_fichier = "employe.bin";
    
    system("cls");
    drawFrame(20, 1, 100, 31);
    drawLogo();
    drawSeparator(20, 100, 10);

    setColor(9);
    gotoxy(20, 12);
    printf("╠═════════════════════════════════════ AJOUTER UN EMPLOYÉ ══════════════════════════════════════════╣");
            
    // Utilisation de drawInputField pour chaque champ
    drawInputField("Nom", 13, employe.nom, 0);
    Sleep(100);

    drawInputField("Prénom", 16, employe.prenom, 0);
    Sleep(100);

    drawInputField("Poste", 19, employe.poste, 0);
    Sleep(100);

    drawInputField("Ville", 22, employe.ville, 0);
    Sleep(100);

    drawInputField("Email", 25, employe.email, 0);
    Sleep(100);

    drawInputField("Téléphone", 28, employe.telephone, 0);
    Sleep(100);

    do {
        system("cls");
        drawFrame(20, 1, 100, 31);
        drawLogo();
        drawSeparator(20, 100, 10);

        setColor(9);
        gotoxy(20, 12);
        printf("╠═════════════════════════════════════ AJOUTER UN EMPLOYÉ ══════════════════════════════════════════╣");
                
        drawInputField("Identifiant", 14, employe.identifiant, 0);
        
        if (identifiantExiste(nom_fichier, employe.identifiant)) {
            setColor(12);
            gotoxy(55, 18);
            printf("❌ Identifiant déjà existant!");
            Sleep(1500);
        }
    } while (identifiantExiste(nom_fichier, employe.identifiant));

    drawInputField("Mot de passe", 17, employe.mot_de_passe, 1);

    // Animation de sauvegarde
    drawWaitingAnimation(45, 25, "Enregistrement en cours");

    FILE *fichier = fopen(nom_fichier, "ab");
    if (fichier == NULL) {
        setColor(12);
        gotoxy(55, 25);
        printf("❌ Erreur lors de l'enregistrement!");
        Sleep(2000);
        return;
    }

    fwrite(&employe, sizeof(Employe), 1, fichier);
    fclose(fichier);

    setColor(10);
    gotoxy(55, 25);
    printf("✅ Employé ajouté avec succès!");
    Sleep(2000);
}

void supprimerEmploye() {
    char identifiant[30];
    
    system("cls");
    drawFrame(20, 1, 100, 31);
    drawLogo();
    drawSeparator(20, 100, 10);

    setColor(9);
    gotoxy(20, 12);
    printf("╠════════════════════════════════════ SUPPRIMER UN EMPLOYÉ ═════════════════════════════════════════╣");
            
    drawInputField("Identifiant  ", 14, identifiant, 0);

    // Animation de recherche
    drawWaitingAnimation(55, 20, "Recherche de l'employé");

    FILE *fichier = fopen("employe.bin", "rb");
    if (fichier == NULL) {
        setColor(12);
        gotoxy(55, 20);
        printf("❌ Aucun employé enregistré!");
        Sleep(2000);
        return;
    }

    FILE *tempFile = fopen("temp.bin", "wb");
    if (tempFile == NULL) {
        setColor(12);
        gotoxy(55, 20);
        printf("❌ Erreur système!");
        fclose(fichier);
        Sleep(2000);
        return;
    }

    Employe employe;
    int trouve = 0;

    while (fread(&employe, sizeof(Employe), 1, fichier)) {
        if (strcmp(employe.identifiant, identifiant) != 0) {
            fwrite(&employe, sizeof(Employe), 1, tempFile);
        } else {
            trouve = 1;
        }
    }

    fclose(fichier);
    fclose(tempFile);

    if (!trouve) {
        setColor(12);
        gotoxy(55, 20);
        printf("❌ Employé non trouvé!");
        remove("temp.bin");
        Sleep(2000);
        return;
    }

    // Animation de suppression
    drawWaitingAnimation(45, 20, "Suppression en cours");

    remove("employe.bin");
    rename("temp.bin", "employe.bin");

    setColor(10);
    gotoxy(55, 20);
    printf("✅ Employé supprimé avec succès!");
    Sleep(2000);
}
void afficherEmployes() {
    int currentPage = 0;
    int itemsPerPage = 2;
    
    while(1) {
        system("cls");
        drawFrame(20, 1, 100, 31);
        drawLogo();
        drawSeparator(20, 100, 10);

        setColor(9);
        gotoxy(20, 12);
        printf("╠══════════════════════════════════════ LISTE DES EMPLOYÉS ═════════════════════════════════════════╣");
                

        FILE *fichier = fopen("employe.bin", "rb");
        if(fichier == NULL) {
            setColor(12);
            gotoxy(45, 14);
            printf("❌ Aucun employé enregistré!");
            Sleep(2000);
            return;
        }

        // Calculer le nombre total d'employés
        fseek(fichier, 0, SEEK_END);
        int totalEmployes = ftell(fichier) / sizeof(Employe);
        int totalPages = (totalEmployes + itemsPerPage - 1) / itemsPerPage;
        rewind(fichier);

        // Se positionner à la page courante
        Employe employe;
        int count = 0;
        while(count < currentPage * itemsPerPage && fread(&employe, sizeof(Employe), 1, fichier)) {
            count++;
        }

        // Afficher les employés de la page courante
        count = 0;
        while(count < itemsPerPage && fread(&employe, sizeof(Employe), 1, fichier)) {
            int baseY = 14 + (count * 7);
            
            setColor(11);
            gotoxy(30, baseY);
            printf("► Employé #%s", employe.identifiant);

            setColor(10);
            gotoxy(30, baseY + 1);
            printf("  Nom complet: ");
            setColor(15);
            printf("%s %s", employe.nom, employe.prenom);

            setColor(14);
            gotoxy(30, baseY + 2);
            printf("  Poste: ");
            setColor(15);
            printf("%s", employe.poste);

            setColor(13);
            gotoxy(30, baseY + 3);
            printf("  Ville: ");
            setColor(15);
            printf("%s", employe.ville);

            setColor(11);
            gotoxy(30, baseY + 4);
            printf("  Contact: ");
            setColor(15);
            printf("Email: %s - Tél: %s", employe.email, employe.telephone);

            setColor(8);
            gotoxy(30, baseY + 5);
            printf("------------------------------------------------------------------------------------");
            count++;
        }

        fclose(fichier);

        // Navigation
        setColor(13);
        gotoxy(30, 30);
        printf("◄ Page précédente");
        gotoxy(75, 30);
        printf("Page suivante ►");
        
        setColor(11);
        gotoxy(55, 30);
        printf("Page %d/%d", currentPage + 1, totalPages);

        // Gestion des touches
        char key = _getch();
        if(key == 27) break; // Échap
        if(key == 75 && currentPage > 0) currentPage--; // Gauche
        if(key == 77 && currentPage < totalPages - 1) currentPage++; // Droite
    }
}

void gestionEmploye() {
    const int NUM_OPTIONS = 4;
    char* options[] = {
        "Ajouter un employé",
        "Supprimer un employé",
        "Afficher les employés",
        "Retour au menu principal"
    };

    int currentChoice = 0;

    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 14;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠════════════════════════════════════ GESTION DES EMPLOYÉS ═════════════════════════════════════════╣");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13); // Mauve pour la sélection
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→  %s", options[i]);
            } else {
                setColor(15); // Blanc pour les autres options
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation (centrées)
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");

            switch(currentChoice) {
                case 0:
                    ajouterEmploye();
                    break;
                case 1:
                    supprimerEmploye();
                    break;
                case 2:
                    afficherEmployes();
                    break;
                case 3:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////










//gestions des ofrres
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void afficherOffres() {
    FILE *file = fopen("offres.bin", "rb");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier des offres.\n");
        return;
    }

    OffreEmploi offre;
    int count = 0;
    int currentPage = 0;
    int itemsPerPage = 3;
    
    fseek(file, 0, SEEK_END);
    int totalOffres = ftell(file) / sizeof(OffreEmploi);
    int totalPages = (totalOffres + itemsPerPage - 1) / itemsPerPage;
    rewind(file);

    while(1) {
        system("cls");
        drawFrame(20, 1, 100, 31);
        drawLogo();
        drawSeparator(20, 100, 10);

        setColor(9);
        gotoxy(20, 12);
        printf("╠═════════════════════════════════ LISTE DES OFFRES D'EMPLOI ═══════════════════════════════════╣");

        fseek(file, currentPage * itemsPerPage * sizeof(OffreEmploi), SEEK_SET);
        count = 0;

        while (count < itemsPerPage && fread(&offre, sizeof(OffreEmploi), 1, file)) {
            // ID et Titre
            gotoxy(30, 14 + (count * 4));
            setColor(11); // Cyan clair pour "ID"
            printf("► ID: ");
            setColor(15); // Blanc pour la valeur
            printf("%s - ", offre.id);
            setColor(13); // Magenta clair pour le titre
            printf("%s", offre.titre);

            // Description
            gotoxy(30, 15 + (count * 4));
            setColor(14); // Jaune pour "Description"
            printf("  Description: ");
            setColor(7); // Gris clair pour le texte
            printf("%s", offre.description);

            // Localisation, Type et Salaire
            gotoxy(30, 16 + (count * 4));
            setColor(10); // Vert clair pour "Localisation"
            printf("  Localisation: ");
            setColor(15); // Blanc pour la valeur
            printf("%s - ", offre.localisation);
            
            setColor(12); // Rouge clair pour "Type"
            printf("Type: ");
            setColor(15); // Blanc pour la valeur
            printf("%s - ", offre.type_contrat);
            
            setColor(11); // Cyan clair pour "Salaire"
            printf("Salaire: ");
            setColor(15); // Blanc pour la valeur
            printf("%d DH", offre.salaire);

            // Ligne de séparation
            gotoxy(30, 17 + (count * 4));
            setColor(8); // Gris foncé pour la ligne
            printf("------------------------------------------------------------------------------------");
            count++;
        }

        // Navigation
        setColor(13); // Magenta clair pour la navigation
        gotoxy(30, 30);
        printf("◄ Page précédente");
        gotoxy(75, 30);
        printf("Page suivante ►");
        
        setColor(11); // Cyan clair pour le numéro de page
        gotoxy(55, 30);
        printf("Page %d/%d", currentPage + 1, totalPages);
        
        // Navigation
        char key = _getch();
        if (key == 27) { // Échap
            break;
        }
        else if (key == 75 && currentPage > 0) { // Gauche
            currentPage--;
        }
        else if (key == 77 && currentPage < totalPages - 1) { // Droite
            currentPage++;
        }
    }

    fclose(file);
}

void ajouterOffre() {
    OffreEmploi newOffre;
    char salaire_str[20];  // Pour stocker le salaire temporairement
    FILE *file = fopen("offres.bin", "ab");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier des offres.\n");
        return;
    }

    // Définir l'entreprise par défaut
    strcpy(newOffre.entreprise, "AMANLINES");

    system("cls");
    drawFrame(20, 1, 100, 31);
    drawLogo();
    drawSeparator(20, 100, 10);

    setColor(9);
    gotoxy(20, 12);
    printf("╠═════════════════════════════════ AJOUTER UNE OFFRE D'EMPLOI ══════════════════════════════════════╣");

    drawInputField("ID de l'offre", 13, newOffre.id, 0);
    Sleep(100);  // Petit délai pour une meilleure expérience utilisateur
    
    drawInputField("Titre", 16, newOffre.titre, 0);
    Sleep(100);
    
    drawInputField("Description", 19, newOffre.description, 0);
    Sleep(100);
    
    drawInputField("Localisation", 22, newOffre.localisation, 0);
    Sleep(100);
    
    drawInputField("Type de contrat", 25, newOffre.type_contrat, 0);
    Sleep(100);
    
    drawInputField("Salaire", 28, salaire_str, 0);
    newOffre.salaire = atoi(salaire_str);  // Conversion de la chaîne en entier

    setColor(10);
    gotoxy(55, 31);
    printf("✅ Offre ajoutée avec succès!");
    
    fwrite(&newOffre, sizeof(OffreEmploi), 1, file);
    fclose(file);
    Sleep(5000);
}




void supprimerOffre() {
    char id[20];
    system("cls");
    drawFrame(20, 1, 100, 31);
    drawLogo();
    drawSeparator(20, 100, 10);

    setColor(9);
    gotoxy(20, 12);
    printf("╠═════════════════════════════════ SUPPRIMER UNE OFFRE D'EMPLOI ════════════════════════════════════╣");

    drawInputField("ID de l'offre", 14, id, 0);

    // Vérifier d'abord si l'offre existe
    FILE *check_file = fopen("offres.bin", "rb");
    if (check_file == NULL) {
        setColor(12);
        gotoxy(55, 25);
        printf("❌ Erreur lors de l'ouverture des fichiers!");
        Sleep(2000);
        return;
    }

    OffreEmploi offre;
    int found = 0;
    while (fread(&offre, sizeof(OffreEmploi), 1, check_file)) {
        if (strcmp(offre.id, id) == 0) {
            found = 1;
            break;
        }
    }
    fclose(check_file);

    if (!found) {
        setColor(12);
        gotoxy(55, 25);
        printf("❌ Offre non trouvée.");
        Sleep(2000);
        return;
    }

    // Si l'offre existe, commencer la suppression avec animation
    FILE *file = fopen("offres.bin", "rb");
    FILE *temp = fopen("temp.bin", "wb");
    if (file == NULL || temp == NULL) {
        setColor(12);
        gotoxy(55, 25);
        printf("❌ Erreur lors de l'ouverture des fichiers!");
        Sleep(2000);
        return;
    }

    // Animation de la ligne d'attente
    int animationStep = 0;
    for(int i = 0; i < 12; i++) {  // 12 étapes d'animation
        drawWaitingAnimation(50, 25, animationStep);
        Sleep(200);  // Délai entre chaque point
        animationStep = (animationStep + 1) % 4;  // 0 à 3 points
    }

    // Effectuer la suppression réelle
    rewind(file);
    while (fread(&offre, sizeof(OffreEmploi), 1, file)) {
        if (strcmp(offre.id, id) != 0) {
            fwrite(&offre, sizeof(OffreEmploi), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);
    remove("offres.bin");
    rename("temp.bin", "offres.bin");

    // Effacer la ligne d'attente
    gotoxy(50, 25);
    printf("                           ");  // Effacer la ligne d'animation

    // Message de succès
    setColor(10);
    gotoxy(50, 25);
    printf("✅ Offre supprimée avec succès!");
    Sleep(2000);
}

void consulterCandidatures() {
    char offre_id[20];
    int found = 0;

    // Demander l'ID de l'offre
    system("cls");
    drawFrame(20, 1, 100, 31);
    drawLogo();
    drawSeparator(20, 100, 10);

    setColor(9);
    gotoxy(20, 12);
    printf("╠════════════════════════════════ CONSULTATION DES CANDIDATURES ════════════════════════════════════╣");

    drawInputField("ID de l'offre", 13, offre_id, 0);

    // Vérifier si l'offre existe
    FILE *offres_file = fopen("offres.bin", "rb");
    if (offres_file == NULL) {
        setColor(12);
        gotoxy(55, 31);
        printf("❌ Erreur lors de l'ouverture du fichier des offres!");
        Sleep(2000);
        return;
    }

    OffreEmploi offre;
    while (fread(&offre, sizeof(OffreEmploi), 1, offres_file)) {
        if (strcmp(offre.id, offre_id) == 0) {
            found = 1;
            break;
        }
    }
    fclose(offres_file);

    if (!found) {
        setColor(12);
        gotoxy(55, 20);
        printf("❌ Offre non trouvée!");
        Sleep(2000);
        return;
    }

    // Ouvrir le fichier des candidatures
    FILE *file = fopen("candidatures.bin", "rb");
    if (file == NULL) {
        setColor(12);
        gotoxy(55, 20);
        printf("❌ Erreur lors de l'ouverture du fichier des candidatures!");
        Sleep(2000);
        return;
    }

    Candidature candidature;
    int count = 0;
    int currentPage = 0;
    int itemsPerPage = 2;
    
    // Compter le nombre de candidatures pour cette offre
    int totalCandidatures = 0;
    while (fread(&candidature, sizeof(Candidature), 1, file)) {
        if (strcmp(candidature.offre_id, offre_id) == 0) {
            totalCandidatures++;
        }
    }
    
    if (totalCandidatures == 0) {
        setColor(14);
        gotoxy(45, 31);
        printf("⚠ Aucune candidature pour cette offre!");
        Sleep(2000);
        fclose(file);
        return;
    }

    int totalPages = (totalCandidatures + itemsPerPage - 1) / itemsPerPage;
    rewind(file);
while(1) {
        system("cls");
        drawFrame(20, 1, 100, 31);
        drawLogo();
        drawSeparator(20, 100, 10);

        setColor(9);
        gotoxy(20, 12);
        printf("╠════════════════════════════ CANDIDATURES POUR L'OFFRE %s ═══════════════════════════════════╣", offre_id);

        // Afficher les détails de l'offre
        setColor(15);
        gotoxy(30, 13);
        printf("Titre: %s - Type: %s - Localisation: %s", offre.titre, offre.type_contrat, offre.localisation);
        
        count = 0;
        int displayCount = 0;
        rewind(file);

        while (fread(&candidature, sizeof(Candidature), 1, file)) {
            if (strcmp(candidature.offre_id, offre_id) == 0) {
                if (count >= currentPage * itemsPerPage && displayCount < itemsPerPage) {
                    int baseY = 15 + (displayCount * 8); // Augmenté l'espacement pour ajouter la ligne du CV

                    // En-tête de la candidature
                    gotoxy(30, baseY);
                    setColor(11);
                    printf("► Candidat #%d", count + 1);
                    setColor(12);
                    printf(" - Statut: ");
                    setColor(candidature.statut[0] == 'A' ? 10 : (candidature.statut[0] == 'R' ? 4 : 14));
                    printf("%s", candidature.statut);

                    // Informations personnelles
                    gotoxy(30, baseY + 1);
                    setColor(10);
                    printf("  Nom complet: ");
                    setColor(13);
                    printf("%s %s", candidature.nom, candidature.prenom);
                    
                    // Contact
                    gotoxy(30, baseY + 2);
                    setColor(14);
                    printf("  Contact: ");
                    setColor(7);
                    printf("Email: %s - Tél: %s", candidature.email, candidature.telephone);

                    // Formation
                    gotoxy(30, baseY + 3);
                    setColor(11);
                    printf("  Formation: ");
                    setColor(15);
                    printf("%s en %s", candidature.niveau_etude, candidature.specialite);

                    // Compétences et Langues
                    gotoxy(30, baseY + 4);
                    setColor(13);
                    printf("  Compétences: ");
                    setColor(7);
                    printf("%s", candidature.competences);
                    gotoxy(30, baseY + 5);
                    setColor(12);
                    printf("  Langues: ");
                    setColor(15);
                    printf("%s", candidature.langues);

                    // CV Link (nouvelle section)
                    gotoxy(30, baseY + 6);
                    setColor(10); // Vert clair pour le symbole
                    printf("  📄 "); // Symbole de document
                    setColor(11); // Cyan clair pour "CV"
                    printf("CV: ");
                    setColor(15); // Blanc pour le chemin
                    printf("%s", candidature.cv_path);

                    // Ligne de séparation
                    gotoxy(30, baseY + 7);
                    setColor(8);
                    printf("------------------------------------------------------------------------------------");
                    displayCount++;
                }
                count++;
            }
        }

        // Navigation
        setColor(13);
        gotoxy(30, 30);
        printf("◄ Page précédente");
        gotoxy(75, 30);
        printf("Page suivante ►");
        
        setColor(11);
        gotoxy(55, 30);
        printf("Page %d/%d", currentPage + 1, totalPages);
        
        // Navigation et option pour ouvrir le CV
        char key = _getch();
        if (key == 27) { // Échap
            break;
        }
        else if (key == 75 && currentPage > 0) { // Gauche
            currentPage--;
        }
        else if (key == 77 && currentPage < totalPages - 1) { // Droite
            currentPage++;
        }
    }

    fclose(file);
}

void gestionOffres() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 5;
    char* options[] = {
        "Afficher les offres d'emploi",
        "Ajouter une offre",
        "Supprimer une offre",
        "Consulter les candidatures",
        "Retour"
    };

    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 16;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═════════════════════════════════ GESTION DES OFFRES D'EMPLOI ═════════════════════════════════╣");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→  %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");

            switch(currentChoice) {
                case 0:
                    afficherOffres();
                    break;
                case 1:
                    ajouterOffre();
                    break;
                case 2:
                    supprimerOffre();
                    break;
                
                case 3:
                    consulterCandidatures();
                    break;
                case 5:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}

//Menu d'administrateur
void MenuAdministrateur() {
    
    const int NUM_OPTIONS = 7;
    char* options[] = {
        "Gestion des Lignes",
        "Gestion des réservations",
        "Consulter les Statistiques",
        "Gestion des Employes",
        "Gestion d'Offre d'emploi",
        "Nos Collaborateurs",
        "Deconnexion"
    };

    OffreEmploi offres[100];
    int nbOffres = 0;
    int currentChoice = 0;

    // Constantes pour le centrage
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    const int MENU_START_Y = 14;

    while(1) {
        // Affichage initial
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        // Titre
        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠════════════════════════════════════ MENU ADMINISTRATEUR ══════════════════════════════════════════╣");

        // Afficher les options (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2) + 8;

            if(i == currentChoice) {
                setColor(13); // Mauve pour la sélection
                gotoxy(startX - 2, MENU_START_Y + i * 2);
                printf("→  %s", options[i]);
            } else {
                setColor(15); // Blanc pour les autres options
                gotoxy(startX, MENU_START_Y + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation (centrées)
        setColor(13);
        const char* instructions = "↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour";
        gotoxy(CENTER_X - (strlen(instructions) / 2) + 7, 30);
        printf("%s", instructions);

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            system("cls");

            switch(currentChoice) {
                case 0:
                    gestionlignes();
                    break;
                case 1:
                    GestionReservations();
                    break;
                case 2:
                    ConsulterStatistique();
                    break;
                case 3:
                    gestionEmploye();
                    break;
                case 4:
                    gestionOffres();
                    break;
                case 5:
                   gestionCompagnies();  
                    break;
                
                case 6:
                    return;
            }
            system("cls");
        }
        else if(key == 27) { // Échap
            return;
        }
        system("cls");
    }
}


















//// partie employee


// Fonction pour vérifier la connexion d'un employé
int loginEmploye() {
    Employe employe;
    FILE *file = fopen("employe.bin", "rb");

    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }

    // Configuration initiale
    system("cls");
    
    // Dessiner le cadre principal
    drawFrame(20, 1, 100, 30);
    
    // Dessiner le logo
    drawLogo();
    
    // Dessiner le séparateur
    drawSeparator(20, 100, 10);

    // Titre
    setColor(9);
    gotoxy(20, 13);
    printf("╠═══════════════════════════════════════ CONNEXION EMPLOYÉ ══════════════════════════════════════════╣");

    // Saisie de l'identifiant
    drawInputField("Identifiant    ", 16, employe.identifiant, 0);
    
    // Saisie du mot de passe
    drawInputField("Password   ", 20, employe.mot_de_passe, 1);

    Employe tempEmploye;
    int loginSuccess = 0;
    while (fread(&tempEmploye, sizeof(Employe), 1, file) == 1) {
        if (strcmp(tempEmploye.identifiant, employe.identifiant) == 0 && 
            strcmp(tempEmploye.mot_de_passe, employe.mot_de_passe) == 0) {
            setColor(10);
            gotoxy(45, 24);
            printf("Connexion réussie ! Bienvenue, %s", tempEmploye.identifiant);
            loginSuccess = 1;
            break;
        }
    }

    if (!loginSuccess) {
        setColor(12);
        gotoxy(45, 24);
        printf("Identifiant ou mot de passe employé incorrect");
        Sleep(2000);
    } else {
        Sleep(2000);
    }

    fclose(file);
    system("cls");
    return loginSuccess;
}
void ReserverVoyageHorsLineInterne(char *username) {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);

    // Configuration initiale
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═══════════════════════════════════ RÉSERVATION VOYAGE INTERNE ════════════════════════════════╣");

    FILE *fichierVoyages = Ouvrir_Fichier("voyageInterne.bin", "rb");
    FILE *fichierReservations = Ouvrir_Fichier("reservationsInterne.bin", "ab");
    if (fichierVoyages == NULL || fichierReservations == NULL) {
        setColor(12);
        gotoxy(CENTER_X - 20, 14);
        printf("✗ Erreur d'ouverture des fichiers!");
        Sleep(2000);
        return;
    }

    // Étape 1: Saisie des critères
    char depart[30], arrive[30];
    Date date;
    int nb_places;
    saisirCriteresRecherche(depart, arrive, &date, &nb_places);

    // Étape 2: Affichage des voyages disponibles
    int nb_voyages_disponibles = afficherVoyagesDisponiblesInternes(fichierVoyages, depart, arrive, date, nb_places);
    
    if (nb_voyages_disponibles == 0) {
        setColor(14);
        gotoxy(CENTER_X - 25, 28);
        printf("⚠ Aucun voyage disponible ne correspond à vos critères.");
        Sleep(2000);
        fclose(fichierVoyages);
        fclose(fichierReservations);
        return;
    }

    // Étape 3: Choix du voyage
    int choix_voyage = demanderChoixVoyage(nb_voyages_disponibles);
    if (choix_voyage <= 0) {
        fclose(fichierVoyages);
        fclose(fichierReservations);
        return;
    }

    // Étape 4: Recherche du voyage sélectionné
    rewind(fichierVoyages);
    VoyageInterne voyage;
    int index = 1;
    int voyage_trouve = 0;

    while (fread(&voyage, sizeof(VoyageInterne), 1, fichierVoyages)) {
        if (strcmp(voyage.aeroport_depart, depart) == 0 &&
            strcmp(voyage.aeroport_arrive, arrive) == 0 &&
            voyage.date_voyage.jour == date.jour &&
            voyage.date_voyage.mois == date.mois &&
            voyage.date_voyage.annee == date.annee &&
            voyage.nb_place >= nb_places) {
            
            if (index == choix_voyage) {
                voyage_trouve = 1;
                break;
            }
            index++;
        }
    }

    if (voyage_trouve) {
        // Étape 5: Affichage du récapitulatif
        system("cls");
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═══════════════════════════════════ CONFIRMATION RÉSERVATION ══════════════════════════════╣");

        float montant_total = voyage.prix * nb_places;

        // Afficher le récapitulatif dans un cadre
        setColor(11);
        gotoxy(CENTER_X - 25, 14);
        printf("┌─────────────────────────────────────────────┐");
        gotoxy(CENTER_X - 25, 15);
        printf("│           DÉTAILS DE LA RÉSERVATION         │");
        gotoxy(CENTER_X - 25, 16);
        printf("├─────────────────────────────────────────────┤");
        gotoxy(CENTER_X - 25, 17);
        printf("│ ID Vol: %-35s │", voyage.id);
        gotoxy(CENTER_X - 25, 18);
        printf("│ De: %-38s │", voyage.aeroport_depart);
        gotoxy(CENTER_X - 25, 19);
        printf("│ À: %-39s │", voyage.aeroport_arrive);
        gotoxy(CENTER_X - 25, 20);
        printf("│ Date: %02d/%02d/%04d                          │", 
               date.jour, date.mois, date.annee);
        gotoxy(CENTER_X - 25, 21);
        printf("│ Places: %-35d │", nb_places);
        gotoxy(CENTER_X - 25, 22);
        printf("│ Prix total: %.2f MAD                      │", montant_total);
        gotoxy(CENTER_X - 25, 23);
        printf("└─────────────────────────────────────────────┘");

        // Étape 6: Confirmation
        setColor(14);
        gotoxy(CENTER_X - 25, 25);
        printf("Confirmez-vous la réservation ? (O/N): ");
        char confirmation = _getch();

        if (confirmation == 'O' || confirmation == 'o') {
            // Étape 7: Mise à jour du nombre de places
            voyage.nb_place -= nb_places;

            // Étape 8: Enregistrement de la réservation
            enregistrerReservationInterne(username, voyage, nb_places, montant_total);

            // Étape 9: Mise à jour du fichier des voyages
            FILE *fichierTemp = Ouvrir_Fichier("temp.bin", "wb");
            if (fichierTemp == NULL) {
                setColor(12);
                gotoxy(CENTER_X - 25, 27);
                printf("✗ Erreur lors de la mise à jour!");
                Sleep(2000);
                fclose(fichierVoyages);
                fclose(fichierReservations);
                return;
            }

            rewind(fichierVoyages);
            VoyageInterne tempVoyage;
            while (fread(&tempVoyage, sizeof(VoyageInterne), 1, fichierVoyages)) {
                if (strcmp(tempVoyage.id, voyage.id) == 0) {
                    fwrite(&voyage, sizeof(VoyageInterne), 1, fichierTemp);
                } else {
                    fwrite(&tempVoyage, sizeof(VoyageInterne), 1, fichierTemp);
                }
            }

            fclose(fichierTemp);
            fclose(fichierVoyages);
            remove("voyagesInterne.bin");
            rename("temp.bin", "voyagesInterne.bin");
        } else {
            setColor(14);
            gotoxy(CENTER_X - 15, 27);
            printf("Réservation annulée.");
            Sleep(1500);
        }
    }

    fclose(fichierReservations);
}
// Fonction pour réserver un voyage hors ligne pour les voyages externes
void ReserverVoyageExterneHorsLine(char *username) {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);

    // Configuration initiale
    system("cls");
    drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
    drawLogo();
    drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

    setColor(9);
    gotoxy(FRAME_LEFT, 12);
    printf("╠═══════════════════════════════════ RÉSERVATION VOYAGE EXTERNE ════════════════════════════════╣");

    FILE *fichierVoyages = Ouvrir_Fichier("voyageExterne.bin", "rb");
    FILE *fichierReservations = Ouvrir_Fichier("reservationsExterne.bin", "ab");
    if (fichierVoyages == NULL || fichierReservations == NULL) {
        setColor(12);
        gotoxy(CENTER_X - 20, 14);
        printf("✗ Erreur d'ouverture des fichiers!");
        Sleep(2000);
        return;
    }

    // Étape 1: Saisie des critères
    char pays_depart[30], pays_arrive[30];
    char aeroport_depart[30], aeroport_arrive[30];
    Date date;
    int nb_places;

    saisirCriteresRechercheExterne(pays_depart, pays_arrive, aeroport_depart, aeroport_arrive, &date, &nb_places);

    // Étape 2: Affichage des voyages disponibles
    int nb_voyages_disponibles = afficherVoyagesDisponiblesExterne(fichierVoyages, pays_depart, pays_arrive, 
                                                                  aeroport_depart, aeroport_arrive, date, nb_places);
    
    if (nb_voyages_disponibles == 0) {
        setColor(14);
        gotoxy(CENTER_X - 25, 28);
        printf("⚠ Aucun voyage disponible ne correspond à vos critères.");
        Sleep(2000);
        fclose(fichierVoyages);
        fclose(fichierReservations);
        return;
    }

    // Étape 3: Choix du voyage
    int choix_voyage = demanderChoixVoyage(nb_voyages_disponibles);
    if (choix_voyage <= 0) {
        fclose(fichierVoyages);
        fclose(fichierReservations);
        return;
    }

    // Étape 4: Recherche du voyage sélectionné
    rewind(fichierVoyages);
    VoyageExterne voyage;
    int index = 1;
    int voyage_trouve = 0;

    while (fread(&voyage, sizeof(VoyageExterne), 1, fichierVoyages)) {
        if (strcmp(voyage.pays_depart, pays_depart) == 0 &&
            strcmp(voyage.pays_arrivee, pays_arrive) == 0 &&
            strcmp(voyage.aeroport_depart, aeroport_depart) == 0 &&
            strcmp(voyage.aeroport_arrive, aeroport_arrive) == 0 &&
            voyage.date_voyage.jour == date.jour &&
            voyage.date_voyage.mois == date.mois &&
            voyage.date_voyage.annee == date.annee &&
            voyage.nb_place >= nb_places) {
            
            if (index == choix_voyage) {
                voyage_trouve = 1;
                break;
            }
            index++;
        }
    }

    if (voyage_trouve) {
        // Étape 5: Affichage du récapitulatif et confirmation
        system("cls");
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═══════════════════════════════════ CONFIRMATION RÉSERVATION ══════════════════════════════╣");

        float montant_total = voyage.prix * nb_places;

        // Afficher le récapitulatif
        setColor(11);
        gotoxy(CENTER_X - 25, 14);
        printf("┌─────────────────────────────────────────────┐");
        gotoxy(CENTER_X - 25, 15);
        printf("│           DÉTAILS DE LA RÉSERVATION         │");
        gotoxy(CENTER_X - 25, 16);
        printf("├─────────────────────────────────────────────┤");
        gotoxy(CENTER_X - 25, 17);
        printf("│ ID Vol: %-35s │", voyage.id);
        gotoxy(CENTER_X - 25, 18);
        printf("│ Pays départ: %-30s │", voyage.pays_depart);
        gotoxy(CENTER_X - 25, 19);
        printf("│ Pays arrivée: %-29s │", voyage.pays_arrivee);
        gotoxy(CENTER_X - 25, 20);
        printf("│ Aéroport départ: %-26s │", voyage.aeroport_depart);
        gotoxy(CENTER_X - 25, 21);
        printf("│ Aéroport arrivée: %-25s │", voyage.aeroport_arrive);
        gotoxy(CENTER_X - 25, 22);
        printf("│ Date: %02d/%02d/%04d                          │", 
               date.jour, date.mois, date.annee);
        gotoxy(CENTER_X - 25, 23);
        printf("│ Places: %-35d │", nb_places);
        gotoxy(CENTER_X - 25, 24);
        printf("│ Prix total: %.2f MAD                      │", montant_total);
        gotoxy(CENTER_X - 25, 25);
        printf("└─────────────────────────────────────────────┘");

        // Confirmation
        setColor(14);
        gotoxy(CENTER_X - 25, 27);
        printf("Confirmez-vous la réservation ? (O/N): ");
        char confirmation = _getch();

        if (confirmation == 'O' || confirmation == 'o') {
            // Mise à jour du nombre de places
            voyage.nb_place -= nb_places;

            // Enregistrement de la réservation
            enregistrerReservationExterne(username, voyage, nb_places, montant_total);

            // Mise à jour du fichier des voyages
            FILE *fichierTemp = Ouvrir_Fichier("temp.bin", "wb");
            if (fichierTemp == NULL) {
                setColor(12);
                gotoxy(CENTER_X - 25, 29);
                printf("✗ Erreur lors de la mise à jour!");
                Sleep(2000);
                fclose(fichierVoyages);
                fclose(fichierReservations);
                return;
            }

            rewind(fichierVoyages);
            VoyageExterne tempVoyage;
            while (fread(&tempVoyage, sizeof(VoyageExterne), 1, fichierVoyages)) {
                if (strcmp(tempVoyage.id, voyage.id) == 0) {
                    fwrite(&voyage, sizeof(VoyageExterne), 1, fichierTemp);
                } else {
                    fwrite(&tempVoyage, sizeof(VoyageExterne), 1, fichierTemp);
                }
            }

            fclose(fichierTemp);
            fclose(fichierVoyages);
            remove("voyagesExterne.bin");
            rename("temp.bin", "voyagesExterne.bin");
        } else {
            setColor(14);
            gotoxy(CENTER_X - 15, 29);
            printf("Réservation annulée.");
            Sleep(1500);
        }
    }

    fclose(fichierReservations);
}

void ReserverVoyageHorsLine(char *username) {
    const int FRAME_LEFT = 20;
    const int FRAME_RIGHT = 100;
    const int FRAME_WIDTH = FRAME_RIGHT - FRAME_LEFT;
    const int CENTER_X = FRAME_LEFT + (FRAME_WIDTH / 2);
    
    int currentChoice = 0;
    const int NUM_OPTIONS = 3;
    char* options[] = {
        "Réserver un voyage interne",
        "Réserver un voyage externe",
        "Retour au menu principal"
    };

    while(1) {
        system("cls");
        drawFrame(FRAME_LEFT, 1, FRAME_RIGHT, 31);
        drawLogo();
        drawSeparator(FRAME_LEFT, FRAME_RIGHT, 10);

        setColor(9);
        gotoxy(FRAME_LEFT, 12);
        printf("╠═══════════════════════════════════ RÉSERVATION DE VOYAGE ══════════════════════════════════╣");

        // Afficher le nom d'utilisateur
        setColor(14);
        gotoxy(FRAME_LEFT + 2, 14);
        printf("👤 Utilisateur: %s", username);

        // Cadre du menu
        setColor(11);
        gotoxy(CENTER_X - 25, 16);
        printf("┌─────────────────────────────────────────────┐");
        gotoxy(CENTER_X - 25, 17);
        printf("│               MENU PRINCIPAL                │");
        gotoxy(CENTER_X - 25, 18);
        printf("└─────────────────────────────────────────────┘");

        // Affichage des options
        for(int i = 0; i < NUM_OPTIONS; i++) {
            int optionLength = strlen(options[i]);
            int startX = CENTER_X - (optionLength / 2);

            if(i == currentChoice) {
                setColor(13);
                gotoxy(startX - 2, 20 + i * 2);
                printf("→ %s", options[i]);
            } else {
                setColor(15);
                gotoxy(startX, 20 + i * 2);
                printf("%s", options[i]);
            }
        }

        // Instructions de navigation
        setColor(11);
        gotoxy(CENTER_X - 30, 28);
        printf("╔═══════════════════════════════════════════════════════════╗");
        gotoxy(CENTER_X - 30, 29);
        printf("║  ↑↓: Navigation    ENTRÉE: Sélectionner    ÉCHAP: Retour  ║");
        gotoxy(CENTER_X - 30, 30);
        printf("╚═══════════════════════════════════════════════════════════╝");

        // Gestion des touches
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Entrée
            switch(currentChoice) {
                case 0: {
                    // Animation de transition
                    drawWaitingAnimation(CENTER_X - 12, 26, "Chargement du module interne");
                    ReserverVoyageHorsLineInterne(username);
                    break;
                }
                case 1: {
                    // Animation de transition
                    drawWaitingAnimation(CENTER_X - 12, 26, "Chargement du module externe");
                    ReserverVoyageExterneHorsLine(username);
                    break;
                }
                case 2: {
                    setColor(14);
                    gotoxy(CENTER_X - 15, 26);
                    printf("Au revoir !");
                    Sleep(1500);
                    return;
                }
            }
        }
        else if(key == 27) { // Échap
            setColor(14);
            gotoxy(CENTER_X - 15, 26);
            printf("Au revoir !");
            Sleep(1500);
            return;
        }
    }
}

void MenueEmploye() {
    int currentChoice = 0;
    const int NUM_OPTIONS = 4;
    char* options[] = {
        "Lignes",
        "Réservations",
        "Voyages",
        "Déconnexion"
    };

    do {
        // Configuration initiale
        system("cls");

        // Dessiner le cadre principal
        drawFrame(20, 1, 100, 30);

        // Dessiner le logo
        drawLogo();

        // Dessiner le séparateur
        drawSeparator(20, 100, 10);

        // Titre du menu
        setColor(9);
        gotoxy(20, 12);
        printf("╠═══════════════════════════════════════ ESPACE EMPLOYE ════════════════════════════════════════╣");

        // Afficher les options du menu (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            drawMenuItem(options[i], 60, 15 + i * 2, i == currentChoice);
        }
        printf("\n");

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Touche Entrée
            system("cls");

            switch(currentChoice) {
                case 0:
                    gestionlignes();
                    break;
                case 1: {
                    char* username = signUp();
                    ReserverVoyageHorsLine(username);
                    break;
                }
                case 2:
                    ConsulterVoyages();
                    break;
                case 3:
                    system("cls");
                    drawFrame(20, 1, 100, 30);
                    drawLogo();
                    drawSeparator(20, 100, 10);

                    setColor(9);
                    gotoxy(20, 12);
                    printf("╠════════════════════════════════════ AU REVOIR ! ═══════════════════════════════════════╣");

                    setColor(COLOR_ACCENT);
                    gotoxy(50, 15);
                    printf("A bientôt cher(e) employé(e) !");

                    gotoxy(0, 31);
                    Sleep(2000);
                    return;
            }
            system("cls");
        }
    } while(1);
}




















void MenuePrincipal() {
    char* username;
    int loggedIn;
    int currentChoice = 0;
    const int NUM_OPTIONS = 4;
    char* options[] = {
        "Client",
        "Admin",
        "Staff",
        "Quitter"
    };

    // Configuration initiale
    system("cls");

    while(1) {
        // Dessiner le cadre principal
        drawFrame(20, 1, 100, 30);

        // Dessiner le logo
        drawLogo();

        // Dessiner le séparateur
        drawSeparator(20, 100, 10);

        // Titre du menu
        setColor(9);
        gotoxy(20, 12);
        printf("╠════════════════════════════════════ BIENVENUE DANS AMANLINES ═════════════════════════════════════╣");

        // Afficher les options du menu (centrées)
        for(int i = 0; i < NUM_OPTIONS; i++) {
            drawMenuItem(options[i], 60, 15 + i * 2, i == currentChoice);
        }
        printf("\n");

        // Navigation
        char key = _getch();
        if(key == 72) { // Flèche haut
            currentChoice = (currentChoice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
        }
        else if(key == 80) { // Flèche bas
            currentChoice = (currentChoice + 1) % NUM_OPTIONS;
        }
        else if(key == 13) { // Touche Entrée
            system("cls");

            switch(currentChoice) {
                case 0: { // Utilisateur
                    int userChoice = 0;
                    char* userOptions[] = {"S'inscrire", "Se connecter", "Retour"};
                    const int NUM_USER_OPTIONS = 3;

                    while(1) {
                        drawFrame(20, 1, 100, 30);
                        drawLogo();
                        drawSeparator(20, 100, 10);

                        setColor(9);
                        gotoxy(20, 12);
                        printf("╠═══════════════════════════════════════ ESPACE UTILISATEUR ════════════════════════════════════════╣");

                        for(int i = 0; i < NUM_USER_OPTIONS; i++) {
                            drawMenuItem(userOptions[i], 60, 15 + i * 2, i == userChoice);
                        }
                        printf("\n");

                        key = _getch();
                        if(key == 72) userChoice = (userChoice - 1 + NUM_USER_OPTIONS) % NUM_USER_OPTIONS;
                        else if(key == 80) userChoice = (userChoice + 1) % NUM_USER_OPTIONS;
                        else if(key == 13) {
                            system("cls");
                            switch(userChoice) {
                                case 0:
                                    username = signUp();
                                    if(username != NULL) menuPostConnexion(username);
                                    break;
                                case 1:
                                    username = login();
                                    if(username != NULL) menuPostConnexion(username);
                                    break;
                                case 2:
                                    goto main_menu;
                            }
                            break;
                        }
                        system("cls");
                    }
                    break;
                }

                case 1: { // Administrateur
                    int adminChoice = 0;
                    char* adminOptions[] = {"Se connecter", "Retour"};
                    const int NUM_ADMIN_OPTIONS = 2;

                    while(1) {
                        drawFrame(20, 1, 100, 30);
                        drawLogo();
                        drawSeparator(20, 100, 10);

                        setColor(9);
                        gotoxy(20, 12);
                        printf("╠══════════════════════════════════════ ESPACE ADMINISTRATEUR ══════════════════════════════════════╣");

                        for(int i = 0; i < NUM_ADMIN_OPTIONS; i++) {
                            drawMenuItem(adminOptions[i], 60, 15 + i * 2, i == adminChoice);
                        }
                        printf("\n");

                        key = _getch();
                        if(key == 72) adminChoice = (adminChoice - 1 + NUM_ADMIN_OPTIONS) % NUM_ADMIN_OPTIONS;
                        else if(key == 80) adminChoice = (adminChoice + 1) % NUM_ADMIN_OPTIONS;
                        else if(key == 13) {
                            system("cls");
                            switch(adminChoice) {
                                case 0:
                                    loggedIn = loginAdmin();
                                    if(loggedIn) MenuAdministrateur();
                                    break;
                                case 1:
                                    goto main_menu;
                            }
                            break;
                        }
                        system("cls");
                    }
                    break;
                }

                case 2: { // Employé
                    int empChoice = 0;
                    char* empOptions[] = {"Se connecter", "Retour"};
                    const int NUM_EMP_OPTIONS = 2;

                    while(1) {
                        drawFrame(20, 1, 100, 30);
                        drawLogo();
                        drawSeparator(20, 100, 10);

                        setColor(9);
                        gotoxy(20, 12);
                        printf("╠══════════════════════════════════════════ ESPACE EMPLOYE ═════════════════════════════════════════╣");


                        for(int i = 0; i < NUM_EMP_OPTIONS; i++) {
                            drawMenuItem(empOptions[i], 60, 15 + i * 2, i == empChoice);
                        }
                        printf("\n");

                        key = _getch();
                        if(key == 72) empChoice = (empChoice - 1 + NUM_EMP_OPTIONS) % NUM_EMP_OPTIONS;
                        else if(key == 80) empChoice = (empChoice + 1) % NUM_EMP_OPTIONS;
                        else if(key == 13) {
                            system("cls");
                            switch(empChoice) {
                                case 0:
                                    loggedIn = loginEmploye();
                                    if(loggedIn) MenueEmploye();
                                    break;
                                case 1:
                                    goto main_menu;
                            }
                            break;
                        }
                        system("cls");
                    }
                    break;
                }

                case 3: // Quitter
                    system("cls");
                    drawFrame(20, 1, 100, 30);
                    drawLogo();
                    drawSeparator(20, 100, 10);

                    setColor(9);
                    gotoxy(20, 12);
                    printf("╠════════════════════════════════════════ AU REVOIR ! ═══════════════════════════════════════╣");
                    setColor(COLOR_ACCENT);
                    gotoxy(50, 15);
                    printf("Merci d'avoir utilisé AMANLINES!");

                    gotoxy(0, 31);
                    Sleep(2000);
                    return;
            }

            main_menu:
            system("cls");
        }
        system("cls");
    }
}






int main() {
    system("cls");
    SetConsoleOutputCP(CP_UTF8); // UTF-8 pour le support des accents
    MenuePrincipal();
    return 0;
}
