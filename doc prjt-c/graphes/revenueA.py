import matplotlib.pyplot as plt
import sys

def read_revenue_from_file(filename):
    """Lit les revenus depuis un fichier au format 'mois revenu'."""
    mois = []
    revenus = []
    try:
        with open(filename, 'r') as file:
            for line in file:
                parts = line.split()
                if len(parts) >= 2:
                    mois.append(int(parts[0]))  # Le mois (1 à 12)
                    revenus.append(float(parts[1]))  # Le revenu
    except FileNotFoundError:
        print(f"Erreur : Impossible de trouver le fichier {filename}.")
    except Exception as e:
        print(f"Erreur lors de la lecture du fichier {filename}: {str(e)}")
    return mois, revenus

def plot_annual_revenue(internal_file, external_file):
    """Trace les revenus annuels internes et externes sur le même graphe."""
    
    # Lire les données des fichiers
    mois_interne, revenus_interne = read_revenue_from_file(internal_file)
    mois_externe, revenus_externe = read_revenue_from_file(external_file)
    
    if not mois_interne or not mois_externe:
        print("Les fichiers de données sont vides ou introuvables.")
        return
    
    # Initialiser les revenus à 0 pour les mois manquants
    revenus_interne_full = [0] * 12
    revenus_externe_full = [0] * 12

    for i, mois in enumerate(mois_interne):
        revenus_interne_full[mois - 1] = revenus_interne[i]
        
    for i, mois in enumerate(mois_externe):
        revenus_externe_full[mois - 1] = revenus_externe[i]

    # Indices des mois (1 à 12)
    mois_labels = list(range(1, 13))  # Mois de 1 à 12
    
    # Largeur des barres
    bar_width = 0.35  # Largeur des barres
    index = range(12)  # Indices des mois (0 à 11)
    
    # Tracer les barres côte à côte
    plt.bar([i - bar_width / 2 for i in index], revenus_interne_full, width=bar_width, color='#1f77b4', label='Revenus Internes')
    plt.bar([i + bar_width / 2 for i in index], revenus_externe_full, width=bar_width, color='#ff7f0e', label='Revenus Externes')
    
    # Ajouter les labels, le titre et la légende
    plt.xlabel('Mois de l\'Année', fontsize=12, fontweight='bold')
    plt.ylabel('Revenu (MAD)', fontsize=12, fontweight='bold')
    plt.title('Revenus Annuelle', fontsize=14, fontweight='bold')
    
    # Afficher un mois sur 1 pour éviter une surcharge sur l'axe des X
    plt.xticks(index, mois_labels, rotation=45)
    
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
    fig.canvas.manager.set_window_title('Statistiques Annuelle des Revenues')
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 revenueA.py <fichier_interne> <fichier_externe>")
    else:
        internal_file = sys.argv[1]
        external_file = sys.argv[2]
        plot_annual_revenue(internal_file, external_file)
