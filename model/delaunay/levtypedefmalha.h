#ifndef __LEVTYPEDEFMALHA_H__
#define __LEVTYPEDEFMALHA_H__

#include <vector>
#include "ponto.hpp"

//===================================================================================================
//	Arquivo:			LevDef.h
//...................................................................................................
//	Descrição:			Contém os typedef utilizados pelo gerador de malha
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						Angelo Passaro
//...................................................................................................
//	Observações:
//...................................................................................................
//	Data:				Junho/2000
//	Modificado em:
//	Modificado por:
//	Motivo:
//===================================================================================================
//using namespace std;

class Pt_Malha;

class Aresta_Geometria;

class Elemento_2D;

typedef std::vector<Pt_Malha> 			vCLevPtMalha;

typedef	std::vector<Aresta_Geometria>	vArestaGeometria;// Vector STL para armazenar arestas de elementos

typedef	std::vector<Elemento_2D>		vElemento2D;// Vector STL para armazenar elementos



#endif
