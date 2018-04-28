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

class Nodo {
  private:
    estado base;
    list<Action> path;

    // A*
    int g, h, f;
  public:
    Nodo() { }
    Nodo(estado estado) {
      base = estado;
    }
    Nodo(int fila, int columna) {
      base.fila = fila;
      base.columna = columna;
    }
    void addToPath(Action step) {
      path.push_back(step);
    }

    //Setters
    void setG(int g) {
      this->g = g;
    }
    void setH(int h) {
      this->h = h;
    }
    void setF(int f) {
      this->f = f;
    }
    void setBase(estado base) {
      this->base = base;
    }
    //Getters
    list<Action> getPath() const {
      return path;
    }
    int getG() const {
      return g;
    }
    int getH() const {
      return h;
    }
    int getF() const {
      return f;
    }
    estado getBase() const {
      return base;
    }
    int getFila() const {
      return base.fila;
    }
    int getColumna() const {
      return base.columna;
    }
    int getOrientacion() const {
      return base.orientacion;
    }
    bool operator==(Nodo &otro) const {
      return otro.getFila() == base.fila && otro.getColumna() == base.columna;
    }
		bool operator==(const Nodo &otro) const {
      return otro.getFila() == base.fila && otro.getColumna() == base.columna;
    }
};

struct functorNodos{
  bool operator()(Nodo &a, Nodo &b){
    return a.getF() > b.getF() ||
      (a.getF() == b.getF() && a.getH() > b.getH() );
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
