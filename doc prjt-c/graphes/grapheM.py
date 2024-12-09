import matplotlib.pyplot as plt
import sys

# Vérifier si un argument a été fourni
if len(sys.argv) < 2:
    print("Erreur : Aucun fichier n'a été fourni.")
    sys.exit(1)

# Récupérer le nom du fichier depuis les arguments
filename = sys.argv[1]

# Lire les données du fichier
jours = []
reservations = []

try:
    with open(filename, "r") as file:
        for line in file:
            jour, nombre = map(int, line.split())
            jours.append(jour)
            reservations.append(nombre)
except FileNotFoundError:
    print(f"Erreur : Le fichier {filename} est introuvable.")
    sys.exit(1)

# Tracer le graphe
plt.bar(jours, reservations)
plt.xlabel("Jour du mois")
plt.ylabel("Nombre de réservations")
plt.title("Réservations mensuelles")
plt.show()