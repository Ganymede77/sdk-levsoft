#ifndef UNIFORMDEVIATE_H
#define UNIFORMDEVIATE_H


#include <randomnumber.h>


class UniformDeviate : public RandomNumber
{

public:

	UniformDeviate();
	virtual ~UniformDeviate();
    double Generate();
};


#endif // UNIFORMDEVIATE_H

