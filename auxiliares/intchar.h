
#ifndef __INTSTRING__
#define __INTSTRING__

#include <cstdio>
#include <string>

// Projeto: SDK-LEVSOFT
// Autor: Nancy Mieko Abe, *NMA*
// Data: 01/06/2006

/**
* Classe auxiliar que converte um n�mero inteiro em char e possui operador de incremento
*
* Motiva��o para cria��o:
* Facilitar a composi��o de nome de arquivos para os casos
* com evolu��o no tempo e outros problemas com v�rias solu��es.
*/

class CIntChar
{
protected:
    int intvalue;			///< Armazena o valor inteiro
    char charvalue[100];	///< Armazena o valor convertido para char
    std::string stdstr_value;//< Armazena o valor convertido para std::string

    void Convert(void)
    {
        sprintf(charvalue, "%d", intvalue);
        stdstr_value = charvalue;
    }///<Converte valor inteiro em um char

public:
    CIntChar(){SetValue(0);}

    CIntChar(int val)
    {
        SetValue(val);
        // intvalue = val;
        // Convert();
    } ///<Construtor que recebe um inteiro e o armazena em um char

    ~CIntChar(void){;}///<Destrutor

    void SetValue(int val)
    {
        intvalue = val;
        Convert();
    }

    int IntValue(void) {return intvalue;} ///< Fun��o que retorna o valor atual como inteiro
    char* CharValue(void) {return charvalue;} ///< Fun��o que retorna o valor atual como inteiro
    std::string stdStringValue(void) {return stdstr_value;}///< Fun��o que retorna o valor atual como std::string

    //*NMA*:operador de p�s-incremento ver Deitel, pag. 546
    char * operator++(int)
    {
        intvalue++;
        Convert();
        return charvalue;
    }///<Operador de p�s-incremento que retorna o valor incrementado como um char

};//end of CIntString

#endif

