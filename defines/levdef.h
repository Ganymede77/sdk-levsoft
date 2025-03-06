#ifndef __LEVDEF_H__
#define __LEVDEF_H__

#include <string>

//===================================================================================================
//	Arquivo:			LevDef.h
//...................................................................................................
//	Descrição:			Contém os defines utilizados pelo sistema
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						Angelo Passaro
//...................................................................................................
//	Observações:
//...................................................................................................
//	Data:				Junho/2000
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//===================================================================================================

//#define  USINT unsigned short  // as duas proximas macros sao definidas tambem em Lo_Templ.hpp
//#define  Uchar unsigned char

const std::string SDKVERSION = "0.1.0";
const long	UNDEFINED = -1000;

const double PI = 3.1415926535;
const double ctePI = 3.1415926535;

// gerador de malha de elementos finitos
//const long int MAXPTS =  200000;
//const long int MAXPTSTRN	=   200000;
//const long int MAXGEO =  200000;
//const long int MAXTRN =  400000;
//const long int MAXCIR =    4000;
const long int MAXPTS		=   20000;//20000
const long int MAXPTSTRN	=   100;//210000;//80000;
const long int MAXGEO		=   20000;//20000
const long int MAXTRN		=   100000;//5400000;//160000;
const long int MAXTRNFRONTEIRA =   20000;//300000;//160000;
const long int MAXCIR		=    4000;

//const long int MAXPTS		=   10000;
//const long int MAXPTSTRN	=   80000;
//const long int MAXGEO		=   10000;
//const long int MAXTRN		=  160000;
//const long int MAXCIR		=    2000;


const int   NUM_MAX_VIZINHOS = 3;    // as proximas tres constantes sao iguais
const unsigned char num_vertices     = 3;    // somente para os simplex 2D
const unsigned char num_faces        = 3;


#define SEMerro   0
#define FIMinesp  1
#define FALHAleit 2
#define FALHAescr 3
#define DADOinv   4
#define MEMinsuf  5
#define EXCpts    6
#define EXCseg    7
#define EXCtrn    8
#define EXCcir    9
#define EXCver    10

#define ARQvelhoG 1
#define ARQnovoG  2

#define MAXUNDOLevel 10

#endif
