#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <random>
#include <chrono>
#include <fstream>
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
int exhaustiveSearch(const vector<int>& weights, bool useShortCircuit, vector<int>& bestAssignment, long long& nodesVisited) {
    int n = weights.size();
    int minDiff = numeric_limits<int>::max();
    vector<int> assignment(n, 0);

    // Generar todas las combinaciones posibles
    for (int mask = 0; mask < (1 << n); mask++) {
        nodesVisited++;
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

/**
 * Branch and Bound para resolver el problema de partición.
 * Complejidad Temporal: O(2^n) en el peor caso, pero con poda.
 */
void branchAndBound(const vector<int>& weights, int index, int currentDiff, vector<int>& assignment, int& minDiff, vector<int>& bestAssignment, long long& nodesVisited) {
    nodesVisited++;
    if (index == weights.size()) {
        if (currentDiff < minDiff) {
            minDiff = currentDiff;
            bestAssignment = assignment;
        }
        return;
    }

    // Prune if the current difference is already worse than the best
    if (currentDiff >= minDiff) {
        return;
    }

    // Assign to set 0
    assignment[index] = 0;
    branchAndBound(weights, index + 1, abs(currentDiff + weights[index]), assignment, minDiff, bestAssignment, nodesVisited);

    // Assign to set 1
    assignment[index] = 1;
    branchAndBound(weights, index + 1, abs(currentDiff - weights[index]), assignment, minDiff, bestAssignment, nodesVisited);
}

int main() {
    int minWeight = 1;
    bool useShortCircuit = true;

    // Archivo CSV para guardar los resultados
    ofstream csvFile("/workspaces/Act5.6/Ejercicio_07");
    if (!csvFile.is_open()) {
        cerr << "Error al abrir el archivo CSV." << endl;
        return 1;
    }
    csvFile << "n,Exhaustivo,BranchAndBound\n";

    // Probar con diferentes valores de n
    for (int n = 5; n <= 25; n++) {
        int maxWeight = n;
        vector<int> weights = generateRandomWeights(n, minWeight, maxWeight);

        // Búsqueda exhaustiva
        vector<int> bestAssignmentExhaustive;
        long long exhaustiveNodesVisited = 0;
        auto startExhaustive = chrono::high_resolution_clock::now();
        int resultExhaustive = exhaustiveSearch(weights, useShortCircuit, bestAssignmentExhaustive, exhaustiveNodesVisited);
        auto endExhaustive = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsedExhaustive = endExhaustive - startExhaustive;

        // Branch and Bound
        vector<int> bestAssignmentBB;
        long long BBNodesVisited = 0;
        int minDiffBB = numeric_limits<int>::max();
        vector<int> assignmentBB(n, 0);
        auto startBB = chrono::high_resolution_clock::now();
        branchAndBound(weights, 0, 0, assignmentBB, minDiffBB, bestAssignmentBB, BBNodesVisited);
        auto endBB = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsedBB = endBB - startBB;

        // Guardar resultados en el archivo CSV
        csvFile << n << "," << exhaustiveNodesVisited << "," << BBNodesVisited << "\n";

        // Mostrar resultados en la consola
        cout << "n = " << n << endl;
        cout << "Búsqueda Exhaustiva: Diferencia mínima = " << resultExhaustive << ", Nodos visitados = " << exhaustiveNodesVisited << ", Tiempo = " << elapsedExhaustive.count() << " segundos" << endl;
        cout << "Branch and Bound: Diferencia mínima = " << minDiffBB << ", Nodos visitados = " << BBNodesVisited << ", Tiempo = " << elapsedBB.count() << " segundos" << endl;
        cout << endl;
    }

    // Cerrar el archivo CSV
    csvFile.close();
    cout << "Resultados guardados en 'Ejercicio_07/resultados.csv'." << endl;

    return 0;
}