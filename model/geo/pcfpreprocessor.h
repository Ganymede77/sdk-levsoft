#ifndef __PCFPREPROCESSOR_H
#define __PCFPREPROCESSOR_H

#include <pcf_layerdata.h>
#include <point.hpp>
#include "pcf_layer.h"
#include "dgr15_10_2d.hpp"
#include "levgeodata.h"
#include "levphysdata.h"

/**
 * @stereotype visitor&&observer
 */
class CPCFPreProcessor
{
protected:
    Delaunay2D				malha;
    Point                   central_hole;
	bool					is_complete_geometry;
	bool					has_boundary_lines;
	bool					has_pml_lines;

    Point					pitch;
	int						material; //indice da lista completa de materiais
	vpCLevPoint				vpPointsX;
	vpCLevPoint				vpPointsY;
//	double					xcmax, yc_of_xcmax;
	double					xcmax , ycmax_of_xcmax , ycmax;
	double					xmax, ymax_of_xmax,  ymax;
	CLevPoint				pcentral;
	CLevPoint				*ppcentral, *ppx0ymax, *ppxmaxy0, *ppxmaxyofxmax;
	int						ind_color;

	CLevRegion				background_region, background_region_ext;
	CLevRegion				region_right,region_left,region_top, region_down,
							region_topright, region_downright, region_topleft, region_downleft;

	// dados da PML ou regiões de transformação de coordenadas
	double					d1, d2, d3, d4;
	int						mat_r1, mat_r2, mat_r3, mat_r4;

    std::vector<CPCF_Layer> vpcfLayers;

	CLevGeoData				*pModel;
	CLevPhysData			*pPhysData;

	void					IncludeUpdatePitchCompleteGeometry(void);
	void					IncludeUpdatePitchQuarterGeometry(void);

	void					IncludeBoundaryLines(void);
	void					IncludePMLLines(void);

public:

 //   void		SetData(CSimpleXY pitch, CSimpleXY chole, std::vector<CPCF_LayerData> & vld  );

	CPCFPreProcessor();
	~CPCFPreProcessor();

	void		Associate(CLevGeoData * pm);
	void		Associate(CLevPhysData	*pphys);

    void		IdentifyRefinedElementsToRegions();

	void		GetLayersData(std::vector<CPCF_LayerData> &vl );
	void		SetLayersData(std::vector<CPCF_LayerData> vl );

	void		SetPitch(Point p);
	Point		GetPitch(void);

	void		SetCentralHoleDiameters(Point p);
	Point		GetCentralHoleDiameters(void);

	void		SetBackgroundMaterial(int); //indice da lista completa de materiais
	int			GetBackgroundMaterial(void);

	void		IsCompleteGeometry(bool b);
	bool		IsCompleteGeometry(void);

	void		HasBoundaryLines(bool b);
	bool		HasBoundaryLines(void);

	void		IncludeUpdateData(void);

	void		SetPMLDim(double dd1, double dd2, double dd3, double dd4);
	void		SetPMLMaterial(int mr1, int mr2, int mr3, int mr4);

	void		GetPMLDim(double &dd1, double &dd2, double &dd3, double &dd4);
	void		GetPMLMaterial(int &mr1, int &mr2, int &mr3, int &mr4);

	void		HasPMLLines(bool b);
	bool		HasPMLLines(void);

    /**
     * @associates <{++}>
     */
    std::vector<CLevLine*>			vpBoundaryLines;
	std::vector<CLevEllipse*>		vpHoles;

};
#endif //CPCFPREPROCESSOR_H
