#ifndef __PREDIS_H__
#define __PREDIS_H__


class CPredis
{
public:

	CPredis(){nPoints =0;factor =1;}; //Constructor
	~CPredis(){}; //Destructor

protected:

public: 
	
	int			nPoints;
	double		factor; 
	double		x,y;		//*NMA* posi��o "clickada" pelo usu�rio, utilizada para
							// determinar o lado com maior concentra��o de pontos no 
							// caso de divis�o geom�trica
};

#endif
