//===================================================================================================
//	Arquivo:			Material.cpp
//...................................................................................................
//	Descrição:			Implementação das classes
//						- CMaterial
//...................................................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//...................................................................................................
//	Observações:
//
//===================================================================================================

#include "material_lev.h"
#include <stdio.h>

//===================== Class CPropertyType =======================================================
//===================================================================================================

CMaterial::CMaterial()
{
	status		 = LEV_INVALID;
	//rgb2int(255, 255, 255, color);
	color.setRGB(255,255,255);

	permittivity = NULL;
	conductivity = NULL;
	relutivity   = NULL;
}


CMaterial::~CMaterial()
{ }


LEV_OBJECTVALIDITY CMaterial::Status(void)
{
	return status;
}

void CMaterial::Status(LEV_OBJECTVALIDITY s)
{
	status = s;
}


double CMaterial::Get_Permittivity(void)
{
	if(permittivity)
		return permittivity->Get_Value();
	else return 0;
}

void CMaterial::Set_Permittivity(double v)
{
	permittivity->Set_Value(v);
}

double CMaterial::Get_Conductivity(void)
{
	if(conductivity)
		return conductivity->Get_Value();
	else return 0;
}

void CMaterial::Set_Conductivity(double v)
{
	conductivity->Set_Value(v);
}

double CMaterial::Get_Relutivity(void)
{
	if(relutivity)
		return relutivity->Get_Value();
	else return 0;
}

void CMaterial::Set_Relutivity(double v)
{
	relutivity->Set_Value(v);
}


std::string CMaterial::Get_Name(void)
{
	return name;
}

void CMaterial::Set_Name(std::string n)
{
	name = n ;
}

#if 0
void CMaterial::Set_Color(long cor)
{
	color = cor ;
}

long CMaterial::Get_Color(void)
{
	return color;
}
#endif

void CMaterial::Set_Color(CLEV_ColorRGB cor)
{
	color = cor ;
}

CLEV_ColorRGB CMaterial::Get_Color(void)
{
	return color;
}

int CMaterial::Get_Codigo(void)
{
	return codigo;
}

void CMaterial::Set_Codigo(int cod)
{
	codigo = cod;
}

bool CMaterial::operator == (const CMaterial &m)
{
	// por enquanto so compara nome e código, que são os dados usados
	// no modelo

	bool retorno = false;

	if(status && m.status)
	{
		if(name == m.name)
		{
			if(codigo == m.codigo)
			{
				retorno = true;
			}
		}
	}

	return retorno;
}

CMaterial& CMaterial::operator = (const CMaterial &m)
{
	// por enquanto copia nome e código, que são os dados usados
	// no modelo

	int i;

	name	= m.name;
	codigo	= m.codigo;
	status  = m.status;
	color	= m.color;

	material.clear();
	for(i=0;i<m.material.size();i++)
		material.push_back(m.material[i]);

	return *this;
}


void int2rgb(long cor, int &red, int &green, int &blue)
{
	blue = cor & 255;
	green = (cor >> 8) & 255;
	red = (cor >> 16) & 255;
	//cor_blue = floor(cor / 65536);
	//cor_green = floor((cor - cor_blue * 65536) / 256);
	//cor_red = cor - cor_blue * 65536 - cor_green * 256;
}


void rgb2int(int red, int green, int blue, long &cor)
{
	cor = red;
	cor = (cor << 8) + green;
	cor = (cor << 8) + blue;
    //'Convert RGB to LONG:
    // LONG = B * 65536 + G * 256 + R
}


