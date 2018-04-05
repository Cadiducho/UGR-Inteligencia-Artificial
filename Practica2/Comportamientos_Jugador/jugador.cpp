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

estado ComportamientoJugador::delta(estado inicio, estado fin) {
	estado delta;
	delta.fila = abs(inicio.fila - fin.fila);
	delta.columna = abs(inicio.columna - inicio.columna);
	return delta;
}
//Euclides
int ComportamientoJugador::heuristic(estado inicio, estado fin) {
    estado delta = delta(inicio, fin);
    return static_cast<uint>(10 * sqrt(pow(delta.fila, 2) + pow(delta.columna, 2)));
}

Node* ComportamientoJugador::buscarNodoEnSet(NodeSet& nodos, estado estado) {
    for (auto node : nodos) {
        if (node->coordenadas.fila == estado.fila && node->coordenadas.columna == estado.columna) {
            return node;
        }
    }
    return nullptr;
}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	plan.clear();

	//Declaraciones para el inicio del algoritmo
	Node *actual;
	NodeSet openSet, closedSet;
	openSet.insert(new Node(origen));

	while (!openSet.empty()) {
		actual = *openSet.begin();
		for (auto node : openSet) {
    	if (node->getPuntuacion() <= actual->getPuntuacion()) {
      	actual = node;
      }
    }

		if (actual->coordenadas.fila == destino.fila && actual->coordenadas.columna == destino.columna && actual->coordenadas.orientacion == destino.orientacion) {
			break; //Solucion alcanzada
		}

		closedSet.insert(actual);
		openSet.erase(std::find(openSet.begin(), openSet.end(), actual));

		for (int i = 0; i < 3; ++i) {
			//Establecer nuevo hijo segun si es norte, sur, este u oeste
			estado nuevoHijo = actual->coordenadas;
			switch (i) {
				case 0: nuevoHijo.fila--; break;
				case 1: nuevoHijo.columna++; break;
				case 2: nuevoHijo.fila++; break;
				case 3: nuevoHijo.columna--; break;
			}

			//Crear nodo y buscarlo en la lista para inicializarlo
			int totalCost = actual->G + 10;
			Node *siguiente = findNodeOnList(openSet, nuevoHijo);
			if (siguiente == nullptr) {
				siguiente = new Node(nuevoHijo, actual);
				siguiente->G = totalCost;
				siguiente->H = heuristic(siguiente->coordenadas, destino);
				openSet.insert(siguiente);
			} else if (totalCost < siguiente->G) {
			  siguiente->parent = actual;
			  siguiente->G = totalCost;
			}

		}
	}


	VisualizaPlan(origen, plan);

	return true;
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

	if (sensores.reset) {
		hayPlan = false;
	}

	//Si se ha cambiado el destino, calcular un nuevo plan
 	if (hayPlan && (sensores.destinoF != ultimaPosicionF || sensores.destinoC != ultimaPosicionC)) {
		cout << "El destino ha cambiado" << endl;
		hayPlan = false;
	}

	// Generar nuevo plan
	if (!hayPlan) {
		estado origen, destino;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;

    hayPlan = pathFinding(origen,destino,plan);

		ultimaPosicionF = sensores.destinoF;
		ultimaPosicionC = sensores.destinoC;
	}

	// Ejecutar el plan
	Action sigAccion;
	if (hayPlan && plan.size() > 0) {
		sigAccion = plan.front();
		plan.erase(plan.begin());
	} else {
		sigAccion = actIDLE;
	}

	ultimaAccion = sigAccion;
	return sigAccion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
