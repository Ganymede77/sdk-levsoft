#ifndef	__LEV_COLORSRGB_H__
#define __LEV_COLORSRGB_H__

#include "levcolorsRGB.h"

// Defines the oolors used to show points, lines, polygons, highligths, etc
//const COLORREF COLOROFDIRICHLETBC		= RGB(50,50,250);
const CLEV_ColorRGB COLOROFDIRICHLETBC(50,50,250);
const CLEV_ColorRGB COLOROFNEUMANNBC(50,150,0);
const CLEV_ColorRGB COLOROFCONVECTIONBC		(50,250,250);
const CLEV_ColorRGB COLOROFFLOATREFBC		(10,0,0);
const CLEV_ColorRGB COLOROFFLOATBC(200,50,10);
const CLEV_ColorRGB COLOROFPECBC	(90,150,150);
const CLEV_ColorRGB COLOROFCICLICBC(150,150,200);
const CLEV_ColorRGB COLOROFREFCICLICBC		(0,0,120);
const CLEV_ColorRGB COLOROFANTICICLICBC		(200,180,250);
const CLEV_ColorRGB COLOROFREFANTICICLICBC	(130,50,50);

const CLEV_ColorRGB COLOROFPOINT				(255,255,255);//RGB(200,0,80);
const CLEV_ColorRGB COLOROFPOINTPREDIS		(255,0,0);//RGB(200,0,80);
const CLEV_ColorRGB COLOROFPOINTHIGHLIGHT	(0,0,255);
const CLEV_ColorRGB COLOROFPOINTSINGLE		(100,200,200);
const CLEV_ColorRGB COLOROFLINE				(255,0,0);
const CLEV_ColorRGB COLOROFELLIPSE			(255,0,0);
const CLEV_ColorRGB COLOROFLINEINCLUSION		(0,0,255);
const CLEV_ColorRGB COLOROFELLIPSEINCLUSION  (0,0,255);
const CLEV_ColorRGB COLOROFHIGHLIGHT			(255,255,0);
const CLEV_ColorRGB COLOROFSELECTED			(0,255,0);
const CLEV_ColorRGB COLOROFGROUP 			(0,255,255);
const CLEV_ColorRGB COLOROFFRONTIER 			(0,100,255);
//const COLORREF COLOROFPOLYGON			= RGB(0,255,255);
const CLEV_ColorRGB COLOROFMESH				(0,0,255);
//const COLORREF COLOROFFLOATBC			= RGB(200,0,200);
//const COLORREF COLOROFFLOATREFBC		= RGB(200,250,2);
const CLEV_ColorRGB COLOROFREGION 			(205,200,0);
const CLEV_ColorRGB COLOROFSUBREGION			(240,255,0);
const CLEV_ColorRGB COLOROFSOUTHFRONTIER		(10,50,150);
const CLEV_ColorRGB COLORGRAY		(128,128,128);


// ................ para Materiais .......................................

const CLEV_ColorRGB BLACK		(0,0,0);
const CLEV_ColorRGB BLUE		(0,0,255);
const CLEV_ColorRGB GREEN		(0,255,0);
const CLEV_ColorRGB CYAN		(0,255,255);
const CLEV_ColorRGB RED			(255,0,0);
const CLEV_ColorRGB MAGENTA		(255,0,144);
const CLEV_ColorRGB YELLOW		(255,255,0);
const CLEV_ColorRGB WHITE		(255,255,255);


#define MAXCOLOR_LEVRGB 25 //*NMA* : quantidade de cores definida no vetor colors

static CLEV_ColorRGB colors_LEVRGB[] =
{
	CLEV_ColorRGB( 255,255,204), //0 amarelo palido
	CLEV_ColorRGB( 204,255,255), //1 azul clarinho 			\\vacuo
	CLEV_ColorRGB( 153,255,255), //2 azul esverdeado 		\\ar
	CLEV_ColorRGB( 153,153,153), //3 marrom claro/cinza		\\p_teste
	CLEV_ColorRGB( 255,102,255), //4 maravilha				\\agua

	CLEV_ColorRGB( 153,  0,102), //5 tom de vermelho/carmim	\\papel
	CLEV_ColorRGB( 102,255,  0), //6 verde claro			\\ mica
	CLEV_ColorRGB( 102, 51,  0), //7 marron					\\ambar
	CLEV_ColorRGB(  51,  0,102), //8 roxo quase preto		\\procelana
	CLEV_ColorRGB( 204,102,  0), //9 laranja				\\quatzo fundido

	CLEV_ColorRGB( 204,102,150), //10 laranja/vermelho		\\vidro pirex
	CLEV_ColorRGB( 255,255,102), //11 amarelo				\\baquelite
	CLEV_ColorRGB( 204,255,  0), //12 verde/amarelo			\\polietileno
	CLEV_ColorRGB(  51,  0,254), //13 azul/violeta			\\polistireno
	CLEV_ColorRGB( 255, 102,102),//14 rosa					\\teflon

	CLEV_ColorRGB( 255,  51,  0),//15 laranja forte			\\neoprene
	CLEV_ColorRGB( 255, 153,204),//16 rosinha/roxinho		\\oleo piranol
	CLEV_ColorRGB(  51, 205,255),//17 azul  				\\cobre
	CLEV_ColorRGB(  51, 204,153),//18 amarelinho 			\\prata
	CLEV_ColorRGB(  51, 153,102),//19 verdinho 				\\ouro

	CLEV_ColorRGB( 102,  51,102),//20 roxo fosco			\\cobre_cond0
	CLEV_ColorRGB( 153, 102,102),//21 roxo/marrom			\\aluminio
	CLEV_ColorRGB( 255, 204,102),//22 amarelinho			\\tungstenio
	CLEV_ColorRGB(   0, 153,153),//23 azul/verde			\\zinco
	CLEV_ColorRGB( 204, 153,  0),//24 marron claro/laranja	\\platina
	CLEV_ColorRGB( 153,  51,  0),//25 marron				\\estanho


	//26//chumbo
	//27//mercurio
	//28//grafite
	//29//manganina
	//30//constantana
	//31//ferro
	//32//ferrosil
	//33//ferro_linear_1000
	//34//Fe_linear_mil_cond0
	//35//aco_ALNICO
	//36//Ferrite_de_Bario
	//37//ferrite

#if 0
	CLEV_ColorRGB( 153,   0,255),//17 roxinho
	CLEV_ColorRGB( 102, 51,102),
	BLUE, GREEN,  CYAN, RED
#endif
};

#if 0
static CLEV_ColorRGB colors_LEVRGB[] =
{
	CLEV_ColorRGB(   0,128,128), CLEV_ColorRGB( 128,  0,  0), CLEV_ColorRGB( 128,  0,128), CLEV_ColorRGB( 128,128,  0),

	CLEV_ColorRGB(   0,  0,255), CLEV_ColorRGB(   0,255,  0), CLEV_ColorRGB(   0,255,255), CLEV_ColorRGB( 255,  0,  0),
	CLEV_ColorRGB( 255,  0,255), CLEV_ColorRGB( 255,255,  0), CLEV_ColorRGB(  220, 80, 15),CLEV_ColorRGB (  51,153,155),
	BLUE, GREEN,  CYAN, RED
};
#endif
#if 0
static CLEV_ColorRGB colors_LEVRGB[] = {
	( 192,192,192), ( 192,220,192), (   0,  0,128), (   0,128,  0),
	(   0,128,128), ( 128,  0,  0), ( 128,  0,128), ( 128,128,  0),

	(   0,  0,255), (   0,255,  0), (   0,255,255), ( 255,  0,  0),
	( 255,  0,255), ( 255,255,  0), (  220, 80, 15), (  51,153,155)
};
#endif

#endif
