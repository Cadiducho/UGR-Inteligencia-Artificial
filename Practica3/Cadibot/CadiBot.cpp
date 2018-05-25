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
	Movimiento movimiento;

	int alpha = INT_MIN;
	int beta = INT_MAX;
	movimiento = alfabeta(state, this->getPlayer(), 0, alpha, beta);

	return movimiento.move;
}

//ToDo: Hacer una heurística mucho mejor
int CadiBot::heuristica(const GameState &state, Player jugador) {
	Player rival = (jugador == J2 ? J1 : J2); //el contrario

	if (state.getSeedsAt(jugador, GRANERO) > 24) {
		return 30;
	} else if (state.getSeedsAt(rival, GRANERO) > 24) {
		return -30;
	}

	return state.getSeedsAt(jugador, GRANERO);
}

Movimiento CadiBot::alfabeta(const GameState &state, Player jugador, int depth, int alpha, int beta) {
	Movimiento movimiento;

	if (depth == MAX_DEPTH || state.isFinalState()) {
		movimiento.score = heuristica(state, jugador);
		return movimiento;
	}

	for (int i = 1; (i <= 6) && (alpha < beta); i++) {
		if (state.getSeedsAt(state.getCurrentPlayer(), (Position) i) > 0) {
			GameState nuevoHijo = state.simulateMove((Move) i);
			Movimiento anterior = alfabeta(nuevoHijo, jugador, depth + 1, alpha, beta);
			if (state.getCurrentPlayer() == jugador) {
				if (anterior.score > alpha) {
					alpha = anterior.score;
					movimiento.move = (Move) i;
				}
			} else {
				if (anterior.score < beta) {
					beta = anterior.score;
				}
			}
		}
	}

	if (state.getCurrentPlayer() == jugador) {
		movimiento.score = alpha;
		return movimiento;
	} else {
		movimiento.score = beta;
		return movimiento;
	}

	return movimiento;
}
