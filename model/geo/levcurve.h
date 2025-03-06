#ifndef __LEV_CURVE_H__
#define __LEV_CURVE_H__


//=============================================================================
//    Arquivo:            LevCurve.h
//.............................................................................
//    Descricao:          Interface da classe CLevCurve
//.............................................................................
//    Autores:            Angelo Passaro
//                        Nancy Mieko Abe
//-----------------------------------------------------------------------------
//    Classe CLevCurve
//-----------------------------------------------------------------------------
//    Dados membros:
//
//    Associacoes:
//
//    Observacoes:
//.............................................................................
//    Data:                Junho/2000
//    Modificado em:
//    Modificado por:
//    Motivo:
//=============================================================================


#include "levprimitive.h"


class CLevCurve : public CLevPrimitive
{

protected:

    double startAngle;
    double endAngle;

public:

    CLevCurve();
    virtual ~CLevCurve();

    // Se CLevCurve for abstrata, podemos manter Clone() = 0
    virtual CLevPrimitive* Clone() const override = 0;

    //*NMA*
    // Nao esquecer de implementar o construtor de copia e a operacao de atribuicao

};

#endif
