#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <set>
#include <algorithm>

struct estado {
  int fila;
  int columna;
  int orientacion;
};

class Node {
public:
  Node * parent;
  estado coordenadas;
  int G, H;

  Node(const estado &coord) {
    coordenadas = coord;
    G = H = 0;
  }

  inline int getPuntuacion() {
    return G + H;
  }
};

using NodeSet = std::set<Node*>;

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

    Node* buscarNodoEnSet(NodeSet& nodos, estado estado);
    estado ComportamientoJugador::delta(estado inicio, estado fin);
    int ComportamientoJugador::heuristic(estado inicio, estado fin);
};

#endif
