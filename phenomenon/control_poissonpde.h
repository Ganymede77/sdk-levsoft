/* 
 * File:   CControl_StPoissonPDE.h
 * Author: nancy
 *
 * Created on 14 de Junho de 2012, 15:51
 */

#ifndef CCONTROL_STPOISSONPDE_H
#define	CCONTROL_STPOISSONPDE_H

//#include "phenomenon_poisson.h"
//#include "lev_input_exception.hpp"
//#include "lev_memory_exception.hpp"
#include "lo_templ.hpp"
#include "point.hpp"

/**
 * @stereotype control 
 */
class CLEV_ControlPoissonPDE 
{
protected:
    std::string proj_Name_with_path;
    lev_math::Lo_Templ<Point *, long>         lo_points;
    
///* variaveis de controle   
#if 0    
    bool flag_compute_Grad_of_Solution;
    bool flag_compute_Rot_of_Solution;
    
    bool status_compute_Grad_of_Solution;
    bool status_compute_Rot_of_Solution;
#endif    
public:
    CLEV_ControlPoissonPDE();
    //CControl_StPoissonPDE(const CControl_StPoissonPDE& orig);
     ~CLEV_ControlPoissonPDE();

     
    void setproj_Name(std::string name_with_path) {proj_Name_with_path = name_with_path;}
    std::string  getproj_Name(void) {return proj_Name_with_path;}

#if 0    
///*---------------------------------------------------------------------    
    inline void setflag_compute_Grad_of_Solution(bool f)   {flag_compute_Grad_of_Solution = f;}
    inline bool getflag_compute_Grad_of_Solution(void)     {return flag_compute_Grad_of_Solution;}

    inline void setflag_compute_Rot_of_Solution(bool f)    {flag_compute_Rot_of_Solution = f;}
    inline bool getflag_compute_Rot_of_Solution(void)      {return flag_compute_Rot_of_Solution;}

    inline void setstatus_compute_Grad_of_Solution(bool f) {status_compute_Grad_of_Solution = f;}
    inline bool getstatus_compute_Grad_of_Solution(void)   {return status_compute_Grad_of_Solution;}

    inline void setstatus_compute_Rot_of_Solution(bool f)  {status_compute_Rot_of_Solution = f;}
    inline bool getstatus_compute_Rot_of_Solution(void)    {return status_compute_Rot_of_Solution;}

//--------------------------------------------------------------------------
    int grad_of_PDESolution(void);
    int rot_of_PDESolution(void);
#endif    

    
    int run_PDESolver(void);

        
};

#endif	/* CCONTROL_STPOISSONPDE_H */

