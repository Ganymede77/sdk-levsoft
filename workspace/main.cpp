#include <cstdlib>
#include <iostream>
#include "project_solver.h"
#include "phenomenon_poisson.h"
#include "lev_input_exception.hpp"
#include "control_poissonpde.h"
//#include "debug_new.h"

int main(int argc, char** argv) {

	int codeErr = 0;
    std::string project_name;

    if(argv[1]) project_name = argv[1];
    else project_name = "1D_FEM";

	CLEV_ControlPoissonPDE *controlPoisson = new CLEV_ControlPoissonPDE;

	controlPoisson->setproj_Name(project_name.c_str());

	codeErr = controlPoisson->run_PDESolver();

	if(controlPoisson)
		delete controlPoisson;

    return codeErr;
}

