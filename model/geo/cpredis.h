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
	double		x,y;		//*NMA* posição "clickada" pelo usuário, utilizada para
							// determinar o lado com maior concentração de pontos no 
							// caso de divisão geométrica
};

#endif
