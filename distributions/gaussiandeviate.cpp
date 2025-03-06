#include <gaussiandeviate.h>
#include <cmath>
#include <cstdlib>


GaussianDeviate::GaussianDeviate()
{
	mean = 0.0;
	standard_deviation = 1.0;
	//amostra uniforme usada para gerar o numero gaussiano
	p_uniformsample.SetSeed(p_uniformsample.GetSeed()/1000);
}


GaussianDeviate::~GaussianDeviate()
{ }


// Gerador de numeros aleatorios gaussianos: usa o gerador de numeros uniformes e gera dois
// numeros gaussianos; um eh devolvido e o outro eh armazenado e retornado na chamada
// seguinte. Este controle eh feito pela flag iset;
double GaussianDeviate::Generate()
{
 	static int iset = 0;
	static double gset, hset;
	double fac, rsq, v1, v2;
	
	if(iset == 0) {
		do {
			v1 = 2.0 * p_uniformsample.Generate() - 1.0;
			v2 = 2.0 * p_uniformsample.Generate() - 1.0;
			rsq = v1 * v1 + v2 * v2;
		} while (rsq>= 1 || rsq == 0);
		fac = sqrt(-2.0 * log(rsq)/rsq);
		gset = (v1 * fac);
		hset = (v2 * fac) * standard_deviation + mean;
		iset = 1;
		return hset;
	} else {
		iset = 0;
		gset = (gset * standard_deviation) + mean;
		return gset;
	}
}


void GaussianDeviate::SetMean_Deviation(double media, double desvio)
{
	mean = media;
	standard_deviation = desvio;
}
