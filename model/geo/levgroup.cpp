//=============================================================================
//	Arquivo:			LevGroup.cpp
//.............................................................................
//	Descricao:			Implementacao da classe CLevGroup
//.............................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//.............................................................................
//	Observacoes:
//
//=============================================================================
#if 10
#include "levgroup.h"

////////////////////////
//The constructor function
////////////////////////
CLevGroup::CLevGroup()
{
	pt_minxminy.X(1e308); pt_minxminy.Y(1e308); pt_minxminy.Z(1e308);
	pt_maxxmaxy.X(-1e308); pt_maxxmaxy.Y(-1e308); pt_maxxmaxy.Z(1e308);

}


////////////////////////
//The destructor function
////////////////////////
CLevGroup::~CLevGroup()
{
	vpPoints.clear();
	vpLines.clear();
	vpEllipses.clear();
}

//-----------------------------------------------------------------------------------------------------
//	Move()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o:
//
//	par�metros:
//
//	retorno:
//
//	Observa��es:
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			Abril/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevGroup::Move(double deltax, double deltay, double deltaz)
{
	unsigned int i;

	for (i = 0; i < vpPrimitives.size(); i++) {
		vpPrimitives[i]->Move(deltax, deltay, deltaz);
		vpPrimitives[i]->FixEntity(true);
	}

	for (i = 0; i < vpPrimitives.size(); i++)
		vpPrimitives[i]->FixEntity(false);

	TakeLimits();
}

void CLevGroup::ReflectionByY(void)
{
	unsigned int i;

	for (i = 0; i < vpPrimitives.size(); i++) {
		vpPrimitives[i]->ReflectionByY();
		vpPrimitives[i]->FixEntity(true);
	}

	for (i = 0; i < vpPoints.size(); i++)
		vpPoints[i]->ReflectionByY();

	for (i = 0; i < vpPrimitives.size(); i++)
		vpPrimitives[i]->FixEntity(false);

	TakeLimits();
}


void CLevGroup::ReflectionByX(void)
{
	unsigned int i;

	for (i = 0; i < vpPrimitives.size(); i++) {
		vpPrimitives[i]->ReflectionByX();
		vpPrimitives[i]->FixEntity(true);
	}

	for (i = 0; i < vpPoints.size(); i++)
		vpPoints[i]->ReflectionByX();

	for (i = 0; i < vpPrimitives.size(); i++)
		vpPrimitives[i]->FixEntity(false);
}


void CLevGroup::Rotation(double angle, CLevPoint &pointc)
{
	unsigned int i;

	for (i = 0; i < vpPrimitives.size(); i++) {
		vpPrimitives[i]->Rotation(angle, pointc);
		vpPrimitives[i]->FixEntity(true);
	}

	for (i = 0; i < vpPoints.size(); i++)
		vpPoints[i]->Rotation(angle, pointc);

	for (i = 0; i < vpPrimitives.size(); i++)
		vpPrimitives[i]->FixEntity(false);

	TakeLimits();
}


////////////////////////
//The set_edges() function
////////////////////////

void CLevGroup::SetLines(vpCLevLine l)
{
	vpLines = l;
	TakeLimits();
}


void CLevGroup::SetEllipses(vpCLevEllipse e)
{
	vpEllipses = e;
	TakeLimits();
}


////////////////////////////////////////////////////////////////////////////////////
// as pr�ximos tres m�todos permitem incluir novas linhas e pontos � lista do poligono
// Devem ser utilizadas enquanto constroi-se um novo poligono.
// Por consist�ncia o pol�gono s� � considerado v�lido quando o �ltimo e o primeiro
// pontos coincidem!
////////////////////////////////////////////////////////////////////////////////////
int CLevGroup::IsAPoint(CLevPoint  *p)
{	// valor de retorno: -1 se ponto *p n�o pertence � lista local
	//                  >=0 indica posi��o na lista
	int retorno = -1;
	for (unsigned int i = 0; i < vpPoints.size(); i++) {
		if(vpPoints[i]->Status())
			if (vpPoints[i] == p) {
				retorno = i;
				break;
			}
	}
	return retorno;
}


int CLevGroup::IsALine(CLevLine  *l)
{
	// valor de retorno: -1 se linha *l n�o pertence � lista local
	//                  >=0 indica posi��o na lista
	int retorno = -1;
	for (unsigned int i = 0; i < vpPrimitives.size(); i++)
		if(vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == LINELEV)
			if (vpPrimitives[i] == l) {
				retorno = i;
				break;
			}

	return retorno;
}

int CLevGroup::IsAnEllipse(CLevEllipse  *e)
{	// valor de retorno: -1 se linha *l n�o pertence � lista local
	//                  >=0 indica posi��o na lista

	int retorno = -1;

	for (unsigned int i = 0; i < vpPrimitives.size(); i++)
		if(vpPrimitives[i]->Status() && vpPrimitives[i]->GeoEntityType() == ELLIPSELEV)
			if (vpPrimitives[i] == e) {
				retorno = i;
				break;
			}

	return retorno;
}


int CLevGroup::IsAPrimitive(CLevPrimitive  *p)
{	// valor de retorno: -1 se a primitiva *p n�o pertence � lista local
	//                  >=0 indica posi��o na lista
	int retorno = -1;
	for (unsigned int i = 0; i < vpPrimitives.size(); i++)
		if(vpPrimitives[i]->Status() )
			if (vpPrimitives[i] == p) {
				retorno = i;
				break;
			}

	return retorno;
}


//-----------------------------------------------------------------------------------------------------
//	IncludeNextPoint()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o: Inclui um ponto no vetor de pontos que formam o grupo.
//
//
//	par�metros:	CLevPoint *
//
//	retorno:		-1 se o ponto *e j� existia na lista local
//					>=0 indica posi�ao de inclus�o do novo ponto na lista
//
//	Observa��es:
//					A fun��o inclui na lista de pontos vpPoints, somente
//					pontos isolados, que n�o s�o v�rtices de nenhuma das
//					primitivas j� inclu�das no grupo.
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			14/09/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGroup::IncludeNextPoint(CLevPoint  *p)
{
	// valor de retorno : -1 se o ponto *p j� existia na lista local
	//                   >=0 indica posi�ao de inclus�o da nova linha na lista
	int retorno;
	unsigned int i;
	bool isIncluded = false;

	// verifica se o ponto � vertice de alguma linha
	for (i = 0; i < vpLines.size() && isIncluded == false; i++) {
		if(vpLines[i]->vpPoints[0] == p) {
			isIncluded = true;
			retorno = -1;
		} else {
			if(vpLines[i]->vpPoints[1] == p) {
				isIncluded = true;
				retorno = -1;
			}
		}
	}

	// verifica se o ponto � vertice de alguma elipse
	for (i = 0; i < vpEllipses.size() && isIncluded == false; i++) {
		if(vpEllipses[i]->vpPoints[0] == p) {
			isIncluded = true;
			retorno = -1;
		} else {
			if(vpEllipses[i]->vpPoints[1] == p) {
				isIncluded = true;
				retorno = -1;
			}
		}
	}

	// inclui na lista de pontos somente os pontos que n�o s�o v�rtices de
	// alguma das primitivas j� inclu�das no grupo
	 if(isIncluded == false) {
		if (!vpPoints.size())
		{  // ainda n�o havia ponto na lista
			vpPoints.push_back(p);
			retorno = vpPoints.size()-1;
		} else {
			retorno = IsAPoint(p);
			if (retorno == -1) {
			   vpPoints.push_back(p);
			   retorno = vpPoints.size()-1;
			}
		}
	 }

	 return retorno;
}


//-----------------------------------------------------------------------------------------------------
//	IncludeNextPrimitive()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o: Inclui uma primitiva no vetor de primitivas que formam o grupo.
//
//
//	par�metros:	CLevPrimitive *
//
//	retorno:		-1 se a primitiva *p j� existia na lista local
//					>=0 indica posi�ao de inclus�o da nova primitiva na lista
//
//	Observa��es:  A fun��o chama DeletePoint, para eliminar da lista de
//					pontos isolados, os v�rtices da primitiva que, eventualmente,
//					tenham sido inseridos anteriormente
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			14/09/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGroup::IncludeNextPrimitive(CLevPrimitive  *p)
{
	int retorno;

	if (!vpPrimitives.size())
	{  // ainda n�o havia primitiva na lista
		vpPrimitives.push_back(p);
		retorno = vpPrimitives.size()-1;
	} else {
		retorno = IsAPrimitive(p);
		if (retorno == -1) {
		   vpPrimitives.push_back(p);
		   retorno = vpPrimitives.size()-1;
		}
	}

	if(retorno != -1) // a linha foi inclu�da
	{
		//Elimina pontos inclu�dos anteriormente de maneira isolada,
		// mas que s�o v�rtices da nova primitiva.
		DeletePoint(vpPrimitives[retorno]->vpPoints[0]);
		DeletePoint(vpPrimitives[retorno]->vpPoints[1]);
	}

	TakeLimits();

	return retorno;
}


//-----------------------------------------------------------------------------------------------------
//	IncludeNextLine()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o: Inclui uma linha no vetor de linhas que formam o grupo.
//
//
//	par�metros:	CLevLine *
//
//	retorno:		-1 se a linha *l j� existia na lista local
//					>=0 indica posi�ao de inclus�o da nova linha na lista
//
//	Observa��es:  A fun��o chama DeletePoint, para eliminar da lista de
//					pontos isolados, os v�rtices da linhas que, eventualmente,
//					tenham sido inseridos anteriormente
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			14/09/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGroup::IncludeNextLine(CLevLine  *l)
{
	int retorno;

	if (!vpPrimitives.size())
	{  // ainda n�o havia primitiva na lista
		vpPrimitives.push_back(l);
		retorno = vpPrimitives.size()-1;
	}
	else
	{
		retorno = IsALine(l);
		if (retorno == -1)
		{
		   vpPrimitives.push_back(l);
		   retorno = vpPrimitives.size()-1;
		}
	}

	if(retorno != -1) // a linha foi inclu�da
	{
		//Elimina pontos inclu�dos anteriormente de maneira isolada,
		// mas que s�o v�rtices da nova primitiva.
		DeletePoint(vpPrimitives[retorno]->vpPoints[0]);
		DeletePoint(vpPrimitives[retorno]->vpPoints[1]);
	}

	TakeLimits();

	return retorno;
}


//-----------------------------------------------------------------------------------------------------
//	IncludeNextElipse()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o: Inclui uma elipse no vetor de elipses que formam o grupo.
//
//
//	par�metros:	CLevElipse *
//
//	retorno:		-1 se a elipse *e j� existia na lista local
//					>=0 indica posi�ao de inclus�o da nova elipse na lista
//
//	Observa��es:  A fun��o chama DeletePoint, para eliminar da lista de
//					pontos isolados, os v�rtices da elipses que, eventualmente,
//					tenham sido inseridos anteriormente
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			14/09/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CLevGroup::IncludeNextEllipse(CLevEllipse  *e)
{
	// valor de retorno : -1 se a linha *l j� existia na lista local
	//                   >=0 indica posi�ao de inclus�o da nova linha na lista
	int retorno;

	if (!vpPrimitives.size())
	{  // ainda n�o havia primitivas na lista
		vpPrimitives.push_back(e);
		retorno = vpPrimitives.size()-1;
	}
	else
	{
		retorno = IsAnEllipse(e);
		if (retorno == -1)
		{
		   vpPrimitives.push_back(e);
		   retorno = vpPrimitives.size()-1;
		}
	}

	if(retorno != -1) // a elipse foi inclu�da
	{
		//Elimina pontos inclu�dos anteriormente de maneira isolada,
		// mas que s�o v�rtices da nova primitiva.
		DeletePoint(vpPrimitives[retorno]->vpPoints[0]);
		DeletePoint(vpPrimitives[retorno]->vpPoints[1]);
	}

	TakeLimits();

	return retorno;

}


//-----------------------------------------------------------------------------------------------------
//	DeletePoint()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o: Exclui um ponto do vetor de pontos que formam o grupo.
//
//
//	par�metros:	CLevPoint *
//
//	retorno:
//
//	Observa��es: A exclus�o � feita de fato porque n�o � poss�vel setar a
//				primitiva como inv�lida. O vetor original � copiado em um
//				vetor tempor�rio, logo ap�s � limpo, e s�o inseridas
//				as linhas constantes do vetor tempor�rio com excess�o da linha exclu�da.
//				Tentei usar erase() do vector STL, mas deu erro.
//
//				Elimina ponto somente da lista de pontos vpPoints, o que
//				significa que este ponto � n�o � v�rtice de nenhuma das
//				primitivas.
//
//				Na inclus�o das primitivas, esta fun��o � chamada para eliminar
//				pontos inclu�dos separadamente, mas que s�o v�rtices da nova primitiva.
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			14/09/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevGroup::DeletePoint(CLevPoint  *p)
{
	unsigned int i;
	int pos;
	vpCLevPoint temp;

	pos = IsAPoint(p);

	if(pos != -1) {
		temp = vpPoints;
		vpPoints.clear();
		for(i=0;i<temp.size();i++)
			if((signed)i != pos)
				vpPoints.push_back(temp[i]);
	}
}


//-----------------------------------------------------------------------------------------------------
//	DeleteEllipse()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o: Exclui uma elipse do vetor de linhas que forma o grupo.
//
//
//	par�metros:	CLevEllipse *
//
//	retorno:
//
//	Observa��es: A exclus�o � feita de fato porque n�o � poss�vel setar a
//				primitiva como inv�lida. O vetor original � copiado em um
//				vetor tempor�rio, logo ap�s � limpo, e s�o inseridas
//				as linhas constantes do vetor tempor�rio com excess�o da linha exclu�da.
//				Tentei usar erase() do vector STL, mas deu erro.
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			21/05/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevGroup::DeleteEllipse(CLevEllipse  *e)
{
	unsigned int i;
	int pos;
	vpCLevEllipse temp;

	pos = IsAnEllipse(e);

	if(pos != -1) {
		temp = vpEllipses;
		vpEllipses.clear();

		for(i=0;i<temp.size();i++)
			if((signed)i != pos)
				vpEllipses.push_back(temp[i]);
	}
}


//-----------------------------------------------------------------------------------------------------
//	DeleteLine()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o: Exclui uma linha do vetor de linhas que forma o grupo.
//
//
//	par�metros:	CLevLine *
//
//	retorno:
//
//	Observa��es: A exclus�o � feita de fato porque n�o � poss�vel setar a
//				primitiva como inv�lida. O vetor original � copiado em um
//				vetor tempor�rio, logo ap�s � limpo, e s�o inseridas
//				as linhas constantes do vetor tempor�rio com excess�o da linha exclu�da.
//				Tentei usar erase() do vector STL, mas deu erro.
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			21/05/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevGroup::DeleteLine(CLevLine  *l)
{
	unsigned int i;
	int pos;
	vpCLevLine temp;

	pos = IsALine(l);

	if(pos != -1) {
		temp = vpLines;
		vpLines.clear();

		for(i=0;i<temp.size();i++)
			if((signed)i != pos)
				vpLines.push_back(temp[i]);
	}
}


//-----------------------------------------------------------------------------------------------------
//	DeletePrimitive()
//-----------------------------------------------------------------------------------------------------
//
//	Descri��o: Exclui uma primitiva do vetor de primitivas que forma o grupo.
//
//
//	par�metros:	CLevPrimitive *
//
//	retorno:
//
//	Observa��es: A exclus�o � feita de fato porque n�o � poss�vel setar a
//				primitiva como inv�lida. O vetor original � copiado em um
//				vetor tempor�rio, logo ap�s � limpo, e s�o inseridas
//				as linhas constantes do vetor tempor�rio com excess�o da linha exclu�da.
//				Tentei usar erase() do vector STL, mas deu erro.
//
//......................................................................................................
//	Vers�o:			1.0
//	Autor:			Angelo Passaro
//					Nancy Mieko Abe
//	Data:			21/05/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevGroup::DeletePrimitive(CLevPrimitive  *p)
{
	unsigned int i;
	int pos;
	vpCLevPrimitive temp;

	pos = IsAPrimitive(p);

	if(pos != -1) {
		temp = vpPrimitives;
		vpPrimitives.clear();

		for(i=0;i<temp.size();i++)
			if((signed)i != pos)
				vpPrimitives.push_back(temp[i]);
	}
}


void CLevGroup::Reinitialize()
{
	vpPoints.clear();
	vpLines.clear();
	vpEllipses.clear();
	vpPrimitives.clear();

	pt_minxminy.X(1e308); pt_minxminy.Y(1e308); pt_minxminy.Z(1e308);
	pt_maxxmaxy.X(-1e308); pt_maxxmaxy.Y(-1e308); pt_maxxmaxy.Z(1e308);

}


int CLevGroup::NumValidLines(void)
{
	unsigned int i;
	int num = 0;

	for(i=0; i<vpPrimitives.size(); i++)
		if(vpPrimitives[i]->GeoEntityType() == LINELEV)
			num ++;

	return num;
}


int CLevGroup::NumValidCurves(void)
{
	int num = 0;
	unsigned int i;

	for(i=0; i< vpPrimitives.size(); i++)
		if(vpPrimitives[i]->GeoEntityType() == ELLIPSELEV )
			num ++;

	return num;
}


int CLevGroup::NumValidPoints(void)
{
	int num = 0;
	unsigned int i;

	for(i=0; i< vpPoints.size(); i++)
		if(vpPoints[i]->Status())
			num ++;

	return num;
}


int CLevGroup::NumValidPrimitives(void)
{
	unsigned int i;
	int num = 0;

	for(i=0; i< vpPrimitives.size(); i++)
		if(vpPrimitives[i]->Status())
			num ++;

	return num;
}


CLevGroup & CLevGroup::operator = (const CLevGroup & ro_group)
{

	if (this != &ro_group)
	{

		status				= ro_group.status;
		pt_minxminy			= ro_group.pt_minxminy;
		pt_maxxmaxy			= ro_group.pt_maxxmaxy;

		vpEllipses			= ro_group.vpEllipses;
		vpLines				= ro_group.vpLines;
		vpPoints			= ro_group.vpPoints;
		vpPrimitives		= ro_group.vpPrimitives;

		geoEntityType		= ro_group.geoEntityType;

	}

	return *this;

}

bool CLevGroup::operator == ( CLevGroup &a)
{
	unsigned int i;
	int ret = -1;
	bool retorno = true;

	if(NumValidLines() == a.NumValidLines() && NumValidCurves() == a.NumValidCurves()) {
		for(i=0;i<vpPrimitives.size() && retorno == true;i++) {
			if(vpPrimitives[i]->Status()  ) {
				ret = a.IsAPrimitive(vpPrimitives[i]);
				if(ret==-1)
					retorno = false;
			}
		}
	}

	return retorno;
}


//*NMA*: opera��o de escalamento
void CLevGroup::Scale(double factor)
{
	unsigned int i;

	for(i=0;i<vpPrimitives.size();i++)
		vpPrimitives[i]->Scale(factor);

	for(i=0;i<vpPrimitives.size();i++)
		vpPrimitives[i]->FixEntity(false);

	for(i=0;i<vpPoints.size();i++)
		vpPoints[i]->FixEntity(false);
}


void CLevGroup::Limits(CLevPoint & p1, CLevPoint & p2)
{
	p1 = pt_minxminy;
	p2 = pt_maxxmaxy;
}


void CLevGroup::TakeLimits()
{
	CLevPoint pMin,pMax;

	unsigned int i;

	pt_minxminy.X(1e308); pt_minxminy.Y(1e308); pt_minxminy.Z(1e308);
	pt_maxxmaxy.X(-1e308); pt_maxxmaxy.Y(-1e308); pt_maxxmaxy.Z(1e308);

	if(vpPrimitives.size()) {
		for(i=0; i<vpPrimitives.size(); i++) {
			if(vpPrimitives[i]->Status()) {
				vpPrimitives[i]->Limits(pMin,pMax);
				if ( pMin.X() <= pt_minxminy.X()) pt_minxminy.X(pMin.X());
				if ( pMax.X() >= pt_maxxmaxy.X()) pt_maxxmaxy.X(pMax.X());
				if ( pMin.Y() <= pt_minxminy.Y()) pt_minxminy.Y(pMin.Y());
				if ( pMax.Y() >= pt_maxxmaxy.Y()) pt_maxxmaxy.Y(pMax.Y());
			}
		}
	}
}


#endif
