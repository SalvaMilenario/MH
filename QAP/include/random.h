#include <cmath>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <iostream>


/***** GENERADOR DE NUMEROS PSEUDOALETORIOS *****/

/*******************************************************************/
/*  Rand genera un numero real pseudoaleatorio entre 0 y 1,        */
/*  excluyendo el 1.						   */
/*  Randint genera un numero entero entre low y high, ambos 	   */
/*  incluidos.	   						   */
/*  Randfloat genera un numero real entre low y high, incluido low */
/*  y no incluido high                                             */
/*******************************************************************/

class Random
{
private:
	/* used in random number generator below */
	const int MASK = 2147483647;
	const int PRIME = 65539;
	const double SCALE = 0.4656612875e-9;
	int Seed;

public:
	Random(int seed)
	{
		this->Seed = seed;
	}
	double Rand()
	{
		Seed = ( (Seed * PRIME) & MASK);
		return Seed * SCALE;
	}
	int Randint(int low, int high)
	{
		int rd=(low + (high-(low)+1) * Rand());
		return rd;
	}
	float Randfloat(float low, float high)
	{
		float rd= (low + (high-(low))*Rand());
		return rd;
	}
	~Random()
	{
		
	}
};
