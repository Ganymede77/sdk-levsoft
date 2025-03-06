//////////////////////////
//BoundCond.h
////////////////////////

// *NMA*: Classe de condições de contorno simplificada,
//			considerando, sem preocupaçòes, as condições
//			de contorno: Dirichlet, float e periódicas

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
	int						ref_node;		// nó de referência (float) 
											//  e nó de referência da outra 
											//  primitiva para periódicas (preciso dos nós para saber o sentido de atribuição)

	int						ref_prim;		//primitiva de referência (periódicas)
	
	int						ref_node_local; // nó de referência da própria primitiva para periódicas
	

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
        //*NMA*: 25/01/2013: retirado CBoundCond:: da declaração dos operadores porque o compilador reclamou
	bool operator == (const CBoundCond &a);//bool CBoundCond::operator == (const CBoundCond &a);

	bool operator != (const CBoundCond &a);//bool CBoundCond::operator != (const CBoundCond &a);

	CBoundCond& operator = (const CBoundCond &a);//CBoundCond& CBoundCond::operator = (const CBoundCond &a);

	//...................... Gravação e Leitura ............................................
	int Save(FILE *parq);//int CBoundCond::Save(FILE *parq);

	int Read(FILE *parq);//int CBoundCond::Read(FILE *parq);
	
};

#endif