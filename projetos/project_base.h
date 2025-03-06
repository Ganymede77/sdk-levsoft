#ifndef CPROJECTBASEV1_H
#define CPROJECTBASEV1_H


#include "levenum.h"
#include "project_names_builder_v1.h"
#include "message_err.h"
#include "phenspec.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>


namespace lev
{
//.............................................................................
// Implementado por: Nancy Mieko Abe, *NMA*
// Data: 18/05/2006
//............................................................................
/**
 * Projeto: SDK-LEVSOFT
 *
 * O objetivo da classe CProject_Base � armazenar e fornecer as especifica��es de um projeto.
 * As seguintes informa��es podem ser armazenadas:
 * - descri��o do projeto;
 * - nome do estudo e especifica��o de cada fen�meno que o comp�e
 * Para estudos simples (que n�o apresentam acoplamento entre dois ou mais fenomenos,
 * o nome do estudo e do fenomeno sao iguais).
 */
class CProject_Base
{
 protected:
	std::string sDesignerName;					///< string que armazena o nome do designer (projetista)
	std::string sDescription;					///< string que armazena descri��o do projeto
	std::string sDateOfCreation;				///< string que armazena data da cria��o do projeto
	std::string sDateLastModifiedIn;			///< string que armazena data da �ltima modifica��o
	std::string	sProjectDimension;				///< string que armazena a dimens�o adotada para a simula��o (one_dimensional, two_dimensional, three_dimensional)
	int			iProjectDimension;
	std::string sProjectSymmetry;				///< string que armazena a simetria adotada para simula��o

	std::string sProjectName;					///< string que armazena o nome do projeto SEM path e SEM extens�o
	std::string sProjectNameWithPath;			///< string que armazena o nome do projeto com path e sem extens�o

	double		dProjectVersion;				///< double que armazena um numero que est� associado a vers�o de arquivos


//*NMA*: 13/03/2006: colocar as associa��es com as classes de estudo e especifica��o de fen�meno
//		 no pr�ximo passo
// String que armazena o nome do estudo
// Vetor que armazena informa��es sobre cada fenomeno

	std::vector<lev::CLevMessage> vMessageVector;

	DATASTATUS		XML_PROJECT_STATUS;

	int				current_phenomenon_index;	///< armazena o indice do fen�meno corrente no vetor vpPhenSpecificationVector
    std::string trim(std::string& str);
    std::string zeroPadNumber(long int num, int length);
    std::string zeroPadNumber(double num);

	public:

    	std::vector<lev::CPhenSpecBase *> vpPhenSpecificationVector;///< armazena ponteiros para objetos de especificação de fenômeno
    	CProject_Name_Builder_v1 o_NameBuilder_v1;		///< objeto para constru��o de nomes de arquivos vers�o 1

    	CProject_Base() {current_phenomenon_index=-1; dProjectVersion = 1.0;} ///< Constructor

		virtual ~CProject_Base()   ///< Destructor
		{
			current_phenomenon_index = -1;
			vMessageVector.clear();
			CleanPhenSpecificationVector();
		}

		void CleanPhenSpecificationVector(void)
		{
			if(vpPhenSpecificationVector.size())
				for(unsigned long i=0;i<vpPhenSpecificationVector.size();i++)
					if(vpPhenSpecificationVector[i])
						delete vpPhenSpecificationVector[i];
			vpPhenSpecificationVector.clear();
		}

		CProject_Base & operator = (const CProject_Base &p)		///< Opera��o de atribui��o
		{
			sDesignerName		= p.sDesignerName;
			sDescription		= p.sDescription;
			sDateOfCreation		= p.sDateOfCreation;
			sDateLastModifiedIn = p.sDateLastModifiedIn;
			sProjectDimension	= p.sProjectDimension;
			sProjectName		= p.sProjectName;
			sProjectSymmetry	= p.sProjectSymmetry;
			dProjectVersion		= p.dProjectVersion;
			vMessageVector		= p.vMessageVector;
			return *this;
		}

		virtual int			ReadPRJ(std::string name);///< Leitura do arquivo PRJ em XML
		virtual int			SavePRJ(std::string name){return 100;};///< Grava��o do arquivo PRJ em XML

		virtual int			ReadSpecDataFile(std::string );///<Leitura do arquivo de especifica��o preliminar (*.dados)

		inline	DATASTATUS	GetPRJXMLFileStatus(void){return XML_PROJECT_STATUS;}///< Opera��o que retorna o status do arquivo PRJ em formato XML

		inline void			DesignerName(std::string name) {sDesignerName = name;}	///< Opera��o que atribui nome do projetista
		inline std::string	DesignerName(void) {return sDesignerName;}	///< Opera��o que retorna nome do projetista

		inline void			DateOfCreation(std::string date){sDateOfCreation = date;} ///< Opera��o que atribui data de cria��o projeto
		inline std::string	DateOfCreation(void){return sDateOfCreation;}	///< Opera��o que retorna data de cria��o do projeto

		inline void			DateLastModifiedIn(std::string date){sDateLastModifiedIn = date;}	///< Opera��o que atribui data da �ltima altera��o do projeto
		inline std::string	DateLastModifiedIn(void){return sDateLastModifiedIn;}	///< Opera��o que retorna data da �ltima altera��o do projeto

		inline void			Description(std::string descrip){sDescription = descrip;}	///< Opera��o que atribui descri��o do projeto
		inline std::string	Description(void){return sDescription;}	///< Opera��o que retorna descri��o do projeto

		inline void			Symmetry(std::string symm){sProjectSymmetry = symm;}	///< Opera��o que atribui simetria do projeto
		inline std::string	Symmetry(void){return sProjectSymmetry;}	///< Opera��o que retorna simetria do projeto

		inline void			Dimension(std::string dim) 	///< Opera��o que atribui dimens�o adotada para o projeto
							{
								sProjectDimension = dim;
								if (sProjectDimension == "one_dimensional")       iProjectDimension = 1;
								else if (sProjectDimension == "two_dimensional")  iProjectDimension = 2;
								else if (sProjectDimension == "three_dimensional")iProjectDimension = 3;
							}
		inline std::string	Dimension(void){return sProjectDimension;}	///< Opera��o que retorna dimens�o adotada para o projeto
		inline int			IntDimension(void){ return iProjectDimension;}

		inline void			Version(double ver) {dProjectVersion = ver;} ///< Atribui vers�o associada a formato de arquivos
		inline double		Version(void) {return dProjectVersion;}	///< Retorna vers�o associada a formato de arquivos

		inline void			ProjectNameWithPath(std::string name)
							{
								sProjectNameWithPath = name;
								BuildFileNames();
							} ///< Fun��o para atribui��o do nome do projeto com path e sem extens�o
		inline std::string	ProjectNameWithPath(void) {return sProjectNameWithPath;} ///< Fun��o que retorna do nome do projeto com path e sem extens�o

		inline std::string	ProjectName(void) {return sProjectName;} ///< Retorna do nome do projeto sem path e sem extens�o. O nome do projeto � obtido a partir do nome fornecido com path.

		virtual std::string GetFileName(enum FILENAME); ///< Retorna o nome do arquivo associado ao enum FILENAME dado

		virtual int			BuildFileNames(void) ; ///< Constroi o nome do arquivos
		virtual int			BuildFileNamesMeshBased(void);

		void				SetErrorMessage(lev::CLevMessage mdata);///< Inclui mensagem no vetor de mensagens gerados pelos diversos m�dulos

		lev::CLevMessage	GetErrorMessageByCode(int codigo);///< Retorna a mensagem associada ao c�digo fornecido

		lev::CLevMessage	GetErrorMessageByIndex(int index);///< Retorna a mensagem associada ao �ndice no vetor


		int					SaveErrorLog(void);///< Grava��o (append) do vetor de mensagens no arquivo nome_projeto.errorlog
		void				EraseErrorLog(void);///< Apaga arquivo nome_projeto.errorlog para come�ar de novo


		friend	std::ostream & operator <<(std::ostream & p, const CProject_Base & s); ///< Operador para imprimir na tela os dados do projeto

		virtual int			CreateAndIncludePhenSpecification(void);///<  aloca e inclui no vetor de ponteiros para objetos de especifica��o de fen�meno ponteiro para classe CPhenSpecBase

		inline int			SetCurrentPhenIndex(unsigned int ind)
		{
			if((ind < vpPhenSpecificationVector.size()) && (ind > 0)) {
				current_phenomenon_index = ind;
				return ind;
			} else
				return -1;
		}

		int	GetCurrentPhenIndex(void);

};//end of class CProject_Base


}//fecha namespace lev
#endif //CPROJECTBASEV1_H
