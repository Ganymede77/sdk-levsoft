#ifndef _LO_TEMPL_
#define _LO_TEMPL_

#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "listao.hpp"

/* ****************************************************************************
*  TEMPLATE Lo_Templ
*  descricao:  Template para vetores da classe T
*              A forma final da template s�o:
*                  template<class T, class Q> class Lo_Templ
*
*              onde a classe T corresponde ao tipo de componente do vetor e
*                   a classe Q corresponde ao tipo de indice a ser utilizado:
*                              (int, unsigned int, char, Q, etc.)
*
* exemplos de uso :
*    Lo_Templ<double,Q>        AA  : AA é um vetor de double que utiliza como indices
*                                       numeros do tipo Q
*    Lo_Templ<Q, unsigned int> BB : BB é um vetor de longs que utiliza como indice
*                                       nmero do tipo unsigned int
*    typedef Lo_Templ<float, int> Lo_FLOAT : Lo_Float �um tipo de vetor que armazena
*                                       floats e o índice é um nmero inteiro
*    Lo_FLOAT       CC               : CC é um vetor do tipo Lo_FLOAT (ver acima)
******************************************************************************/


namespace lev_math {


typedef unsigned short USINT;
typedef unsigned char  Uchar;


//****************** Template Declaretion ************************
template<class T, class Q> class Lo_Templ : public Lista_Ordenada<Q>
{
  protected :
    T      *ptr_pontos;
    virtual  void	    Erro(const USINT err, const char * text) const;
    virtual  void	    Erro_Continuacao (const USINT err, const char * text) const;
  public :
    Lo_Templ( ) ;           // default constructor to initialize local variables
    Lo_Templ( const Lo_Templ<T,Q> &); // copy constructor
    Lo_Templ(T * , Q );
    Lo_Templ(const Q) ;     // construtor  to define the max numbers of components
    ~Lo_Templ();            // destructor to delete allocated pointers.
    virtual inline	void			Apaga         (void);  // erase all list
    virtual inline	void			Libera         (void);  // Dealocate the list
    virtual inline	USINT			Get_Componente(const Q indice, T &);
	T* Get_PtrPontos() { return ptr_pontos; }
    virtual		    Uchar		    Inclui(T & numero);                                 // testada
    virtual		    Uchar		    Inclui(T & numero, const Q  posicao_na_lista);
    virtual		    void		    Copia(T * numeros, const Q quant);
    virtual		    USINT		    Modifica(const Q  ind, T *);                         // testada
    virtual		    USINT		    Modifica(const Q  indice, T &numero);                // testada
                    T &	            operator[] (Q);                                         // testado
    const		    T &		        operator[] (Q) const;                                   // testado
				Lo_Templ<T,Q> &	    operator= (const Lo_Templ<T,Q> &);  // testado
                    bool	        operator!=(const Lo_Templ<T,Q> &);
                    bool	        operator==(const Lo_Templ<T,Q> &);
	inline		    void	        Set_N_Max_Com_Abs (const Q  max_pts) ;                         // testada
	inline  	    Q      	        Get_Data(T* v, const Q a);
                    void	        Set_Data(const T* , const Q a);
                    void	        Troca(const Q  origem, const Q  destino);         // testada
    virtual		    Uchar	        Change_N_Max_Comp_Abs(const Q max_pts);

                    void	        Read(std::string );
                    void	        Save(std::string );
};


//*****************************************************************************
//     Error Mensages used by this Source Code.
//*****************************************************************************
// code        function                           meaning
//-----------------------------------------------------------------------------
// PWP206   -     Change          - indicate that one of indices(from or to)
//                                  are out of the limites from the elements allocated.
// PWP210   -     Get_Componente  - Show that required index number is greater than
//                                   number of allocated component.
// PWP211   - Constructor, Delete - unable to increase the vector ptr_pontos.
//            copy constructor
// PWP213   -     Modify           - Indicated index is greater than the position number
//                                  alocated to a list.
// PWP214   - Get_Componente, op[]- index greater than the number of list components.
//
//*****************************************************************************

const char  texto_mensagem[4][80] = {" : N�o foi poss�vel dimensionar o vetor!!",
             " : Indice do vetor maior que o nmero de componentes alocados!!",
             " : Um dos indices indicados excede o nmero de componentes alocados!!",
             " : Indice maior que o numero de componentes da lista!!"
             };


//***************************************************************************
//
//         T E M P L A T E    FOR LINKED LIST  -  IMPLEMENTATION
//
//***************************************************************************
template <class T, class Q> Lo_Templ<T,Q> :: Lo_Templ() : Lista_Ordenada<Q>()
{
	ptr_pontos = NULL;
}


template<class T, class Q> Lo_Templ<T,Q> :: Lo_Templ(const Q num_comp)
{
	Lo_Templ();
	ptr_pontos = new T[num_comp];
	if (!ptr_pontos) {
		USINT status = 211;
		Erro_Continuacao(status, texto_mensagem[0]);
		this->num_max_componentes_absoluto = 0;
	} else
		this->num_max_componentes_absoluto  = num_comp;
}


template<class T, class Q> Lo_Templ<T,Q>::Lo_Templ(T* v, Q n)
{
	Lo_Templ();
	ptr_pontos = new T[n];
	if (!ptr_pontos) {
		USINT status = 211;
		Erro_Continuacao(status, texto_mensagem[0]);
		this->num_max_componentes_absoluto = 0;
	} else
		this->num_max_componentes_absoluto  = n;
	if (this->num_max_componentes_absoluto == n) {
		memcpy(ptr_pontos, v, n * sizeof (T));
		this->num_componentes_absoluto = n;
	}
}


template<class T, class Q> Lo_Templ<T,Q>::Lo_Templ(const Lo_Templ<T,Q> & orig)
{
	Lo_Templ();
	ptr_pontos = new T[orig.num_max_componentes_absoluto];
	if (!ptr_pontos) {
		USINT status = 211;
		Erro_Continuacao(status, texto_mensagem[0]);
		this->num_max_componentes_absoluto = 0;
	} else
		this->num_max_componentes_absoluto = orig.num_max_componentes_absoluto;
	if (this->num_max_componentes_absoluto == orig.num_max_componentes_absoluto) {
		memcpy(ptr_pontos, orig.ptr_pontos, orig.num_max_componentes_absoluto * sizeof (T));
		this->num_componentes_absoluto = orig.num_componentes_absoluto;
	}
}


template <class T, class Q> Lo_Templ<T,Q>::~Lo_Templ()
{
	this->num_max_componentes_absoluto = this->num_componentes_absoluto = 0;
	if(ptr_pontos) {
		delete [] ptr_pontos;
		ptr_pontos = NULL;
	}
}


//--------------------------------------------------------------------------
// nesta implementacao todos os controles dos elementos sao feito comparando
// o numero de componentes. O vetor nao e zerado de fato
//--------------------------------------------------------------------------
template <class T, class Q> void Lo_Templ<T,Q>::Apaga(void)
{
	this->num_componentes_absoluto = 0;
	if (ptr_pontos) {
		delete [] ptr_pontos;
		ptr_pontos = NULL;
	}
	ptr_pontos = new T[this->num_max_componentes_absoluto];
	if (!ptr_pontos) {
		USINT status = 211;
		Erro_Continuacao(status, texto_mensagem[0]);
		this->num_max_componentes_absoluto = 0;
	}
}


template <class T, class Q> void Lo_Templ<T,Q>::Libera(void)
{
	this->num_componentes_absoluto = 0;
	if(ptr_pontos) {
		delete [] ptr_pontos;
		ptr_pontos = NULL;
	}
}


// =========================================================================
///    Class Lo_Templ :: Get_Componente
// =========================================================================
//  \brief              Fornece uma copia do componente especificado da
//                      lista de double
// observacoes        : seu uso deve ser evitado, mas foi mantida para garantir
//        compatibilidade com programas desenvolvidos com esta biblioteca.
//                      Em seu lugar deve-se usar, preferencialmente, o operador []
//
// observacao         : Este m�odo faz uso do operador de atribui�o de objetos
//                      se os componentes s� objetos complexos (n� tipos b�icos)
//                      o operador de atribui�o para a classe deve ser fornecido pelo
//                      programador.
//
// parametros         : indice : indice de identificacao do elemento da lista
//                      e referencia para o objeto que recebe a copia
//
// \return            : status
// where                0  = OK
//                      210  - As described in table showed on this file header.
//                      214  - As described in table showed on this file header.
//
// versao             : 1.00
// programador        : Angelo Passaro
// ultima modificacao : 09/3/96
// =========================================================================
template <class T, class Q>
USINT Lo_Templ<T,Q> :: Get_Componente (const Q  indice, T & ponto)
{
	USINT status = 0 ;    // retorno de status de existencia do componente requisitado
                          // 0 - OK,   outro - c�igo de erro
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// verifica se o indice �menor que o numero de componentes alocados
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (indice < this->num_max_componentes_absoluto) {
		if (indice < this->num_componentes_absoluto)
			ponto = ptr_pontos[indice];
		else {
			status = 214;
			Erro(status, texto_mensagem[3]);
		}
	} else {
		status = 210;
		Erro(status, texto_mensagem[1]);
	}
	return status;
}


// =========================================================================
//    Class Lo_Templ :: Inclui
// =========================================================================
// descricao          : permite a inclusao de uma novo componente �lista ordenada
//
// observa�o         : A classe T deve fornecer a defini�o do operador de atribui�o (=)
//
// parametros         : o  componente da lista
//
// retorno            : status da inclusao
//                         0 - inclusao normal;
//                         1 - elemento nao incluido (num_componentes igual
//                             ao maximo
//
// versao             : 1.00
// programador        : Angelo Passaro
// ultima modificacao : 29/01/96
// =========================================================================
template <class T, class Q>
Uchar Lo_Templ<T,Q> :: Inclui (T& numero)
{
	Uchar retorno = 0;
	if ((this->num_componentes_absoluto) >= this->num_max_componentes_absoluto) {
		Q  incremento_lista = Q(this->num_max_componentes_absoluto *.1 +1);
		retorno = Change_N_Max_Comp_Abs(this->num_max_componentes_absoluto + incremento_lista);
	}
	if (!retorno) {
		T &interm = ptr_pontos[this->num_componentes_absoluto++];
		interm = numero;
	}
	return retorno;
}


// =========================================================================
//    Class Lo_Templ :: Inclui
// =========================================================================
// descricao          : permite a inclusao de um novo componente �lista, numa posicao
//                      determinada
//
// observa�o         : A classe T deve fornecer a defini�o do operador de atribui�o (=)
//
// observa�o 2       : Esta fun�o membro apresenta um comportamento muito perigoso.
//                      O contador de componentes (this->num_componentes_absoluto �sempre incrementado
//                      independente de a posi�o_na_lista indicada j�conter um componente.
//                      Assim, se forem incluidos em sequencia N objetos na posicao_da_lista
//                      M, o contador indicar�o valor 1000, m�, na realidade somente uma
//                      posi�o est�realmente ocupada.
//                      Sua constru�o foi imaginada para ser de utilidade somente na gera�o
//                      dos pontos do paralelepipedo envolvente no gerador autom�ico de malha.
//
// parametros         : o valor do novo componente, e a posicao na lista
//                      em que este devera ser alocado
//
// retorno            : status da inclusao
//                         0 - inclusao normal;
//                         1 - elemento nao incluido (num_componentes igual
//                             ao maximo
//
// versao             : 1.00
// programador        : Angelo Passaro
// ultima modificacao : 29/01/96
// =========================================================================
template <class T, class Q>
Uchar Lo_Templ<T,Q> :: Inclui (T & numero, const Q  posicao_na_lista)
{
	Uchar retorno;
	if (posicao_na_lista < this->num_max_componentes_absoluto) {
		T &interm = ptr_pontos[posicao_na_lista]; interm = numero;
		this->num_componentes_absoluto++;      // *AP* leia a observa�o 2 no cabe�lho da fun�o
		retorno = 0;
	} else
		retorno = 1;
	return retorno;
}


// =========================================================================
//    Class Lo_Templ :: Copia
// =========================================================================
// descricao          : permite o preenchimento da lista a partir de um vetor de valores
//
// observa�o         :
//
//
// parametros         : o vetor de valores a ser copiado para a lista
//                      e a quantidade de elementos desse vetor
//
// retorno            : status da inclusao
//                         0 - inclusao normal;
//                         1 - elemento nao incluido (num_componentes igual
//                             ao maximo
//
// versao             : 1.00
// programadores      : Ademar Muraro Junior e Angelo Passaro
// ultima modificacao : 07/07/2002
// =========================================================================
template <class T, class Q>
void Lo_Templ<T,Q>::Copia(T * numeros, const Q quant)
{
	Set_N_Max_Com_Abs(quant);  // reinicializa a lista
	memcpy(ptr_pontos, numeros, quant * sizeof(T));
	this->num_componentes_absoluto = quant;
}


// =========================================================================
//    Class Lo_Templ :: Modifica
// =========================================================================
// descricao          : Atualiza os dados do componente indice na
//                      lista ordenada
//
// parametros         : indice : indice de identificacao do componente na lista
//                      pp    ponteiro para o novo componente
//
// retorno            : status de retorno da modificacao :
//                          0  : OK
//                        213  : veja a sobrecarga desta fun�o
//
// observacao         : Esta vers� desta fun�o membro �mantida apenas por compatibilidade
//                      com vers�s anteriores de listas ordenadas. Sugere-se que esta fun�o
//                      n� mais seja utilizada, sendo dada prefer�cia �vers� sobrecarregada.
//                      O operador de atribui�o (=) para a classe de componentes da lista
//                      deve ser definida pelo criador da classe.
//
// Modificado por     : Angelo Passaro
// Modificado em      : 29/01/96
// =========================================================================
template <class T, class Q>
USINT Lo_Templ<T,Q> :: Modifica(const Q indice, T *pp)
{
	return Modifica(indice, *pp);   // *AP* Se houver um erro na Modifica chamada
                                    // aqui, ela interrompe o programa
}


// =========================================================================
//    Class Lo_Templ :: Modifica
// =========================================================================
// descricao          : Atualiza os dados do componente indice na
//                      lista ordenada
//
// observacao         : O operador de atribui�o (=) para a classe de componentes da lista
//                      deve ser definida pelo criador da classe.
//
// parametros         : indice : indice de identificacao do componente na lista
//                      pp    referencia para o novo componente
//
// retorno            : status de retorno da modificacao :
//                          0  : OK
//                        213  : indice indicado maior que o nmero de posi�es alocado
//                               para a lista
//
// Modificado por     : Angelo Passaro
// Modificado em      : 29/01/96
// =========================================================================
template <class T, class Q>
USINT Lo_Templ<T,Q> :: Modifica(const Q  indice, T &pp)
{
	USINT status = 0;
	if (indice < this->num_max_componentes_absoluto) {
		T &interm = ptr_pontos[indice];
		interm = pp;
	} else {
		status = 213;
		Erro (status, texto_mensagem[1]);
	}
	return status;
}


// =========================================================================
//    Class Lo_Templ :: operator []    este m�odo se aplica no programa cliente
// =========================================================================
// CUIDADO : se a funcao erro for retirada o valor devolvido pode nao ter significado
template <class T, class Q>
T & Lo_Templ<T,Q> :: operator[] (Q i)
{
	USINT status;
	if (i >= this->num_max_componentes_absoluto) {
		status = 210;
		Erro(status, texto_mensagem[1]);
	}
	return ptr_pontos[i];
}


// =========================================================================
//    Class Lo_Templ :: operator []    este m�odo se aplica no construtor copiador
//                                     e no operador de associacao
// =========================================================================
// CUIDADO : se a funcao erro for retirada o valor devolvido pode nao ter significado
template <class T, class Q>
const T & Lo_Templ<T,Q> :: operator[] (Q i) const
{
	USINT status;
	if (i < this->num_max_componentes_absoluto) {
		if (i >= this->num_componentes_absoluto) {
			status = 214;
			Erro(status, texto_mensagem[3]);
		}
	} else {
		status = 210;
		Erro(status, texto_mensagem[1]);
	}
	return ptr_pontos[i];
}


// =========================================================================
//    Class Lo_Templ :: operator =
// =========================================================================
template <class T, class Q>
Lo_Templ<T,Q> & Lo_Templ<T,Q> :: operator= (const Lo_Templ<T,Q> &lista_externa)
{
	if (this == &lista_externa)
		return *this;
	else {
		if (ptr_pontos)
			delete [] ptr_pontos;
		ptr_pontos = NULL;
	}
	this->num_max_componentes_absoluto = lista_externa.num_max_componentes_absoluto;
	ptr_pontos = new T[this->num_max_componentes_absoluto];
	if (!ptr_pontos)
		this->num_max_componentes_absoluto = 0;
	else {
		memcpy(ptr_pontos, lista_externa.ptr_pontos,this->num_max_componentes_absoluto*sizeof(T));
		this->num_componentes_absoluto = lista_externa.num_componentes_absoluto;
	}
	return *this;
}


// =========================================================================
//    Class Lo_Templ :: operator !=
// =========================================================================
template <class T, class Q>
bool  Lo_Templ<T,Q> :: operator != (const Lo_Templ<T,Q> & L1)
{
	bool ret = false;
	if (this->num_componentes_absoluto == L1.num_componentes_absoluto) {
		for (int i = 0; i < this->num_componentes_absoluto; i++) {
			if (ptr_pontos[i] != L1.ptr_pontos[i]) {
				ret = true;
				break;
			}
		}
	} else
		ret = true;
	return ret;
}


// =========================================================================
//    Class Lo_Templ :: operator ==
// =========================================================================
template <class T, class Q>
bool  Lo_Templ<T,Q> ::	operator ==   (const Lo_Templ<T,Q> & L1)
{
	return (!(*this != L1));
}


// =========================================================================
//    Class Lo_Templ :: Set_N_Max_Comp_Abs
// =========================================================================
// descricao          : Define o numero maximo de componentes permitidos
//                      na lista.
//
// parametros         : max_pts : numero maximo de componentes
//
// retorno            : void
//
//
// Modificado por     : Angelo Passaro
// Modificado em      : 29/01/96
// =========================================================================
template <class T, class Q>
void Lo_Templ<T,Q>::Set_N_Max_Com_Abs(const Q max_pts)
{
	if(ptr_pontos) {
		delete [] ptr_pontos;
		ptr_pontos = NULL;
	}
	this->num_componentes_absoluto = 0;
	ptr_pontos = new T[max_pts];
	if(!ptr_pontos) {
		USINT status = 211;
		Erro_Continuacao (status, texto_mensagem[0]);
		this->num_max_componentes_absoluto = 0;
	} else
		this->num_max_componentes_absoluto = max_pts;
}


// =========================================================================
//    Class Lo_Templ :: Troca
// =========================================================================
// descricao          : Troca a posicao de dois elementos da lista
//
// parametros         : origem e destino
//
// retorno            : void
//
// erro               : ocorrencia de erro 206. Verifique a tabela de erros no
//                      cabecalho deste fonte
//
// Modificado por     : Angelo Passaro
// Modificado em      : 30/01/96
// =========================================================================
template <class T, class Q>
void Lo_Templ<T,Q> :: Troca (const Q  origem, const Q  destino)
{
	if (origem < this->num_max_componentes_absoluto && destino < this->num_max_componentes_absoluto) {
		T interm = ptr_pontos[origem], & prim = ptr_pontos[origem], & sec = ptr_pontos[destino];
		prim = sec; sec = interm;
	} else {
		USINT status = 206;
		Erro (status, texto_mensagem[2]);
	}
}


// =========================================================================
//    Class Lo_Templ :: Change_N_Max_Comp_Abs
// =========================================================================
// descricao          : Muda o numero maximo de componentes permitidos na
//                      lista, sem perder o conteudo atual. Se o programador
//                      precisar reinicializar a lista deve-se usar o m�odo
//                      Apaga em conjunto com Change_N_Com_Abs.
//
// observacao         : este servi� foi preservado para manter compatibilidade
//                      com vers�s anteriores de classes de listas ordenadas.
//                      As limita�es quanto ao redimensionamento com um nmero
//                      maior de elementos foram eliminados.
//                      �de responsabilidade do programador verificar se o redimensionamento
//                      foi completado conforme especificado.
//
// observa�o 2       : O uso do servi� Set_N_Max_Com_Abs, em substitui�o a este,
//                      para casos em que a lista deve ser reinicializada �recomendado.
//
// parametros         : max_pts numero maximo de componentes
//
// retorno            : 0   :  alteracao concluida com sucesso.  Note-se que se os
//                             elementos nao estiverem armazenados sequencialmente na
//                             memoria aqueles que possuirem �dice maior que
//                             this->num_componentes_absoluto ser� perdidos.
//                             OBSERVACAO: Se max_pts < num_componentes _absoluto
//                             Lista redimensionada para o numero de componentes
//                             ja existente. vALE A MESMA OBSERVACAO PARA O RETORNO = 0
//                             SE OS ELEMENTOS NAO ESTIVEREM ARMAZENADOS SEQUENCIALMENTE.
//                      2   :  lista ordenada nao pode ser redimensionada (mem�ia insuficiente)
//                             A lista original permanece intacta.
//
// Modificado por     : Angelo Passaro
// Modificado em      : 30/01/96
// =========================================================================

/* *************************************************************************
sugestao:

	testar max_pts contra o numero de componentes atual (necessario para o Solver)
	de acordo com os valores de retorno :
		se max_pts > numero atual de componentes e menor que o maximo de componentes
			redimensionar com set_N_Max_Com_Abs e copiar a lista atual. retorno = 0;
		se max_pts < numero atual de componentes
			deixar inalterado o vetor, retorno = 1;
		se max_pts > num_max_componentes_absoluto
			proceder como no primeiro teste e retorno = 0.
***************************************************************************/
template <class T, class Q>
Uchar Lo_Templ<T,Q> :: Change_N_Max_Comp_Abs(const Q max_pts)
{
	Uchar retorno = 0;
	Q limite = max_pts;
	if (max_pts == this->num_max_componentes_absoluto)
		return retorno;
	else if (max_pts < this->num_componentes_absoluto)
		limite = this->num_componentes_absoluto;
	else
		limite = max_pts;

	//copia a lista atual para um novo vetor dimensionado convenientemente
	T *ptr_aux;
	ptr_aux = new T [limite];
	if (ptr_aux) {
		this->num_max_componentes_absoluto = limite;
		memcpy(ptr_aux, ptr_pontos, this->num_componentes_absoluto*sizeof(T));
		delete [] ptr_pontos;
		ptr_pontos = ptr_aux;
	} else
		retorno = 2;
	return retorno;
}


template <class T, class Q>
void Lo_Templ<T,Q> :: Set_Data(const T* v, const Q a)
{
	Set_N_Max_Com_Abs(a);
	this->num_componentes_absoluto = a;
	memcpy(ptr_pontos, v, a * sizeof(T));
}


template <class T, class Q>
Q Lo_Templ<T,Q> :: Get_Data(T* v, const Q a)
{
	Q ret = this->num_componentes_absoluto;
	if (a == this->num_componentes_absoluto)
		memcpy(v,ptr_pontos, this->num_componentes_absoluto * sizeof(T));
	else  ret = 0;
	return ret;
}


template <class T, class Q>
void Lo_Templ<T,Q> :: Erro(const USINT err,const char *texto) const
{
	Erro_Continuacao(err, texto);
    #ifdef __MENS_LO_TEMPL__
	MsgTempl.Display("Execution Stopped ....",BRANCO,AZUL);
  	#endif
	exit (err);   // *AP* Todos os programas devem ser revistos para tratar o
                  // retorno de Modifica. Quando todos forem verificados esta
                  // linha dever ser eliminada, passando status a ser devolvido
                  // para a função ou objeto que acionou Modifica.
}


template <class T, class Q>
void Lo_Templ<T,Q> :: Erro_Continuacao(const USINT err,const char *texto) const
{
	std::string mens("\nERRO PWP ");
	std::stringstream ss;
	ss << err;
	mens += ss.str();
	std::cout << mens << std::endl;
}


template <class T, class Q>
void Lo_Templ<T,Q>::Save(std::string lista_file)
{
	Q  ii;
	std::ofstream  fo;

	// grava col matriz
	std::cout<<   std::endl <<  std::endl <<" List is Storing lista (position and value): "<<std::endl;

	fo.open(lista_file.c_str(),std::ios::out);

	fo << this->num_componentes_absoluto << std::endl;  // numero de elemetos do vetor

	for(ii=0;ii<this->num_componentes_absoluto;ii++) {
		fo << ii+1 << "   " << ptr_pontos[ii] << std::endl;
	}
	fo.close();
}


template <class T, class Q>
void Lo_Templ<T,Q> :: Read(std::string lista_file)
{
	Q  ii, aux;
	T auxdd;

	std :: ifstream  fo;

	fo.open(lista_file.c_str(),std::ios::in);
	fo >> aux;

	Set_N_Max_Com_Abs(aux);
	this->num_componentes_absoluto = aux;  // numero de elemetos do vetor
	for(ii=0;ii<this->num_componentes_absoluto;ii++) {
		fo >> aux >> auxdd ;
		ptr_pontos[ii] = auxdd;
	}

	fo.close();
}


} // end namespace listas


#endif   // teste de declaracao de _LO_TEMPL_

