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
 * Complejidad Temporal: O(n)
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
 * Complejidad Temporal: O(n)
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
 * Heurística Mejorada
 * Complejidad Aproximada: O(n log n)
 */
int improvedHeuristic(vector<int> weights, int maxIterations) {
    int n = weights.size();
    vector<int> assignment(n, 0);

    sort(weights.rbegin(), weights.rend());
    int sum1 = 0, sum2 = 0;
    for (int i = 0; i < n; i++) {
        if (sum1 <= sum2) {
            sum1 += weights[i];
            assignment[i] = 0;
        } else {
            sum2 += weights[i];
            assignment[i] = 1;
        }
    }

    int bestDiff = abs(sum1 - sum2);
    random_device rd;
    mt19937 gen(rd());

    for (int iter = 0; iter < maxIterations; iter++) {
        int i = gen() % n, j = gen() % n;
        if (i != j && assignment[i] != assignment[j]) {
            swap(assignment[i], assignment[j]);
            int newDiff = calculateDifference(weights, assignment);

            if (newDiff < bestDiff) {
                bestDiff = newDiff;
            } else {
                swap(assignment[i], assignment[j]);
            }
        }
        if (bestDiff == 0) break;
    }
    return bestDiff;
}

/**
 * Hill-Climbing para la partición de pesos
 * Complejidad Aproximada: O(n log n) en la inicialización, O(n^2) en el refinamiento.
 */
int hillClimbing(vector<int> weights, int maxIterations) {
    int n = weights.size();
    vector<int> assignment(n, 0);
    sort(weights.rbegin(), weights.rend());
    int sum1 = 0, sum2 = 0;
    for (int i = 0; i < n; i++) {
        if (sum1 <= sum2) {
            sum1 += weights[i];
            assignment[i] = 0;
        } else {
            sum2 += weights[i];
            assignment[i] = 1;
        }
    }

    int bestDiff = abs(sum1 - sum2);
    random_device rd;
    mt19937 gen(rd());

    for (int iter = 0; iter < maxIterations; iter++) {
        bool improved = false;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (assignment[i] != assignment[j]) {
                    swap(assignment[i], assignment[j]);
                    int newDiff = calculateDifference(weights, assignment);
                    if (newDiff < bestDiff) {
                        bestDiff = newDiff;
                        improved = true;
                    } else {
                        swap(assignment[i], assignment[j]);
                    }
                }
            }
        }
        if (!improved) break;
    }
    return bestDiff;
}

int main() {
    int n, minWeight, maxWeight, maxIterations = 10000;
    char algorithmChoice;

    cout << "Ingrese el número de pesos: ";
    cin >> n;
    cout << "Ingrese el valor mínimo de los pesos: ";
    cin >> minWeight;
    cout << "Ingrese el valor máximo de los pesos: ";
    cin >> maxWeight;
    cout << "Seleccione el algoritmo (I: Heurística Mejorada, H: Hill-Climbing): ";
    cin >> algorithmChoice;

    vector<int> weights = generateRandomWeights(n, minWeight, maxWeight);
    cout << "Pesos generados: ";
    for (int weight : weights) {
        cout << weight << " ";
    }
    cout << endl;

    int result;
    auto start = chrono::high_resolution_clock::now();

    if (algorithmChoice == 'I' || algorithmChoice == 'i') {
        result = improvedHeuristic(weights, maxIterations);
        cout << "Resultado (Heurística Mejorada): ";
    } else if (algorithmChoice == 'H' || algorithmChoice == 'h') {
        result = hillClimbing(weights, maxIterations);
        cout << "Resultado (Hill-Climbing): ";
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
