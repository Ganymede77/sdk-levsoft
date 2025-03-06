#include <ctime>
#include <cmath>
#include <uniformdeviate.h>


// Construtor do gerador uniforme: a semente eh inicializada com o
// numero de segundos desde 1/1/1970
UniformDeviate::UniformDeviate()
{
	time_t sec_since70;
	time(&sec_since70);
    seed = 123456;//sec_since70;
    present_seed = seed;
}


UniformDeviate::~UniformDeviate() { }


/*
 *Generate: metodo para gerar n�meros aleat�rios minimal de Park e Miller
 * com a modifica��o de Bays-Duham (shuffle e added safeguards).
 *
 *- Retorna uma distribui��o aleat�ria uniformentre 0.0 e 1.0
 * (excluindo os extremos);
 *- idum deve ser inicializada com um inteiro negativo, e nao deve
 * ser alterada na gera��o de uma sequ�ncia aleatr�ria;
 *- RNMX aproxima o maior decimal menor que 1;
 *- esse gerador passa em todos os testes estat�sticos conhecidos e
 * tem per�odo 1.0e+8 aprox. 100.000.000 amostras;
 * (Numerical Recipes in C, p�g. 280, 2a ed.)
 */
double UniformDeviate::Generate()
{
	const long IA		= 16807;
	const long IM		= 2147483647;
	const long IQ		= 127773;
	const long IR		= 2836;
	const long NTAB		= 32;
	const long NDIV		= (1+(IM-1)/NTAB);
	const double AM		= 1.0 / IM;
	const double EPS	= 1.2e-14; // originalmente precisao simples: e-7
	const double RNMX	= 1.0 - EPS;
	int j;
	long k;
	static long iy=0;
	static long iv[NTAB];
	double temp;

	if (present_seed <= 0) {
		if (-(present_seed) < 1)
			present_seed = 1;
		else
			present_seed = -(present_seed);
	}

	if (!iy) {
		for (j=NTAB+7; j>=0;j--) {
			k=(present_seed) / IQ;
			present_seed = IA * (present_seed - k*IQ) - IR*k;
			if (present_seed < 0)
				present_seed +=IM;
			if (j < NTAB)
				iv[j] = present_seed;
		}
		iy = iv[0];
	}

	k = (present_seed) / IQ;
	present_seed = IA * (present_seed - k*IQ) - IR * k;
	if (present_seed < 0)
		present_seed +=IM;
	j = iy / NDIV;
	iy = iv[j];
	iv[j] = present_seed;
	if ((temp = AM*iy) > RNMX)
		return RNMX;
	else
		return temp;
}
