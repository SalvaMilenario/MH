#ifndef PROBLEMA_H
#define PROBLEMA_H


#include <fstream>
#include <vector>

using namespace std;

class Problema{
private:
	int tamano;
	vector <vector<int> > flujo;
	vector <vector<int> > distancias;

	/**
	@brief Método privado que servirá al constructor para crear la matriz de distancias.

	*/
public:

	/**
	@brief El constructor de la clase, inicializa el problema guardando los datos en las matrices
	@param string con el nombre del fichero que le pasamos.

	*/
	Problema(string fname);

	/**
	@brief El constructor de copia
	@param problema a copiar
	*/
	Problema(Problema *otroP);

	/**
	@brief get de tamaño del problema.
	@return entero con el tamaño del problema.
	*/

	int getTamano();

	/**
	@brief get de la matriz de flujo
	@return entero con el flujo de 'una' a 'otra'
	*/

	int getFlujo(int una, int otra);

	/**
	@brief get de la matriz de distancias
	@return entero con el distancias de 'una' a 'otra'
	*/

	int getDistancia(int una, int otra);


	~Problema();



	

};








#endif