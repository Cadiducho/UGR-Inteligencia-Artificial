#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <set>
#include <queue>
#include <algorithm>

struct estado {
  int fila;
  int columna;
  int orientacion;
};

struct nodo{
		estado first;
		list<Action> second;

		int g, h;
    inline int getF() {
      return g + h;
    }
		bool operator<(const nodo &x) const {
				return ((first.fila < x.first.fila)
              || (first.fila == x.first.fila and first.columna < x.first.columna)
              || (first.fila == x.first.fila and first.columna == x.first.columna and first.orientacion < x.first.orientacion));
		}
};

struct functorNodos{
  bool operator()(nodo &a, nodo &b){
    return a.getF() > b.getF() ||
      (a.getF() == b.getF() && a.h > b.h );
  }
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;

      ultimaAccion = actIDLE;
      hayPlan = false;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;

      ultimaAccion = actIDLE;
      hayPlan = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    int fil, col, brujula;
    estado destino;
    list<Action> plan;

    //Nuevas variables de Estado
    Action ultimaAccion;
    bool hayPlan;
    int ultimaPosicionF;
    int ultimaPosicionC;

    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);
    void PintaPlan(list<Action> plan);
};

#endif
