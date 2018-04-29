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
	  } else if (*it == actTURN_R){
	    cst.orientacion = (cst.orientacion+1)%4;
	  } else {
	    cst.orientacion = (cst.orientacion+3)%4;
	  }
	  it++;
	}
}

bool esObstaculo(char ch){
		return !(ch == 'T' or ch == 'K' or ch == 'S');
}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
    list<nodo> abiertos;
    set<nodo> cerrados;
    nodo actual;
    actual.first=origen;
    abiertos.push_back(actual);

    while(!abiertos.empty() and abiertos.front().first.fila != destino.fila or abiertos.front().first.columna != destino.columna ){
            actual = abiertos.front();
            abiertos.pop_front();
            cerrados.insert(actual);

						//dcha
            nodo siguiente = actual;
            siguiente.first.orientacion = (actual.first.orientacion + 1) % 4;
            siguiente.second.push_back(actTURN_R);
            if (cerrados.find(siguiente) == cerrados.end()){
                abiertos.push_back(siguiente);
                cerrados.insert(siguiente);
            }

						//izq
            siguiente = actual;
            siguiente.first.orientacion = (actual.first.orientacion + 3) % 4;
            siguiente.second.push_back(actTURN_L);
            if (cerrados.find(siguiente) == cerrados.end()){
                abiertos.push_back(siguiente);
                cerrados.insert(siguiente);
            }

						//avanzar
            siguiente = actual;
            switch(actual.first.orientacion){
            case 0:
                siguiente.first.fila --;
            break;
            case 1:
                siguiente.first.columna ++;
            break;
            case 2:
                siguiente.first.fila ++;
            break;
            case 3:
                siguiente.first.columna --;
            break;
            }
            if(!esObstaculo(mapaResultado[siguiente.first.fila][siguiente.first.columna])){
                siguiente.second.push_back(actFORWARD);
                if (cerrados.find(siguiente) == cerrados.end()){
                    abiertos.push_back(siguiente);
                    cerrados.insert(siguiente);
                }
            }
    }

    if (!abiertos.empty()) {
        cout << "Solución encontrada" << endl;
        plan = abiertos.front().second;
        return true;
    } else {
				cout << "Sin plan" << endl;
        return false;
    }
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

    hayPlan = pathFinding(origen, destino, plan);
		VisualizaPlan(origen, plan);

		ultimaPosicionF = sensores.destinoF;
		ultimaPosicionC = sensores.destinoC;
	}

	//Intentar detectar a un aldeano y evitarlo
	int ticksDeEspera = 10;
	bool aldeanoDelante = sensores.superficie[2] == 'a';
	if (aldeanoDelante && !plan.empty() && plan.front() == actFORWARD) {
		cout << "Se evita la colisión" << endl;
		if (ticksDeEspera > 10) {
			cout << "Intento calcular un nuevo plan evitando al aldeano" << endl;
			estado origen, destino;
			origen.fila = fil;
			origen.columna = col;
			origen.orientacion = brujula;
			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;
			list<Action> nuevo_plan;
      pathFinding(origen, destino, nuevo_plan);
      int incremento = nuevo_plan.size() - plan.size();
      // Si el nuevo plan es posible y no añade más del tiempo del que se va a esperar, lo aplico
      if (!nuevo_plan.empty() && (incremento < 10 || ticksDeEspera >= 10)) {
        ticksDeEspera = 0;
        plan = nuevo_plan;
        VisualizaPlan(origen, plan);
				cout << "Nuevo plan aplicado" << endl;
			} else {
				ticksDeEspera++;
				cout << "Esperando" << endl;
	      return actIDLE;
			}
		} else {
			ticksDeEspera++;
			cout << "Esperando" << endl;
      return actIDLE;
			//Corto con el return para que después no se salte acciones del plan creado
		}
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
