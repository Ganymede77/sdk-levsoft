#ifndef __DELAUNAY_2D_AUTOADAP__
#define __DELAUNAY_2D_AUTOADAP__


#include "dgr15_10_2d.hpp"


class Delaunay2D_AutoAdaptativo: public Delaunay2D
{
protected:

	struct erro {double Media, Desvio_Padrao, Maior, Limite, Menor;} Erro_Global_Malha;

	void Associa_ElmNos();
	void Calc_CampoNos();
	void Calc_Erro_Elementos(erro & );

public:

	struct materiais_struct
	{
		int NM;
		int codigos[10000];
		double materiais[10000][7];
	} * mat;

	Delaunay2D_AutoAdaptativo(void);
	~Delaunay2D_AutoAdaptativo(void);

	bool Elimina_EFront();
	int Le_BD_Materiais(char * nome_propfis_mei);
	lev_math::USINT Refina(int metodo);  // refinamento autoadaptativo
};


#endif
