/* Generated by Together */

#ifndef __PCF_LAYER_H
#define __PCF_LAYER_H
#include <meshdata.h>
#include "levellipse.h"
#include "pcf_layerdata.h"

class CPCF_Layer
{
protected:
    CLevEllipse oEllipsePattern;
    CLevEllipse oEllipsePatternHalfHorizontal;
    CLevEllipse oEllipsePatternHalfVertical;
    meshData holePatternBoundaryMesh;
    meshData holeHalfHorizontalBoundaryMesh;
    meshData holeHalfVerticalBoundaryMesh;
public:
    /**
     * @associates <{++}>
     */
    std::vector<CLevEllipse*> vpEllipses;

    /**
     * @associates <{++}>
     */
    std::vector<CLevLine*> vpLines;
    CPCF_LayerData layer_data;


};

#endif //CPCF_LAYER_H
