#include "Bot.h"

#ifndef CADIBOT_H_
#define CADIBOT_H_

#define JUGADOR_MAX 0
#define JUGADOR_MIN 1
#define MAX_DEPTH 5

struct Movimiento {
	Move move;
	int score; //heuristica
}

class CadiBot:Bot {
public:
	CadiBot();
	~CadiBot();


	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);

	/**

	*/
	Movimiento alfabeta(const GameState &state, int depth, int maxDepth, int alpha, int beta, int jugador);
};

#endif /* CADIBOT_H_ */
