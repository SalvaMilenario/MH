#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <climits>
#include "problema.h"
#include "solucion.h"
#include "time.h"
#include <cmath>

using namespace std;
/**
@brief Busca la mejor solucion de una poblacion de soluciones
@param Se le pasan un vector de punteros a soluciones
@return La mejor solucion del la poblacion
*/
Random *rd;

Solucion* mejorSolucion(vector <Solucion*> &poblacion) 
{
    Solucion *mejor = poblacion.at(0);
    int iMejor=0;
    for (int i = 1; i < poblacion.size(); ++i)
    {
        if (mejor->getCoste()>(poblacion.at(i))->getCoste())
        {
            mejor = poblacion.at(i);
            iMejor = i;
        }
    }
    poblacion.erase(poblacion.begin()+iMejor);
    return mejor;
}

Solucion* gredy(Problema* qap)
{
    Solucion *sg=new Solucion(qap);
    sg->gredy();
    return sg;
}

void generarPoblacion(vector <Solucion*> &poblacion, Problema *qap, int nSol)
{
    for (int i = 0; i < poblacion.size(); ++i)
    {
        delete poblacion[i];
    }
    poblacion.clear();
    poblacion.resize(0);

    for (int i = 1; i <= nSol; ++i)
    {
        int seed = 1+(i*10000);
        Solucion *solAle = new Solucion(qap);
        solAle->solAleatoria(rd);
        poblacion.push_back(solAle);
    }
}

void busquedaLocal(vector<Solucion*> &poblacion, int limEva)
{
    int evaluaciones=0;
    bool cambio;
    for (int i = 0; i < poblacion.size(); ++i)
    {
        do
        {
            cambio=(poblacion.at(i))->busquedaLocalPVM(evaluaciones, limEva);
        }while(cambio&&evaluaciones<limEva);
        evaluaciones=0;
    }
}

void busquedaLocal(vector<Solucion*> &poblacion, int limEva, int &yaEva)
{
    int evaluaciones=0;
    bool cambio;
    for (int i = 0; i < poblacion.size(); ++i)
    {
        do
        {
            cambio=(poblacion.at(i))->busquedaLocalPVM(evaluaciones, limEva);
        }while(cambio&&evaluaciones<limEva);
        yaEva+=evaluaciones;
        evaluaciones=0;
    }
}

void starTime(double &start_time){start_time=clock();}

double endTime(double &start_time)
{
    double elapsed = clock()- start_time;
    elapsed = elapsed / CLOCKS_PER_SEC;
    return elapsed;
}

double desv(vector<Solucion*> poblacion, Solucion *laOptima)
{
    double desv = 0.0;
    double coste , costeOptimo = laOptima->getCoste();
    for (int i = 0; i < poblacion.size(); ++i)
    {
        coste = (poblacion.at(i))->getCoste();
        desv += (coste-costeOptimo)/costeOptimo;
    }
    return desv*(1.0/((double)poblacion.size()));
}

vector<Solucion*> seleccionAG(vector<Solucion*> &poblacionInicial, int nSelec)
{
    vector<Solucion*> poblacionSelec;
    for (int i = 0; i < nSelec; ++i)
    {
        Solucion *s1, *s2;
        s1 = new Solucion(poblacionInicial.at(rd->Randint(0,poblacionInicial.size()-1)));
        s2 = new Solucion(poblacionInicial.at(rd->Randint(0,poblacionInicial.size()-1)));
        if (s1->getCoste()<s2->getCoste())
            poblacionSelec.push_back(s1);
        else
            poblacionSelec.push_back(s2);
    }
    return poblacionSelec;
}

vector<Solucion*> cruceBP(vector<Solucion*> &poblacion, float proC, int &evaluaciones)
{
    int nCruces = proC*(poblacion.size()/2.0);
    int inicio = (poblacion.size()/2.0)-(nCruces);
    vector<Solucion*> hijos;
    for (int i = 0; i < nCruces; i++)
    {
            Solucion *hijo=poblacion[inicio]->cruzarSolucionesBP(poblacion[inicio+1], rd);
            evaluaciones++;
            inicio+=2;
            hijos.push_back(hijo);
    }
    return hijos;
}


vector<Solucion*> crucePMX(vector<Solucion*> poblacion, float proC, int &evaluaciones)
{
    int nCruces = proC*(poblacion.size()/2.0);
    int inicio = (poblacion.size()/2.0)-(nCruces);
    vector<Solucion*>  hijos;
    for (int i = 0; i < nCruces; i++)
    {
        pair<Solucion*, Solucion*> h = poblacion[inicio]->cruzarSolucionesPMX(poblacion[inicio+1]);
        hijos.push_back(h.first);
        hijos.push_back(h.second);
        inicio+=2;
        evaluaciones+=2;
    }
    return hijos;
}

void mutacion(vector<Solucion*> &poblacion, float proM, int &evaluaciones)
{
    int nMutaciones = poblacion.size()*poblacion[0]->getTamano()*proM;
    for (int i = 0; i < nMutaciones; ++i)
    {
        poblacion[rd->Randint(0, poblacion.size()-1)]->mutar(rd);
        evaluaciones++;
    }
}

void reposicionAGE(vector<Solucion*> hijos, vector<Solucion*> &poblacion)
{
    Solucion *mala, *masMala;
    int iMala=0, iMasMala=1;
    if (poblacion.at(0)->getCoste()>poblacion.at(1)->getCoste())
    {
        mala = poblacion.at(1);
        iMala=1;
        masMala = poblacion.at(0);
        iMasMala=0;
    }
    else
    {
        mala = poblacion.at(0);
        masMala = poblacion.at(1);
    }
    for (int i = 2; i < poblacion.size(); ++i)
    {
        Solucion *solActual = poblacion.at(i);
        if (solActual->getCoste()>mala->getCoste())
        {
            if (solActual->getCoste()>masMala->getCoste())
            {
                mala=masMala;
                iMala=iMasMala;
                masMala=solActual;
                iMasMala=i;
            }
            else
            {
                mala=solActual;
                iMala=i;
            }
        }
    }
    //cual esta antes?
    if(iMala < iMasMala)
    {
        poblacion.erase(poblacion.begin()+iMala);
        iMasMala--;
        poblacion.erase(poblacion.begin()+iMasMala);
    }
    else
    {
        poblacion.erase(poblacion.begin()+iMasMala);
        iMala--;
        poblacion.erase(poblacion.begin()+iMala);
    }
    Solucion *buena=hijos.at(0), *laMejor=hijos.at(1);
    if (buena->getCoste()<laMejor->getCoste())
    {
        Solucion *aux = buena;
        buena = laMejor;
        laMejor = aux;
    }

    vector<Solucion*> orden;
    if (mala->getCoste()<laMejor->getCoste())
    {
        orden.push_back(mala);
        if (masMala->getCoste()<laMejor->getCoste())
        {
            orden.push_back(masMala);
            orden.push_back(buena);
            orden.push_back(laMejor);
        }
        else
        {
            orden.push_back(laMejor);
            orden.push_back(buena);
            orden.push_back(masMala);
        }
    }
    else
    {
        orden.push_back(laMejor);
        if (mala->getCoste()<buena->getCoste())
        {
            orden.push_back(mala);
            orden.push_back(buena);
            orden.push_back(masMala);
        }
        else
        {
            orden.push_back(buena);
            orden.push_back(mala);
            orden.push_back(masMala);
        }
    }
    for (int i = 0; i < orden.size(); ++i)
        if(i<2)
            poblacion.push_back(orden.at(i));
        else
            delete orden[i];
}
vector<Solucion*> AGG (vector<Solucion*> &poblacion, int limEva, float proC, float proM, bool esBP, int limGen, int &yaEva)
{
    vector<Solucion*> selec;
    vector<Solucion*> hijos;
    int repoblar, evaluaciones = yaEva, generaciones = 0;
    do{
        selec = seleccionAG(poblacion, poblacion.size());
        if(esBP)
            hijos = cruceBP(selec, proC, evaluaciones);
        else
            hijos = crucePMX(selec, proC, evaluaciones);
        for (int i = 0; i < selec.size(); ++i)
        {
            delete selec[i];
        }
        selec.clear();
        selec.resize(0);
        mutacion(hijos, proM, evaluaciones);
        Solucion *mejor = mejorSolucion(poblacion);
        hijos.push_back(mejor);
        repoblar = poblacion.size()-hijos.size()+1;

        for (int i = 0; i < poblacion.size(); ++i)
        {
            if (i>=repoblar)
            {
                delete poblacion[i];
            }
            else
            {
                hijos.push_back(poblacion[i]);
            }
        }
        poblacion.clear();
        poblacion.resize(0);
        poblacion = hijos;
        generaciones++;
    }while(evaluaciones < limEva&&generaciones<limGen);
    return poblacion;
}

vector<Solucion*> AGE(vector<Solucion*> &poblacion, int limEva, float proM, bool esBP)
{
    int evaluaciones = 0;
    vector<Solucion*> selec;
    vector<Solucion*> hijos;
    do{
        selec = seleccionAG(poblacion, 2);
        if(esBP)
        {
            hijos = cruceBP(selec, 1.0, evaluaciones);
            Solucion* aux = selec[0];
            selec[0] = selec[1];
            selec[1] = aux;
            hijos.push_back(cruceBP(selec, 1.0, evaluaciones)[0]);
        }
        else
            hijos = crucePMX(selec, 1.0, evaluaciones);
        for (int i = 0; i < selec.size(); ++i)
        {
            delete selec[i];
        }
        selec.clear();
        selec.resize(0);
        mutacion(hijos, proM, evaluaciones);
        reposicionAGE(hijos, poblacion);
    }while(evaluaciones < limEva);
    return poblacion;
}

void AM(vector<Solucion*> &poblacion, int limEva, float proC, float proM, float ppbl, int generacionesBL, int evaluacioneBL, bool sobreMejor)
{
    int evaluaciones=0;
    do
    {
        AGG(poblacion, limEva, proC, proM, true, 10, evaluaciones);
        if(evaluaciones<limEva)
        {
            if (ppbl==1.0)
            {
                busquedaLocal(poblacion, evaluacioneBL, evaluaciones);
            }
            else
            {
                int nBL = poblacion.size()*ppbl;
                vector<Solucion*> poblaBL;
                if (!sobreMejor)
                    for (int i = 0; i < nBL; ++i)
                    {
                        int intRd = rd->Randint(0, poblacion.size()-1);
                        Solucion *solRd = poblacion[intRd];
                        poblacion.erase(poblacion.begin()+intRd);
                        poblaBL.push_back(solRd);
                    }
                else
                {
                    for (int i = 0; i < nBL; ++i)
                        poblaBL.push_back(mejorSolucion(poblacion));
                }
                busquedaLocal(poblaBL, evaluacioneBL, evaluaciones);
                for (int i = 0; i < poblaBL.size(); ++i)
                {
                    poblacion.push_back(poblaBL[i]);
                }
            }
        }
    }while(evaluaciones<limEva);
}

Solucion* enfriamientoSimulado(Problema *qap)
{
    double t = 0.001, t0, beta, tk, difF;
    int max_vecinos = 10*qap->getTamano(), max_exitos = qap->getTamano(), k = 1, contExitos, eva=0;
    Solucion *sol, *bestSolucion;
    do
    {
        sol = new Solucion(qap);
        sol->gredy();
        t0=(0.3*sol->getCoste())/-(log(0.3));
    }while(t>=t0);
    beta = (t0-t)/(50000*t0*t);
    tk = t0;
    bestSolucion = new Solucion(sol);
    do
    {
        contExitos=0;
        for(int contVecinos = 0; contVecinos<max_vecinos&&contExitos<max_exitos;contVecinos++)
        {
            Solucion *difSol = new Solucion(sol);
            difSol->mutar(rd);// este operador canbia la soluciona una vecina.
            eva++;
            difF = difSol->getCoste()-sol->getCoste();
            if (difF<0||rd->Rand()<=exp((-1*difF)/(k*tk)))
            {
                delete sol;
                sol = difSol;
                contExitos++;
                if(sol->getCoste()<bestSolucion->getCoste())
                {
                    delete bestSolucion;
                    bestSolucion=new Solucion(sol);
                }
            }
        }
        k++;
        tk = tk/(1.0+(beta*tk));
    }while(tk>t&&eva<50000);
    return bestSolucion;    
}

Solucion* enfriamientoSimuladoILS(Problema*qap, Solucion *solEnfriar)
{
    double t = 0.001, t0, beta, tk, difF;
    int max_vecinos = 10*qap->getTamano(), max_exitos = qap->getTamano(), k = 1, contExitos, eva=0;
    Solucion *sol, *bestSolucion;
    t0=(0.3*solEnfriar->getCoste())/-(log(0.3));
    beta = (t0-t)/(50000*t0*t);
    tk = t0;
    bestSolucion = new Solucion(solEnfriar);
    sol = new Solucion(solEnfriar);
    do
    {
        contExitos=0;
        for(int contVecinos = 0; contVecinos<max_vecinos&&contExitos<max_exitos;contVecinos++)
        {
            Solucion *difSol = new Solucion(sol);
            difSol->mutar(rd);// este operador canbia la soluciona una vecina.
            eva++;
            difF = difSol->getCoste()-sol->getCoste();
            if (difF<0||rd->Rand()<=exp((-1*difF)/(k*tk)))
            {
                delete sol;
                sol = difSol;
                contExitos++;
                if(sol->getCoste()<bestSolucion->getCoste())
                {
                    delete bestSolucion;
                    bestSolucion=new Solucion(sol);
                }
            }
        }
        k++;
        tk = tk/(1.0+(beta*tk));
    }while(tk>t&&eva<50000);
    return bestSolucion;    
}

Solucion* multiArranque(Problema *qap)
{
    vector <Solucion*> poblacion;
    generarPoblacion(poblacion, qap, 25);
    busquedaLocal(poblacion, 50000);
    return mejorSolucion(poblacion);
}

Solucion* grasp(Problema *qap)
{
    vector <Solucion*> poblacion;
    Solucion *sol;
    for (int i = 0; i < 25; ++i)
    {
        sol = new Solucion(qap);
        sol->gredyAleatorio(rd, 0.3);
        poblacion.push_back(sol);
    }
    busquedaLocal(poblacion, 50000);
    return mejorSolucion(poblacion);
}

Solucion* ils(Problema* qap, bool enSim)
{
    vector <Solucion*> poblacion;
    Solucion *sol = new Solucion(qap), *bestSolucion;
    sol->solAleatoria(rd);
    bestSolucion = new Solucion(sol);
    for (int i = 0; i < 25; ++i)
    {
        if(enSim)
        {
            sol = enfriamientoSimuladoILS(qap, sol);
        }
        else
        {
            poblacion.push_back(sol);
            busquedaLocal(poblacion, 50000);
            sol = poblacion.at(0);
            poblacion.clear();
        }
        if(bestSolucion->getCoste()>sol->getCoste())
        {
            delete bestSolucion;
            bestSolucion = new Solucion(sol);
        }
        else
        {
            delete sol;
            sol = new Solucion(bestSolucion);
        }
        sol->mutacionIls(rd ,0.25);
    }
    return bestSolucion;
}

int main(int argc, char** argv) 
{
    cout << fixed;
    double start_time;
    double end_time;
    vector <Solucion*> poblacion;
    if (argc == 4)
        rd = new Random(atoi(argv[3])*170417);
    else if (argc == 3)
        rd = new Random(clock());
    else
    {
        cout << "Numero insuficiente de argumentos ./qap [problema] [solucion_optima_conocida] [semilla del Random]";
        return -1;
    }
        
    int intParaNada=0;
    //creo, guardo el problema y la solucion optima
    Problema *qap;
    qap = new Problema((argv[1]));
    Solucion *laOptima = new Solucion(qap, argv[2]);
    
    //gredy
    starTime(start_time);
    Solucion *solGre= gredy(qap);    
    end_time = endTime(start_time);

    poblacion.push_back(solGre);
    cout << "Gredy\t" << solGre->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;
    
    // Busqueda local
    generarPoblacion(poblacion, qap, 1);
    starTime(start_time);
    busquedaLocal(poblacion, 50000);
    end_time = endTime(start_time);
    cout << "BL\t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;
    
    
    //AGG-BP
    generarPoblacion(poblacion, qap, 50);
    starTime(start_time);
    AGG(poblacion, 50000, 0.7, 0.001, true, INT_MAX, intParaNada);
    end_time = endTime(start_time);
    cout << "AGG-BP\t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;
    
    //AGG-PMX
    intParaNada=0;
    generarPoblacion(poblacion, qap, 50);;
    starTime(start_time);
    AGG(poblacion, 50000, 0.7, 0.001, false, INT_MAX, intParaNada);
    end_time = endTime(start_time);
    cout << "AGG-PMX\t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;

    //AGE-BP
    generarPoblacion(poblacion, qap, 50);
    starTime(start_time);
    AGE(poblacion, 50000, 0.001, true);
    end_time = endTime(start_time);
    cout << "AGE-BP\t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;

    //AGE-PMX
    generarPoblacion(poblacion, qap, 50);
    starTime(start_time);
    AGE(poblacion, 50000, 0.001, false);
    end_time = endTime(start_time);
    cout << "AGE-PMX\t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;

    generarPoblacion(poblacion, qap, 10);
    starTime(start_time);
    AM(poblacion, 50000, 0.7, 0.001, 1.0, 10, 400, false);
    end_time = endTime(start_time);
    cout << "AM-(10,1.0)\t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;

    generarPoblacion(poblacion, qap, 10);
    starTime(start_time);
    AM(poblacion, 50000, 0.7, 0.001, 0.1, 10, 400, false);
    end_time = endTime(start_time);
    cout << "AM-(10,0.1)\t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;

    generarPoblacion(poblacion, qap, 10);
    starTime(start_time);
    AM(poblacion, 50000, 0.7, 0.001, 0.1, 10, 400, true);
    end_time = endTime(start_time);
    cout << "AM-(10,0.1mej)\t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;
    

    starTime(start_time);
    poblacion.push_back(enfriamientoSimulado(qap));
    end_time = endTime(start_time);
    cout << "Enfriamiento\t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;
    
    
    starTime(start_time);
    poblacion.push_back(multiArranque(qap));
    end_time = endTime(start_time);
    cout << "Multiarranque\t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;
    
    starTime(start_time);
    poblacion.push_back(grasp(qap));
    end_time = endTime(start_time);
    cout << "Grasp \t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;
    
    starTime(start_time);
    poblacion.push_back(ils(qap, false));
    end_time = endTime(start_time);
    cout << "ILS \t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;

    starTime(start_time);
    poblacion.push_back(ils(qap, true));
    end_time = endTime(start_time);
    cout << "ILS-ES\t" << mejorSolucion(poblacion)->getCoste() << "\t" << end_time << "\t" << desv(poblacion, laOptima)<<endl;


    delete qap;
    return 0;
}