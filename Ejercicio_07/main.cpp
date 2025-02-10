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
 * Búsqueda Exhaustiva: Complejidad O(2^n)
 */
int exhaustiveSearch(const vector<int>& weights, bool useShortCircuit) {
    int n = weights.size();
    int minDiff = numeric_limits<int>::max();
    vector<int> assignment(n, 0);

    for (int mask = 0; mask < (1 << n); mask++) {
        for (int i = 0; i < n; i++) {
            assignment[i] = (mask >> i) & 1;
        }
        int diff = calculateDifference(weights, assignment);
        if (diff < minDiff) {
            minDiff = diff;
            if (useShortCircuit && minDiff == 0) break;
        }
    }
    return minDiff;
}

/**
 * Función auxiliar para Branch and Bound.
 */
void branchAndBoundHelper(const vector<int>& weights, vector<int>& assignment, int index, int sum1, int sum2, int& minDiff, bool useShortCircuit) {
    if (index == weights.size()) {
        int diff = abs(sum1 - sum2);
        if (diff < minDiff) {
            minDiff = diff;
        }
        return;
    }

    // Ramificación: conjunto 1
    assignment[index] = 0;
    branchAndBoundHelper(weights, assignment, index + 1, sum1 + weights[index], sum2, minDiff, useShortCircuit);
    if (useShortCircuit && minDiff == 0) return;

    // Ramificación: conjunto 2
    assignment[index] = 1;
    branchAndBoundHelper(weights, assignment, index + 1, sum1, sum2 + weights[index], minDiff, useShortCircuit);
    if (useShortCircuit && minDiff == 0) return;
}

/**
 * Branch and Bound: Complejidad O(2^n)
 */
int branchAndBound(const vector<int>& weights, bool useShortCircuit) {
    int minDiff = numeric_limits<int>::max();
    vector<int> assignment(weights.size(), 0);
    branchAndBoundHelper(weights, assignment, 0, 0, 0, minDiff, useShortCircuit);
    return minDiff;
}

/**
 * **Heurística Aleatoria Mejorada**
 * - Inicia con una asignación aleatoria de pesos a dos subconjuntos.
 * - Realiza swaps aleatorios para minimizar la diferencia.
 * - Se detiene si encuentra una diferencia de 0 o tras `maxIterations` intentos.
 * Complejidad Aproximada: O(n log n)
 */
int randomizedHeuristic(const vector<int>& weights, int maxIterations) {
    int n = weights.size();
    vector<int> assignment(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> coinFlip(0, 1);

    // Asignar aleatoriamente los pesos a dos conjuntos
    for (int i = 0; i < n; i++) {
        assignment[i] = coinFlip(gen);
    }

    int bestDiff = calculateDifference(weights, assignment);

    for (int iter = 0; iter < maxIterations; iter++) {
        int i = gen() % n;
        assignment[i] = 1 - assignment[i];  // Cambiar de conjunto
        int newDiff = calculateDifference(weights, assignment);

        if (newDiff < bestDiff) {
            bestDiff = newDiff;
        } else {
            assignment[i] = 1 - assignment[i];  // Revertir cambio si no mejora
        }

        if (bestDiff == 0) break; // Solución óptima encontrada
    }

    return bestDiff;
}

int main() {
    int n, minWeight, maxWeight, maxIterations = 10000;
    bool useShortCircuit;
    char algorithmChoice;

    cout << "Ingrese el número de pesos: ";
    cin >> n;
    cout << "Ingrese el valor mínimo de los pesos: ";
    cin >> minWeight;
    cout << "Ingrese el valor máximo de los pesos: ";
    cin >> maxWeight;
    cout << "¿Usar short circuit? (1: Sí, 0: No): ";
    cin >> useShortCircuit;
    cout << "Seleccione el algoritmo (E: Exhaustiva, B: Branch and Bound, H: Heurística Aleatoria): ";
    cin >> algorithmChoice;

    vector<int> weights = generateRandomWeights(n, minWeight, maxWeight);

    cout << "Pesos generados: ";
    for (int weight : weights) {
        cout << weight << " ";
    }
    cout << endl;

    int result;
    auto start = chrono::high_resolution_clock::now();

    if (algorithmChoice == 'E' || algorithmChoice == 'e') {
        result = exhaustiveSearch(weights, useShortCircuit);
        cout << "Resultado (Búsqueda Exhaustiva): ";
    } else if (algorithmChoice == 'B' || algorithmChoice == 'b') {
        result = branchAndBound(weights, useShortCircuit);
        cout << "Resultado (Branch and Bound): ";
    } else if (algorithmChoice == 'H' || algorithmChoice == 'h') {
        result = randomizedHeuristic(weights, maxIterations);
        cout << "Resultado (Heurística Aleatoria): ";
    } else {
        cout << "Opción no válida." << endl;
        return 1;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "Diferencia mínima: " << result << endl;
    cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << endl;

    return 0;
}
