/* AUTORES
 * Carlos Ernesto Soto Alarcón A01747990
 * Sergio Alfonso Casillas Santoyo A01424863
 * Karime Itzel Ruvalcaba Pérez A01656188
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <random>
#include <chrono>
using namespace std;

/**
 * Genera una lista de pesos aleatorios.
 * Complejidad Temporal: O(n), donde n es el número de pesos.
 */
vector<int> generateRandomWeights(int n, int minWeight, int maxWeight) {
    vector<int> weights(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(minWeight, maxWeight);
    for (int i = 0; i < n; i++) {
        weights[i] = dist(gen);
    }
    return weights;
}

/**
 * Calcula la diferencia entre dos conjuntos de pesos.
 * Complejidad Temporal: O(n), donde n es el número de pesos.
 */
int calculateDifference(const vector<int>& weights, const vector<int>& assignment) {
    int sum1 = 0, sum2 = 0;
    for (size_t i = 0; i < weights.size(); i++) {
        if (assignment[i] == 0) sum1 += weights[i];
        else sum2 += weights[i];
    }
    return abs(sum1 - sum2);
}

/**
 * Búsqueda exhaustiva para resolver el problema de partición.
 * Complejidad Temporal: O(2^n), donde n es el número de pesos.
 */
/**
 * Búsqueda exhaustiva para resolver el problema de partición.
 * Complejidad Temporal: O(2^n), donde n es el número de pesos.
 */
int exhaustiveSearch(const vector<int>& weights, bool useShortCircuit, vector<int>& bestAssignment) {
    int n = weights.size();
    int minDiff = numeric_limits<int>::max();
    vector<int> assignment(n, 0);

    // Generar todas las combinaciones posibles
    for (int mask = 0; mask < (1 << n); mask++) {
        for (int i = 0; i < n; i++) {
            assignment[i] = (mask >> i) & 1; // Asignar 0 o 1 a cada peso
        }
        int diff = calculateDifference(weights, assignment);
        if (diff < minDiff) {
            minDiff = diff;
            bestAssignment = assignment; // Guardar la asignación óptima
            if (useShortCircuit && minDiff == 0) break; // Short circuit
        }
    }
    return minDiff;
}

int main() {
    int n, minWeight, maxWeight;
    bool useShortCircuit;

    // Entrada del usuario
    cout << "Ingrese el número de pesos: ";
    cin >> n;
    cout << "Ingrese el valor mínimo de los pesos: ";
    cin >> minWeight;
    cout << "Ingrese el valor máximo de los pesos: ";
    cin >> maxWeight;
    cout << "¿Usar short circuit? (1: Sí, 0: No): ";
    cin >> useShortCircuit;

    // Generar pesos aleatorios
    vector<int> weights = generateRandomWeights(n, minWeight, maxWeight);

    // Mostrar los pesos generados
    cout << "Pesos generados: ";
    for (int weight : weights) {
        cout << weight << " ";
    }
    cout << endl;

    // Realizar la búsqueda exhaustiva
    vector<int> bestAssignment;
    auto start = chrono::high_resolution_clock::now();
    int result = exhaustiveSearch(weights, useShortCircuit, bestAssignment);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // Mostrar resultados
    cout << "Resultado (Búsqueda Exhaustiva): Diferencia mínima: " << result << endl;
    cout << "Asignación óptima: ";
    for (int a : bestAssignment) {
        cout << a << " ";
    }
    cout << endl;
    cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << endl;

    return 0;
}