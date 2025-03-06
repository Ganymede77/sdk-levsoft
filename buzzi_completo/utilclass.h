#ifndef UTILCLASS_H_
#define UTILCLASS_H_


#include <complex>


//==========================================================================
// If T is not a std::complex then  one has a basic type T.
//==========================================================================
template <typename T> struct number_traits {
	typedef T number_type;
};


//==========================================================================
// If T is std::complex then shows the basic type of the std::complex
//==========================================================================
template <typename T> struct number_traits< std::complex<T> > {
	typedef T number_type;
};


#endif /*UTILCLASS_H_*/
