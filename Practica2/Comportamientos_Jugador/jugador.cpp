#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>

void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}


bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
  return false;
}

Action ComportamientoJugador::think(Sensores sensores) {
	//capturar la fila y la columna
	if (sensores.mensajeF != -1 && sensores.mensajeC != -1) {
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
	}

	//actualizar mi estado en el mundo
	switch (ultimaAccion) {
		case actTURN_R:
			brujula = (brujula + 1) % 4; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
			break;
		case actTURN_L:
			brujula = (brujula - 1 + 3) % 4;  // +3 porque C no sabe hacer mod de números negativos, se fuerza positivo pero no altera el resultado del mod
			break;
		case actFORWARD:
			switch (brujula) {
				case 0: fil--; break;
				case 1: col++; break;
				case 2: fil++; break;
				case 3: col--; break;
			}
			break;
	}

	cout << "fil: " << fil << ", col: " << col << ", orientacion: " << brujula << endl;

	//Comportamiento del agente
	Action sigAccion;
  if (sensores.terreno[2] == 'T' || sensores.terreno[2] == 'S'|| sensores.terreno[2] == 'K'
			&& sensores.superficie[2] == 'a') {
		sigAccion = actFORWARD;
	} else {
		sigAccion = actTURN_R;
	}

	//recordar la ultima accion
	ultimaAccion = sigAccion;
	return sigAccion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
