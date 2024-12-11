import matplotlib.pyplot as plt
import sys
import warnings

# Masquer les warnings inutiles de matplotlib
warnings.filterwarnings("ignore")

def read_reservations_from_file(filename):
    """Lit les réservations depuis un fichier au format 'mois nombre_de_reservations'."""
    mois = []
    reservations = []
    try:
        with open(filename, 'r') as file:
            for line in file:
                parts = line.split()
                if len(parts) >= 2:
                    mois.append(int(parts[0]))  # Le mois (1 à 12)
                    reservations.append(int(parts[1]))  # Le nombre de réservations
    except FileNotFoundError:
        print(f"Erreur : Impossible de trouver le fichier {filename}.")
    return mois, reservations


def plot_annual_reservations(internal_file, external_file):
    """Trace les réservations annuelles internes et externes sur le même graphe."""
    
    # Lire les données des fichiers
    mois_interne, reservations_interne = read_reservations_from_file(internal_file)
    mois_externe, reservations_externe = read_reservations_from_file(external_file)
    
    if not mois_interne or not mois_externe:
        print("Les fichiers de données sont vides ou introuvables.")
        return
    
    # Initialiser les réservations à 0 pour les mois manquants
    reservations_interne_full = [0] * 12
    reservations_externe_full = [0] * 12

    for i, mois in enumerate(mois_interne):
        reservations_interne_full[mois - 1] = reservations_interne[i]
        
    for i, mois in enumerate(mois_externe):
        reservations_externe_full[mois - 1] = reservations_externe[i]

    # Indices des mois (1 à 12)
    mois_labels = ['Jan', 'Fév', 'Mar', 'Avr', 'Mai', 'Juin', 'Juil', 'Août', 'Sept', 'Oct', 'Nov', 'Déc']
    index = range(12)  # Indices des mois (0 à 11)
    
    # Largeur des barres
    bar_width = 0.35  # Largeur des barres
    
    # Tracer les barres côte à côte
    plt.bar([i - bar_width / 2 for i in index], reservations_interne_full, width=bar_width, color='#1f77b4', label='Réservations Internes')
    plt.bar([i + bar_width / 2 for i in index], reservations_externe_full, width=bar_width, color='#ff7f0e', label='Réservations Externes')
    
    # Ajouter les labels, le titre et la légende
    plt.xlabel('Mois de l\'année', fontsize=12, fontweight='bold')
    plt.ylabel('Nombre de Réservations', fontsize=12, fontweight='bold')
    plt.title('Réservations Annuelle', fontsize=14, fontweight='bold')
    
    # Rotation des labels de l'axe des X pour éviter le chevauchement
    plt.xticks(index, mois_labels, rotation=45, fontsize=10)
    plt.yticks(fontsize=10)
    
    # Afficher la grille
    plt.grid(axis='y', linestyle='--', alpha=0.6)
    
    # Afficher la légende
    plt.legend(fontsize=10, loc='upper left')
    
    # Améliorer la mise en page pour éviter que le texte ne soit coupé
    plt.tight_layout()
    
    # Personnaliser le titre de la fenêtre (propre et pro)
    fig = plt.gcf()  # Obtenir la figure actuelle
    fig.canvas.manager.set_window_title('Statistiques Annuelle des Réservations')
    
    # Afficher le graphique (block=True pour le garder ouvert jusqu'à fermeture manuelle)
    plt.show(block=True)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 reservationsA.py <fichier_interne> <fichier_externe>")
    else:
        internal_file = sys.argv[1]
        external_file = sys.argv[2]
        plot_annual_reservations(internal_file, external_file)
