#ifndef __LEVTYPEDEF_H__
#define __LEVTYPEDEF_H__


//===================================================================================================
//	Arquivo:			LevTypeDef.h
//...................................................................................................
//	Descrição:			Contém os typedef's de vetores utilizados pelo sistema
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						Angelo Passaro
//...................................................................................................
//	Observações:	Para evitar ambiguidades é melhor sempre acessar os tipos de STL por std::
//...................................................................................................
//	Data:				Setembro/2001
//	Modificado em:		abril/2002
//	Modificado por:		Nancy
//	Motivo:				Uso explícito de std:: e definição de todos os vetores de pritivas
//===================================================================================================


#include <vector>


class CEntityData;
class CLevPoint;
class CLevPrimitive;
class CLevLine;
class CLevEllipse;
class CLevGroup;
class CSubRegion;
class CArrowDevice;
class CModulatorDevice;
class CPcfDevice;
class CLevGeoData;
class CLevGeoPhysData;
class CBoundCond;
class CSource;
class CMaterial;
class CLevRegion;
class CLevFrontier;
class CObserverBase;


typedef std::vector<long          > vLong;
typedef std::vector<int           > vInt;
typedef std::vector<double        > vDouble;

typedef std::vector<CEntityData*>	vpCEntityData;
typedef std::vector<CLevPoint    *> vpCLevPoint;
typedef std::vector<CLevPrimitive*>	vpCLevPrimitive;
typedef std::vector<CLevLine     *>	vpCLevLine;
typedef std::vector<CLevEllipse  *>	vpCLevEllipse;
typedef std::vector<CLevGroup    *>	vpCLevGroup;
typedef std::vector<CLevFrontier *>	vpCLevFrontier;
typedef std::vector<CLevRegion   *>	vpCLevRegion;
typedef std::vector<CObserverBase *>	vpCObserverBase;


typedef std::vector<CLevGeoPhysData*>	vpCLevGeoPhysData;

typedef std::vector<CEntityData	    > vCEntityData;
typedef std::vector<CLevPoint		> vCLevPoint;
typedef std::vector<CLevPrimitive   > vCLevPrimitive;
typedef std::vector<CLevLine		> vCLevLine;
typedef std::vector<CLevEllipse		> vCLevEllipse;
typedef std::vector<CLevGroup		> vCLevGroup;
typedef std::vector<CLevFrontier	> vCLevFrontier;
typedef std::vector<CLevRegion      > vCLevRegion;

typedef std::vector<CSubRegion		> vCSubRegion;

typedef std::vector<CArrowDevice	> vCArrowDevice;
typedef std::vector<CModulatorDevice> vCModulatorDevice;
typedef std::vector<CPcfDevice		> vCPcfDevice;

typedef std::vector<CLevGeoData   >		vCLevGeoData;
typedef std::vector<CLevGeoPhysData>	vCLevGeoPhysData;

typedef std::vector<CBoundCond    >	vCLevBoundCond;
typedef std::vector<CSource		  >	vCLevSource;
typedef std::vector<CMaterial     >	vCLevMaterial;

typedef std::vector<CObserverBase >	vCObserverBase;

#endif
