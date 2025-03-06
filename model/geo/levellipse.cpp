//=============================================================================
//	Arquivo:			LevEllipse.cpp
//.............................................................................
//	Descricao:			Implementacao da classe CLevEllipse
//.............................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//.............................................................................
//	Observacoes:
//
//=============================================================================

#include "levellipse.h"
#include "levdef.h"
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>


CLevEllipse::CLevEllipse()
{
	x_semiAxis = 0;
	y_semiAxis = 0;

	vpPoints.reserve(5);

	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);

	//Inclui o prï¿½prio ponteiro na lista de Ellipses
	vpEllipses.push_back(this);

}

CLevEllipse::CLevEllipse(double xct, double yct, double x_semiA, double y_semiA,double sAng, double eAng, vCLevPoint &vPtVer)
{
	xc = xct;
	yc = yct;
	startAngle = sAng;
	endAngle   = eAng;
	x_semiAxis = x_semiA;
	y_semiAxis = y_semiA;

	double x =0., y=0.;

	// Calculo das coordenadas dos pontos vertices
	Locate_Extremity_Coordinates(startAngle,x,y);
	vPtVer[0].X(x);
	vPtVer[0].Y(y);

	Locate_Extremity_Coordinates(endAngle,x,y);
	vPtVer[1].X(x);
	vPtVer[1].Y(y);

	// Inicializa vetor de ponteiros para pontos
	vpPoints.reserve(5);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);

	//Inclui o prï¿½prio ponteiro na lista de Ellipses
	vpEllipses.push_back(this);
}


// *NMA*: Nesta sobrecarga sï¿½o fornecidos ponteiros para pontos p1 e p2, que sï¿½o preenchidos no
//			construtor a partir do cï¿½lculo dos pontos a partir dos ï¿½ngulos.
CLevEllipse::CLevEllipse(CLevPoint *p1, CLevPoint *p2, CLevPoint *pCentral, CLevPoint *pAxisX,CLevPoint *pAxisY, double sAng, double eAng)
{
	xc = pCentral->X();
	yc = pCentral->Y();
	startAngle = sAng;
	endAngle   = eAng;
	x_semiAxis = pAxisX->X();
	y_semiAxis = pAxisY->Y();

	if(startAngle == endAngle)
	{
		if(x_semiAxis == y_semiAxis)
			geoEntityType = CIRCUNFERENCELEV;
	}

	double x =0., y=0.;

	// Calculo das coordenadas dos pontos vï¿½rtices
	Locate_Extremity_Coordinates(startAngle,x,y);
	p1->X(x);
	p1->Y(y);

	Locate_Extremity_Coordinates(endAngle,x,y);
	p2->X(x);
	p2->Y(y);
	// Inicializa vetor de ponteiros para pontos
	vpPoints.reserve(5);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);

	vpPoints[0] = p1;
	vpPoints[1] = p2;
	vpPoints[2] = pCentral;
	vpPoints[3] = pAxisX;
	vpPoints[4] = pAxisY;

	if(*vpPoints[0] != *vpPoints[1])
	{
		geoEntityType = ARCLEV;
	}


	//Inclui o prï¿½prio ponteiro na lista de Ellipses
	vpEllipses.push_back(this);
}

//*NMA*: Nesta sobrecarga os pontos p1 e p2 sï¿½o conhecidos,a partir deles sï¿½o calculados os ï¿½ngulos inicial e final
//			da elipse
CLevEllipse::CLevEllipse(CLevPoint *p1, CLevPoint *p2, CLevPoint *pCentral, CLevPoint *pAxisX,CLevPoint *pAxisY)
{
	xc = pCentral->X();
	yc = pCentral->Y();
	x_semiAxis = pAxisX->X();
	y_semiAxis = pAxisY->Y();

	// Calculo do ï¿½ngulo inicial a partir de p1
	startAngle = Get_Point_Angle(p1);
	// Calculo do ï¿½ngulo final a partir de p2
	if((p1->X() == p2->X()) && (p1->Y() == p2->Y()))
		endAngle = startAngle;
	else
		endAngle = Get_Point_Angle(p2);

	// Inicializa vetor de ponteiros para pontos
	vpPoints.reserve(5);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);
	vpPoints.push_back(NULL);

	vpPoints[0] = p1;
	vpPoints[1] = p2;
	vpPoints[2] = pCentral;
	vpPoints[3] = pAxisX;
	vpPoints[4] = pAxisY;

	if(*vpPoints[0] != *vpPoints[1])
	{
		geoEntityType = ARCLEV;
	}


	// Inclui o prï¿½prio ponteiro na lista de Ellipses
	vpEllipses.push_back(this);
}


CLevEllipse::~CLevEllipse()
{ }

CLevPrimitive* CLevEllipse::Clone() const
{
    // Invoca o construtor de cópia de CLevEllipse
    return new CLevEllipse(*this);
}

void CLevEllipse::Limits(CLevPoint & p1, CLevPoint & p2)
{
	TakeLimits();
	p1 = pt_minxminy;
	p2 = pt_maxxmaxy;
}

//-----------------------------------------------------------------------------------------------------
//	Locate_Extremity_Coordinates()
//-----------------------------------------------------------------------------------------------------
//
//	Descriï¿½ï¿½o:
//
//	parï¿½metros:
//
//	retorno:
//
//	Observaï¿½ï¿½es:
//
//......................................................................................................
//	Versï¿½o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//					Anna Carolina S. Volpi (Adaptado de System_POI)
//	Data:			Junho/2000
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------

void CLevEllipse::Locate_Extremity_Coordinates(double angle, double &xCoord, double &yCoord)
{

//	double	angle=0;
	int		angle1=0;
	bool	is_angle_int = false;

//	if(indice == 1) angle = startAngle;
//	else			angle = endAngle;

	if(angle < 0) angle += 360;

	if(angle == 0 || angle == 90 || angle == 180 || angle == 270 || angle == 360)
	{
		angle1 = (int) angle;
		is_angle_int = true;
	}

	if(is_angle_int)
	{
		switch(angle1)
		{
		case 0:
		case 360:	xCoord = xc + x_semiAxis;
					yCoord = yc;
					break;

		case 90:	xCoord = xc;
					yCoord = yc + y_semiAxis;
					break;

		case 180: 	xCoord = xc - x_semiAxis;
					yCoord = yc;
					break;

		case 270: 	xCoord = xc;
					yCoord = yc - y_semiAxis;
					break;

		}
	}
	else Locate_Crossing_Coordinates(angle,xCoord,yCoord);

}// end of Locate_End_Coordinates


void CLevEllipse::Locate_Crossing_Coordinates(double angle, double &xCoord, double &yCoord)
{
	double D=0;
	double angCoef;
	double AngleRad;
	double a2, b2, m2;

	a2 = x_semiAxis * x_semiAxis;
	b2 = y_semiAxis * y_semiAxis;

	if(angle < 0) angle += 360;

	AngleRad = angle * ctePI / 180.;
	angCoef = tan(AngleRad);
	m2 = angCoef * angCoef;
	D = sqrt( (a2 * b2) / ( b2 + (a2*m2) ) );

	if(angle < 90)
	{
		xCoord = D ;
		yCoord = angCoef*D;
	}
	else
	{
		if(angle < 180)
		{
			xCoord = D ;
			yCoord = angCoef*D;

			xCoord = - xCoord;
			yCoord = - yCoord;
		}
		else
		{
			if(angle <270)
			{
				xCoord = D ;
				yCoord = angCoef*D;

				xCoord = - xCoord;
				yCoord = - yCoord;

			}
			else
			{
				xCoord = D ;
				yCoord = angCoef*D;

			}
		}
	}

	// translada os pontos de acordo com o deslocamento do centro da elipse em
	// relaï¿½ï¿½o ï¿½ origem do sistema global
	xCoord += xc ;
	yCoord += yc;


}// end of Locate_Crossing_Coordinates

double CLevEllipse::Get_Point_Angle(CLevPoint *o_point)
{
 	double angle = 0;
 	double degreeAng =0;
 	double theta = 0;

 	if((o_point->Y()-yc) == 0.0)
 	{
 		if( (o_point->X()-xc) >= 0.) angle = 0;
 		else angle = 180;
 	}
 	else
 	{
 		if( (o_point->X()-xc) == 0)
 		{
 			if( (o_point->Y()-yc) >= 0.) angle = 90;
 			else angle = 270;
 		}
 		else
 		{
 			theta		= atan2((o_point->Y() - yc),(o_point->X()-xc));
 			degreeAng	= theta*180/ctePI;
 			angle		= degreeAng;
 		}
 	}

 	return angle;

}


void CLevEllipse::TakeLimits(void)
{
	double	xmin;
	double	ymin;
	double	xmax;
	double	ymax;
	double	angleIni, angleFim;
	int		quadranteIni = 1, quadranteFim =1;

	angleIni = startAngle;
	angleFim = endAngle;
	if(startAngle < 0 ) angleIni = startAngle + 360;
	if(endAngle   <= 0 ) angleFim = endAngle + 360;

	if(angleFim - angleIni == 360 || angleFim - angleIni == 0)
	{
		xmin = xc - x_semiAxis;
		ymin = yc - y_semiAxis;
		xmax = xc + x_semiAxis;
		ymax = yc + y_semiAxis;
	} else {
		// determinaï¿½ï¿½o dos quadrantes dos pontos inicial e final
		if(angleIni>=0 && angleIni <=90)	quadranteIni = 1;
		else
		if(angleIni>90 && angleIni <=180)	quadranteIni = 2;
		else
		if(angleIni>180 && angleIni <=270)	quadranteIni = 3;
		else
		if(angleIni>270 && angleIni <360)	quadranteIni = 4;

		if(angleFim>0 && angleFim <=90)		quadranteFim = 1;
		else
		if(angleFim>90 && angleFim <=180)	quadranteFim = 2;
		else
		if(angleFim>180 && angleFim <=270)	quadranteFim = 3;
		else
		if(angleFim>270 && angleFim <=360)	quadranteFim = 4;

		// determina mï¿½nimos e mï¿½ximos considerando somente os pontos inicial e final
		if(vpPoints[0]->X() < vpPoints[1]->X())
		{
			xmin = vpPoints[0]->X();
			xmax = vpPoints[1]->X();
		}
		else
		{
			xmin = vpPoints[1]->X();
			xmax = vpPoints[0]->X();
		}

		if(vpPoints[0]->Y() < vpPoints[1]->Y())
		{
			ymin = vpPoints[0]->Y();
			ymax = vpPoints[1]->Y();
		}
		else
		{
			ymin = vpPoints[1]->Y();
			ymax = vpPoints[0]->Y();
		}

		// redefine os mï¿½ximos ou mï¿½nimos caso a caso
		if(quadranteIni == quadranteFim)
		{
			// Se o angulo final for menor do que o angulo inicial: a elipse ï¿½ quase inteira
			// entï¿½o vale os limites calculados para a inteira
			if(angleIni > angleFim)
			{
				xmin = xc - x_semiAxis;
				ymin = yc - y_semiAxis;
				xmax = xc + x_semiAxis;
				ymax = yc + y_semiAxis;
			}
		}
		else
		{
			switch(quadranteIni)
			{
			case 1:
				switch(quadranteFim)
				{
				case 2:
						ymax = yc + y_semiAxis;
					break;
				case 3:
						ymax = yc + y_semiAxis;
						xmin = xc - x_semiAxis;
					break;
				case 4:
						ymax = yc + y_semiAxis;
						xmin = xc - x_semiAxis;
						ymin = yc - y_semiAxis;
					break;
				}
				break;
			case 2:
				switch(quadranteFim)
				{
				case 1:
						xmin = xc - x_semiAxis;
						ymin = yc - y_semiAxis;
						xmax = xc + x_semiAxis;
					break;
				case 3:
						xmin = xc - x_semiAxis;
					break;
				case 4:
						xmin = xc - x_semiAxis;
						ymin = yc - y_semiAxis;
					break;
				}
				break;
			case 3:
				switch(quadranteFim)
				{
				case 1:
						xmax = xc + x_semiAxis;
						ymin = yc - y_semiAxis;
					break;
				case 2:
						ymin = yc - y_semiAxis;
						ymax = yc + y_semiAxis;
						xmax = xc + x_semiAxis;
					break;
				case 4:
						ymin = yc - y_semiAxis;
					break;
				}
				break;
			case 4:
				switch(quadranteFim)
				{
				case 1:
						xmax = xc + x_semiAxis;
					break;
				case 2:
						xmax = xc + x_semiAxis;
						ymax = yc + y_semiAxis;
					break;
				case 3:
						xmax = xc + x_semiAxis;
						xmin = xc - x_semiAxis;
						ymax = yc + y_semiAxis;
					break;
				}
				break;
			}
		}

	}


	pt_minxminy.X(xmin);
	pt_minxminy.Y(ymin);
	pt_maxxmaxy.X(xmax);
	pt_maxxmaxy.Y(ymax);

}

void CLevEllipse::Move(double deltax, double deltay, double deltaz)
{
	if(fixedEntity == false)
	{
		if(vpPoints[0] != vpPoints[1])
		{
			vpPoints[0]->Move(deltax, deltay, deltaz);	// v1
			vpPoints[1]->Move(deltax, deltay, deltaz);	// v2
		}
		else vpPoints[0]->Move(deltax, deltay, deltaz);	// v1

		vpPoints[2]->Move(deltax, deltay, deltaz);  // ponto central

		xc += deltax;
		yc += deltay;

		// atualiza coordenadas dos pontos invisï¿½veis correspondentes aos semi-eixos
		vpPoints[3]->Y(yc);
		vpPoints[4]->X(xc);

	}

	TakeLimits();
}

//-----------------------------------------------------------------------------------------------------
//	Rotation
//-----------------------------------------------------------------------------------------------------
//
//	Descriï¿½ï¿½o: Rotaciona a elipse
//
//	parï¿½metros:
//
//	retorno:
//
//	Observaï¿½ï¿½es:
//
//......................................................................................................
//	Versï¿½o:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Abril/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevEllipse::Rotation(double angle, CLevPoint &pointc)
{

	CLevPoint p0,p1;

	if(!fixedEntity)
	{
		p0.X(vpPoints[0]->X());
		p0.Y(vpPoints[0]->Y());
		p1.X(vpPoints[1]->X());
		p1.Y(vpPoints[1]->Y());

		//rotaciona o ponto central
		vpPoints[2]->Rotation(angle, pointc);
		xc = vpPoints[2]->X();
		yc = vpPoints[2]->Y();

		//rotaciona e atualiza coordenadas dos pontos invisï¿½veis correspondentes aos semi-eixos
	//	vpPoints[3]->Rotation(angle, pointc);
	//	vpPoints[4]->Rotation(angle, pointc);
		vpPoints[3]->Y(yc);
		vpPoints[4]->X(xc);

		// rotaciona os vï¿½rtices
		if( (vpPoints[0]->X() == vpPoints[1]->X()) && (vpPoints[0]->Y() == vpPoints[1]->Y()) )
		{
			p0.Rotation(angle, pointc);

			startAngle = endAngle = Get_Point_Angle(&p0);

			vpPoints[0]->X(p0.X());
			vpPoints[0]->Y(p0.Y());

			vpPoints[1]->X(p0.X());
			vpPoints[1]->Y(p0.Y());

		}
		else
		{
			p0.Rotation(angle, pointc);
			p1.Rotation(angle, pointc);

			startAngle = Get_Point_Angle(&p0);
			endAngle   = Get_Point_Angle(&p1);

			vpPoints[0]->X(p0.X());
			vpPoints[0]->Y(p0.Y());

			vpPoints[1]->X(p1.X());
			vpPoints[1]->Y(p1.Y());

		}
	}

	TakeLimits();

#if 0
	p0.X(vpPoints[0]->X() - xc);
	p0.Y(vpPoints[0]->Y() - yc);
	p1.X(vpPoints[1]->X() - xc);
	p1.Y(vpPoints[1]->Y() - yc);

	//rotaciona o ponto central
	vpPoints[2]->Rotation(angle, pointc);
	xc = vpPoints[2]->X();
	yc = vpPoints[2]->Y();

	// atualiza coordenadas dos pontos invisï¿½veis correspondentes aos semi-eixos
	vpPoints[3]->Y(yc);
	vpPoints[4]->X(xc);


	// rotaciona os vï¿½rtices
	if( (vpPoints[0]->X() == vpPoints[1]->X()) && (vpPoints[0]->Y() == vpPoints[1]->Y()) )
	{
		p0.Rotation(angle, pointc);

		vpPoints[0]->X(p0.X()+xc);
		vpPoints[0]->Y(p0.Y()+yc);

		vpPoints[1]->X(p0.X()+xc);
		vpPoints[1]->Y(p0.Y()+yc);
	}
	else
	{
		p0.Rotation(angle, pointc);
		p1.Rotation(angle, pointc);

		vpPoints[0]->X(p0.X()+xc);
		vpPoints[0]->Y(p0.Y()+yc);

		vpPoints[1]->X(p1.X()+xc);
		vpPoints[1]->Y(p1.Y()+yc);

	}
#endif



}// end of Rotation

void CLevEllipse::SetPoints(vpCLevPoint &vp)
{
	//inclui os pontos central e as extremidades (semi-eixos (x,0) e (0,y))
	if(vpPoints.size())
	{
		vpPoints.clear();
	}

	vpPoints.push_back(vp[0]);
	vpPoints.push_back(vp[1]);
	vpPoints.push_back(vp[2]);
	vpPoints.push_back(vp[3]);
	vpPoints.push_back(vp[4]);


	xc = vpPoints[2]->X();
	yc = vpPoints[2]->Y();

	x_semiAxis = vpPoints[3]->X();
	y_semiAxis = vpPoints[4]->Y();

//	if(*vpPoints[0] != *vpPoints[1])
//	{
//		geoEntityType = ARC;
//	}

}// end of SetPoints

//*NMA*: escalamento
void CLevEllipse::Scale(double factor)
{
	if(!fixedEntity)
	{
		vpPoints[0]->Scale(factor);
		vpPoints[1]->Scale(factor);

		vpPoints[2]->Scale(factor);
		vpPoints[3]->Scale(factor);
		vpPoints[4]->Scale(factor);

		xc = vpPoints[2]->X();
		yc = vpPoints[2]->Y();

		x_semiAxis = vpPoints[3]->X();
		y_semiAxis = vpPoints[4]->Y();

		TakeLimits();

		fixedEntity = true;
	}

}

////////////////////////
//The reinitialize() function
////////////////////////
void CLevEllipse::Reinitialize(void)
{ memset(this, NULL,sizeof(CLevEllipse)); }


//-----------------------------------------------------------------------------------------------------
//	IsInBoundaryBox
//-----------------------------------------------------------------------------------------------------
//
//	Descriï¿½ï¿½o: Verifica se um ponto estï¿½ dentro dos limites do arco.
//
//	parï¿½metros:	Um objeto ponto
//
//
//	retorno:	false: o ponto nï¿½o estï¿½ dentro dos limites do arco
//				true: o ponto estï¿½ dentro dos limites do arco
//
//	Observaï¿½ï¿½es:
//
//......................................................................................................
//	Versï¿½o:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Dezembro/2001
//	Modificado por: Nancy Mieko Abe
//	Modificado em:  19/02/2002
//	Motivo:			Correï¿½ï¿½o da verificaï¿½ï¿½o :
//						if(startAngle > endAngle) endAngle += 360;
//					para
//						if(angleIni > angleFim) angleFim += 360;
//
//-----------------------------------------------------------------------------------------------------
bool CLevEllipse::IsInBoundingBox(CLevPoint o_point)
{
	bool   codRetorno = false, changedAngleEnd = false;
	double anglePoint,angleRad;
	double angleIni, angleFim;
	double dx, dy;
	CLevPoint o_pointLocal;

	dx = fabs(vpPoints[3]->X())/1000;
	dy = fabs(vpPoints[4]->Y())/1000;

	o_pointLocal.X(o_point.X()-xc);
	o_pointLocal.Y(o_point.Y()-yc);

	angleIni = startAngle;
	angleFim = endAngle;

	if(startAngle == endAngle || (endAngle-startAngle)==360) {
		angleIni = 0;
		angleFim = 360;
	} else {
		if(startAngle < 0)
			angleIni +=360;
		if(endAngle <= 0)
			angleFim +=360;
	}

	if(o_pointLocal.X()== 0.) {
		if(o_pointLocal.Y() >= 0.)
			anglePoint = 90;
		else
			anglePoint = 270;
	} else {
		if( fabs(o_pointLocal.Y()) < 1e-10 ) {
			if(o_pointLocal.X() >= 0.) {
				if(changedAngleEnd == true)
					anglePoint = 360;
				else
					anglePoint = 0;
			} else
				anglePoint = 180;
		} else {
			angleRad = atan(o_pointLocal.Y()/o_pointLocal.X());
			anglePoint = angleRad * 180 / ctePI;

			if(anglePoint > 0) { // 1o. ou 3o. quadrante
				// 3o. quadrante
				if( o_pointLocal.X() < 0. && o_pointLocal.Y() < 0 )
					anglePoint += 180;
			} else { // 4o. ou 2o. quadrante
				anglePoint += 360;

				//2o. quadrante
				if(o_pointLocal.X() < 0. && o_pointLocal.Y() > 0 )
					anglePoint -= 180;
			}
		}
	}

	if(pt_minxminy.X()-dx <= o_point.X() && o_point.X() <= pt_maxxmaxy.X()+dx)
		if(pt_minxminy.Y()-dy <= o_point.Y() && o_point.Y() <= pt_maxxmaxy.Y()+dy) {
			if(angleIni > angleFim) {
				if( (angleIni <= anglePoint && anglePoint <=360) ||
					(0 <= anglePoint && anglePoint <= angleFim) )
					codRetorno = true;
			} else if(angleIni <= anglePoint && anglePoint <= angleFim)
				codRetorno = true;
		}
	return codRetorno;
}


//-----------------------------------------------------------------------------------------------------
//	operator ==
//-----------------------------------------------------------------------------------------------------
//
//	Descriï¿½ï¿½o: Verifica se duas elipses sï¿½o iguais. No caso de elipse a ordem dos
//				pontos ï¿½ importante, dois arcos podem ter dois pontos vï¿½rtices iguais
//				porï¿½m se os pontos iniciais e finais estiverem invertidos, os arcos sï¿½o
//				diferentes.
//
//	parï¿½metros:	Uma referencia a um objeto elipse
//
//	retorno:	false: as elipse sï¿½o diferentes
//				true: as elipses sï¿½o iguais
//
//	Observaï¿½ï¿½es:
//
//......................................................................................................
//	Versï¿½o:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Dezembro/2001
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
bool CLevEllipse::operator == (const CLevEllipse &a)
{
	bool	ret = false;

	if (status && a.status)
	{
		if (vpPoints[0] == a.vpPoints[0]) // ponto inicial
		{
			if (vpPoints[1] == a.vpPoints[1]) // ponto final
			{
				if (vpPoints[2] == a.vpPoints[2]) // ponto central
				{
					if (vpPoints[3]->X() == a.vpPoints[3]->X()) //semi-eixo x
					{
						if (vpPoints[4]->Y() == a.vpPoints[4]->Y()) //semi-eixo y
						{
							ret = true;
						}
					}
				}
			}
		}
	}

	return ret;

}

//-----------------------------------------------------------------------------------------------------
//	operator !=
//-----------------------------------------------------------------------------------------------------
//
//	Descriï¿½ï¿½o: Verifica se duas elipses sï¿½o iguais. No caso de elipse a ordem dos
//				pontos ï¿½ importante, dois arcos podem ter dois pontos vï¿½rtices iguais
//				porï¿½m se os pontos iniciais e finais estiverem invertidos, os arcos sï¿½o
//				diferentes.
//
//	parï¿½metros:	Uma referencia a um objeto elipse
//
//	retorno:	false: as elipse sï¿½o diferentes
//				true: as elipses sï¿½o iguais
//
//	Observaï¿½ï¿½es:
//
//......................................................................................................
//	Versï¿½o:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Dezembro/2001
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
bool CLevEllipse::operator != (const CLevEllipse &a)
{
	bool ret = true;

	if (status && a.status)
	{
		if (vpPoints[0] == a.vpPoints[0]) // ponto inicial
		{
			if (vpPoints[1] == a.vpPoints[1]) // ponto final
			{
				if (vpPoints[2] == a.vpPoints[2]) // ponto central
				{
					if (vpPoints[3]->X() == a.vpPoints[3]->X()) //semi-eixo x
					{
						if (vpPoints[4]->Y() == a.vpPoints[4]->Y()) //semi-eixo y
						{
							ret = false;
						}
					}
				}
			}
		}
	}

	return ret;
}

//-----------------------------------------------------------------------------------------------------
//	operator =
//-----------------------------------------------------------------------------------------------------
//
//	Descriï¿½ï¿½o: copia dados de uma elipse para outra
//
//	parï¿½metros:	Uma referencia a um objeto elipse
//
//	retorno:	false: as elipse sï¿½o diferentes
//				true: as elipses sï¿½o iguais
//
//	Observaï¿½ï¿½es:
//
//......................................................................................................
//	Versï¿½o:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Dezembro/2001
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
CLevEllipse & CLevEllipse :: operator = (const CLevEllipse & ro_entrada)
{
	if (this != &ro_entrada)
	{
		// isto ï¿½ copia dos dados de primitive
		//=====================================
		geoEntityType =  ro_entrada.geoEntityType;

		pt_maxxmaxy = ro_entrada.pt_maxxmaxy;
		pt_minxminy = ro_entrada.pt_minxminy;

		vpPoints    = ro_entrada.vpPoints;
		xc			= ro_entrada.xc;
		yc			= ro_entrada.yc;
		x_semiAxis	= ro_entrada.x_semiAxis;
		y_semiAxis	= ro_entrada.y_semiAxis;
		startAngle	= ro_entrada.startAngle;
		endAngle	= ro_entrada.endAngle;

		isOfGeometry = ro_entrada.isOfGeometry;

		vpEllipses.clear();
		vpEllipses.push_back(this);

		status      = ro_entrada.status;

		//propriedades fï¿½sicas
		materialIndex	 = ro_entrada.materialIndex;
		sourceIndex		 = ro_entrada.sourceIndex;
		boundConditionIndex = ro_entrada.boundConditionIndex;
		//======================================
		//dados de prï¿½-discretizaï¿½ï¿½o
		oPredis		=	ro_entrada.oPredis;
		vIndexArestaGeometria = ro_entrada.vIndexArestaGeometria;
	}
	return *this;
} // fim operator =

//-----------------------------------------------------------------------------------------------------
//	PreDiscretize()
//-----------------------------------------------------------------------------------------------------
//
//	Descriï¿½ï¿½o:
//
//	parï¿½metros:	Referencia a um vector (STL) de CLevPoints
//
//	retorno:
//
//	Observaï¿½ï¿½es:
//		A determinaï¿½ï¿½o dos pontos de prï¿½-discretizaï¿½ï¿½o leva em conta a excentricidade
//	da elipse, resultando em mais pontos onde a curvatura ï¿½ maior para melhor descrever
//	a elipse. No caso de circunferï¿½ncia, os pontos sï¿½o espaï¿½ados igualmente.
//		As fï¿½rmulas para cï¿½lculo dos pontos de prï¿½-discretizaï¿½ï¿½o foram obtidas em:
//
//		- Mathematical elements for computer graphics, David F. Rogers and J. Alan Adams,
//			pï¿½g. 219.
//
//......................................................................................................
//	Versï¿½o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			Outubro/2001
//	Modificado por:	Nancy e Angelo
//	Modificado em:	Janeiro/2002
//	Motivo:			Utilizaï¿½ï¿½o dos ï¿½ngulos inicial e final da representaï¿½ï¿½o paramï¿½trica
//					da elipse a partir dos pontos inicial e final. Tratamento de casos
//					particulares (angulo inicial maior do que o final, quadrantes).
//-----------------------------------------------------------------------------------------------------
void CLevEllipse::PreDiscretize(vCLevPoint &vo_ptPredis)
{

	CLevPoint	o_paux;
	int			nPoints;
	double		factor;
	double		dThetaTotal, dThetaInc;
	double		cosdeltheta, sindeltheta;
	double		angleIni = 0.0, angleFim = 0.0;
	double		a = 0.,b = 0.;
	double		x,y,x_previous,y_previous, xini= 0.,yini,xfim,yfim, xcaux, ycaux;
	int			i,j;

	if(oPredis.NPoints())
	{
		//---------------------------------------------------------------------
		//*NMA*
		//	limpa e redimensiona o vetor de pontos de prï¿½-discretizaï¿½ï¿½o
		nPoints			= oPredis.NPoints();
		factor			= oPredis.Factor();

		vo_ptPredis.clear();
		vo_ptPredis.reserve(oPredis.NPoints());
		//---------------------------------------------------------------------

		a = x_semiAxis;
		b = y_semiAxis;

		// Ponto inicial e final em coordenadas locais da elipse
		xini = (vpPoints[0]->X()-xc);
		yini = (vpPoints[0]->Y()-yc);

		xfim = (vpPoints[1]->X()-xc);
		yfim = (vpPoints[1]->Y()-yc);

		xcaux   = xc;
		ycaux   = yc;

		if(startAngle == endAngle || (endAngle-startAngle)==360 ) {
			dThetaTotal = 2*ctePI;
			nPoints		= oPredis.NPoints()+1;
		} else {
			// Calculo dos angulos correspondentes aos pontos inicial e final em relaï¿½ï¿½o
			// ï¿½s coordenadas parametrizadas da elipse. Os angulos inicial e
			// final (startAngle e endAngle) sï¿½o ï¿½ngulos fornecidos pelo usuï¿½rio como mï¿½todo
			// construtivo.
			// ï¿½ngulos em radianos.

			if(startAngle == 0)
				angleIni = 0;
			else if(startAngle == 90 || startAngle == -90)
				angleIni = ctePI/2;
			else if(startAngle == 180)
				angleIni = ctePI;
			else {
				if((xini/a) > 1. || (xini/a) <  -1.) {
					//mensagem de erro, usar matherr: ver help acos
				} else
					angleIni = acos(xini/a);
			}

			if(endAngle == 0)
				angleFim = 0;
			else if(endAngle == 90 || endAngle == -90)
				angleFim = ctePI/2;
			else if(endAngle == 180)
				angleFim = ctePI;
			else {
				if((xfim/a) > (double) 1 || (xfim/a) < (double) -1) {
					// mensagem de erro, usar matherr: ver help acos
				} else
					angleFim = acos(xfim/a);
			}

			// 3o. quadrante e 4o. quadrantes
			if((xini<=0 && yini<=0) || (xini>=0 && yini<=0)) angleIni = 2*ctePI-angleIni;
			if((xfim<=0 && yfim<=0) || (xfim>=0 && yfim<=0)) angleFim = 2*ctePI-angleFim;

			if (angleIni > angleFim) angleFim += 2*ctePI;

			dThetaTotal = fabs( angleFim - angleIni);
			nPoints		= oPredis.NPoints();
		}

		//  Inclui pontos na lista que sï¿½o alterados na funï¿½ï¿½o que calcula
		//  os pontos de prï¿½-disretizaï¿½ï¿½o
		//  Usando o tamanho do vetor a funï¿½ï¿½o de desenho sabe quantos pontos desenhar:
		//	no caso de elipse ou circunferï¿½ncia inteira, ï¿½ calculado um ponto a mais
		//	pois os pontos extremos coincidem
		for(j=0;j<nPoints-2;j++) vo_ptPredis.push_back(o_paux);

		dThetaInc = dThetaTotal/(nPoints-1);


		x_previous = xini;
		y_previous = yini;

		cosdeltheta = cos(dThetaInc);
		sindeltheta = sin(dThetaInc);

		for(i=0;i<nPoints-2;i++)
		{
			x = x_previous*cosdeltheta - (a/b)*y_previous*sindeltheta;
			y = (b/a)*x_previous*sindeltheta + y_previous*cosdeltheta;

			// Para evitar erros de aproximacoes com calculos com ponto flutuante
			// Pontos nos semieixos
			if( isnan(fabs(fabs(x)-a))/* < 1e-6*/)
				y = 0.;
			if( isnan(fabs(fabs(y)-b))/* < 1e-6*/)
				x = 0;

			vo_ptPredis[i].X(x+xcaux);
			vo_ptPredis[i].Y(y+ycaux);

			x_previous = x;
			y_previous = y;
		}

		oPredis.SetPredisPoints(vo_ptPredis);
	}
}


//-----------------------------------------------------------------------------------------------------
//	Save
//-----------------------------------------------------------------------------------------------------
//
//	Descriï¿½ï¿½o: grava dados de uma elipse para um arquivo.
//
//	parï¿½metros:	FILE * parq: ponteiro para arquivo
//				int unidade: unidade de comprimento
//				FILENAME tipoArq:	qual arquivo estï¿½ sendo gravado
//									- .ell: MODEL_CONNECTION
//									- .mgp: MODEL_GEOMETRY_PREDIS
//	retorno:	int: 0 - sucesso
//
//
//	Observaï¿½ï¿½es: O argumento FILENAME tipoArq ï¿½ utilizado para diferenciaï¿½ï¿½o na gravaï¿½ï¿½o
//				de dados de prï¿½-discretizaï¿½ï¿½o dos arquivos .ell e .mgp. Quando a gravaï¿½ï¿½o de
//				arquivos do tipo .ell forem abandonados, este parï¿½metro pode ser eliminado.
//
//......................................................................................................
//	Versï¿½o:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Fevereiro/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevEllipse::Save(FILE * parq, int unidade, FILENAME tipoArq)
{
	int retCod = 0;
	double angleIni = startAngle;
	double angleFim = endAngle;

	if(startAngle == endAngle){ angleIni = 0.; angleFim = 360;}


	if(tipoArq == MODEL_GEOMETRY_PREDIS)	//(.mgp) //*NMA*: arquivo novo (merge .ell + pre)
	{
		if(x_semiAxis == y_semiAxis)
		{
			if(isOfGeometry)
				fprintf(parq,"\n\n%s %d","circunferencia",Id());
			else
				fprintf(parq,"\n\n%s %d","circunferencia_auxiliar",Id());

			 fprintf(parq,"\n\n%g", xc*pow(10,unidade));
			 fprintf(parq,"\n\n%g", yc*pow(10,unidade));
			 fprintf(parq,"\n\n%g", x_semiAxis*pow(10,unidade));
			 fprintf(parq,"\n\n%g", angleIni);
			 fprintf(parq,"\n\n%g", angleFim);
		} else {
			if(isOfGeometry)
				fprintf(parq,"\n\n%s %d","elipse",Id());
			else
				fprintf(parq,"\n\n%s %d","elipse_auxiliar",Id());

			 fprintf(parq,"\n\n%g", xc*pow(10,unidade));
			 fprintf(parq,"\n\n%g", yc*pow(10,unidade));
			 fprintf(parq,"\n\n%g", x_semiAxis*pow(10,unidade));
			 fprintf(parq,"\n\n%g", y_semiAxis*pow(10,unidade));
			 fprintf(parq,"\n\n%g", angleIni);
			 fprintf(parq,"\n\n%g", angleFim);
		}

		//coordenadas dos pontos inicial e final
		fprintf(parq,"\n\n%g", vpPoints[0]->X()*pow(10,unidade));
		fprintf(parq,"\n\n%g", vpPoints[0]->Y()*pow(10,unidade));
		fprintf(parq,"\n\n%g", vpPoints[1]->X()*pow(10,unidade));
		fprintf(parq,"\n\n%g", vpPoints[1]->Y()*pow(10,unidade));

		//Informaï¿½ï¿½es de Pre-discretizaï¿½ï¿½o
		fprintf(parq,"\n\n%d",  oPredis.NPoints());
		fprintf(parq,"\n\n%g", oPredis.Factor());
		fprintf(parq,"\n\n%g", oPredis.XClicked());
		fprintf(parq,"\n\n%g", oPredis.YClicked());
	} else if(tipoArq == MODEL_CONNECTION)	{ // (.ell)
		if(x_semiAxis == y_semiAxis) {
			fprintf(parq,"\n\n%s %d","circunferencia",Id());
			fprintf(parq,"\n\n%g", xc);
			fprintf(parq,"\n\n%g", yc);
			fprintf(parq,"\n\n%g", x_semiAxis);
			fprintf(parq,"\n\n%g", angleIni);
			fprintf(parq,"\n\n%g", angleFim);
		} else {
			fprintf(parq,"\n\n%s %d","elipse",Id());
			fprintf(parq,"\n\n%g", xc);
			fprintf(parq,"\n\n%g", yc);
			fprintf(parq,"\n\n%g", x_semiAxis);
			fprintf(parq,"\n\n%g", y_semiAxis);
			fprintf(parq,"\n\n%g", angleIni);
			fprintf(parq,"\n\n%g", angleFim);
		}

		if(x_semiAxis != y_semiAxis) {
			fprintf(parq,"\n\n%d"  , 0);   // informaï¿½ï¿½o nï¿½o identificada
			fprintf(parq,"\n\n%d"  , oPredis.NPoints());  //numero de pontos de prediscretizacao
												// *NMA* deixado como 0, para manter consistencia para
												// leitura deste arquivo pelo edimaq. Nestea versï¿½o nï¿½o
												// estï¿½o sendo gravados dados para recuperaï¿½ï¿½o de pre-discretizaï¿½ï¿½o
												// para o edimaq
		} else
			fprintf(parq,"\n\n%d"  , oPredis.NPoints());  //numero de pontos de prediscretizacao
												// *NMA* deixado como 0, para manter consistencia para
												// leitura deste arquivo pelo edimaq. Nestea versï¿½o nï¿½o
												// estï¿½o sendo gravados dados para recuperaï¿½ï¿½o de pre-discretizaï¿½ï¿½o
												// para o edimaq
	}
	return retCod;
}


//-----------------------------------------------------------------------------------------------------
//	Read
//-----------------------------------------------------------------------------------------------------
//
//	Descriï¿½ï¿½o: lï¿½ dados de uma elipse de um arquivo
//
//	parï¿½metros:	FILE * parq: ponteiro para arquivo
//				int unidade: unidade de comprimento
//				char *tp   : string com tipo da primitiva
//
//	retorno:	int: 0 - sucesso
//
//
//	Observaï¿½ï¿½es: O vetor de ponteiros para pontos jï¿½ deve estar preenchido,
//					para poder somente ser alterado.
//
//......................................................................................................
//	Versï¿½o:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Fevereiro/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevEllipse::Read(FILE * parq, int unidade, char *tp, FILENAME tipoArq)
{
	char		elipse[] ="elipse";
	char		circ[]	 ="circunferencia";

	char		elipse_aux[] ="elipse_auxiliar";
	char		circ_aux[]	 ="circunferencia_auxiliar";

	double		xcl, ycl, semiEixoX, semiEixoY, angleIni, angleFim;
	double		xp1,yp1, xp2, yp2;
	// variï¿½veis utilizadas na leitura dos dados de prï¿½-discretizaï¿½ï¿½o
	int			nPoints;
	double		x_clicked,y_clicked, factor;
	CLevPoint	o_pointclicked;

//	if(!strcmp(tp,circ))
	if(!strcmp(tp,circ) || !strcmp(tp,circ_aux))
	{
		 fscanf(parq,"\n%lf", &xcl);
		 fscanf(parq,"\n%lf", &ycl);
		 fscanf(parq,"\n%lf", &semiEixoX);
		 fscanf(parq,"\n%lf", &angleIni);
		 fscanf(parq,"\n%lf", &angleFim);
		 semiEixoY = semiEixoX;
		 if(!strcmp(tp,circ_aux))
			 isOfGeometry = false;
	}
	else
	{
		 fscanf(parq,"\n%lf", &xcl);
		 fscanf(parq,"\n%lf", &ycl);
		 fscanf(parq,"\n%lf", &semiEixoX);
		 fscanf(parq,"\n%lf", &semiEixoY);
		 fscanf(parq,"\n%lf", &angleIni);
		 fscanf(parq,"\n%lf", &angleFim);
		 if(!strcmp(tp,elipse_aux))
			 isOfGeometry = false;
	}


	if(tipoArq == MODEL_GEOMETRY_PREDIS)	//(.mgp) //*NMA*: arquivo novo (merge .ell + pre)
	{
		// *NMA*: As coordenadas no arquivo MGP sï¿½o armazendas em metros
		xc = xcl*pow(10,-unidade);
		yc = ycl*pow(10,-unidade);
		x_semiAxis = semiEixoX*pow(10,-unidade);
		y_semiAxis = semiEixoY*pow(10,-unidade);
		startAngle = angleIni;
		endAngle   = angleFim;

		//coordenadas dos pontos inicial e final
		fscanf(parq,"\n%lf", &xp1);
		fscanf(parq,"\n%lf", &yp1);
		fscanf(parq,"\n%lf", &xp2);
		fscanf(parq,"\n%lf", &yp2);

		xp1 *= pow(10,-unidade);
		yp1 *= pow(10,-unidade);
		xp2 *= pow(10,-unidade);
		yp2 *= pow(10,-unidade);

		//Informaï¿½ï¿½es de Pre-discretizaï¿½ï¿½o
		fscanf(parq,"\n%d", &nPoints);
		fscanf(parq,"\n%lf", &factor);
		fscanf(parq,"\n%lf", &x_clicked);
		fscanf(parq,"\n%lf", &y_clicked);
		oPredis.SetPreDiscretizationData(nPoints,factor,x_clicked,y_clicked);
	}
	else
	if(tipoArq == MODEL_CONNECTION)		//(.ell)
	{

		// *NMA*: As coordenadas no arquivo ELL sï¿½o armazendas na unidade especificada
		xc = xcl;
		yc = ycl;
		x_semiAxis = semiEixoX;
		y_semiAxis = semiEixoY;
		startAngle = angleIni;
		endAngle   = angleFim;

		// Calculo das coordenadas dos pontos vï¿½rtices
		Locate_Extremity_Coordinates(startAngle,xp1,yp1);
		Locate_Extremity_Coordinates(endAngle,xp2,yp2);

		fscanf(parq,"\n\n%d"  , &nPoints);   //numero de pontos de prediscretizacao
										// *NMA* deixado como 0, para manter consistencia para
										// leitura deste arquivo pelo edimaq. Nestea versï¿½o nï¿½o
										// estï¿½o sendo gravados dados para recuperaï¿½ï¿½o de pre-discretizaï¿½ï¿½o
										// para o edimaq
		if(!strcmp(tp,elipse))
		fscanf(parq,"\n\n%d"  , &nPoints);   //mais uma informaï¿½ï¿½o que nï¿½o identifiquei o que ï¿½

		oPredis.SetPreDiscretizationData(nPoints,1.0, 0, 0);

	}


//	O vetor de ponteiros para pontos jï¿½ deve estar preenchido,
//			para poder somente ser alterado.

	vpPoints[0]->X(xp1);
	vpPoints[0]->Y(yp1);

	vpPoints[1]->X(xp2);
	vpPoints[1]->Y(yp2);

	vpPoints[2]->X(xc);
	vpPoints[2]->Y(yc);

	vpPoints[3]->X(x_semiAxis);
	vpPoints[3]->Y(yc);

	vpPoints[4]->X(xc);
	vpPoints[4]->Y(y_semiAxis);

	if(*vpPoints[0] != *vpPoints[1])
	{
		geoEntityType = ARCLEV;
	}

	return 0;

}// end of Read

void CLevEllipse::ReflectionByX(void)
{

	CLevPoint p0,p1;


	if(!fixedEntity)
	{
		p0.X(vpPoints[0]->X());
		p0.Y(vpPoints[0]->Y());
		p1.X(vpPoints[1]->X());
		p1.Y(vpPoints[1]->Y());
		//vpPoints[0]->ReflectionByX();
		//vpPoints[1]->ReflectionByX();
		//vpPoints[2]->ReflectionByX();

		vpPoints[2]->ReflectionByX();

		xc = vpPoints[2]->X();
		yc = vpPoints[2]->Y();

		vpPoints[3]->Y(yc);
		vpPoints[4]->X(xc);

		if( (vpPoints[0]->X() == vpPoints[1]->X()) && (vpPoints[0]->Y() == vpPoints[1]->Y()) )
		{
			p0.ReflectionByX();

			startAngle = endAngle = Get_Point_Angle(&p0);

			vpPoints[0]->X(p0.X());
			vpPoints[0]->Y(p0.Y());

			vpPoints[1]->X(p0.X());
			vpPoints[1]->Y(p0.Y());

		}
		else
		{
			p0.ReflectionByX();
			p1.ReflectionByX();

			startAngle = Get_Point_Angle(&p1);
			endAngle   = Get_Point_Angle(&p0);



			vpPoints[0]->X(p1.X());
			vpPoints[0]->Y(p1.Y());

			vpPoints[1]->X(p0.X());
			vpPoints[1]->Y(p0.Y());

		}


	}
	TakeLimits();
}
void CLevEllipse::ReflectionByY(void)
{
	CLevPoint p0,p1;


	if(!fixedEntity)
	{
		p0.X(vpPoints[0]->X());
		p0.Y(vpPoints[0]->Y());
		p1.X(vpPoints[1]->X());
		p1.Y(vpPoints[1]->Y());
		//vpPoints[0]->ReflectionByX();
		//vpPoints[1]->ReflectionByX();
		//vpPoints[2]->ReflectionByX();

		vpPoints[2]->ReflectionByY();
		xc = vpPoints[2]->X();
		yc = vpPoints[2]->Y();

		vpPoints[3]->Y(yc);
		vpPoints[4]->X(xc);

		if( (vpPoints[0]->X() == vpPoints[1]->X()) && (vpPoints[0]->Y() == vpPoints[1]->Y()) )
		{
			p0.ReflectionByY();


			startAngle = endAngle = Get_Point_Angle(&p0);

			vpPoints[0]->X(p0.X());
			vpPoints[0]->Y(p0.Y());

			vpPoints[1]->X(p0.X());
			vpPoints[1]->Y(p0.Y());

		}
		else
		{
			p0.ReflectionByY();
			p1.ReflectionByY();

			startAngle = Get_Point_Angle(&p1);
			endAngle   = Get_Point_Angle(&p0);


#if 0
			vpPoints[0]->X(p0.X());
			vpPoints[0]->Y(p0.Y());

			vpPoints[1]->X(p1.X());
			vpPoints[1]->Y(p1.Y());
#endif

			vpPoints[0]->X(p1.X());
			vpPoints[0]->Y(p1.Y());

			vpPoints[1]->X(p0.X());
			vpPoints[1]->Y(p0.Y());


		}


	}
	TakeLimits();
}

//-----------------------------------------------------------------------------------------------------
//	GetMiddlePoint(void)
//-----------------------------------------------------------------------------------------------------
//
//	Descriï¿½ï¿½o: Calcula ponto mï¿½dio da ellipse
//
//	parï¿½metros:
//
//	retorno:	CLevPoint
//
//
//	Observaï¿½ï¿½es:
//......................................................................................................
//	Versï¿½o:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Abril/2004
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
CLevPoint CLevEllipse::GetMiddlePoint(void)
{
	CLevPoint middle;
	double    xm, ym;
	double	angleIni, angleFim;
	double  angleMiddle;

	//Acha o angulo correspondente ao ponto mï¿½dio
	angleIni = startAngle;
	angleFim = endAngle;

	//verificar se os angulos estï¿½o entre 0 e 360
	if(startAngle == endAngle || (endAngle-startAngle)==360)
	{
		angleIni = 0;
		angleFim = 360;
	}
	else
	{
		if(startAngle < 0) angleIni +=360;
		if(endAngle <= 0)   angleFim +=360;
	}

	// verifica se o angulo inicial ï¿½ maior do que o final
	if( angleIni < angleFim )
	{
		angleMiddle = (angleFim - angleIni)/2; //tamanho do arco dividido por 2

		angleMiddle += angleIni;

	}
	else
	{ //angulo inicial ï¿½ maior do que o final
		double angleAux;
		//calcula quantos graus do angulo inicial atï¿½ 360
//		angleAux = 360 - angleFim;
		angleAux = angleFim -360;

		angleMiddle = (angleAux + angleIni)/2;
	}

	Locate_Crossing_Coordinates(angleMiddle,xm,ym);

	middle.X(xm);
	middle.Y(ym);


	return middle;

}// fim de CLevPoint CLevEllipse::GetMiddlePoint(void)


