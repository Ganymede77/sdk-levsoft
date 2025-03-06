#ifndef __PROJECTMODEL__
#define __PROJECTMODEL__

#include <iostream>
#include <fstream>
#include "project_solver.h"
#include "levenum.h"

/**
 * Projeto: SDK-LEVSOFT
 * namespace: lev
 * 
 * Esta classe provê funções para gravação e leitura de dados de modelagem geométrica e física.
 * 
 * 
 */

class CLevGeoData;
class CLevPhysData;

namespace lev
{

struct keyandvalue
{
	std::string item;
	std::string content;
};

struct phen_struct
{
	std::string phenomenon_name;
	std::vector<keyandvalue> userdatasettings; ///< dados de especificacao definidos pelo usuario
};

class CProject_Model : public CProject_Base
{

protected:

    std::string     sCompanyName;
    
    CLevGeoData     *pModel;            ///< ponteiro para objeto de modelo geometrico
    CLevPhysData    *pPhysData;         ///< ponteiro para objeto de modelo fisico
    
    //arquivos de especificacao
    bool             b_readPREPROJ,b_savePREPROJ; ///< flags para gravacao e leitura do arquivo de especificacao preliminar .preproj
    bool             b_readDADOS,b_saveDADOS; ///< flags para gravacao e leitura do arquivo de especificacao temporario .dados (s ser substituido pelo .preproj)
  
    //arquivos de modelo geometrico
    bool             b_readMGP, b_saveMGP; ///< flags para gravacao e leitura do arquivo de primitivas .mgp
    bool             b_readPRX, b_savePRX; ///< flags para gravacao e leitura do arquivo de primitivas AUXILIARES .PRX
    bool             b_readRGN, b_saveRGN; ///< flags para gravacao e leitura do arquivo de regioes do model geometrico .rgn
    bool             b_readMMF, b_saveMMF; ///< flags para gravacao e leitura do arquivo de malha de fronteira auxiliar as regioes do model geometrico .mmf
    
    //arquivos de modelo fisico
    bool             b_readMEI, b_saveMEI; ///< flags para gravacao e leitura do arquivo de banco de dados de materiais .mei
    bool             b_readMTS, b_saveMTS; ///< flags para gravacao e leitura do arquivo de banco de dados de materiais em uso .mts (o indice do material esta associado aa regiao do modelo)

    bool             b_readSRC, b_saveSRC; ///< flags para gravacao e leitura do arquivo de source em uso .src (o indice da fonte esta associado aa regiao do modelo)
    
    bool             b_readBCO, b_saveBCO; ///< flags para gravacao e leitura do arquivo de condicoes de contorno .bco
    bool             b_readBCP, b_saveBCP; ///< flags para gravacao e leitura do arquivo de primitivas com condicoes de contorno .bcp

	int				readPRJ, savedPRJ;


	std::vector<keyandvalue> 	userdatasettings; ///< dados de especificacao definidos pelo usuario
	std::vector<phen_struct> 	vphen_userdatasettings; ///< dados de especificacao definidos pelo usuario para cada fenomeno do estudo
	std::string		 			str_study_name; ///< nome do estudo (ex: electrostatic, thermooptic, etc, pode ter um mais fenômenos acoplados)

	//variaveis de controle de leitura ou gravacaoo dos arquivos

	//------------------------------------------------------------------------------------------------------------------------------------
	//---------------------------------- configuracao do sistema levsoft -----------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------------------------------
	std::string					str_solver_executable_file;///< nome e path do arquivo executavel do solver
	std::vector<std::string>	vtr_solver_executable_file_list;///< lista de nomes e paths do arquivo executavel do solver

	std::string					str_explorer_executable_file;///< nome e path do arquivo executavel do explorador
	std::vector<std::string>	vtr_explorer_executable_file_list;///< lista de nomes e paths do arquivo executavel do explorador

	void						UpdateSettingsfromPhenSpecification(void);
    
public:

	CProject_Model();						// Construtor
	~CProject_Model();						// Destructor

	CProject_Model(const CProject_Model &other);
	CProject_Model& operator=(const CProject_Model &other);

    void Associate(CLevGeoData *p) {pModel = p;}         ///< Associa ponteiro para objeto de modelo geometrico
    void Associate(CLevPhysData *p) {pPhysData = p;}     ///< Associa ponteiro para objeto de modelo físico

    void            		BuildArchivesNames(std::ofstream &, int withPath= 1);	///< Monta nome dos arquivos de dados a partir do nome do projeto
        	std::string		GetCompanyName(void);		// Devolve o nome do projetista
	virtual int             GetPhenomenonEnum(void);        ///< Devolve tipo de fenomeno
    virtual int             GetSymmetryEnum(void);		// Devolve tipo de simetria geometrica
	virtual int             GetType_of_MeshEnum(void);		///< Devolve tipo de malha como enum
	virtual int             GetUnit_of_AngleEnum(void);		///< Devolve unidade de tempo como enum
	virtual int             GetUnit_of_LengthEnum(void);	///< Devolve unidade de comprimento como enum
	virtual int             GetUnit_of_MassEnum(void);		///< Devolve unidade de massa como enum
	virtual int             GetUnit_of_TimeEnum(void);		///< Devolve unidade de tempo como enum
	virtual int             GetUnit_of_TemperatureEnum(void);		///< Devolve unidade de temperatura como enum
            std::string     GetUnit_of_LengthName(void);	// Devolve o "nome" da unidade de medida no argumento
    
        	std::string             GetUnit_of_AngleName(void);		///< Devolve o "nome" da unidade de angulo

        	void					Set_Item(std::string &item,std::string &content); ///< Inclui "nome" e "especificacao" da "variavel ou configuracao" no vetor usedatasettings (ex: (length, milimeter))
        	void					Get_Item(std::string &item,std::string &content);
        	void					Get_Item(int index,std::string &item_name);
        	void					Get_Content(int index,std::string &content_name);
        	int						Get_NumberofItens(void);

	virtual void            SetCompanyName(std::string);	///< Atribui nome da empresa ou instituicao
	virtual void            SetMeshType(int);               ///< Atribui tipo de geracao de malha	
    virtual void            SetSymmetryType(int);           ///< Atribui tipo de simetria geometrica    
    virtual void            SetPhenomenonType(int);         ///< Atribui tipo de fenomeno
	virtual void            SetUnit_of_Angle(int);			///< Atribui unidade de angulo
	virtual void            SetUnit_of_Length(int);         ///< Atribui unidade de comprimento
	virtual void            SetUnit_of_Mass(int);           ///< Atribui unidade de massa
	virtual void            SetUnit_of_Temperature(int);    ///< Atribui unidade de temperature
	virtual void            SetUnit_of_Time(int);           ///< Atribui unidade de tempo

	void					Set_UserDataSettings(std::vector<phen_struct>); ///< Atribui dados de especificacao definidos pelo usuario

	void					SetStudyName(std::string n); ///< Atribui nome do estudo
    std::string				GetStudyName(void ); ///< Devolve nome do estudo

    void					InitializeUserDataSettings(void); ///< Inicializa dados de especificacao caso a leitura nao tenha sido feita....
    
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Leitura e gravação   
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::    
    virtual int             Read(void);                     ///< Chama funcoes de gravacao dos dados de projeto, dos modelos geometrico e fisico
    virtual int             ReadPREPRJ(void);               ///< Le dados do projeto em formato livre mas com "tags"
    virtual int             ReadPREPRJv001(void);           ///< Le dados do projeto em formato livre mas com "tags"

	virtual int				ReadPRJ(std::string name);		///< Leitura do arquivo PRJ em XML
	virtual int				SavePRJ(std::string name);		///< Gravação do arquivo PRJ em XML (//*NMA*2016-06-07: descontinuado, usar SaveUserDataSettings )

	virtual int				SaveUserDataSettings(std::string ); ///< Gravação dos dados de especificacao definidos pelo usuário em padrão XML (diferente do formato usado em SavePRJ)

	virtual int				SaveSpecDataFile(std::string ); ///<Gravacao do arquivo de especificacao preliminar (*.dados)
    
    virtual int             ReadGeometricModel(void);         ///< Le dados do modelo geometrico 
    virtual int             ReadPhysicalModel(void);         ///< Le dados do modelo fisico 
    
    virtual int             Save(void);                     ///< Chama funcoes de gravacao dos dados de projeto, dos modelos geometrico e fisico
    virtual int             SavePREPRJ(void);               ///< Grava dados do projeto em formato livre mas com "tags"
    virtual  int            SaveGeoPhysModel(void);         ///< Grava dados dos modelos geometrico e fisico
    
    virtual short 			SaveProj(PHENOMENONTYPE phen=SCALAROPTICALWAVE); //obs:implementacao do roberto temporaria

    virtual short 	    	ReadProj01(void); ///<leitura de arquivo binario .proj para compatibilidade com arquivos gerados em versoes anteriores
    virtual short 	    	ReadProj(void); ///<leitura de arquivo binario .proj para compatibilidade com arquivos gerados em versoes anteriores
    virtual short           SaveProj(int qualquer); ///<gravacao de arquivo binario .proj para compatibilidade com arquivos gerados em versoes anteriores

     int                	ReadBinaryInt(std::ifstream &file);
     std::string        	ReadBinaryString(std::ifstream &file);

    
    void            SetFlagsReadSavePREPROJ(bool, bool); ///< flags para gravacao e leitura do arquivo de especificacao preliminar .preproj
    void            SetFlagsReadSaveDADOS(bool, bool); ///< flags para gravacao e leitura do arquivo de especificacao temporario .dados (s ser substituido pelo .preproj)
  
    //arquivos de modelo geometrico
    void            SetFlagsReadSaveMGP(bool, bool); ///< flags para gravacao e leitura do arquivo de primitivas .mgp
    void            		SetFlagsReadSavePRX(bool, bool); ///< flags para gravacao e leitura do arquivo de primitivas AUXILIARES .prx
    
    void            SetFlagsReadSaveRGN(bool, bool); ///< flags para gravacao e leitura do arquivo de regioes do model geometrico .rgn
    void            SetFlagsReadSaveMMF(bool, bool); ///< flags para gravacao e leitura do arquivo de malha de fronteira auxiliar as regioes do model geometrico .mmf
    
    //arquivos de modelo fisico
    void            SetFlagsReadSaveMEI(bool, bool); ///< flags para gravacao e leitura do arquivo de banco de dados de materiais .mei
    void            SetFlagsReadSaveMTS(bool, bool); ///< flags para gravacao e leitura do arquivo de banco de dados de materiais em uso .mts (o indice do material esta associado aa regiao do modelo)

    void            SetFlagsReadSaveSRC(bool, bool); ///< flags para gravacao e leitura do arquivo de source em uso .src (o indice da fonte esta associado aa regiao do modelo)
    
    void            SetFlagsReadSaveBCO(bool, bool); ///< flags para gravacao e leitura do arquivo de condicoes de contorno .bco
    void            SetFlagsReadSaveBCP(bool, bool); ///< flags para gravacao e leitura do arquivo de primitivas com condicoes de contorno .bcp

	//------------------------------------------------------------------------------------------------------------------------------------
	//---------------------------------- configuracao do sistema levsoft -----------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------------------------------

    std::string					Get_SolverFileName(void);			///< Obtem nome do arquivo executavel do solver (a ser chamado pela opcao de menu)
    void						Set_SolverFileName(std::string);	///< Atribui nome do arquivo executavel do solver

    std::vector<std::string>	Get_SolverFileNameList(void); ///< Obtem lista dos arquivos executaveis do solver
    void						Set_SolverFileNameList(std::vector<std::string>); ///< Atribui lista dos arquivos executaveis do solver

    std::string					Get_ExplorerFileName(void);			///< Obtem nome do arquivo executavel do explorador (a ser chamado pela opcao de menu)
    void						Set_ExplorerFileName(std::string);	///< Atribui nome do arquivo executavel do explorador

    std::vector<std::string>	Get_ExplorerFileNameList(void); ///< Obtem lista dos arquivos executaveis do explorador
    void						Set_ExplorerFileNameList(std::vector<std::string>); ///< Atribui lista dos arquivos executaveis do explorador

    bool						Save_CfgExeFilesXML(std::string filename);
};

}


#endif
