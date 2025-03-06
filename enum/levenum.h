#ifndef __LEV_ENUM_H_
#define __LEV_ENUM_H_

//===================================================================================================
//	Arquivo:			LevEnum.h
//...................................................................................................
//	Descricao:			Contem a declaracaoo de enum utilizados pelo sistema
//...................................................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//...................................................................................................
//	Observa��es:
//...................................................................................................
//	Data:				Junho/2000
//	Modificado em:
//	Modificado por:
//	Motivo:
//===================================================================================================


namespace lev_enum {

enum  LEV_BOOL {NO = 0, YES};

}

enum LEV_OBJECTVALIDITY
{
	LEV_INVALID = 0,
	LEV_VALID,
	LEV_GROUP,
	LEV_FRONTIER
};

enum GEOMETRICOPERATION
{
	NOOPERATION = 0,
	DELETEOPERATION,
	MOVEOPERATION,
	INCLUDEOPERATION
};

enum OPERATIONCONTROL
{
	NONE = 0,
	MOUSEINCLUDELINE = 200001,
	MOUSEINCLUDELINEFIRSTPOINT,
	REFLECTIONMOUSEINCLUDELINEFIRSTPOINT,
	REFLECTION2POINTSKEYBORD,
	REFLECTIONGIVENLINE,
	REFLECTIONXAXIS,
	REFLECTIONYAXIS,
	REFLECTIONBYANGLE,
	REFLECTIONOPERATION,
	REFLECTION2POINTGIVENFIRSTPOINT,
	REFLECTION2POINTGIVENLASTPOINT,
	REFLECTION2POINTS,
	INCLUDELINEBYDIALOG,
	PRIMITIVEPARALLELLINES,
	PRIMITIVEPOLYGONBYMOUSE,
	PRIMITIVEPOLYGONBYSHIFTMOUSEPOINT,
	PRIMITIVEPOLYGONBYSHIFTMOUSELINE,
	PRIMITIVEPOLYGONBYMOUSEFREE,
	PRIMITIVEPOLYGONDONE,
	PRIMITIVEGROUPBYSHIFTMOUSE,
	PRIMITIVEGROUPREFLECTIONBYSHIFTMOUSE,
	PRIMITIVEGROUPBYRUBBERBAND,
	PRIMITIVELINESBYROTATION,
	PRIMITIVELINEBYREFLECTION,
	PRIMITIVELINEREFLECTION2POINTS,
	PRIMITIVELINEREFLECTIONYAXIS,
	PRIMITIVELINEREFLECTIONXAXIS,
	PRIMITIVEELLIPSEBYREFLECTION,
	PRIMITIVEELLIPSEREFLECTION2POINTS,
	PRIMITIVEELLIPSEREFLECTIONYAXIS,
	PRIMITIVEELLIPSEREFLECTIONXAXIS,
	PRIMITIVEGROUPBYREFLECTION,
	PRIMITIVEGROUPREFLECTION2POINTS,
	PRIMITIVEGROUPREFLECTIONYAXIS,
	PRIMITIVEGROUPREFLECTIONXAXIS,
	PRIMITIVEELLIPSESBYROTATION,
	PRIMITIVEELLIPSESBYDISPLACEMENT,
	PRIMITIVEBYGROUPROTATION,
	PRIMITIVEBYGROUPDISPLACEMENT,
	MOUSEDELETE,
	MOUSEDELETEOK,       // esta sendo utilizado em mousemove somente para nao redesenhar uma primitiva apagada no passo anterior
	MOVEBYMOUSE,
	MOVEPOINTBYMOUSEFIRSTPOINT,
	MOVELINEBYMOUSEFIRSTPOINT,
	MOVEELLIPSEBYMOUSEFIRSTPOINT,
	MOVEBYDIALOG,
	MOVEPOINTBYDIALOG,
	MOVELINEBYDIALOG,
	ROTATIONBYDIALOG,
	ZOOM,
	ZOOMFIRSTPOINT,
	CREATEMACHZEHNDERMODULATOR,
	CREATEMULTILAYERMODULATOR,
	PREDISCRETIZATION,
	MESH,
	UNGROUP,
	REFLECTION,
	REFLECTIONBY2POINTSMOUSE,
	///////////////// associacao de propriedades fisicas ////////////////////////
	MATERIALATTRIBUTION,
	MATERIALATTRIBUTIONINSERT,
	MATERIALATTRIBUTIONDELETE,
	SOURCEATTRIBUTION,
	SOURCEATTRIBUTIONINSERT,
	SOURCEATTRIBUTIONDELETE,
	BOUNDARYCONDITIONATTRIBUTION,
	BOUNDARYCONDITIONATTRIBUTIONPOINTTOPOINT,
	BOUNDARYCONDITIONATTRIBUTIONEDGE,
	BOUNDARYCONDITIONREMOVEPOINTTOPOINT,
	BOUNDARYCONDITIONAREMOVEEDGE,
	BOUNDARYCONDITIONATTRIBUTIONNOREF,
	///////////////// malha estruturada e escoamento em meios porosos ////////////////////////
	DEFINEFRONTIERS,
	DEFININGFRONTIERS,
	REMOVEFRONTIERS,
	SUBREGIONS,
	SET_AUXILIAR_OR_GEOMETRY
};

enum LENGTHUNIT
{
	METER = 0,
	CENTIMETER,
	MILIMETER,
	MICROMETER,
	NANOMETER,
	ANGSTROM
};

enum MASSUNITY
{
	KILOGRAM = 0,
	GRAM
};

enum TIMEUNITY
{
	HOUR = 0,
	MINUTE,
	SECOND,
	MILISECOND,
	MICROSECOND
};

enum ANGLEUNITY
{
	RAD = 0,
	DEG
};

enum TEMPERATUREUNITY
{
	DEGC = 0,
	DEGK
};

enum PHENOMENONTYPE
{
	ELECTROSTATIC =0,
	ELECTROKINETIC,
	MAGNETOSTATIC,
	THERMALSTATIONARY,
	SCALAROPTICALFIBER,
	SCALAROPTICALWAVE,
	ELETROOPTIC,
	MAGNETOOPTIC,
	THERMOOPTIC,
	ACUSTOOPTIC,
	VECTORIALWAVEPROPAGATION,
	TRANSIENTHEATCONDUCTION,
	TRANSPORTPHENOMENA,
	UNKNOWNPHENOMENON
};

enum MESHTYPE
{
	FRONTEIRA = -2,
	MED_VIZ = -1,
	ARITHMETIC =0,
	GEOMETRIC,
	HARMONIC,
	SQUARED_HARMONIC,
	SELF_ADAPTIVE,
	STRUCTURED
};

enum SYMMETRYTYPE
{
	PLANE = 0,
	AXIAL,
	ASSIMETRIC,
	QUADRANTE
};

enum GEOENTITYTYPE
{
   GEOUNDEFINED = -1,
   GEOM = 0,
   PRE,
   REF,
   POINTLEV,
   POINTINVISIBLELEV,
   POINTAUXILIARLEV,
   LINELEV,
   ARCLEV,
   ELLIPSELEV,
   CIRCUNFERENCELEV,
   POLYGONLEV,
   GROUPLEV,
   FRONTIERLEV
};

enum FILENAME
{
	NOFILENAME = -1,
	MODEL_CONNECTION = 0,		//(.ell)
	MODEL_GEOMETRY,				//(.ege)
	MODEL_COORDINATES,			//(.ept) obs: mod_interrogation
	MODEL_PRE,					//(.epd)
	MODEL_PREDIS,				//(.pre)
	MODEL_BOUNDARYCONDITIONS,   //(.bcp) // indice de primitivas com condi��o de contorno e indice da condi��o de contorno
	MODEL_BOUNDARYCONDITIONSINUSE,//(.bco) // objetos condi��o de contorno em uso
	MODEL_SOURCESINUSE,			//(.src) //// objetos source em uso (local)
	MODEL_MATERIALINUSE,		//(.mts) //// objetos material em uso (local)
	MODEL_REGIONS,				//(.rgn)former(.reg) VER ABAIXO: MODEL_REGIONS_REG
	ARESTASGEOMETRIA,			//(.arg)
	GEOMETRY,					//(.n1?)
	SEGMENTS,					//(.g1?)
	ELEMENTS,					//(.t1?)
	BOUNDARYCONDITIONS,			//(.c1?)
	MATERIAL,					//(.m1?)
	SOURCE,						//(.f1?)
	POTENTIAL,					//(.p1?)
	ARESTASGEO,					//(.ar)
	ELEMENTOSARESTAS,			//(.art)
	PROPFISMEI,					//(propfis.mei)
	MODEL_GEOMETRY_PREDIS,		//(.mgp) //*NMA*: arquivo novo (merge .ell + pre)
	POINT_TYPE,					//(.gpr) //*NMA*: arquivo criado pelo gerador de malha autom�tico, � lido quando da gera��o de malha auto-adaptativa
	SOLVER_SCALAR_OPTFIBER,		//(.fsd) // dados para solver de fibras MatLab
	SOLVER_SCALAR_OPTWAVE,		//(.wsd) // dados para solver de guias de onda MatLab
	MODEL_SUBREGIONS,			//(.srd) // dados de sub-regi�es e fronteiras
	STRUCTUREDMESHINPUTDATA,	//(.gme) // dados de entrada para o gerador de malha
	MODELMESHFRONTEIRA,			//(.mmf) // dados da malha de fronteira para id. de regioes fechadas do modelo geom�trico
	STRUCTUREDMESHESPECIFICATIONS, //(.ime) // dados de especifica��o para o gerador de malha
	STRUCTUREDMESHPOINTS,		//(.plt) // pontos da malha estruturada
	ERRORLOGFILE,				//(.errorlog) // mensagens de erro
	ELEMENTAREASFILE,			//(.areas)	// areas dos elementos finitos
	ELEMENTQUALITYFILE,			//(.qlt)	// qualidade dos elementos finitos (gam - delaunay)
	MODEL_REGIONS_REG,			//(.reg) //obsoleto: antiga extensao arquivo de regioes
	FILEDATASPEC,				//(.dados) //preliminar para armazernar especificacoes de fenomeno
	MODEL_AUXILIARPRIMITIVES	//(.prx) //indices das primitivas auxiliares
};

enum BOUNDARYCONDITIONTYPE
{
	INVALIDBC   = 0,
	NEUMANNBC   = 1,
	DIRICHLETBC = 2,	//*NMA*: para manter padrao antigo
	FLOATBC,		//3
	CICLICBC,		//4
	ANTICICLICBC,	//5
	CONVECTIONBC,   //6
	PECBC			//7
};

enum SOURCETYPE
{
	INVALIDSOURCE,
	CTESOURCE,
	SINUSOIDALSOURCE,
	EXPONENTIALSOURCE,
	EXPONENTIALSHIFTSOURCE,
	PULSESOURCE,
	STEPSOURCE,
	COMPOSEDSOURCE
};

enum FUNCTIONTYPE
{
	UNDEFINEDFUNCTION,
	CTEFUNCTION,
	SINUSOIDALFUNCTION,
	EXPONENTIALFUNCTION,
	EXPONENTIALSHIFTFUNCTION,
	PULSEFUNCTION,
	STEPFUNCTION,
	TRAPEZOIDALFUNCTION,
	COMPOSEDFUNCTION
};

enum FUNCTIONPARAMETERS
{
	NOFUNCTIONPARAMETER=0,
	FREQUENCYFUNCTIONPARAMETER,
	SHIFTFUNCTIONPARAMETER,
	AMPLITUDEFUNCTIONPARAMETER

};

enum DATASTATUS
{
	NODATA,
	INVALIDDATA,
	ATRIBUTTEDDATA,
	SAVEDDATA,
	TRANSFERREDDATA
};


namespace lev_math {  //namespace lev_math


enum MATRIXFORMAT {
	UNDEFINED_FORMAT = 0,
	UNSYMMETRIC,
	SYMMETRIC_SUPERIOR,
	SYMMETRIC_INFERIOR,
	TRIANGULAR_SUPERIOR,
	TRIANGULAR_INFERIOR
};

enum MATRIXORIENTATION {
	UNDEFINED_ORIENTATION = 0,
	COLUMN,
	LINE
};

enum TYPE {TRANSPOSE = 0, NO_TRANSPOSE, CONPLEX_CONJ };


} // namespace lev_math


#endif
