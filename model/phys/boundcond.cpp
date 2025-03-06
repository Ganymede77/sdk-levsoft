// *NMA*: Classe de condições de contorno simplificada,
//			considerando, sem preocupaçòes, as condições
//			de contorno: Dirichlet, float e periódicas


#include "boundcond.h"


CBoundCond::CBoundCond()
{	
	Reinitialize();
}


CBoundCond::~CBoundCond()
{ }


void CBoundCond::Reinitialize (void)
{ 
	type			=  INVALIDBC;
	value			=  0;
	ref_node		= -1;
	ref_prim		= -1;
	ref_node_local	= -1;
	neumannValue	=  0;
}


void CBoundCond::Type(BOUNDARYCONDITIONTYPE t) {type = t;}

BOUNDARYCONDITIONTYPE CBoundCond::Type(void) { return type;}

void CBoundCond::Value(double v) { value = v;}

double	CBoundCond::Value(void)		{ return value;}

void CBoundCond::RefNode(int n)	{ ref_node = n;}
int CBoundCond::RefNode(void)	{ return ref_node;}

void CBoundCond::RefNodeLocal(int n)	{ ref_node_local = n;}
int CBoundCond::RefNodeLocal(void)	{ return ref_node_local;}
	
void CBoundCond::RefPrim(int p)	{ ref_prim = p;}
int CBoundCond::RefPrim(void)	{ return ref_prim;}

//............ operadores .............................................
bool CBoundCond::operator == (const CBoundCond &a)
{
		bool ret = false;

		if(type == a.type)
			if(value == a.value)
				if(ref_node == a.ref_node)
					if(ref_node_local == a.ref_node_local)
						if(neumannValue == a.neumannValue)
							//if(neumannPoint == a.neumannPoint)
							{
								ret = true;
							}
		return ret;
}

bool CBoundCond::operator != (const CBoundCond &a)
{
		bool ret = true;

		if(type == a.type)
			if(value == a.value)
				if(ref_node == a.ref_node)
					if(ref_node_local == a.ref_node_local)
						if(neumannValue == a.neumannValue)
							//if(neumannPoint == a.neumannPoint)
							{
								ret = false;
							}
		return ret;
}

CBoundCond& CBoundCond::operator = (const CBoundCond &a)
{
		type			= a.type;
		value			= a.value;
		ref_node		= a.ref_node;
		ref_node_local	= a.ref_node_local;
		neumannValue	= a.neumannValue;
		//neumannPoint	= a.neumannPoint;
		return *this;
}

	//...................... Gravação e Leitura ............................................
int CBoundCond::Save(FILE *parq)
{
		int retorno = 0;
		int tipo;

		tipo = (enum BOUNDARYCONDITIONTYPE) type;

		fprintf(parq,"\n\n%d",tipo);
		fprintf(parq,"\n%d",ref_node);
		fprintf(parq,"\n%d",ref_node_local);
		fprintf(parq,"\n%d",ref_prim);
		fprintf(parq,"\n%lf",value);

		return retorno;
}

int CBoundCond::Read(FILE *parq)
{
		int retorno = 0;
		int tipo;

		fscanf(parq,"\n\n%d",&tipo);
		fscanf(parq,"\n%d",&ref_node);
		fscanf(parq,"\n%d",&ref_node_local);
		fscanf(parq,"\n%d",&ref_prim);
		fscanf(parq,"\n%lf",&value);

		type = (enum BOUNDARYCONDITIONTYPE) tipo;

		return retorno;
}

