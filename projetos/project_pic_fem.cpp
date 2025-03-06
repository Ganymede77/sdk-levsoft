/*#include "project_pic_fem.h"
#include "phenomenon_pic.h"

#include <fstream>

void  CProject_PIC_FEM :: Associate(CLevPhenomenon *pt_p)
{
	ptr_phen = static_cast<CLevPIC *> (pt_p);
}

short CProject_PIC_FEM :: SaveIteration( int iter)
{  // iter is used in order to build the file names
	short int codeErr = 0;
	short pos_aux, i,j;

	char numIter[6];
	std::string nome_base, nome_potencial, nome_pos , nome_vel, nome_sol, nome_pot_base, nome_pot ;

	itoa(iter,numIter,10);

	nome_base = project_name + numIter;

	nome_pot = nome_base;

	nome_potencial = nome_base + ".p1g";
	nome_pos       = nome_base + "_pos.dat";
	nome_vel       = nome_base + "_vel.dat";
	nome_sol	   = project_name + ".sol";


	codeErr = SaveSolution(nome_potencial, ptr_phen->o_E_Field.solution);
	codeErr = SaveParticles(nome_pos, nome_vel);


	// grava�ao do arquivo .sol
	if(iter == 1)
	{
		std::ofstream fout(nome_sol.c_str(), ios::out);
		if(!fout.fail())
		{
			fout.close();
		}

	}

	std::ofstream fout(nome_sol.c_str(), ios::app);
	if(!fout.fail())
	{
		pos_aux = nome_pot.rfind('\\');

		nome_pot_base.resize(nome_pot.size() - pos_aux + 2);
		for(i=pos_aux+1, j=0;i<nome_pot.size();i++, j++)
		{
			nome_pot_base[j] = nome_pot[i];
		}

//		nome_pot = nome_pot_base + numIter;

		fout <<"\n" << iter << "  " << nome_pot_base;
		fout.close();
	}


	return codeErr;


}

short CProject_PIC_FEM :: SaveParticles(std::string filenamepos, std::string filenamevel)
{
	short int 	codeErr = 0;
	long i, num_particles = 0;


	double charge, mass, x,y,vx,vy;

	Point              position;
	Te_Vector<double>  velocity;


	std::ofstream foutpos(filenamepos.c_str());
	std::ofstream foutvel(filenamevel.c_str());

	if(foutpos.fail() || foutvel.fail())
	{
		cout << "\nErro na abertura de um dos arquivos para grava��o de particulas " << filenamepos.c_str() << "  ,  " << filenamevel.c_str() << endl;

		// *NMA*: !!!!!usar trhow....
		codeErr = 100;
	}
	else
	{
		cout << "\n Gravando arquivos de particulas " << filenamepos.c_str() << "  ,  " << filenamevel.c_str() << endl;
		cout.flush();

		num_particles = ptr_phen->lo_particle.Get_Total_Componentes();


		charge   = ptr_phen->lo_particle[0].GetCharge();
		mass     = ptr_phen->lo_particle[0].GetMass();

		foutpos << num_particles << "\n";
		cout << num_particles << "\n";
		foutvel << num_particles << "\n";

		foutpos << "\n\ncharge = " << charge << ",      mass = " << mass << "\n";
		cout << "\n\ncharge = " << charge << ",      mass = " << mass << endl;
		foutvel << "\n\ncharge = " << charge << ",      mass = " << mass << "\n" ;

		int cont;
		for(i = 0, cont = 0; i < num_particles; i++, cont++)
		{
			position = ptr_phen->lo_particle[i].GetPosition();
			velocity = ptr_phen->lo_particle[i].GetVelocity();

			if (ptr_phen->lo_particle[i].GetCharge() != charge   || ptr_phen->lo_particle[i].GetMass() != mass)
			{
				charge   = ptr_phen->lo_particle[i].GetCharge();
				mass     = ptr_phen->lo_particle[i].GetMass();

				foutpos << "\n\ncharge = " << charge << ", mass = " << mass << "\n\n";
				foutvel << "\n\ncharge = " << charge << ", mass = " << mass << "\n\n";
				cont = 1;
			}

			if (!(cont%15)) { foutpos << "\n"; foutvel << "\n";}

			x = position.X();
			y = position.Y();
			vx = velocity(1);
			vy = velocity(2);


			foutpos<< x << " " << y << " ";
			cout<< x << "  " << y << "  ";
			foutvel << vx  << " " << vy  << " ";

		}// end of for(i = 0; i < ptr_lo_points.Get_Total_Componentes(); i++)
		foutpos.close();
		foutvel.close();
	} //fecha else


	return codeErr;
}

#if 10
short	CProject_PIC_FEM :: SaveEnergy(int iter)
{
	short codeErr = 0;
	std::string filename = project_name + "_energy.dat";
	if (iter == 1)
	{
		std::ofstream fout(filename.c_str(),ios::out);
		fout << "en. potencial" << " " << "en. cinetica" << " " << "en. total" << "\n";

		if (!fout.fail()) fout.close();
	}

	std::ofstream fout(filename.c_str(),ios::app);

	if(fout.fail())
	{
		cout << "\nErro na abertura do arquivo de energias " << filename.c_str() << endl;
		cout.flush();

		// *NMA*: !!!!!verificar a possibilidade de usar trhow....
		codeErr = 100;
	}
	else
	{
		cout << "\n Gravando arquivo de energias " << filename.c_str() << endl;
		cout.flush();

		Te_Vector<double>	ov_vel(2);
		double				massa, vx2, vy2;


		Gauss_Pt pt_gauss[10],  // dimensionar com a maior ordem de quadratura
                 ptransf;

		Nodal_Element	* elem;

		Material		*mater;
		double			permittivity, size;

		int i, j, k,  npgauss;
		long nodes[20];

		int dim = 2, dim_el =2, num_nodes = 3, num_nodes_el = 3, ord_integ = 1;
		Te_Vector<double> ve(dim), vetPot(num_nodes);
		Te_Matrix<double> grad(dim, num_nodes),
			              mataux(dim,num_nodes),
						  zero (dim, num_nodes);

		double ep = 0, ec = 0;

// calculo da energia potencial ep = 0.5 eps sqr(E)
		ptr_regions[0].ptr_lo_elements[0][0]->Gauss_Points(ord_integ,npgauss,&pt_gauss[0]);

		for(i=0;i<ptr_regions[0].ptr_lo_elements->Get_Total_Componentes();i++)
		{
	        elem = ptr_regions[0].ptr_lo_elements[0][i];

			dim_el			= elem->Get_Dimension();
			num_nodes_el	= elem->Get_Nodes(nodes);

			if(dim_el != dim || num_nodes_el != num_nodes)
			{
				grad.ChangeDimensions(dim_el,num_nodes_el);
				mataux.ChangeDimensions(dim_el,num_nodes_el);
				zero.ChangeDimensions(dim_el,num_nodes_el);

				elem->Gauss_Points(ord_integ,npgauss,&pt_gauss[0]);

				num_nodes	= num_nodes_el;
				dim		= dim_el;
			}


			//calcula energia potencial
			mater  = elem->Get_Material();
			permittivity = mater->permittivity;
			size = elem->Get_Size();

			     // laco de integracao
			mataux = zero;
			for (k = 0; k < npgauss; k++)
			{
			// transforma o ponto de integracao em ponto do dominio transformado
			   elem->HomToCartesian(pt_gauss[k],ptransf);
			   elem->GradN(&pt_gauss[k], grad);
			   mataux += grad;
			} // end of for (k)

			for(j=0;j<num_nodes;j++)  //  *AP*
			{
				vetPot(j+1) = ptr_phen->o_E_Field.solution[nodes[j]];
			}


			ve = - (mataux * vetPot);  // sinal menos vem de E = - gradV
			ep += permittivity * 0.5 * size * Dot(ve,ve);

		} // end of for(i=0;i<ptr_lo_regions[0]->ptr_lo_elements->Get_Total_Componentes()

// calculo da energia cin�tica ec = 0.5 m sqr(v)
		// varre particulas
		for ( i = 0 ; i < ptr_phen->lo_particle.Get_Total_Componentes(); i++)
		{
			ov_vel = ptr_phen->lo_particle[i].GetVelocity();
			massa  = ptr_phen->lo_particle[i].GetMass();

			vx2 = ov_vel(1);
			vx2 *= vx2;

			vy2 = ov_vel(2);
			vy2 *= vy2;

			ec += 0.5*massa*(vx2+vy2);

		}

		fout << ep << " " << ec << " " << ep+ec << "\n";

		fout.close();
		return codeErr;

	}
	return codeErr;

}
#endif

*/

