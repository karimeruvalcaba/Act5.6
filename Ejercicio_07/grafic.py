import matplotlib.pyplot as plt
import csv
import numpy as np

# Leer datos del archivo CSV
tamanos = []
nodos_exh = []
nodos_bb = []

# Ruta del archivo CSV
csv_file = 'Ejercicio_07/resultados.csv'

with open(csv_file, 'r') as file:
    reader = csv.reader(file)
    next(reader)  # Saltar la cabecera
    for row in reader:
        tamanos.append(int(row[0]))  # Tamaño del conjunto (n)
        nodos_exh.append(int(row[1]))  # Nodos visitados en Exhaustivo
        nodos_bb.append(int(row[2]))  # Nodos visitados en Branch & Bound

# Graficar nodos visitados
plt.figure()
plt.plot(tamanos, nodos_exh, label="Exhaustivo", color="red", marker='o')
plt.plot(tamanos, nodos_bb, label="Branch & Bound", color="blue", marker='s')
plt.xlabel("Tamaño del conjunto (n)")
plt.ylabel("Nodos visitados")
plt.title("Nodos visitados vs Tamaño del conjunto")
plt.legend()
plt.grid()
plt.show()

# Graficar logaritmo de nodos visitados
plt.figure()
plt.plot(tamanos, [np.log10(x) for x in nodos_exh], label="Log(Exhaustivo)", color="red", marker='o')
plt.plot(tamanos, [np.log10(x) for x in nodos_bb], label="Log(Branch & Bound)", color="blue", marker='s')
plt.xlabel("Tamaño del conjunto (n)")
plt.ylabel("Log(Nodos visitados)")
plt.title("Log(Nodos visitados) vs Tamaño del conjunto")
plt.legend()
plt.grid()
plt.show()