#ifndef GAUSSIANDEVIATE_H
#define GAUSSIANDEVIATE_H


#include <randomnumber.h>
#include <uniformdeviate.h>


class GaussianDeviate : public RandomNumber 
{

public:    
	GaussianDeviate();
	~GaussianDeviate();
	double Generate();
	void SetMean_Deviation( double media, double desvio);

private:    
	double mean;
    double standard_deviation;
    UniformDeviate  p_uniformsample; //amostra de distribuicao uniforme
};

#endif // GAUSSIANDEVIATE_H
