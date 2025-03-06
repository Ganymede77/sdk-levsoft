/* 
 * File:   CControl_StPoissonPDE.cpp
 * Author: nancy
 * 
 * Created on 14 de Junho de 2012, 15:51
 */
#include <string>
#include "control_poissonpde.h"
#if 10
//#include "phenomenon_poisson.h"
#include "phenomenon_poisson.h"
#include "phen_poisson_tensor.h"
#include "lev_input_exception.hpp"
#include "lev_memory_exception.hpp"
//#include "project_solver.h"
#include "project_poisson.h"
#endif


CLEV_ControlPoissonPDE::CLEV_ControlPoissonPDE()
{
#if 0    
   flag_compute_Grad_of_Solution    = true;
   flag_compute_Rot_of_Solution     = false;
    
   status_compute_Grad_of_Solution  = false;
   status_compute_Rot_of_Solution   = false;
#endif    
}

CLEV_ControlPoissonPDE::~CLEV_ControlPoissonPDE()
{
}

int CLEV_ControlPoissonPDE::run_PDESolver(void)
{
   int                  codeErr = 0;
#if 10
   //CLevPhenomenon               *phen=NULL;

   CLev_FEM_Poisson_Tensor      *phen=NULL;
//   CProject_Solver               projeto;   
   CProject_Poisson               projeto;   

  // lev_math::Lo_Templ<Point *, long>         lo_points;
   lev_math::Lo_Templ<long,long>            lo_bc_dirichlet;
   lev_math::Lo_Templ<Nodal_Element *,long>   lo_elements;
   
   _region::Ordinary2D_Region            region;
   
   lev_math::Ve_Templ<double,long>         ve_solution;

   Finite_Element :: Associate (&lo_points);

//====================================================================================================    
//   Nodal_Element :: Set_Order(1);
//====================================================================================================    

   region.Associate(&lo_elements);

   projeto.Associate(&lo_points);
   projeto.Associate(&region);
   projeto.Associate(&lo_bc_dirichlet);

   std::cout << "\nCLEV_ControlPoissonPDE::run_PDESolver project name: " << proj_Name_with_path;

   codeErr = projeto.Read(proj_Name_with_path);

   std::cout << "\nCLEV_ControlPoissonPDE::run_PDESolver nome do projeto: " << projeto.ProjectNameWithPath();

   if(!codeErr)
   {

//      phen = new CLev_FEM_Poisson; 
      phen = new CLev_FEM_Poisson_Tensor; 
      
      if(phen)
      {
        phen->Associate(&projeto);

        phen->Associate(&lo_points);
        phen->Associate(&region);
        phen->Associate(&lo_bc_dirichlet);

        //associa tipo de propriedade
        std::string str_phen = projeto.vpPhenSpecificationVector[0]->PhenomenonName();

        std::cout << "\nCLEV_ControlPoissonPDE::run_PDESolver - Study: " << str_phen;
        std::cout.flush();


        if(str_phen == "Electrostatic")
            phen->SetPropertyName("permittivity");
        else
        if(str_phen == "Electrokinect")    
            phen->SetPropertyName("conductivity");
        else
        if(str_phen == "Magnetostatic")    
            phen->SetPropertyName("permeability");
        else
        if(str_phen == "ThermalStationary")    
            phen->SetPropertyName("thermalconductivity");
        else
        {
            std::cout << "\nCLEV_ControlPoissonPDE::run_PDESolver - ATENTION: The study " << str_phen << "  cannot be solved by this module."; std::cout.flush();
            std::cout << "\nThe allowed studies are: " << " Electrostatic, Electrokinect, Magnetostatic or ThermalStationary"; std::cout.flush();

        }

        std::cout << "\nCLEV_ControlPoissonPDE::run_PDESolver - phenomenon property name: " << phen->GetPropertyName() << "\n"; std::cout.flush();
        
        try
        {
            phen->Resolve();
        }

        catch (lev_math::CInput_Exception cie) 
        {
            std::cout << cie;
        }

        std::string nome_solution = proj_Name_with_path + ".p1g";

        projeto.SaveSolution(); // os valores de potencial foram atualizados nos pontos
    }
    else
    {
        std::cout<< "\n Erro na alocação do fenômeno. O programa será encerrado\n";
    }
   }
   else
    {
        std::cout<< "\n Erro na leitura do projeto, codigo: "<< codeErr << "\n";
    }


   
   //desalocação de memória
//   if(phen) delete[] phen;
   if(phen) delete phen;
#endif        
   return codeErr;
    
}
