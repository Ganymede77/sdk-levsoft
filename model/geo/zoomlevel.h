// ZoomLevel.h: interface for the CZoomLevel class.
//
//////////////////////////////////////////////////////////////////////

//===================================================================================================
//    Arquivo:            ZoomLevel.h
//...................................................................................................
//    Descri��o:            armazena e gerencia os n�veis de zoom.
//...................................................................................................
//    Autores:            Diogo Hideo Odan
//                        Angelo Passaro
//                        Nancy Mieko Abe
//---------------------------------------------------------------------------------------------------
//    Classe CZoomLevel
//---------------------------------------------------------------------------------------------------
//    Dados membros:
//
//    Associa��es:
//
//    Observa��es:
//...................................................................................................
//    Data:                Dez/2002
//    Modificado em:
//    Modificado por:
//    Motivo:
//===================================================================================================

#if !defined(AFX_ZOOMLEVEL_H__33EBD0C9_0DC4_11D7_9960_000103D57990__INCLUDED_)
#define AFX_ZOOMLEVEL_H__33EBD0C9_0DC4_11D7_9960_000103D57990__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "levstack.h"
#include "worldwindow.h"


class CZoomLevel : public CWWStack
{
public:
    CZoomLevel();
    CZoomLevel(int stacksize);
    virtual ~CZoomLevel();
protected:

public:
    void            SetOriginalZoomLevel(CWorld_Window worldwnd);
    CWorld_Window   Forward();
    CWorld_Window   Backward();
    void            New_Zoom(CWorld_Window worldwnd);
    void            Reset_Zoom_Index();
    bool            IsBackOK();
    bool            IsForwardOK();
};

#endif // !defined(AFX_ZOOMLEVEL_H__33EBD0C9_0DC4_11D7_9960_000103D57990__INCLUDED_)
