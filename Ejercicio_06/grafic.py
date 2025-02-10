import os
import matplotlib.pyplot as plt
import csv
import numpy as np

# Use the correct file path
csv_file = "resultados.csv"

# Check if the file exists
if not os.path.exists(csv_file):
    print(f"Error: The file '{csv_file}' was not found in the current directory.")
    exit()

tamanos = []
nodos_exh = []
nodos_bb = []

# Read CSV file
with open(csv_file, 'r') as file:
    reader = csv.reader(file)
    next(reader)  # Skip header
    for row in reader:
        tamanos.append(int(row[0]))
        nodos_exh.append(int(row[1]))
        nodos_bb.append(int(row[2]))

# Plot normal scale
plt.figure()
plt.plot(tamanos, nodos_exh, label="Exhaustivo", color="red", marker='o')
plt.plot(tamanos, nodos_bb, label="Branch & Bound", color="blue", marker='s')
plt.xlabel("Tamaño del conjunto (n)")
plt.ylabel("Nodos visitados")
plt.title("Nodos visitados vs Tamaño del conjunto")
plt.legend()
plt.grid()
plt.savefig("nodos_visitados.png")  # Save plot in current directory
print("Plot saved as 'nodos_visitados.png'")

# Plot log scale
plt.figure()
plt.plot(tamanos, [np.log10(x) for x in nodos_exh], label="Log(Exhaustivo)", color="red", marker='o')
plt.plot(tamanos, [np.log10(x) for x in nodos_bb], label="Log(Branch & Bound)", color="blue", marker='s')
plt.xlabel("Tamaño del conjunto (n)")
plt.ylabel("Log(Nodos visitados)")
plt.title("Log(Nodos visitados) vs Tamaño del conjunto")
plt.legend()
plt.grid()
plt.savefig("log_nodos_visitados.png")
print("Log plot saved as 'log_nodos_visitados.png'")
