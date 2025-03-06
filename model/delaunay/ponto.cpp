//****************************************************************************

// P R O J E T O      G R O U N D - 3 D

//============================================================================


// Descricao  : Classes de pontos. Cada classe esta definida em funcao da
//              sua area de aplicacao : GAM, SOLVER e EXPLORADOR

#include "ponto.hpp"
#include <stdio.h>
#include <memory.h>
#include <iostream>//#include <iostream.h>
//***************************************************************************
//***************************************************************************
//
//                    C L A S S     P O N T O
//
//***************************************************************************
//***************************************************************************
#if 0   // eliminado com a substituição por CLevPoint
Ponto :: Ponto() { X=Y=Z=0;}
//____________________________________________________________________________

Ponto :: ~Ponto () {}
//____________________________________________________________________________

//===========================================================================
//               class Ponto :: Get_X
//===========================================================================
//
//  Descricao          : informa a coordenada X do ponto
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 23/04/94
//
//===========================================================================

//float Ponto :: Get_X(void) {return X;}


//===========================================================================
//               class Ponto :: Get_Y
//===========================================================================
//
//  Descricao          : informa a coordenada Y do ponto
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 23/04/94
//
//===========================================================================

//float Ponto :: Get_Y(void) {return Y;}


//===========================================================================
//               class Ponto :: Get_Z
//===========================================================================
//
//  Descricao          : informa a coordenada Z do ponto
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 23/04/94
//
//===========================================================================

//float Ponto :: Get_Z (void) {return Z;}

//===========================================================================
//               class Ponto :: Modifica
//===========================================================================
//
//  Descricao          : Modifica as coordenadas (x,y,z) do ponto
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 14/07/94
//
//===========================================================================

void Ponto :: Modifica (double xn, double yn, double zn) {	X=xn; Y=yn; Z=zn;}

//===========================================================================
//               class Ponto :: operator =  / operator ==
//===========================================================================

Ponto & Ponto :: operator =(Ponto & p) {	X = p.X;	Y = p.Y;	Z = p.Z; return *this;}

char    Ponto :: operator==(Ponto & p)
						{ 	if   (X == p.X && Y == p.Y && Z == p.Z) return YES;
							else return NO;
						}

//===========================================================================
//               class Ponto :: Set_Coord
//===========================================================================
//
//  Descricao          : atribui as coordenadas do ponto
//
//  parametros         : coordenadas X,Y e Z do ponto
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 23/04/94
//
//===========================================================================

void  Ponto :: Set_Coord (double x, double y)
{X = x; Y = y; Z = 0; }

#endif  // eliminado com a substituição por CLevPoint


//***************************************************************************
//***************************************************************************
//
//                    C L A S S     P O N T O _ B A S E
//
//***************************************************************************
//***************************************************************************

//Ponto_Base :: Ponto_Base() : Ponto() {tipo = 2;}//GEOM
Ponto_Base :: Ponto_Base() : CLevPoint() {tipo = GEOM;}//GEOM
//____________________________________________________________________________

Ponto_Base :: ~Ponto_Base () { }
//____________________________________________________________________________

//===========================================================================
//               class Ponto_Base :: Get_Tipo, Set_Tipo
//===========================================================================
//
//  Descricao          : Recupera e define o tipo do ponto :
//                                                  0 - geom
//                                                  1 - pre-discr
//                                                  2 - refinam.
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 19/08/94
//
//===========================================================================

lev_math::Uchar Ponto_Base :: Get_Tipo (void)      {return tipo;}
void  Ponto_Base :: Set_Tipo (lev_math::Uchar tip) { tipo = tip;}
Ponto_Base & Ponto_Base :: operator= (Ponto_Base & p)
{ x = p.x; x = p.x; z = p.z; tipo = p.tipo; return *this;}

char  Ponto_Base :: operator==(Ponto_Base & p)
						{ 	if   (x == p.x && y == p.y && z == p.z && tipo == p.tipo) return lev_enum::YES;
							else return lev_enum::NO;
						}


//***************************************************************************
//***************************************************************************
//
//                    C L A S S     P T _ M A L H A
//
//***************************************************************************
//***************************************************************************

Pt_Malha :: Pt_Malha()  :  Ponto_Base()
{
	Peso=0;
	Num_Geometrias=0;
	num_materiais_derivadas = num_fontes = num_cc = num_potenciais =0;
	TipoCC=0;
	ValorCC=0;
	Potencial=0;
	memset(material,	0, COISAS_POR_NO*sizeof(long));
	memset(DerivadaPrim,0, COISAS_POR_NO*sizeof(double));
	memset(Fontes,		0, COISAS_POR_NO*sizeof(double));
}
//____________________________________________________________________________

Pt_Malha :: ~Pt_Malha () {}
//____________________________________________________________________________

//===========================================================================
//               class Pt_Malha :: Get_TipoCC
//===========================================================================
//
//  Descricao          : Recupera o tipo de condicao de contorno associada ao
//                       ponto. Esta informacao esta armazenada no atributo
//                       TipoCC.
//
//  versao             : 1.1
//  programador        : Angelo Passaro
//  ultima modificacao : 12/04/2001
//  por: Guilherme Moreira Magalhães
//===========================================================================

short int Pt_Malha :: Get_TipoCC  (void) {return TipoCC;}

//===========================================================================
//               class Pt_Malha :: Get_Peso
//===========================================================================
//
//  Descricao          : Recupera o peso associado ao ponto
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 14/07/94
//
//===========================================================================

double Pt_Malha :: Get_Peso  (void) {return Peso;}

//===========================================================================
//               class Pt_Malha :: Set_TipoCC
//===========================================================================
//
//  Descricao          : Associa um tipo de condicao de contorno ao ponto
//                       ponto. Esta informacao e armazenada no atributo
//                       TipoCC.
//
//  versao             : 1.1
//  programador        : Angelo Passaro
//  ultima modificacao : 11/04/2001
//  por: Guilherme Moreira Magalhães
//===========================================================================

void Pt_Malha :: Set_TipoCC  (short int tipocc) {TipoCC = tipocc;}


//===========================================================================
//               class Pt_Malha :: Set_Peso
//===========================================================================
//
//  Descricao          : Define o peso no ponto, de acordo com o tipo de
//						 operação. Se a operação for o ajuste dos pesos de
//						 pontos pertencentes a geometria é do tipo 1.
//
//	Parametros		   : Pes - peso do ponto
//						 tipo - (1) caso o tipo seja igual a 1
//						 o tratamento do ponto será para calculo de peso
//						 de acordo com o comprimento e o número de arestas
//						 de geometria que utilizam o ponto.
//						 Caso o tipo seja qualquer outro valor o ajuste do
//						 peso é direto
//
//
//  versao             : 1.1
//  programador        : Angelo Passaro
//  modificado por	   : Guilherme Moreira Magalhães
//  ultima modificacao : 02/08/2000
//
//===========================================================================

void Pt_Malha :: Set_Peso      (double Pes)
{
	Peso = Pes;
}
//____________________________________________________________________________
//===========================================================================
//               class Pt_Malha :: Build_Peso
//===========================================================================
//
//  Descricao          : Define o peso no ponto, de acordo com o tipo de
//						 operação. Se a operação for o ajuste dos pesos de
//						 pontos pertencentes a geometria é do tipo 1.
//
//	Parametros		   : Pes - peso do ponto
//						 tipo - (1) caso o tipo seja igual a 1
//						 o tratamento do ponto será para calculo de peso
//						 de acordo com o comprimento e o número de arestas
//						 de geometria que utilizam o ponto.
//						 Caso o tipo seja qualquer outro valor o ajuste do
//						 peso é direto
//
//
//  versao             : 1.1
//  programador        : Angelo Passaro
//  modificado por	   : Guilherme Moreira Magalhães
//  ultima modificacao : 02/08/2000
//
//===========================================================================

void Pt_Malha :: Build_Peso      (char  tipo, double Pes)
{
	if (tipo == '*')

		Peso/=Num_Geometrias;
	else
	{
		Num_Geometrias++;
		Peso += Pes;
	}
}
//____________________________________________________________________________

#if 0
Pt_Malha & Pt_Malha :: operator= (Pt_Malha & p)
{	X = p.X; Y = p.Y; Z = p.Z; tipo = p.tipo; Peso = p.Peso; dummy = p.dummy;
 Num_Geometrias = p.Num_Geometrias; material = p.material;
 TipoCC = p.TipoCC; ValorCC = p.ValorCC; Potencial=p.Potencial;
 Fontes = p.Fontes; DerivadaPrim = p.DerivadaPrim;
 return *this;}

char    Pt_Malha :: operator==(Pt_Malha & p)
{ 	if   (X == p.X && Y == p.Y && Z == p.Z && tipo == p.tipo && Peso == p.Peso
		  && material == p.material && TipoCC == p.TipoCC
		  && ValorCC == p.ValorCC && TipoCC == p.TipoCC
		  && Potencial == p.Potencial && Fontes==p.Fontes
		  && DerivadaPrim == p.DerivadaPrim
		  ) return YES;
	else return NO;
}
#endif
//Pt_Malha & Pt_Malha :: operator= (Pt_Malha & p)
Pt_Malha & Pt_Malha :: operator= (Pt_Malha const & p)
{
 x= p.x; y = p.y; z = p.z; tipo = p.tipo; Peso = p.Peso; dummy = p.dummy;
 Num_Geometrias = p.Num_Geometrias;
 TipoCC = p.TipoCC; ValorCC = p.ValorCC; Potencial=p.Potencial;

 num_potenciais				= p.num_potenciais;
 num_cc						= p.num_cc;
 num_materiais_derivadas	= p.num_materiais_derivadas;
 num_fontes					= p.num_fontes;

 memset(material,		0,	COISAS_POR_NO * sizeof(long));
 memset(Fontes,			0,	COISAS_POR_NO * sizeof(double));
 memset(DerivadaPrim,	0,	COISAS_POR_NO * sizeof(double));

 memcpy(material,		p.material,		p.num_materiais_derivadas * sizeof(long));
 memcpy(Fontes,			p.Fontes,		p.num_fontes * sizeof(double));
 memcpy(DerivadaPrim,	p.DerivadaPrim,	p.num_materiais_derivadas * sizeof(double));
 return *this;
}
//===================================================================================
// Pt_Malha :: operator ==
//===================================================================================
// Esta versão não testa o conteudo do vetor Derivada Prim
//===================================================================================
//char    Pt_Malha :: operator==(Pt_Malha & p)
char    Pt_Malha :: operator==( Pt_Malha const & p)
{
	bool continua = true;
	if   (x == p.x && y == p.y && z == p.z && tipo == p.tipo && Peso == p.Peso
		  && TipoCC == p.TipoCC && ValorCC == p.ValorCC && TipoCC == p.TipoCC
		  && Potencial == p.Potencial
		  && num_potenciais == p.num_potenciais && num_cc == p.num_cc
		  && num_materiais_derivadas == p.num_materiais_derivadas
		  && num_fontes == p.num_fontes)
			{
				int i;
				for (i = 0; i < num_materiais_derivadas && continua; i++)
				{
					if( Esta_Associado(p.material[i]) < 0) continua = false;
				}
				if (continua)
				{
					for (i = 0; i < num_fontes && continua; i++)
					{
						if( Esta_Associado(p.Fontes[i]) < 0 ) continua = false;
					}

				}

			}
	else continua = false;

	return continua;
}
//____________________________________________________________________________

double Pt_Malha::Get_Num_Geometrias()
{
	return Num_Geometrias;
}

void Pt_Malha::Set_Num_Geometrias(int n)
{
	Num_Geometrias=n;
}

//===============================================================
short int Pt_Malha::Get_Material(void)
{ return num_materiais_derivadas; }

//===============================================================
// Get_Material
// obs. Se o indice é maior que o numero de materiais associados
//      ao ponto retorna -1;
//===============================================================
long Pt_Malha::Get_Material(short int i)
{
	if (i < num_materiais_derivadas && i >= 0) return material[i];
	else return(-1);
}

//===============================================================
long * Pt_Malha::Get_Material(long *m)
{
	memcpy(m, material, num_materiais_derivadas* sizeof(long));
	return m;
}
//===============================================================

void Pt_Malha::Set_Material(long mat)
{
	if(num_materiais_derivadas <= COISAS_POR_NO)
	{
		if(Esta_Associado(mat) < 0)
			material[num_materiais_derivadas++]=mat;
	}
	else
	{
		std::cout << "\n\aATENCAO: esta versão permite apenas "<< COISAS_POR_NO << "materiais por nó";//mensagem
		std::cout << "\npara aumentar este valor modifique COISAS_POR_NO em ponto.hpp";
		getchar();
	}

}
//===============================================================

void Pt_Malha::Set_Material(long * mat, short int nm)
{
	if (nm >0 && nm <= COISAS_POR_NO && mat)
	{
		num_materiais_derivadas = nm;
		memcpy(material, mat, nm * sizeof(long));
	}
	else
	{
		std::cout << "\n\a Dados de entrada incompatíveis em Pt_Malha::Set_Material(long * mat, short int nm)";
		getchar();
	}
}

//===============================================================
double Pt_Malha::Get_ValorCC()
{
	return (ValorCC);
}

//===============================================================
void Pt_Malha::Set_ValorCC(double vcc)
{
	ValorCC=vcc;
}

//===============================================================
double Pt_Malha::Get_Potencial()
{
	return (Potencial);
}

//===============================================================
void Pt_Malha::Set_Potencial(double p)
{
	Potencial = p;
}

//===============================================================
short int Pt_Malha::Get_Fonte(void)
{ return num_fontes; }

//===============================================================
// Get_Fontes
// obs. Se o indice é maior que o numero de fontes associados
//      ao ponto retorna -1;
//===============================================================
double Pt_Malha::Get_Fonte(short int i)
{
	if (i < num_fontes && i >= 0) return Fontes[i];
	else return(-1);
}

//===============================================================
double * Pt_Malha::Get_Fonte(double *m)
{
	memcpy(m, Fontes, num_fontes* sizeof(double));
	return m;
}
//===============================================================

void Pt_Malha::Set_Fonte(double fon)
{
	if (fon != double(0.0))
	{
		if(num_fontes <= COISAS_POR_NO)
		{
			if(Esta_Associado(fon) < 0)
				Fontes[num_fontes++]=fon;
		}
		else
		{
			std::cout << "\n\aATENCAO: esta versão permite apenas "<< COISAS_POR_NO << "fontes por nó";//mensagem
			std::cout << "\npara aumentar este valor modifique COISAS_POR_NO em ponto.hpp";
			getchar();
		}
	}
}
//===============================================================

void Pt_Malha::Set_Fonte(double * fon, short int nf)
{
	if (nf >= 0 && nf <= COISAS_POR_NO && fon)
	{
		num_fontes = nf;
		memcpy(Fontes, fon, nf * sizeof(double));
	}
	else
	{
		std::cout << "\n\a Dados de entrada incompatíveis em Pt_Malha::Set_Fontes(double *, shor int)";
		getchar();
	}
}

//===============================================================
//===============================================================
short int Pt_Malha::Get_DerivadaPrim(void)
{ return num_materiais_derivadas; }

//===============================================================
// Get_DerivadaPrim
// obs. Se o indice é maior que o numero de fontes associados
//      ao ponto retorna -1;
//===============================================================
double Pt_Malha::Get_DerivadaPrim(short int i)
{
	if (i < num_materiais_derivadas && i >= 0) return DerivadaPrim[i];
	else return(-1);
}

//===============================================================
double * Pt_Malha::Get_DerivadaPrim(double *m)
{
	memcpy(m, DerivadaPrim, num_materiais_derivadas* sizeof(double));
	return m;
}
//===============================================================
#if 10

void Pt_Malha::Set_DerivadaPrim(double fon, long pos)
{
	DerivadaPrim[pos]=fon;
}
#endif

//===============================================================

void Pt_Malha::Set_DerivadaPrim(double * fon, short int nf)
{
	if (nf >0 && nf <= COISAS_POR_NO && fon)
	{
		num_materiais_derivadas = nf;
		memcpy(DerivadaPrim, fon, nf * sizeof(double));
	}
	else
	{
		std::cout << "\n\a Dados de entrada incompatíveis em Pt_Malha::Set_DerivadaPrim(double *, shor int)";
		getchar();
	}
}

//===============================================================
// class Pt_Malha::Esta_Associado(long mat)
//--------------------------------------------------------------
// Descrição : Verifica se material está associado ao ponto
//
// retorno:		-1: não está associado
//				>0: indice do material no array material[]
//===============================================================
int Pt_Malha::Esta_Associado(long mat)			// verifica se o material está associada ao ponto
{
	int i;
	int material_na_lista = -1;
	for(i=0; i < num_materiais_derivadas && material_na_lista < 0; i++)
	{
		if(material[i] == mat) material_na_lista = i;
	}
	return material_na_lista;
}
//===============================================================

//===============================================================
// class Pt_Malha::Esta_Associado(double font)
//--------------------------------------------------------------
// Descrição : Verifica se fonte está associado ao ponto
//
// retorno:		-1: não está associado
//				>0: indice da fonte no array fonte[]
//===============================================================
int Pt_Malha::Esta_Associado(double font)			// verifica se a fonte está associada ao ponto
{
	int i;
	int fontes_na_lista = -1;
	for(i=0; i < num_fontes && fontes_na_lista < 0 ; i++)
	{
		if(Fontes[i] == font) fontes_na_lista = i;
	}
	return fontes_na_lista;
}

//===============================================================


//***************************************************************************
//***************************************************************************
//
//                    C L A S S     P T _ E L
//
//***************************************************************************
//***************************************************************************

Pt_El :: Pt_El()  : Ponto_Base() { Potencial = 0;}
//____________________________________________________________________________

Pt_El :: ~Pt_El () {}
//____________________________________________________________________________

//===========================================================================
//               class Ponto :: Get_Potencial
//===========================================================================
//
//  Descricao          : Recupera o potencial no ponto calculado atraves do
//                       solver
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 14/07/94
//
//===========================================================================

double Pt_El :: Get_Potencial (void) {return Potencial;}

//===========================================================================
//               class Ponto :: Set_Potencial
//===========================================================================
//
//  Descricao          : Define o potencial no ponto
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 14/07/94
//
//===========================================================================

void Pt_El :: Set_Potencial (double Pot) { Potencial = Pot;}
//____________________________________________________________________________

Pt_El & Pt_El :: operator= (Pt_El & p)
{	x = p.x; y = p.y; z = p.z; tipo = p.tipo; Potencial = p.Potencial; return *this;}

char    Pt_El :: operator==(Pt_El & p)
						{ 	if   (x == p.x && y == p.y && z == p.z && tipo == p.tipo && Potencial == p.Potencial) return lev_enum::YES;
							else return lev_enum::NO;
						}




// ========== fim do arquivo ponto.cpp  =================================
