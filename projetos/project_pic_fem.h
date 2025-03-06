#ifndef __PROJECT_PIC_FEM_H__
#define __PROJECT_PIC_FEM_H__


#include <project_solver.h>


class CLevPIC;
class CLevPhenomenon;


class CProject_PIC_FEM : public CProject_Solver
{
protected:

	CLevPIC * ptr_phen;
	int persist_each_x_iterations;

public :

	CProject_PIC_FEM() { ptr_phen = 0;  persist_each_x_iterations =1;}	
	   
	int     Get_Save_Step ()  { return  persist_each_x_iterations;}
	
	void    Save_Steps(int a) { persist_each_x_iterations = a;}

	virtual short   SaveIteration(int iter) =0;
	virtual short	SaveEnergy(int iter) =0;
	virtual short   SaveParticles(std::string filenamepos, std::string filenamevel) =0;
	//virtual void    Associate(CLevPhenomenon *pt_p) =0;	
	//virtual void    Associate(Lo_Templ<Point * , long> *lp);//{CProject_Solver::Associate(lp);}
	//virtual void    Associate(Region *l);//{CProject_Solver::Associate(l);}
	//virtual void    Associate(Lo_Templ<long , long> *l);//{CProject_Solver::Associate(l);}


}; // end class CProject_PIC_FEM


#endif
