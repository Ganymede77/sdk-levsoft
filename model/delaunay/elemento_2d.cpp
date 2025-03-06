#include "lo_el_2d.hpp"
#include "math.h"
#include "elgaussp.h"


//------------------- CLASS ELEMENTO_2D ----------------------------------

Elemento_2D :: Elemento_2D()   // construtor
{
	N_vertices=3;
	Num_Vizinhos = 0;
	volume = 0;
	Estado = LEV_INVALID;
	Coef[0] = Coef[1] = Coef[2] = 0;
	Vertices[0] = Vertices[1] = Vertices[2] = 0;
	Vizinhos[0] = Vizinhos[1] = Vizinhos[2] = -1;
	material = 0;
	Fonte = 0;
	Angulo = 0;
	Campo = 0;
	DerivadaPrim = 0;
	Erro = 1e308;
	Raio_Circ = 0.0;
}


Elemento_2D :: ~Elemento_2D()
{ }


//-----------------------------------------------------------------------
//               class Elemento_2D :: Get_V1, V2, V3
//-----------------------------------------------------------------------
//
//  Descricao          : informa os vertices do triangulo
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 17/11/1999
//
//-----------------------------------------------------------------------
long int Elemento_2D :: Get_V1(void) {return Vertices[0];}
long int Elemento_2D :: Get_V2(void) {return Vertices[1];}
long int Elemento_2D :: Get_V3(void) {return Vertices[2];}


//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Set_N_Vizinhos
//-----------------------------------------------------------------------
//
//  Descricao          : atribui número de vizinhos do elemento
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 07/02/2000
//
//-----------------------------------------------------------------------
lev_math::Uchar      Elemento_2D :: Set_N_Vizinhos(lev_math::Uchar n) {return (Num_Vizinhos = n);}


//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Set_vertices
//-----------------------------------------------------------------------
//
//  Descricao          : atribui os indices dos vertices do elemento
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 17/11/1999
//
//-----------------------------------------------------------------------
void Elemento_2D :: Set_Vertices (long int *vert)
{
   lev_math::USINT i;
   for(i=0; i<num_vertices; i++)
	   Vertices[i] = vert [i];
}



//-----------------------------------------------------------------------
//               class Elemento_2D :: Get_Volume
//-----------------------------------------------------------------------
//
//  Descricao          : informa a volume do elemento
//						 se o valor for negativo, inverte o sinal
//						 e troca a ordem dos vértices 1 e 2.
//
//  versao             : 1.1
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 06/09/2000
//
//-----------------------------------------------------------------------

double Elemento_2D :: Get_Volume(void)
{
//long int aux;
//
//	if (volume < 0 )
//	{
//		volume = -volume;
//		aux = Vertices[0];
//		Vertices[0] = Vertices[1];
//		Vertices[1] = aux;
//	}
	return volume;
}

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Set_Volume
//-----------------------------------------------------------------------
//
//  Descricao          : atribui a volume de um elemento
//
//  Parametro          : a volume do elemento
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 17/11/1999
//
//-----------------------------------------------------------------------

void Elemento_2D :: Set_Volume(double volume_i) {volume = volume_i ;}

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Get_Vizinhos
//-----------------------------------------------------------------------
//
//  Descricao          : Fornece a lista de indices de elementos vizinhos
//
//  Retorno            : um ponteiro para a lista
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 17/11/1999
//
//-----------------------------------------------------------------------

long int * Elemento_2D :: Get_Vizinhos(void)  {return &Vizinhos[0];}

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Get_N_Vizinhos
//-----------------------------------------------------------------------
//
//  Descricao          : Fornece a lista de indices de elementos vizinhos
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 17/11/1999
//
//-----------------------------------------------------------------------

lev_math::USINT Elemento_2D :: Get_N_Vizinhos(void) {return Num_Vizinhos;}

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Get_Validade
//-----------------------------------------------------------------------
//
//  Descricao          : Informa se o elemento eh valido
//
//  Retorno            : VALIDO ou INVALIDO
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 17/11/1999
//
//-----------------------------------------------------------------------

lev_math::Uchar Elemento_2D :: Get_Validade(void) {return Estado;}

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Set_Validade
//-----------------------------------------------------------------------
//
//  Descricao          : atribui a validade do elemento
//
//  parametro          : VALIDO ou INVALIDO
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 17/11/1999
//
//-----------------------------------------------------------------------

void Elemento_2D :: Set_Validade(lev_math::Uchar  val) {Estado = val;}

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: OPERATOR =
//-----------------------------------------------------------------------
//
//  Descricao          : copia um objeto Elemento_2D
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 17/11/1999
//
//-----------------------------------------------------------------------

Elemento_2D Elemento_2D :: operator= (const Elemento_2D & el)
{
   volume = el.volume;
   Estado = el.Estado;
   Num_Vizinhos = el.Num_Vizinhos;
   Coef[0] = el.Coef[0];
   Coef[1] = el.Coef[1];
   Coef[2] = el.Coef[2];
   Vertices[0] = el.Vertices[0];
   Vertices[1] = el.Vertices[1];
   Vertices[2] = el.Vertices[2];
   Vizinhos[0] = el.Vizinhos[0];
   Vizinhos[1] = el.Vizinhos[1];
   Vizinhos[2] = el.Vizinhos[2];
   material = el.material;
   Fonte = el.Fonte;
   Angulo = el.Angulo;
   DerivadaPrim = el.DerivadaPrim;
   Campo = el.Campo;
   Erro = el.Erro;
   Raio_Circ = el.Raio_Circ;
   return *this;
}  //  fim do metodo operator

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Get_C0, Get_C1, Get_C2
//-----------------------------------------------------------------------
//
//  Descricao          : informam os coeficientes do circulo circunscrito
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 17/11/1999
//
//-----------------------------------------------------------------------

double Elemento_2D :: Get_C0(void) {return Coef[0];}
double Elemento_2D :: Get_C1(void) {return Coef[1];}
double Elemento_2D :: Get_C2(void) {return Coef[2];}

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Set_C0, Set_C1, Set_C2
//-----------------------------------------------------------------------
//
//  Descricao          : definem, a partir de dados externos, os coeficientes
//                       do circulo circunscrito ao elemento. Utilizados
//                       particulamente em compactação.
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 17/11/1999
//
//-----------------------------------------------------------------------

void Elemento_2D :: Set_C0(double c0) {Coef[0] = c0;}
void Elemento_2D :: Set_C1(double c1) {Coef[1] = c1;}
void Elemento_2D :: Set_C2(double c2) {Coef[2] = c2;}

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Calc_Circuns
//-----------------------------------------------------------------------
//
//  Descricao          : Calcula os parametros do Circuncírculo do elemento 2D
//                       Resolve um sistema de equações nlinhas x nlinhas pelo
//                       Método de Eliminação de Gauss
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 13/10/2000
//
//-----------------------------------------------------------------------

//void Elemento_2D :: Calc_Circuns(Lo_Pt_Malha *pt) {
void Elemento_2D :: Calc_Circuns(vCLevPtMalha &pt) {
   Pt_Malha *pt_at;
   pt_at = new Pt_Malha[3];
   lev_math::USINT ret1=0, ret2=0, ret3=0;

   // coeficientes do circuncículo e elementos da matriz b = Ax

   double Maux[3][4];
   double ** M = NULL;
   M = new double * [3];
	M[0] = &Maux[0][0];
	M[1] = &Maux[1][0];
	M[2] = &Maux[2][0];

   //------------------- calculo de variáveis auxiliares -----------------------

   pt_at[0] = pt[Vertices[0]];
   pt_at[1] = pt[Vertices[1]];
   pt_at[2] = pt[Vertices[2]];

   if (!ret1 && !ret2 && !ret3) {
      M[0][0] = pt_at[0].X();
      M[0][1] = pt_at[0].Y();
      M[0][2] = 1;
      M[0][3] = - ( M[0][0] * M[0][0] + M[0][1] * M[0][1]);

      M[1][0] = pt_at[1].X();
      M[1][1] = pt_at[1].Y();
      M[1][2] = 1;
      M[1][3] = - ( M[1][0] * M[1][0] + M[1][1] * M[1][1]);

      M[2][0] = pt_at[2].X();
      M[2][1] = pt_at[2].Y();
      M[2][2] = 1;
      M[2][3] = - ( M[2][0] * M[2][0] + M[2][1] * M[2][1]);

      delete [] pt_at; pt_at = NULL;

	  volume = ( M[0][0] * M[1][1] + M[2][0] * M[0][1] + M[1][0] * M[2][1] -
		         M[2][0] * M[1][1] - M[0][0] * M[2][1] - M[1][0] * M[0][1] );


     if (volume){  // verifica se a área é diferente de zero

		Eliminacao_Gauss(M, Coef, 3);
		if (volume <0)
		{
			long aux;
			aux=Vertices[0];
			Vertices[0]=Vertices[1];
			Vertices[1]=aux;
			volume = -volume;
		}

	 }  // fim da verificação do volume
	 else
	 {// neste caso a área do triângulo é zero. O calculo dos
		  // coeficientes é feito de forma diferente.
		  // as coordenadas do centro do circuncírculo são obtidas calculando a
		  // média das coordenadas dos três pontos...

			 Coef[0] = - (M[0][0] + M[1][0] + M[2][0])*.5;
             Coef[1] = - (M[0][1] + M[1][1] + M[2][1])*.5;
			 Coef[2] = - (M[0][0] * M [0][0] + M[0][1] * M [0][1] +
				          M[0][0] * Coef[0] + M[0][1] * Coef[1]);
	 }
 }
 else
 {
	lev_math::USINT status = 301;
#	if defined INTERFACE_GRAFICA
	 Msg1.Display("Erro 301: Dados corrompidos. Execucao interrompida...",BRANCO,AZUL);
#	else
	std::cout << "\n Erro 301: Dados corrompidos em Calc_Circuns. Execucao interrompida..." ;
#	endif
	 exit (status);
 }

   if (M) {
	 delete [] M; M = NULL;
   }

	Raio_Circ = (Coef[0]*Coef[0] + Coef[1]*Coef[1] ) * .25 - Coef[2];

	if (Raio_Circ > 0.0)
		Raio_Circ = sqrt(Raio_Circ);
	else
	{
		printf ("Raio circunscrito do elemento <= 0");
	}

 return;
}


//---------------------------------------------------------------------------
//               class Elemento_2D :: Elimina_Vizinhanca
//---------------------------------------------------------------------------
//
//  Descricao          : Elimina o objeto elemento ind da lista de objetos
//                       vizinhos ao objeto em foco
//
//  parametro          : o indice do elemento da lista de elementos
//
//  retorno            : o codigo de status de eliminacao
//                          0 : eliminacao normal;
//                        302 : nenhum dos vizinhos corresponde ao indice
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 13/12/1999
//
//---------------------------------------------------------------------------
lev_math::USINT Elemento_2D :: Elimina_Vizinhanca(long int  ind)
{
	lev_math::USINT i =0, status = 302;
	for (; i < Num_Vizinhos; i++)
	 if (Vizinhos[i] == ind)
	 {
		Vizinhos[i] = Vizinhos[--Num_Vizinhos];
		status = 0;
		break /* i */ ;
	 }
	return status;
} // fim do metodo Elimina_Vizinhanca


//---------------------------------------------------------------------------
//               class Elemento_2D :: Set_Vizinho
//---------------------------------------------------------------------------
//
//  Descricao          : associa um vizinho a um elemento. Recebe uma mensagem
//                       que indica ao objeto qual o indice de um outro objeto
//                       da mesma classe, vizinho, e o inclue na lista de
//                       vizinhos. Na mensagem tambem vem uma indicacao se o
//                       objeto deve se incluido com ou sem verificacao
//
//  parametro          : indice do elemento vizinho
//  retorno            : status da inclusao de vizinho
//                           0 : inclusao normal;
//                         306 : numero de vizinhos supera o valor maximo permissivel.
//                               Isto significa que a malha e inconsistente.
//
//  versao             : 1.1
//  programador        : Angelo Passaro
//  ultima modificacao : 21/12/96
//
//---------------------------------------------------------------------------

lev_math::USINT  Elemento_2D :: Set_Vizinho(long int  indice)
{
	lev_math::USINT status = 0;
	if (Num_Vizinhos < NUM_MAX_VIZINHOS) // definido NUM_MAX_VIZINHOS = 3
		Vizinhos[Num_Vizinhos++] = indice;
	else {
		status = 306;
	}
	return (status);
} // fim de Set_Vizinho


//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Get_Vertices
//-----------------------------------------------------------------------
//
//  Descricao          : Retorna um vertice do elemento de acordo com o indice
//
//  Retorno            : retorna o vertice indice n
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 07/02/2000
//
//-----------------------------------------------------------------------

long int Elemento_2D :: Get_Vertices(int indice) {return Vertices[indice];}



//---------------------------------------------------------------------------
//               class Elemento_2D :: Calc_R_Insc
//---------------------------------------------------------------------------
//
//  Descricao          : cálcula o raio da circunferência inscrita no elemento
//
//  parametro          : o vetor de pontos na forma de um objeto de gerenciamento
//						 de lista ordenada de pontos
//
//  retorno            : o raio do círculo inscito (zero se o elemento tem área zero)
//                        302 : nenhum dos vizinhos corresponde ao indice
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 16/12/1999
//
//---------------------------------------------------------------------------

//double Elemento_2D :: Calc_R_Insc(Lo_Pt_Malha *bloco_pts)
double Elemento_2D :: Calc_R_Insc(vCLevPtMalha &bloco_pts)
{        //RELACIONAR DIRETAMENTE COM O OBJETO
   int        i, status=0;
   double     rinsc;
   Ponto      P0, P1, P2;
   Pt_Malha   P;
   double     Quad;         //armazena o dividendo da equação descrita a seguir ao quadrado
							// A realização dos calculos de Valor Absoluto e Raiz Quadrada
							// levam mais tempo que elevar o dividendo ao quadrado e
							// extrair a raiz da distância.

   double     CoefInsc[3];  //A equação para cálculo da distância de um ponto a reta:
							// d(P,r)= |ax+by+d|/ sqrt(a^2+b^2)
							// onde:
							//     d - distância do ponto a reta
							//     P - ponto desejado para o cálculo
							//     r - a reta desejada para o cálculo
							//     a, b, d - coeficientes obtidos por meio das coordenadas
							//     dos pontos da reta
							//     x, y - coordenadas do ponto desejado
							// A equação reescrita para o algoritmo
							// D[i].H = |coef[0]*P0x+coef[1]P0y-coef[2]|/
							//			/sqrt(coef[0]^2+coef[1]^2)
							//
							// O cálculo dos coeficientes é dado a seguir:
							// coef[0]=(P2y-P1y)/(P2x-P1x)
   							// coef[1]= 0 para reta vertical ou 1 caso geral.
   							// coef[2]= P1y-(P1y-P2y)/(P1y-P2y)*Px1 ou
							// coef[2]=P1y-coef[0]*Px1
   struct {long int iponto; long aresta[2]; double H;}  D[3];


   D[0].iponto     = Vertices[0];
   D[0].aresta[0]  = Vertices[1];
   D[0].aresta[1]  = Vertices[2];

   D[1].iponto     = Vertices[1];
   D[1].aresta[0]  = Vertices[2];
   D[1].aresta[1]  = Vertices[0];

   D[2].iponto     = Vertices[2];
   D[2].aresta[0]  = Vertices[0];
   D[2].aresta[1]  = Vertices[1];

   for (i=0; i < 3; i++)
   {   // calcula os coeficientes para o calculo da distância
	   P = bloco_pts[D[i].aresta[0]];
	   if (status)
		   exit(11000);
	   else {
		   P1=P;
		   P = bloco_pts[D[i].aresta[1]];
	       if (status)
	    	   exit(11000);
	       else {
			   P2=P;
			   P = bloco_pts[D[i].iponto];
			   if (status) exit(11000); //interrompe
			   else	  P0=P;

			   if (P1.X()==P2.X())
			   {				  // A de uma reta vertical ay+c=0, portanto b=0 (coef[1]=0)
				  D[i].H = P1.X() - P0.X();
				  D[i].H = (D[i].H >0)? D[i].H: -D[i].H;
			   } else { // caso a reta não seja vertical
			      CoefInsc[0]=(P2.Y() - P1.Y()) / (P2.X() - P1.X());
			      CoefInsc[1]=( 1 );
			      CoefInsc[2]=P1.Y() - CoefInsc[0] * P1.X();
				  Quad = - CoefInsc[0]*P0.X() + CoefInsc[1]*P0.Y() - CoefInsc[2];
				  Quad = Quad*Quad;
				  D[i].H=Quad/(CoefInsc[0]*CoefInsc[0]+CoefInsc[1]*CoefInsc[1]);
				  D[i].H=sqrt(D[i].H);
			   }
		   }
	   }
	   // calcula a distância do ponto à reta
	  if (!D[i].H)
	  {
		  std::cout <<"\n eliminação de achatado";
		  std::cout << "\n distância ponto reta = zero";
	  }
   }// fim do laço for(i)
   // calcula raio da circunferência inscrita
   if (D[0].H==0 || D[1].H==0 || D[2].H==0) rinsc = 0;
   else
   {
	   rinsc = 1./D[0].H+1./ D[1].H+ 1./D[2].H;
	   rinsc = 1./rinsc;
   }
   return (rinsc);
}


//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Get_Vertices
//-----------------------------------------------------------------------
//
//  Descricao          : Fornece os indices dos vertiices de um
//						 elemento
//
//  Retorno            : um ponteiro para os vertices
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 07/02/2000
//
//-----------------------------------------------------------------------

long int *  Elemento_2D ::  Get_Vertices (long int *vert)
{
	memcpy(vert, Vertices, sizeof(long int) *N_vertices);
	return (vert);
}

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Get_N_vertices
//-----------------------------------------------------------------------
//
//  Descricao          : Fornece o númerttto de vertices
//
//  Retorno            : N_vertices
//
//  versao             : 1.0
//  programador        : Angelo Passaro
//  ultima modificacao : 07/02/2000
//
//-----------------------------------------------------------------------

int  Elemento_2D ::  Get_N_Vertices() {return N_vertices;};

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Monta_Faces
//-----------------------------------------------------------------------
//
//  Descricao          : Monta a lista de faces de um elemento
//						 e devolve a lista à função chamadora
//						 por meio do ponteiro "faces"
//
//
//  versao             : 1.0
//  programador        : Guilherme Moreira Magalhães
//  ultima modificacao : 31/05/2000
//
//-----------------------------------------------------------------------
void Elemento_2D::Monta_Faces(FACE *faces)
{
		faces[0].V1 = Vertices[0];      faces[1].V1 = Vertices[1];
		faces[0].V2 = Vertices[1];      faces[1].V2 = Vertices[2];
		faces[0].Valido = LEV_VALID;    faces[1].Valido = LEV_VALID;

		faces[2].V1 = Vertices[0];
		faces[2].V2 = Vertices[2];
		faces[2].Valido = LEV_VALID;
}

long Elemento_2D::Get_Material(void)
{
	return material;
}

void Elemento_2D::Set_Material(long &mat)
{
	material = mat;
}

void Elemento_2D::Set_Fonte(double f)
{
	Fonte = f;
}

double Elemento_2D::Get_Fonte()
{
	return Fonte;
}

double Elemento_2D::Get_Angulo()
{
	return Angulo;
}

void Elemento_2D::Set_Angulo(double &a)
{
	Angulo=a;
}

void Elemento_2D::Set_DerivadaPrim(double dp)
{
	DerivadaPrim = dp;
}

double Elemento_2D::Get_DerivadaPrim()
{
	return (DerivadaPrim);
}

void Elemento_2D::Set_Campo(double c)
{
	Campo = c;
}

double Elemento_2D::Get_Campo()
{
	return (Campo);
}


double Elemento_2D::Get_Maior_Comp(vCLevPtMalha & lista_pts, Aresta_Geometria & aresta)
{
	long i;
	Aresta_Geometria arestas[3];
	FACE faces[3];
	double maior_comp=0, comp;

	Monta_Faces(faces);
	for(i=0; i < 3; i++) {
		arestas[i].g1=faces[i].V1;
		arestas[i].g2=faces[i].V2;
		comp = arestas[i].Get_Comprimento(lista_pts);
		if (comp > maior_comp) {
			maior_comp = comp;
			aresta = arestas[i];
		}
	}
	return (maior_comp);
}


double Elemento_2D::Get_Maior_Comp(vCLevPtMalha & lista_pts)
{
	long i;
	Aresta_Geometria arestas[3];
	FACE faces[3];
	double maior_comp=0, comp;

	Monta_Faces(faces);
	for(i=0; i < 3; i++) {
		arestas[i].g1=faces[i].V1;
		arestas[i].g2=faces[i].V2;
		comp = arestas[i].Get_Comprimento(lista_pts);
		(comp > maior_comp)? maior_comp = comp: maior_comp;
	}
	return (maior_comp);
}


void Elemento_2D::Set_Erro(double e)
{
	Erro = e;
}


double Elemento_2D::Get_Erro()
{
	return Erro;
}


void Elemento_2D::Get_Baricentro(vCLevPtMalha &lista, Pt_Malha & o_ponto)
{
	Pt_Malha ponto;
	long i;
	double aux_x, aux_y;
	double x,y;

	x = y = 0;
	for (i = 0; i < N_vertices; i++) {
		ponto = lista[Vertices[i]];
		aux_x = ponto.X();
		aux_y = ponto.Y();
		x += aux_x;
		y += aux_y;
	}
	x = x/ (double)N_vertices;
	y = y/ (double)N_vertices;
	o_ponto.Set_Coord(x,y);
	o_ponto.Set_Tipo(REF);
	o_ponto.Set_Material(material);
	if (Fonte)
		o_ponto.Set_Fonte(Fonte);

}


void Elemento_2D::Calc_Erro(vCLevPtMalha & lista_pts)
{
	double maior_comp = Get_Maior_Comp(lista_pts);

	Erro = DerivadaPrim * DerivadaPrim * maior_comp * maior_comp;
}


//void Elemento_2D::Ordena_Vizinhos(Lista_Elementos_2D_D * lista_elem, long * vizinhos_ordenados)
void Elemento_2D::Ordena_Vizinhos(vElemento2D & lista_elem, long * vizinhos_ordenados)
{
	long i, j, k, verts_elem_viz[3][3];
	Elemento_2D el;

	for (i=0; i<Num_Vizinhos; i++) {
		el = lista_elem[Vizinhos[i]];
		verts_elem_viz[i][0]=el.Get_Vertices(0);
		verts_elem_viz[i][1]=el.Get_Vertices(1);
		verts_elem_viz[i][2]=el.Get_Vertices(2);
	}

	vizinhos_ordenados[0]=vizinhos_ordenados[1]=vizinhos_ordenados[2]=-1;
	for(k=0; k < Num_Vizinhos; k++) {
		for (i = 0; i < N_vertices; i++) {
			(i == 2)? j=0: j=i+1;
			if(((Vertices[i]==verts_elem_viz[k][0]) ||
			    (Vertices[i]==verts_elem_viz[k][1]) ||
			    (Vertices[i]==verts_elem_viz[k][2])) &&
			   ((Vertices[j]==verts_elem_viz[k][0]) ||
			    (Vertices[j]==verts_elem_viz[k][1]) ||
			    (Vertices[j]==verts_elem_viz[k][2]))   )
			{
				vizinhos_ordenados[i]=Vizinhos[k];
			}
		}
	}
}

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: Transformation_Coefficients
//-----------------------------------------------------------------------
//
//  Descricao          : Calcula coeficientes para mapeamento de coordenadas
//						 para o sistema homogêneo
//
//
//
//  versao             : 1.0
//  programador        : Angelo Passaro, adaptado para elemento 2D por Nancy Mieko Abe
//  ultima modificacao : 05/06/2001
//
//-----------------------------------------------------------------------
//void   Elemento_2D::Transformation_Coefficients(Lo_Pt_Malha *lista, double *a, double *b, double *c)
void   Elemento_2D::Transformation_Coefficients(vCLevPtMalha &lista, double *a, double *b, double *c)
{

	double		x_aux1, x_aux2, x_aux3;
	double		y_aux1, y_aux2, y_aux3;
	int i;

	Pt_Malha	p[3];  // triangle vertices
	Pt_Malha	o_aux_ponto;


	for (i = 0; i < N_vertices; i++)
	{
//		lista->Get_Componente(Vertices[i], o_aux_ponto);
		o_aux_ponto = lista[Vertices[i]];
		p[i] = o_aux_ponto;
	}

	x_aux1 = p[0].X();
	y_aux1 = p[0].Y();

	x_aux2 = p[1].X();
	y_aux2 = p[1].Y();

	x_aux3 = p[2].X();
	y_aux3 = p[2].Y();


// -----------------------------------------------------------------------------
// coefficient calculation for the x1 coordinate
// -----------------------------------------------------------------------------

	a[1] = (x_aux2 * y_aux3) - (x_aux3 * y_aux2);

	b[1] = (y_aux2 - y_aux3);

	c[1] = (x_aux3 - x_aux2);

// -----------------------------------------------------------------------------
// coefficient calculation for the x2 coordinate
// -----------------------------------------------------------------------------
	a[2] = (x_aux3 * y_aux1) - (x_aux1 * y_aux3);

	b[2] = (y_aux3 - y_aux1);

	c[2] = (x_aux1 - x_aux3);


// -----------------------------------------------------------------------------
// coefficient calculation for the x3 coordinate
// -----------------------------------------------------------------------------
	a[3] = (x_aux1 * y_aux2) - (x_aux2 * y_aux1);

	b[3] = (y_aux1 - y_aux2);

	c[3] = (x_aux2 - x_aux1);


} // end of Transformation_Coefficients

//==============================================================================

//-----------------------------------------------------------------------
//                 clsass Elemento_2D :: CartesianToHom
//-----------------------------------------------------------------------
//
//  Descricao          : Mapeia um ponto em coordenadas cartesianas
//						 para o sistema homogêneo
//
//
//
//  versao             : 1.0
//  programador        : Angelo Passaro, adaptado para elemento 2D por Nancy Mieko Abe
//  ultima modificacao : 05/06/2001
//
//-----------------------------------------------------------------------
//void   Elemento_2D::CartesianToHom(Lo_Pt_Malha *lista, Pt_Malha &point, Pt_Malha &phom)
void   Elemento_2D::CartesianToHom(vCLevPtMalha &lista, Pt_Malha &point, Pt_Malha &phom)
{
   double zeta2 , zeta3 ;
   double a[4], b[4], c[4];
   Transformation_Coefficients(lista,&a[0], &b[0], &c[0]);
// -----------------------------------------------------------------------------
// homogeneous coordinates calculation
// -----------------------------------------------------------------------------
//   zeta1  = a[1] + b[1] * point.X() + c[1] * point.Y();
//   zeta1 /= volume;
   zeta2  = a[2] + b[2] * point.X() + c[2] * point.Y();
   zeta2 /= volume;
   zeta3  = a[3] + b[3] * point.X() + c[3] * point.Y();
   zeta3 /= volume;
   phom.Set_Coord(zeta2,zeta3);

 } // end of Elemento2D :: CartesianToHom

//-----------------------------------------------------------------------
//                 class Elemento_2D :: HomToCartesian
//-----------------------------------------------------------------------
//
//  Descricao          : Mapeia um ponto em coordenadas homogêneas
//						 para o sistema cartesiano
//
//
//
//  versao             : 1.0
//  programador        : Angelo Passaro, adaptado para elemento 2D por Nancy Mieko Abe
//  ultima modificacao : 05/06/2001
//
//-----------------------------------------------------------------------
void   Elemento_2D ::  HomToCartesian(vCLevPtMalha &lista,Pt_Malha &phom, Pt_Malha &point)
{
	int			i;
	double		coordx, coordy;
	Pt_Malha	p[3];  // triangle vertices
	Pt_Malha	o_aux_ponto;

	for(i = 0; i < N_vertices; i++) {
		o_aux_ponto = lista[Vertices[i]];
		p[i] = o_aux_ponto;
	}

	double X1 = p[0].X(), X2 = p[1].X(), X3 = p[2].X();
	double Y1 = p[0].Y(), Y2 = p[1].Y(), Y3 = p[2].Y();

	coordx = X1 + (X2-X1)*phom.X() + (X3-X1)*phom.Y();
	coordy = Y1 + (Y2-Y1)*phom.X() + (Y3-Y1)*phom.Y();

	point.Set_Coord(coordx, coordy);
}


//-----------------------------------------------------------------------
//                 class Elemento_2D :: Is_Point_Inside
//-----------------------------------------------------------------------
//
//  Descricao          : Verifica se o ponto está dentro do elemento
//
//  retorno			   : true: o ponto está dentro do elemento
//						 false: o ponto não está dentro do elemento
//
//  versao             : 1.0
//  programador        : Nancy Mieko Abe
//  ultima modificacao : 05/06/2001
//
//-----------------------------------------------------------------------
bool Elemento_2D :: Is_Point_Inside(vCLevPtMalha &lista, Pt_Malha &point)
{
	Pt_Malha phom;
	double coef_angular, b;
	bool retorno = false;

	// equação da reta (aresta do elemento no sistema de coordenadas homogêneo)
	coef_angular = -1; //reta que passa pelos ponto (1,0) e (0,1)
	b = 1; // b = y1 - coef_angular * x1

	CartesianToHom(lista, point, phom);

	if(phom.X() >= 0 && phom.X() <= 1.0)
		if( phom.Y() >= 0 && phom.Y() <= (coef_angular*phom.X()+b) )
			retorno = true;

	return retorno;
}


double Elemento_2D :: Get_Raio_Circ (void)
{
	return Raio_Circ;
}

//===================================================================================
// class Elemento_2D :: operator ==
//===================================================================================
// Compara todos os dados membros e dados derivados, deve-se verificar oportunamente
// em função da aplicação
//===================================================================================
char Elemento_2D :: operator ==(Elemento_2D & el)
{
	bool continua = true;

	if(volume == el.volume && Estado == el.Estado && Num_Vizinhos == el.Num_Vizinhos
	   && Coef[0] == el.Coef[0] && Coef[1] == el.Coef[1] && Coef[2] == el.Coef[2]
	   && Vertices[0] == el.Vertices[0] && Vertices[1] == el.Vertices[1]
	   && Vertices[2] == el.Vertices[2] && Vizinhos[0] == el.Vizinhos[0]
	   && Vizinhos[1] == el.Vizinhos[1] && Vizinhos[2] == el.Vizinhos[2]
	   && material == el.material && Fonte == el.Fonte && Angulo == el.Angulo
	   && DerivadaPrim == el.DerivadaPrim && Campo == el.Campo && Erro == el.Erro
	   && Raio_Circ == el.Raio_Circ)
	{
		continua = true;
	}
	else continua = false;

	return continua;
}
//____________________________________________________________________________
