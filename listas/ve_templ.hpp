#ifndef  __VE_TEMPL__
#define  __VE_TEMPL__


/*============================================================
  Projeto: SDK-LEVSOFT
  Assunto: List Ve_Templ
  ============================================================
  Desenvolvido por: Laborat�rio de Engenharia Virtual- IEAv

  @author   Angelo Passara, Nancy Mieko Abe
  @file     ve_templ.hpp
  @begin
  @email    angelo@ieav.com.br, nancy@ieav.cta.br
*/


#include "lo_templ.hpp"
#include <complex>


/**
 @ingroup lists
 \class Ve_Templ
 \brief  Class template to build complex vectors structures.

      The template has a class T and  class Q  datas where T correspond to the type of component
        vector and Q  correspond  the type of the vector indices:
                              (int, unsigned int, char, long, etc.)

\Example

\code
    Ve_Templ<double,long>        AA       //: AA is a double vector that uses long integer indices.

    Ve_Templ<long, unsigned int> BB        //: BB is a long integer vector that uses unsigned int indices.

    typedef Ve_Templ<float, int> Lo_FLOAT  //: Lo_Float is a vector that store float numbes and use
                                            //the integer numbers as indices.

    Lo_FLOAT                     CC         //: CC is a Lo_FLOAT vector (see above)

\endcode


 Remark : This template is derived from Lo_Templ, which has a set of operations that
 can not be applyed to generalized list where its elements do not have mathematical operations
 defined as example : +, *.
   Although this template works with basic  mathematical operations, the programmer must create the
the same operetions if the list elements are complex mathematical objects.

*/



/**
 \defgroup enumaration
*/



/**
 \enum Norma
 \ingroup enumaration
 \brief  Select  which norm will be used on vector norms calculus

*/


namespace lev_math {



/* ***** Forward declaretions for functions friends */
template<class T, class Q> class Ve_Templ;
template<class T, class Q>
Ve_Templ<T,Q> operator* (int,const Ve_Templ<T,Q> &);

template<class T, class Q>
Ve_Templ<T,Q>	        operator*     (T,const  Ve_Templ<T,Q> &);

template<class T, class Q>
Ve_Templ<T,Q>	        operator*     (const  Ve_Templ<T,Q> &, T );

template<class T, class Q>
std::ostream &	 operator << (std::ostream & , const Ve_Templ<T,Q> &);

/* ***** End Forward declaretions for functions friends */


enum NORM_TYPE
{
	NORM_EUCLIDIAN = 1, ///< two-norm will be calculeted ||x||_2.
	NORM_INFINITE,      ///< Infinity-norm will be calculeted max_i|x_i|.
	N_TYPE              ///< To be defined.
};


template<class T, class Q> class Ve_Templ : public Lo_Templ<T,Q>
{
public :
	Ve_Templ( ) ;           // construtor default, inicia vari�veis internas
	Ve_Templ( const Ve_Templ<T,Q> &); // construtor copiador
	Ve_Templ(const Q) ;     // construtor 	com definicao do numero maximo de componentes
	Ve_Templ(T*, Q);
	~Ve_Templ();            // destruidor libera ponteiros utilizados

	Ve_Templ<T,Q>   		operator+   (const Ve_Templ<T,Q> &);
	Ve_Templ<T,Q>			operator+   (const T);
	Ve_Templ<T,Q>			operator-   (const Ve_Templ<T,Q> &);
	Ve_Templ<T,Q>			operator*   (const T);
	Ve_Templ<T,Q>			operator*   (const int) const;
	Ve_Templ<T,Q>			operator/   (const T);
	Ve_Templ<T,Q>	&		operator+=  (const Ve_Templ<T,Q> &);
	Ve_Templ<T,Q>	&		operator/=  (const T);

	T				operator*   (const Ve_Templ<T,Q> &);

	void				Soma(T);
	void				Zera(void);
	void				Conjugate();

    virtual double Norma(NORM_TYPE)const;
    void Read(void);

	friend Ve_Templ<T,Q> lev_math::operator* <> (int,const Ve_Templ<T,Q> &);
	friend Ve_Templ<T,Q> lev_math::operator* <> (T,const  Ve_Templ<T,Q> &);
	friend Ve_Templ<T,Q> lev_math::operator* <> (const  Ve_Templ<T,Q> &, T );
	friend std::ostream & operator << <>(std::ostream & p, const Ve_Templ<T,Q> &);
};


//***************************************************************************
//
//         T E M P L A T E    PARA VETORES   - IMPLEMENTA��O
//
//***************************************************************************
template <class T, class Q> Ve_Templ<T,Q> :: Ve_Templ() : Lo_Templ<T,Q>() {}

//__________________________________________________________________________
template<class T, class Q> Ve_Templ<T,Q> :: Ve_Templ(const Q num_comp) :
Lo_Templ<T,Q>(num_comp)  {}
//__________________________________________________________________________
template<class T, class Q> Ve_Templ<T,Q> :: Ve_Templ(const Ve_Templ<T,Q> & orig) :
Lo_Templ<T,Q>(orig) {}
//__________________________________________________________________________
template<class T, class Q> Ve_Templ<T,Q> :: Ve_Templ(T * orig, Q N) :
Lo_Templ<T,Q>(orig, N) {}
//__________________________________________________________________________
template <class T, class Q> Ve_Templ<T,Q> :: ~Ve_Templ()
{
	this->num_max_componentes_absoluto = this->num_componentes_absoluto = 0;
	if(this->ptr_pontos) {
		delete [] this->ptr_pontos;
		this->ptr_pontos = NULL;
	}
}


// =========================================================================
//    Class Ve_Templ :: Zera
// =========================================================================
template <class T, class Q>
void Ve_Templ<T,Q> :: Zera(void)
{
	int zero = 0;
	if (this->num_max_componentes_absoluto)
		memset(this->ptr_pontos, zero, this->num_max_componentes_absoluto * sizeof(T));
}


// =========================================================================
//    Class Ve_Templ :: operator +
// =========================================================================
template <class T, class Q>
Ve_Templ<T,Q>   Ve_Templ<T,Q> :: operator+(const Ve_Templ<T,Q> &lista_ext)
{
	Ve_Templ<T,Q> resultado(lista_ext.num_max_componentes_absoluto);
	for (Q i = 0; i < lista_ext.num_max_componentes_absoluto; i++)
		resultado.ptr_pontos[i] = this->ptr_pontos[i] + lista_ext.ptr_pontos[i];
	resultado.num_componentes_absoluto = lista_ext.num_componentes_absoluto;
	return resultado;
}


// =========================================================================
//    Class Ve_Templ :: operator -
// =========================================================================
template <class T, class Q>
Ve_Templ<T,Q>    Ve_Templ<T,Q> :: operator-(const Ve_Templ<T,Q> &lista_ext)
{
	Ve_Templ<T,Q> resultado(lista_ext.num_max_componentes_absoluto);
	for (Q i = 0; i < lista_ext.num_max_componentes_absoluto; i++)
		resultado.ptr_pontos[i] = this->ptr_pontos[i] - lista_ext.ptr_pontos[i];
	resultado.num_componentes_absoluto = lista_ext.num_componentes_absoluto;
	return resultado;
}


// =========================================================================
//    Class Ve_Templ :: operator +
// descri��o : soma num a todos os elementos do vetor. Equivalente a somar
//             o vetor com um outro vetor onde todos os elementos s�o iguais.
// =========================================================================
template <class T, class Q>
Ve_Templ<T,Q>   Ve_Templ<T,Q> :: operator+(const  T num)
{
	Ve_Templ<T,Q> resultado(this->num_max_componentes_absoluto);
	for (Q i = 0; i < this->num_componentes_absoluto; i++) resultado.ptr_pontos[i] = this->ptr_pontos[i] + num;
	resultado.num_componentes_absoluto = this->num_componentes_absoluto;
	return resultado;
}


// =========================================================================
//    Class Ve_Templ :: operator +=
// descri��o : soma dois vetores
// =========================================================================
template <class T, class Q>
Ve_Templ<T,Q>  &  Ve_Templ<T,Q> :: operator+=(const Ve_Templ<T,Q> &lista_ext)
{
	for (Q i = 0; i < lista_ext.num_max_componentes_absoluto; i++)
		this->ptr_pontos[i] += lista_ext.ptr_pontos[i];
	this->num_componentes_absoluto = lista_ext.num_componentes_absoluto;
	return *this;
}


// =========================================================================
//    Class Ve_Templ :: operator *
// descri��o  : multiplica um valor constante da tipo T a cada um dos componentes
//              do vetor.
//
// data da �ltima modifica��o :  23/01/2003
// modificado por   : Angelo Passaro
// =========================================================================
template <class T, class Q>
Ve_Templ<T,Q>    Ve_Templ<T,Q> :: operator*(const T num)
{
	Ve_Templ<T,Q> resultado(this->num_max_componentes_absoluto);
	for (Q i = 0; i < this->num_componentes_absoluto; i++) resultado.ptr_pontos[i] = this->ptr_pontos[i] * num;
	resultado.num_componentes_absoluto = this->num_componentes_absoluto;
	return resultado;
}


// =========================================================================
//    Class Ve_Templ :: operator *
// descri��o  : multiplica um valor constante inteiro a cada um dos componentes
//              do vetor.
//
// data da �ltima modifica��o :  27/08/2005
// modificado por   : Angelo Passaro
// =========================================================================
template <class T, class Q>
Ve_Templ<T,Q>    Ve_Templ<T,Q> :: operator*(const int num) const
{
	Ve_Templ<T,Q> resultado(this->num_max_componentes_absoluto);
	for (Q i = 0; i < this->num_componentes_absoluto; i++) resultado.ptr_pontos[i] = this->ptr_pontos[i] * num;
	resultado.num_componentes_absoluto = this->num_componentes_absoluto;
	return resultado;
}


// =========================================================================
//    Class Ve_Templ :: operator /
// descri��o  : divide cada um dos componentes do vetor por um valor
//              constante da tipo T.
//
//
//
// data da �ltima modifica��o :
// modificado por   : Angelo Passaro
// =========================================================================
template <class T, class Q>
Ve_Templ<T,Q>    Ve_Templ<T,Q> :: operator/(const T num)
{
	Ve_Templ<T,Q> resultado(this->num_max_componentes_absoluto);
	for (Q i = 0; i < this->num_componentes_absoluto; i++) resultado.ptr_pontos[i] = this->ptr_pontos[i] / num;
	resultado.num_componentes_absoluto = this->num_componentes_absoluto;
	return resultado;
}


// =========================================================================
//    Class Ve_Templ :: operator /=
// descri��o  : divide cada um dos componentes do vetor por um valor
//              constante da tipo T.
//
// data da �ltima modifica��o :
// modificado por   : Angelo Passaro
// =========================================================================
template <class T, class Q>
Ve_Templ<T,Q> &   Ve_Templ<T,Q> :: operator/=(const T num)
{
	for (Q i = 0; i < this->num_componentes_absoluto; i++) this->ptr_pontos[i] /= num;
	return *this;
}


// =========================================================================
//    Class Ve_Templ :: operator *
// descri��o  : o par de fun��es a seguir implementa as opera��es n�o comutativas num*v[nx1], e
//              v[nx1]*num, onde num � um escalar int e e v[nx1] � um vetor.
//
// observa��o : esta opera��o pressup�e que a opera��o num*elemento do vetor
//              esteja definida (por exemplo, o produto de um escalar por um
//              numero complexo
// data da �ltima multiplica��o :  27/08/2005
// modificado por   : Angelo Passaro
// =========================================================================
template <class T, class Q>
Ve_Templ<T,Q>   operator*(int num,const Ve_Templ<T,Q> &lista_ext)
{
	return lista_ext*num;
}


// =========================================================================
//    Class Ve_Templ :: operator *
// =========================================================================
// descri��o  : o par de fun��es a seguir implementa as opera��es n�o comutativas num*v[nx1], e
//              v[nx1]*num, onde num � um escalar double e e v[nx1] � um vetor.
//
// observa��o : esta opera��o pressup�e que a opera��o num*elemento do vetor
//              esteja definida (por exemplo, o produto de um escalar por um
//              numero complexo)
// data modifica��o :  13/03/96
// modificado por   : Angelo Passaro
// data modifica��o :  24/08/2005
// modificado por   : Angelo Passaro
//                    Acesso aos dados membros de lista_ext por fun��o
//                    (problema apresentado na compila��o com VC ++ 6.0,
//                     que n�o est� permitindo o acesso a um dado membro
//                     protected a partir deste operator friend )
//==========================================================================
template <class T, class Q>
 Ve_Templ<T,Q>    operator*(T num,const class Ve_Templ<T,Q> &lista_ext)
{
	T  aux;
	Ve_Templ<T,Q> resultado(lista_ext.Get_N_Max_Com_Abs());
	for (Q i = 0; i < lista_ext.Get_Total_Componentes(); i++) {
		aux = num*lista_ext[i];
		resultado.Inclui(aux);
	}
	return resultado;
}


template <class T, class Q>
Ve_Templ<T,Q>    operator*(const Ve_Templ<T,Q> &lista_ext,T num)
{
	return num*lista_ext;
}


// =========================================================================
//    Class Ve_Templ :: operator *
//
// descri��o : produto interno de dois vetores, cujo resultado � um escalar
//             v[1xn] * w[nx1]  = R[1x1]
// observa��o : A verifica��o do n�mero de elementos nos dois vetores n�o �
//              realizada nesta vers�o. O n�mero de elementos � considerado
//              o menor entre os dois vetores envolvidos.
//                      v[1,n] * w[m,1] = v[1,s] * w[sx1],
//              onde s = menor(n,m);
// importante : EXIGE A IMPLEMENTA��O DA OPERA��O += PARA OS ELEMENTOS DO VETOR
//              EXIGE A IMPLEMENTA��O DA OPERA��O *  PARA OS ELEMENTOS DO VETOR
// data da �ltima multiplica��o :  13/03/96
// modificado por   : Angelo Passaro
// =========================================================================
template <class T, class Q>
T  Ve_Templ<T,Q> :: operator*(const Ve_Templ<T,Q> &lista_ext)
{
	T resultado = 0;
	Q num_elementos = this->num_componentes_absoluto;
	if (num_elementos > lista_ext.num_componentes_absoluto)
		num_elementos = lista_ext.num_componentes_absoluto;
	for (Q i=0; i < num_elementos; i++)
		resultado += this->ptr_pontos[i]*lista_ext.ptr_pontos[i];
	return resultado;
}


// =========================================================================
//    Class Ve_Templ :: Soma
//
// descri��o : realiza a soma de um mesmo numero em todos os elementos do vetor.
//             � equivalente ao operator + (T), so que mais eficiente.
//
//==========================================================================
template <class T, class Q>
void  Ve_Templ<T,Q> :: Soma(T num)
{
	for (Q i = 0; i < this->num_componentes_absoluto; i++)
		this->ptr_pontos[i] += num;
}


#ifndef __COMPLEX__
// =========================================================================
//    Class Ve_Templ :: Norma
//
// descri��o : calcula a norma do vetor de acordo com o tipo de norma.
//
//==========================================================================
template <class T, class Q>
double  Ve_Templ<T,Q> :: Norma(NORM_TYPE tipo)const
{
	T sum = 0;
	double max = 0.0;
	Q i;

	switch(tipo) {
		case NORM_EUCLIDIAN:
			if (this->num_componentes_absoluto > 0) {
				for (i = 0; i < this->num_componentes_absoluto; i++)
					sum += this->ptr_pontos[i]*this->ptr_pontos[i];
			}
			return sqrt(std::abs(sum));
			break;
		case NORM_INFINITE:
			if (this->num_componentes_absoluto > 0) {
				max = std::abs(this->ptr_pontos[0]);
				for(i = 1; i < this->num_componentes_absoluto; i++) {
					if(std::abs(this->ptr_pontos[i]) > max)
						max = std::abs(this->ptr_pontos[i]);
				}
			}
			return max;
			break;
		case N_TYPE:
			return 0;
		default:
			return 0;
	}
}

#else

// =========================================================================
//    Class Ve_Templ :: Norma
//
// descri��o : calcula a norma do vetor de acordo com o tipo de norma.
//
//==========================================================================
template <class T, class Q>
double  Ve_Templ<T,Q> :: Norma(NORM_TYPE tipo)const
{
   // *RYT*
   Q i;

   switch (tipo)
   {
      case NORM_EUCLIDIAN:
		  {
			 T sum;
			 sum = 0;

			 if (this->num_componentes_absoluto > 0)
			 {
				for (i = 0; i < this->num_componentes_absoluto; i++)
				{
					sum += utility::conj(this->ptr_pontos[i]) * this->ptr_pontos[i];
				}
			 }

			 return utility::Abs(sqrt(sum));
		  }
		 break;
      case NORM_INFINITE:
		  {
			 double max;
			 double vabs;

			 if (this->num_componentes_absoluto > 0)
			 {
				max = utility::Abs(this->ptr_pontos[0]);

				for (i = 0; i < this->num_componentes_absoluto; i++)
				{
				   vabs = utility::Abs(this->ptr_pontos[i]);
				   if (vabs > max)
				   {
					  max = vabs;
				   }
				}
			 }

			 return max;
		  }
		 break;
      case N_TYPE:
         return 0;

      default:
         return 0;
   }
}

#endif


template <class T, class Q>
std::ostream & operator << (std::ostream & p, const Ve_Templ<T, Q> & M)
{
	p << std::endl << " Number of Elements : "<< M.Get_Total_Componentes();

	T valor;
	p << std::endl;
	for (int i = 0; i < M.Get_Total_Componentes(); i++) {
		valor = M[i];
		p << valor << " ";
	}
	return p;
}


template <class T, class Q>
void  Ve_Templ<T,Q> :: Conjugate()
{
	 T * mat = this->ptr_pontos;
	 for (long i = 0; i < this->num_componentes_absoluto; i++,mat++)
		 *mat = std::conj(*mat);
}


}; //end namespace


#endif  //__VE_TEMPL__
