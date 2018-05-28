#include "Bot.h"

#ifndef CADIBOT_H_
#define CADIBOT_H_

#define MAX_DEPTH 12

struct Movimiento {
	Move move;
	int score; //heuristica
};

class Cadibot:Bot {
public:
	Cadibot();
	~Cadibot();


	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);

	int heuristica(const GameState &state, Player jugador);

	Movimiento alfabeta(const GameState &state, Player jugador, int depth, int alpha, int beta);
};

#endif /* CADIBOT_H_ */
