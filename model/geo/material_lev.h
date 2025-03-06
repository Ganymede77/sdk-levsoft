#ifndef __MATERIAL_H_
#define __MATERIAL_H_

//=============================================================================
//	Arquivo:			Material.h
//.............................................................................
//	Descricao:			Interface da classe CMaterial
//.............................................................................
//	Autores:			Nancy Mieko Abe
//						Angelo Passaro
//-----------------------------------------------------------------------------
//	Class CMaterial:
//-----------------------------------------------------------------------------
//	Dados membros:
//
//	Associacoes:
//
//	Observacoes:
//
//.............................................................................
//	Data:				Julho/2001
//	Modificado em:
//	Modificado por:
//	Motivo:
//=============================================================================


#include "levenum.h"
#include "propertytensor.h"
#include <iostream>
#include <vector>
#include "levcolorsRGB.h"


class CMaterial
{
//attibutes
protected:
	std::string			name;

	CPropertyTensor *permittivity;
	CPropertyTensor *conductivity;
	CPropertyTensor *relutivity;

	//long			 	color;
	CLEV_ColorRGB		color;
	int					codigo; // codigo considerando padr�o antigo (LMAG)
	LEV_OBJECTVALIDITY	status;

public:

	//double				material[7];
	std::vector<double>		material;

//implementations
protected:

public:
	CMaterial(); //Constructor
	virtual ~CMaterial(); //Destructor

	LEV_OBJECTVALIDITY  Status(void);
			void		Status(LEV_OBJECTVALIDITY);

	virtual std::string		Get_Name(void);

	virtual double		Get_Permittivity(void);
	virtual void		Set_Permittivity(double v);

	virtual double		Get_Conductivity(void);
	virtual void		Set_Conductivity(double v);

	virtual double		Get_Relutivity(void);
	virtual void		Set_Relutivity(double v);


	virtual void		Set_Name(std::string);

	//virtual void		Set_Color(long);
	virtual void		Set_Color(CLEV_ColorRGB); //*NMA* 06/01/2015: substitui uso do QColor
	///virtual long		Get_Color(void);
	virtual CLEV_ColorRGB	Get_Color(void); //*NMA* 06/01/2015: substitui uso do QColor
	//virtual void		Set_Color(CLEV_ColorRGB); //*NMA* 06/01/2015: substitui uso do QColor
	//virtual CLEV_ColorRGB	Get_Color(void); //*NMA* 06/01/2015: substitui uso do QColor

			void		Set_Codigo(int);
			int			Get_Codigo(void);

	CMaterial& operator =  (const CMaterial &);
	bool operator == (const CMaterial &);


};// end of class CPropertyTensor


void int2rgb(long cor, int &red, int &green, int &blue);
void rgb2int(int red, int green, int blue, long &cor);


#endif
