/* Generated by Together */

#include <pcf_layerdata.h>

int CPCF_LayerData::SubLayersNumber()
{
	return number_of_sublayers;
}

void CPCF_LayerData::SubLayersNumber(int num )
{
	number_of_sublayers = num;
}

long CPCF_LayerData::Material()
{
	return material;
}

void CPCF_LayerData::Material(long mat)
{
	material = mat;
}

int CPCF_LayerData::PredisPoints()
{
	return number_of_predis_points;
}

void CPCF_LayerData::PredisPoints(int num )
{
	number_of_predis_points = num;
}
