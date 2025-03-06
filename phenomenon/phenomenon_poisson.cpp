#include <phenomenon_poisson.h>
#include <lev_non_sta_ite_met_bicgstab.hpp>

#ifdef _SOLVEBYSUPERLU_
#include <slusolve.hpp>
#endif


using namespace std;


#define NUM_MEDIO_LIGACOES_POR_NO 5


//=========================== PENDENCIAS E VERIFICA��ES ====================

//
//    Importante: utiliza��o de �ndices 0-based!!!!!!!!!!!!!!!!!!
//
//    - Seek_Node_Links: faz chamada a NODE::Consolida() // verificar se �
//      necess�rio manter definir NUM_MEDIO_LIGACOES_POR_NO (usada em
//      Initialize) definida acima definir TipoVariavel em algum lugar:
//      retirado
//
//    - Verificar se seria vi�vel passar a matriz, o rhs e solution como
//      par�metro para as fun��es ou referencias
//
//    - Build_Local_Matrices: Existe um problema de dimensionamento de
//      matrizes na Build_Local_Matrices pois ela agora considera elementos
//      1D, 2D e 3D
//    - o dimensionamento das matrizes usadas deve ser feito nas fun��es
//      que as recebem como par�metro
//        - em Region::Mat_Jacobian
//        - em GradN, DivN, ... dos elementos simplex
//      por enquanto, est� acertado para 2D
//
//    - Build_Local_Matrices: � necess�rio verificar o tipo de estudo para
//      obten��o da propriedade adequada, por enquanto est� acertado para
//      casos eletrost�ticos
//
//==========================================================================

//==============================================================================
//...........I M P L E M E N T A T I O N ......................................
//==============================================================================


void CLev_FEM_Poisson::Resolve(void)
{
    Initialize();
    Build();
    Impose_Boundary_Conditions();
    Solve();
}


/***************************************************************************/
/* Nome do Arquivo:    phenomenon_Poisson.cpp                              */
/* Nome da Funcao:     Initialize   (antiga Build_Skeleton )               */
/* Versao:             1.0                                                 */
/* Compilador:         C++                                                 */
/* Programador:        Nancy Mieko Abe                                     */
/* (extens�o para n_graus de liberdade : Angelo Passaro                  */
/* Ultima modificacao: 04/02/98                                            */
/* Modificacoes:                                                           */
/*                     Volta ao codigo antigo de montagem dos vetores      */
/*                     de ligacoes                                         */
/*                                                                         */
/*        25/02/97     Inclusao da funcao AP_Estende_Esqueleto, que permite*/
/*                     estender os vetores coluna e diagonal em funcao do  */
/*                     numero de graus de liberdade por no.                */
/*                                                                         */
/*        04/02/98     uso das funcoes preparadas para as novas classes de */
/*                     elementos finitos, regiao e material, as quais      */
/*                     utilizam os recursos disponiveis no C++             */
/*                                                                         */
/***************************************************************************/
/* Descricao:                                                              */
/*                                                                         */
/*   Esta funcao monta o vetor COL, a partir dos vetores de ligacoes       */
/* dos nos.                                                                */
/*                                                                         */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
/* Strings que indica possiveis modificaoes futuras:                       */
/*                                                                         */
/*                             *NMA* *AP*                                  */
/*                                                                         */
/***************************************************************************/
//ultimo codigo de erro: 3289
/***************************************************************************/
void CLev_FEM_Poisson::Initialize(void)
{
	#define __NEW__

    long i;
    int retorn;
    double zero = 0.0;
    long dom_size = ptr_lo_points->Get_Total_Componentes() * proj->vpPhenSpecificationVector[0]->DegreesOfFreedom();

    Gmatrix.set(dom_size, lev_math::UNSYMMETRIC);

    rhs.Set_N_Max_Com_Abs(dom_size);

    for(i=0; i<dom_size; i++) {
        retorn = rhs.Inclui(zero);
        if (retorn) {
            retorn = 3523;
            saida(retorn);
        }
    }

    solution.Set_N_Max_Com_Abs(dom_size);
    for(i=0; i<dom_size; i++) {
        retorn = solution.Inclui(zero);
        if (retorn) {
            retorn = 3523;
            saida(retorn);
        }
    }
}



/**********************************************************************
GNM - 05/07/2006

    As fun��es:
        void Mesh_Node_Links(lev_math::NODE *);   *GNM*
        void TransferAll_Node_Links(lev_math::NODE *);  *GNM*
realizar�o a tarefa de Seek_Node_Links(lev_math::NODE *) em duas etapas pela chamada das duas fun��es acima.
Isso permite que o EFGM utilize a primeira fun��o, Mesh_Node_Links(lev_math::NODE *), para obter
a conectividade geom�trica real da malha, antes das associa��es dos n�s com condi��o de contorno
com os n�s de refer�ncia, facilitando o c�lculo dos raios de influ�ncia nodais.
Ap�s esta etapa e o c�lculo dos dom�nios de influ�ncia, a segunda etapa � realizada
pelo EFGM com a chamada aa fun��o void TransferAll_Node_Links(lev_math::NODE *) mantendo a
consistencia com os procedimentos usados na FEM_poisson que executara a
Seek_Node_Links().
***********************************************************************/

/***************************************************************************/
/*            void CLev_FEM_Poisson :: Mesh_Node_Links(lev_math::NODE * nos) */
/* Descricao:                                                              */
/*                                                                         */
/*   Esta funcao monta os vetores de ligacoes geometricas de todos os nos. */
/*   Utiliza a classe lev_math::NODE para isso.                            */
/*                                                                         */
/***************************************************************************/
void CLev_FEM_Poisson :: Mesh_Node_Links(lev_math::NODE * nos)
{
	long j,k, NOh;
	long e[21];    // indice de nos, obtido a partir dos elementos
	long n_lig;
	long index_no;
	short regiao, nnodes, no_pertence_vetor;
	Nodal_Element * elem;

	//.........................................................................................
	// Por enquanto verifica liga��es de todos os elementos (1D, 2D, 3D) que existirem
	for (regiao = 0; regiao < _region::Region::Total_Regions(); regiao++) {
		for (j = 0; j < ptr_regions[regiao].ptr_lo_elements->Get_Total_Componentes(); j++) {
			elem = ptr_regions[regiao].ptr_lo_elements[0][j];
			nnodes = elem->Get_Nodes(&e[0]);
			for (NOh = 0; NOh < nnodes; NOh++) {
				for (k = 0; k < nnodes; k++) {
					no_pertence_vetor = false;
					index_no = e[NOh];
					n_lig = nos[index_no].n_ligacoes;
					nos[e[NOh]].Inclui_Verifica_Ligacao(e[k]);
				}
			}
		}
	}
}


/***************************************************************************/
/*            void CLev_FEM_Poisson :: TransferAll_Node_Links(lev_math::NODE * nos)    */
/* Descricao:                                                              */
/*                                                                         */
/*   Esta funcao transfere as liga��es dos n�s com condi��o de contorno    */
/*   flutuante, c�clica e anti-c�clica  para os n�s de refer�ncia usando a */
/*   lista "nos", passada por refer�ncia, que contem todas as              */
/*   liga��es geom�tricas dos n�s.                                         */
/*                                                                         */
/***************************************************************************/
void CLev_FEM_Poisson :: TransferAll_Node_Links(lev_math::NODE * nos)
{
  Te_Nodal_Point<double> *ponto;
  int        tipo_cc=0, tipo_cc_no_ref;
  long        i, j,num_points;
  long        no_ref;

    num_points = ptr_lo_points->Get_Total_Componentes();
    for(i=0;i<num_points;i++)
    {
        ponto = (Te_Nodal_Point<double> * &) ptr_lo_points[0][i];

        tipo_cc = ponto->Boundary_Condition();

        if(tipo_cc == 3 || tipo_cc == 4 || tipo_cc == 5) // � ponto com condi��o de contorno 3:float 4:c�clica 5: anti-c�clica
        {
            no_ref = (long) ponto->Get_Variables(1); // no caso destas cc valor_cc � o n� de referencia

            //verifica se o n� de refer�ncia � no com cc tamb�m, se for, pega o n� de referencia do no de referencia
            ponto = (Te_Nodal_Point<double> * &) ptr_lo_points[0][no_ref];

            if((tipo_cc_no_ref = ponto->Boundary_Condition()))
            {
                if(tipo_cc_no_ref == 3 || tipo_cc_no_ref == 4 || tipo_cc_no_ref == 5) // � ponto com condi��o de contorno 3:float 4:c�clica 5: anti-c�clica
                {
                    no_ref = (long) ponto->Get_Variables(1);
                }
            }

            Transfer_Node_Links(nos, i, no_ref);
        }
    }

    for (j = 0; j < ptr_lo_points->Get_Total_Componentes(); j++)
    {
        nos[j].Consolida();  // *AP* ver comentarios na funcao
    }
}


/***************************************************************************/
/* Projeto: GROUND-3D                                                      */
/***************************************************************************/
/*                                                                         */
/* Nome do Arquivo:    skeleton.cpp                                        */
/* Nome da Funcao:     Seek_Node_Links                                     */
/* Versao:             1.0                                                 */
/* Compilador:         C++                                                   */
/* Programador:        Angelo Passaro                                      */
/* Ultima modificacao: 11/06/2004 (Nancy)                                   */
/* Modificacao:        Substitui��o de elemento3D por Nodal_Element *  */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
/* Descricao:                                                              */
/*                                                                         */
/*   Esta funcao monta os vetores de ligacoes de todos os nos.             */
/*   Utiliza a classe lev_math::NODE para isso.                                      */
/*                                                                         */
/***************************************************************************/
/* String que indica possiveis modificaoes futuras:                        */
/*                                                                         */
/*                                 *AP*                                    */
/*                                                                         */
/***************************************************************************/


void CLev_FEM_Poisson :: Seek_Node_Links(lev_math::NODE * nos)
{
    Mesh_Node_Links(nos);
    TransferAll_Node_Links(nos);
}


int CLev_FEM_Poisson::GetProperty(Finite_Element &elem, lev_math::Te_Matrix<double> &TensorK )
{
    double            property;//propriedade do material;
    Material        *mater;
    Point            p;//somente para passar para o material devolver o valor da propriedade

    //*NMA*:03/02/2006: talvez o Get_Property deva ficar no projeto, assim o explorador tamb�m poder� obter
    //a informa��o desta forma

    mater  = elem.Get_Material();
    property = mater->Property_Value(p);
    //property = mater->permittivity;

    //particularizado para 2d e isotr�pico
    TensorK(1,1) = property;
    TensorK(1,2) = 0.0;
    TensorK(2,1) = 0.0;
    TensorK(2,2) = property;

    return  0;
}


double CLev_FEM_Poisson::Get_Property(Finite_Element &elem)
{
    double property;
    Material *mater;
    mater    = elem.Get_Material();
    property = mater->permittivity;
    return  property;
}


//*NMA*: Transfere liga��es do n� no_cc para o n� no_ref
// Utilizada para transferir liga��es de n�s com condi��o de contorno periodica ou flutuante
void  CLev_FEM_Poisson :: Transfer_Node_Links(lev_math::NODE *nos, long no_cc, long no_ref)
{
  long j;
  long noLig;

  // A posi��o 0 de ligacoes cont�m o �ndice do proprio n� at� a chamada a  lev_math::NODE::Consolida()
  // Desta forma a transferencia de liga��es deve come�ar da posi��o 1
   for(j=1;j<=nos[no_cc].n_ligacoes;j++)
   {
    noLig = nos[no_cc].ligacoes[j];

    nos[no_ref].Inclui_Verifica_Ligacao(noLig);


    // inclui a liga��o a no_ref nos n�s ligados a no_cc,
    // a liga��o a no_cc continua existindo mas n�o ser� usada
    nos[noLig].Inclui_Verifica_Ligacao(no_ref);


   }// fecha for j

   nos[no_cc].n_ligacoes=0;


}


/***************************************************************************/
/* Projeto: GROUND-3D                                                      */
/***************************************************************************/
/*                                                                         */
/* Nome do Arquivo:    build.cpp                                           */
/* Nome da Funcao:     Build()                                             */
/* Versao:             1.0                                                 */
/* Compilador:         C++                                                 */
/* Programador:        Angelo Passaro                                      */
/* Ultima modificacao: 02/02/98                                            */
/* Modificacao:                                                            */
/*                                                                         */
/***************************************************************************/
/* Descricao:                                                              */
/*                                                                         */
/*   Calculates the  matrix for the  finite elements .                     */
/*                                                                         */
/***************************************************************************/
/* String que indica possiveis modificaoes futuras:                        */
/*                                                                         */
/*                       *AP*                                              */
/***************************************************************************/
void CLev_FEM_Poisson :: Build(void)
{
    cout << "\nExecutando CLev_FEM_Poisson::Build" << endl;
    cout.flush();

    Build_Local_Matrices_Blocks();

    cout<< "\nTerminou montagem da matriz global" << endl;
    cout.flush();
}


void CLev_FEM_Poisson::Build_Local_Matrices_Blocks(void)
{
    int i, j, matloc_dimension = 3, num_nodes_el=2;
    Nodal_Element *elem;
    Material *mater;
    double property;
    long order_elem = ptr_regions[0].ptr_lo_elements[0][0]->Order();

    // obtem a ordem do elemento.
    int ord_integ[21] = {order_elem,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

    lev_math::Te_Matrix<double> MatLoc(matloc_dimension,matloc_dimension);
    lev_math::Te_Vector<double> vsource(matloc_dimension), vsourcet(matloc_dimension);
    double source_value = 0.0;

    for(i=0; i<_region::Region::Total_Regions(); i++) {
        for(j=0; j<ptr_regions[i].ptr_lo_elements->Get_Total_Componentes(); j++) {
            elem = ptr_regions[i].ptr_lo_elements[0][j];

            num_nodes_el = elem->Get_Nodes();
            if(num_nodes_el != matloc_dimension)
            {
                MatLoc.ChangeDimensions(num_nodes_el,num_nodes_el);
                vsource.ChangeDimension(num_nodes_el);
                vsourcet.ChangeDimension(num_nodes_el);
                matloc_dimension = num_nodes_el;
            }

            IntegralGradNGradNdV(*elem,MatLoc,ord_integ[i]);
            mater  = elem->Get_Material();

            // Eletroima
            //property = 1/mater->permeability;

            // Cabo e Capacitor
            property = mater->permittivity;

            // Resistor
            // property = mater->conductivity;

            // Quantum problem
            /// property = 1.;

            MatLoc = property * MatLoc;

            Fill_Stiffness_Matrix(*elem, MatLoc) ;

            //tratamento do termo de fonte
            if(elem->Get_Source())
            {
                CSource * source = elem->Get_Source();
                source_value = source->Get_Value();
                //source_value = Get_Source_Value(i,j);//i:region j:indice do elemento
                // (mesmo no caso est�tico a fonte pode variar em fun��o da posi��o)
                IntegralNdV(*elem,vsource,ord_integ[i]);
                vsourcet = source_value*vsource;
                Fill_RHS_Vector(*elem,vsourcet);
            }//fecha if(elem->Get_Source())
            /* fim tratamento do termo de fonte */
        } // end of for (j< regioes[i].elementos_3D.....
    } // end of for (i<regioes :: Total_Regions)
}  // end of Build_Local_Matrices_Blocks


//==============================================================================
//     Fill_Stiffness_Matrix
//==============================================================================
/*   obs. Esta funcao devera ser modificada para a utilizacao de condicoes de
          contorno associadas diretamente aos pontos, e nao mais �s estruturas
          auxiliares, tais como bloco_cc_ciclica...

          Para tanto, a leitura dever� ser modificada para associar as condicoes
          de contorno aos pontos correspondentes, usando Set_Tipo e associando a
          cc ao vetor de variaveis de estado.
============================================================================= */
void CLev_FEM_Poisson :: Fill_Stiffness_Matrix(Finite_Element & el, lev_math::Te_Matrix<double> &matloc)
{
    long i,j,a,b;
    long *e;      // stores temporarily the element vertices indexes
    long *e_ref;  // stores temporarily the element vertices indexes with the reference node
    int *cond; // stores temporarily the type of the element vertices boundary condition
    double value;
    Te_Nodal_Point<double> *ponto_i, *ponto_j;
    int  cond1 = 0;

    e = new long [el.Get_Nodes()];
    if (!e) {
    	cout <<"\n \a insufficient memory in Fill_Stiffness_Matrix";
    	saida(0);
    }

    el.Get_Nodes(&e[0]);

    //-----------------------------------------------------------------------------------------------------------
    // para o tratamento de condi��es de contorno peri�dicas e flutuantes
    //-----------------------------------------------------------------------------------------------------------
    e_ref = new long [el.Get_Nodes()];
    if (!e_ref) {
    	cout <<"\n \a insufficient memory in Fill_Stiffness_Matrix";
    	saida(0);
    }
    cond = new int [el.Get_Nodes()];
    if (!cond) {
    	cout <<"\n \a insufficient memory in Fill_Stiffness_Matrix";
    	saida(0);
    }

    // obtem cc dos nos do elemento
    for (i = 0; i < el.Get_Nodes(); i++)
        e_ref[i] = GetNodeRef(e[i], cond[i]);

    for(i=0; i<el.Get_Nodes(); i++) {
        for(j=0; j<el.Get_Nodes(); j++)    {
            ponto_i = (Te_Nodal_Point<double> *) ptr_lo_points[0][ e_ref[i] ];
            ponto_j = (Te_Nodal_Point<double> *) ptr_lo_points[0][ e_ref[j] ];
            for(a=0; a<ponto_i->Get_N_Variables(); a++) {
                for(b=0; b<ponto_j->Get_N_Variables(); b++) {

                    value = matloc(i*ponto_i->Get_N_Variables()+a+1, j*ponto_j->Get_N_Variables()+b+1); //*NMA*: matloc is 1-based (Buzzi)

                    if(cond[i]==5 || cond[j]==5) {
                        if(cond[i] == cond[j])
                            cond1=0;
                        else
                            cond1 = 5;
                    } else
                        cond1=0;

                    if(cond1 == 5)
                        value = -value;

                    //*NMA*:24/01/2007: condi��o de contorno float: quando dois n�s com cc float tem o
                    //        o mesmo n� de referencia, o termo fora da diagonal tem que ser inserido duas
                    //        vezes na posi��o  A(no_ref,no_ref), onde e_ref[i] == e_ref[j] = no_ref :
                    //        - uma parcela referente ao termo A(e_ref[i],e_ref[j])
                    //        - a outra parcela referente ao termo A(e_ref[j],e_ref[i])
                    //        como e_ref[i] == e_ref[j], o valor ser� colocado na diagonal do no de referncia

                    if(i!=j) { //    se i!=j e e_ref[i]==e_ref[j], os dois n�s tem cc float com mesmo no_ref
                               //                                ou um no com cc float est� ligado ao no_ref
                               //    no caso das periodicas, os nos de referenci s�o sempre diferentes, pois a rela��o � de 1 para 1
                        if(e_ref[i]==e_ref[j])
                            value = 2*value;
                    }

                    //std::cout << "\n   " << e_ref[i]*ponto->Get_N_Variables()+a << " - " << e_ref[j]*ponto->Get_N_Variables()+b;
                    Gmatrix.addValue(ponto_i->DLNC[0]+a, ponto_j->DLNC[0]+b, value);
                }
            }
        }
    }

    if(e)
        delete [] e;
    if(e_ref)
        delete [] e_ref;
    if(cond)
        delete [] cond;
} // End of Fill_Stiffness_Matrix


// *****************************************************************************
// *  function : Impose_Boundary_Conditions                                     *
// *                                                                           *
// *  description : This function changes the global matrix putting 1 in the   *
// *                diagonal d corresponding to the boundary condition point   *
// *                and zero in all the nonzero elements of the d row and      *
// *                d column. The Load_Vector is also modified.                *
// *                                                                           *
// *  last version :   13/03/98                                                *
// *                                                                           *
// *                                                                           *
// *****************************************************************************
// *AP* versao para uma �nica variavel de estado
// *AP* Verificar vers�o para N vari�veis de estado em Build_magneto*.cpp
//void CLev_FEM_Poisson :: Impose_Boundary_Conditions(lev_math::Lo_Templ<long, long> &NO_CC)
void CLev_FEM_Poisson::Impose_Boundary_Conditions()
{
    double aij;
    long i, j, a;
    long NCC, NC, NN;

    rhs.Save("rhs_before.dat");

    Te_Nodal_Point<double> *ponto, *pontoJ;
    int tipo_cc;

    NC = ptr_lo_long_cc_dirichlet[0].Get_Total_Componentes();
    NN = ptr_lo_points->Get_Total_Componentes();

    // Imposicao das CC Dirichlet
    for(i=0; i<NC; i++) { // NC eh o numero de nos com CC Dirichlet
        NCC = ptr_lo_long_cc_dirichlet[0][i];

        ponto = (Te_Nodal_Point<double> *)ptr_lo_points[0][NCC];

        Gmatrix.setValue(ponto->DLNC[0], ponto->DLNC[0], 1.0);

        for(j=0; j<NN; j++) {
        	pontoJ = (Te_Nodal_Point<double> *)ptr_lo_points[0][j];
            if(j==NCC) {
                rhs[pontoJ->DLNC[0]] = 0.0;
				#define _OTHERSTATEVARS_ // RYT 15/02/2015 - ISTO ESTA CORRETO!!!!
                #ifdef _OTHERSTATEVARS_
                for(a=1; a<pontoJ->Get_N_Variables(); a++) {
                    aij = Gmatrix.getValue(pontoJ->DLNC[a], ponto->DLNC[0]);

                    rhs[pontoJ->DLNC[a]] = rhs[pontoJ->DLNC[a]] - aij*ponto->Get_Variables(1); // No caso de CC != de zero

                    if(aij) {
                        Gmatrix.setValue(pontoJ->DLNC[a], ponto->DLNC[0], 0.0);
                    }
                }
				#endif
            } else {
            	a = 0;
				#ifdef _OTHERSTATEVARS_
            	for(a=0; a<ponto->Get_N_Variables(); a++) {
				#endif
                    aij = Gmatrix.getValue(pontoJ->DLNC[a], ponto->DLNC[0]);

                    rhs[pontoJ->DLNC[a]] = rhs[pontoJ->DLNC[a]] - aij*ponto->Get_Variables(1);// No caso de CC != de zero

                    if(aij){
                        Gmatrix.setValue(pontoJ->DLNC[a], ponto->DLNC[0], 0.0);
                    }
				#ifdef _OTHERSTATEVARS_
                }
				#endif
            }
        }
    }

	#if 0
    // Imposicao das CC Neumann
    double neumannval = 0.0;
    int varstate = 2;
    for(i=0; i<NC; i++) { // NC eh o numero de nos com CC Dirichlet
        NCC = ptr_lo_long_cc_dirichlet[0][i];

        ponto = (Te_Nodal_Point<double> *)ptr_lo_points[0][NCC];

        Gmatrix.setValue(ponto->DLNC[varstate], ponto->DLNC[varstate], 1.0);

        for(j=0; j<NN; j++) {
        	pontoJ = (Te_Nodal_Point<double> *)ptr_lo_points[0][j];
            if(j==NCC) {
                rhs[pontoJ->DLNC[varstate]] = 0.0;
				#define _OTHERSTATEVARS_ // RYT 15/02/2015 - ISTO ESTA CORRETO!!!!
                #ifdef _OTHERSTATEVARS_
                for(a=0; a<pontoJ->Get_N_Variables(); a++) {
                	if(a==varstate)
                		continue;
                    aij = Gmatrix.getValue(pontoJ->DLNC[a], ponto->DLNC[varstate]);

                    rhs[pontoJ->DLNC[a]] = rhs[pontoJ->DLNC[a]] - aij*neumannval; // No caso de CC != de zero

                    if(aij) {
                        Gmatrix.setValue(pontoJ->DLNC[a], ponto->DLNC[varstate], 0.0);
                    }
                }
				#endif
            } else {
            	a = 0;
				#ifdef _OTHERSTATEVARS_
            	for(a=0; a<ponto->Get_N_Variables(); a++) {
				#endif
                    aij = Gmatrix.getValue(pontoJ->DLNC[a], ponto->DLNC[varstate]);

                    rhs[pontoJ->DLNC[a]] = rhs[pontoJ->DLNC[a]] - aij*neumannval;// No caso de CC != de zero

                    if(aij){
                        Gmatrix.setValue(pontoJ->DLNC[a], ponto->DLNC[varstate], 0.0);
                    }
				#ifdef _OTHERSTATEVARS_
                }
				#endif
            }
        }
    }
	#endif

    // Imposicao das CC periodicas
    // *NMA*: poderia colocar as cc dirichlet neste trecho tambem
    for(i=0;i<NN;i++) {
        ponto = (Te_Nodal_Point<double> *) ptr_lo_points[0][i];

        tipo_cc = ponto->Boundary_Condition();

        if(tipo_cc) {
            switch(tipo_cc) {
                case 3:
                case 4:
                case 5:
                    Gmatrix.setValue(ponto->DLNC[0], ponto->DLNC[0], 1.0);
                    rhs[ponto->DLNC[0]] = 0;
                    break;
            }
        }
    }
} // end of Impose_Boundary_Conditions


int CLev_FEM_Poisson :: Solve(void)
{
	int tmp1;
    int i;
    int codeErr = 0;

    A_NS.setValue(Gmatrix, lev_math::LINE, lev_math::UNSYMMETRIC);

	#ifndef _SOLVEBYSUPERLU_
    //lev_math::CNon_Sta_Ite_Method_CG < lev_math::Sparse_Matrix<double,long>, double,long > o_solve;
    lev_math::CNon_Sta_Ite_Method_BICGSTAB<lev_math::Sparse_Matrix<double, long>, double, long> o_solve;
    o_solve.set_precision(1e-10);
    const char* pre = "CInc_Fac_Preconditioner_ILU"; // "CInc_Fac_Preconditioner_NULL"; // "CInc_Fac_Preconditioner_IC"; // "CInc_Fac_Preconditioner_ILU";
    cout << "\nExecutando CLev_FEM_Poisson::Solve BICGSTAB" << endl;
    cout << "\n----------------------------------------------" << endl;
    cout << "\nFor Hermite Finite Element use SUPERLU Solver." << endl;
    cout << "\n----------------------------------------------" << endl;
    cout.flush();
    solution = o_solve.solve(A_NS, rhs, pre);

	#else

    cout << "\nExecutando CLev_FEM_Poisson::Solve SUPERLU" << endl;
    cout.flush();
	std::string prec("CInc_Fac_Preconditioner_ILU");

    lev_math::Lo_Templ<int, int> Aprim, Aposition;
	//lev_math::SuperLU<double,int> *solve_sparse = new lev_math::SuperLU<double,int>(lev_math::NO_TRANSPOSE);;
    lev_math::SuperLU<double, int> solve_sparse(lev_math::NO_TRANSPOSE);

	Aprim.Set_N_Max_Com_Abs(A_NS.prim.Get_Total_Componentes());
	for(i=0; i<A_NS.prim.Get_Total_Componentes(); i++) {
		tmp1 = A_NS.prim[i];
		Aprim.Inclui(tmp1);
	}

	Aposition.Set_N_Max_Com_Abs(A_NS.position.Get_Total_Componentes());
	for(i=0; i<A_NS.position.Get_Total_Componentes(); i++) {
		tmp1 = A_NS.position[i];
		Aposition.Inclui(tmp1);
	}

	// Conversao das estruturas  de matrizes para <double, int>
	lev_math::Sparse_Matrix<double, int> MA(&Aprim[0], Aprim.Get_Total_Componentes(),
			&Aposition[0], &A_NS.value[0], A_NS.position.Get_Total_Componentes(), A_NS.Orientation(), A_NS.Format());

	solution = solve_sparse.solve(MA, rhs, prec);

	#endif

    // Gravacao dos dados do sistema
	#if 1
    std::ofstream fout("solucao.dat");
    if(fout.fail())
        cout << "\nErro na abertura do arquivo de solucao.dat";
    else {
        double value;
        long num_points = solution.Get_Total_Componentes();
        fout << num_points << '\n';
        for(i=0; i<num_points; i++) {
            value = solution[i];
            fout << value <<  '\n';
        }
        fout.close();
    }
	#endif

    SetBoundaryConditions();
    UpdateVariablesInPoints();

    return codeErr;
}


//================================================================================================
// Atualiza valores da vari�vel de estado nos pontos a partir da solu��o obtida
//================================================================================================
void CLev_FEM_Poisson :: UpdateVariablesInPoints(void)
{
    long i, j;
    Te_Nodal_Point<double> *ponto;
    long no_ref;
    int  tipo_cc;

    for(i=0; i<ptr_lo_points->Get_Total_Componentes(); i++) {
    	no_ref = GetNodeRef(i, tipo_cc);
    	ponto = (Te_Nodal_Point<double> *)ptr_lo_points[0][no_ref];
    	for(j=0; j<ponto->Get_N_Variables(); j++)
    		ponto->Set_Variables(j+1, solution[ponto->DLNC[j]]);
    }
} // End of UpdateVariablesInPoints



//===============================================================================================
// Descricao: Atribui os valores de cc no vetor solution, ap�s a resolu��o do sistema
//              Implementada  para cc dirichlet, float e c�clicas
//===============================================================================================
void CLev_FEM_Poisson :: SetBoundaryConditions()
{
	long i;
	long NCC;
	long NC;
	long NN;
	long no_ref;
	int tipo_cc;
	Te_Nodal_Point<double> *ponto, *ponto_ref;

	NC = ptr_lo_long_cc_dirichlet[0].Get_Total_Componentes();
	NN = ptr_lo_points->Get_Total_Componentes();

	// Dirichlet BC
	for(i=0;i<NC;i++) {
		NCC = ptr_lo_long_cc_dirichlet[0][i];
		ponto = (Te_Nodal_Point<double> *) ptr_lo_points[0][NCC];
		solution[ponto->DLNC[0]] = ponto->Get_Variables(1);
	}

    for(i=0;i<ptr_lo_points->Get_Total_Componentes();i++)
    {
        ponto = (Te_Nodal_Point<double> *) ptr_lo_points[0][i];
        no_ref = GetNodeRef(i, tipo_cc);

        if(i != no_ref) {
        	ponto_ref = (Te_Nodal_Point<double> *) ptr_lo_points[0][no_ref];
        	solution[ponto->DLNC[0]] = solution[ponto_ref->DLNC[0]];
        }
    }
} // End of SetBoundaryConditions


//-----------------------------------------------------------------------------------------------
/** *NMA* 24/11/2005
//-----------------------------------------------------------------------------------------------
// void CLev_FEM_Poisson::Fill_RHS_Vector(Finite_Element & el, lev_math::Te_Vector<double> &vecloc)
// Descricao: Inclui dados fornecidos no vetor vecloc relativos ao
//                elemento el, no vetor RHS global
//-----------------------------------------------------------------------------------------------
*/
void CLev_FEM_Poisson::Fill_RHS_Vector(Finite_Element & el, lev_math::Te_Vector<double> &vecloc)
{
    long i, a;
    long *e;     // stores temporarily the element vertices indexes
    long *e_ref; // stores temporarily the element vertices indexes with the reference node
    int  *cond;  // stores temporarily the type of the element vertices boundary condition
    double value;
    Te_Nodal_Point<double> *p;

    e = new long [el.Get_Nodes()];
    if(!e) { cout <<"\n \a insufficient memory in Fill_RHS_Vector"; saida(0); }

    el.Get_Nodes(&e[0]);

    //-------------------------------------------------------------------
    // para o tratamento de condicoes de contorno periodicas e flutuantes
    //-------------------------------------------------------------------
    e_ref = new long [el.Get_Nodes()];
    if(!e_ref) { cout <<"\n \a insufficient memory in Fill_RHS_Vector"; saida(0); }
    cond = new int [el.Get_Nodes()];
    if(!cond) { cout <<"\n \a insufficient memory in Fill_RHS_Vector"; saida(0); }

    // Obtem cc dos nos do elemento
    for(i=0; i<el.Get_Nodes(); i++)
    	e_ref[i] = GetNodeRef(e[i], cond[i]);

    for(i=0; i<el.Get_Nodes(); i++) {
    	p = (Te_Nodal_Point<double> *)ptr_lo_points[0][e_ref[i]];
    	for(a=0; a<p->Get_N_Variables(); a++) {
    		value = vecloc(i*p->Get_N_Variables()+a+1); //*NMA*:vecloc is 1-based (Buzzi)
    		rhs[p->DLNC[a]] = rhs[p->DLNC[a]] + value;
    	}
    }

    if(e)
    	delete [] e;
    if(e_ref)
    	delete [] e_ref;
    if(cond)
    	delete [] cond;
} // End of void CLev_FEM_Poisson::Fill_RHS_Vector(Finite_Element & el, lev_math::Te_Vector<double> &vecloc)


/**
* Opera��o para salvar o sistema de equa��es.
* Parametros:
* std::string: identifica tipo de arquivo de gravacao (ex: matlab, superLU, etc)
* std::string: nome do arquivo com path
*/
void CLev_FEM_Poisson :: SaveSystemOfEquations(std::string destino, std::string nome_arquivo)
{
    //*NMA*: compara��o com if para poder usar string ao inv�s de enum
    bool continua = true;

    if( (continua == true) && (destino == "matlab") )
    {
        SaveSystemOfEquationsFormatMatlab(nome_arquivo);
        continua = false;
    }
}


void CLev_FEM_Poisson :: SaveSystemOfEquationsFormatMatlab(std::string nome_arquivo)
{
     long num_linhas;
     long i,j;
     std :: ofstream  fo;

     if(A_NS.Orientation() == lev_math::LINE)
     {
        std::cout << "\n Gravando Matriz: ";
         fo.open(nome_arquivo.c_str(),ios::out);

         if(!fo.fail())
         {
            num_linhas = A_NS.prim.Get_Total_Componentes();// corresponde ao numero de linha da matriz

            for(i=0;i<num_linhas-1;i++)
            {
                for(j=A_NS.prim[i];j<A_NS.prim[i+1];j++)
                {

                    fo << "A("<< i+1 << "," << A_NS.position[j]+1 << ")=" << A_NS.value[j] << ";" << std::endl;

                    if(i!=A_NS.position[j])
                    {
                        if(A_NS.Format() == lev_math::SYMMETRIC_SUPERIOR || A_NS.Format() == lev_math::SYMMETRIC_INFERIOR)
                        {
                            fo << "A("<< A_NS.position[j]+1 << "," << i+1 << ")=" << A_NS.value[j] << ";" << std::endl;
                        }
                    }
                }
            }
            //ultima linha
            for(j=A_NS.prim[i];j<A_NS.position.Get_Total_Componentes();j++)
            {
                fo << "A("<< i+1 << "," << A_NS.position[j]+1 << ")=" << A_NS.value[j] << ";" << std::endl;
            }


            //grava�ao do vetor de a��es (RHS)
            std::cout << "\n Gravando RHS: ";
            for(i=0;i<num_linhas;i++)
            {
                fo << "b("<< i+1 << ")=" << rhs[i] << ";" << std::endl;
            }

            fo << "b = b';" << std::endl;//transposta para virar vetor
            fo << "x = A"<< "\\" <<"b;" << std::endl;
            fo << "disp("<<"\'solucao\'"<<")" << std::endl;
            fo << "x ";

            fo.close();

         } else {
            std::cout << "\nErro na abertura do arquivo: " << nome_arquivo.c_str() << endl;
            std::cout.flush();
         }
     } else
        std::cout << "\n Grava��o somente de matrizes armazenadas por LINHA.";
}


/**
* Opera��o para gravar informa��es do elemento, a matriz local e a matriz global ap�s a inclus�o
* dos valores da matriz local, as informa��es s�o anexadas (append) ao final do arquivo
* Parametros:
* std::string        : texto a ser inclu�do no arquivo antes da grava��o das matrizes
* Nodal_Element &    :
* lev_math::Te_Matrix        &    : matriz (buzzi)
* char *            : nome do arquivo (com path)
*/
void CLev_FEM_Poisson :: SaveParcialMatrices(std::string texto, Nodal_Element &el,lev_math::Te_Matrix<double> &matrix, char* nome_arquivo)
{
    std :: ofstream  fo;
    fo.open(nome_arquivo,ios::app);
    if(fo.fail())
        std::cout << "CLev_FEM_Poisson :: SaveParcialInfo: Can�t open file " << nome_arquivo;
    else {
        fo << texto << std::endl;
        fo.close();
        el.Save(nome_arquivo,1);
        matrix.Save(nome_arquivo,1);
    }
}


long CLev_FEM_Poisson :: GetNodeRef(long pointIndex, int &cond)
{
    Te_Nodal_Point<double> *ponto;
    int  cond_no_ref;
    long e_ref, no_ref;

    ponto = (Te_Nodal_Point<double> * &) ptr_lo_points[0][pointIndex];
    cond  = ponto->Boundary_Condition();

    // Ponto com condicao de contorno 3:float 4:ciclica 5: anti-ciclica
    if(cond == 3 || cond == 4 || cond == 5) {
        // no caso destas cc valor_cc eh o noh de referencia
        no_ref = (long) ponto->Get_Variables(1);
        e_ref  = GetNodeRef(no_ref, cond_no_ref);
    } else
        e_ref  = pointIndex;

    return e_ref;
}

