#include <iostream>
#include <fstream>
#include <string>
#include "problema.h"

using namespace std;

Problema::Problema(string fname)
{
	ifstream archi;
	archi.open(fname.c_str());
	archi >> tamano;
	int basura;
	//inicializo las matrices de flujo y distancias
	for (int i = 0; i < tamano; ++i)
	{
		vector<int> f;
		for (int j = 0; j < tamano; ++j)
		{
			int aux;
			archi >> aux;
			f.push_back(aux);
		}
		flujo.push_back(f);
	}
	for (int i = 0; i < tamano; ++i)
	{
		vector<int> d;
		for (int j = 0; j < tamano; ++j)
		{
			int aux;
			archi >> aux;
			d.push_back(aux);
		}
		distancias.push_back(d);
	}
	/*
	for (int i = 0; i < tamano; ++i)
	{
		for (int j = 0; j < tamano; ++j)
		{
			cout << flujo[i][j]<<'\t';
		}
		cout << endl;
	}
	for (int i = 0; i < tamano; ++i)
	{
		for (int j = 0; j < tamano; ++j)
		{
			cout << distancias[i][j]<<'\t';
		}
		cout << endl;
	}
	*/
}

int Problema::getTamano()
{
	return tamano;
}

int Problema::getFlujo(int una, int otra)
{
	return flujo[una][otra];
}

int Problema::getDistancia(int una, int otra){
	return distancias[una][otra];
}
Problema::Problema(Problema *otroP)
{
	this->tamano=otroP->tamano;
	for (int i = 0; i < this->tamano; ++i)
	{
		vector<int> f,d;
		for (int j = 0; j < this->tamano; ++j)
		{
			f.push_back(otroP->flujo[i][j]);
			d.push_back(otroP->distancias[i][j]);
		}
		this->flujo.push_back(f);
		this->distancias.push_back(d);
	}
}

Problema::~Problema()
{
	for (int i = 0; i < flujo.size(); ++i)
	{
		flujo[i].clear();
		flujo[i].resize(0);
		distancias[i].clear();
		distancias[i].resize(0);
	}
	flujo.clear();
	flujo.resize(0);
	distancias.clear();
	distancias.resize(0);
}