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
#include <fstream>
#include <tuple> // Include tuple library for std::tie and std::make_tuple

using namespace std;

// Global variables for counting visited nodes
long long nodos_exhaustivo = 0;
long long nodos_branch_bound = 0;

/**
 * Generates a list of random weights.
 * Time Complexity: O(n), where n is the number of weights.
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
 * Calculates the difference between two sets of weights.
 * Time Complexity: O(n), where n is the number of weights.
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
 * Exhaustive search for solving the partition problem.
 * Time Complexity: O(2^n), where n is the number of weights.
 */
void particiones(const vector<int>& weights, int index, int sum1, int sum2, int& minDiff) {
    nodos_exhaustivo++;
    if (index == weights.size()) {
        int diff = abs(sum1 - sum2);
        if (diff < minDiff) {
            minDiff = diff;
        }
        return;
    }

    // Assign the current weight to the first set
    particiones(weights, index + 1, sum1 + weights[index], sum2, minDiff);

    // Assign the current weight to the second set
    particiones(weights, index + 1, sum1, sum2 + weights[index], minDiff);
}

/**
 * Branch and Bound for solving the partition problem.
 * Time Complexity: O(2^n) in the worst case, but with pruning.
 */
void branch_and_bound_iterativo(const vector<int>& weights) {
    int n = weights.size();
    int minDiff = numeric_limits<int>::max();
    vector<int> assignment(n, 0);

    // Using a stack to simulate recursion
    vector<tuple<int, int, int, vector<int>>> stack;
    stack.push_back(make_tuple(0, 0, 0, assignment));

    while (!stack.empty()) {
        nodos_branch_bound++;
        int index, sum1, sum2;
        vector<int> currentAssignment;
        tie(index, sum1, sum2, currentAssignment) = stack.back();
        stack.pop_back();

        if (index == n) {
            int diff = abs(sum1 - sum2);
            if (diff < minDiff) {
                minDiff = diff;
            }
            continue;
        }

        // Pruning: if the current difference is already worse than the best found, do not explore further
        if (abs(sum1 - sum2) >= minDiff) {
            continue;
        }

        // Assign the current weight to the first set
        currentAssignment[index] = 0;
        stack.push_back(make_tuple(index + 1, sum1 + weights[index], sum2, currentAssignment));

        // Assign the current weight to the second set
        currentAssignment[index] = 1;
        stack.push_back(make_tuple(index + 1, sum1, sum2 + weights[index], currentAssignment));
    }
}

/**
 * Conducts experiments for different set sizes.
 */
void realizar_experimentos() {
    ofstream archivo("resultados.csv");
    archivo << "Tamaño,Nodos_Exhaustivo,Nodos_BranchBound\n";
    for (int n = 1; n <= 25; ++n) {
        vector<int> pesos(n);
        for (int i = 0; i < n; ++i) {
            pesos[i] = i + 1; // Weights from 1 to n
        }
        nodos_exhaustivo = 0;
        nodos_branch_bound = 0;

        // Execute methods
        int min_dif_exh = numeric_limits<int>::max();
        particiones(pesos, 0, 0, 0, min_dif_exh);
        branch_and_bound_iterativo(pesos);

        // Write results to file
        archivo << n << "," << nodos_exhaustivo << "," << nodos_branch_bound << "\n";
        cout << "n = " << n
             << ", Exhaustive nodes = " << nodos_exhaustivo
             << ", Branch & Bound nodes = " << nodos_branch_bound << endl;
    }

    archivo.close();
    cout << "Results saved in 'resultados.csv'" << endl;
}

/**
 * Main function that runs the partition experiments.
 */
int main() {
    realizar_experimentos();
    return 0;
}
