// =========================================================================
//
//    File:  fin_elem.cpp
//
//   Description : Finite element classes
//
//   written by: Angelo Passaro
//
//   written at: september / 1997
//
//   Modified by:
//
//   Modified at:
//
//   Included Classes :
//              Finite_Element (Abstract)
//                Nodal_Elements (Abstract)
//                Simplex_Nodal_Element_1D
//                Simplex_Nodal_Element_2D
//                Simplex_Nodal_Element_3D ( to mantain compatibility with the old
//                      class architecture the Elemento_3D_Basico class will be derived
//                      from it)
//
//    Error Codes : from 23000 to 23999
//==========================================================================
// Class Finite_Element:
//    Data:
//         short int n_vertices: number of element vertices
//
//    Associations:
//        long int * nodes : At the present time (09/09/97), this is a vector containing the
//                                 index of the node points into the general node point
//                                 list;
//
//                                 In the future, this must be substituted for a list of
//                                 pointers to the node points (Pt_Malha ** nodes), to increase
//                                 the finite element calculation efficiency;
//
//        Material * material : association with the base class of material.
//
//==========================================================================
// Class Nodal_Element:
//    Derived from Finite_Element
//
//    Data:
//        static unsigned char order   : approximation order of the finite element
//                                                 (ATTENTION: It is a FLAG to define the
//                                                 behaviour of the Build_higher_order and
//                                                 Base_function methods.)
//
//==========================================================================
//Class Simplex_Nodal_Element_3D
//    Derived from Nodal_Element
//
//    Data:
//        static short int n_nodes    : number of nodes of the element for the
//                                                defined order;
//        double Volume               : derived data  used to increase the finite
//                                                element calculation efficiency
//
//==========================================================================


#include <limits>
#include <matrix.hpp>
#include <fin_elem.hpp>


lev_math::Lo_Templ<Point *,long> * Finite_Element::lo_Points = NULL;


Finite_Element::Finite_Element()
{
    if (lo_Points) {
        num_max_vizinhos = 0;
        num_vizinhos = 0;
        n_vertices = 0;
        nodes = NULL;
        material = NULL;
        source = NULL;
        vizinhos = NULL;
    } else {
        short int err = 23009;
        Message ( err, (char*)("Finite_Element constructor."));
        exit (0);
    }
}


Finite_Element::Finite_Element(short int nv)
{
    if (lo_Points) {
        num_max_vizinhos = 0;
        num_vizinhos = 0;
        nodes = NULL;
        material = NULL;
        source = NULL;
        vizinhos = NULL;
        n_vertices = nv;
    } else {
        short int err = 23009;
        Message ( err, (char*)("Finite_Element constructor."));
        exit(err);
    }
}


Finite_Element::~Finite_Element()
{
    n_vertices = 0;
}


short int Finite_Element::Set_Vertices( const long int *a)
{
    // The values in the vector a are not verified.
    short int err = 0;  // no errors
    if (n_vertices) {
        if (nodes)
            memcpy(&nodes[0],a,sizeof(long int) * n_vertices);
        else
            err = 23005;
    }
    return err;
}


//---------------------------------------------------------------------------
//               class Finite_Element::Set_Neighbor
//---------------------------------------------------------------------------
//
//  Descricao          : associa um vizinho a um elemento. Recebe uma mensagem
//                       que indica ao objeto qual o indice de um outro objeto
//                       da mesma classe, vizinho, e o inclue na lista de
//                       vizinhos. Na mensagem tambem vem uma indicacao se o
//                       objeto deve se incluido com ou sem verificacao
//                       num_max_vizinhos deve ser definido no construtor das
//                       classes derivadas
//
//  parametro          : indice do elemento vizinho
//  retorno            : status da inclusao de vizinho
//                           0 : inclusao normal;
//                         306 : numero de vizinhos supera o valor maximo permissivel.
//                               Isto significa que a malha e inconsistente.
//
//  versao             : 1.1
//  programador        : Angelo Passaro
//  ultima modificacao : 21/12/96
//
//---------------------------------------------------------------------------
unsigned int Finite_Element::Set_Neighbor(long int  indice)
{
    unsigned int status = 0;
    if (num_vizinhos < num_max_vizinhos)
        vizinhos[num_vizinhos++] = indice;
    else
        status = 306;
    return (status);
} // fim de Set_Neighbor


// =============================================================================
//                     Finite_Element::Message
// =============================================================================
void Finite_Element::Message (short int err, char *loc)
{
    char message[256];
    sprintf(message, "\nError %d: ", err);
    switch (err) {
        case 23000 :
            strcat(message, "function not implemented - ");
            break;
        case 23001 :
            strcat(message, "second order finite elements not implemented - ");
            break;
        case 23002 :
            strcat(message, "third order finite elements not implemented - ");
            break;
        case 23003 :
            strcat(message, "higher order finite elements not implemented - ");
            break;
        case 23004:
            strcat(message, "Insufficient memory - ");
            break;
        case 23005 :
            strcat(message, "nodes are not dimensioned - ");
            break;
        case 23006 :
            strcat(message, " Inconsistent order - ");
            break;
        case 23007 :
            strcat(message, " quadrature order not implemented - ");
            break;
        case 23008 :
            strcat(message, " base function number must be between 1 and 4 for this element order - ");
            break;
        case 23009 :
            strcat(message, " Point List is not Associated to the class - ");
            break;
        default :
            strcat(message, "Unknown error. Copy the Project Files for analysis... - ");
            break;
    } // end of switch (err)
    strcat(message, "at ");
    strcat(message, loc);
    std::cout << message;
} // end of Finite_Element::Message


Nodal_Element::Nodal_Element() : Finite_Element()
{
    size = 0;
    num_vizinhos = 0;
    vizinhos = NULL;
    order = 1;
    n_nodes = 0;
    jacHom =0;
    jacDet = 0.0;
}


Nodal_Element::Nodal_Element(int o) : Finite_Element()
{
    size = 0;
    num_vizinhos = 0;
    vizinhos = NULL;
    order = o;
    n_nodes = 0;
    jacHom =0;
    jacDet = 0.0;
}


void Nodal_Element::MatJacobHom(lev_math::Te_Matrix<double> &Jhom)
{
    if(!jacHom)
        MatJacobHom();
    Jhom = JHom;
} // end of Nodal_Element::MatJacobHom


void Nodal_Element::MatJacobHom(lev_math::Te_Matrix<double> &Jhom, double &jac)
{
    if(!jacHom)
        MatJacobHom(Jhom);
    Jhom = JHom;
    jac = jacHom;
}


double Nodal_Element::MatJacobHomDet(void)
{
    if(!jacHom)
        MatJacobHom();
    return jacHom;
}


void Nodal_Element::Save(char* nome_arquivo,int op)//*NMA*:23/01/2007: op=0-> "w" (write) op=1->"a" (append) ex: Save("elemento.dat",1)
{
    int    i;
    Point *aux_point = NULL;
    std::ofstream fo;

    if(!op)
        fo.open(nome_arquivo,std::ios::out);
    else
        fo.open(nome_arquivo,std::ios::app);

    if(fo.fail())
        std::cout << "Nodal_Element::Save(): Cannot open file " << nome_arquivo;
    else {
        fo << "--- ELEMENT -----------------------------------\n";
        fo << "dimension: "        << dimension << "  ";
        fo << "n_vertices: "    << n_vertices << "  ";
        fo << "n_nodes: "        << Get_N_Nodes() << "  ";
        fo << "\njacHom (determinante da matriz jacobiana): " << jacHom << "\n";
        fo << "JHom (matriz jacobiana):\n";
        fo << "             " << JHom(1,1) << "   " << JHom(1,2) << "\n";
        fo << "             " << JHom(2,1) << "   " << JHom(2,2) << "\n";
        fo << "n_nodes: " << n_nodes << "\n";
        fo << "order: " << order << "\n";
        fo << "size: " << size << "\n";
        fo << "Dados do material:\n";
        fo << "      " << "angle: " << material->angle << "\n";
        fo << "      " << "code: " << material->code << "\n";
        fo << "      " << "conductivity: " << material->conductivity << "\n";
        fo << "      " << "ident: " << material->ident << "\n";
        fo << "      " << "name: " << material->name << "\n";
        fo << "      " << "numberof: " << material->numberof << "\n";
        fo << "      " << "permeability: " << material->permeability << "\n";
        fo << "      " << "permitivity: " << material->permittivity << "\n";
        fo << "\nnodes (0-based): ";
        for(i=0;i<Get_N_Nodes();i++)
            fo << nodes[i] << " ";
        fo << "\n\n\n\n";
        fo.close();

        for(i=0;i<Get_N_Nodes();i++) {
            lo_Points->Get_Componente(nodes[i],aux_point);
            aux_point->Save(nome_arquivo,1);//append
        }
    }
}


//short int Simplex_Nodal_Element_1D::n_nodes=0;


short int  Simplex_Nodal_Element_1D::Build_Higher_Order(void)
{
    // this function must resize nodes to store the new points index
    // USE method Resize_Nodes to do so!
    short int err = 0; // no errors
    if (order == 1)
        return err;
    else {
        err = Resize_Nodes();
        if (!err)
            Message ( 23000,(char*) ("Simplex_Nodal_Element_1D::Build_Higher_Order"));
    }
    return err;
}


Simplex_Nodal_Element_1D::Simplex_Nodal_Element_1D() : Nodal_Element()
{
    material = NULL;
    nodes = NULL;
    dimension = 1;
    n_vertices = 2;
    num_max_vizinhos = 2;
    num_vizinhos = 0; // *AP* 15/10/2007
}


Simplex_Nodal_Element_1D::Simplex_Nodal_Element_1D(int o) : Nodal_Element(o)
{
    material = NULL;
    nodes = NULL;
    dimension = 1;
    n_vertices = 2;
    num_max_vizinhos = 2;
    num_vizinhos = 0;  //*AP* 15/10/2007
}


//==============================================================================
// fun��o  Simplex_Nodal_Element_1D::  Transformation_Coefficients(double *a, double *b)
// *AP* implementada em 10/10/2007
// verificar :
//      testes de implementa��o
//      se estas fun��es nao devem ser privadas ou protegidas para todas as
//              classes de elementos finitos, j� que o n�mero de par�metros
//              � vari�vel de classe para classe
//      dath-Touzot para as expressoes
//==============================================================================
void   Simplex_Nodal_Element_1D::Transformation_Coefficients(double *a, double *b)
{
    // transformation to the homogeneous coordintate system
    //
    //      zeta = (-z1 + z) / L = (a + bz)/L
    //
    //      a = -z1        e      b = 1.
    //
    Point *p[2];  // triangle vertices
    Point *aux_point = NULL;
    lo_Points->Get_Componente(nodes[0],aux_point);
    p[0] = aux_point;
      a[0] = -(p[0]->Get_X());
    b[0] = 1;
} // end of Transformation_Coefficients


void Simplex_Nodal_Element_1D::MatJacobHom()
{
    if(!jacDet) {
        JHom.ChangeDimensions(1,1); // *CM* 12/10/2007 incluindo a funcao de dimensionamento
        Jac.ChangeDimensions(1,1);
        jac.ChangeDimensions(1,1);
        Set_Size();
    }
    Point *p[2];

    for(int i = 0; i<n_vertices; i++)
        lo_Points->Get_Componente(nodes[i], p[i]);

    Jac.SetValue(1, 1, (p[1]->Get_X() - p[0]->Get_X())/2.0);
    JacDet = Jac(1,1);
    jac.SetValue(1, 1, 1.0/Jac(1,1));
    jacDet = jac(1,1);

    //=====================================================================
    // For 1D: size = line_length/2
    //=====================================================================
    JHom = jac;
    jacHom = jacDet;
} // End of Simplex_Nodal_Element_1D::MatJacobHom


void Simplex_Nodal_Element_1D::CartesianToHom(Point &point, Point &phom)
{
    double zeta1;
    double a[4], b[4];
    Transformation_Coefficients(&a[0], &b[0]);
    // -----------------------------------------------------------------------------
    // homogeneous coordinates calculation
    // -----------------------------------------------------------------------------
    zeta1  = a[0] + b[0] * point.Get_X();

    /* *CM* 31/10/2007

    //zeta1 /= size;    // CÓDIGO ANTERIOR

     Foi encontrado um erro no calculo da variavel "zeta1" no metodo "void Simplex_Nodal_Element_1D ::
     CartesianToHom(Point &point, Point &phom)"A variavel "size", na verdade,
     era o determinante da matriz jacobiana quando deveria ser o comprimento
     do elemento (para elementos 1D). A solucao foi substituir a variavel "size" pelo dobro
     da variavel "jacHom", que eh calculado pelo metodo "Simplex_Nodal_Element_1D ::
     MatJacobHomDet ()".
     */

    if(!jacHom)
        MatJacobHomDet();
    zeta1 /= 2.0 * jacHom;
    phom.Set_Coordinates(&zeta1);
} // end of Simplex_Nodal_Element_1D::CartesianToHom


//==============================================================================
// fun��o  Simplex_Nodal_Element_1D:: HomToCartesian(Point &phom, Point &point)
// *CM* implementada em 12/10/2007
//==============================================================================
void Simplex_Nodal_Element_1D::HomToCartesian(Point &phom, Point &point)
{
    int i;
    double coord[1];
    Point *p[2];  // tetrahedron vertices
    Point *aux_point = NULL;
    for ( i = 0; i < n_vertices; i++) {
        lo_Points->Get_Componente(nodes[i], aux_point);
        p[i] = aux_point;
    }
    double X1 = p[0]->Get_X(), X2 = p[1]->Get_X();//, X3 = p[2]->Get_X(), X4 = p[3]->Get_X();
    coord[0] = X2 + (X1-X2)*phom.Get_X();// + (X2-X4)*phom.Get_Y() + (X3-X4)*phom.Get_Z();
    point.Set_Coordinates(&coord[0]);
} // end of Simplex_Nodal_Element_1D::HomToCartesian


Simplex_Nodal_Element_1D & Simplex_Nodal_Element_1D::operator = (Simplex_Nodal_Element_1D & el)
{
    memcpy (&nodes[0], &el.nodes[0], sizeof (long) * el.n_nodes) ; // the nodes content is copied
    material = el.material;      // the material pointer is copied
    size = el.size;
    //   the other data are static
    return *this;
} // end of Simplex_Nodal_Element_1D::operator =


short int Simplex_Nodal_Element_1D::Gauss_Points(int quad_ord, int &npgauss, Gauss_Pt * pt)
{
    short int ret = 0;
    int k;
    double x[5], peso[5];

    quad_ord = integrationOrder;

    switch( quad_ord ) {
        case 1:
            npgauss = 1; x[0] = 0.; peso[0] = 2.;
            break;
        case 2:
            npgauss = 2;
            x[0] =  0.577350269189626; peso[0] = 1.;
            x[1] = -0.577350269189626; peso[1] = 1.;
            break;
        case 3:
            npgauss = 3;
            x[0]=  0.774596669241483; peso[0] = 0.555555555555556;
            x[1]= -0.774596669241483; peso[1] = 0.555555555555556;
            x[2]=  0.;                peso[2] = 0.888888888888889;
            break;
        case 4:
            npgauss = 4;
            x[0]=  0.861136311594953; peso[0] = 0.347854845137454;
            x[1]= -0.861136311594953; peso[1] = 0.347854845137454;
            x[2]=  0.339981043584856; peso[2] = 0.652145154862546;
            x[3]= -0.339981043584856; peso[3] = 0.652145154862546;
            break;
        default:
            ret = 23007;
            Message (ret, (char*)( "Gauss Points"));
            break;
    } // end of switch(quad_ord)

    if (!ret) {
        for (k = 0; k < npgauss; k++) {
            pt[k].Set_Variables(1,peso[k]);
            pt[k].Set_Coordinates(&x[k]);
        }
    }
    return ret;
} // end of  Simplex_Nodal_Element_1D::Gauss_Points


/**Obs: n�o testado . *AP* Esta implementa��o n�o d� conta de retas no plano ou no espa�o*/
int Simplex_Nodal_Element_1D::Get_Centroid(Point &p_centroid)
{
    int retorno = 0;
    Point  *pt_at[2];
    double coord[1];
    double x[2];

    if (nodes) {
        for (int i = 0; i < n_vertices; i++)
            pt_at[i] = lo_Points[0][nodes[i]];

        x[0] = pt_at[0]->Get_X();
        x[1] = pt_at[1]->Get_X();

        coord[0] = (x[0]+x[1])/n_vertices;

        p_centroid.Dimension(1);//por consistencia
        p_centroid.Set_Coordinates(&coord[0]);
    } else
        retorno = 1;
    return retorno;
} // End Simplex_Nodal_Element_1D::Get_Centroid(Point &p_centroid)


bool Simplex_Nodal_Element_1D::Is_Inside(Point & point)
{
    Point        phom;
    bool        retorno = false;
    CartesianToHom(point, phom);
    if(phom.X() >= -1 && phom.X() <= 1.0)   //*AP* corrigido o limite inferior em 11/10/2007
        retorno = true;
    return retorno;
} // Elemento_1D ::Is_Inside


Lagrange_Element_1D::Lagrange_Element_1D():Simplex_Nodal_Element_1D ()
{
    short int err;
    integrationOrder = 2;
    /*
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes) {
        err = 23004; Message(err,(char*)("Lagrange_Element_1D default constructor"));
        exit (err);
    }
    vizinhos = new long [2];
    if(!vizinhos) {
        Message(33006,(char*)("Lagrange_Element_1D default constructor"));
        exit (33006);
    }
    */
}


Lagrange_Element_1D::Lagrange_Element_1D(int o):Simplex_Nodal_Element_1D (o)
{
	std::cout <<".";
    short int err;
	order = o;
    integrationOrder = 2;
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes) {
        err = 23004; Message(err,(char*)("Lagrange_Element_1D default constructor"));
        exit (err);
    }
    vizinhos = new long [2];
    if(!vizinhos) {
        Message(33006,(char*)("Lagrange_Element_3D default constructor"));
        exit (33006);
    }
}


void Lagrange_Element_1D::Base_Function(Point *point, lev_math::Te_Vector<double> & vetorN)
{
    double zeta;
    point->Coordinate(1,zeta);   // homogeneous coordinates

    switch(order) {
        case  1 : // first order
            vetorN(1) = (1.0-zeta) * 0.5;
            vetorN(2) = (1.0+zeta) * 0.5;
            break;
        case  2 : // second order
            vetorN(1) = (zeta - 1.0) * zeta * 0.5;
            vetorN(2) = (zeta + 1.0) * zeta * 0.5;
            vetorN(3) = (zeta + 1.0) * (1.0 - zeta);         // central point
            break;
        case  3 :
            Message (23002 , (char*)("Lagrange_Element_1D::Base_Function"));
            break;
        default :
            Message (23003 , (char*)("Lagrange_Element_1D::Base_Function"));
            break;
    } // end of switch(order)
} // end of  Simplex_Nodal_Element_1D::Base_Function


double Lagrange_Element_1D::Base_Function(Point *point, short int qual)
{
    double value = 0.0;
    double zeta;
    point->Coordinate(1, zeta);
    switch (order) {
        case 1:
            switch (qual) {
                case 1:
                    value = (1.-zeta) * 0.5;
                    break;
                case 2:
                    value = (1.+zeta) * 0.5;
                    break;
                default:
                    Message (23008, (char*)("Base_Function"));
                    break;
            } // end of switch (qual) first order element
            break;
        case 2:
            switch (qual) {
                case 1:
                    value = (zeta - 1.) * zeta * 0.5;
                    break;
                case 2:
                    value = (zeta + 1.) * zeta * 0.5;
                    break;
                case 3:
                    value = (zeta + 1.) * (1.0 - zeta);
                    break;
                default:
                    Message (23008, (char*)("Base_Function"));
                    break;
            } // end of switch (qual) second order element
        default:
            Message (23003,(char*)("Base_Function"));
            break;
    } // end of switch order
    return value;
} // end of  Lagrange_Element_1D::Base_Function


void Lagrange_Element_1D::GradN(Point *point, lev_math::Te_Matrix<double> &DN)
{
    double zeta;
    point->Coordinate(1,zeta);
    switch(order) {
        case  1 : // first order
            DN(1,1) = -0.5;
            DN(1,2) =  0.5;
            break;
        case  2 : // second order
            DN(1,1) = -0.5 + zeta;
            DN(1,2) =  0.5 + zeta;
            DN(1,3) = -2.0 * zeta;
            break;
        case  3 :
            Message (23002 , (char*)("Lagrange_Element_1D::GradN"));
            break;
        default :
            Message (23003 ,(char*)( "Lagrange_Element_1D::GradN"));
            break;
    }
    DN = jac * DN; // Convert to Real Coordinates
} // end of Lagrange_Element_1D:: GradN


void Lagrange_Element_1D::DivN(Point *point, lev_math::Te_Matrix<double> &divN)
{
    Message(23000, (char*)("Lagrange_Element_1D::  DivN"));
}


void Lagrange_Element_1D::RotN(Point *point, lev_math::Te_Matrix<double> &rotN)
{
    Message(23000,(char*) ( "Lagrange_Element_1D::  RotN"));
}


Hermite_Element_1D::Hermite_Element_1D() : Simplex_Nodal_Element_1D()
{
    short int err;
    integrationOrder = 4;
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes)
    {
        err = 23004; Message(err,(char*)("Hermite_Element_1D default constructor")); exit (err);
    }
    vizinhos = new long[2];
    if(!vizinhos)
    {
        Message(33006,(char*)("Hermite_Element_1D default constructor")); exit (33006);
    }
}


Hermite_Element_1D::Hermite_Element_1D(int o)
{
    short int err;
	order = o;
    integrationOrder = 4;
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes)
    {
        err = 23004; Message(err,(char*)("Hermite_Element_1D default constructor")); exit (err);
    }
    vizinhos = new long[2];
    if(!vizinhos)
    {
        Message(33006,(char*)("Hermite_Element_1D default constructor")); exit (33006);
    }
}


void  Hermite_Element_1D::Base_Function(Point *point, lev_math::Te_Vector<double> & vetorN)
{
    double Xi, Xi2, Xi3;
    double f = 1.0 / Get_Material()->effectiveMass_Electron;
    point->Coordinate(1, Xi);   // homogeneous coordinates
    Xi2 = Xi*Xi;
    Xi3 = Xi2*Xi;

    switch(order) {
        case 3 : // Cubic Hermite
            vetorN.ChangeDimension(4);
            vetorN(1) = 0.25 * (2.0 - 3.0*Xi + Xi3);
            vetorN(2) = f * 0.25 * (1.0 - Xi - Xi2 + Xi3) * JacDet;
            vetorN(3) = 0.25 * (2.0 + 3.0*Xi - Xi3);
            vetorN(4) = f * 0.25 * (-1.0 - Xi + Xi2 + Xi3) * JacDet;
            break;
        case 5 : // 5th Order
            vetorN.ChangeDimension(6);
            vetorN(1) = 0.0625 * (8.0 - 15.0*Xi + 10.0*Xi3 - 3.0*Xi2*Xi3);
            vetorN(2) = 0.0625 * (5.0 - 7.0*Xi - 6.0*Xi2 + 10.0*Xi3 + Xi2*Xi2 - 3.0*Xi2*Xi3) * JacDet;
            vetorN(3) = 0.0625 * (1.0 - Xi - 2.0*Xi2 + 2.0*Xi3 + Xi2*Xi2 - Xi2*Xi3) * JacDet * JacDet;
            vetorN(4) = 0.0625 * (8.0 + 15.0*Xi - 10.0*Xi3 + 3.0*Xi2*Xi3);
            vetorN(5) = 0.0625 * (-5.0 - 7.0*Xi + 6.0*Xi2 + 10.0*Xi3 - Xi2*Xi2 - 3.0*Xi2*Xi3) * JacDet;
            vetorN(6) = 0.0625 * (1.0 + Xi - 2.0*Xi2 - 2.0*Xi3 + Xi2*Xi2 + Xi2*Xi3) * JacDet * JacDet;
            break;
        default :
            Message (23003 , (char*)("Hermite_Element_1D::Base_Function"));
            break;
    }
} // end of  Hermite_Element_1D::Base_Function


double Hermite_Element_1D::Base_Function(Point *point, short int qual)
{
    std::cout << "\n\nENTROU EM double Hermite_Element_1D::Base_Function(Point *point, short int qual)\n\n";
    std::cout.flush();

    double value = 0.0;
    double zeta;
    point->Coordinate(1, zeta);
    switch (order) {
        default:
            Message (23003,(char*)("Base_Function"));
            break;
    } // end of switch order
    return value;
} // End of  Hermite_Element_1D::Base_Function


void Hermite_Element_1D::GradN(Point *point, lev_math::Te_Matrix<double> &DN)
{
    double Xi, Xi2, Xi3;
    double f = 1.0 / Get_Material()->effectiveMass_Electron;
    point->Coordinate(1,Xi);
    Xi2 = Xi*Xi;
    Xi3 = Xi2*Xi;

    switch(order) {
        case 3: // 3rd order
            DN.ChangeDimensions(1,4);
            DN(1,1) = 0.25 * (-3.0 + 3.0*Xi2);
            DN(1,2) = f * 0.25 * (-1.0 - 2.0*Xi + 3.0*Xi2) * JacDet;
            DN(1,3) = 0.25 * ( 3.0 - 3.0*Xi2);
            DN(1,4) = f * 0.25 * (-1.0 + 2.0*Xi + 3.0*Xi2) * JacDet;
            break;
        case 5: // 5th order
            DN.ChangeDimensions(1,6);
            DN(1,1) = 0.0625 * (-15.0 + 30.0*Xi2 - 15.0*Xi2*Xi2);
            DN(1,2) = 0.0625 * (-7.0 - 12.0*Xi + 30.0*Xi2 + 4.0*Xi3 - 15.0*Xi2*Xi2) * JacDet;
            DN(1,3) = 0.0625 * (-1.0 - 4.0*Xi + 6.0*Xi2 + 4.0*Xi3 - 5.0*Xi2*Xi2) * JacDet * JacDet;
            DN(1,4) = 0.0625 * (15.0 - 30.0*Xi2 + 15.0*Xi2*Xi2);
            DN(1,5) = 0.0625 * (-7.0 + 12.0*Xi + 30.0*Xi2 - 4.0*Xi3 - 15.0*Xi2*Xi2) * JacDet;
            DN(1,6) = 0.0625 * (1.0 - 4.0*Xi - 6.0*Xi2 + 4.0*Xi3 + 5.0*Xi2*Xi2) * JacDet * JacDet;
            break;
        default:
            Message (23003 ,(char*)( "Hermite_Element_1D::GradN"));
            break;
    }
    DN = jac * DN; // Convert to Real Coordinates
} // End of Hermite_Element_1D::GradN


void Hermite_Element_1D::DivN(Point *point, lev_math::Te_Matrix<double> &divN)
{
    Message(23000, (char*)("Hermite_Element_1D:: DivN"));
}


void Hermite_Element_1D::RotN(Point *point, lev_math::Te_Matrix<double> &rotN)
{
    Message(23000,(char*) ( "Hermite_Element_1D:: RotN"));
}


//===============================================================================
//===============================================================================
//===============================================================================
// ****** Modificacao do programa para inclusao de elementos 2D - AMJ ************
//
//short int Simplex_Nodal_Element_2D::n_nodes=0 ;

// =============================================================================

short int  Simplex_Nodal_Element_2D::Build_Higher_Order(void)
{ // this function must resize nodes to store the new points index
  // USE method Resize_Nodes to do so!
   short int err = 0; // no errors
   if (order ==1)
       return err;
   else {
       err = Resize_Nodes();
       if (!err)
           Message ( 23000, (char*)("Simplex_Nodal_Element_2D - Build_Higher_Order"));
   };
   return err;
} // end of Simplex_Nodal_Element_2D::Build_Higher_Order


Simplex_Nodal_Element_2D::Simplex_Nodal_Element_2D () : Nodal_Element()
{
    dimension            = 2;
    n_vertices        = 3;
    num_max_vizinhos    = 3;
    num_vizinhos      = 0; // *AP* 15/10/2007
    material            = NULL;
    nodes                = NULL;
} // end of Simplex_Nodal_Element_2D default constructor


Simplex_Nodal_Element_2D::Simplex_Nodal_Element_2D (int o) : Nodal_Element(o)
{
    dimension            = 2;
    n_vertices        = 3;
    num_max_vizinhos    = 3;
    num_vizinhos      = 0; // *AP* 15/10/2007
    material            = NULL;
    nodes                = NULL;
} // end of Simplex_Nodal_Element_2D constructor


void Simplex_Nodal_Element_2D::Transformation_Coefficients(double *a, double *b, double *c)
{
    double x_aux1, x_aux2, x_aux3;
    double y_aux1, y_aux2, y_aux3;
    int i;
    Point *p[3];  // triangle vertices
    Point *aux_point = NULL;
    for ( i = 0; i < n_vertices; i++) {
        lo_Points->Get_Componente(nodes[i],aux_point);
        p[i] = aux_point;
    }
    x_aux1 = p[0]->Get_X(); // P1 coordinate
    y_aux1 = p[0]->Get_Y();
    x_aux2 = p[1]->Get_X(); // P2 coordinate
    y_aux2 = p[1]->Get_Y();
    x_aux3 = p[2]->Get_X(); // P3 coordinate
    y_aux3 = p[2]->Get_Y();

    // -----------------------------------------------------------------------------
    // coefficient calculation for the P1 coordinate
    // -----------------------------------------------------------------------------
    a[1] = x_aux2 * y_aux3 - x_aux3 * y_aux2;
    b[1] = y_aux2 - y_aux3;
    c[1] = x_aux3 - x_aux2;

    // -----------------------------------------------------------------------------
    // coefficient calculation for the P2 coordinate
    // -----------------------------------------------------------------------------
    a[2] = x_aux3 * y_aux1 - x_aux1 * y_aux3;
    b[2] = y_aux3 - y_aux1;
    c[2] = x_aux1 - x_aux3;

    // ----------------------------------------------------------------------------
    // coefficient calculation for the P3 coordinate
    // ----------------------------------------------------------------------------
    a[3] = x_aux1 * y_aux2 - x_aux2 * y_aux1;
    b[3] = y_aux1 - y_aux2;
    c[3] = x_aux2 + x_aux1;
} // end of Transformation_Coefficients


void Simplex_Nodal_Element_2D::MatJacobHom(void)
{
    //#define __DHATT__
    #ifdef __DHATT__
    double b[3], c[3];
    double x_aux1, x_aux2, x_aux3;
    double y_aux1, y_aux2, y_aux3;

    if(!jacHom) {
        JHom.ChangeDimensions(2,2);

        int i;
        Point *p[3];  // triangle vertices
        Point *aux_point=NULL;

        for ( i = 0; i < n_vertices; i++) {
            lo_Points->Get_Componente(nodes[i], aux_point);
            p[i] = aux_point;
        }

        x_aux1 = p[0]->Get_X();
        y_aux1 = p[0]->Get_Y();

        x_aux2 = p[1]->Get_X();
        y_aux2 = p[1]->Get_Y();

        x_aux3 = p[2]->Get_X();
        y_aux3 = p[2]->Get_Y();

        // -----------------------------------------------------------------------------
        // coefficient calculation for the P1 coordinate
        // -----------------------------------------------------------------------------
        b[1] = y_aux2 - y_aux3;

        c[1] = x_aux3 - x_aux2;

        // -----------------------------------------------------------------------------
        // coefficient calculation for the P2 coordinate
        // -----------------------------------------------------------------------------
        b[2] = y_aux3 - y_aux1;

        c[2] = x_aux1 - x_aux3;

        JHom.SetValue(1,1,b[1]); JHom.SetValue(1,2,b[2]);
        JHom.SetValue(2,1,c[1]); JHom.SetValue(2,2,c[2]);

        size = jacHom = JHom(1,1)*JHom(2,2) - JHom(2,1)*JHom(1,2) ; // size = 2*Area
    }
    #else
    #define _LEV_
    Point *p[3];  // Triangle vertices

    if(!jacDet) {
        Jac.ChangeDimensions(2,2);
        jac.ChangeDimensions(2,2);

        for(int i = 0; i<n_vertices; i++)
            lo_Points->Get_Componente(nodes[i], p[i]);

        #ifdef _LEV_
        Jac.SetValue(1, 1, (p[0]->Get_X() - p[2]->Get_X()));    Jac.SetValue(1, 2, (p[0]->Get_Y() - p[2]->Get_Y()));
        Jac.SetValue(2, 1, (p[1]->Get_X() - p[2]->Get_X()));    Jac.SetValue(2, 2, (p[1]->Get_Y() - p[2]->Get_Y()));
        #else
        Jac.SetValue(1, 1, (p[1]->Get_X() - p[0]->Get_X()));    Jac.SetValue(1, 2, (p[1]->Get_Y() - p[0]->Get_Y()));
        Jac.SetValue(2, 1, (p[2]->Get_X() - p[0]->Get_X()));    Jac.SetValue(2, 2, (p[2]->Get_Y() - p[0]->Get_Y()));
        #endif

        JacDet = (Jac(1,1)*Jac(2,2) - Jac(2,1)*Jac(1,2));

        jac.SetValue(1,1,  Jac(2,2));    jac.SetValue(1,2, -Jac(1,2));
        jac.SetValue(2,1, -Jac(2,1));    jac.SetValue(2,2,  Jac(1,1));

        jacDet = (jac(1,1)*jac(2,2) - jac(2,1)*jac(1,2));

        jac = (1.0/JacDet) * jac;

        //=====================================================================
        // For 2D: size = 2 * triangle area
        //=====================================================================
        JHom = jac;
        size = jacHom = jacDet;
    }
    #endif
} // End of Simplex_Nodal_Element_2D::MatJacobHom


void   Simplex_Nodal_Element_2D::CartesianToHom(Point &point, Point &phom)
{
    double zeta[2];
    double &zeta1 = zeta[0], &zeta2 = zeta[1];
    double a[4], b[4], c[4];
    Transformation_Coefficients(&a[0], &b[0], &c[0]);

    // -----------------------------------------------------------------------------
    // homogeneous coordinates calculation
    // -----------------------------------------------------------------------------
    zeta1  = a[1] + b[1] * point.Get_X() + c[1] * point.Get_Y();
    zeta1 /= size;
    zeta2  = a[2] + b[2] * point.Get_X() + c[2] * point.Get_Y();
    zeta2 /= size;

    phom.Set_Coordinates(&zeta[0]);
} // end of Simplex_Nodal_Element_2D::CartesianToHom


void Simplex_Nodal_Element_2D::HomToCartesian(Point &phom, Point &point)
{
    int i;
    double coord[2];
    Point *p[3];  // triangle vertices
    Point *aux_point = NULL;
    for ( i = 0; i < n_vertices; i++) {
        lo_Points->Get_Componente(nodes[i], aux_point);
        p[i] = aux_point;
    }

    double X1 = p[0]->Get_X(), X2 = p[1]->Get_X(), X3 = p[2]->Get_X();
    double Y1 = p[0]->Get_Y(), Y2 = p[1]->Get_Y(), Y3 = p[2]->Get_Y();

    coord[0] = X3 + (X1-X3)*phom.Get_X() + (X2-X3)*phom.Get_Y();
    coord[1] = Y3 + (Y1-Y3)*phom.Get_X() + (Y2-Y3)*phom.Get_Y();

    point.Set_Coordinates(&coord[0]);
} // end of Simplex_Nodal_Element_2D::HomToCartesian


Simplex_Nodal_Element_2D & Simplex_Nodal_Element_2D::operator = (Simplex_Nodal_Element_2D & el)
{
    memcpy (&nodes[0], &el.nodes[0], sizeof (long) * el.n_nodes) ; // the nodes content is copied
    material = el.material;      // the material pointer is copied
    size = el.size;
    return *this;
} // end of Simplex_Nodal_Element_2D::operator =


short int Simplex_Nodal_Element_2D::Gauss_Points (int quad_ord, int &npgauss, Gauss_Pt * pt)
{
    short int ret = 0;
    const double cte1 = 0.333333333333333333; // 1/3
    const double cte2 = 0.166666666666666667; // 1/6
    const double cte4 = 0.260416666666666667; // 25/96
    double peso;
    double coord[2], a, b, c, d;
    double &X = coord[0],
           &Y = coord[1];
    int k;

    quad_ord = integrationOrder;

    switch(quad_ord) {
        case 1:
            npgauss = 1;
            peso = 0.5;
            X = Y = cte1;
            pt[0].Set_Coordinates(&coord[0]);
            pt[0].Set_Variables(1,peso);
            break;
        case 2:
            npgauss = 3;
            peso = cte2;
            X = Y = 0.5;
            pt[0].Set_Coordinates(&coord[0]);
            pt[0].Set_Variables(1,peso);
            X = 0.;
            Y = 0.5;
            pt[1].Set_Coordinates(&coord[0]);
            pt[1].Set_Variables(1,peso);
            X = 0.5;
            Y = 0.;
            pt[2].Set_Coordinates(&coord[0]);
            pt[2].Set_Variables(1,peso);
            break;
        case 3:
            npgauss = 4;
            peso = -0.28125;
            pt[0].Set_Variables(1,peso);
            peso = cte4;
            for(k=1; k<npgauss; k++)
                pt[k].Set_Variables(1,peso);
            X = Y = cte1;
            pt[0].Set_Coordinates(&coord[0]);
            X = Y = 0.2;
            pt[1].Set_Coordinates(&coord[0]);
            X = 0.6; Y = 0.2;
            pt[2].Set_Coordinates(&coord[0]);
            X = 0.2; Y = 0.6;
            pt[3].Set_Coordinates(&coord[0]);
            break;
        case 4:
            npgauss = 6;
            a = 0.445948490915965;
            b = 0.091576213509771;
            for(k=0; k<3; k++)
                pt[k].Set_Variables(1,0.111690794839005);
            for(k=3; k<6; k++)
                pt[k].Set_Variables(1,0.054975871827661);
            X = Y = a;
            pt[0].Set_Coordinates(&coord[0]);
            X = 1.0-2.0*a; Y = a;
            pt[1].Set_Coordinates(&coord[0]);
            X = a; Y = 1.0-2.0*a;
            pt[2].Set_Coordinates(&coord[0]);
            X = b; Y = b;
            pt[3].Set_Coordinates(&coord[0]);
            X = 1.0-2.0*b; Y = b;
            pt[4].Set_Coordinates(&coord[0]);
            X = b; Y = 1.0-2.0*b;
            pt[5].Set_Coordinates(&coord[0]);
            break;
        case 5:
            npgauss = 7;
            a = 0.470142064105115; //(6.0+sqrt(15))/21
            b = 0.101286507323456; // 4/7 - a
            pt[0].Set_Variables(1, 0.1125); // 9/80
            for(k=1; k<4; k++)
                pt[k].Set_Variables(1, 0.0661970763942530); // A = (155+sqrt(15))/2400
            for(k=4; k<7; k++)
                pt[k].Set_Variables(1, 0.0629695902724135); // 31/240 - A
            X = Y = cte1;
            pt[0].Set_Coordinates(&coord[0]);
            X = a; Y = a;
            pt[1].Set_Coordinates(&coord[0]);
            X = 1.0-2.0*a; Y = a;
            pt[2].Set_Coordinates(&coord[0]);
            X = a; Y = 1.0-2.0*a;
            pt[3].Set_Coordinates(&coord[0]);
            X = b; Y = b;
            pt[4].Set_Coordinates(&coord[0]);
            X = 1.0-2.0*b; Y = b;
            pt[5].Set_Coordinates(&coord[0]);
            X = b; Y = 1.0-2.0*b;
            pt[6].Set_Coordinates(&coord[0]);
            break;
        case 6:
            npgauss = 12;
            a = 0.063089014491502;
            b = 0.249286745170910;
            c = 0.310352451033785;
            d = 0.053145049844816;
            for(k=0; k<3; k++)
                pt[k].Set_Variables(1, 0.025422453185103);
            for(k=3; k<6; k++)
                pt[k].Set_Variables(1, 0.058393137863189);
            for(k=6; k<12; k++)
                pt[k].Set_Variables(1, 0.041425537809187);
            X = a; Y = a;
            pt[0].Set_Coordinates(&coord[0]);
            X = 1.0-2.0*a; Y = a;
            pt[1].Set_Coordinates(&coord[0]);
            X = a; Y = 1.0-2.0*a;
            pt[2].Set_Coordinates(&coord[0]);
            X = b; Y = b;
            pt[3].Set_Coordinates(&coord[0]);
            X = 1.0-2.0*b; Y = b;
            pt[4].Set_Coordinates(&coord[0]);
            X = b; Y = 1.0-2.0*b;
            pt[5].Set_Coordinates(&coord[0]);
            X = c; Y = d;
            pt[6].Set_Coordinates(&coord[0]);
            X = d; Y = c;
            pt[7].Set_Coordinates(&coord[0]);
            X = 1.0-(c+d); Y = c;
            pt[8].Set_Coordinates(&coord[0]);
            X = 1.0-(c+d); Y = d;
            pt[9].Set_Coordinates(&coord[0]);
            X = c; Y = 1.0-(c+d);
            pt[10].Set_Coordinates(&coord[0]);
            X = d; Y = 1.0-(c+d);
            pt[11].Set_Coordinates(&coord[0]);

            break;
        default:
            ret = 23007;
            Message (ret, (char*)("Gauss Points"));
            break;
    } // end of switch(quad_ord)
    return ret;
} // end of  Simplex_Nodal_Element_2D::Gauss_Points


void Simplex_Nodal_Element_2D::Set_Size(double vol)
{
    if (vol) {
        size = vol;
        return;
    }

    // ===========================================================================
    // trecho usado para o calculo de size a partir das coordenadas dos vertices do
    // elemento
    // ===========================================================================

    // produtos do determinante denominado na documentacao detA1 ++++++++++++++

    Point  *pt_at[4];
    double detA;
    double x[3], y[3] ;

    if (nodes) {
        for (int i = 0; i < n_vertices; i++)  pt_at[i] = lo_Points[0][nodes[i]];

        x[0] = pt_at[0]->Get_X();
        x[1] = pt_at[1]->Get_X();
        x[2] = pt_at[2]->Get_X();
        y[0] = pt_at[0]->Get_Y();
        y[1] = pt_at[1]->Get_Y();
        y[2] = pt_at[2]->Get_Y();


        detA  = (x[1] - x[0]) * y[2] + (x[0] - x[2]) * y[1] + (x[2] - x[1]) * y[0];

        size = (detA >0)? detA: -detA;  // *AP* consultar caderno 4, pag.17 //manter o menos? e o fator 2 ??
    } else
        size = 0;
}


// =============================================================================
// =============================================================================
// ****** Fim das modificacoes para 2D *************
//===============================================================================


//-----------------------------------------------------------------------
//                Simplex_Nodal_Element_2D ::Is_Inside
//-----------------------------------------------------------------------
//
//  Descricao          : Verifica se o ponto est� dentro do elemento
//
//  retorno               : true: o ponto est� dentro do elemento
//                         false: o ponto n�o est� dentro do elemento
//
//  versao             : 1.0
//  programador        : Nancy Mieko Abe
//  ultima modificacao : 05/06/2001
//
//-----------------------------------------------------------------------
bool Simplex_Nodal_Element_2D::Is_Inside(Point & point)
{
    Point        phom;
    double        coef_angular, b;
    bool        retorno = false;

    //equa��o da reta (aresta do elemento no sistema de coordenadas homog�neo)
    coef_angular = -1; //reta que passa pelos ponto (1,0) e (0,1)
    b = 1; // b = y1 - coef_angular * x1

    CartesianToHom(point, phom);

    if(phom.X() >= 0 && phom.X() <= 1.0)
    {
        if( phom.Y() >= 0 && phom.Y() <= (coef_angular*phom.X()+b) )
        {
            retorno = true;
        }
    }

    return retorno;

}// Elemento_2D ::Is_Inside


//-----------------------------------------------------------------------
/** Simplex_Nodal_Element_xD ::Get_Centroid(Point &p_centroid)
//-----------------------------------------------------------------------
//  Descricao          : Calcula e retorna baricentro do elemento
//
//  retorno               : 0: sucesso
//                         1: erro
//
//  versao             : 1.0
//  programador        : Nancy Mieko Abe
//  ultima modificacao : 22/11/2005
//
*///-----------------------------------------------------------------------
int Simplex_Nodal_Element_2D::Get_Centroid(Point &p_centroid)
{
    int retorno = 0;
    Point  *pt_at[4];
    double coord[2];
    double x[3], y[3] ;

    if (nodes)
    {
        for (int i = 0; i < n_vertices; i++)  pt_at[i] = lo_Points[0][nodes[i]];

        x[0] = pt_at[0]->Get_X();
        x[1] = pt_at[1]->Get_X();
        x[2] = pt_at[2]->Get_X();

        coord[0] = (x[0]+x[1]+x[2])/n_vertices;

        y[0] = pt_at[0]->Get_Y();
        y[1] = pt_at[1]->Get_Y();
        y[2] = pt_at[2]->Get_Y();

        coord[1] = (y[0]+y[1]+y[2])/n_vertices;

        p_centroid.Dimension(2);//por consistencia
        p_centroid.Set_Coordinates(&coord[0]);
    }
    else retorno = 1;

    return retorno;

} // End of int Simplex_Nodal_Element_2D::Get_Centroid(Point &p_centroid)


// ===================================================
/**  funcoes double Get_Biggest_Edge()
      obtencao da maior aresta de um elemento */
// ===================================================
double Simplex_Nodal_Element_2D::Get_Biggest_Edge()
{
    int i;
    long int a[2];;
    Lagrange_Element_1D aresta[3];
    double maior_comp=-1, comp;

    a[0] = nodes[0]; a[1] = nodes[2];
    aresta[0].Set_Nodes(nodes);
    aresta[1].Set_Nodes(nodes+1);
    aresta[2].Set_Nodes(a);

    for (i=0; i < 3; i++)
    {
        comp = aresta[i].Get_Size();
        (comp > maior_comp)? maior_comp = comp: maior_comp;
    }
    return (maior_comp);
}


Lagrange_Element_2D ::Lagrange_Element_2D () : Simplex_Nodal_Element_2D ()
{
    integrationOrder = 2;
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes)
    {
        Message(23004,(char*)("Lagrange_Element_2D default constructor")); exit (23004);
    }
    vizinhos = new long [3];
    if(!vizinhos)
    {
        Message(33006,(char*)("Lagrange_Element_2D default constructor")); exit (33006);
       }
}


Lagrange_Element_2D ::Lagrange_Element_2D (int o):Simplex_Nodal_Element_2D (o)
{
	order = o;
    integrationOrder = 2;
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes)
    {
        Message(23004,(char*)("Lagrange_Element_2D default constructor")); exit (23004);
    }
    vizinhos = new long [3];
    if(!vizinhos)
    {
        Message(33006,(char*)("Lagrange_Element_2D default constructor")); exit (33006);
    }
}


void Lagrange_Element_2D::Base_Function(Point  *point, lev_math::Te_Vector<double> & vetorN)
{
    short int err;
    double zeta1 = point->Get_X(),       // homogeneous coordinates
           zeta2 = point->Get_Y(),
           zeta3 = 1.0 - zeta1 - zeta2;
    switch(order)
    {
        case 1: // first order
            #ifdef _LEV_
            vetorN(1) = zeta1;
            vetorN(2) = zeta2;
            vetorN(3) = zeta3;
            #else
            vetorN(1) = zeta3;
            vetorN(2) = zeta1;
            vetorN(3) = zeta2;
            #endif
            break;
        case 2: // second order
            vetorN(1) = zeta1 * ( -1. + 2. * zeta1);
            vetorN(2) = zeta2 * ( -1. + 2. * zeta2);
            vetorN(3) = zeta3 * ( -1. + 2. * zeta3);
            vetorN(4) =  zeta1 * zeta2 * 4.;
            vetorN(5) = zeta2 * zeta3 * 4.;
            vetorN(6) = zeta1 * zeta3 * 4.;
            break;
        case 3:
            err = 23002;
            Message (err ,(char*)( "Simplex_Nodal_Element_2D::Base_Function")); exit (err);
            break;
        default :
            err = 23003;
            Message (err ,(char*)( "Simplex_Nodal_Element_2D::Base_Function")); exit (err);
            break;
    } // end of switch(order)
} // End of Simplex_Nodal_Element_2D::Base_Function


double  Lagrange_Element_2D:: Base_Function(Point *point, short int qual)
{
  short int err;
    double zeta1 = point->Get_X(),       // homogeneous coordinates
           zeta2 = point->Get_Y(),
           zeta3 = 1. - zeta1 - zeta2;
   double value;
   switch (order)
   {
   case   1:
      switch (qual)
      {
         case    1:   value = zeta1; break;
         case    2:   value = zeta2; break;
         case    3:   value = zeta3; break;
         default  :
          err = 23008;
             Message (err,(char*)( "Base_Function")); exit (err);
          break;

      } // end of switch (qual) first order element
      break;

   case   2:     // second order
      switch (qual)
      {
           case   1: value = zeta1 * ( -1. + 2. * zeta1); break;
        case   2: value = zeta2 * ( -1. + 2. * zeta2); break;
        case   3: value = zeta3 * ( -1. + 2. * zeta3); break;
          case   4: value = zeta1 * zeta2 * 4.; break;
        case   5: value = zeta2 * zeta3 * 4.; break;
        case   6: value = zeta1 * zeta3 * 4.; break;
        default  :
          err = 23008;
             Message (err, (char*)("Base_Function")); exit (err);
          break;
      }  // end of switch (qual)
        break;
   default :
     err = 23003;
       Message (err, (char*)("Base_Function")); exit (err);
     break;
   } // end of switch order
   return value;
} // end of  Simplex_Nodal_Element_2D::Base_Function


void Lagrange_Element_2D::GradN(Point *point, lev_math::Te_Matrix<double> &DN)
{
    short int err;
    double zeta1 = point->Get_X(),       // homogeneous coordinates
           zeta2 = point->Get_Y(),
           zeta3 = 1. - zeta1 - zeta2;
    switch(order) {
        case  1 : // first order
            #ifdef _LEV_
            DN(1,1) = 1.0;
            DN(1,2) = 0.0;
            DN(1,3) = -1.0; //DN(1,3)= dN3/dzeta1
            DN(2,1) = 0.0;
            DN(2,2) = 1.0;
            DN(2,3) = -1.0;
            #else
            DN(1,1) = -1.0;
            DN(1,2) =  1.0;
            DN(1,3) =  0.0;
            DN(2,1) = -1.0;
            DN(2,2) =  0.0;
            DN(2,3) =  1.0;
            #endif
            break;

        case  2 : // second order
            DN(1,1) = -1. + 4.*zeta1;
            DN(1,2) = 0.;
            DN(1,3) = 1 - 4*zeta3;
            DN(1,4) = 4*zeta2;
            DN(1,5) = -4*zeta2;
            DN(1,6) = 4*(zeta3 - zeta1);

            DN(2,1) =  0.;
            DN(2,2) =  -1 + 4*zeta2;
            DN(2,3) =  1 - 4*zeta3;
            DN(2,4) =  4*zeta1;
            DN(2,5) =  4*(zeta3 - zeta2);
            DN(2,6) = -4*zeta1;
            break;
        case  3 :
            err = 23002;
            Message (err , (char*)("Simplex_Nodal_Element_2D::GradN")); exit (err);
            break;
        default :
            err = 23003;
            Message (err ,(char*)( "Simplex_Nodal_Element_2D::GradN")); exit (err);
            break;
    } // end of switch(order)
    DN = jac*DN; // Convert to Real Coordinates
} // End of Simplex_Nodal_Element_2D:: GradN


void Lagrange_Element_2D::DivN(Point *point, lev_math::Te_Matrix<double> &divN)
{
    Message(23000,(char*)( "Lagrange_Element_2D::  DivN"));
}


void Lagrange_Element_2D::RotN(Point *point, lev_math::Te_Matrix<double> &rotN)
{
    Message(23000,(char*)("Lagrange_Element_2D::  RotN"));
}

//==============================================================================

//==============================================================================
// Bell's triangle
//==============================================================================
Hermite_Element_2D::Hermite_Element_2D() : Simplex_Nodal_Element_2D()
{
    integrationOrder = 6;
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes)
    {
        Message(23004,(char*)("Hermite_Element_2D default constructor")); exit (23004);
    }
    vizinhos = new long [3];
    if(!vizinhos)
    {
        Message(33006,(char*)("Hermite_Element_2D default constructor")); exit (33006);
       }
}


Hermite_Element_2D::Hermite_Element_2D(int o)
{
	order = o;
    integrationOrder = 6;
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes)
    {
        Message(23004,(char*)("Hermite_Element_2D default constructor")); exit (23004);
    }
    vizinhos = new long [3];
    if(!vizinhos)
    {
        Message(33006,(char*)("Hermite_Element_2D default constructor")); exit (33006);
       }
}


void Hermite_Element_2D::Base_Function(Point *point, lev_math::Te_Vector<double> &vetorN)
{
    lev_math::Te_Matrix<double> T, N, R;
    short int err;
    #ifdef _SCHRODINGER_BENDANIELDUKE_BC_
    double f = 1.0 / Get_Material()->effectiveMass_Electron;
    #else
    double f = 1.0;
    #endif
    double Xi, Xi2, Xi3, Eta, Eta2, Eta3, Lambda, Lambda2, Lambda3;
    double Xi4, Xi5, Xi6, Eta4, Eta5, Eta6;
    Xi = point->Get_X();
    Eta = point->Get_Y();
    Lambda = 1.0 - Xi - Eta;
    Lambda2 = Lambda*Lambda;
    Lambda3 = Lambda2*Lambda;
    Xi2 = Xi*Xi;
    Xi3 = Xi2*Xi;
    Xi4 = Xi3*Xi;
    Xi5 = Xi4*Xi;
    Xi6 = Xi5*Xi;
    Eta2 = Eta*Eta;
    Eta3 = Eta2*Eta;
    Eta4 = Eta3*Eta;
    Eta5 = Eta4*Eta;
    Eta6 = Eta5*Eta;

    switch(order) {
        case 5: // fifth order
            N.ChangeDimensions(1,18);
            #ifdef _LEV_
            /*
            N(1,1)  = Xi2 * (10.0*Xi-15.0*Xi2+6.0*Xi3+15.0*Eta2*Lambda);
            N(1,2)  = 0.5*Xi2 * (-8.0*Xi+14.0*Xi2-6.0*Xi3-15.0*Eta2*Lambda);
            N(1,3)  = 0.5*Xi2*Eta * (6.0-4.0*Xi-3.0*Eta-3.0*Eta2+3.0*Xi*Eta);
            N(1,4)  = 0.25*Xi2 * (2.0*Xi*(1.0-2.0*Xi+Xi2)+5.0*Eta2*Lambda);
            N(1,5)  = 0.5*Xi2*Eta * (-2.0+2.0*Xi+Eta+Eta2-Xi*Eta);
            N(1,6)  = 0.25*Xi2*Eta2*Lambda + 0.5*Xi3*Eta2;
            N(1,7)  = Eta2 * (10.0*Eta-15.0*Eta2+6.0*Eta3+15.0*Xi2*Lambda);
            N(1,8)  = 0.5*Xi*Eta2 * (6.0-3.0*Xi-4.0*Eta-3.0*Xi2+3.0*Xi*Eta);
            N(1,9)  = 0.5*Eta2 * (-8.0*Eta+14.0*Eta2-6.0*Eta3-15.0*Xi2*Lambda);
            N(1,10) = 0.25*Xi2*Eta2*Lambda + 0.5*Xi2*Eta3;
            N(1,11) = 0.5*Xi*Eta2 * (-2.0+Xi+2.0*Eta+Xi2-Xi*Eta);
            N(1,12) = 0.25*Eta2 * (2.0*Eta*(1.0-2.0*Eta+Eta2)+5.0*Xi2*Lambda);
            N(1,13) = Lambda2 * (10.0*Lambda-15.0*Lambda2+6.0*Lambda3+30.0*Xi*Eta*(Xi+Eta));
            N(1,14) = Xi*Lambda2 * (3.0-2.0*Lambda-3.0*Xi2+6.0*Xi*Eta);
            N(1,15) = Eta*Lambda2 * (3.0-2.0*Lambda-3.0*Eta2+6.0*Xi*Eta);
            N(1,16) = 0.5*Xi2*Lambda2 * (1.0-Xi+2.0*Eta);
            N(1,17) = Xi*Eta*Lambda2;
            N(1,18) = 0.5*Eta2*Lambda2 * (1.0+2.0*Xi-Eta);
            */
            N(1,1)  = 15.0 * Eta2 * Xi2 - 15.0 * Eta3 * Xi2 + 10.0 * Xi3 - 15.0 * Eta2 * Xi3 - 15.0 * Xi4 + 6.0 * Xi5;
            N(1,2)  = 0.5 * (-15.0 * Eta2 * Xi2 + 15.0 * Eta3 * Xi2 - 8.0 * Xi3 + 15.0 * Eta2 * Xi3 + 14.0 * Xi4 - 6.0 * Xi5);
            N(1,3)  = 0.5 * (6.0 * Eta * Xi2 - 3.0 * Eta2 * Xi2 - 3.0 * Eta3 * Xi2 - 4.0 * Eta * Xi3 + 3.0 * Eta2 * Xi3);
            N(1,4)  = 0.25 * (5.0 * Eta2 * Xi2 - 5.0 * Eta3 * Xi2 + 2.0 * Xi3 - 5.0 * Eta2 * Xi3 - 4.0 * Xi4 + 2.0 * Xi5);
            N(1,5)  = 0.5 * (-2.0 * Eta * Xi2 + Eta2 * Xi2 + Eta3 * Xi2 + 2.0 * Eta * Xi3 - Eta2 * Xi3);
            N(1,6)  = 0.25 * (Eta2 * Xi2 - Eta3 * Xi2 + Eta2 * Xi3);
            N(1,7)  = 10.0 * Eta3 - 15.0 * Eta4 + 6.0 * Eta5 + 15.0 * Eta2 * Xi2 - 15.0 * Eta3 * Xi2 - 15.0 * Eta2 * Xi3;
            N(1,8)  = 0.5 * (6.0 * Eta2 * Xi - 4.0 * Eta3 * Xi - 3.0 * Eta2 * Xi2 + 3.0 * Eta3 * Xi2 - 3.0 * Eta2 * Xi3);
            N(1,9)  = 0.5 * (-8.0 * Eta3 + 14.0 * Eta4 - 6.0 * Eta5 - 15.0 * Eta2 * Xi2 + 15.0 * Eta3 * Xi2 + 15.0 * Eta2 * Xi3);
            N(1,10) = 0.25 * (Eta2 * Xi2 + Eta3 * Xi2 - Eta2 * Xi3);
            N(1,11) = 0.5 * (-2.0 * Eta2 * Xi + 2.0 * Eta3 * Xi + Eta2 * Xi2 - Eta3 * Xi2 + Eta2 * Xi3);
            N(1,12) = 0.25 * (2.0 * Eta3 - 4.0 * Eta4 + 2.0 * Eta5 + 5.0 * Eta2 * Xi2 - 5.0 * Eta3 * Xi2 - 5.0 * Eta2 * Xi3);
            N(1,13) = 1.0 - 10.0 * Eta3 + 15.0 * Eta4 - 6.0 * Eta5 - 30.0 * Eta2 * Xi2 + 30.0 * Eta3 * Xi2 - 10.0 * Xi3 + 30.0 * Eta2 * Xi3 + 15.0 * Xi4 - 6.0 * Xi5;
            N(1,14) = Xi - 3.0 * Eta2 * Xi + 2.0 * Eta3 * Xi - 6.0 * Eta2 * Xi2 + 6.0 * Eta3 * Xi2 - 6.0 * Xi3 + 9.0 * Eta2 * Xi3 + 8.0 * Xi4 - 3.0 * Xi5;
            N(1,15) = Eta - 6.0 * Eta3 + 8.0 * Eta4 - 3.0 * Eta5 - 3.0 * Eta * Xi2 - 6.0 * Eta2 * Xi2 + 9.0 * Eta3 * Xi2 + 2.0 * Eta * Xi3 + 6.0 * Eta2 * Xi3;
            N(1,16) = 0.5 * (Xi2 - 3.0 * Eta2 * Xi2 + 2.0 * Eta3 * Xi2 - 3.0 * Xi3 + 3.0 * Eta2 * Xi3 + 3.0 * Xi4 - Xi5);
            N(1,17) = Eta * Xi - 2.0 * Eta2 * Xi + Eta3 * Xi - 2.0 * Eta * Xi2 + 2.0 * Eta2 * Xi2 + Eta * Xi3;
            N(1,18) = 0.5 * (Eta2 - 3.0 * Eta3 + 3.0 * Eta4 - Eta5 - 3.0 * Eta2 * Xi2 + 3.0 * Eta3 * Xi2 + 2.0 * Eta2 * Xi3);
            #else
            /*
            N(1,1)  = Lambda2 * (10.0*Lambda-15.0*Lambda2+6.0*Lambda3+30.0*Xi*Eta*(Xi+Eta));
            N(1,2)  = Xi*Lambda2 * (3.0-2.0*Lambda-3.0*Xi2+6.0*Xi*Eta);
            N(1,3)  = Eta*Lambda2 * (3.0-2.0*Lambda-3.0*Eta2+6.0*Xi*Eta);
            N(1,4)  = 0.5*Xi2*Lambda2 * (1.0-Xi+2.0*Eta);
            N(1,5)  = Xi*Eta*Lambda2;
            N(1,6)  = 0.5*Eta2*Lambda2 * (1.0+2.0*Xi-Eta);
            N(1,7)  = Xi2 * (10.0*Xi-15.0*Xi2+6.0*Xi3+15.0*Eta2*Lambda);
            N(1,8)  = 0.5*Xi2 * (-8.0*Xi+14.0*Xi2-6.0*Xi3-15.0*Eta2*Lambda);
            N(1,9)  = 0.5*Xi2*Eta * (6.0-4.0*Xi-3.0*Eta-3.0*Eta2+3.0*Xi*Eta);
            N(1,10) = 0.25*Xi2 * (2.0*Xi*(1.0-2.0*Xi+Xi2)+5.0*Eta2*Lambda);
            N(1,11) = 0.5*Xi2*Eta * (-2.0+2.0*Xi+Eta+Eta2-Xi*Eta);
            N(1,12) = 0.25*Xi2*Eta2*Lambda + 0.5*Xi3*Eta2;
            N(1,13) = Eta2 * (10.0*Eta-15.0*Eta2+6.0*Eta3+15.0*Xi2*Lambda);
            N(1,14) = 0.5*Xi*Eta2 * (6.0-3.0*Xi-4.0*Eta-3.0*Xi2+3.0*Xi*Eta);
            N(1,15) = 0.5*Eta2 * (-8.0*Eta+14.0*Eta2-6.0*Eta3-15.0*Xi2*Lambda);
            N(1,16) = 0.25*Xi2*Eta2*Lambda + 0.5*Xi2*Eta3;
            N(1,17) = 0.5*Xi*Eta2 * (-2.0+Xi+2.0*Eta+Xi2-Xi*Eta);
            N(1,18) = 0.25*Eta2 * (2.0*Eta*(1.0-2.0*Eta+Eta2)+5.0*Xi2*Lambda);
            */
            N(1,1)  = 1.0 - 10.0 * Eta3 + 15.0 * Eta4 - 6.0 * Eta5 - 30.0 * Eta2 * Xi2 + 30.0 * Eta3 * Xi2 - 10.0 * Xi3 + 30.0 * Eta2 * Xi3 + 15.0 * Xi4 - 6.0 * Xi5;
            N(1,2)  = Xi - 3.0 * Eta2 * Xi + 2.0 * Eta3 * Xi - 6.0 * Eta2 * Xi2 + 6.0 * Eta3 * Xi2 - 6.0 * Xi3 + 9.0 * Eta2 * Xi3 + 8.0 * Xi4 - 3.0 * Xi5;
            N(1,3)  = Eta - 6.0 * Eta3 + 8.0 * Eta4 - 3.0 * Eta5 - 3.0 * Eta * Xi2 - 6.0 * Eta2 * Xi2 + 9.0 * Eta3 * Xi2 + 2.0 * Eta * Xi3 + 6.0 * Eta2 * Xi3;
            N(1,4)  = 0.5 * (Xi2 - 3.0 * Eta2 * Xi2 + 2.0 * Eta3 * Xi2 - 3.0 * Xi3 + 3.0 * Eta2 * Xi3 + 3.0 * Xi4 - Xi5);
            N(1,5)  = Eta * Xi - 2.0 * Eta2 * Xi + Eta3 * Xi - 2.0 * Eta * Xi2 + 2.0 * Eta2 * Xi2 + Eta * Xi3;
            N(1,6)  = 0.5 * (Eta2 - 3.0 * Eta3 + 3.0 * Eta4 - Eta5 - 3.0 * Eta2 * Xi2 + 3.0 * Eta3 * Xi2 + 2.0 * Eta2 * Xi3);
            N(1,7)  = 15.0 * Eta2 * Xi2 - 15.0 * Eta3 * Xi2 + 10.0 * Xi3 - 15.0 * Eta2 * Xi3 - 15.0 * Xi4 + 6.0 * Xi5;
            N(1,8)  = 0.5 * (-15.0 * Eta2 * Xi2 + 15.0 * Eta3 * Xi2 - 8.0 * Xi3 + 15.0 * Eta2 * Xi3 + 14.0 * Xi4 - 6.0 * Xi5);
            N(1,9)  = 0.5 * (6.0 * Eta * Xi2 - 3.0 * Eta2 * Xi2 - 3.0 * Eta3 * Xi2 - 4.0 * Eta * Xi3 + 3.0 * Eta2 * Xi3);
            N(1,10) = 0.25 * (5.0 * Eta2 * Xi2 - 5.0 * Eta3 * Xi2 + 2.0 * Xi3 - 5.0 * Eta2 * Xi3 - 4.0 * Xi4 + 2.0 * Xi5);
            N(1,11) = 0.5 * (-2.0 * Eta * Xi2 + Eta2 * Xi2 + Eta3 * Xi2 + 2.0 * Eta * Xi3 - Eta2 * Xi3);
            N(1,12) = 0.25 * (Eta2 * Xi2 - Eta3 * Xi2 + Eta2 * Xi3);
            N(1,13) = 10.0 * Eta3 - 15.0 * Eta4 + 6.0 * Eta5 + 15.0 * Eta2 * Xi2 - 15.0 * Eta3 * Xi2 - 15.0 * Eta2 * Xi3;
            N(1,14) = 0.5 * (6.0 * Eta2 * Xi - 4.0 * Eta3 * Xi - 3.0 * Eta2 * Xi2 + 3.0 * Eta3 * Xi2 - 3.0 * Eta2 * Xi3);
            N(1,15) = 0.5 * (-8.0 * Eta3 + 14.0 * Eta4 - 6.0 * Eta5 - 15.0 * Eta2 * Xi2 + 15.0 * Eta3 * Xi2 + 15.0 * Eta2 * Xi3);
            N(1,16) = 0.25 * (Eta2 * Xi2 + Eta3 * Xi2 - Eta2 * Xi3);
            N(1,17) = 0.5 * (-2.0 * Eta2 * Xi + 2.0 * Eta3 * Xi + Eta2 * Xi2 - Eta3 * Xi2 + Eta2 * Xi3);
            N(1,18) = 0.25 * (2.0 * Eta3 - 4.0 * Eta4 + 2.0 * Eta5 + 5.0 * Eta2 * Xi2 - 5.0 * Eta3 * Xi2 - 5.0 * Eta2 * Xi3);
            #endif

            T.ChangeDimensions(18,18,0.0);
            for(long n=1;n<18;n+=6) {
                T(n+0, n+0) = 1.0;
                T(n+1, n+1) = f * (Jac(1,1));
                T(n+1, n+2) = f * (Jac(1,2));
                T(n+2, n+1) = f * (Jac(2,1));
                T(n+2, n+2) = f * (Jac(2,2));
                T(n+3, n+3) = (Jac(1,1)*Jac(1,1));
                T(n+3, n+4) = (2.0*Jac(1,1)*Jac(1,2));
                T(n+3, n+5) = (Jac(1,2)*Jac(1,2));
                T(n+4, n+3) = (Jac(1,1)*Jac(2,1));
                T(n+4, n+4) = (( Jac(1,2)*Jac(2,1) + Jac(1,1)*Jac(2,2) ));
                T(n+4, n+5) = (Jac(1,2)*Jac(2,2));
                T(n+5, n+3) = (Jac(2,1)*Jac(2,1));
                T(n+5, n+4) = (2.0*Jac(2,1)*Jac(2,2));
                T(n+5, n+5) = (Jac(2,2)*Jac(2,2));
            }

            break;
        default:
            err = 23003;
            Message (err ,(char*)( "Simplex_Nodal_Element_2D::Base_Function")); exit (err);
            break;
    }
    R = N * T; // Convert to Real Coordinates
    vetorN = R.GetRow(1);
}


double Hermite_Element_2D::Base_Function(Point *point, short int qual)
{
    std::cout << "\n\nENTROU EM double Hermite_Element_2D::Base_Function(Point *point, short int qual)\n\n";
    std::cout.flush();

    double value = 0.0;
    double zeta;
    point->Coordinate(1, zeta);
    switch (order) {
        case 1:
            switch (qual) {
                default:
                    Message (23008, (char*)("Base_Function"));break;
            }
            break;
        case 5:
        default:
            Message (23003,(char*)("Base_Function"));
            break;
    }
    return value;
} // End of Hermite_Element_2D::Base_Function


void Hermite_Element_2D::GradN(Point *point, lev_math::Te_Matrix<double> &DN)
{
    lev_math::Te_Matrix<double> T;
    #ifdef _SCHRODINGER_BENDANIELDUKE_BC_
    double f = 1.0 / Get_Material()->effectiveMass_Electron;
    #else
    double f = 1.0;
    #endif
    double Xi, Xi2, Xi3, Eta, Eta2, Eta3, Lambda, Lambda2, Lambda3;
    double Xi4, Xi5, Xi6, Eta4, Eta5, Eta6;
    Xi = point->Get_X();
    Eta = point->Get_Y();
    Lambda = 1.0 - Xi - Eta;
    Lambda2 = Lambda*Lambda;
    Lambda3 = Lambda2*Lambda;
    Xi2 = Xi*Xi;
    Xi3 = Xi2*Xi;
    Xi4 = Xi3*Xi;
    Xi5 = Xi4*Xi;
    Xi6 = Xi5*Xi;
    Eta2 = Eta*Eta;
    Eta3 = Eta2*Eta;
    Eta4 = Eta3*Eta;
    Eta5 = Eta4*Eta;
    Eta6 = Eta5*Eta;

    switch(order) {
        case 5: // Fifth order
            DN.ChangeDimensions(2,18);

            #ifdef _LEV_
            /*
            DN(1,1)  = -2.0*Lambda * (10.0*Lambda - 15.0*Lambda2 + 6.0*Lambda3 + 30.0*Xi2*Eta + 30.0*Xi*Eta2) + Lambda2*(-10.0 + 30.0*Lambda - 18.0*Lambda2 + 60.0*Xi*Eta + 30.0*Eta2);
            DN(1,2)  = (Lambda2 - 2.0*Xi*Lambda) * (3.0 - 2.0*Lambda - 3.0*Xi2 + 6.0*Xi*Eta) + Xi*Lambda2 * (2.0 - 6.0*Xi + 6.0*Eta);
            DN(1,3)  = -2.0*Eta*Lambda * (3.0 - 2.0*Lambda - 3.0*Eta2 + 6.0*Xi*Eta) + Eta*Lambda2 * (2.0 + 6.0*Eta);
            DN(1,4)  = (Xi*Lambda2 - Xi2*Lambda) * (1.0 - Xi + 2.0*Eta) - 0.5*Xi2*Lambda2;
            DN(1,5)  = Eta * (Lambda2 - 2.0*Xi*Lambda);
            DN(1,6)  = Eta2 * (Lambda2 - Lambda * (1.0 + 2.0*Xi - Eta));
            DN(1,7)  = 2.0*Xi * (10.0*Xi - 15.0*Xi2 + 6.0*Xi3 + 15.0*Eta2*Lambda) + Xi2 * (10.0 - 30.0*Xi + 18.0*Xi2 - 15.0*Eta2);
            DN(1,8)  = Xi * (-8.0*Xi + 14.0*Xi2 - 6.0*Xi3 - 15.0*Eta2*Lambda) + 0.5*Xi2 * (-8.0 + 28.0*Xi - 18.0*Xi2 + 15.0*Eta2);
            DN(1,9)  = Eta * (Xi * (6.0 - 4.0*Xi - 3.0*Eta - 3.0*Eta2 + 3.0*Xi*Eta) + 0.5*Xi2 * (-4.0 + 3.0*Eta));
            DN(1,10) = 0.25 * (2.0*Xi * (2.0*Xi - 4.0*Xi2 + 2.0*Xi3 + 5.0*Eta2*Lambda) + Xi2 * (2.0 - 8.0*Xi + 6.0*Xi2 - 5.0*Eta2));
            DN(1,11) = Eta * (Xi * (-2.0 + 2.0*Xi + Eta + Eta2 - Xi*Eta) + 0.5*Xi2 * (2.0 - Eta));
            DN(1,12) = 0.25*Eta2 * (2.0*Xi*Lambda - Xi2) + 1.5*Eta2*Xi2;
            DN(1,13) = 15.0*Eta2 * (2.0*Xi*Lambda - Xi2);
            DN(1,14) = 0.5*Eta2 * ((6.0 - 3.0*Xi - 4.0*Eta - 3.0*Xi2 + 3.0*Xi*Eta) + Xi * (-3.0 - 6.0*Xi + 3.0*Eta));
            DN(1,15) = -7.5*Eta2 * (2.0*Xi*Lambda - Xi2);
            DN(1,16) = 0.25*Eta2 * (2.0*Xi*Lambda - Xi2) + Eta3*Xi;
            DN(1,17) = 0.5*Eta2 * ((-2.0 + Xi + 2.0*Eta + Xi2 - Xi*Eta) + Xi * (1.0 + 2.0*Xi - Eta));
            DN(1,18) = 0.25*Eta2 * (10.0*Xi*Lambda - 5.0*Xi2);

            DN(2,1)  = -2.0*Lambda * (10.0*Lambda - 15.0*Lambda2 + 6.0*Lambda3 + 30.0*Xi2*Eta + 30.0*Xi*Eta2) + Lambda2 * (-10.0 + 30.0*Lambda - 18.0*Lambda2 + 60.0*Xi*Eta + 30.0*Xi2);
            DN(2,2)  = -2.0*Xi*Lambda * (3.0 - 2.0*Lambda - 3.0*Xi2 + 6.0*Xi*Eta) + Xi*Lambda2 * (2.0 + 6.0*Xi);
            DN(2,3)  = (Lambda2 - 2.0*Eta*Lambda) * (3.0 - 2.0*Lambda - 3.0*Eta2 + 6.0*Xi*Eta) + Eta*Lambda2 * (2.0 - 6.0*Eta + 6.0*Xi);
            DN(2,4)  = -Xi2*Lambda * (1.0 - Xi + 2.0*Eta) + Xi2*Lambda2;
            DN(2,5)  = Xi * (Lambda2 - 2.0*Eta*Lambda);
            DN(2,6)  = (Eta*Lambda2 - Eta2*Lambda) * (1.0 + 2.0*Xi - Eta) - 0.5*Eta2*Lambda2;
            DN(2,7)  = 15.0*Xi2 * (2.0*Eta*Lambda - Eta2);
            DN(2,8)  = -7.5*Xi2 * (2.0*Eta*Lambda - Eta2);
            DN(2,9)  = 0.5*Xi2 * ((6.0 - 4.0*Xi - 3.0*Eta - 3.0*Eta2 + 3.0*Xi*Eta) + Eta * (-3.0 - 6.0*Eta + 3.0*Xi));
            DN(2,10) = 0.25*Xi2 * (10.0*Eta*Lambda - 5.0*Eta2);
            DN(2,11) = 0.5*Xi2 * ((-2.0 + 2.0*Xi + Eta + Eta2 -Xi*Eta) + Eta * (1.0 + 2.0*Eta - Xi));
            DN(2,12) = 0.25*Xi2 * (2.0*Eta*Lambda - Eta2) + Xi3*Eta;
            DN(2,13) = 2.0*Eta * (10.0*Eta - 15.0*Eta2 + 6.0*Eta3 + 15.0*Xi2*Lambda) + Eta2 * (10.0 - 30.0*Eta + 18.0*Eta2 - 15.0*Xi2);
            DN(2,14) = Xi * (Eta * (6.0 - 3.0*Xi - 4.0*Eta - 3.0*Xi2 + 3.0*Xi*Eta) + 0.5*Eta2 * (-4.0 + 3.0*Xi));
            DN(2,15) = Eta * (-8.0*Eta + 14.0*Eta2 - 6.0*Eta3 - 15.0*Xi2*Lambda) + 0.5*Eta2 * (-8.0 + 28.0*Eta - 18.0*Eta2 + 15.0*Xi2);
            DN(2,16) = 0.25*Xi2 * (2.0*Eta*Lambda - Eta2) + 1.5*Xi2*Eta2;
            DN(2,17) = Xi * (Eta * (-2.0 + Xi + 2.0*Eta + Xi2 - Xi*Eta) + 0.5*Eta2 * (2.0 - Xi));
            DN(2,18) = 0.25 * (2.0*Eta * (2.0*Eta * (1.0 - 2.0*Eta + Eta2) + 5.0*Xi2*Lambda) + Eta2 * (2.0 - 8.0*Eta + 6.0*Eta2 - 5.0*Xi2));
            */
            DN(1,1)  = 30.0 * Eta2 * Xi - 30.0 * Eta3 * Xi + 30.0 * Xi2 - 45.0 * Eta2 * Xi2 - 60.0 * Xi3 + 30.0 * Xi4;
            DN(1,2)  = 0.5 * (-30.0 * Eta2 * Xi + 30.0 * Eta3 * Xi - 24.0 * Xi2 + 45.0 * Eta2 * Xi2 + 56.0 * Xi3 - 30.0 * Xi4);
            DN(1,3)  = 0.5 * (12.0 * Eta * Xi - 6.0 * Eta2 * Xi - 6.0 * Eta3 * Xi - 12.0 * Eta * Xi2 + 9.0 * Eta2 * Xi2);
            DN(1,4)  = 0.25 * (10.0 * Eta2 * Xi - 10.0 * Eta3 * Xi + 6.0 * Xi2 - 15.0 * Eta2 * Xi2 - 16.0 * Xi3 + 10.0 * Xi4);
            DN(1,5)  = 0.5 * (-4.0 * Eta * Xi + 2.0 * Eta2 * Xi + 2.0 * Eta3 * Xi + 6.0 * Eta * Xi2 - 3.0 * Eta2 * Xi2);
            DN(1,6)  = 0.25 * (2.0 * Eta2 * Xi - 2.0 * Eta3 * Xi + 3.0 * Eta2 * Xi2);
            DN(1,7)  = 30.0 * Eta2 * Xi - 30.0 * Eta3 * Xi - 45.0 * Eta2 * Xi2;
            DN(1,8)  = 0.5 * (6.0 * Eta2 - 4.0 * Eta3 - 6.0 * Eta2 * Xi + 6.0 * Eta3 * Xi - 9.0 * Eta2 * Xi2);
            DN(1,9)  = 0.5 * (-30.0 * Eta2 * Xi + 30.0 * Eta3 * Xi + 45.0 * Eta2 * Xi2);
            DN(1,10) = 0.25 * (2.0 * Eta2 * Xi + 2.0 * Eta3 * Xi - 3.0 * Eta2 * Xi2);
            DN(1,11) = 0.5 * (-2.0 * Eta2 + 2.0 * Eta3 + 2.0 * Eta2 * Xi - 2.0 * Eta3 * Xi + 3.0 * Eta2 * Xi2);
            DN(1,12) = 0.25 * (10.0 * Eta2 * Xi - 10.0 * Eta3 * Xi - 15.0 * Eta2 * Xi2);
            DN(1,13) = -60.0 * Eta2 * Xi + 60.0 * Eta3 * Xi - 30.0 * Xi2 + 90.0 * Eta2 * Xi2 + 60.0 * Xi3 - 30.0 * Xi4;
            DN(1,14) = 1.0 - 3.0 * Eta2 + 2.0 * Eta3 - 12.0 * Eta2 * Xi + 12.0 * Eta3 * Xi - 18.0 * Xi2 + 27.0 * Eta2 * Xi2 + 32.0 * Xi3 - 15.0 * Xi4;
            DN(1,15) = -6.0 * Eta * Xi - 12.0 * Eta2 * Xi + 18.0 * Eta3 * Xi + 6.0 * Eta * Xi2 + 18.0 * Eta2 * Xi2;
            DN(1,16) = 0.5 * (2.0 * Xi - 6.0 * Eta2 * Xi + 4.0 * Eta3 * Xi - 9.0 * Xi2 + 9.0 * Eta2 * Xi2 + 12.0 * Xi3 - 5.0 * Xi4);
            DN(1,17) = Eta - 2.0 * Eta2 + Eta3 - 4.0 * Eta * Xi + 4.0 * Eta2 * Xi + 3.0 * Eta * Xi2;
            DN(1,18) = -3.0 * Eta2 * Xi + 3.0 * Eta3 * Xi + 3.0 * Eta2 * Xi2;

            DN(2,1)  = 30.0 * Eta * Xi2 - 45.0 * Eta2 * Xi2 - 30.0 * Eta * Xi3;
            DN(2,2)  = 0.5 * (-30.0 * Eta * Xi2 + 45.0 * Eta2 * Xi2 + 30.0 * Eta * Xi3);
            DN(2,3)  = 0.5 * (6.0 * Xi2 - 6.0 * Eta * Xi2 - 9.0 * Eta2 * Xi2 - 4.0 * Xi3 + 6.0 * Eta * Xi3);
            DN(2,4)  = 0.25 * (10.0 * Eta * Xi2 - 15.0 * Eta2 * Xi2 - 10.0 * Eta * Xi3);
            DN(2,5)  = 0.5 * (-2.0 * Xi2 + 2.0 * Eta * Xi2 + 3.0 * Eta2 * Xi2 + 2.0 * Xi3 - 2.0 * Eta * Xi3);
            DN(2,6)  = 0.25 * (2.0 * Eta * Xi2 - 3.0 * Eta2 * Xi2 + 2.0 * Eta * Xi3);
            DN(2,7)  = 30.0 * Eta2 - 60.0 * Eta3 + 30.0 * Eta4 + 30.0 * Eta * Xi2 - 45.0 * Eta2 * Xi2 - 30.0 * Eta * Xi3;
            DN(2,8)  = 0.5 * (12.0 * Eta * Xi - 12.0 * Eta2 * Xi - 6.0 * Eta * Xi2 + 9.0 * Eta2 * Xi2 - 6.0 * Eta * Xi3);
            DN(2,9)  = 0.5 * (-24.0 * Eta2 + 56.0 * Eta3 - 30.0 * Eta4 - 30.0 * Eta * Xi2 + 45.0 * Eta2 * Xi2 + 30.0 * Eta * Xi3);
            DN(2,10) = 0.25 * (2.0 * Eta * Xi2 + 3.0 * Eta2 * Xi2 - 2.0 * Eta * Xi3);
            DN(2,11) = 0.5 * (-4.0 * Eta * Xi + 6.0 * Eta2 * Xi + 2.0 * Eta * Xi2 - 3.0 * Eta2 * Xi2 + 2.0 * Eta * Xi3);
            DN(2,12) = 0.25 * (6.0 * Eta2 - 16.0 * Eta3 + 10.0 * Eta4 + 10.0 * Eta * Xi2 - 15.0 * Eta2 * Xi2 - 10.0 * Eta * Xi3);
            DN(2,13) = -30.0 * Eta2 + 60.0 * Eta3 - 30.0 * Eta4 - 60.0 * Eta * Xi2 + 90.0 * Eta2 * Xi2 + 60.0 * Eta * Xi3;
            DN(2,14) = -6.0 * Eta * Xi + 6.0 * Eta2 * Xi - 12.0 * Eta * Xi2 + 18.0 * Eta2 * Xi2 + 18.0 * Eta * Xi3;
            DN(2,15) = 1.0 - 18.0 * Eta2 + 32.0 * Eta3 - 15.0 * Eta4 - 3.0 * Xi2 - 12.0 * Eta * Xi2 + 27.0 * Eta2 * Xi2 + 2.0 * Xi3 + 12.0 * Eta * Xi3;
            DN(2,16) = -3.0 * Eta * Xi2 + 3.0 * Eta2 * Xi2 + 3.0 * Eta * Xi3;
            DN(2,17) = Xi - 4.0 * Eta * Xi + 3.0 * Eta2 * Xi - 2.0 * Xi2 + 4.0 * Eta * Xi2 + Xi3;
            DN(2,18) = 0.5 * (2.0 * Eta - 9.0 * Eta2 + 12.0 * Eta3 - 5.0 * Eta4 - 6.0 * Eta * Xi2 + 9.0 * Eta2 * Xi2 + 4.0 * Eta * Xi3);
            //std::cout << "\n";
            //for(long i=1; i<19; i++)
            //    std::cout << "\nDN(1," << i << "): = " << DN(1,i);
            #else
            DN(1,1) = -60.0 * Eta2 * Xi + 60.0 * Eta3 * Xi - 30.0 * Xi2 + 90.0 * Eta2 * Xi2 + 60.0 * Xi3 - 30.0 * Xi4;
            DN(1,2) = 1.0 - 3.0 * Eta2 + 2.0 * Eta3 - 12.0 * Eta2 * Xi + 12.0 * Eta3 * Xi - 18.0 * Xi2 + 27.0 * Eta2 * Xi2 + 32.0 * Xi3 - 15.0 * Xi4;
            DN(1,3) = -6.0 * Eta * Xi - 12.0 * Eta2 * Xi + 18.0 * Eta3 * Xi + 6.0 * Eta * Xi2 + 18.0 * Eta2 * Xi2;
            DN(1,4) = 0.5 * (2.0 * Xi - 6.0 * Eta2 * Xi + 4.0 * Eta3 * Xi - 9.0 * Xi2 + 9.0 * Eta2 * Xi2 + 12.0 * Xi3 - 5.0 * Xi4);
            DN(1,5) = Eta - 2.0 * Eta2 + Eta3 - 4.0 * Eta * Xi + 4.0 * Eta2 * Xi + 3.0 * Eta * Xi2;
            DN(1,6) = -3.0 * Eta2 * Xi + 3.0 * Eta3 * Xi + 3.0 * Eta2 * Xi2;
            DN(1,7)  = 30.0 * Eta2 * Xi - 30.0 * Eta3 * Xi + 30.0 * Xi2 - 45.0 * Eta2 * Xi2 - 60.0 * Xi3 + 30.0 * Xi4;
            DN(1,8)  = 0.5 * (-30.0 * Eta2 * Xi + 30.0 * Eta3 * Xi - 24.0 * Xi2 + 45.0 * Eta2 * Xi2 + 56.0 * Xi3 - 30.0 * Xi4);
            DN(1,9)  = 0.5 * (12.0 * Eta * Xi - 6.0 * Eta2 * Xi - 6.0 * Eta3 * Xi - 12.0 * Eta * Xi2 + 9.0 * Eta2 * Xi2);
            DN(1,10) = 0.25 * (10.0 * Eta2 * Xi - 10.0 * Eta3 * Xi + 6.0 * Xi2 - 15.0 * Eta2 * Xi2 - 16.0 * Xi3 + 10.0 * Xi4);
            DN(1,11) = 0.5 * (-4.0 * Eta * Xi + 2.0 * Eta2 * Xi + 2.0 * Eta3 * Xi + 6.0 * Eta * Xi2 - 3.0 * Eta2 * Xi2);
            DN(1,12) = 0.25 * (2.0 * Eta2 * Xi - 2.0 * Eta3 * Xi + 3.0 * Eta2 * Xi2);
            DN(1,13) = 30.0 * Eta2 * Xi - 30.0 * Eta3 * Xi - 45.0 * Eta2 * Xi2;
            DN(1,14) = 0.5 * (6.0 * Eta2 - 4.0 * Eta3 - 6.0 * Eta2 * Xi + 6.0 * Eta3 * Xi - 9.0 * Eta2 * Xi2);
            DN(1,15) = 0.5 * (-30.0 * Eta2 * Xi + 30.0 * Eta3 * Xi + 45.0 * Eta2 * Xi2);
            DN(1,16) = 0.25 * (2.0 * Eta2 * Xi + 2.0 * Eta3 * Xi - 3.0 * Eta2 * Xi2);
            DN(1,17) = 0.5 * (-2.0 * Eta2 + 2.0 * Eta3 + 2.0 * Eta2 * Xi - 2.0 * Eta3 * Xi + 3.0 * Eta2 * Xi2);
            DN(1,18) = 0.25 * (10.0 * Eta2 * Xi - 10.0 * Eta3 * Xi - 15.0 * Eta2 * Xi2);

            DN(2,1)  = -30.0 * Eta2 + 60.0 * Eta3 - 30.0 * Eta4 - 60.0 * Eta * Xi2 + 90.0 * Eta2 * Xi2 + 60.0 * Eta * Xi3;
            DN(2,2)  = -6.0 * Eta * Xi + 6.0 * Eta2 * Xi - 12.0 * Eta * Xi2 + 18.0 * Eta2 * Xi2 + 18.0 * Eta * Xi3;
            DN(2,3)  = 1.0 - 18.0 * Eta2 + 32.0 * Eta3 - 15.0 * Eta4 - 3.0 * Xi2 - 12.0 * Eta * Xi2 + 27.0 * Eta2 * Xi2 + 2.0 * Xi3 + 12.0 * Eta * Xi3;
            DN(2,4)  = -3.0 * Eta * Xi2 + 3.0 * Eta2 * Xi2 + 3.0 * Eta * Xi3;
            DN(2,5)  = Xi - 4.0 * Eta * Xi + 3.0 * Eta2 * Xi - 2.0 * Xi2 + 4.0 * Eta * Xi2 + Xi3;
            DN(2,6)  = 0.5 * (2.0 * Eta - 9.0 * Eta2 + 12.0 * Eta3 - 5.0 * Eta4 - 6.0 * Eta * Xi2 + 9.0 * Eta2 * Xi2 + 4.0 * Eta * Xi3);
            DN(2,7)  = 30.0 * Eta * Xi2 - 45.0 * Eta2 * Xi2 - 30.0 * Eta * Xi3;
            DN(2,8)  = 0.5 * (-30.0 * Eta * Xi2 + 45.0 * Eta2 * Xi2 + 30.0 * Eta * Xi3);
            DN(2,9)  = 0.5 * (6.0 * Xi2 - 6.0 * Eta * Xi2 - 9.0 * Eta2 * Xi2 - 4.0 * Xi3 + 6.0 * Eta * Xi3);
            DN(2,10) = 0.25 * (10.0 * Eta * Xi2 - 15.0 * Eta2 * Xi2 - 10.0 * Eta * Xi3);
            DN(2,11) = 0.5 * (-2.0 * Xi2 + 2.0 * Eta * Xi2 + 3.0 * Eta2 * Xi2 + 2.0 * Xi3 - 2.0 * Eta * Xi3);
            DN(2,12) = 0.25 * (2.0 * Eta * Xi2 - 3.0 * Eta2 * Xi2 + 2.0 * Eta * Xi3);
            DN(2,13) = 30.0 * Eta2 - 60.0 * Eta3 + 30.0 * Eta4 + 30.0 * Eta * Xi2 - 45.0 * Eta2 * Xi2 - 30.0 * Eta * Xi3;
            DN(2,14) = 0.5 * (12.0 * Eta * Xi - 12.0 * Eta2 * Xi - 6.0 * Eta * Xi2 + 9.0 * Eta2 * Xi2 - 6.0 * Eta * Xi3);
            DN(2,15) = 0.5 * (-24.0 * Eta2 + 56.0 * Eta3 - 30.0 * Eta4 - 30.0 * Eta * Xi2 + 45.0 * Eta2 * Xi2 + 30.0 * Eta * Xi3);
            DN(2,16) = 0.25 * (2.0 * Eta * Xi2 + 3.0 * Eta2 * Xi2 - 2.0 * Eta * Xi3);
            DN(2,17) = 0.5 * (-4.0 * Eta * Xi + 6.0 * Eta2 * Xi + 2.0 * Eta * Xi2 - 3.0 * Eta2 * Xi2 + 2.0 * Eta * Xi3);
            DN(2,18) = 0.25 * (6.0 * Eta2 - 16.0 * Eta3 + 10.0 * Eta4 + 10.0 * Eta * Xi2 - 15.0 * Eta2 * Xi2 - 10.0 * Eta * Xi3);
            //std::cout << "\n";
            //for(long i=1; i<19; i++)
            //    std::cout << "\nDN(1," << i << "): = " << DN(1,i);
            //getchar();
            #endif
            T.ChangeDimensions(18,18,0.0);
            for(long n=1;n<18;n+=6) {
                T(n+0, n+0) = 1.0;
                T(n+1, n+1) = f * (Jac(1,1));
                T(n+1, n+2) = f * (Jac(1,2));
                T(n+2, n+1) = f * (Jac(2,1));
                T(n+2, n+2) = f * (Jac(2,2));
                T(n+3, n+3) = (Jac(1,1)*Jac(1,1));
                T(n+3, n+4) = (2.0*Jac(1,1)*Jac(1,2));
                T(n+3, n+5) = (Jac(1,2)*Jac(1,2));
                T(n+4, n+3) = (Jac(1,1)*Jac(2,1));
                T(n+4, n+4) = (( Jac(1,2)*Jac(2,1) + Jac(1,1)*Jac(2,2) ));
                T(n+4, n+5) = (Jac(1,2)*Jac(2,2));
                T(n+5, n+3) = (Jac(2,1)*Jac(2,1));
                T(n+5, n+4) = (2.0*Jac(2,1)*Jac(2,2));
                T(n+5, n+5) = (Jac(2,2)*Jac(2,2));
            }
            break;
        default:
            Message (23003 ,(char*)( "Hermite_Element_2D::GradN"));
            break;
    }
    DN = jac * (DN * T); // Convert to Real Coordinates
} // End of Hermite_Element_2D::GradN


void Hermite_Element_2D::DivN(Point *point, lev_math::Te_Matrix<double> &divN)
{
    Message(23000, (char*)("Hermite_Element_2D:: DivN"));
}


void Hermite_Element_2D::RotN(Point *point, lev_math::Te_Matrix<double> &rotN)
{
    Message(23000,(char*) ( "Hermite_Element_2D:: RotN"));
}


//===============================================================================
//short int Simplex_Nodal_Element_3D::n_nodes=0 ;
// =============================================================================


Simplex_Nodal_Element_3D::Simplex_Nodal_Element_3D() : Nodal_Element()
{
    dimension           = 3;
    n_vertices            = 4;
    num_max_vizinhos    = 4;
    num_vizinhos        = 0; // *AP* 15/10/2007
    material            = NULL;
    nodes               = NULL;
} // end of Simplex_Nodal_Element_3D default constructor


// =========================================================================


Simplex_Nodal_Element_3D::Simplex_Nodal_Element_3D (int o) : Nodal_Element(o)
{
    dimension           = 3;
    n_vertices            = 4;
    num_max_vizinhos    = 4;
    num_vizinhos        = 0; // *AP* 15/10/2007
    material            = NULL;
    nodes               = NULL;
} // end of Simplex_Nodal_Element_3D default constructor



short int  Simplex_Nodal_Element_3D::Build_Higher_Order(void)
{ // this function must resize nodes to store the new points index
  // USE method Resize_Nodes to do so!
   short int err = 0; // no errors
   if(order == 1) return err;
   else
   {
      err = Resize_Nodes();
      if (!err)
      { Message ( 23000, (char*)("Build_Higher_Order")); }
   };
   return err;
} // end of Simplex_Nodal_Element_3D::Build_Higher_Order


/** bool Simplex_Nodal_Element_3D::Is_Inside(Point & point)
*AP* nao implementada
*/
bool Simplex_Nodal_Element_3D::Is_Inside(Point & point)
{
    Point        phom;
    bool        retorno = false;

    CartesianToHom(point, phom);

//    if(phom.X() >= 0 && phom.X() <= 1.0)
//    {
//        retorno = true;
//    }

    return retorno;

} // Elemento_3D ::Is_Inside


void   Simplex_Nodal_Element_3D::  Transformation_Coefficients(double *a, double *b, double *c, double *d)
{
   double aux1, aux2, aux3, aux_zz1,aux_zz2,aux_zz3;
   double x_aux1, x_aux2, x_aux3, x_aux4;
   double y_aux1, y_aux2, y_aux3, y_aux4;
   double z_aux1, z_aux2, z_aux3, z_aux4;
    int i;
   Point *p[4];  // tetrahedron vertices
   Point *aux_point = NULL;
    for ( i = 0; i < n_vertices; i++)
   {
      lo_Points->Get_Componente(nodes[i],aux_point);
      p[i] = aux_point;
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

// -----------------------------------------------------------------------------
// coefficient calculation for the x1 coordinate
// -----------------------------------------------------------------------------

    a[1] = x_aux2 * (y_aux3 * z_aux4 - y_aux4 * z_aux3);
    a[1]+= x_aux3 * (y_aux4 * z_aux2 - y_aux2 * z_aux4);
    a[1]+= x_aux4 * (y_aux2 * z_aux3 - y_aux3 * z_aux2);

    aux_zz1 = ( z_aux4 - z_aux3 );
    aux_zz2 = ( z_aux2 - z_aux4 );
    aux_zz3 = ( z_aux3 - z_aux2 );

    aux1 = y_aux2 * ( aux_zz1 );
    aux2 = y_aux3 * ( aux_zz2 );
    aux3 = y_aux4 * ( aux_zz3 );

    b[1] = aux1 + aux2 + aux3;

    aux1 = x_aux2 * ( -aux_zz1 );
    aux2 = x_aux3 * ( -aux_zz2 );
    aux3 = x_aux4 * ( -aux_zz3 );

    c[1] = aux1 + aux2 + aux3;

    aux1 = x_aux2 * ( y_aux4 - y_aux3 );
    aux2 = x_aux3 * ( y_aux2 - y_aux4 );
    aux3 = x_aux4 * ( y_aux3 - y_aux2 );

    d[1] = aux1 + aux2 + aux3;

// -----------------------------------------------------------------------------
// coefficient calculation for the x2 coordinate
// -----------------------------------------------------------------------------
    a[2] = x_aux1 * (y_aux4 * z_aux3 - y_aux3 * z_aux4);
    a[2]+= x_aux3 * (y_aux1 * z_aux4 - y_aux4 * z_aux1);
    a[2]+= x_aux4 * (y_aux3 * z_aux1 - y_aux1 * z_aux3);

    aux_zz1 =  ( z_aux3 - z_aux4 );
    aux_zz2 =  ( z_aux4 - z_aux1 );
    aux_zz3 =  ( z_aux1 - z_aux3 );

    aux1 = y_aux1 * ( aux_zz1 );
    aux2 = y_aux3 * ( aux_zz2 );
    aux3 = y_aux4 * ( aux_zz3 );

    b[2] = aux1 + aux2 + aux3;

    aux1 = x_aux1 * ( -aux_zz1 );
    aux2 = x_aux3 * ( -aux_zz2 );
    aux3 = x_aux4 * ( -aux_zz3 );

   c[2] = aux1 + aux2 + aux3;

   aux1 = x_aux1 * ( y_aux3 - y_aux4 );
   aux2 = x_aux3 * ( y_aux4 - y_aux1 );
   aux3 = x_aux4 * ( y_aux1 - y_aux3 );

   d[2] = aux1 + aux2 + aux3;

// -----------------------------------------------------------------------------
// coefficient calculation for the x3 coordinate
// -----------------------------------------------------------------------------
    a[3] = x_aux1 * (y_aux2 * z_aux4 - y_aux4 * z_aux2);
    a[3]+= x_aux2 * (y_aux4 * z_aux1 - y_aux1 * z_aux4);
    a[3]+= x_aux4 * (y_aux1 * z_aux2 - y_aux2 * z_aux1);

    aux_zz1 = ( z_aux4 - z_aux2 );
   aux_zz2 = ( z_aux1 - z_aux4 );
   aux_zz3 = ( z_aux2 - z_aux1 );

   aux1 = y_aux1 * ( aux_zz1 );
   aux2 = y_aux2 * ( aux_zz2 );
   aux3 = y_aux4 * ( aux_zz3 );

   b[3] = aux1 + aux2 + aux3;

   aux1 = x_aux1 * ( -aux_zz1 );
   aux2 = x_aux2 * ( -aux_zz2 );
   aux3 = x_aux4 * ( -aux_zz3 );

   c[3] = aux1 + aux2 + aux3;

   aux1 = x_aux1 * ( y_aux4 - y_aux2 );
   aux2 = x_aux2 * ( y_aux1 - y_aux4 );
   aux3 = x_aux4 * ( y_aux2 - y_aux1 );

   d[3] = aux1 + aux2 + aux3;

   a[4] = - (a[1] + a[2] + a[3]);
   b[4] = - (b[1] + b[2] + b[3]);
   c[4] = - (c[1] + c[2] + c[3]);
   d[4] = - (d[1] + d[2] + d[3]);

} // end of Transformation_Coefficients

//==============================================================================


void   Simplex_Nodal_Element_3D:: MatJacobHom ()
{
	#if 0
    double b[4], c[4], d[4];
    double aux1, aux2, aux3, aux_zz1,aux_zz2,aux_zz3;
    double x_aux1, x_aux2, x_aux3, x_aux4;
    double y_aux1, y_aux2, y_aux3, y_aux4;
    double z_aux1, z_aux2, z_aux3, z_aux4;
    int i;
    Point *p[4];  // tetrahedron vertices
    Point *aux_point=NULL;
    //   Transformation_Coefficients(&a[0], &b[0], &c[0], &d[0]);

    if(!jacHom) {
        JHom.ChangeDimensions(3,3); // *CM* 12/10/2007 iclu�do a fun��o de dimensionamento

        for(i=0; i<n_vertices; i++) {
            lo_Points->Get_Componente(nodes[i], aux_point);
            p[i] = aux_point;
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

        // -----------------------------------------------------------------------------
        // coefficient calculation for the x1 coordinate
        // -----------------------------------------------------------------------------
        aux_zz1 = ( z_aux4 - z_aux3 );
        aux_zz2 = ( z_aux2 - z_aux4 );
        aux_zz3 = ( z_aux3 - z_aux2 );

        aux1 = y_aux2 * ( aux_zz1 );
        aux2 = y_aux3 * ( aux_zz2 );
        aux3 = y_aux4 * ( aux_zz3 );

        b[1] = aux1 + aux2 + aux3;

        aux1 = x_aux2 * ( -aux_zz1 );
        aux2 = x_aux3 * ( -aux_zz2 );
        aux3 = x_aux4 * ( -aux_zz3 );

        c[1] = aux1 + aux2 + aux3;

        aux1 = x_aux2 * ( y_aux4 - y_aux3 );
        aux2 = x_aux3 * ( y_aux2 - y_aux4 );
        aux3 = x_aux4 * ( y_aux3 - y_aux2 );

        d[1] = aux1 + aux2 + aux3;

        // -----------------------------------------------------------------------------
        // coefficient calculation for the x2 coordinate
        // -----------------------------------------------------------------------------
        aux_zz1 =  ( z_aux3 - z_aux4 );
        aux_zz2 =  ( z_aux4 - z_aux1 );
        aux_zz3 =  ( z_aux1 - z_aux3 );

        aux1 = y_aux1 * ( aux_zz1 );
        aux2 = y_aux3 * ( aux_zz2 );
        aux3 = y_aux4 * ( aux_zz3 );

        b[2] = aux1 + aux2 + aux3;

        aux1 = x_aux1 * ( -aux_zz1 );
        aux2 = x_aux3 * ( -aux_zz2 );
        aux3 = x_aux4 * ( -aux_zz3 );

        c[2] = aux1 + aux2 + aux3;

        aux1 = x_aux1 * ( y_aux3 - y_aux4 );
        aux2 = x_aux3 * ( y_aux4 - y_aux1 );
        aux3 = x_aux4 * ( y_aux1 - y_aux3 );

        d[2] = aux1 + aux2 + aux3;

        // -----------------------------------------------------------------------------
        // coefficient calculation for the x3 coordinate
        // -----------------------------------------------------------------------------
        aux_zz1 = ( z_aux4 - z_aux2 );
        aux_zz2 = ( z_aux1 - z_aux4 );
        aux_zz3 = ( z_aux2 - z_aux1 );

        aux1 = y_aux1 * ( aux_zz1 );
        aux2 = y_aux2 * ( aux_zz2 );
        aux3 = y_aux4 * ( aux_zz3 );

        b[3] = aux1 + aux2 + aux3;

        aux1 = x_aux1 * ( -aux_zz1 );
        aux2 = x_aux2 * ( -aux_zz2 );
        aux3 = x_aux4 * ( -aux_zz3 );

        c[3] = aux1 + aux2 + aux3;

        aux1 = x_aux1 * ( y_aux4 - y_aux2 );
        aux2 = x_aux2 * ( y_aux1 - y_aux4 );
        aux3 = x_aux4 * ( y_aux2 - y_aux1 );

        d[3] = aux1 + aux2 + aux3;

        JHom.SetValue(1,1,b[1]); JHom.SetValue(1,2,b[2]); JHom.SetValue(1,3,b[3]);
        JHom.SetValue(2,1,c[1]); JHom.SetValue(2,2,c[2]); JHom.SetValue(2,3,c[3]);
        JHom.SetValue(3,1,d[1]); JHom.SetValue(3,2,d[2]); JHom.SetValue(3,3,d[3]);

        jacHom = size = (JHom(1,1)*JHom(2,2)*JHom(3,3)) + (JHom(1,2)*JHom(2,3)*JHom(3,1)) + (JHom(2,1)*JHom(3,2)*JHom(1,3)) - (JHom(3,1)*JHom(2,2)*JHom(1,3)) - (JHom(1,2)*JHom(2,1)*JHom(3,3)) - (JHom(1,1)*JHom(3,2)*JHom(2,3)) ;
    }
	#else
    Point *p[4];  // Tetrahedron vertices

    if(!jacDet) {
        Jac.ChangeDimensions(3,3);
        jac.ChangeDimensions(3,3);

        for(int i = 0; i<n_vertices; i++)
            lo_Points->Get_Componente(nodes[i], p[i]);

        Jac.SetValue(1, 1, (p[0]->Get_X() - p[3]->Get_X()));    Jac.SetValue(1, 2, (p[0]->Get_Y() - p[3]->Get_Y()));    Jac.SetValue(1, 3, (p[0]->Get_Z() - p[3]->Get_Z()));
        Jac.SetValue(2, 1, (p[1]->Get_X() - p[3]->Get_X()));    Jac.SetValue(2, 2, (p[1]->Get_Y() - p[3]->Get_Y()));    Jac.SetValue(2, 3, (p[1]->Get_Z() - p[3]->Get_Z()));
        Jac.SetValue(3, 1, (p[2]->Get_X() - p[3]->Get_X()));    Jac.SetValue(3, 2, (p[2]->Get_Y() - p[3]->Get_Y()));    Jac.SetValue(3, 3, (p[2]->Get_Z() - p[3]->Get_Z()));

        JacDet = (Jac(1,1)*Jac(2,2)*Jac(3,3) + Jac(2,1)*Jac(3,2)*Jac(1,3) + Jac(3,1)*Jac(1,2)*Jac(2,3)
        		- Jac(3,1)*Jac(2,2)*Jac(1,3) - Jac(3,2)*Jac(2,3)*Jac(1,1) - Jac(3,3)*Jac(2,1)*Jac(1,2));

        jac.SetValue(1,1, Jac(2,2)*Jac(3,3)-Jac(2,3)*Jac(3,2));    jac.SetValue(1,2, Jac(1,3)*Jac(3,2)-Jac(1,2)*Jac(3,3));    jac.SetValue(1,3, Jac(1,2)*Jac(2,3)-Jac(1,3)*Jac(2,2));
        jac.SetValue(2,1, Jac(2,3)*Jac(3,1)-Jac(2,1)*Jac(3,3));    jac.SetValue(2,2, Jac(1,1)*Jac(3,3)-Jac(1,3)*Jac(3,1));    jac.SetValue(2,3, Jac(1,3)*Jac(2,1)-Jac(1,1)*Jac(2,3));
        jac.SetValue(3,1, Jac(2,1)*Jac(3,2)-Jac(2,2)*Jac(3,1));    jac.SetValue(3,2, Jac(1,2)*Jac(3,1)-Jac(1,1)*Jac(3,2));    jac.SetValue(3,3, Jac(1,1)*Jac(2,2)-Jac(1,2)*Jac(2,1));

        jac = (1.0/JacDet) * jac;

        jacDet = (jac(1,1)*jac(2,2)*jac(3,3) + jac(2,1)*jac(3,2)*jac(1,3) + jac(3,1)*jac(1,2)*jac(2,3)
        	    - jac(3,1)*jac(2,2)*jac(1,3) - jac(3,2)*jac(2,3)*jac(1,1) - jac(3,3)*jac(2,1)*jac(1,2));

        //=====================================================================
        // For 3D: size = 6 * tetrahedron volume
        //=====================================================================
        JHom = jac;
        size = jacHom = jacDet;
    }
	#endif
} // end of Simplex_Nodal_Element_3D::MatJacobHom

//==============================================================================


void   Simplex_Nodal_Element_3D:: CartesianToHom(Point &point, Point &phom)
{
   double zeta[3];
   double &zeta1 = zeta[0], &zeta2 = zeta[1], &zeta3 = zeta[2];
   double a[5], b[5], c[5], d[5];
   Transformation_Coefficients(&a[0], &b[0], &c[0], &d[0]);
// -----------------------------------------------------------------------------
// homogeneous coordinates calculation
// -----------------------------------------------------------------------------
   zeta1  = a[1] + b[1] * point.Get_X() + c[1] * point.Get_Y() + d[1] * point.Get_Z();
   zeta1 /= size;
   zeta2  = a[2] + b[2] * point.Get_X() + c[2] * point.Get_Y() + d[2] * point.Get_Z();
   zeta2 /= size;
   zeta3  = a[3] + b[3] * point.Get_X() + c[3] * point.Get_Y() + d[3] * point.Get_Z();
   zeta3 /= size;
   phom.Set_Coordinates(&zeta[0]);

 } // end of Simplex_Nodal_Element_3D::CartesianToHom

//==============================================================================
void   Simplex_Nodal_Element_3D:: HomToCartesian(Point &phom, Point &point)
{
    int i;
  double coord[3];
    Point *p[4];  // tetrahedron vertices
    Point *aux_point = NULL;
    for ( i = 0; i < n_vertices; i++)
    {
        lo_Points->Get_Componente(nodes[i], aux_point);
        p[i] = aux_point;
  }

    double X1 = p[0]->Get_X(), X2 = p[1]->Get_X(), X3 = p[2]->Get_X(), X4 = p[3]->Get_X();
    double Y1 = p[0]->Get_Y(), Y2 = p[1]->Get_Y(), Y3 = p[2]->Get_Y(), Y4 = p[3]->Get_Y();
    double Z1 = p[0]->Get_Z(), Z2 = p[1]->Get_Z(), Z3 = p[2]->Get_Z(), Z4 = p[3]->Get_Z();
  coord[0] = X4 + (X1-X4)*phom.Get_X() + (X2-X4)*phom.Get_Y() + (X3-X4)*phom.Get_Z();
  coord[1] = Y4 + (Y1-Y4)*phom.Get_X() + (Y2-Y4)*phom.Get_Y() + (Y3-Y4)*phom.Get_Z();
  coord[2] = Z4 + (Z1-Z4)*phom.Get_X() + (Z2-Z4)*phom.Get_Y() + (Z3-Z4)*phom.Get_Z();

    point.Set_Coordinates(&coord[0]);
} // end of Simplex_Nodal_Element_3D::HomToCartesian

//=============================================================================
Simplex_Nodal_Element_3D & Simplex_Nodal_Element_3D::
operator = (Simplex_Nodal_Element_3D & el)
{

//    n_vertices = el.n_vertices;
    memcpy (&nodes[0], &el.nodes[0], sizeof (long) * el.n_nodes) ; // the nodes content is copied
    material = el.material;      // the material pointer is copied
   size = el.size;
//   all other data are static
   return *this;

} // end of Simplex_Nodal_Element_3D::operator =
//==============================================================================
short int Simplex_Nodal_Element_3D::Gauss_Points (int quad_ord, int &npgauss, Gauss_Pt * pt)
{
  short int ret = 0;
   const double cte1 = 0.138196601125010504;
   const double cte2 = 0.1666666666666667;
   const double cte3 = 0.585410196624968404;
   double peso;
   double coord[3];
   double &X = coord[0],
         &Y = coord[1],
         &Z = coord[2];
   int k;

   switch( quad_ord )
   {
      case   1:
         npgauss = 1;
         X = Y = Z = 0.25; peso = cte2;
         pt[0].Set_Coordinates(&coord[0]);
         pt[0].Set_Variables(1,peso);
         break;
      case   2:
         npgauss = 4;
         peso = .0416666666666666644;
         X = Y = Z = cte1;
         pt[0].Set_Coordinates(&coord[0]);
         pt[0].Set_Variables(1,peso);

         X = Y = cte1; Z = cte3;
         pt[1].Set_Coordinates(&coord[0]);
         pt[1].Set_Variables(1,peso);

         X = Z = cte1; Y = cte3;
         pt[2].Set_Coordinates(&coord[0]);
         pt[2].Set_Variables(1,peso);

         Y = Z = cte1; X = cte3;
         pt[3].Set_Coordinates(&coord[0]);
         pt[3].Set_Variables(1,peso);
         break;
      case   3:
         npgauss = 5;
         peso = -.133333333333333331;
         pt[0].Set_Variables(1,peso);
         peso = .0749999999999999972;
         for (k = 1; k < npgauss; k++) pt[k].Set_Variables(1,peso);
         X = Y = Z = 0.25;
         pt[0].Set_Coordinates(&coord[0]);
         X = Y = Z = cte2;
         pt[1].Set_Coordinates(&coord[0]);
         X = Y = cte2; Z = 0.5;
         pt[2].Set_Coordinates(&coord[0]);
         X = Z = cte2 ; Y = 0.5;
         pt[3].Set_Coordinates(&coord[0]);
         X = 0.5; Y = Z = cte2;
         pt[4].Set_Coordinates(&coord[0]);
         break;
      default :
         ret = 23007;
         Message (ret, (char*)("Gauss Points"));
         break;
   } // end of switch(quad_ord)
   return ret;
} // end of  Simplex_Nodal_Element_3D::Gauss_Points


//===============================================================================


double Simplex_Nodal_Element_3D::Get_Biggest_Edge()
{
    int i;
    long int a[2];;
    Lagrange_Element_1D aresta[6];
    double maior_comp=-1, comp;

    aresta[0].Set_Nodes(nodes);  // 01
    aresta[1].Set_Nodes(nodes+1); // 12
    aresta[2].Set_Nodes(nodes+2); // 23
    a[0] = nodes[0]; a[1] = nodes[2];
    aresta[3].Set_Nodes(a);        //02
    a[0] = nodes[0]; a[1] = nodes[3];
    aresta[4].Set_Nodes(a);        //03
    a[0] = nodes[1]; a[1] = nodes[3];
    aresta[5].Set_Nodes(a);        //03

    for (i=0; i < 6; i++)
    {
        comp = aresta[i].Get_Size();
        (comp > maior_comp)? maior_comp = comp: maior_comp;
    }
    return (maior_comp);
}


/**Obs: n�o testado */
int Simplex_Nodal_Element_3D::Get_Centroid(Point &p_centroid)
{
    int retorno = 0;
    Point  *pt_at[4];
    double coord[3];
    double x[4], y[4], z[4] ;

    if (nodes)
    {
        for (int i = 0; i < n_vertices; i++)  pt_at[i] = lo_Points[0][nodes[i]];

        x[0] = pt_at[0]->Get_X();
        x[1] = pt_at[1]->Get_X();
        x[2] = pt_at[2]->Get_X();
        x[3] = pt_at[3]->Get_X();

        coord[0] = (x[0]+x[1]+x[2]+x[3])/n_vertices;

        y[0] = pt_at[0]->Get_Y();
        y[1] = pt_at[1]->Get_Y();
        y[2] = pt_at[2]->Get_Y();
        y[3] = pt_at[3]->Get_Y();

        coord[1] = (y[0]+y[1]+y[2]+y[3])/n_vertices;

        z[0] = pt_at[0]->Get_Y();
        z[1] = pt_at[1]->Get_Y();
        z[2] = pt_at[2]->Get_Y();
        z[3] = pt_at[3]->Get_Y();

        coord[2] = (z[0]+z[1]+z[2]+z[3])/n_vertices;

        p_centroid.Dimension(3);//por consistencia
        p_centroid.Set_Coordinates(&coord[0]);
    }
    else retorno = 1;

    return retorno;

} // End of int Simplex_Nodal_Element_3D::Get_Centroid(Point &p_centroid)


void Simplex_Nodal_Element_3D::Set_Size(double vol)
{
  if (vol) { size = vol; return;}

// ===========================================================================
// trecho usado para o calculo de size a partir das coordenadas dos vertices do
// elemento
// declaracao de variaveis auxiliares ++++++++++++++++++++++++++++++++++++++++
// ===========================================================================

    // produtos do determinante denominado na documentacao detA1 ++++++++++++++

    Point  *pt_at[4];
    double y0z1, y0z2, y0z3 ; double y1z0, y1z2, y1z3 ;
    double y2z0, y2z1, y2z3 ; double y3z0, y3z1, y3z2 ;

    // os 4 termos do determinante denominado detA, utilizados tambem em detA4

    double termo1_detA, termo2_detA, termo3_detA, termo4_detA;

    // os determinantes +++++++++++++++++++++++++++++++++++++++++++++++++++++++

    double detA;
    double b[4];
    double x[4], y[4], z[4];

    // ========================================================================
    // ================ calculo de variaveis auxiliares =======================
    // ========================================================================

  if (nodes)
  {
    for (int i = 0; i < n_vertices; i++)  pt_at[i] = lo_Points[0][nodes[i]];
    x[0] = pt_at[0]->Get_X();   x[1] = pt_at[1]->Get_X();
    x[2] = pt_at[2]->Get_X();   x[3] = pt_at[3]->Get_X();
      y[0] = pt_at[0]->Get_Y();   y[1] = pt_at[1]->Get_Y();
      y[2] = pt_at[2]->Get_Y();   y[3] = pt_at[3]->Get_Y();
      z[0] = pt_at[0]->Get_Z();   z[1] = pt_at[1]->Get_Z();
      z[2] = pt_at[2]->Get_Z();   z[3] = pt_at[3]->Get_Z();

      y0z1 = y[0] * z[1]; y0z2 = y[0] * z[2]; y0z3 = y[0] * z[3];

      y1z0 = y[1] * z[0]; y1z2 = y[1] * z[2]; y1z3 = y[1] * z[3];

      y2z0 = y[2] * z[0]; y2z1 = y[2] * z[1]; y2z3 = y[2] * z[3];

      y3z0 = y[3] * z[0]; y3z1 = y[3] * z[1]; y3z2 = y[3] * z[2];

    b[0] = - ( x[0] * x[0] + y[0] * y[0] + z[0] * z[0] );
    b[1] = - ( x[1] * x[1] + y[1] * y[1] + z[1] * z[1] );
    b[2] = - ( x[2] * x[2] + y[2] * y[2] + z[2] * z[2] );
    b[3] = - ( x[3] * x[3] + y[3] * y[3] + z[3] * z[3] );

    // a forma de calculo adotada a seguir visa reduzir o problemas de
    // diferenca de numeros muito proximos


    termo1_detA =   x[1]*y3z2 + x[2]*y1z3 + x[3]*y2z1 - ( x[1]*y2z3 + x[2]*y3z1 + x[3]*y1z2 );

    termo2_detA =   x[0]*y2z3 + x[2]*y3z0 + x[3]*y0z2 - ( x[0]*y3z2 + x[2]*y0z3 + x[3]*y2z0 );

    termo3_detA =   x[0]*y3z1 + x[1]*y0z3 + x[3]*y1z0 - ( x[0]*y1z3 + x[1]*y3z0 + x[3]*y0z1 );

    termo4_detA =   x[0]*y1z2 + x[1]*y2z0 + x[2]*y0z1 - ( x[0]*y2z1 + x[1]*y0z2 + x[2]*y1z0 );

     // calculo de determinantes  -  calculo e feito para reduzir problemas com
     // subtracao de numeros muito pequenos: todos os termos positivos sao
     // somados, assim como os termos negativos e, ao final, sao feitas as
     // subtracoes.

    detA  = termo1_detA + termo2_detA + termo3_detA + termo4_detA;

         size = - detA;  // *AP* consultar caderno 4, pag.17
  }
  else size = 0;

}


Lagrange_Element_3D::Lagrange_Element_3D():Simplex_Nodal_Element_3D ()
{
	// FALTA INTEGRATION ORDER
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes)
    {
        Message(23004,(char*)("Lagrange_Element_3D default constructor"));
        exit (23004);
    }
    vizinhos = new long[4];
    if(!vizinhos)
    {
        Message(33006,(char*)("Lagrange_Element_3D default constructor"));
        exit (33006);
    }
}


Lagrange_Element_3D::Lagrange_Element_3D(int o):Simplex_Nodal_Element_3D(o)
{
	order = o;
	// FALTA INTEGRATION ORDER
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes)
    {
        Message(23004,(char*)("Lagrange_Element_3D default constructor"));
        exit (23004);
    }

    vizinhos = new long[4];
    if(!vizinhos)
    {
        Message(33006,(char*)("Lagrange_Element_3D default constructor"));
        exit (33006);
    }
}


void  Lagrange_Element_3D::Base_Function(Point  *point, lev_math::Te_Vector<double> & vetorN)
{
    short int err;
    double zeta1 = point->Get_X(),       // homogeneous coordinates
           zeta2 = point->Get_Y(),
           zeta3 = point->Get_Z(),
           zeta4 = 1.0 - zeta1 - zeta2 - zeta3;
    switch(order)
    {
        case  1 : // first order
            vetorN(1) = zeta1;
            vetorN(2) = zeta2;
            vetorN(3) = zeta3;
            vetorN(4) = zeta4;
            break;
        case  2 : // second order
            vetorN(1) = zeta1 * ( -1. + 2. * zeta1);
            vetorN(2) = zeta2 * ( -1. + 2. * zeta2);
            vetorN(3) = zeta3 * ( -1. + 2. * zeta3);
            vetorN(4) = zeta4 * ( -1. + 2. * zeta4);
            vetorN(5) = zeta1 * zeta2 * 4.;
            vetorN(6) = zeta1 * zeta3 * 4.;
            vetorN(7) = zeta1 * zeta4 * 4.;
            vetorN(8) = zeta2 * zeta3 * 4.;
            vetorN(9) = zeta2 * zeta4 * 4.;
            vetorN(10) = zeta3 * zeta4 * 4.;
            break;
        case  3 :
            err = 23002;
            Message (err , (char*)("Lagrange_Element_3D::Base_Function")); exit (err);
            break;
        default :
            err = 23003;
            Message (err ,(char*)( "Lagrange_Element_3D::Base_Function")); exit (err);
            break;
    } // end of switch(order)
} // end of  Lagrange_Element_3D::Base_Function


double  Lagrange_Element_3D::Base_Function(Point *point, short int qual)
{
  short int err;
    double zeta1 = point->Get_X(),       // homogeneous coordinates
              zeta2 = point->Get_Y(),
              zeta3 = point->Get_Z(),
              zeta4 = 1. - zeta1 - zeta2 - zeta3;
   double value;
   switch (order)
   {
   case   1:
      switch (qual)
      {
         case    1:   value = zeta1; break;
         case    2:   value = zeta2; break;
         case    3:   value = zeta3; break;
         case    4:   value = zeta4; break;
         default  :
          err = 23008;
             Message (err, (char*)("Base_Function")); exit (err);
          break;

      } // end of switch (qual) first order element
      break;

   case   2:     // second order
      switch (qual)
      {
          case   1: value = zeta1 * ( -1. + 2. * zeta1); break;
          case   2: value = zeta2 * ( -1. + 2. * zeta2); break;
          case   3: value = zeta3 * ( -1. + 2. * zeta3); break;
          case   4: value = zeta4 * ( -1. + 2. * zeta4); break;
          case   5: value = zeta1 * zeta2 * 4.; break;
          case   6: value = zeta1 * zeta3 * 4.; break;
          case   7: value = zeta1 * zeta4 * 4.; break;
          case   8: value = zeta2 * zeta3 * 4.; break;
          case   9: value = zeta2 * zeta4 * 4.; break;
          case  10: value = zeta3 * zeta4 * 4.; break;
          default  :
              err = 23008;
              Message (err, (char*)("Base_Function")); exit (err);
              break;
      }  // end of switch (qual)
    break;
   default :
     err = 23003;
       Message (err, (char*)("Base_Function")); exit (err);
     break;
   } // end of switch order
   return value;
} // end of  Lagrange_Element_3D::Base_Function


void   Lagrange_Element_3D::GradN (Point *point, lev_math::Te_Matrix<double> &DN)
{
    short int err;
    double zeta1 = point->Get_X(),       // homogeneous coordinates
           zeta2 = point->Get_Y(),
           zeta3 = point->Get_Z(),
           zeta4 = 1.0 - zeta1 - zeta2 - zeta3;
    switch(order) {
        case  1 : // first order
            DN(1,1) = 1.; DN(1,2) = 0.; DN(1,3) = 0.; DN(1,4) = -1.;
            DN(2,1) = 0.; DN(2,2) = 1.; DN(2,3) = 0.; DN(2,4) = -1.;
            DN(3,1) = 0.; DN(3,2) = 0.; DN(3,3) = 1.; DN(3,4) = -1.;
            break;
        case  2 : // second order
            DN(1,1) = -1. + 4.*zeta1;
            DN(1,2) = 0.;
            DN(1,3) = 0.;
            DN(1,4) = 1 - 4*zeta4;
            DN(1,5) = 4*zeta2;
            DN(1,6) = 4*zeta3;
            DN(1,7) = -4*zeta1 + 4*zeta4;
            DN(1,8) = 0.;
            DN(1,9) = -4*zeta2;
            DN(1,10) = -4*zeta3;

            DN(2,1) =  0.;
            DN(2,2) =  -1 + 4*zeta2;
            DN(2,3) =  0.;
            DN(2,4) =  1 - 4*zeta4;
            DN(2,5) =  4*zeta1;
            DN(2,6) =  0.;
            DN(2,7) =  -4*zeta1;
            DN(2,8) =  4*zeta3;
            DN(2,9) =  -4*zeta2 + 4*zeta4;
            DN(2,10) = -4*zeta3;

            DN(3,1) =  0.;
            DN(3,2) =  0.;
            DN(3,3) =  -1 + 4*zeta3;
            DN(3,4) =  1 - 4*zeta4;
            DN(3,5) =  0.;
            DN(3,6) =  4*zeta1;
            DN(3,7) =  -4*zeta1;
            DN(3,8) =  4*zeta2;
            DN(3,9) =  -4*zeta2;
            DN(3,10) = 4*zeta4 - 4*zeta3;
            break;
        case  3 :
            err = 23002;
            Message (err , (char*)("Lagrange_Element_3D::GradN")); exit (err);
            break;
        default :
            err = 23003;
            Message (err , (char*)("Lagrange_Element_3D::GradN")); exit (err);
            break;
    } // end of switch(order)
    DN = jac*DN; // Convert to Real Coordinates
} // end of Lagrange_Element_3D:: GradN


//==============================================================================
void   Lagrange_Element_3D::DivN  (Point *point, lev_math::Te_Matrix<double> &divN)
        {Message(23000, (char*)("Lagrange_Element_3D::  DivN"));  }
//==============================================================================
void   Lagrange_Element_3D::RotN  (Point *point, lev_math::Te_Matrix<double> &rotN)
        {Message(23000, (char*)("Lagrange_Element_3D::  RotN")); }
//==============================================================================



Hermite_Element_3D::Hermite_Element_3D():Simplex_Nodal_Element_3D ()
{
	// FALTA INTEGRATION ORDER
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes)
    {
        Message(23004,(char*)("Hermite_Element_3D default constructor"));
        exit (23004);
    }
    vizinhos = new long[4];
    if(!vizinhos)
    {
        Message(33006,(char*)("Hermite_Element_3D default constructor"));
        exit (33006);
    }
}


Hermite_Element_3D::Hermite_Element_3D(int o):Simplex_Nodal_Element_3D(o)
{
	// FALTA INTEGRATION ORDER
	order = o;
    n_nodes = Get_N_Nodes();
    nodes = new long int [n_nodes];
    if (!nodes)
    {
        Message(23004,(char*)("Hermite_Element_3D default constructor"));
        exit (23004);
    }

    vizinhos = new long[4];
    if(!vizinhos)
    {
        Message(33006,(char*)("Hermite_Element_3D default constructor"));
        exit (33006);
    }
}


void  Hermite_Element_3D::Base_Function(Point  *point, lev_math::Te_Vector<double> & vetorN)
{
    short int err;
    double zeta1 = point->Get_X(),       // homogeneous coordinates
           zeta2 = point->Get_Y(),
           zeta3 = point->Get_Z(),
           zeta4 = 1. - zeta1 - zeta2 - zeta3;
    switch(order)
    {
        case  1 : // first order
            vetorN(1) = zeta1; vetorN(2) = zeta2;
            vetorN(3) = zeta3; vetorN(4) = zeta4;
            break;
        case  2 : // second order
            vetorN(1) = zeta1 * ( -1. + 2. * zeta1); vetorN(2) = zeta2 * ( -1. + 2. * zeta2);
            vetorN(3) = zeta3 * ( -1. + 2. * zeta3); vetorN(4) = zeta4 * ( -1. + 2. * zeta4);
            vetorN(5) = zeta1 * zeta2 * 4.; vetorN(6) = zeta1 * zeta3 * 4.;
            vetorN(7) = zeta1 * zeta4 * 4.; vetorN(8) = zeta2 * zeta3 * 4.;
            vetorN(9) = zeta2 * zeta4 * 4.; vetorN(10) = zeta3 * zeta4 * 4.;
            break;
        case  3 :
            err = 23002;
      Message (err , (char*)("Simplex_Nodal_Element_3D::Base_Function")); exit (err);
            break;
        default :
      err = 23003;
            Message (err ,(char*)( "Simplex_Nodal_Element_3D::Base_Function")); exit (err);
            break;

    } // end of switch(order)

} // end of  Hermite_Element_3D::Base_Function


double  Hermite_Element_3D::Base_Function(Point *point, short int qual)
{
  short int err;
    double zeta1 = point->Get_X(),       // homogeneous coordinates
              zeta2 = point->Get_Y(),
              zeta3 = point->Get_Z(),
              zeta4 = 1. - zeta1 - zeta2 - zeta3;
   double value;
   switch (order)
   {
   case   1:
      switch (qual)
      {
         case    1:   value = zeta1; break;
         case    2:   value = zeta2; break;
         case    3:   value = zeta3; break;
         case    4:   value = zeta4; break;
         default  :
          err = 23008;
             Message (err, (char*)("Base_Function")); exit (err);
          break;

      } // end of switch (qual) first order element
      break;

   case   2:     // second order
      switch (qual)
      {
               case   1: value = zeta1 * ( -1. + 2. * zeta1); break;
        case   2: value = zeta2 * ( -1. + 2. * zeta2); break;
              case   3: value = zeta3 * ( -1. + 2. * zeta3); break;
        case   4: value = zeta4 * ( -1. + 2. * zeta4); break;
              case   5: value = zeta1 * zeta2 * 4.; break;
        case   6: value = zeta1 * zeta3 * 4.; break;
              case   7: value = zeta1 * zeta4 * 4.; break;
        case   8: value = zeta2 * zeta3 * 4.; break;
              case   9: value = zeta2 * zeta4 * 4.; break;
        case  10: value = zeta3 * zeta4 * 4.; break;
        default  :
          err = 23008;
             Message (err, (char*)("Base_Function")); exit (err);
          break;
      }  // end of switch (qual)
    break;
   default :
     err = 23003;
       Message (err, (char*)("Base_Function")); exit (err);
     break;
   } // end of switch order
   return value;
} // end of  Hermite_Element_3D::Base_Function


// =============================================================================
void   Hermite_Element_3D::GradN (Point *point, lev_math::Te_Matrix<double> &DN)
{
  short int err;
    double zeta1 = point->Get_X(),       // homogeneous coordinates
             zeta2 = point->Get_Y(),
             zeta3 = point->Get_Z(),
             zeta4 = 1. - zeta1 - zeta2 - zeta3;
    switch(order)
    {
        case  1 : // first order
            DN(1,1) = 1.; DN(1,2) = 0.; DN(1,3) = 0.; DN(1,4) = -1.;
            DN(2,1) = 0.; DN(2,2) = 1.; DN(2,3) = 0.; DN(2,4) = -1.;
            DN(3,1) = 0.; DN(3,2) = 0.; DN(3,3) = 1.; DN(3,4) = -1.;
            break;
        case  2 : // second order
      DN(1,1) = -1. + 4.*zeta1;
      DN(1,2) = 0.;
      DN(1,3) = 0.;
      DN(1,4) = 1 - 4*zeta4;
            DN(1,5) = 4*zeta2;
      DN(1,6) = 4*zeta3;
      DN(1,7) = -4*zeta1 + 4*zeta4;
      DN(1,8) = 0.;
      DN(1,9) = -4*zeta2;
      DN(1,10) = -4*zeta3;

            DN(2,1) =  0.;
      DN(2,2) =  -1 + 4*zeta2;
      DN(2,3) =  0.;
      DN(2,4) =  1 - 4*zeta4;
            DN(2,5) =  4*zeta1;
      DN(2,6) =  0.;
      DN(2,7) =  -4*zeta1;
      DN(2,8) =  4*zeta3;
      DN(2,9) =  -4*zeta2 + 4*zeta4;
      DN(2,10) = -4*zeta3;

            DN(3,1) =  0.;
      DN(3,2) =  0.;
      DN(3,3) =  -1 + 4*zeta3;
      DN(3,4) =  1 - 4*zeta4;
            DN(3,5) =  0.;
      DN(3,6) =  4*zeta1;
      DN(3,7) =  -4*zeta1;
      DN(3,8) =  4*zeta2;
      DN(3,9) =  -4*zeta2;
      DN(3,10) = 4*zeta4 - 4*zeta3;
            break;
        case  3 :
      err = 23002;
            Message (err , (char*)("Simplex_Nodal_Element_3D::GradN")); exit (err);
            break;
        default :
      err = 23003;
            Message (err , (char*)("Simplex_Nodal_Element_3D::GradN")); exit (err);
            break;

    } // end of switch(order)

} // end of Hermite_Element_3D:: GradN


//==============================================================================
void   Hermite_Element_3D::DivN  (Point *point, lev_math::Te_Matrix<double> &divN)
        {Message(23000, (char*)("Hermite_Element_3D::  DivN"));  }
//==============================================================================
void   Hermite_Element_3D::RotN  (Point *point, lev_math::Te_Matrix<double> &rotN)
        {Message(23000, (char*)("Hermite_Element_3D::  RotN")); }
//==============================================================================
