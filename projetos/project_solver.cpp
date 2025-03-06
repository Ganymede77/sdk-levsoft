//=======================================================================================================
// TÃ³picos para implementaÃ§Ã£o futura:
// - tratamento de erros : verificar utilizaÃ§Ã£oo de tratamento de exceÃ§Ã£o: try, catch, thow
// -


#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "project_solver.h"
#include "phenomenon.h"


CProject_Solver::CProject_Solver()
{
	ptr_lo_points = NULL;
	ptr_regions = NULL;
	ptr_lo_long_cc_dirichlet = NULL;

	num_first_order_points = 0;
	delete_ptr_lo_elements_Flag = false;

    NM=0;       ///< numero de materiais no arquivo
    NELT=0;     ///< numero de colunas para a propriedade 1 (originalmente para permitividade)
    NCCOR=0;    ///< numero de colunas para a propriedade 2 (originalmente para condutividade)
    NMAG=0;     //< numero de colunas para a propriedade 3 (originalmente para permeabilidade), usar este espaço para o caso termico tambem
}


CProject_Solver::~CProject_Solver()
{
	if(ptr_lo_points)
		for(int i=0; i<ptr_lo_points->Get_Total_Componentes(); i++)
			if(ptr_lo_points[0][i]) {
				delete ptr_lo_points[0][i]; ptr_lo_points[0][i] = NULL;
			}
	if(delete_ptr_lo_elements_Flag)
		for(int i=0; i<ptr_regions->ptr_lo_elements->Get_Total_Componentes(); i++)
			if(ptr_regions->ptr_lo_elements[0][i]) {
				delete ptr_regions->ptr_lo_elements[0][i]; ptr_regions->ptr_lo_elements[0][i] = NULL;
			}
}


void CProject_Solver::Associate(lev_math::Lo_Templ<Point * , long> *lp) { ptr_lo_points = lp;}
void CProject_Solver::Associate(_region::Region *l) { ptr_regions = l;}
void CProject_Solver::Associate(lev_math::Lo_Templ<long , long> *l) { ptr_lo_long_cc_dirichlet = l;}
void CProject_Solver::Associate(CLevPhenomenon *ptr_p) { ptr_phen = ptr_p;};


//=============================================================================
//Cria_Elemento(short dim)
//=============================================================================
Nodal_Element *CProject_Solver::Cria_Elemento(short dim)
{
	Nodal_Element *el = NULL;
	switch(dim) {
		case 1:
			if(vpPhenSpecificationVector[0]->ElementType()=="hermite1D")
				el = new Hermite_Element_1D(vpPhenSpecificationVector[0]->ElementOrder());
			else
				el = new Lagrange_Element_1D(vpPhenSpecificationVector[0]->ElementOrder());
			break;

		case 2:
			if(vpPhenSpecificationVector[0]->ElementType()=="hermite2D")
				el = new Hermite_Element_2D(vpPhenSpecificationVector[0]->ElementOrder());
			else
				el = new Lagrange_Element_2D(vpPhenSpecificationVector[0]->ElementOrder());
			break;

		case 3:
			if(vpPhenSpecificationVector[0]->ElementType()=="hermite3D")
				el = new Hermite_Element_3D(vpPhenSpecificationVector[0]->ElementOrder());
			else
				el = new Lagrange_Element_3D(vpPhenSpecificationVector[0]->ElementOrder());
			break;
	}
	//el->Set_Order(vpPhenSpecificationVector[0]->ElementOrder());
	return el;
}


//============================================================================
// Cria_Source(void)
//============================================================================
CSource * CProject_Solver :: Cria_Fonte(void)
{
	CSource *s = NULL;
	s = new  CSource;
	return s;
}


//============================================================================
// Cria_Material(void)
//============================================================================
Material * CProject_Solver :: Cria_Material(void)
{
    CIsotropicMaterial *m = NULL;
    m = new  CIsotropicMaterial;
    return m;
} // end of Cria_Material


//=============================================================================
//============================================================================
//          CLASS PROJ_SOLVER_GROUND  :: Cria_Ponto
//============================================================================
//
// Descricao        : cria um ponto com o numero de graus deliberdade e fontes
//                    associados ao fenomeno em estudo. ï¿½ uma funcao virtual que
//                    deve ser definida para cada fenomeno.
//
// parametro        :
// retorno          : ponteiro para o ponto criado (NULL se o ponto nao pode
//                    ser criado
//
// modificada em 19/10/2007
// por: *AP*
// descriï¿½ï¿½o : generalizaï¿½ï¿½o com acesso aos dados do projeto sobre a dimensï¿½o
//             geomï¿½trica do estudo (uso de iProjectDimension e outros) da
//             nova classe pai CProject_Base
//============================================================================
Point * CProject_Solver :: Cria_Ponto(long &index, long totalPoints)
{
	Te_Nodal_Point<double> *p = NULL;
	short int n_variaveis = vpPhenSpecificationVector[0]->DegreesOfFreedom();
	short int nsource = 1, j;

	p = new Te_Nodal_Point<double>('*', n_variaveis, nsource);
	p->Dimension(iProjectDimension);

	for(j=0; j<n_variaveis; j++) {
		//p->DLNC[j] = j*totalPoints+index;
		p->DLNC[j] = index;
		index++;
	}
	//index++;

	return p;
} // End of Cria_Ponto


//=============================================================================
// CProject_Solver::Read
// parameters:	project_name: name of the project without extension
//				default order = 1: order of the elements
//
//=============================================================================
short CProject_Solver::Read(std::string project, short int order)
{
	short codeErr = 0;

    ProjectNameWithPath(project);

    codeErr = ReadSpecDataFile(GetFileName(FILEDATASPEC));
    if(codeErr) {
        std::cout << "\n\nATENCAO!!!!!!!!!!!!! Arquivo .dados NAO encontrado\n\n";
        return codeErr;
    }

	project_name = project;
	std::string file_elements;
	std::string file_points;
	std::string file_bc;
	std::string file_mat;
	std::string file_source;
	std::string file_propfis;
	std::string nome_propfis = "propfis.mei";
	std::string file_order;

	file_order = project_name + ".order";
	std::ifstream fin(file_order.c_str());
	if(!fin.fail()) {
		fin >> order;
		fin.close();
	}

	if(order == 1) {
		file_points		= project_name + ".n1g";
		file_elements	= project_name + ".t1g";
		file_bc			= project_name + ".c1g";
		file_mat		= project_name + ".m1g";
		file_source		= project_name + ".f1g";
	} else if(order == 2) {
		file_points		= project_name + ".n2g";
		file_elements	= project_name + ".t2g";
		file_bc			= project_name + ".c2g";
		file_mat		= project_name + ".m2g";
		file_source		= project_name + ".f2g";
	}

	file_propfis = project_name + ".mei";
	std::filesystem::path checkfile(file_propfis);
	if(std::filesystem::exists(checkfile) == false)
	{
		int pos_aux = project_name.rfind('\\');
		int size = pos_aux+ 2 + nome_propfis.size();
		file_propfis.resize(size);
		for(int i=0;i<=pos_aux;i++)
			file_propfis[i] = project_name[i];
		for(unsigned int i=0;i<=nome_propfis.size();i++)
			file_propfis[pos_aux+i+1] = nome_propfis[i];
	}

	if(!codeErr)
		codeErr = ReadBDMaterials(file_propfis);

	if(!codeErr)
		codeErr = ReadPoints(file_points);

	// para guardar a informacao para a gravacao de arquivos de 1a. ordem, mesmo para elementos de ordem superior
	if(order == 1)
		Set_First_Order_N_Points(ptr_lo_points->Get_Total_Componentes());

	if(!codeErr)
		codeErr = ReadElements(file_elements, order);

	if(!codeErr)
		codeErr = ReadBoundaryConditions(file_bc);

	if(!codeErr)
		codeErr = ReadMaterials(file_mat);

	if(!codeErr)
		ReadSources(file_source);

	return codeErr;
}


//==========================================================================
// CProject_Solver::ReadPoints(std::string filename)
// modificada em 19/10/2007
// por: *AP*
// descricao : generalizacao com acesso aos dados do projeto sobre a
//             dimensao geometrica do estudo (uso de iProjectDimension) da
//             nova classe pai CProject_Base
//==========================================================================
short CProject_Solver::ReadPoints(std::string filename)
{
	short codeErr = 0;
	long num_points, i, j;
	long DLNC_Index = 0;
	double coord[3] = {0.,0.,0.};
	Point * ptr_p = NULL;

	std::ifstream fin(filename.c_str());
	if(fin.fail()) {
		std::cout << "\nErro na abertura do arquivo de pontos " << filename.c_str() << std::endl;
		std::cout.flush();
		codeErr = 100;
	} else {
		std::cout << "\nLendo arquivo de pontos " << filename.c_str() << std::endl;
		std::cout.flush();

		fin >> num_points;

		ptr_lo_points->Set_N_Max_Com_Abs(num_points);

		for(i=0; i<num_points && codeErr==0; i++) {

			for(j=0; j<iProjectDimension; j++)
				fin >> coord[j];

			ptr_p = Cria_Ponto(DLNC_Index, num_points);

			if(ptr_p) {
				ptr_p->Set_Coordinates(&coord[0]);
				codeErr = ptr_lo_points->Inclui(ptr_p);
				if(codeErr) {
					codeErr = 101;
					std::cout << "\n Memoria insuficiente - leitura de pontos - arquivo:" << filename.c_str()  << std::endl;
				}
			}
		}
		fin.close();
	}
	return codeErr;
}


//=============================================================================
//ReadElements(std::string filename)
//=============================================================================
short CProject_Solver::ReadElements(std::string filename, short int order)
{
	short codeErr = 0, j;
	long num_elements, i;
	long vert[21], viz[10];
	short ver, nnos;
	double size;
	Nodal_Element *ptr_el;

	//**RSM-2016-09-15**
	Nodal_Element *p_element = get_Finite_Element_Instance(vpPhenSpecificationVector[0]->ElementType());

	std::ifstream fin(filename.c_str());
	if(fin.fail()) {
		std::cout << "\nErro na abertura do arquivo de elementos " << filename.c_str() << std::endl;
		std::cout.flush();
		codeErr = 100;
	} else {
		std::cout << "\nLendo arquivo de elementos " << filename.c_str() << std::endl;
		std::cout.flush();

		// leitura do numero total de elementos
		fin >> num_elements;
		fin >> ver;

		delete_ptr_lo_elements_Flag = true;

		// aloca a lista de elementos de Region
		ptr_regions->ptr_lo_elements->Set_N_Max_Com_Abs(num_elements);

		//**RSM-2016-09-15**
		p_element->Set_Order(order);
		nnos = p_element->Get_N_Nodes();

		for(i=0;i<num_elements;i++) {
			ptr_el = Cria_Elemento(iProjectDimension);
			if(ptr_el) {
				// leitura dos vertices
				for(j=0;j<ptr_el->Get_N_Nodes();j++) {
					fin >> vert[j];
					//no arquivo ainda esta 1-based portanto eh necessario decrementar 1
					vert[j]--;
				}

				// leitura dos vizinhos
				for(j=0; j<ptr_el->Get_Max_Neighbors(); j++) {
					fin >> viz[j];
					viz[j]--;
					ptr_el->Set_Neighbor(viz[j]);
				}

				// Leitura de size, somente para arquivos com elementos de ordem != 1
				if(order != 1) {
					fin >> size;
					ptr_el->Set_Size(size/2.0);
				} else {

					if(iProjectDimension == 3) {
						//
						// **RSM-2016-09-15**  - Calculo do size para elementos de primeira ordem
						// INICIO
						//
						double b[4], c[4], d[4];
						double x_aux1, x_aux2, x_aux3, x_aux4;
						double y_aux1, y_aux2, y_aux3, y_aux4;
						double z_aux1, z_aux2, z_aux3, z_aux4;
						double aux;

						Point *p[4];  // tetrahedron vertices
						Point *aux_point=NULL;


						//for(int z=0;z<24;z++){
						do {

							for (int w = 0; w < nnos; w++)
							{
							   ptr_lo_points->Get_Componente(vert[w], aux_point);
							   p[w] = aux_point;
							}
							x_aux1 = p[0]->Get_X();
							y_aux1 = p[0]->Get_Y();
							z_aux1 = p[0]->Get_Z();

							x_aux2 = p[1]->Get_X();
							y_aux2 = p[1]->Get_Y();
							z_aux2 = p[1]->Get_Z();

							x_aux3 = p[2]->Get_X();
							y_aux3 = p[2]->Get_Y();
							z_aux3 = p[2]->Get_Z();

							x_aux4 = p[3]->Get_X();
							y_aux4 = p[3]->Get_Y();
							z_aux4 = p[3]->Get_Z();

							// Calculo do size

							b[1] = x_aux1-x_aux4;
							b[2] = y_aux1-y_aux4;
							b[3] = z_aux1-z_aux4;

							c[1] = x_aux2-x_aux4;
							c[2] = y_aux2-y_aux4;
							c[3] = z_aux2-z_aux4;

							d[1] = x_aux3-x_aux4;
							d[2] = y_aux3-y_aux4;
							d[3] = z_aux3-z_aux4;

							size = b[1]*c[2]*d[3]+b[2]*c[3]*d[1]+c[1]*d[2]*b[3]-d[1]*c[2]*b[3]-b[2]*c[1]*d[3]-b[1]*d[2]*c[3];

							if(size<=0){
								aux = vert[0];
								vert[0] = vert[3];
								vert[3] = aux;
							}
							//	if(size>0)
							//		break;
						} while(size<0.0);
						//
						// **RSM-2016-09-15**  - Calculo do size para elementos de primeira ordem
						// FIM
						//
					}
				}

				ptr_el->Set_Nodes(vert);
				ptr_el->MatJacobHom();

				// Inclui elemento na lista de elementos da regiao
				codeErr = ptr_regions->ptr_lo_elements->Inclui(ptr_el);
				if(codeErr)
					codeErr = 101;
			} else
				codeErr = 101;
		}
		fin.close();
	}
	if(p_element) {
		delete p_element; p_element = NULL;
	}
	return codeErr;
}

//=============================================================================
//ReadBDMaterials(std::string filename)
// Obs: Verifica se arquivo esta no formato do propfis.mei, se não está, dá mesangem de erro
//=============================================================================
short CProject_Solver::ReadBDMaterials(std::string filename)
{
	short codeErr = 0;
    std::string line;

	std::ifstream fin(filename.c_str());

	if(fin.fail()) {
		std::cout << "\nCProject_Solver::ReadBDMaterials - Erro na abertura do arquivo de materiais " << filename.c_str() << std::endl;
		std::cout.flush();

		codeErr = 100;
	} else {
		std::cout << "\nCProject_Solver::ReadBDMaterials - Lendo arquivo de materiais " << filename.c_str() << std::endl;
		std::cout.flush();

        getline(fin, line);

        trim(line);

        //*NMA* 2013-10-08 para evitar fazer leitura em arquivos .mei de formatos diferentes
        // se contiver a letra v (de versao) na primeira linha nao esta no formato lido por esta classe de projetos
        if( line.find("v") != std::string::npos ) {
            fin.close();
            std::cout << "\nCProject_Solver::ReadBDMaterials > File format of *.mei not supported for this study ! " << filename.c_str() << std::endl;
            std::cout.flush();
            codeErr = 1002; // formato de arquivo .mei nao suportado
        } else {
            fin.close();
            return ReadBDMaterialsv1(filename);
        }
		fin.close();
	}

	return codeErr;
} // end of ReadBdMaterials


//=============================================================================
//ReadBDMaterialsv1(std::string filename)
// Obs: Leitura de arquivo no formato do propfis.mei
//=============================================================================
short CProject_Solver::ReadBDMaterialsv1(std::string filename)
{
	short	codeErr = 0;
	long	i;
//	int		NELT, NCCOR, NMAG; // auxiliares que nao usadas neste modulo
	char	nome_temp[250];

	std::ifstream fin(filename.c_str());

	if(fin.fail())
	{
		std::cout << "\nCProject_Solver::ReadBDMaterialsv1 - Erro na abertura do arquivo de materiais " << filename.c_str() << std::endl;
		std::cout.flush();
		codeErr = 100;
	} else {
		std::cout << "\nCProject_Solver::ReadBDMaterialsv1 - Lendo arquivo de materiais " << filename.c_str() << std::endl;
		std::cout.flush();

		fin >> NM;
		fin >> NELT;
		fin >> NCCOR;
		fin >> NMAG;

		for(i=0;i<NM;i++)
		{
			fin >> codigos[i];
			fin >> nome_temp;
            vMaterialName.push_back(nome_temp); // nome do material (persiste para gravar o arquivo)
			fin >> materiais[i][0]; // permissividade
			fin >> materiais[i][1]; // condutividade
			fin >> materiais[i][2]; // k1
			fin >> materiais[i][3]; // k2
			fin >> materiais[i][4]; // k3 ou permeabilidade
			fin >> materiais[i][5]; //
			fin >> materiais[i][6];
			//fin >> materiais[i][7]; // gama non-parabolicity parameter - *NMA* 2013-10-08- esta leitura nao deve ser feita nesta classe - ver classes ProjetoSchrodinger
		}

		fin.close();
	}// fecha else

	return codeErr;
}// end of ReadBdMaterialsv1


//=============================================================================
//ReadMaterials(std::string filename)
//=============================================================================
short CProject_Solver::ReadMaterials(std::string filename)
{
	short	codeErr = 0, j;
	long	num_meios,i;
	int		codigo;
	int		pos = 0;
	bool    achou = false;
	Material * mater, *mater_aux;
	double	angulo;

	std::ifstream fin(filename.c_str());

	if(!fin.is_open()) {
		std::cout << "\nCProject_Solver::ReadMaterials - Erro na abertura do arquivo de materiais (meios) " << filename.c_str() << std::endl;
		std::cout.flush();
		codeErr = 100;
	} else {
		std::cout << "\nCProject_Solver::ReadMaterials - Lendo arquivo de materiais (meios) " << filename.c_str() << std::endl;
		std::cout.flush();
		fin >> num_meios;
		for(i=0;i<num_meios;i++) {
			fin >> codigo;
			fin >> angulo;
			achou = false;
			for(j=0;j<NM && achou == false;j++) {
				if(codigos[j] == codigo) {
					achou = true;
					pos = j;
				}
			}

			if(achou == true) {
				mater_aux = ptr_regions->ptr_lo_elements[0][i]->Get_Material();
				if(mater_aux) delete mater_aux;
				mater = Cria_Material();
				if(mater) {
					mater->code			= codigo;
					mater->angle		= angulo;
					mater->permittivity = materiais[pos][0];
					mater->conductivity = materiais[pos][1];
                    // *NMA* e *RYT* 17/10/2007
                    // Alteracao inicialmente realizada pelo Angelo na versao do Rafael
					mater->permeability = 1/materiais[pos][4];// materiais nao lineares nao estao sendo tratados
					ptr_regions->ptr_lo_elements[0][i]->Set_Material(mater);
				} else
					codeErr = 101;
			} else
				codeErr = 102; // codigo de material nao cadastrado
		}
	}
	return codeErr;
}


//=============================================================================
// ReadBoundaryConditions(std::string filename)
//=============================================================================
short CProject_Solver::ReadBoundaryConditions(std::string filename)
{
	short		codeErr = 0;
	long		num_cc,i;
	long		node;
	short int	tipo;
	double		value;
	Te_Nodal_Point<double> *ptr_p;

	std::ifstream fin(filename.c_str());

	if(fin.fail()) {
		std::cout << "\nErro na abertura do arquivo de condicoes de contorno " << filename.c_str() << std::endl;
		std::cout.flush();
		codeErr = 100;
	} else {
		std::cout << "\nLendo arquivo de condicoes de contorno " << filename.c_str() << std::endl;
		std::cout.flush();

		fin >> num_cc;

		//aloca lista de ï¿½ndice de nï¿½s com cc dirichlet
		ptr_lo_long_cc_dirichlet->Set_N_Max_Com_Abs(num_cc);

		for(i=0;i<num_cc;i++) {
			fin >> node;
			node --;  //ï¿½ndice do nï¿½ no arquivo 1-based

			fin >> tipo;
			fin >> value;

			if(tipo == 2) // dirichlet
			{
				codeErr = ptr_lo_long_cc_dirichlet->Inclui(node);
				if(codeErr)
					codeErr = 103;
			}

			ptr_p = (Te_Nodal_Point<double>*) ptr_lo_points[0][node];

			if(tipo == 3 || tipo == 4 || tipo == 5) {
				//*NMA*: para cc float, cï¿½clica e anti-cï¿½clica, value ï¿½ o no de referencia
				value = value-1;
			}

			ptr_p->Boundary_Condition(tipo, value);
		}
		fin.close();
	}
	return codeErr;
}

short CProject_Solver::SaveSolution(std::string filename)
{
	short	codeErr;
	long	num_points, i;
	double	value;
	Point * p_aux;
	Te_Nodal_Point<double> *ponto;

	std::ofstream fout(filename.c_str());

	if(fout.fail())
	{
		std::cout << "\nErro na abertura do arquivo de soluÃ§Ã£o: " << filename.c_str() << std::endl;

		//*NMA*: !!!!!verificar a possibilidade de usar trhow....
		codeErr = 100;
	}
	else
	{
//		num_points = ptr_ve_double_solution->Get_Total_Componentes();
		num_points = ptr_lo_points->Get_Total_Componentes(); //OK

//		num_points = solution.Get_Total_Componentes();

		fout << num_points << std::endl;

		for(i=0;i<num_points;i++) {
			p_aux = ptr_lo_points[0][i];
			ponto = (Te_Nodal_Point<double> *) p_aux;
			value = ponto->Get_Variables(1);
			fout.setf(std::ios_base::fixed, std::ios_base::floatfield);
			fout << value <<  std::endl;
		}
		fout.close();
	}
	return codeErr;
}


short CProject_Solver::SaveSolution(std::string filename, lev_math::Lo_Templ<double,long> &sol)
{
	short	codeErr;
	long	num_points, i;
	double	value;

	std::ofstream fout(filename.c_str());

	if(fout.fail()) {
		std::cout << std::endl << "Erro na abertura do arquivo de soluÃ§Ã£o: " << filename.c_str() << std::endl;
		codeErr = 100;
	} else {
		num_points = sol.Get_Total_Componentes();
		fout << num_points << '\n';
		for(i=0;i<num_points;i++) {
			value = sol[i];
			fout << value <<  '\n';
		}
		fout.close();
	}
	return codeErr;
}


short CProject_Solver::SaveElements(std::string filename, short int order)
{
	long i, j, k, num_elements;
	long ver[21], *viz;
	long nnos = 0;
	short int	nviz = 0;
	short int	codeErr = 0;
	short int	versao = 1;
	long  int	ind;
	double size;

	std::ofstream fout(filename.c_str());

	if(fout.fail()) {
		std::cout << std::endl << "Erro na abertura do arquivo de elementos para gravaÃ§Ã£o " << filename.c_str() << std::endl;
		codeErr = 100;
	} else {
		std::cout << "\n Gravando arquivo de elementos " << filename.c_str() << std::endl;
		std::cout.flush();

		for ( k = 0; k < _region::Region::Total_Regions(); k++) {
			num_elements = ptr_regions[k].ptr_lo_elements->Get_Total_Componentes();

			fout << num_elements << " " << versao << '\n';

			for (i = 0; i < ptr_regions[k].ptr_lo_elements->Get_Total_Componentes(); i++) {

				nnos = ptr_regions[k].ptr_lo_elements[0][i]->Get_Nodes(&ver[0]);
				viz = ptr_regions[k].ptr_lo_elements[0][i]->Get_Neighbors(nviz);

				if(order == 1)
					nnos = 3;

			// grava indices dos vï¿½rtices
			for (j = 0; j < nnos; j++)
			{
				ind = ver[j]+1;
				fout << ind << " "; // os arquivos ainda sï¿½o 1-based
			}
			// grava indices dos elementos vizinhos
			for (j = 0; j < 3; j++)
			{
				ind = viz[j]+1;
				fout << ind << " ";// os arquivos ainda sï¿½o 1-based
			}
			//*NMA*: Obs: O projeto nï¿½o poderï¿½ ser lido pelo Explorador, como estï¿½ hoje 05/07/2004
			// grava size
			if(order == 2)
			{
				size = ptr_regions[k].ptr_lo_elements[0][i]->Get_Size();
				fout << " " << size;
			}

			fout << "\n" ;

		  } // fim do laco i
		} // fim do laco k

	}

	return codeErr;

} //end of short CProject_Solver::SaveElements(std::string filename)

short CProject_Solver::SavePoints(std::string filename, short int order)
{
	short int 	codeErr = 0;
	long i, num_points = 0;

	double x, y;

	Te_Nodal_Point<double> *ponto;

	std::ofstream fout(filename.c_str());

	if(fout.fail())
	{
		std::cout << "\nErro na abertura para gravaÃ§Ã£o do arquivo de pontos " << filename.c_str() << std::endl;

		//*NMA*: !!!!!verificar a possibilidade de usar trhow....
		codeErr = 100;
	}
	else
	{
		std::cout << "\n Gravando arquivo de pontos " << filename.c_str() << std::endl;
		std::cout.flush();

		num_boundary_conditions = 0;
		num_points = ptr_lo_points->Get_Total_Componentes();

		if(order == 1)
		{
			if(num_first_order_points)
				num_points = num_first_order_points;
		}

		fout << num_points << "\n";

		for(i = 0; i < num_points; i++)
		{
			ponto = (Te_Nodal_Point<double> *) ptr_lo_points[0][i];

			x = ponto->X();
			y = ponto->Y();

			fout << x << " " << y << "\n";
//			fout << y;
//			if(ponto->Dimension() == 3)
//				fout << ponto->Z();
//			fout << "\n";

			// conta o nï¿½mero de pontos com boudary condition
			if (ponto->Boundary_Condition())
				num_boundary_conditions++;
		}// end of for(i = 0; i < ptr_lo_points.Get_Total_Componentes(); i++)
	} //fecha else

	return codeErr;
}

short CProject_Solver::SaveBoundaryConditions(std::string filename, short int order)
{
	short int codeErr = 0;
	short int tipo;
	double value;
	long num_points = 0;

	Te_Nodal_Point<double> *ponto;
	long i;

	std::ofstream fout(filename.c_str());

	if(fout.fail()) {
		std::cout << "\nErro na abertura para gravaÃ§Ã£o do arquivo de pontos " << filename.c_str() << std::endl;
		codeErr = 100;
	} else {
		std::cout << "\n Gravando arquivo de condiÃ§Ãµes de contorno " << filename.c_str() << std::endl;
		std::cout.flush();

		// num_boundary_conditions foi atualizado na gravaï¿½ï¿½o do arquivo de pontos
		fout << num_boundary_conditions << "\n";

		if(order == 1) {
			if(num_first_order_points)
				num_points = num_first_order_points;
		} else
			num_points = ptr_lo_points->Get_Total_Componentes();

		for(i = 0; i < num_points; i++) {
			ponto = (Te_Nodal_Point<double> *) ptr_lo_points[0][i];
			if(ponto->Boundary_Condition()) {
				tipo = ponto->Boundary_Condition();
				value =ponto->Get_Variables(1);
				if(tipo == 3 || tipo == 4 || tipo == 5)
				{
					//*NMA*: para cc float, cï¿½clica e anti-cï¿½clica, value ï¿½ o no de referencia
					value = value+1;
				}
				fout << i+1 << " "; // o arquivo ainda ï¿½ 0-based
				fout << tipo << " ";
				fout << value << "\n" ;
			}
		}
	}
	// a variavel global numero_CC ï¿½ obtida no laco de gravacao da funcao Grava_Pontos
	return codeErr;
}


short CProject_Solver::SaveMaterials(std::string filename)
{
	short int codeErr;
	long num_elements, i;
	Material * material = NULL;

	std::ofstream fout(filename.c_str());

	if(fout.fail()) {
		std::cout << "\nErro na abertura para gravaï¿½ï¿½o do arquivo de materiais " << filename.c_str() << std::endl;
		codeErr = 100;
	} else {
		std::cout << "\n Gravando arquivo de materiais " << filename.c_str() << std::endl;
		std::cout.flush();

		num_elements = ptr_regions->ptr_lo_elements->Get_Total_Componentes();

		fout << num_elements;
		fout << "\n" ;

		for(i=0;i<num_elements;i++) {
			material = ptr_regions->ptr_lo_elements[0][i]->Get_Material();
			fout << material->code << "  " << material->angle;
			fout << "\n" ;
		}
	}
	return codeErr;
}


//=============================================================================
// For compatibility with the old LEV Explorer
//=============================================================================
short CProject_Solver::SaveProj(PHENOMENONTYPE phen)
{
	short codeErr = 0;
	int i;

	//==================
    // Get current time
	//==================
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);

	std::string s[20];

	s[0]  = sProjectName;           // Project name
	s[1]  = "LEV";                  // Designer name
	s[2]  = "IEAv/DCTA";            // Company name
	s[3]  = std::string(buffer);    // Creation Date
	s[4]  = std::string(buffer);    // Last Modified In
	s[5]  = "";                     // Description
	s[6]  = sProjectName + ".ell";  // Modelo Conexao
	s[7]  = sProjectName + ".ege";  // Modelo Geometria
	s[8]  = sProjectName + ".ept";  // Modelo Coordinates
	s[9]  = sProjectName + ".epd";  // Modelo Pre;
	s[10] = sProjectName + ".n1g";  // Geometria
	s[11] = sProjectName + ".g1g";  // Segmentos
	s[12] = sProjectName + ".t1g";  // Elementos
	s[13] = sProjectName + ".c1g";  // CC
	s[14] = sProjectName + ".m1g";  // Meios
	s[15] = sProjectName + ".f1g";  // Fontes
	s[16] = sProjectName + ".p1g";  // Potenciais
	s[17] = sProjectName + "_areas.ar";   // Arestas
	s[18] = sProjectName + ".art";  // Elementos Arestas

	std::string filename = project_name + ".proj";
	FILE *far = fopen(filename.c_str(), "wb");

	for(i=0;i<5;i++)
		if(s[i].size())
			fprintf(far, "%c%s", s[i].size(), s[i].c_str());
		else
			fprintf(far, "%c", 0);

	// Length unit, Mass unit, Time unit, Angle unit, Temperature unit
	for(i=0; i<20; i++)
		fprintf(far, "%c", 0x00); // 5 int, 1 int = 4 bytes

	// Description
	if(s[5].size())
		fprintf(far, "%c%s", s[5].size(), s[5].c_str());
	else
		fprintf(far, "%c", 0);

	// Phenomenon type
	switch(phen) {
		case SCALAROPTICALWAVE:
			fprintf(far, "%c", 0x05);                     // 1
			for(i=0; i<3; i++)
				fprintf(far, "%c", 0x00);  // 3
			break;
		default:
			for(i=0; i<4; i++)
				fprintf(far, "%c", 0x00);  // 4
	}

	// Mesh type, Symmetry type
	fprintf(far, "%c", 0x01);  // 1
	for(i=0; i<7; i++)
		fprintf(far, "%c", 0x00);  // 7

	for(i=6;i<19;i++)
		if(s[i].size())
			fprintf(far, "%c%s", s[i].size(), s[i].c_str());
		else
			fprintf(far, "%c", 0);

	fclose(far);

	return codeErr;
}


short CProject_Solver::Save(std::string project_name, short int order)
{
	short codeErr = 0;
	short element_order;

	std::string file_elements;
	std::string file_points;
	std::string file_bc;
	std::string file_mat;
	std::string file_source;
	std::string file_order;

	file_order = project_name + ".order";
	std::ofstream fout(file_order.c_str());
	if(!fout.fail()) {
		fout << order;
		fout.close();
	}

	// verificaï¿½ï¿½o da ordem dos elementos: para poder gravar como 1a. ordem, mesmo os de segunda ordem
	element_order = ptr_regions->ptr_lo_elements[0][0]->Order();

	// se a ordem de elemento pedida for maior do que a ordem do elemento, salva como a ordem real
	if(order > element_order)
		order = element_order;

	//*NMA*: criando nomes com as extensï¿½es mais usadas para rapidez
	if(order == 1) {
		file_points		= project_name + ".n1g";
		file_elements	= project_name + ".t1g";
		file_bc			= project_name + ".c1g";
		file_mat		= project_name + ".m1g";
		file_source		= project_name + ".f1g";
	} else if(order == 2) {
		file_points		= project_name + ".n2g";
		file_elements	= project_name + ".t2g";
		file_bc			= project_name + ".c2g";
		file_mat		= project_name + ".m2g";
		file_source		= project_name + ".f2g";
	}
	if(!codeErr)
		codeErr = SavePoints(file_points, order);
	if(!codeErr)
		codeErr = SaveElements(file_elements, order);
	if(!codeErr)
		codeErr = SaveBoundaryConditions(file_bc, order);
	if(!codeErr)
		codeErr = SaveMaterials(file_mat);
	//	if(!codeErr)
	//		codeErr = ReadSource(file_source);
	return codeErr;
}


short int CProject_Solver::ReadSources(std::string filename)
{
	int	codeErr = 0;
	long num_el_fontes,i;
	long ind_element;
	double value;
	CSource	*ptr_source=NULL, *ptr_source_aux=NULL;
	CFunction *function_base = NULL;

	std::ifstream fin(filename.c_str());
	if(fin.fail()) {
		std::cout << "\nErro na abertura do arquivo de fontes ou arquivo nï¿½o existente. " << filename.c_str() << std::endl;
		std::cout.flush();
		codeErr = 1014;
	} else {
		std::cout << "\nLendo arquivo de fontes " << filename.c_str() << std::endl;
		std::cout.flush();

		fin >> num_el_fontes;//nï¿½mero de elementos com fontes

		for(i=0;i<num_el_fontes;i++) {
			fin >> ind_element;
			ind_element --;  //ï¿½ndice do elemento no arquivo 1-based
			fin >> value;

			ptr_source_aux = ptr_regions->ptr_lo_elements[0][ind_element]->Get_Source();
			if(ptr_source_aux)
				delete ptr_source_aux;

			ptr_source = Cria_Fonte();

			if(ptr_source) {
				//*NMA*: por enquanto somente fonte constante
				function_base = new CFunctionConstant;
				if(function_base) {
					function_base->Set_Value(value);
					ptr_source->AssociateConstantFunction((CFunctionConstant*)function_base);
					//associa fonte ao elemento
					ptr_regions->ptr_lo_elements[0][ind_element]->Set_Source(ptr_source);
				} else {
					std::cout << "\n Memoria insuficiente - leitura de fontes  - arquivo:" << filename.c_str()  << std::endl;
					codeErr = 1015;
				}
			} else {
				std::cout << "\n Memoria insuficiente - leitura de fontes  - arquivo:" << filename.c_str()  << std::endl;
				codeErr = 1016;
			}
		}
		fin.close();
	}
	return codeErr;
}


Nodal_Element * CProject_Solver :: get_Finite_Element_Instance(const  std::string type)
{
	if (type == "simplex1D" || type == "lagrange1D") {
		return new Lagrange_Element_1D();
	} else if (type == "hermite1D") {
		return new Hermite_Element_1D();
	} else if (type == "simplex2D" || type == "lagrange2D") {
		return new Lagrange_Element_2D();
	} else if (type == "hermite2D") {
		return new Hermite_Element_2D();
	} else if (type == "simplex3D" || type == "lagrange3D") {
		return new Lagrange_Element_3D();
	} else if (type == "hermite3D") {
		return new Hermite_Element_3D();
	}
	return 0;
}



