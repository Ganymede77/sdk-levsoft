#ifndef __CPCF_LAYERDATA_H
#define __CPCF_LAYERDATA_H


#include <point.hpp>


class CPCF_LayerData 
{
protected:
    long material;
    int number_of_sublayers;
	int	number_of_predis_points;

public:
	Point diameters;
    
public:

    void	Material(long );
    long	Material();

    void	SubLayersNumber(int );
    int		SubLayersNumber();

	void	PredisPoints(int );
	int		PredisPoints(void);
};
#endif //CPCF_LAYERDATA_H
