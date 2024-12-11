import matplotlib.pyplot as plt
import sys

def read_reservations_from_file(filename):
    """Lit les réservations depuis un fichier au format 'jour nombre_de_reservations'."""
    jours = []
    reservations = []
    try:
        with open(filename, 'r') as file:
            for line in file:
                parts = line.split()
                if len(parts) >= 2:
                    jours.append(int(parts[0]))  # Le jour (1 à 31)
                    reservations.append(int(parts[1]))  # Le nombre de réservations
    except FileNotFoundError:
        print(f"Erreur : Impossible de trouver le fichier {filename}.")
    except Exception as e:
        print(f"Erreur lors de la lecture du fichier {filename}: {str(e)}")
    return jours, reservations

def plot_monthly_reservations(internal_file, external_file):
    """Trace les réservations mensuelles internes et externes sur le même graphe."""
    
    # Lire les données des fichiers
    jours_interne, reservations_interne = read_reservations_from_file(internal_file)
    jours_externe, reservations_externe = read_reservations_from_file(external_file)
    
    if not jours_interne or not jours_externe:
        print("Les fichiers de données sont vides ou introuvables.")
        return
    
    # Initialiser les réservations à 0 pour les jours manquants
    reservations_interne_full = [0] * 31
    reservations_externe_full = [0] * 31

    for i, jour in enumerate(jours_interne):
        reservations_interne_full[jour - 1] = reservations_interne[i]
        
    for i, jour in enumerate(jours_externe):
        reservations_externe_full[jour - 1] = reservations_externe[i]

    # Indices des jours (1 à 31)
    jours_labels = list(range(1, 32))  # Jours de 1 à 31
    
    # Largeur des barres
    bar_width = 0.35  # Largeur des barres
    index = range(31)  # Indices des jours (0 à 30)
    
    # Tracer les barres côte à côte
    plt.bar([i - bar_width / 2 for i in index], reservations_interne_full, width=bar_width, color='#1f77b4', label='Réservations Internes')
    plt.bar([i + bar_width / 2 for i in index], reservations_externe_full, width=bar_width, color='#ff7f0e', label='Réservations Externes')
    
    # Ajouter les labels, le titre et la légende
    plt.xlabel('Jours du Mois', fontsize=12, fontweight='bold')
    plt.ylabel('Nombre de Réservations', fontsize=12, fontweight='bold')
    plt.title('Réservations Monsuelle', fontsize=14, fontweight='bold')
    
    # Afficher un jour sur 5 pour éviter une surcharge sur l'axe des X
    plt.xticks(index[::5], jours_labels[::5], rotation=45)  
    
    # Améliorer la lisibilité de l'axe y
    plt.yticks(fontsize=10)
    plt.xticks(fontsize=10)
    
    # Afficher la grille
    plt.grid(axis='y', linestyle='--', alpha=0.6)
    
    # Afficher la légende
    plt.legend(fontsize=10, loc='upper left')
    
    # Afficher le graphique
    plt.tight_layout()  # Assurer que tout le texte et les éléments sont bien visibles
    # Personnaliser le titre de la fenêtre (propre et pro)
    fig = plt.gcf()  # Obtenir la figure actuelle
    fig.canvas.manager.set_window_title('Statistiques Monsuelle des Réservations')
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 reservationsM.py <fichier_interne> <fichier_externe>")
    else:
        internal_file = sys.argv[1]
        external_file = sys.argv[2]
        plot_monthly_reservations(internal_file, external_file)
