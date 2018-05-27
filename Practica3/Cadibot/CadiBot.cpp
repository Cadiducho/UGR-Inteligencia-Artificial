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

int CadiBot::heuristica(const GameState &state, Player jugador) {
	Player rival = (jugador == J2 ? J1 : J2); //el contrario

	float miLado = 0;
	float otroLado = 0;
	for (int i = 1; i <= 6; i++) {
			miLado += state.getSeedsAt(jugador, (Position) i);
			otroLado += state.getSeedsAt(rival, (Position) i);
	}
	//Rebajo esos valores a su peso
	miLado *= 0.45;
	otroLado *= 0.45;

	miLado += (state.getScore(jugador) * 3.45);
	otroLado += (state.getScore(rival) * 3.45);

	float diferencia = miLado - otroLado;
	int heuristica = int(diferencia + 0.5); //redondear al entero superior

	return heuristica;
}

Movimiento CadiBot::alfabeta(const GameState &state, Player jugador, int depth, int alpha, int beta) {
	Movimiento movimiento;

	if (depth == MAX_DEPTH || state.isFinalState()) {
		movimiento.score = heuristica(state, jugador);
		return movimiento;
	}

	for (int i = 1; i <= 6; i++) {
		if (state.getSeedsAt(state.getCurrentPlayer(), (Position) i) > 0) { //no suicidarse
			GameState nuevoHijo = state.simulateMove((Move) i);
			Movimiento recursivo = alfabeta(nuevoHijo, jugador, depth + 1, alpha, beta);
			if (state.getCurrentPlayer() == jugador) {
				//alpha = max(score, beta)
				if (recursivo.score > alpha) {
					alpha = recursivo.score;
					movimiento.move = (Move) i;
				}
			} else {
				//beta = max(score, beta)
				if (recursivo.score < beta) {
					beta = recursivo.score;
				}
			}
			//poda
			if (beta <= alpha) {
				break;
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
