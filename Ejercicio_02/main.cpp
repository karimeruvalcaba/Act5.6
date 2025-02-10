/* AUTORES
 * Carlos Ernesto Soto Alarcón A01747990
 * Sergio Alfonso Casillas Santoyo A01424863
 * Karime Itzel Ruvalcaba Pérez A01656188
 */

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

// Encontrar una jugada para el jugador
// Complejidad: O(9), es decir O(1), checa las posiciones del tablero
int findWinningMove(vector<char> &board, char player){
    for (int i = 0; i < 9; i++){
        if (board[i] == ' '){
            board[i] = player;
            if (isWin(board, player)){
                board[i] = ' ';
                return i;
            }
            board[i] = ' ';
        }
    }
    return -1;
}

// Función recursiva para simular todos los juegos posibles
// Complejidad O(9!), en el peor de los casos revisa todas las posibles secuencias
void CountGames(vector<char> &board, int turn, int &xWins, int&oWins, int &draws){

    if (isWin(board, 'X')){
        xWins++;
        return;
    }

    if (isWin(board, 'O')){
        oWins++;
        return;
    }

    bool full = true;
    for (char c : board){
        if (c == ' '){
            full = false;
            break;
        }
    }
    if (full){
        draws++;
        return;
    }

    char player = (turn % 2 == 0) ? 'X' : 'O';
    vector<int> availableMoves;
    for (int i = 0; i < 9; i++){
        if (board[i] == ' '){
           availableMoves.push_back(i);
        }
    }

    int winningMove = findWinningMove(board, player);
    if (winningMove != -1){
        board[winningMove] = player;
        CountGames(board, turn + 1, xWins, oWins, draws);
        board[winningMove] = ' ';
        return;
    }

    char opponent = (player == 'X') ? 'O' : 'X';
    int blockMove = findWinningMove(board, opponent);
    if (blockMove != -1){
        board[blockMove] = player;
        CountGames(board, turn + 1, xWins, oWins, draws);
        board[blockMove] = ' ';
        return;
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(availableMoves.begin(), availableMoves.end(), g);

    for (int i:availableMoves){
        board[i] = player;
        CountGames(board, turn + 1, xWins, oWins, draws);
        board[i] = ' ';
    }
    
}

int main(){
    vector<char> board(9, ' ');
    int xWins = 0, oWins = 0, draws = 0;

    cout << "Ingresa el estado inicial del tablero fila por fila (usa X, O o un espacio para vacío):" << endl;
    
    for (int i = 0; i < 3; i++) {
        string row;
        getline(cin, row);
        for (int j = 0; j < 3; j++) {
            if (j < row.size() && (row[j] == 'X' || row[j] == 'O')) {
                board[i * 3 + j] = row[j];
            } else {
                board[i * 3 + j] = ' ';
            }
        }
    }

    CountGames(board, 0, xWins, oWins, draws);

    cout << "Gana X: " << xWins << endl;
    cout << "Gana O: " << oWins << endl;
    cout <<  "Empate: " << draws << endl;
    cout << "Total de juegos: " << (xWins + oWins + draws) << endl;

    return 0;
}