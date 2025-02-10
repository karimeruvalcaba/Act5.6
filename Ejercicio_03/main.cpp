#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

// Combinaciones ganadoras
int win [8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

// Se checa si un jugador ha ganado
// Complejidad: O(1), checa si tiene probalidades de ganar
bool isWin(const vector<char> &board, char player){
    for (int i=0; i < 8; i++){
        if (board[win[i][0]] == player && board[win[i][1]] == player && board[win[i][2]] == player){
            return true;
        }
    }
    return false;
}


/**
* Verifica si el tablero está lleno (no hay espacios vacios).
* Complejidad: O(n), n es el tamaño del tablero, es decir 9 casillas
*/

bool isFull (const vector<char> &board){
    for (char c : board){
        if (c == ' '){
            return false;
        }
    }
    return true;
}

/**
* Se implementa el algoritmo de Minimax para determinar la mejor ppuntuación dependiendo del estado del tablero
* Complejidad: O(b^d), donde b es el factor de ramificación (9 para jufar gato) y d la profundidad de la recursión (maxDepth)
*/
int minimax(vector<char> &board, int depth, bool isMax, int maxDepth){
    if (isWin(board, 'X')) return 10 - depth;
    if (isWin(board, 'O')) return depth - 10;
    if (isFull(board) || depth == maxDepth) return 0;

    int bestScore = isMax ? numeric_limits<int>::min() : numeric_limits<int>::max();
    char player = isMax ? 'X' : 'O';

    for (int i = 0; i < 9; i++){
        if (board[i] == ' '){
            board[i] = player;
            int score = minimax(board, depth + 1, !isMax, maxDepth);
            board[i] = ' ';
            bestScore = isMax ? max(bestScore, score) : min(bestScore, score);
        }
    }
    return bestScore;
}

/**
 * Encuentra el mejor movimiento para el jugador actual utilizando el algoritmo Minimax.
 * 
 * Complejidad Temporal: O(b^d) - Donde b es el factor de ramificación (~9 para Tic-Tac-Toe)
 * y d es la profundidad máxima de la recursión (maxDepth).
 */
int bestMove(vector<char> &board, int maxDepth) {
    int move = -1, bestScore = numeric_limits<int>::min();
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = 'X';
            int score = minimax(board, 0, false, maxDepth);
            board[i] = ' ';
            if (score > bestScore) {
                bestScore = score;
                move = i;
            }
        }
    }
    return move;
}

/**
 * Realiza el movimiento de la computadora según la dificultad seleccionada.
 * 
 * Complejidad Temporal: O(n) - Donde n es el número de movimientos disponibles (máximo 9).
 */
void computerMove(vector<char> &board, int difficulty) {
    vector<int> availableMoves;
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') availableMoves.push_back(i);
    }

    if (difficulty == 1) {
        random_device rd;
        mt19937 g(rd());
        shuffle(availableMoves.begin(), availableMoves.end(), g);
        board[availableMoves[0]] = 'X';
    } else {
        int move = bestMove(board, difficulty == 2 ? 3 : 9);
        board[move] = 'X';
    }
}

/**
 * Muestra el tablero en la consola.
 * 
 * Complejidad Temporal: O(n) - Donde n es el tamaño del tablero (9 en este caso).
 */
void displayBoard(const vector<char> &board) {
    for (int i = 0; i < 9; i++) {
        cout << (board[i] == ' ' ? '-' : board[i]) << ((i % 3 == 2) ? "\n" : " ");
    }
}

int main() {
    vector<char> board(9, ' ');
    int difficulty;
    char player;

    cout << "Elige tu simbolo (X o O): ";
    cin >> player;
    cout << "Elige dificultad: 1-Random, 2-Beginner, 3-Expert: ";
    cin >> difficulty;

    if (difficulty < 1 || difficulty > 3) {
        cout << "Dificultad inválida. Saliendo del juego.\n";
        return 1;
    }

    while (!isFull(board) && !isWin(board, 'X') && !isWin(board, 'O')) {
        displayBoard(board);

        if (player == 'O') {
            int move;
            cout << "Ingresa tu movimiento (0-8): ";
            cin >> move;
            if (move < 0 || move > 8 || board[move] != ' ') {
                cout << "Movimiento inválido. Intenta de nuevo.\n";
                continue;
            }
            board[move] = 'O';
        } else {
            computerMove(board, difficulty);
        }

        if (isWin(board, 'X') || isWin(board, 'O') || isFull(board)) break;

        if (player == 'X') {
            int move;
            cout << "Ingresa tu movimiento (0-8): ";
            cin >> move;
            if (move < 0 || move > 8 || board[move] != ' ') {
                cout << "Movimiento inválido. Intenta de nuevo.\n";
                continue;
            }
            board[move] = 'X';
        } else {
            computerMove(board, difficulty);
        }
    }

    displayBoard(board);
    if (isWin(board, 'X')) cout << "Gana X!\n";
    else if (isWin(board, 'O')) cout << "Gana O!\n";
    else cout << "Empate!\n";

    return 0;
}