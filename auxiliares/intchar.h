
#ifndef __INTSTRING__
#define __INTSTRING__

#include <cstdio>
#include <string>

// Projeto: SDK-LEVSOFT
// Autor: Nancy Mieko Abe, *NMA*
// Data: 01/06/2006

/**
* Classe auxiliar que converte um número inteiro em char e possui operador de incremento
*
* Motivação para criação:
* Facilitar a composição de nome de arquivos para os casos
* com evolução no tempo e outros problemas com várias soluções.
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

    int IntValue(void) {return intvalue;} ///< Função que retorna o valor atual como inteiro
    char* CharValue(void) {return charvalue;} ///< Função que retorna o valor atual como inteiro
    std::string stdStringValue(void) {return stdstr_value;}///< Função que retorna o valor atual como std::string

    //*NMA*:operador de pós-incremento ver Deitel, pag. 546
    char * operator++(int)
    {
        intvalue++;
        Convert();
        return charvalue;
    }///<Operador de pós-incremento que retorna o valor incrementado como um char

};//end of CIntString

#endif

