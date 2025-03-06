#include <fstream>
#include <iostream>
#include <ios>
#include <point.hpp>


void Point :: Deinitialize (void)
{
	if(coordinates) {
		delete [] coordinates;
		coordinates = NULL;
	}
}


void Point :: Error(short int err, char *msg)
{
      if (!msg) return;        // msg pointer NULL

      sprintf(msg, "Error %d", err);

      switch (err)
      {
        case 26001:
          strcat(msg, ": unable to alloc memory!");
        break;
        case 26002:
          strcat(msg, ": Inconsistent geometric dimension!");
        break;
        case 26003:
          strcat(msg, ": NULL pointer!");
        break;
        case 26004:
          strcat(msg, ": NULL pointer!");
        break;
        default:
          strcat(msg, ": Unknown...");
      }
}  // =========================================================end of Error


void Point :: Initialize(const short int dim)
    { dimension = dim;  origin = -1; coordinates = NULL;
      coordinates = new double[dimension];
      if (!coordinates)
      { char msg[512] = ""; Error(26001, &msg[0]); std::cout << msg; exit(26001);}
    } // ======================================================end of Initialize


void Point :: Save(char* nome_arquivo,int op)//*NMA*:23/01/2007: op=0-> "w" (write) op=1->"a" (append) ex: Save("arquivo.dat",1)
{
	std::ofstream	fo;

	if(!op)// abre para grava��o
		fo.open(nome_arquivo,std::ios::out);
	else //abre para append
		fo.open(nome_arquivo,std::ios::app);

	if(fo.fail())
	{
		std::cout << "Point :: Save(): Can�t open file "<< nome_arquivo;
	}
	else
	{
		fo << "-- POINT -----------------------------------\n";
		fo << "dimension: "		<< dimension << std::endl;
		fo << "origin: "		<< origin << std::endl;
		fo << "coordinates:"	<< std::endl;
		fo << "x = " << X();
		if(dimension != 1) fo << "  y = " << Y();
		fo << "\n";
		fo.close();
	}

}//Point :: Save(char* nome_arquivo,int op=0)
