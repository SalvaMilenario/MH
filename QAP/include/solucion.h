#ifndef SOLUCION_H
#define SOLUCION_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random.h>
#include <climits>
#include <string>
#include <utility>
#include <algorithm>
#include "problema.h"
using namespace std;

class Solucion{
private:
	Problema *problema;
	int *orden;
	double coste;
	int tamano;

public:

	/**
	@brief constructor que inicializará los punteros.
	@param problema para esta solucion
	*/
	Solucion(Problema *problema_introducido);

	/**
	@brief constructor para soluciones existentes
	@param problema y archivo con la solucion
	*/
	
	Solucion(Problema *problema_introducido, string fname);

	/**
	@brief Constructor de copia
	@param una solucion a copiar
	*/
	Solucion(Solucion *sol);

	/**
	@brief destructor de la clase.
	*/
	~Solucion();

	/**
	@brief crea la solucion gredy del problema pasado
	*/
	void gredy();

	void gredyAleatorio(Random *rd, double alfa);

	/**
	@brief Genera solucion aleatoria
	*/

	void solAleatoria(Random *rd);

	/**
	
	*/
	bool busquedaLocalPVM(int &evaluaciones, int limEva);

	string mostrarSolucion();

	double diferenciaCoste(int unidad, int otraUni);

	/**
	@brief cruza dos soluciones 
	*/

	Solucion* cruzarSolucionesBP(Solucion *padre, Random *rd);

	pair<Solucion*, Solucion*> cruzarSolucionesPMX(Solucion *padre);

	void mutar(Random *rd);

	void hacerCambio(int unidad, int otraUni);

	void mutacionIls(Random *rd, double limite);

	/**
	@brief Get del coste de la solucion
	@return entero con el valor del coste de esa soliucion
	*/

	inline double getCoste(){return coste;}

	inline int getTamano(){return tamano;}

	inline Problema* getProblema(){return problema;}

};



#endif