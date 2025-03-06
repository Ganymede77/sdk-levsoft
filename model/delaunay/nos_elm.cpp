#include "nos_elm.hpp"


Nos_Elementos::Nos_Elementos()
{
	n_elementos=0;
}


Nos_Elementos::~Nos_Elementos()
{}


char Nos_Elementos::Ad_Elemento(long indice)
{
	if (n_elementos+1>64) {
		std::cout << "\n Erro: O número máximo de elementos associados ao no foi excedido...";
		return 1;
	}
	lista_elem[(int)n_elementos++] = indice;
	return 0;
}


Nos_Elementos & Nos_Elementos::operator =(Nos_Elementos &v)
{
	int i;
	n_elementos = v.n_elementos;
	for(i=0; i<n_elementos; i++)
		lista_elem[i]=v.lista_elem[i];
	return *this;
}


char Nos_Elementos::operator ==(Nos_Elementos &v)
{
	char flag = 0;
	int i;
	if(n_elementos == v.n_elementos) {
		for (i=0; i<n_elementos; i++)
			if (lista_elem[i]!=v.lista_elem[i]) {
				flag=1;
				break;
			}
		} else
			flag = 1;
	return (flag);
}


char Nos_Elementos::Get_NElementos()
{
	return(n_elementos);
}


void Nos_Elementos::Set_NElementos(char n)
{
	n_elementos = n;
}

//long * Nos_Elementos::Get_IElementos(long *elemtos)
//{
//	memcpy(elemtos, lista_elem, sizeof(char) *n_elementos);
//	return (elemtos);
//}

long Nos_Elementos::Get_IElementos(long indice)
{
//	memcpy(elemtos, lista_elem, sizeof(char) *n_elementos);
	return (lista_elem[indice]);
}


//long Nos_Elementos::Get_IElemento(char * i)
//{
//	return (n_elementos[i]);
//	return (lista_elem[(int)i]);
//}
