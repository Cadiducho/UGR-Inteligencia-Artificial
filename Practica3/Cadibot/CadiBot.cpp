#include "CadiBot.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <list>
#include <stack>
#include <limits.h>

using namespace std;

CadiBot::CadiBot() {
	// Inicializar las variables necesarias para ejecutar la partida
	srand(time(0));

}

CadiBot::~CadiBot() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void CadiBot::initialize() {
	// Inicializar el bot antes de jugar una partida
}

string CadiBot::getName() {
	return "CadiBot"; // Sustituir por el nombre del bot
}

Move CadiBot::nextMove(const vector<Move> &adversary, const GameState &state) {
	Move movimiento;

	int alpha = INT_MIN;
	int beta = INT_MAX;
	movimiento = alfabeta(state, 0, MAX_DEPTH, alpha, beta, JUGADOR_MAX);
/*	Nodo nodoActual;
	nodoActual.first = actual;

	stack<Nodo> pila;
	pila.push(nodoActual);

	while (!pila.empty() && nodoActual.first.getWinner() != yo) {
		nodoActual = pila.top();
		pila.pop();

		for (int i = 1; i <= 6; i++) {
			//Si hay más semillas que 0 es un posible movimiento
			if (nodoActual.first.getSeedsAt(yo, (Position) i) > 0) {
				Nodo hijo = nodoActual;
				hijo.first = nodoActual.first.simulateMove((Move) i);
				hijo.second.push_back(i);
				pila.push(hijo);
			}
		}

		nodoActual = pila.top();
	}

	if (pila.empty()) {
		cerr << "Creo que ya no puedo ganar" << endl;
		movimiento = (Move) ((rand() % 6) + 1);
	} else {
		movimiento = (Move) nodoActual.second.front();
	} */

	return movimiento;
}

Movimiento CadiBot::alfabeta(const GameState &state, int depth, int maxDepth, int alpha, int beta, int jugador) {
	Movimiento movimiento;

	cerr << "Profundidad actual: " << depth << endl;

	if (depth == maxDepth) {
		cerr << "Maxima profundidad (" << depth << "). Evaluando heurísiticas..." << endl;
	} else {
		if (jugador == JUGADOR_MIN) {
			movimiento.score = INT_MAX;
		} else if (jugador == JUGADOR_MAX) {
			movimiento.score = INT_MIN;
		}
	}

	return movimiento;
}
