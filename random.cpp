#include "random.h"
#include <stdlib.h>
#include <math.h>

double ranf(void)
//returns random number between 0 and 1
	{
	return rand()/(double)(RAND_MAX-1);
	}

double randn(double average, double stdev)
	{
	double x1,x2,w;
	do
		{
		x1=2*ranf()-1;
		x2=2*ranf()-1;
		w=x1*x1+x2*x2;
		} while (w>=1);
	w=sqrt(-2*log(w)/w);

	return average+stdev*x1*w;
	}

double d_random(double low, double high) {return low+(high-low)*drand48();}

int i_random(int low, int high)
/*
 * returns (almost) random integer in range [low,high]
 * aside from the pseudo-randomness of the generating function
 * the modulus means that lower numbers have a slightly greater probability of being chosen
 */
{
	int r=rand()%(high-low+1);
	return r+low;
}

int SelectEpsilonGreedy(const dvector& vec, double epsilon)
{
	if( drand48() < (1.0 - epsilon) )
	{
		return vec.IndexOfMax();
	}
	else
		return i_random(0, vec.Length()-1);
}

