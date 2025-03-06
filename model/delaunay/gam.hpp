#ifndef _GAM_
#define _GAM_


#include <levdef.h>
#include <lo_templ.hpp>


extern double SIGMA;


class Aresta_Geometria; // *NMA*: o compilador nao estava reconhecendo
                        // face == aresta, que nao estava declarada na classe
                        // FACE, para que nao ocorra erro, coloquei esta
                        // declaracao

class FACE
{
	public:

		lev_math::Uchar Valido;     // Validade do elemento
		long int V1, V2, V3;        // Indice dos vertices que compoem as faces
		friend bool operator == (const FACE &, const FACE &);
		friend bool operator == (FACE const&,  Aresta_Geometria const&);
} ;


#endif
