#ifndef _ARESTA_GEOMETRIA_
#define _ARESTA_GEOMETRIA_


// ============================================================================
// Arquivo : Aresta_Geom.hpp
// Descricao :	Este arquivo contem a struct para guardar uma aresta(ou
//              segmento de geometria). A ser usado para gerar a lista de arestas.
// Implementacao :Versao : 1.0
// programador : Guilherme Moreira Magalhaes
// ultima revisao em : 10/05/2000
// ============================================================================


#include "levenum.h"
#include "math.h"
#include "lo_pt.hpp"
#include "lo_templ.hpp"
#include "levtypedefmalha.h"


class Aresta_Geometria
{

public:
	int	g1,
		g2,
		prim;

	int primType;


	double Calc_Comprimento(vCLevPtMalha & pts)
	{
		double comprimento;
		Pt_Malha p1, p2;
		p1 = pts[g1];
		p2 = pts[g2];
		comprimento = sqrt( (p1.X()-p2.X())*(p1.X()-p2.X())
						  +	(p1.Y()-p2.Y())*(p1.Y()-p2.Y()) );
		(comprimento < 0)? comprimento = - comprimento: comprimento;
		return(comprimento);
	}

	lev_math::USINT Get_Central_Point(vCLevPtMalha &lista, Pt_Malha &p)
	{
		Pt_Malha p1, p2;
		double xp, yp, t = 0.5;
		lev_math::USINT retorno = 0;

		p.Set_Tipo(PRE);

		p1 = lista[g1];
		p2 = lista[g2];

		if(!(retorno))
			if(!(retorno))
			{
				xp= p1.X()+t*(p2.X()-p1.X());
				yp= p1.Y()+t*(p2.Y()-p1.Y());
				p.Set_Coord(xp, yp);

				if (p1.Get_TipoCC() && p2.Get_TipoCC())
				{
					if (p1.Get_TipoCC() == p2.Get_TipoCC())
					{
						if (p1.Get_ValorCC() == p2.Get_ValorCC())
						{
							p.Set_TipoCC(p1.Get_TipoCC());
							p.Set_ValorCC (p1.Get_ValorCC());
						}
						else
						{
							if (p1.Get_TipoCC() == 2)
							{
								p.Set_TipoCC(p1.Get_TipoCC());
								p.Set_ValorCC ((p1.Get_ValorCC()+p2.Get_ValorCC())/2);
							}
						}

					}
				}
				else p.Set_TipoCC(0);

				short int i, n_coisas = 0;
				long coisas[COISAS_POR_NO];
				for (i = 0; i < p1.Get_Material() ; i++)
				{
					if ( p2.Esta_Associado(p1.Get_Material(i)) >= 0 )
					{
						coisas[n_coisas++] = p1.Get_Material(i);
					}
				}
				if(n_coisas) p.Set_Material(coisas, n_coisas);

				double dcoisas[COISAS_POR_NO];
				n_coisas = 0;
				for (i = 0; i < p1.Get_Fonte() ; i++)
				{
					if (p2.Esta_Associado(p1.Get_Fonte(i)) >= 0)
					{
						dcoisas[n_coisas++] = p1.Get_Fonte(i);
					}
				}
				if(n_coisas) p.Set_Fonte(dcoisas, n_coisas);

			}
		return retorno ;
	}

	double Get_Comprimento(vCLevPtMalha & lista)
	{
		Pt_Malha p1, p2;
		double comprimento=0;
		double comp2;

		p1 = lista[g1];
		p2 = lista[g2];
		comp2 = ( (p2.X()-p1.X())*(p2.X()-p1.X())+(p2.Y()-p1.Y())*(p2.Y()-p1.Y()) );
		comprimento = sqrt(comp2);
		return comprimento;
	}

	Aresta_Geometria& operator = (const Aresta_Geometria & b)
	{
		g1			= b.g1;
		g2			= b.g2;
		prim		= b.prim;
		primType	= b.primType;

		return *this;
	}

	bool operator == (const Aresta_Geometria & b)  //*NMA*: Mudei para m�todo da classe, pois o compilador n�o reconhecia
	{
		if ((g1==b.g1 && g2==b.g2) || (g1==b.g2 && g2==b.g1))
			return (1);
		return(0);
	}

	friend bool operator == (const Aresta_Geometria & a, const FACE & b);
	friend bool operator == ( FACE const & b,  Aresta_Geometria const& a);
};


typedef lev_math::Lo_Templ<Aresta_Geometria, long> Lo_Aresta_Geometria;


#endif
