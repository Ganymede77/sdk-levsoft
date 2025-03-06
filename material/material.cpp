#include "material.hpp"
#include <string.h>

// =============================================================================
//   Abstract class Material
// =============================================================================
short int Material :: numberof = 0;
double    Material :: vacuum_permittivity = 8.854187817e-12; // farad/m
double    Material :: vacuum_permeability = 1.25663706e-6; // henry/m

Material :: Material()
{
	ident = ++numberof;
	code = 0;
	angle = 0.; // so tem sentido para imas
}

// =============================================================================
//  Abstract Class Material_Ground
// =============================================================================
Material_Ground :: Material_Ground() : Material()
{ }


Material_Ground :: ~Material_Ground() 
{
	if(physical_dimensions) {
		delete [] physical_dimensions;
		physical_dimensions = NULL;
	}
}


short int Material_Ground :: Relevant_Sizes(double * dim)
{
	for (int i = 0; i < num_dimensions; i++)
		dim[i] = physical_dimensions[i];
	return num_dimensions;
}


//=============================================================================
// Mat_Condutor :: Mat_Conductor
/*-----------------------------------------------------------------------------
  Description: .
---------------------------------------------------------------------------- */
Mat_Conductor :: Mat_Conductor() : Material_Ground()
{
	num_dimensions = 1;
	physical_dimensions = new double[num_dimensions];
	if (!physical_dimensions)
	{
		std::cout << "\n Insufficient memory: Constructor material_condutor";
		exit (0);
	}
	for (int i = 0; i < num_dimensions; i++ )
		physical_dimensions[i] = 0;
}


//=============================================================================
// Mat_Condutor :: Recover
/*-----------------------------------------------------------------------------
  Description: Reads the material data from the data base file.
---------------------------------------------------------------------------- */
short int Mat_Conductor :: Recover(FILE *fp_file, short int mat_id, short int ver)
{
	short int continua = 1;
	short int ret      = 0;
	int       retor    = 0;
	if (fp_file) {
		rewind (fp_file);
		if (ver)  // data base version
		{      // original version
			short int aux1, aux2;
			int ch;
			double    aux3, aux4, aux5, aux6, aux7;
			char      name_id[81] = "";
			do {
				// ordem de leitura : nome id mat_type cross_section resistivity temperature line.coef tf
				ch = getc(fp_file);
				if((fgets(&name_id[0],40,fp_file))) {
					retor = fscanf(fp_file,"%hd %hd %lg %lg %lg %lg %lg",
							&aux1, &aux2, &aux3, &aux4, &aux5, &aux6, &aux7);
					switch( retor) {
						case EOF:
							continua = 0;
							ret = 22000;
							break;
						case 0:
							continua = 0;
							ret = 22001;
							break;
						case 7:
							ret = 0;
							break;
						default:
							continua = 0;
							ret = 22002;
							break;
					}
					if (continua) { // verify  mat_id
						if (mat_id == aux1) { // material identified
							continua = 0;
							conductivity = 1./aux4;
							strcpy(name, name_id);
							physical_dimensions[0] = aux3 * 1e-6;   //transforms  mm2 to m2
						}
					}
				} else {
					continua = 0;
					ret = 22001;
				}
			} while (continua);
		} else { // version 1 : Static and Magnetodynamic
			short int aux1, aux2;
			int ch;
			double    aux3, aux4, aux5, aux6, aux7, aux8, aux9;
			char      name_id[81] = "";
			do {
				// ordem de leitura : nome id mat_type cross_section resistivity temperature line.coef tf
				ch = getc(fp_file);
				if((fgets(&name_id[0],40,fp_file))) {
					retor = fscanf(fp_file,"%hd %hd %lg %lg %lg %lg %lg %lg %lg",
							&aux1, &aux2, &aux3, &aux4, &aux5, &aux6, &aux7, &aux8, &aux9);
					switch( retor) {
						case EOF:
							continua = 0;
							ret = 22000;
							break;
						case 0:
							continua = 0;
							ret = 22001;
							break;
						case 9:
							ret = 0;
							break;
						default:
							continua = 0;
							ret = 22002;
							break;
					} // end of switch(ret)
					if (continua) { // verify  mat_id
						if (mat_id == aux1) { // material identified
							continua = 0;
							conductivity = 1./aux4;
							permittivity = vacuum_permittivity * aux8;
							permeability = vacuum_permeability * aux9;
							strcpy(name, name_id);
							physical_dimensions[0] = aux3 * 1e-6;   //transforms  mm2 to m2
						}
					}
				} else {
					continua = 0;
					ret = 22001;
				}
			} while (continua);
		}
	}
	return ret;
}


//=============================================================================
// Mat_Soil :: Mat_Soil
/*-----------------------------------------------------------------------------
  Description: .

---------------------------------------------------------------------------- */
Mat_Soil :: Mat_Soil() : Material_Ground()
{
	num_dimensions = 2;
	physical_dimensions = new double[num_dimensions];
	if (!physical_dimensions) {
		std::cout << "\n Insufficient memory: Constructor material_Soil";
		exit (0);
	}
	for (int i = 0; i < num_dimensions; i++ )
		physical_dimensions[i] = 0;
}


//=============================================================================
// Mat_Soil :: Recover
/*-----------------------------------------------------------------------------
  Description: Reads the material data from the material file.

---------------------------------------------------------------------------- */
short int Mat_Soil :: Recover(FILE *fp_file,short int mat_id, short int ver)
{
   short int continua = 1;
   short int ret      = 0;
   int       retor    = 0;
   if (fp_file)
   {
      if (ver)  // data base version
      {      // original version
         double    aux1, aux2, aux3;
         char      name_id[81];
         do{
            // ordem de leitura : name resistivity Zi Zf
            retor = fscanf(fp_file,"%s %lg %lg %lg",
                  &name_id[0], &aux1, &aux2, &aux3);
            switch( retor)
            {
               case EOF :
                  continua = 0; ret = 22000; break;
               case 0   :
                  continua = 0; ret = 22001; break;
               case 4   :
                  ret = 0; break;
               default  :
                  continua = 0; ret = 22002; break;
            } // end of switch(ret)
            if (continua)  // verify  mat_id
            {
               continua = 0;
               conductivity = 1./aux1;
               strcpy(name, name_id);
               physical_dimensions[0] = aux2;
               physical_dimensions[1] = aux3;
            }
         } while (continua);
      }
      else  // version 1 : Static and Magnetodynamic
      {
         double    aux1, aux2, aux3, aux4, aux5;
         char      name_id[81];
         do{
            // ordem de leitura : name resistivity relative_permittivity relative_permeability Zi Zf
            retor = fscanf(fp_file,"%s %lg %lg %lg %lg %lg",
                  &name_id[0], &aux1, &aux2, &aux3, &aux4, &aux5);
            switch( retor)
            {
               case EOF :
                  continua = 0; ret = 22000; break;
               case 0   :
                  continua = 0; ret = 22001; break;
               case 6   :
                  ret = 0; break;
               default  :
                  continua = 0; ret = 22002; break;
            } // end of switch(ret)
            if (continua)  // verify  mat_id
            {
               continua = 0;
               conductivity = 1./aux1;
               permittivity = vacuum_permittivity * aux2;
               permeability = vacuum_permeability * aux3;
               strcpy(name, name_id);
               physical_dimensions[0] = aux4;
               physical_dimensions[1] = aux5;
            }
         } while (continua);
      } // end of if (ver)
   } // end of if (fp_file)
   return ret;
} // end of Material :: Recover


char * Material :: Message(short int err, char * mess)
{
	char message[300] = "";
	char *auxmess = mess;
	if (!mess)
	auxmess = &message[0];
	char *aux;
	strcpy (auxmess, "Error ");

	std::stringstream ss;
	ss << err;
	aux = const_cast<char*>(ss.str().c_str());
	strcat (auxmess, aux);
	switch (err)
	{
		case 22000 :      // Recover
			strcat (auxmess, " Material: EOF encountered!");
			break;
		case 22001 :     // Recover
			strcat (auxmess, " Material : unable to read the material or material data base file");
			break;
		case 22002 :     // Recover
			strcat (auxmess, " Material : inconsistent data file");
			break;
		default :
			strcat (auxmess, " Material : unknown error number");
	}
	std::cout << auxmess;
	return mess;
}   // end of Material::Message


bool Material :: Property_Value(Point & p,char * nom, double *value)
{
	// *AP* nesta implementacao a propriedade ainda nao varia com a posicao (homogeneo)
	int i;
	bool found = false;
	for ( i = 0; i < n_properties && !found; i++)
	{
		if (!strcmp(nom, Phys_Prop_Name[i]))
		{
			*value = Physical_Property [i];
			found = true;
		}
	}
	return found;
}  // end of Material :: Property_Value


double Material :: Property_Value(Point &p)
{
	return Physical_Property [0];
}


//============================================================================
// retorna false se a propriedade nao puder ser incluida
bool Material :: Set_Property(double value, char *nom)
{
	bool found = false;
	int i;

// testa se propriedade ja existe
	for (i = 0; i < n_properties && !found; i++)
	{
		if (!strcmp(nom, Phys_Prop_Name[i]))
		{
			Physical_Property [i] = value;
			found = true;
		}
	}

// atribui nova propriedade
	if (n_properties < TAM_PHYS && !found)
	{
		Physical_Property [n_properties] = value;
		n_properties++;
		found = true;
	}
	return found;
}  // end of Material :: Set_Property

