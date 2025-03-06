//////////////////////////
//BoundCond.h
////////////////////////

// *NMA*: Classe de condi��es de contorno simplificada,
//			considerando, sem preocupa��es, as condi��es
//			de contorno: Dirichlet, float e peri�dicas

//Class declaration

#ifndef __BOUNDCOND_H__
#define __BOUNDCOND_H__

#include "levenum.h"
#include "levpoint.h"
#include <cstdio>

class CBoundCond
{
	
protected:

	BOUNDARYCONDITIONTYPE	type;
	double					value;			// valor no caso Dirichlet
	int						ref_node;		// n� de refer�ncia (float) 
											//  e n� de refer�ncia da outra 
											//  primitiva para peri�dicas (preciso dos n�s para saber o sentido de atribui��o)

	int						ref_prim;		//primitiva de refer�ncia (peri�dicas)
	
	int						ref_node_local; // n� de refer�ncia da pr�pria primitiva para peri�dicas
	

	double					neumannValue;	//	valor no caso Newmann
	CLevPoint				neumannPoint;	//  ponto no caso de Newmann


public:


	CBoundCond();
	~CBoundCond(); //Destructor


	void					Reinitialize (void);

	void					Type(BOUNDARYCONDITIONTYPE t);
	BOUNDARYCONDITIONTYPE	Type(void);

	void					Value(double v);
	double					Value(void);

//	void					NeumannValue(double v1) { neumannValue= v1;}
//	double					NeumannValue(void)		{ return neumannValue;}
	
//	void					NeumannPoint(CLevPoint point) { neumannPoint= point;}
//	CLevPoint				NeumannPoint(void)		{ return neumannPoint;}

	void					RefNode(int n);
	int						RefNode(void);

	void					RefNodeLocal(int n);
	int						RefNodeLocal(void);
	
	void					RefPrim(int p);
	int						RefPrim(void);

//............ operadores .............................................
        //*NMA*: 25/01/2013: retirado CBoundCond:: da declara��o dos operadores porque o compilador reclamou
	bool operator == (const CBoundCond &a);//bool CBoundCond::operator == (const CBoundCond &a);

	bool operator != (const CBoundCond &a);//bool CBoundCond::operator != (const CBoundCond &a);

	CBoundCond& operator = (const CBoundCond &a);//CBoundCond& CBoundCond::operator = (const CBoundCond &a);

	//...................... Grava��o e Leitura ............................................
	int Save(FILE *parq);//int CBoundCond::Save(FILE *parq);

	int Read(FILE *parq);//int CBoundCond::Read(FILE *parq);
	
};

#endif