#include "solucion.h"

using namespace std;

Solucion::Solucion(Problema *problema_introducido)
{
	coste = 0;
	problema = problema_introducido;
	tamano = problema->getTamano();
	orden = new int[tamano];
}

Solucion::Solucion(Problema *problema_introducido, string fname)
{
	ifstream archi;
	archi.open(fname.c_str());
	archi >> tamano;
	archi >> coste;
	orden = new int[tamano];
	problema=problema_introducido;
	for (int i = 0; i < tamano; ++i)
	{
		archi >> orden[i];
	}
}

Solucion::Solucion(Solucion *sol)
{
	this->tamano =sol->problema->getTamano();
	this->problema=sol->problema;
	this->orden = new int[this->tamano];
	for (int i = 0; i < tamano; ++i)
	{
		this->orden[i]=sol->orden[i];
	}
	this->coste = sol->coste;
}

void Solucion::gredy()
{
	vector<int> fi;
	vector<int> di;
	for (int i = 0; i < this->tamano; ++i)
	{
		int fsun=0, dsun=0;
		for (int j = 0; j < this->tamano; ++j)
		{
			fsun+=problema->getFlujo(i,j);
			dsun+=problema->getDistancia(i,j);
		}
		fi.push_back(fsun);
		di.push_back(dsun);
	}
	int nAnadidos=0;
	do
	{
		int maxf=0, mind=0;
			for (int i = 1; i < tamano; ++i)
			{
				if(fi.at(maxf)<fi.at(i))
					maxf=i;
				if (di.at(mind)>di.at(i))
					mind=i;
			}
		orden[maxf]=mind;
		fi[maxf]=INT_MIN;
		di[mind]=INT_MAX;
		nAnadidos++;
	}while(nAnadidos<tamano);
	for (int i = 0; i < tamano; ++i)
	{
		for (int j = 0; j < tamano; ++j)
		{
			if(problema->getFlujo(i,j)>0)
			{
				coste += problema->getFlujo(i,j)*problema->getDistancia(orden[i],orden[j]);
			}
		}
		//cout << i << '-' << orden[i]<<endl;
	}
	//cout << coste << endl;
}

void Solucion::gredyAleatorio(Random *rd, double alfa)
{
	vector<int> fi, ordenFi, listaFi, ordenRestrinFi, listaRestrinFi;
	vector<int> di, ordenDi, listaDi, ordenRestrinDi, listaRestrinDi;
	for (int i = 0; i < this->tamano; ++i)
	{
		int fsun=0, dsun=0;
		for (int j = 0; j < this->tamano; ++j)
		{
			fsun+=problema->getFlujo(i,j);
			dsun+=problema->getDistancia(i,j);
		}
		fi.push_back(fsun);
		di.push_back(dsun);
	}

	int nAnadidos=0;

	do
	{
		int maxf=0, mind=0;
			for (int i = 1; i < tamano; ++i)
			{
				if(fi.at(maxf)<fi.at(i))
					maxf=i;
				if (di.at(mind)>di.at(i))
					mind=i;
			}
		listaFi.push_back(maxf);
		listaDi.push_back(mind);
		ordenFi.push_back(fi[maxf]);
		ordenDi.push_back(di[mind]);
		fi[maxf]=INT_MIN;
		di[mind]=INT_MAX;
		nAnadidos++;
	}while(nAnadidos<tamano);

	/*for (int i = 0; i < tamano; ++i)
	{
		cout<<"f"<<listaFi.at(i)<<" = " <<ordenFi.at(i)<<endl;
	}*/

	int umbralF = ordenFi.at(0) - alfa * (ordenFi.at(0)-ordenFi.at(ordenFi.size()-1));
	int umbralD = ordenDi.at(0) - alfa * (ordenDi.at(0)-ordenDi.at(ordenDi.size()-1));
	int fr, dr;
	bool fase1=true, zero = false;;
	nAnadidos=0;
	//cout<<umbralF<<endl<<umbralD<<endl;
	do
	{
		if (!fase1)
		{
			if (!ordenFi.empty())
				if (ordenRestrinFi.empty())
					umbralF = ordenFi.at(0) - alfa * (ordenFi.at(0)-ordenFi.at(ordenFi.size()-1));
				else
					umbralF = ordenRestrinFi.at(0) - alfa * (ordenRestrinFi.at(0)-ordenFi.at(ordenFi.size()-1));
			if (!ordenDi.empty())
				if (ordenRestrinDi.empty())
					umbralD = ordenDi.at(0) - alfa * (ordenDi.at(0)-ordenDi.at(ordenDi.size()-1));
				else
					umbralD = ordenRestrinDi.at(0) - alfa * (ordenRestrinDi.at(0)-ordenDi.at(ordenDi.size()-1));
		}
		if (!ordenFi.empty())
			while(ordenFi.at(0)>=umbralF)
			{
				ordenRestrinFi.push_back(ordenFi.at(0));
				listaRestrinFi.push_back(listaFi.at(0));
				listaFi.erase(listaFi.begin());
				ordenFi.erase(ordenFi.begin());
				if (listaFi.empty())
					break;
			}
		if (!ordenDi.empty())
			while(ordenDi.at(0)<=umbralD)
			{
				ordenRestrinDi.push_back(ordenDi.at(0));
				listaRestrinDi.push_back(listaDi.at(0));
				listaDi.erase(listaDi.begin());
				ordenDi.erase(ordenDi.begin());
				if (listaDi.empty())
					break;
			}
		fr = rd->Randint(0, listaRestrinFi.size()-1);
		dr = rd->Randint(0, listaRestrinDi.size()-1);
		orden[listaRestrinFi.at(fr)]=listaRestrinDi.at(dr);
		nAnadidos++;
		listaRestrinFi.erase(listaRestrinFi.begin()+fr);
		listaRestrinDi.erase(listaRestrinDi.begin()+dr);
		ordenRestrinFi.erase(ordenRestrinFi.begin()+fr);
		ordenRestrinDi.erase(ordenRestrinDi.begin()+dr);
		if(fase1)
		{
			fr = rd->Randint(0, listaRestrinFi.size()-1);
			dr = rd->Randint(0, listaRestrinDi.size()-1);
			orden[listaRestrinFi.at(fr)]=listaRestrinDi.at(dr);
			nAnadidos++;
			listaRestrinFi.erase(listaRestrinFi.begin()+fr);
			listaRestrinDi.erase(listaRestrinDi.begin()+dr);
			ordenRestrinFi.erase(ordenRestrinFi.begin()+fr);
			ordenRestrinDi.erase(ordenRestrinDi.begin()+dr);
			fase1 = false;
		}
	}while(nAnadidos<tamano);

	for (int i = 0; i < tamano; ++i)
	{
		for (int j = 0; j < tamano; ++j)
		{
			if(problema->getFlujo(i,j)>0)
			{
				coste += problema->getFlujo(i,j)*problema->getDistancia(orden[i],orden[j]);
			}
		}
		//cout << i << '-' << orden[i]<<endl;
	}
}

void Solucion::solAleatoria(Random *rd)
{
	bool *uniAsignadas;
	uniAsignadas = new bool[tamano];
	for (int i = 0; i < tamano; ++i)
		uniAsignadas[i]=false;
	int unidad=0;
	do
	{
		int localizacion = rd->Randint(0, tamano-1);
		if(!uniAsignadas[localizacion])
		{
			orden[unidad]=localizacion;
			unidad++;
			uniAsignadas[localizacion]=true;
		}
	}while(unidad<tamano);

	for (int i = 0; i < tamano; ++i)
	{
		for (int j = 0; j < tamano; ++j)
		{
			if(problema->getFlujo(i,j)>0)
			{
				coste += problema->getFlujo(i,j)*problema->getDistancia(orden[i],orden[j]);
			}
		}
		//cout << i << '-' << orden[i]<<endl;
	}
	delete [] uniAsignadas;
	//cout << coste << endl;
}

bool Solucion::busquedaLocalPVM(int &evaluaciones, int limEva)
{
	bool cambio = false;
	bool *vetados = new bool[tamano];
	for (int i = 0; i < tamano && evaluaciones < limEva; ++i)
		if(!vetados[i])
		{
			for (int j = 0; j < tamano && evaluaciones < limEva; ++j)
			{
				double diff=diferenciaCoste(i,j);
				evaluaciones++;
				if (diff<0)
				{
					hacerCambio(i,j);
					cambio = true;
					vetados[i] = false;
					vetados[j] = false;
				}
			}
			if (!cambio)
				vetados[i]=true;
		}
	delete [] vetados;
	return cambio;
}

string Solucion::mostrarSolucion()
{
	string salida="";
	for (int i = 0; i < tamano; ++i)
	{
		string is=to_string(i), ds=to_string(orden[i]);
		salida += "Unidad " + is + " asignada en " + ds+'\n';
	}
	string costeS = to_string(coste);
	salida += /*"Coste "+*/costeS;
	return salida;
}

double Solucion::diferenciaCoste(int unidad, int otraUni)
{
	//r=unidad s=otraUni π(r)=ubica π(s)=otraUbi
	int ubica=orden[unidad], otraUbi=orden[otraUni];
	double diferencial=0.0;
	for (int k = 0; k < tamano; ++k)
	{
		if (k!=unidad&&k!=otraUni)
			diferencial += problema->getFlujo(unidad, k)*(problema->getDistancia(otraUbi,orden[k])-problema->getDistancia(ubica, orden[k]))
			+ problema->getFlujo(otraUni, k)*(problema->getDistancia(ubica, orden[k])-problema->getDistancia(otraUbi, orden[k]))
			+ problema->getFlujo(k, unidad)*(problema->getDistancia(orden[k], otraUbi)-problema->getDistancia(orden[k], ubica))
			+ problema->getFlujo(k, otraUni)*(problema->getDistancia(orden[k], ubica)-problema->getDistancia(orden[k], otraUbi));
	}
	return diferencial;
}

void Solucion::hacerCambio(int unidad, int otraUni)
{
	coste += diferenciaCoste(unidad, otraUni);
	int aux = orden[unidad];
	orden[unidad]=orden[otraUni];
	orden[otraUni]=aux;
}

Solucion* Solucion::cruzarSolucionesBP(Solucion *padre, Random *rd)
{
	Solucion *hijo = new Solucion(this->problema);
	vector<int> restos;
	for (int i = 0; i < tamano; ++i)
	{
		if(this->orden[i]==padre->orden[i])
			hijo->orden[i]=this->orden[i];
		else
		{
			hijo->orden[i]=-1;
			restos.push_back(padre->orden[i]);
		}
	}
	for (int i = 0; i < tamano; ++i)
	{
		if (hijo->orden[i]==-1)
		{
			int insertar = rd->Randint(0, restos.size()-1);
			hijo->orden[i]=restos.at(insertar);
			restos.erase(restos.begin()+insertar);
		}
	}
	restos.clear();
	restos.resize(0);
	for (int i = 0; i < hijo->tamano; ++i)
		for (int j = 0; j < hijo->tamano; ++j)
			if(problema->getFlujo(i,j)>0)
				hijo->coste += problema->getFlujo(i,j)*problema->getDistancia(hijo->orden[i],hijo->orden[j]);
	return hijo;
}

pair<Solucion*, Solucion*> Solucion::cruzarSolucionesPMX(Solucion* padre)
{
	Solucion *hijo1 = new Solucion(padre->problema), *hijo2 = new Solucion(padre->problema);
	vector<pair<int, int> > correspondencias;
	int iniCentral = tamano/4, finCentral = (3*tamano)/4;

	for (int i = iniCentral; i < finCentral; ++i)
	{
		int a = this->orden[i];
		int b = padre->orden[i];
		pair<int, int> correspondencia = make_pair(a,b);
		hijo1->orden[i]=b;
		hijo2->orden[i]=a;
		correspondencias.push_back(correspondencia);
	}

	for (int i = 0; i < this->tamano; ++i)
	{
		//Hijo 1
		bool correspondeFirst=false, correspondeSecond=false, reiniciar;
		pair<int, int> correspondencia;
		int nuevo = this->orden[i];
		do
		{
			reiniciar=false;
			for (int j = 0; !reiniciar && j < correspondencias.size(); ++j)
			{
				correspondencia=correspondencias.at(j);
				if (correspondencia.second==nuevo)
				{
					reiniciar=true;
					nuevo=correspondencia.first;
				}
			}
		}
		while(reiniciar);

		hijo1->orden[i]=nuevo;

		//hijo 2
		nuevo = padre->orden[i];
		do
		{
			reiniciar = false;
			for (int j = 0; !reiniciar && j < correspondencias.size(); ++j)
			{
				correspondencia=correspondencias.at(j);
				if (correspondencia.first==nuevo)
				{
					reiniciar=true;
					nuevo=correspondencia.second;
				}
			}
		}while(reiniciar);

		hijo2->orden[i]=nuevo;

		//Ajuste para no tocar zona central
		if (!(i < iniCentral-1)&& i < finCentral)
		{
			i = finCentral-1;
		}
	}

	for (int i = 0; i < tamano; ++i)
		for (int j = 0; j < tamano; ++j)
			{
				if(problema->getFlujo(i,j)>0)
				{
					hijo1->coste += hijo1->problema->getFlujo(i,j)*hijo1->problema->getDistancia(hijo1->orden[i],hijo1->orden[j]);
					hijo2->coste += hijo2->problema->getFlujo(i,j)*hijo2->problema->getDistancia(hijo2->orden[i],hijo2->orden[j]);
				}
				//cout << i << '-' << orden[i]<<endl;
			}

	pair<Solucion*, Solucion*> hijos = make_pair(hijo1, hijo2);
	return hijos;
}

void Solucion::mutar(Random *rd)
{
	int primero = rd->Randint(0,tamano-1), segundo;
	do
	{
		segundo = rd->Randint(0,tamano-1);
	}while(primero==segundo);
	hacerCambio(primero,segundo);
}
void Solucion::mutacionIls(Random *rd, double limite)
{
	int limist = tamano*limite, init = rd->Randint(0, tamano*(1-limite));
	vector<int> d;
	for (int i = 0; i < limist; ++i)
	{
		d.push_back(orden[init+i]);
	}
	random_shuffle(d.begin(), d.end());
	for (int i = 0; i <  limist; ++i)
	{
		orden [init+i]=d[i]; 
	}
	coste = 0;
	for (int i = 0; i < tamano; ++i)
	{
		for (int j = 0; j < tamano; ++j)
		{
			if(problema->getFlujo(i,j)>0)
			{
				coste += problema->getFlujo(i,j)*problema->getDistancia(orden[i],orden[j]);
			}
		}
		//cout << i << '-' << orden[i]<<endl;
	}
}

Solucion::~Solucion()
{
	delete [] orden;
}