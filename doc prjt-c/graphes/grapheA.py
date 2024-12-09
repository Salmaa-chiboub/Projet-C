import matplotlib.pyplot as plt
import sys

# Vérifier si un argument a été fourni
if len(sys.argv) < 2:
    print("Erreur : Aucun fichier n'a été fourni.")
    sys.exit(1)

# Récupérer le nom du fichier depuis les arguments
filename = sys.argv[1]

# Lire les données du fichier
mois = []
reservations = []

try:
    with open(filename, "r") as file:
        for line in file:
            mois_, nombre = map(int, line.split())
            mois.append(mois_)
            reservations.append(nombre)
except FileNotFoundError:
    print(f"Erreur : Le fichier {filename} est introuvable.")
    sys.exit(1)

# Tracer le graphe
plt.bar(mois, reservations, color='skyblue')
plt.xlabel("Mois de l'année")
plt.ylabel("Nombre de réservations")
plt.title("Réservations annuelles")
plt.xticks(mois, [
    "Jan", "Fév", "Mar", "Avr", "Mai", "Juin", 
    "Juil", "Août", "Sep", "Oct", "Nov", "Déc"
])  # Noms des mois
plt.grid(axis='y', linestyle='--', linewidth=0.7)
plt.show()