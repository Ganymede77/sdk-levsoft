
#ifndef __DOUBLESTRING__
#define __DOUBLESTRING__

#include <string>
// Projeto: SDK-LEVSOFT
// Autor: Nancy Mieko Abe, *NMA*
// Data: 07/11/2006

/**
* Classe auxiliar que converte um número double em char
* 
* Motivação para criação:
* Facilitar a composição de nome de arquivos para os casos
* com evolução no tempo e outros problemas com várias soluções.
*/

class CDoubleChar
{
protected:
    double doublevalue;		///< Armazena o valor double
    char charvalue[100];	///< Armazena o valor convertido para char
    std::string stdstr_value;///< Armazena o valor convertido para std::string

    void Convert(void) 
    { 
        sprintf(charvalue, "%g", doublevalue);
        stdstr_value = charvalue; 
    }///<Converte valor double em um char

public:
    CDoubleChar(double val)
    {
        SetValue(val);
    } ///<Construtor que recebe um inteiro e o armazena em um char
   
    CDoubleChar(void) {SetValue(0.0);}

    ~CDoubleChar(void){;}///<Destrutor

    void SetValue(double val) 
    {
        doublevalue = val;
        Convert();
    }

    int DoubleValue(void) {return doublevalue;} ///< Função que retorna o valor atual como double
    char* CharValue(void) {return charvalue;} ///< Função que retorna o valor atual como char
    std::string StdStrValue(void){return stdstr_value;}///< Função que retorna o valor atual como std::string

};//end of CIntString


#endif
