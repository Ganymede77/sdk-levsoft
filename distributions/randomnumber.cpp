#include <randomnumber.h>


RandomNumber::RandomNumber() { }


RandomNumber::~RandomNumber() { }


// Metodo para alterar a semente
void RandomNumber::SetSeed(long semente)
{
	seed = semente;
	present_seed = seed;
}


// Recupera a semente original
long RandomNumber::GetSeed()
{
	return(seed);
}
