#include <ctime>
#include <iomanip>
#include <limits>
#include "project_model.h"
#include "levenum.h"
#include "levgeodata.h"
#include "levphysdata.h"

namespace lev
{

CProject_Model::CProject_Model() : pModel(nullptr), pPhysData(nullptr)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    //arquivos de especificacao
    b_readPREPROJ = true;
    b_savePREPROJ = true; 
    
    b_readDADOS   = true;
    b_saveDADOS   = true;
  
    //arquivos de modelo geometrico
    b_readMGP    = true;
    b_saveMGP    = true; 

    b_readPRX    = true;
    b_savePRX    = true;
    
    b_readRGN   = true;
    b_saveRGN   = true; 
    b_readMMF   = true;
    b_saveMMF   = true; 
    
    //arquivos de modelo fisico
    b_readMEI   = true;
    b_saveMEI   = true;
    b_readMTS   = true;
    b_saveMTS   = true; 

    b_readSRC   = true;
    b_saveSRC   = true;
    
    b_readBCO   = true;
    b_saveBCO   = true; 
    
    b_readBCP   = true;
    b_saveBCP   = true; 

    str_study_name = "electrostatic";
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
}

CProject_Model::~CProject_Model()
{ }

CProject_Model::CProject_Model(const CProject_Model &other)
{
    // Copiar os valores primitivos
    b_readPREPROJ = other.b_readPREPROJ;
    b_savePREPROJ = other.b_savePREPROJ;
    b_readDADOS = other.b_readDADOS;
    b_saveDADOS = other.b_saveDADOS;
    b_readMGP = other.b_readMGP;
    b_saveMGP = other.b_saveMGP;
    b_readPRX = other.b_readPRX;
    b_savePRX = other.b_savePRX;
    b_readRGN = other.b_readRGN;
    b_saveRGN = other.b_saveRGN;
    b_readMMF = other.b_readMMF;
    b_saveMMF = other.b_saveMMF;
    b_readMEI = other.b_readMEI;
    b_saveMEI = other.b_saveMEI;
    b_readMTS = other.b_readMTS;
    b_saveMTS = other.b_saveMTS;
    b_readSRC = other.b_readSRC;
    b_saveSRC = other.b_saveSRC;
    b_readBCO = other.b_readBCO;
    b_saveBCO = other.b_saveBCO;
    b_readBCP = other.b_readBCP;
    b_saveBCP = other.b_saveBCP;

    str_study_name = other.str_study_name;
    str_solver_executable_file = other.str_solver_executable_file;
    str_explorer_executable_file = other.str_explorer_executable_file;

    // Criar cópia profunda dos vetores
    vtr_solver_executable_file_list = other.vtr_solver_executable_file_list;
    vtr_explorer_executable_file_list = other.vtr_explorer_executable_file_list;
    vphen_userdatasettings = other.vphen_userdatasettings;

    // Criar cópia profunda dos ponteiros
    pModel = other.pModel;
    pPhysData = other.pPhysData;
}

CProject_Model& CProject_Model::operator=(const CProject_Model &other)
{
    if (this != &other) // Evitar auto-atribuição
    {
        // Copiar valores primitivos
        b_readPREPROJ = other.b_readPREPROJ;
        b_savePREPROJ = other.b_savePREPROJ;
        b_readDADOS = other.b_readDADOS;
        b_saveDADOS = other.b_saveDADOS;
        b_readMGP = other.b_readMGP;
        b_saveMGP = other.b_saveMGP;
        b_readPRX = other.b_readPRX;
        b_savePRX = other.b_savePRX;
        b_readRGN = other.b_readRGN;
        b_saveRGN = other.b_saveRGN;
        b_readMMF = other.b_readMMF;
        b_saveMMF = other.b_saveMMF;
        b_readMEI = other.b_readMEI;
        b_saveMEI = other.b_saveMEI;
        b_readMTS = other.b_readMTS;
        b_saveMTS = other.b_saveMTS;
        b_readSRC = other.b_readSRC;
        b_saveSRC = other.b_saveSRC;
        b_readBCO = other.b_readBCO;
        b_saveBCO = other.b_saveBCO;
        b_readBCP = other.b_readBCP;
        b_saveBCP = other.b_saveBCP;

        str_study_name = other.str_study_name;
        str_solver_executable_file = other.str_solver_executable_file;
        str_explorer_executable_file = other.str_explorer_executable_file;

        // Criar cópia profunda dos vetores
        vtr_solver_executable_file_list = other.vtr_solver_executable_file_list;
        vtr_explorer_executable_file_list = other.vtr_explorer_executable_file_list;
        vphen_userdatasettings = other.vphen_userdatasettings;

        // Criar cópia profunda dos ponteiros
        pModel = other.pModel;
        pPhysData = other.pPhysData;
    }
    return *this;
}

std::string CProject_Model::GetCompanyName(void)
{
    return sCompanyName;
}

void CProject_Model::SetCompanyName(std::string name)
{
    sCompanyName = name;
}

int CProject_Model::GetPhenomenonEnum(void)
{
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "electrostatic")
    {
        return ELECTROSTATIC;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "electrokinetic")
    {
        return  ELECTROKINETIC;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "magnetostatic")
    {
        return  MAGNETOSTATIC;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "thermal_stationary")
    {
        return THERMALSTATIONARY;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "scalar_optical_fiber")
    {
        return SCALAROPTICALFIBER;
    }
    
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "scalar_optical_wave")
    {
        return SCALAROPTICALWAVE;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "electrooptic")
    {
        return ELETROOPTIC;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "magnetooptic")
    {
        return MAGNETOOPTIC;
    }
    
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "thermooptic")
    {
        return THERMOOPTIC;
    }
    
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "acustooptic")
    {
        return ACUSTOOPTIC;
    }
    
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "vectorial_wave_propagation")
    {
        return VECTORIALWAVEPROPAGATION;
    }
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "transient_heat_conduction")
    {    
        return TRANSIENTHEATCONDUCTION;
    }
    
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName() == "heat_transfer_for_turbulent_flow")
    {    
        return TRANSPORTPHENOMENA;
    }
    return 0;
}

int CProject_Model::GetSymmetryEnum(void)
{
    if(Symmetry() == "plane")
    {
        return PLANE;
    }
    else
    if(Symmetry() == "axial")
    {
        return AXIAL;        
    }
        
    return PLANE;
}

int CProject_Model::GetUnit_of_LengthEnum(void)
{
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit() == "meter")
    {
        return METER;
    }
    
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit() == "centimeter")
    {
        return CENTIMETER;
    }
    
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit() == "milimeter")
    {
        return MILIMETER;
    }
    
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit() == "micrometer")
    {
        return MICROMETER;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit() == "nanometer")
    {
        return NANOMETER;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit() == "angstrom")
    {
        return ANGSTROM;
    }
    
    return METER;
}

std::string  CProject_Model::GetUnit_of_LengthName(void)
{
    std::string nome;
    nome = vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit();
    
    return nome;    
}

std::string  CProject_Model::GetUnit_of_AngleName(void)
{
    std::string nome;
    nome = vpPhenSpecificationVector[GetCurrentPhenIndex()]->AngleUnit();

    return nome;
}

int CProject_Model::GetUnit_of_MassEnum(void)
{
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->MassUnit() == "kilogram")
    {
        return KILOGRAM;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->MassUnit() == "gram")
    {
        return GRAM;
    }

    return KILOGRAM;
}

int CProject_Model::GetUnit_of_TimeEnum(void)
{
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->TimeUnit() == "hour")
    {
        return HOUR;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->TimeUnit() == "minute")
    {
        return MINUTE;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->TimeUnit() == "second")
    {
        return SECOND;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->TimeUnit() == "milisecond")
    {
        return MILISECOND;
    }
    
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->TimeUnit() == "microsecond")
    {
        return MICROSECOND;
    }
    
    return SECOND;
}

int CProject_Model::GetUnit_of_AngleEnum(void)
{
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->AngleUnit() == "radian")
    {
        return RAD;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->AngleUnit() == "degree")
    {
        return DEG;
    }

    return DEG;
}

int CProject_Model::GetUnit_of_TemperatureEnum(void)
{
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->TemperatureUnit() == "celsius")
    {
        return DEGC;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->TemperatureUnit() == "kelvin")
    {
        return DEGK;
    }

    return DEGC;
}

int CProject_Model::GetType_of_MeshEnum(void)
{
    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->MeshType() == "fronteira")
    {
        return  FRONTEIRA;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->MeshType() == "media_vizinho")
    {
        return MED_VIZ;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->MeshType() == "arithmetic")
    {
        return ARITHMETIC;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->MeshType() == "geometric")
    {
        return GEOMETRIC;
    }


    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->MeshType() == "harmonic")
    {
        return HARMONIC;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->MeshType() == "squared_harmonic")
    {
        return SQUARED_HARMONIC;
    }

    if(vpPhenSpecificationVector[GetCurrentPhenIndex()]->MeshType() == "self_adaptive")
    {
        return SELF_ADAPTIVE;
    }

    return GEOMETRIC;
}

void CProject_Model::SetPhenomenonType(int p)
{
    std::string phenomenon;
    switch(p)
    {
        case ELECTROSTATIC:        phenomenon = "electrostatic";
            break;

        case ELECTROKINETIC:    phenomenon = "electrokinetic";
            break;

        case MAGNETOSTATIC:        phenomenon = "magnetostatic";
            break;

        case THERMALSTATIONARY:    phenomenon = "thermal_stationary";
            break;

        case SCALAROPTICALFIBER: phenomenon = "scalar_optical_fiber";
            break;

        case SCALAROPTICALWAVE:    phenomenon = "scalar_optical_wave";
            break;

        case ELETROOPTIC:        phenomenon = "electrooptic";
            break;

        case MAGNETOOPTIC:        phenomenon = "magnetooptic";
            break;

        case THERMOOPTIC:        phenomenon = "thermooptic";
            break;

        case ACUSTOOPTIC:        phenomenon = "acustooptic";
            break;

        case VECTORIALWAVEPROPAGATION:        phenomenon = "vectorial_wave_propagation";
            break;

        case TRANSIENTHEATCONDUCTION:        phenomenon = "transient_heat_conduction";
            break;
        case TRANSPORTPHENOMENA:            phenomenon = "heat_transfer_for_turbulent_flow";
            break;
    }

    vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName(phenomenon);
}

void CProject_Model::SetMeshType(int type)
{
    std::string mesh;

    switch(type)
    {
        case FRONTEIRA:         mesh = "fronteira";
            break;

        case MED_VIZ:           mesh = "media_vizinhos";
            break;

        case ARITHMETIC:        mesh = "arithmetic";
            break;

        case GEOMETRIC:         mesh = "geometric";
            break;

        case HARMONIC:          mesh = "harmonic";
            break;

        case SQUARED_HARMONIC:    mesh = "squared_harmonic";
            break;

        case SELF_ADAPTIVE:        mesh = "self_adaptive";
            break;
    }

    vpPhenSpecificationVector[GetCurrentPhenIndex()]->MeshType(mesh);
}

void CProject_Model::SetSymmetryType(int type)
{
    std::string sym;

    switch(type)
    {
        case PLANE:             sym = "plane";
            break;

        case AXIAL:             sym = "axial";
            break;

        case ASSIMETRIC:        sym = "assimetric";
            break;

        case QUADRANTE:         sym = "quadrante";
            break;
    }

    Symmetry(sym);
}

void CProject_Model::SetUnit_of_Angle(int unit)
{
    switch(unit)
    {
        case RAD:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->AngleUnit("radian");
        break;

        case DEG:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->AngleUnit("degree");
        break;
    }
}

void CProject_Model::SetUnit_of_Length(int unit)
{
    switch(unit)
    {
        case METER:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit("meter");
        break;

        case CENTIMETER:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit("centimeter");
        break;

        case MILIMETER:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit("milimeter");
        break;

        case MICROMETER:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit("micrometer");
        break;

        case NANOMETER:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit("nanometer");
        break;

        case ANGSTROM:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit("angstrom");
        break;
    }
}

void CProject_Model::SetUnit_of_Mass(int unit)
{
    switch(unit)
    {
        case KILOGRAM:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->MassUnit("kilogram");
        break;

        case GRAM:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->MassUnit("gram");
        break;
    }
}

void CProject_Model::SetUnit_of_Temperature(int unit)
{
    switch(unit)
    {
        case DEGC:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->TemperatureUnit("celsius");
        break;

        case DEGK:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->AngleUnit("kelvin");
        break;
    }
}

void CProject_Model::SetUnit_of_Time(int unit)
{
    switch(unit)
    {
        case HOUR:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->TimeUnit("hour");
        break;

        case MINUTE:
            vpPhenSpecificationVector[GetCurrentPhenIndex()]->TimeUnit("minute");
        break;
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// leitura
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//..................................................................................
// Read: chama outras funcoes de leitura
//..................................................................................
//..............................................................................
// -1 : erro
//  0 : sucesso
// codigo de erro inserido no arquivo de log de erro
//..............................................................................
int CProject_Model::Read(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    int codeErr = 0;//2001;
    
    // NAO FAZER A LEITURA DO PROJ NESTA CLASSE!!!!!!! PARA DEIXA-LA SOMENTE COMO LEITURA DOS ARQUIVOS DE MODELAGEM
    // CASO O ARQUIVO DE PROJETO NAO EXISTA ISTO NAO IMPEDE QUE OUTROS ARQUIVOS SEJAM LIDOS
    // read preprj
    // codeErr = ReadPREPRJ();

    // aqui tem que montar o nome do arquivo .dados
    std::string   file_dados = sProjectNameWithPath + ".dados";

    codeErr = ReadSpecDataFile(file_dados);

    // A montagem dos nomes depende do tipo de malha, caso o arquivo de especificacoes nao tenha sido lido, assume-se o valor padrao (geometric)
    BuildFileNames();

    //le dados do modelo geometrico 
    //codeErr = pModel->Read();
    codeErr = ReadGeometricModel();

    if(codeErr)
    {
     // incluir mensagem no log sobre o erro de leitura do pModel   
        return codeErr; //A aplicacao tem que saber tratar o erro (por ex. avisar o usuario...)
    }
   
    //le dados do modelo fisico
    //codeErr = pPhysData->Read();
    codeErr = ReadPhysicalModel();

    if(codeErr)
    {
     // incluir mensagem no log sobre o erro de leitura do pPhysData   
        return codeErr; //A aplicacao tem que saber tratar o erro (por ex. avisar o usuario...)
    }
    
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return codeErr;
}

//..................................................................................
// ReadGeometricModel: chama outras funcoes de leitura de pModel
//..................................................................................
//..............................................................................
// -1 : erro
//  0 : sucesso
// codigo de erro inserido no arquivo de log de erro
//..............................................................................
int CProject_Model::ReadGeometricModel(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    lev::CLevMessage messagedata;    
    std::string msg ;
    int codeErr = 0;//2001;
            
    //le dados do modelo geometrico 
    // codeErr = pModel->Read();
    
    if(b_readMGP)
    {
        codeErr = pModel->ReadGeometricModel(GetFileName(MODEL_GEOMETRY_PREDIS));
        
        if(codeErr == 101) //erro na abertura de MGP (pode não existir)
        {
            codeErr = pModel->ReadELL(GetFileName(MODEL_CONNECTION));
        }

        if(!codeErr) 
            pModel->ModelPrimitivesOk(true);        
        else
            pModel->ModelPrimitivesOk(false);

        if(codeErr)
        {
           switch (codeErr)
           {
               case 101:
                   msg = "Error reading file " + GetFileName(MODEL_GEOMETRY_PREDIS);
                   codeErr = 2004;
                   messagedata.SetMessageData(codeErr,"CProject_Model::ReadGeometricModel", msg , MSGERROR);
                   SetErrorMessage(messagedata); 
                   SaveErrorLog();
                   return -1;                        
               break;

               case 200:
                   msg = "Error reading file " + GetFileName(MODEL_REGIONS);
                   codeErr = 2005;
                   messagedata.SetMessageData(codeErr,"CProject_Model::ReadGeometricModel", msg , MSGWARNING);
                   SetErrorMessage(messagedata); 
                   SaveErrorLog();
                   //return -1;                        
               break;                
           }
       }
        
    }//end of if(b_readMGP)

    // nao devolve mensagem de erro dos arquivos abaixo, pois pode ser que nao existam   
    if(b_readRGN)
    {
        codeErr = pModel->ReadModelRegions(GetFileName(MODEL_REGIONS));

        if(!codeErr)
               pModel->ModelRegionsOk(true);
        else
            pModel->ModelRegionsOk(false);
    }

    if(b_readMMF)
    {
        codeErr = pModel->ReadModelMeshFrontier(GetFileName(MODELMESHFRONTEIRA));

        if(!codeErr && pModel->vElementos_fronteira.size())
        {
            pModel->MeshFronteiraOk(true);
        }
    }

    if(b_readPRX)
        codeErr = pModel->ReadAuxiliarPrimitivesIndexes(o_NameBuilder_v1.GetFileName(MODEL_AUXILIARPRIMITIVES)); //.prx

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return 0;
}

//..................................................................................
// ReadPhysicalModel: le arquivos de propriedades físicas
// chama leitura de levphystdata 
// obs: para os fenomeno schrodinger, devido aa diferenca na versao do arquivo .mei,
// a leitura do arquivo e atualizacao dos materiais e realizada na classe de contole
// 
//..................................................................................
//..............................................................................
// -1 : erro
//  0 : sucesso
// codigo de erro inserido no arquivo de log de erro
//..............................................................................
int CProject_Model::ReadPhysicalModel(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    int codeErr = 0;//2001;
           
    if(pModel)
    {        
        if(pModel->ModelPrimitivesOk())
        {
            if(b_readBCP)
               codeErr = pPhysData->ReadBoundaryConditionsPrimitives(GetFileName(MODEL_BOUNDARYCONDITIONS));

            if(b_readBCO)
               codeErr = pPhysData->ReadBoundaryConditionsInUse(GetFileName(MODEL_BOUNDARYCONDITIONSINUSE));            
    
            //*NMA*: não verifica os retornos anteriores pois a leitura, neste caso,eh independente
            if(pModel->ModelRegionsOk())
             {
                if(b_readMTS)
                    codeErr = pPhysData->ReadMaterialsInUse(GetFileName(MODEL_MATERIALINUSE));
    
                if(b_readSRC)
                    codeErr = pPhysData->ReadSourcesInUse(GetFileName(MODEL_SOURCESINUSE));
    
            }
        }

        if(b_readMEI)
        {
            //codeErr = pPhysData->ReadPropFis(GetFileName(PROPFISMEI));
            codeErr = pPhysData->ReadPropFis02(GetFileName(PROPFISMEI));
            
            if(!codeErr)
                pPhysData->ReadpropfisOk(true);
            else
                pPhysData->ReadpropfisOk(false);
        }
    }
        
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return codeErr;
}

//..............................................................................
// -1 : erro
//  0 : sucesso
// codigo de erro inserido no arquivo de log de erro
//..............................................................................
int CProject_Model::ReadPREPRJ(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    lev::CLevMessage messagedata;
    int codeErr = 0;    
    std::string file_preproj;
    int iVer = -1;
    std::string aux, str_ver;
    
    if(sProjectNameWithPath.empty())
    {
        //std::string msg = "Could not be open the file " + file_preproj;
        codeErr = 2001;
        messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJ", "Project name is empty!", MSGERROR);
        SetErrorMessage(messagedata); 
        SaveErrorLog();
        return -1;
    }
    
    file_preproj = sProjectNameWithPath + ".preproj";
    
    std::ifstream filein(file_preproj.c_str());
    
    if(!filein.fail())
    {
        filein >> aux >> str_ver; //versao do arquivo

        if( str_ver.find("v1") != std::string::npos ) iVer = 1;
        else
        {
            codeErr = 2002;
            messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "File format not supported: " + file_preproj , MSGERROR); 
            SetErrorMessage(messagedata); 
            SaveErrorLog();
            
            return -1;            
        }
        
        switch(iVer)
        {
            case 1:
                ReadPREPRJv001();
                break;
        }
    }
    else
    {
        std::string msg = "Could not be open the file " + file_preproj;
        codeErr = 2003;
        messagedata.SetMessageData(codeErr,"CProject_Model::SavePREPRJ", msg , MSGERROR);
        SetErrorMessage(messagedata); 
        SaveErrorLog();
        
        return -1;        
    }
        
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return 0;
}

//..............................................................................
// -1 : erro
//  0 : sucesso
// codigo de erro inserido no arquivo de log de erro
//..............................................................................
int CProject_Model::ReadPREPRJv001(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    lev::CLevMessage messagedata;
    int codeErr = 0;    
    std::string file_preproj;
    int i=0;
    int iPhen_qty, iaux;
    double daux;
    std::string aux, str_ver, str_data;
    
    if(sProjectNameWithPath.empty())
    {
        codeErr = 2001;
        messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Project name is empty!", MSGERROR);
        SetErrorMessage(messagedata); 
        SaveErrorLog();
        return -1;
    }
    
    file_preproj = sProjectNameWithPath + ".preproj";
    
    std::ifstream filein(file_preproj.c_str());
    
    if(!filein.fail()) {
       filein >> aux >> str_ver; //versao do arquivo
       
       if(str_ver.find("v1") == std::string::npos) {
            codeErr = 2002;
            messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "File format not supported: " + file_preproj , MSGERROR); 
            SetErrorMessage(messagedata); 
            SaveErrorLog();
            return -1;
       }
        
       // dados do projeto
       filein >> aux >> sProjectDimension;
       if(aux.find("Project_Dimension") == std::string::npos) {
            codeErr = 2003;
            messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Project_Dimension in file: " + file_preproj , MSGERROR); 
            SetErrorMessage(messagedata); 
            SaveErrorLog();            
            return -1;
       }      

       filein >> aux >> sProjectSymmetry ;
       if(aux.find("Symmetry_Type") == std::string::npos)
       {
            codeErr = 2004;
            messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Symmetry_Type in file: " + file_preproj , MSGERROR);                    
            SetErrorMessage(messagedata); 
            SaveErrorLog();            
            return -1;
       }      
        
       // quantidade de fenomenos envolvidos no projeto
       filein >> aux >> iPhen_qty ;
       if(aux.find("Phenomenon_Qty") == std::string::npos)
       {
            codeErr = 2005;
            messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Phenomenon_Qty in file: " + file_preproj , MSGERROR);                    
            SetErrorMessage(messagedata); 
            SaveErrorLog();            
            return -1;
       }      

       vpPhenSpecificationVector.clear();
        
       // dados de cada fenomeno
       for(i=0; i<iPhen_qty; i++)
       {
            CreateAndIncludePhenSpecification();
            SetCurrentPhenIndex(i); 
            
            filein >> aux >>  str_data;
            if(aux.find("Phenomenon_Name") == std::string::npos)
            {
                codeErr = 2006;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Phenomenon_Name in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                  
            vpPhenSpecificationVector[i]->PhenomenonName(str_data);

            filein >> aux >>  str_data;
            if(aux.find("Unit_of_Length") == std::string::npos)
            {
                codeErr = 2006;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Unit_of_Length in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->LengthUnit(str_data) ;
            
            filein >> aux >>  str_data;
            if(aux.find("Unit_of_Mass") == std::string::npos)
            {
                codeErr = 2007;
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Unit_of_Mass in file: " + file_preproj , MSGERROR);                    
                return -1;
            }                 
            vpPhenSpecificationVector[i]->MassUnit(str_data);            
            
            filein >> aux >> str_data;
            if(aux.find("Unit_of_Time") == std::string::npos)
            {
                codeErr = 2008;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Unit_of_Time in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->TimeUnit(str_data);                
            
            
            filein >> aux >> str_data;
            if(aux.find("Unit_of_Angle") == std::string::npos) {
                codeErr = 2008;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Unit_of_Angle in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->AngleUnit(str_data);                                    
            
            filein >> aux >> str_data;
            if(aux.find("Unit_of_Temperature") == std::string::npos) {
                codeErr = 2009;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Unit_of_Temperature in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->TemperatureUnit(str_data);

            filein >> aux >> str_data;
            if(aux.find("PDE_Solver_Method") == std::string::npos) {
                codeErr = 2010;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of PDE_Solver_Method in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->PDESolverMethod(str_data);
            
            filein >> aux >> iaux;
            if(aux.find("Degrees_of_Freedom") == std::string::npos) {
                codeErr = 2011;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Degrees_of_Freedom in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->DegreesOfFreedom(iaux);
            
            filein >> aux >> iaux;
            if(aux.find("Element_Order") == std::string::npos) {
                codeErr = 2012;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Element_Order in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->ElementOrder(iaux);
            
            filein >> aux >> str_data;
            if(aux.find("Element_Type") == std::string::npos) {
                codeErr = 2013;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Element_Type in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->ElementType(str_data);

            filein >> aux >> str_data;
            if(aux.find("System_Of_Equations_Solver") == std::string::npos) {
                codeErr = 2014;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of System_Of_Equations_Solver in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->SystemOfEquationsSolver(str_data);   
            
            filein >> aux >> daux;
            if(aux.find("System_Of_Equations_Convergence_Prec") == std::string::npos) {
                codeErr = 2015;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of System_Of_Equations_Convergence_Prec in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->SetSolverPrecision(daux);

            filein >> aux >> daux;
            if(aux.find("System_Of_Equations_Convergence_Steps") == std::string::npos) {
                codeErr = 2016;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of System_Of_Equations_Convergence_Steps in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->SetSolverSaveIteStep(daux); 
            
            filein >> aux >> str_data;
            if(aux.find("Mesh_Type") == std::string::npos )
            {
                codeErr = 2016;
                messagedata.SetMessageData(codeErr,"CProject_Model::ReadPREPRJv001", "Error reading data of Mesh_Type in file: " + file_preproj , MSGERROR);                    
                SetErrorMessage(messagedata); 
                SaveErrorLog();            
                return -1;
            }                 
            vpPhenSpecificationVector[i]->MeshType(str_data);
        }
        filein.close();
    } else {
        std::string msg = "Could not be open the file " + file_preproj;
        codeErr = 2001;
        messagedata.SetMessageData(codeErr,"CProject_Model::SavePREPRJ", msg , MSGERROR);
        SetErrorMessage(messagedata); 
        SaveErrorLog();                    
        return -1;
    }
                
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return 0;
}

//*NMA* Leitura do arquivo .projo binario inteiro, mas ainda nao consegui obter os valores de variaveis inteiras, somente os caracteres
short CProject_Model::ReadProj01(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    int i;

    std::string filename = sProjectNameWithPath + ".proj";
    std::cout << "\n\CProject_Model::ReadProj01 filename = " << filename ;

    std::ifstream file (filename.c_str(), std::ios::in|std::ios::binary);
    if (file)
    {
        // get length of file:
        file.seekg (0, file.end);
        int length = file.tellg();
        file.seekg (0, file.beg);

        char * buffer = new char [length];

        std::cout << "\n\CProject_Model::ReadProj01 Reading " << length << " characters... ";std::cout.flush();
        // read data as a block:
        file.read (buffer,length);

        if (file)
          std::cout << "all characters read successfully.";
        else
          std::cout << "error: only " << file.gcount() << " could be read";
        file.close();
        std::cout.flush();
        // ...buffer contains the entire file...

        std::cout << "\n buffer[i] = "; std::cout.flush();
        for(i=0;i<length;i++)
        {
            std::cout << (char) buffer[i] << " "; std::cout.flush();
        }

        std::cout << "\n int buffer[i] = " ; std::cout.flush();
        for(i=0;i<length;i++)
        {
            std::cout << (int) buffer[i] << " "; std::cout.flush();
        }

        std::cout << "\n";
        std::cout.flush();
        std::string fim = "---FIM---";
        std::string tudo = buffer + fim;


        std::cout << "\n\CProject_Model::ReadProj01 tudo =  \n" << tudo.c_str() << " \n\n\n ";std::cout.flush();

        delete[] buffer;
      }
    else
        std::cout << "\n\CProject_Model::ReadProj01 ERRO ao abrir o arquivo " << filename ;std::cout.flush();

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return 0;
}

short CProject_Model::ReadProj(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    short codeErr = 0;
    int i;

    char str[300], c_aux, str_len[300],cc;
    char data3[300];
    int   aux;
    std::string std_str;

    //==================
    // Get current time
    //==================
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);

    std::string filename = sProjectNameWithPath + ".proj";

    std::ifstream file (filename.c_str(), std::ios::in|std::ios::binary);
//    FILE * file;

  //  file = fopen (filename.c_str(),"rb");

//---nome do projeto------------------------------------------------
    std_str = ReadBinaryString(file);
    std::cout << "\nnome do projeto =  " << std_str.c_str();  std::cout.flush();

//---nome do projetista------------------------------------------------
    std_str = ReadBinaryString(file);
    std::cout << "\nnome do projetista =  " << std_str.c_str();  std::cout.flush();

//---nome da empresa------------------------------------------------
    std_str = ReadBinaryString(file);
    std::cout << "\nnome da empresa =  " << std_str.c_str(); std::cout.flush();

//---data de criacao ------------------------------------------------
    std_str = ReadBinaryString(file);
    std::cout << "\ndata de criacao =  " << std_str.c_str(); std::cout.flush();

//---data da ultima alteracao ------------------------------------------------
    std_str = ReadBinaryString(file);
    std::cout << "\ndata da ultima alteracao =  " << std_str.c_str();  std::cout.flush();


//---unidade de comprimento ------------------------------------------------
     aux = ReadBinaryInt(file);
     SetUnit_of_Length(aux);
     std_str = GetUnit_of_LengthName();
     std::cout << "\n indice da unidade de comprimento = aux =  " << aux;  std::cout.flush();
     std::cout << "\n unidade de comprimento =  " << std_str;  std::cout.flush();

//---unidade de massa ------------------------------------------------
     aux = ReadBinaryInt(file);
     std::cout << "\n indice da unidade de massa = aux =  " << aux;  std::cout.flush();

//---unidade de tempo ------------------------------------------------
     aux = ReadBinaryInt(file);
     std::cout << "\n indice da unidade de tempo = aux =  " << aux;  std::cout.flush();

//---unidade de angulo ------------------------------------------------
     aux = ReadBinaryInt(file);
     std::cout << "\n indice da unidade de angulo = aux =  " << aux;  std::cout.flush();

//---unidade de temperatura ------------------------------------------------
     aux = ReadBinaryInt(file);
     std::cout << "\n indice da unidade de temperatura = aux =  " << aux;  std::cout.flush();

//---data da descricao ------------------------------------------------
     std_str = ReadBinaryString(file);
     std::cout << "\ndescricao =  " << std_str.c_str();  std::cout.flush();

//---indice do fenomeno ------------------------------------------------
     aux = ReadBinaryInt(file);
      std::cout << "\n indice do fenomeno = aux =  " << aux;  std::cout.flush();

//---indice do tipo de malha ------------------------------------------------
      aux = ReadBinaryInt(file);
      std::cout << "\n indice do tipo de malha = aux =  " << aux; std::cout.flush();

//---indice do tipo de simetria ------------------------------------------------
      aux = ReadBinaryInt(file);
      std::cout << "\n indice do tipo de simetria = aux =  " << aux; std::cout.flush();

//--------  problemas na leitura do resto do arquivo, porem o resto do arquivo nao eh necessario ler, pois
//              sao os nomes dos arquivos

    file.close();

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return codeErr;
}//

#if 10
int CProject_Model::ReadBinaryInt(std::ifstream &file)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    char c_aux;
    int aux;
    int i;
    char hex[10];

  //for(i=0;i<4;i++)
  {

      file.get(c_aux);
      hex[0] = (int) c_aux;
    //  std::cout << "\nCProject_Model::ReadBinaryInt char c_aux =  " << (char) c_aux; std::cout << "   ---final\n"; std::cout.flush();
    //  std::cout << "\nCProject_Model::ReadBinaryInt int c_aux =  " << (int)  c_aux; std::cout << "   ---final\n"; std::cout.flush();
      file.get(c_aux);
    //  std::cout << "\nCProject_Model::ReadBinaryInt char c_aux =  " << (char) c_aux; std::cout << "   ---final\n"; std::cout.flush();
    //  std::cout << "\nCProject_Model::ReadBinaryInt int c_aux =  " << (int)  c_aux; std::cout << "   ---final\n"; std::cout.flush();
      hex[1] = (char) c_aux;
      file.get(c_aux);
    //  std::cout << "\nCProject_Model::ReadBinaryInt char c_aux =  " << (char) c_aux; std::cout << "   ---final\n"; std::cout.flush();
    //  std::cout << "\nCProject_Model::ReadBinaryInt int c_aux =  " << (int)  c_aux; std::cout << "   ---final\n"; std::cout.flush();
      hex[2] = (int) c_aux;
      file.get(c_aux);
    //  std::cout << "\nCProject_Model::ReadBinaryInt char c_aux =  " << (char) c_aux; std::cout << "   ---final\n"; std::cout.flush();
    //  std::cout << "\nCProject_Model::ReadBinaryInt int c_aux =  " << (int)  c_aux; std::cout << "   ---final\n"; std::cout.flush();
      //hex[3] = (int) c_aux;
      aux = (int) c_aux;
      std::cout << "\nCProject_Model::ReadBinaryInt aux =  " << aux; std::cout << "   ---final\n"; std::cout.flush();
  }
  hex[4] = '/0';

  //unsigned int x;
  //std::stringstream ss;
  //ss << std::hex << hex;
  //ss >> x;
  //std::cout <<  "\nCProject_Model::ReadBinaryInt x =  " << x; std::cout << "   ---final\n"; std::cout.flush();

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return aux;
}
#endif

#if 10
std::string CProject_Model::ReadBinaryString(std::ifstream &file)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
  std::string std_string;
  char  str[1000];
  char  data3[1000];
  char  vazio[1000]= " ";
  char c_aux;

  int i;
  int aux;
  //le o comprimento

  file.get(c_aux);
  aux = (int) c_aux;
 // std::cout << "\nCProject_Model::ReadBinaryString comprimento =  " << aux; std::cout << "   ---final\n"; std::cout.flush();
  //le a string

  for(i=0; i<aux;i++)
  {
    file.get(c_aux);
//    std::cout << "\n c_aux["<< i << "]=" << c_aux; std::cout.flush();
    str[i] = c_aux;
//    std::cout << "\n str["<< i << "]= " << str[i]; std::cout.flush();
  }
  str[aux] ='/0';

  for(i=aux+1; i<1000;i++)
  {
    data3[i]= ' ';
  }

  strncpy(data3, str,aux);
  //std::cout << "\nstr =  " << str; std::cout << "   ---final\n"; std::cout.flush();
  //std::cout << "\ndata3 =  " << data3; std::cout << "   ---final\n"; std::cout.flush();

  std_string = data3;

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return std_string;
}
#endif

//....................................................................................................................
// Save: chama outras funcoes de gravacao
//....................................................................................................................
int CProject_Model::Save(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    int codeErr = 0;//2001;
    
    //salva projeto em formato XML

    std::string filenameXML = ProjectNameWithPath() + ".xml";

    codeErr = SaveUserDataSettings(filenameXML);//SavePRJ(filenameXML);

    SaveSpecDataFile(o_NameBuilder_v1.GetFileName(FILEDATASPEC));
    
    //salva dados do modelo geometrico e do modelo fisico
    codeErr = SaveGeoPhysModel();

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return codeErr;
}

//..............................................................................
// -1 : erro
//  0 : sucesso
// codigo de erro inserido no arquivo de log de erro
//..............................................................................
int CProject_Model::SavePREPRJ(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    lev::CLevMessage messagedata;
    int codeErr = 0;    
    std::string file_preproj;
    unsigned int i=0;
    
    if(sProjectNameWithPath.empty()) {
        codeErr = 2001;
        messagedata.SetMessageData(codeErr,"CProject_Model::SavePREPRJ", "Project name is empty!", MSGERROR);
        return -1;
    }
    
    file_preproj = sProjectNameWithPath + ".preproj";
    
    if(sDesignerName.empty()) sDesignerName = "unknown";
    if(sDateOfCreation.empty()) sDateOfCreation = "unknown";
    if(sDateLastModifiedIn.empty()) sDateLastModifiedIn = "unknown";
    if(sDescription.empty()) sDescription = "unknown";
    if(sCompanyName.empty()) sCompanyName = "unknown";
    if(sProjectDimension.empty()) sProjectDimension = "two_dimensional";
    if(sProjectSymmetry.empty()) sProjectSymmetry = "plane";

    std::ofstream fileout(file_preproj.c_str());
            
    if(!fileout.fail()) {
        fileout << "File_Version\t " << "v1" << "\n"; //versao do arquivo

        // dados do projeto
        fileout << "Project_Dimension\t" << sProjectDimension << "\n";
        fileout << "Symmetry_Type\t" << sProjectSymmetry << "\n";

        // quantidade de fenomenos envolvidos no projeto
        fileout << "Phenomenon_Qty\t" << vpPhenSpecificationVector.size() << "\n";

        // dados de cada fenomeno
        for(i=0; i<vpPhenSpecificationVector.size(); i++) {
            if(vpPhenSpecificationVector[i]->PhenomenonName().empty()) vpPhenSpecificationVector[i]->PhenomenonName("undefined");
            if(vpPhenSpecificationVector[i]->LengthUnit().empty()) vpPhenSpecificationVector[i]->LengthUnit("undefined");
            if(vpPhenSpecificationVector[i]->MassUnit().empty()) vpPhenSpecificationVector[i]->MassUnit("undefined");
            if(vpPhenSpecificationVector[i]->TimeUnit().empty()) vpPhenSpecificationVector[i]->TimeUnit("undefined");
            if(vpPhenSpecificationVector[i]->AngleUnit().empty()) vpPhenSpecificationVector[i]->AngleUnit("undefined");
            if(vpPhenSpecificationVector[i]->TemperatureUnit().empty()) vpPhenSpecificationVector[i]->TemperatureUnit("undefined");
            if(vpPhenSpecificationVector[i]->PDESolverMethod().empty()) vpPhenSpecificationVector[i]->PDESolverMethod("undefined");
            if(vpPhenSpecificationVector[i]->ElementType().empty()) vpPhenSpecificationVector[i]->ElementType("undefined");
            if(vpPhenSpecificationVector[i]->SystemOfEquationsSolver().empty()) vpPhenSpecificationVector[i]->SystemOfEquationsSolver("undefined");
            if(vpPhenSpecificationVector[i]->MeshType().empty()) vpPhenSpecificationVector[i]->MeshType("undefined");
            
            fileout << "Phenomenon_Name\t" << vpPhenSpecificationVector[i]->PhenomenonName() << "\n";
            fileout << "Unit_of_Length\t" << vpPhenSpecificationVector[i]->LengthUnit() << "\n";
            fileout << "Unit_of_Mass\t" << vpPhenSpecificationVector[i]->MassUnit() << "\n";
            fileout << "Unit_of_Time\t" << vpPhenSpecificationVector[i]->TimeUnit() << "\n";
            fileout << "Unit_of_Angle\t" << vpPhenSpecificationVector[i]->AngleUnit() << "\n";
            fileout << "Unit_of_Temperature\t" << vpPhenSpecificationVector[i]->TemperatureUnit() << "\n";

            fileout << "PDE_Solver_Method\t" << vpPhenSpecificationVector[i]->PDESolverMethod() << "\n";
            fileout << "Degrees_of_Freedom\t" << vpPhenSpecificationVector[i]->DegreesOfFreedom() << "\n";
            fileout << "Element_Order\t" << vpPhenSpecificationVector[i]->ElementOrder() << "\n";
            fileout << "Element_Type\t" << vpPhenSpecificationVector[i]->ElementType() << "\n";
            fileout << "System_Of_Equations_Solver\t" << vpPhenSpecificationVector[i]->SystemOfEquationsSolver() << "\n";            
            fileout << "System_Of_Equations_Convergence_Prec\t" << vpPhenSpecificationVector[i]->GetSolverPrecision() << "\n";            
            fileout << "System_Of_Equations_Convergence_Steps\t" << vpPhenSpecificationVector[i]->GetSolverSaveIteStep() << "\n";            
            
            fileout << "Mesh_Type\t" << vpPhenSpecificationVector[i]->MeshType() << "\n";            
        }
              
        fileout.close();
    }
    else
    {
        std::string msg = "Could not be open the file " + file_preproj;
        codeErr = 2001;
        messagedata.SetMessageData(codeErr,"CProject_Model::SavePREPRJ", msg , MSGERROR);
        
        return -1;
    }
                
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return 0;
}

//--------------------------------------------------------------------------------------------
//............................................................................................
// int  CProject_Model::SaveGeoPhysModel(void)
// Grava dados dos modelos geometrico e fisico
// a partir dos ponteiros para os modelos
// pModel: CLevGeoData *
// pPhysData: CLevPhysData *
// alteracao de dados: nao faz verificacao de existencia/alteracao de dados, entao sempre grava para evitar inconsistencias
// flag para gravacao ou nao: grava somente se o flag for verdadeiro (o arquivo pode ser gravado por outro projeto, ex: .mei)
//............................................................................................
int CProject_Model::SaveGeoPhysModel(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    lev::CLevMessage messagedata;
    int codeErr = 0;  
    int codeReturned = 0;

    if(!pModel) {
        codeErr = 2002;
        messagedata.SetMessageData(codeErr,"CProject_Model::SaveGeoPhysModel", "The geometric model object has not been associated! ", MSGERROR);        
        return -1;
    }

    pModel->Associate(this);
    pModel->SetIdToValidPoints();

    if(b_saveMGP)
        codeErr = pModel->SaveGeometricModel(o_NameBuilder_v1.GetFileName(MODEL_GEOMETRY_PREDIS)); //.mgp

    if(b_saveRGN)
         codeReturned = pModel->SaveModelRegions(o_NameBuilder_v1.GetFileName(MODEL_REGIONS)); //.rgn

    if(b_saveMMF)
         codeReturned = pModel->SaveModelMeshFrontier(o_NameBuilder_v1.GetFileName(MODELMESHFRONTEIRA)); //.mmf

    std::cout << "\n" << __FUNCTION__ << " [01] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();

    if(b_savePRX)
        codeReturned = pModel->SaveAuxiliarPrimitivesIndexes(o_NameBuilder_v1.GetFileName(MODEL_AUXILIARPRIMITIVES)); //.prx

    std::cout << "\n" << __FUNCTION__ << " [02] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();

    if(!pPhysData) {
        codeErr = 2003;
        messagedata.SetMessageData(codeErr,"CProject_Model::SaveGeoPhysModel", "The physical model object has not been associated! ", MSGWARNING);        
        return -1;           
    }
                        
    if(b_saveMEI) 
        codeReturned = pPhysData->SavePropFis(o_NameBuilder_v1.GetFileName(PROPFISMEI)); //.mei

    std::cout << "\n" << __FUNCTION__ << " [04] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();

    if(b_saveBCO)
        codeReturned = pPhysData->SaveBoundaryConditionsInUse(o_NameBuilder_v1.GetFileName(MODEL_BOUNDARYCONDITIONSINUSE)); //.bco

    std::cout << "\n" << __FUNCTION__ << " [05] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();

    if(b_saveBCP)
        codeReturned = pPhysData->SaveBoundaryConditionsPrimitives(o_NameBuilder_v1.GetFileName(MODEL_BOUNDARYCONDITIONS)); //.bcp

    std::cout << "\n" << __FUNCTION__ << " [06] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();

    if(b_saveSRC)
        codeReturned = pPhysData->SaveSourcesInUse(o_NameBuilder_v1.GetFileName(MODEL_SOURCESINUSE)); //.src

    std::cout << "\n" << __FUNCTION__ << " [07] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();

    if(b_saveMTS)
        codeReturned = pPhysData->SaveMaterialsInUse(o_NameBuilder_v1.GetFileName(MODEL_MATERIALINUSE)); //.mts

    std::cout << "\n" << __FUNCTION__ << " [08] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return codeErr;
}

#if 0
int  CProject_Model::SaveGeoPhysModel(void)               
{
    lev::CLevMessage messagedata;
    int codeErr = 0;  
    int codeReturned = 0;
   // std::string file_preproj;
    
    if(!pModel)
    {
        codeErr = 2002;
        messagedata.SetMessageData(codeErr,"CProject_Model::SaveGeoPhysModel", "The geometric model has not been associated! ", MSGERROR);        
        return -1;
    }
            
    // do serialize
    pModel->Associate(this);
    
    pModel->SetIdToValidPoints();
    
    if(pModel->GetNumPrimitives())
    {
        if(b_saveMGP)
            codeErr = pModel->SaveGeometricModel(o_NameBuilder_v1.GetFileName(MODEL_GEOMETRY_PREDIS)); //.mgp
        

        if(pModel->GetNumValidRegions())
        {
            if(b_saveRGN)
                codeReturned = pModel->SaveModelRegions(o_NameBuilder_v1.GetFileName(MODEL_REGIONS)); //.rgn
            
            if(b_saveMMF)
                codeReturned = pModel->SaveModelMeshFrontier(o_NameBuilder_v1.GetFileName(MODELMESHFRONTEIRA)); //.mmf
        }
        else
        {
//            DeleteFile(project->GetFileName(MODEL_REGIONS));        //

        }
        

        if(pPhysData)
        {
            //std::cout << "\nint  CProject_Model::SaveGeoPhysModel(void): pPhysData = " << pPhysData << "  " << std::cout.flush();
            //std::cout << "\nint  CProject_Model::SaveGeoPhysModel(void): pPhysData->Get_Num_Materials() = " << pPhysData->Get_Num_Materials() << "  " << std::cout.flush();
                        
            if(pPhysData->Get_Num_Materials())// && pPhysData->IsMaterialModified())
            {
              if(b_saveMEI) 
                  codeReturned = pPhysData->SavePropFis(o_NameBuilder_v1.GetFileName(PROPFISMEI)); //.mei
            }

//            std::cout << "\nint  CProject_Model::SaveGeoPhysModel(void): pPhysData->GetNumValidBoundaryConditions() = " << pPhysData->GetNumValidBoundaryConditions() << "  " << std::cout.flush();
//            std::cout.flush();
//            std::cout << "\npress ENTER to continue"; std::cout.flush();
//            std::cout.flush();
//            getchar();
            
            if(pPhysData->GetNumValidBoundaryConditions() ) //gravando sempre, pois em Save As, acaba não salvando
            {

                if(b_saveBCO)
                    codeReturned = pPhysData->SaveBoundaryConditionsInUse(o_NameBuilder_v1.GetFileName(MODEL_BOUNDARYCONDITIONSINUSE)); //.bco
                
                if(b_saveBCP)
                    codeReturned = pPhysData->SaveBoundaryConditionsPrimitives(o_NameBuilder_v1.GetFileName(MODEL_BOUNDARYCONDITIONS)); //.bcp
            }
            else
            {
                // se não existe deleta arquivos pré-existentes
                if(!pPhysData->GetNumValidBoundaryConditions())
                {
                    //DeleteFile(project->GetFileName(MODEL_BOUNDARYCONDITIONSINUSE));        //
                    //DeleteFile(project->GetFileName(MODEL_BOUNDARYCONDITIONS));        //
                    //DeleteFile(project->GetFileName(BOUNDARYCONDITIONS));        //
                }
            }//

            //if(pPhysData->GetNumValidSourcesInUse() && pPhysData->IsSourceModified())
            {
                if(b_saveSRC)
                    codeReturned = pPhysData->SaveSourcesInUse(o_NameBuilder_v1.GetFileName(MODEL_SOURCESINUSE)); //.src
            }
          //  else
            {
                if(!pPhysData->GetNumValidSourcesInUse())
                {
    //                DeleteFile(project->GetFileName(SOURCE));
    //                DeleteFile(project->GetFileName(MODEL_SOURCESINUSE));
                }
            }

            if(1)
            {
                if(b_saveMTS)
                    codeReturned = pPhysData->SaveMaterialsInUse(o_NameBuilder_v1.GetFileName(MODEL_MATERIALINUSE)); //.mts
            }
            else
            {
                if(!pPhysData->GetNumValidMaterialsInUse())
                {
                    //DeleteFile(o_NameBuilder_v1.GetFileName(MODEL_MATERIALINUSE));        //
                    //DeleteFile(o_NameBuilder_v1.GetFileName(MATERIAL));        //
                }
            }

           }//end of if(pPhysData)
                                             
    }//end of if(pModel->GetNumPrimitives())
            
}//end of int  CProject_Model::SaveGeoModel(void)
#endif

short CProject_Model::SaveProj(PHENOMENONTYPE phen)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
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
    s[1]  = sDesignerName;//"LEV";  // Designer name
    s[2]  = sCompanyName;//"IEAv/DCTA";            // Company name
    s[3]  = std::string(buffer);    // Creation Date
    s[4]  = std::string(buffer);    // Last Modified In
    s[5]  = "";                     // Description
    s[6]  = o_NameBuilder_v1.GetFileName(MODEL_CONNECTION);//sProjectName + ".ell";  // Modelo Conexao
    s[7]  = o_NameBuilder_v1.GetFileName(MODEL_GEOMETRY);//sProjectName + ".ege";  // Modelo Geometria
    s[8]  = o_NameBuilder_v1.GetFileName(MODEL_COORDINATES);//sProjectName + ".ept";  // Modelo Coordinates
    s[9]  = o_NameBuilder_v1.GetFileName(MODEL_PRE);//sProjectName + ".epd";  // Modelo Pre;
    s[10] = o_NameBuilder_v1.GetFileName(GEOMETRY);//sProjectName + ".n1g";  // Geometria
    s[11] = o_NameBuilder_v1.GetFileName(SEGMENTS);//sProjectName + ".g1g";  // Segmentos
    s[12] = o_NameBuilder_v1.GetFileName(ELEMENTS);//sProjectName + ".t1g";  // Elementos
    s[13] = o_NameBuilder_v1.GetFileName(BOUNDARYCONDITIONS);//sProjectName + ".c1g";  // CC
    s[14] = o_NameBuilder_v1.GetFileName(MATERIAL);//sProjectName + ".m1g";  // Meios
    s[15] = o_NameBuilder_v1.GetFileName(SOURCE);//sProjectName + ".f1g";  // Fontes
    s[16] = o_NameBuilder_v1.GetFileName(POTENTIAL);//sProjectName + ".p1g";  // Potenciais
    s[17] = o_NameBuilder_v1.GetFileName(ARESTASGEO);//sProjectName + "_areas.ar";   // Arestas
    s[18] = o_NameBuilder_v1.GetFileName(ELEMENTOSARESTAS);//sProjectName + ".art";  // Elementos Arestas

    std::string filename = sProjectNameWithPath + ".proj";

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
    switch(phen)
    {
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

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return codeErr;
}

short CProject_Model::SaveProj(int qualquer)
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
    s[1]  = sDesignerName;//"LEV";  // Designer name
    s[2]  = sCompanyName;//"IEAv/DCTA";            // Company name
    s[3]  = std::string(buffer);    // Creation Date
    s[4]  = std::string(buffer);    // Last Modified In
    s[5]  = "";                     // Description
    s[6]  = o_NameBuilder_v1.GetFileName(MODEL_CONNECTION);//sProjectName + ".ell";  // Modelo Conexao
    s[7]  = o_NameBuilder_v1.GetFileName(MODEL_GEOMETRY);//sProjectName + ".ege";  // Modelo Geometria
    s[8]  = o_NameBuilder_v1.GetFileName(MODEL_COORDINATES);//sProjectName + ".ept";  // Modelo Coordinates
    s[9]  = o_NameBuilder_v1.GetFileName(MODEL_PRE);//sProjectName + ".epd";  // Modelo Pre;
    s[10] = o_NameBuilder_v1.GetFileName(GEOMETRY);//sProjectName + ".n1g";  // Geometria
    s[11] = o_NameBuilder_v1.GetFileName(SEGMENTS);//sProjectName + ".g1g";  // Segmentos
    s[12] = o_NameBuilder_v1.GetFileName(ELEMENTS);//sProjectName + ".t1g";  // Elementos
    s[13] = o_NameBuilder_v1.GetFileName(BOUNDARYCONDITIONS);//sProjectName + ".c1g";  // CC
    s[14] = o_NameBuilder_v1.GetFileName(MATERIAL);//sProjectName + ".m1g";  // Meios
    s[15] = o_NameBuilder_v1.GetFileName(SOURCE);//sProjectName + ".f1g";  // Fontes
    s[16] = o_NameBuilder_v1.GetFileName(POTENTIAL);//sProjectName + ".p1g";  // Potenciais
    s[17] = o_NameBuilder_v1.GetFileName(ARESTASGEO);//sProjectName + "_areas.ar";   // Arestas
    s[18] = o_NameBuilder_v1.GetFileName(ELEMENTOSARESTAS);//sProjectName + ".art";  // Elementos Arestas

//    std::string filename = project_name + ".proj";
    std::string filename = sProjectNameWithPath + "_TESTE.proj";

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

#if 10
    // Phenomenon type
    PHENOMENONTYPE phen= MAGNETOSTATIC;//vpPhenSpecificationVector[0]->PhenomenonTypeEnum();//SCALAROPTICALWAVE;
    switch(phen)
    {
    case ELECTROSTATIC:
        fprintf(far, "%c", 0x00);                     // 1
        for(i=0; i<3; i++)
           fprintf(far, "%c", 0x00);  // 3
        break;

    case ELECTROKINETIC:
        fprintf(far, "%c", 0x01);                     // 1
        for(i=0; i<3; i++)
           fprintf(far, "%c", 0x00);  // 3
        break;

    case MAGNETOSTATIC:
        fprintf(far, "%c", 0x02);                     // 1
        for(i=0; i<3; i++)
           fprintf(far, "%c", 0x00);  // 3
        break;

    case THERMALSTATIONARY:
        fprintf(far, "%c", 0x03);                     // 1
        for(i=0; i<3; i++)
           fprintf(far, "%c", 0x00);  // 3
        break;

    case SCALAROPTICALFIBER:
        fprintf(far, "%c", 0x04);                     // 1
        for(i=0; i<3; i++)
           fprintf(far, "%c", 0x00);  // 3
        break;

    case SCALAROPTICALWAVE:
       fprintf(far, "%c", 0x05);                     // 1
       for(i=0; i<3; i++)
          fprintf(far, "%c", 0x00);  // 3
       break;

    case TRANSIENTHEATCONDUCTION:
        fprintf(far, "%c", 0x11);                     // 1
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
#endif

    for(i=6;i<19;i++)
        if(s[i].size())
            fprintf(far, "%c%s", s[i].size(), s[i].c_str());
        else
            fprintf(far, "%c", 0);

    fclose(far);

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return codeErr;
}

void CProject_Model::SetFlagsReadSavePREPROJ(bool b_r, bool b_s) ///< flags para gravacao e leitura do arquivo de especificacao preliminar .preproj
{
    b_readPREPROJ = b_r;
    b_savePREPROJ = b_s;
}
   
void CProject_Model::SetFlagsReadSaveDADOS(bool b_r, bool b_s) ///< flags para gravacao e leitura do arquivo de especificacao temporario .dados (s ser substituido pelo .preproj)
{
    b_readDADOS = b_r;
    b_saveDADOS = b_s;
}
  
//arquivos de modelo geometrico
void CProject_Model::SetFlagsReadSaveMGP(bool b_r, bool b_s) ///< flags para gravacao e leitura do arquivo de primitivas .mgp
{
    b_readMGP = b_r;
    b_saveMGP = b_s;
}

void CProject_Model::SetFlagsReadSavePRX(bool b_r, bool b_s) ///< flags para gravacao e leitura do arquivo de primitivas AUXILIARES .prx
{
    b_readPRX = b_r;
    b_savePRX = b_s;
}

void CProject_Model::SetFlagsReadSaveRGN(bool b_r, bool b_s) ///< flags para gravacao e leitura do arquivo de regioes do model geometrico .rgn
{
    b_readRGN = b_r;
    b_saveRGN = b_s;
}

void CProject_Model::SetFlagsReadSaveMMF(bool b_r, bool b_s) ///< flags para gravacao e leitura do arquivo de malha de fronteira auxiliar as regioes do model geometrico .mmf
{
    b_readMMF = b_r;
    b_saveMMF = b_s;
}
    
//arquivos de modelo fisico
void CProject_Model::SetFlagsReadSaveMEI(bool b_r, bool b_s) ///< flags para gravacao e leitura do arquivo de banco de dados de materiais .mei
{
    b_readMEI = b_r;
    b_saveMEI = b_s;
}

void CProject_Model::SetFlagsReadSaveMTS(bool b_r, bool b_s) ///< flags para gravacao e leitura do arquivo de banco de dados de materiais em uso .mts (o indice do material esta associado aa regiao do modelo)
{
    b_readMTS = b_r;
    b_saveMTS = b_s;
}

void CProject_Model::SetFlagsReadSaveSRC(bool b_r, bool b_s) ///< flags para gravacao e leitura do arquivo de source em uso .src (o indice da fonte esta associado aa regiao do modelo)
{
    b_readSRC = b_r;
    b_saveSRC = b_s;
}
    
void CProject_Model::SetFlagsReadSaveBCO(bool b_r, bool b_s) ///< flags para gravacao e leitura do arquivo de condicoes de contorno .bco
{
    b_readBCO = b_r;
    b_saveBCO = b_s;
}
void CProject_Model::SetFlagsReadSaveBCP(bool b_r, bool b_s) ///< flags para gravacao e leitura do arquivo de primitivas com condicoes de contorno .bcp
{
    b_readBCP = b_r;
    b_saveBCP = b_s;
}

// =====

int CProject_Model::ReadPRJ(std::string project_filename)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    short        posi,posf, pos1, size;
    short        i;
    keyandvalue struct_udata;

    bool bcreatespec = false;

    std::cout << "\nCProject_Model::ReadPRJ : project_filename = " << project_filename.c_str() << std::endl; std::cout.flush();

    std::ifstream fin(project_filename.c_str());

    std::string text_string, text_string_1;

    readPRJ = 0;
    int codeErr = 0;

    if(fin.fail())
    {
        std::cout << "\nErro na abertura do arquivo de projeto: " << project_filename.c_str() << std::endl;
        readPRJ = 1; //erro na abertura do .prj
    }
    else
    {
        if(!vpPhenSpecificationVector.size()) bcreatespec = true;
        else if(vpPhenSpecificationVector[0]==NULL)
        {
            vpPhenSpecificationVector.clear();
            bcreatespec = true;
        }

        if(bcreatespec == true)
        codeErr = CreateAndIncludePhenSpecification();

        if(!codeErr)
        {//
            fin >> text_string;

            if(text_string == "<project>")
            {
                fin >> text_string;

                while(text_string != "</project>" && !readPRJ )
                {
                    if(fin.eof())
                    {
                        readPRJ = 1;
                        return readPRJ;
                    }

                    if( (posi=text_string.find("<")==0) )
                    {
                        posf = text_string.find(">");

                        size = text_string.size()-2;
                        text_string_1 = "";
                        text_string_1.resize(size);

                        //copia o texto interno a <>
                        for(i=1;i<posf;i++)
                            text_string_1[i-1] = text_string[i];

                        struct_udata.item = text_string_1;
                        trim(struct_udata.item);

                        fin >> text_string;

                        while( !((posi=text_string.find("<")==0) && (pos1=text_string.find("/")==1)) )
                        {
                            if(fin.eof())
                            {
                                readPRJ = 1;
                                return readPRJ;
                            }

                            struct_udata.content += (" " + text_string);
                            trim(struct_udata.content);

                            fin >> text_string;
                        }

                        //*NMA*: para n�o fixar os n�meros, como ocorre hoje pelos ENUMs, � melhor
                        // usar somente os nomes
#if 0
                        if(struct_udata.item == "study") str_phenomenon_type = struct_udata.content;
                        else if(struct_udata.item == "version") str_version = struct_udata.content;
                        else if(struct_udata.item == "length_unit") str_length_unit = struct_udata.content;
                        else if(struct_udata.item == "mass_unit") str_mass_unit = struct_udata.content;
                        else if(struct_udata.item == "time_unit") str_time_unit = struct_udata.content;
                        else if(struct_udata.item == "angle_unit") str_angle_unit = struct_udata.content;
                        else if(struct_udata.item == "temperature_unit") str_temperature_unit = struct_udata.content;
                        else if(struct_udata.item == "mesh_refinement") str_mesh_type = struct_udata.content;
                        else if(struct_udata.item == "symmetry") str_symmetry_type = struct_udata.content;
                        else userdatasettings.push_back(struct_udata);
                        struct_udata.item="";struct_udata.content = "";

                        fin >> text_string;
#endif
                        if(struct_udata.item == "study") vpPhenSpecificationVector[GetCurrentPhenIndex()]->PhenomenonName(struct_udata.content);
                        //else if(struct_udata.item == "version") str_version = struct_udata.content;
                        else if(struct_udata.item == "length_unit") vpPhenSpecificationVector[GetCurrentPhenIndex()]->LengthUnit(struct_udata.content);
                        else if(struct_udata.item == "mass_unit") vpPhenSpecificationVector[GetCurrentPhenIndex()]->MassUnit(struct_udata.content);
                        else if(struct_udata.item == "time_unit") vpPhenSpecificationVector[GetCurrentPhenIndex()]->TimeUnit(struct_udata.content);
                        else if(struct_udata.item == "angle_unit") vpPhenSpecificationVector[GetCurrentPhenIndex()]->AngleUnit(struct_udata.content);
                        else if(struct_udata.item == "temperature_unit") vpPhenSpecificationVector[GetCurrentPhenIndex()]->TemperatureUnit(struct_udata.content);
                        else if(struct_udata.item == "mesh_refinement")
                            {
                            //std::cout << "\nCProject_Model::ReadPRJ : struct_udata.content = " <<  struct_udata.content.c_str() ; std::cout.flush();

                                if(struct_udata.content != "arithmetic")
                                 if(struct_udata.content != "geometric")
                                 if(struct_udata.content != "harmonic")
                                 if(struct_udata.content != "squared_harmonic")
                                 if(struct_udata.content != "self_adaptive")
                                 if(struct_udata.content != "fronteira")
                                 if(struct_udata.content != "media_vizinhos")
                                 {
                                     struct_udata.content = "geometric"; //*NMA* atribui valor default
                                 }

                                vpPhenSpecificationVector[GetCurrentPhenIndex()]->MeshType(struct_udata.content);

                            }
                        else if(struct_udata.item == "symmetry") Symmetry(struct_udata.content);
                        else userdatasettings.push_back(struct_udata);
                        struct_udata.item="";struct_udata.content = "";

                        fin >> text_string;

                    }
                }
            }

            fin.close();
        }//fecha else if(!codeErr)

#if 0
        std::cout << "\nCProject_Model::ReadPRJ : vpPhenSpecificationVector[0]->PhenomenonName() = " <<  vpPhenSpecificationVector[0]->PhenomenonName().c_str() ; std::cout.flush();
        std::cout << "\nCProject_Model::ReadPRJ :  vpPhenSpecificationVector[0]->LengthUnit() = " <<   vpPhenSpecificationVector[0]->LengthUnit().c_str() ; std::cout.flush();
        std::cout << "\nCProject_Model::ReadPRJ : vpPhenSpecificationVector[0]->MassUnit() = " <<  vpPhenSpecificationVector[0]->MassUnit().c_str() ; std::cout.flush();
        std::cout << "\nCProject_Model::ReadPRJ : vpPhenSpecificationVector[0]->TimeUnit() = " <<  vpPhenSpecificationVector[0]->TimeUnit().c_str() ; std::cout.flush();
        std::cout << "\nCProject_Model::ReadPRJ : vpPhenSpecificationVector[0]->AngleUnit() = " <<  vpPhenSpecificationVector[0]->AngleUnit().c_str() ; std::cout.flush();
        std::cout << "\nCProject_Model::ReadPRJ : vpPhenSpecificationVector[0]->TemperatureUnit() = " <<  vpPhenSpecificationVector[0]->TemperatureUnit().c_str() ; std::cout.flush();
        std::cout << "\nCProject_Model::ReadPRJ : vpPhenSpecificationVector[0]->MeshType() = " <<  vpPhenSpecificationVector[0]->MeshType().c_str() ; std::cout.flush();
        std::cout << "\nCProject_Model::ReadPRJ :  Symmetry = " <<   Symmetry().c_str() << std::endl; std::cout.flush();
#endif
        //BuildFileNames();
    }
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return readPRJ;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//*NMA*2016-06-07: descontinuado, usar SaveUserDataSettings
//-----------------------------------------------------------------
int CProject_Model::SavePRJ(std::string project_filename)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
//    short        posi,posf, pos1, size;
    short        i;
//    keyandvalue struct_udata;

    std::ofstream fout(project_filename.c_str());

    std::string            item;
    std::string            content;

    std::string            item_aux;
    std::string            content_aux;

    savedPRJ = 0;

    if(fout.fail())
    {
        std::cout << "\nErro na abertura do arquivo de projeto: " << project_filename.c_str() << std::endl;
        savedPRJ = 1; //erro na abertura do .prj
    }
    else
    {
        fout << "<project>" << "\n";

        fout << "<version> ";
        fout << "v001 ";// str_version;
        fout << " </version>" << "\n";

        fout << "<study> ";
        fout << vpPhenSpecificationVector[0]->PhenomenonName().c_str();
        fout << " </study>" << "\n";

        fout << "<length_unit> ";
        fout << vpPhenSpecificationVector[0]->LengthUnit().c_str();
        fout << " </length_unit>" << "\n";

        fout << "<mass_unit> ";
        fout << vpPhenSpecificationVector[0]->MassUnit();
        fout << " </mass_unit>"  << "\n";

        fout << "<time_unit> ";
        fout << vpPhenSpecificationVector[0]->TimeUnit();
        fout << " </time_unit>"  << "\n";

        fout << "<angle_unit> ";
        fout << vpPhenSpecificationVector[0]->AngleUnit();
        fout << " </angle_unit>"  << "\n";

        fout << "<temperature_unit> ";
        fout << vpPhenSpecificationVector[0]->TemperatureUnit();
        fout << " </temperature_unit>"  << "\n";

        fout << "<mesh_refinement> ";
        fout << vpPhenSpecificationVector[0]->MeshType();
        fout << " </mesh_refinement>"  << "\n";

        fout << "<symmetry> ";
        fout << Symmetry().c_str();
        fout << " </symmetry>"  << "\n";

        //userdata.push_back(struct_udata);
        for(i=0;i<Get_NumberofItens();i++)
        {
            Get_Item(i, item_aux);
            Get_Content(i, content);

            item = "<"+item_aux + "> ";
            fout << item.c_str();
            fout << content.c_str();
            item = " </"+item_aux + ">";
            fout << item.c_str() << "\n";

        }

        fout << "</project>";
        fout.close();
    }

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return savedPRJ;
}
//-----------------------------------------------------------------

int CProject_Model::SaveUserDataSettings(std::string project_filename)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
//    short        posi,posf, pos1, size;
    short        i,j;
//    keyandvalue struct_udata;

    std::ofstream fout(project_filename.c_str());

    std::string            str_item;
    std::string            str_content;
    std::string            item_aux;
    std::string            content_aux;

    savedPRJ = 0;

    UpdateSettingsfromPhenSpecification();

    if(fout.fail())
    {
        std::cout << "\nErro na abertura do arquivo de projeto: " << project_filename.c_str() << std::endl;
        savedPRJ = 1; //erro na abertura do .prj
    }
    else
    {
        fout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\n";
        fout << "<project_settings>" << "\n";
        fout << "<study type=\""<< str_study_name.c_str()<<"\">" << "\n";
        for(i=0;i<vphen_userdatasettings.size();i++)
        {
            fout << "<phenomenon name=\""<< vpPhenSpecificationVector[i]->PhenomenonName().c_str()<<"\">" << "\n";

            for(j=0;j<vphen_userdatasettings[i].userdatasettings.size();j++)
            {
                str_item = vphen_userdatasettings[i].userdatasettings[j].item;
                str_content = vphen_userdatasettings[i].userdatasettings[j].content;

                fout << "<" << str_item.c_str() << ">" << str_content.c_str() << "</" <<str_item.c_str() << ">"  << "\n";
            }
            fout << "</phenomenon>" << "\n";
        }
        fout << "</study>" << "\n";
        fout << "</project_settings>";

#if 0
        fout << "v001 ";// str_version;
        fout << " </version>" << "\n";

        fout << "<study> ";
        fout << vpPhenSpecificationVector[0]->PhenomenonName().c_str();
        fout << " </study>" << "\n";

        fout << "<length_unit> ";
        fout << vpPhenSpecificationVector[0]->LengthUnit().c_str();
        fout << " </length_unit>" << "\n";

        fout << "<mass_unit> ";
        fout << vpPhenSpecificationVector[0]->MassUnit();
        fout << " </mass_unit>"  << "\n";

        fout << "<time_unit> ";
        fout << vpPhenSpecificationVector[0]->TimeUnit();
        fout << " </time_unit>"  << "\n";

        fout << "<angle_unit> ";
        fout << vpPhenSpecificationVector[0]->AngleUnit();
        fout << " </angle_unit>"  << "\n";

        fout << "<temperature_unit> ";
        fout << vpPhenSpecificationVector[0]->TemperatureUnit();
        fout << " </temperature_unit>"  << "\n";

        fout << "<mesh_refinement> ";
        fout << vpPhenSpecificationVector[0]->MeshType();
        fout << " </mesh_refinement>"  << "\n";

        fout << "<symmetry> ";
        fout << Symmetry().c_str();
        fout << " </symmetry>"  << "\n";

        //userdata.push_back(struct_udata);
        for(i=0;i<Get_NumberofItens();i++)
        {
            Get_Item(i, item_aux);
            Get_Content(i, content);

            item = "<"+item_aux + "> ";
            fout << item.c_str();
            fout << content.c_str();
            item = " </"+item_aux + ">";
            fout << item.c_str() << "\n";

        }

        fout << "</project>";
#endif
        fout.close();
    }

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return savedPRJ;
}

int CProject_Model::Get_NumberofItens(void)
{
    int num = userdatasettings.size() ;
    return num;
}

void CProject_Model::Get_Item(int index,std::string &item_name)
{
    if(index >=0 && index <userdatasettings.size() )
        item_name = userdatasettings[index].item;
}

void CProject_Model::Get_Content(int index,std::string &content_name)
{
    if(index >=0 && index <userdatasettings.size() )
        content_name = userdatasettings[index].content;
}

/**Gravacao do arquivo de especifica��o preliminar (*.dados)
*/
int    CProject_Model:: SaveSpecDataFile(std::string filename)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    int codeErr = 0;
    int iaux;
    std::string text;
    std::string string_aux;
    CLevMessage    mensagem;
    bool bcreatespec = false;

    std::ofstream fout(filename.c_str());
    if(fout.fail())
    {
        codeErr = 2;
        text = " Erro openning file  " + filename;
        mensagem.SetMessageData(codeErr,"CProject_Model:: SaveSpecDataFile()", text,MSGERROR);
        SetErrorMessage(mensagem);
        return codeErr;
    }
    else
    {

            //fin>> string_aux; //    nome do fen�meno, ex: electrostatic
            //vpPhenSpecificationVector[0]->PhenomenonName(string_aux);
            fout << vpPhenSpecificationVector[0]->PhenomenonName().c_str();
            fout << std::endl;
            fout << Dimension().c_str();
            //fin>> string_aux; //    dimens�o do estudo, ex: two_dimensional
            //Dimension(string_aux);// a fun��o atualiza tanto a string quanto a
                                    // vari�vel iProjectDimension
            fout << std::endl;//"\n";

            fout << vpPhenSpecificationVector[0]->ElementType();
            //fin>> string_aux;//    tipo de elemento, ex: simplex_2D
            //vpPhenSpecificationVector[0]->ElementType(string_aux);
            fout << std::endl;//"\n";

            fout << vpPhenSpecificationVector[0]->ElementOrder();
            //fin>> iaux;//    ordem do elemento, num�rico
            //vpPhenSpecificationVector[0]->ElementOrder(iaux);
            fout << std::endl;//"\n";

            fout << vpPhenSpecificationVector[0]->DegreesOfFreedom();
            //fin>> iaux;//    graus de liberdade, numerico
            //vpPhenSpecificationVector[0]->DegreesOfFreedom(iaux);

            fout.close();
    }// fecha else

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return codeErr;
}

// Obtem nome do arquivo executavel do explorer (a ser chamado pela opcao de menu)
std::string    CProject_Model::Get_ExplorerFileName(void)
{
    return str_explorer_executable_file;
}

///< Atribui nome do arquivo executavel do explorer
void    CProject_Model::Set_ExplorerFileName(std::string name)
{
    str_explorer_executable_file = name;
}

// Obtem lista dos arquivos executaveis do explorer
std::vector<std::string>    CProject_Model::Get_ExplorerFileNameList(void)
{
    return vtr_explorer_executable_file_list;
}

///< Atribui lista dos arquivos executaveis do explorer
void CProject_Model::Set_ExplorerFileNameList(std::vector<std::string> vtr_name)
{
    std::cout << "\nCProject_Model::Set_ExplorerFileNameList - vtr_name.size() = " << vtr_name.size(); std::cout.flush();
    vtr_explorer_executable_file_list = vtr_name;
}

// Obtem nome do arquivo executavel do solver (a ser chamado pela opcao de menu)
std::string    CProject_Model::Get_SolverFileName(void)
{
    return str_solver_executable_file;
}

///< Atribui nome do arquivo executavel do solver
void    CProject_Model::Set_SolverFileName(std::string name)
{
    str_solver_executable_file = name;
}

// Obtem lista dos arquivos executaveis do solver
std::vector<std::string>    CProject_Model::Get_SolverFileNameList(void)
{
    return vtr_solver_executable_file_list;
}

///< Atribui lista dos arquivos executaveis do solver
void CProject_Model::Set_SolverFileNameList(std::vector<std::string> vtr_name)
{
    vtr_solver_executable_file_list = vtr_name;
}

bool CProject_Model::Save_CfgExeFilesXML(std::string filename)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    bool savedFile = false;

    short        i;

    std::ofstream fout(filename.c_str());

    std::string            item;
    std::string            content;

    std::string            item_aux;
    std::string            content_aux;

    if(fout.fail())
    {
        std::cout << "\nCProject_Model::Save_CfgExeFilesXML : Erro na abertura do arquivo de configuracao: " << filename.c_str() << std::endl; std::cout.flush();
        savedFile = false; //erro
    }
    else
    {
        fout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\n";
        fout << "<files_config>" << "\n";
        fout << "    <exefile category=\"solver\">" << "\n";
        if(!str_solver_executable_file.empty())
            fout << "    <default>" << str_solver_executable_file.c_str() << "</default>" << "\n";
        for(i=0;i<vtr_solver_executable_file_list.size();i++)
        {
            fout << "    <list>" << vtr_solver_executable_file_list[i].c_str() << "</list>" << "\n";
        }
        fout << "    </exefile>" << "\n";
        fout << "    <exefile category=\"explorer\">" << "\n";
        if(!str_explorer_executable_file.empty())
            fout << "    <default>" << str_explorer_executable_file.c_str() << "</default>"<< "\n";
        for(i=0;i<vtr_explorer_executable_file_list.size();i++)
        {
            fout << "    <list>" << vtr_explorer_executable_file_list[i].c_str() << "</list>"<< "\n";
        }
        fout << "    </exefile>" << "\n";
        fout << "</files_config>" << "\n";
        fout.close();
        savedFile = true;
    }

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    return savedFile;
}

void CProject_Model::Set_UserDataSettings(std::vector<phen_struct> vphen_data) ///< Atribui dados de especificacao definidos pelo usuário
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    int i, j, codeErr;
    std::string str_phen;
    std::string str_item;
    std::string str_content;

    vphen_userdatasettings = vphen_data;

    bool bcreatespec = false;
    if(!vpPhenSpecificationVector.size())
    	bcreatespec = true;
    else
    {
        vpPhenSpecificationVector.clear();
        bcreatespec = true;
    }

    if(bcreatespec == true)
    {
        for(i=0;i<vphen_userdatasettings.size() && !codeErr;i++)
        {
            codeErr = CreateAndIncludePhenSpecification();
        }
    }

    if(!codeErr)
    {
        for(i=0;i<vphen_userdatasettings.size();i++)
        {
            str_phen = vphen_userdatasettings[i].phenomenon_name;
            vpPhenSpecificationVector[i]->PhenomenonName(str_phen);

            for(j=0;j<vphen_userdatasettings[i].userdatasettings.size();j++)
            {
                str_item = vphen_userdatasettings[i].userdatasettings[j].item;
                str_content = vphen_userdatasettings[i].userdatasettings[j].content;

                #if 0
                std::cout << "\nCProject_Model::Set_UserDataSettings :  vphen_userdatasettings[i].userdatasettings[j].item.c_str() = " <<  str_item.c_str() ; std::cout.flush();
                std::cout << "\nCProject_Model::Set_UserDataSettings :  vphen_userdatasettings[i].userdatasettings[j].content.c_str() = " <<  str_content.c_str() ; std::cout.flush();
                #endif

                if(str_item == "length") vpPhenSpecificationVector[i]->LengthUnit(str_content);
                else if(str_item == "mass") vpPhenSpecificationVector[i]->MassUnit(str_content);
                else if(str_item == "time") vpPhenSpecificationVector[i]->TimeUnit(str_content);
                else if(str_item == "angle") vpPhenSpecificationVector[i]->AngleUnit(str_content);
                else if(str_item == "temperature") vpPhenSpecificationVector[i]->TemperatureUnit(str_content);
                else if(str_item == "mesh_refinement")
				{
					if(str_content != "arithmetic")
					if(str_content != "geometric")
					if(str_content != "harmonic")
					if(str_content != "squared_harmonic")
					if(str_content != "self_adaptive")
					if(str_content != "fronteira")
					if(str_content != "media_vizinhos")
					{
						str_content = "geometric"; //*NMA* atribui valor default
					}
					vpPhenSpecificationVector[i]->MeshType(str_content);
				}
                else if(str_item == "symmetry") Symmetry(str_content);
            }
        }
    }

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
}

void CProject_Model::SetStudyName(std::string n)
{
    str_study_name = n;
}

std::string    CProject_Model::GetStudyName(void)
{
    return str_study_name;
}

void CProject_Model::InitializeUserDataSettings(void) ///< Inicializa dados de especificacao caso a leitura nao tenha sido feita....
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    int codeErr=0;

    phen_struct phen_data;
    keyandvalue keyandvalue_user;

    std::string str_item;
    std::string str_content;

#if 0
    bool bcreatespec = false;
    if(!vpPhenSpecificationVector.size()) bcreatespec = true;
    else
    {
        vpPhenSpecificationVector.clear();
        bcreatespec = true;
    }

    if(bcreatespec == true)
    {
        codeErr = CreateAndIncludePhenSpecification();
    }
#endif
    if(!vpPhenSpecificationVector.size())
    {
        codeErr = CreateAndIncludePhenSpecification();
    }

    if(!codeErr)
    {
        str_study_name = vpPhenSpecificationVector[0]->PhenomenonName();
        phen_data.phenomenon_name =  vpPhenSpecificationVector[0]->PhenomenonName();

        keyandvalue_user.item = "length";
        keyandvalue_user.content = vpPhenSpecificationVector[0]->LengthUnit();
        phen_data.userdatasettings.push_back(keyandvalue_user);

        keyandvalue_user.item = "mass";
        keyandvalue_user.content = vpPhenSpecificationVector[0]->MassUnit();
        phen_data.userdatasettings.push_back(keyandvalue_user);

        keyandvalue_user.item = "time";
        keyandvalue_user.content = vpPhenSpecificationVector[0]->TimeUnit();
        phen_data.userdatasettings.push_back(keyandvalue_user);

        keyandvalue_user.item = "angle";
        keyandvalue_user.content = vpPhenSpecificationVector[0]->AngleUnit();
        phen_data.userdatasettings.push_back(keyandvalue_user);

        keyandvalue_user.item = "temperature";
        keyandvalue_user.content = vpPhenSpecificationVector[0]->TemperatureUnit();
        phen_data.userdatasettings.push_back(keyandvalue_user);

        keyandvalue_user.item = "mesh_refinement";
        keyandvalue_user.content = vpPhenSpecificationVector[0]->MeshType();
        phen_data.userdatasettings.push_back(keyandvalue_user);

        keyandvalue_user.item = "symmetry";
        keyandvalue_user.content = Symmetry();
        phen_data.userdatasettings.push_back(keyandvalue_user);

        vphen_userdatasettings.push_back(phen_data);
    }

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
}

void CProject_Model::UpdateSettingsfromPhenSpecification(void)
{
    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [START] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
    int i,j;

    std::string str_item;
    std::string str_content;
    std::string str_content_phen;

    if(!vphen_userdatasettings.size() )
        InitializeUserDataSettings();

    for(i=0;i<vphen_userdatasettings.size();i++)
    {
        vphen_userdatasettings[i].phenomenon_name = vpPhenSpecificationVector[i]->PhenomenonName();

        for(j=0;j<vphen_userdatasettings[i].userdatasettings.size();j++)
        {
            str_item = vphen_userdatasettings[i].userdatasettings[j].item;
            str_content = vphen_userdatasettings[i].userdatasettings[j].content;

            str_content_phen = str_content; //copia conteudo original

			#if 0
            std::cout << "\nCProject_Model::UpdateSettingsfromPhenSpecification :  vphen_userdatasettings[i].userdatasettings[j].item.c_str() = " <<  str_item.c_str() ; std::cout.flush();
            std::cout << "\nCProject_Model::UpdateSettingsfromPhenSpecification :  vphen_userdatasettings[i].userdatasettings[j].content.c_str() = " <<  str_content.c_str() ; std::cout.flush();
			#endif

            if(str_item == "length") str_content_phen = vpPhenSpecificationVector[i]->LengthUnit();
            else if(str_item == "mass") str_content_phen = vpPhenSpecificationVector[i]->MassUnit();
            else if(str_item == "time") str_content_phen = vpPhenSpecificationVector[i]->TimeUnit();
            else if(str_item == "angle") str_content_phen = vpPhenSpecificationVector[i]->AngleUnit();
            else if(str_item == "temperature") str_content_phen = vpPhenSpecificationVector[i]->TemperatureUnit();
            else if(str_item == "mesh_refinement") str_content_phen = vpPhenSpecificationVector[i]->MeshType();
            else if(str_item == "symmetry") str_content_phen = Symmetry();

            //caso seja um dos itens "fixos", atualiza com o dado inserido em vpPhenSpecificationVector, caso nao seja mantem o dado anterior
            vphen_userdatasettings[i].userdatasettings[j].content = str_content_phen;
        }
    }

    if(vphen_userdatasettings.size() )
        str_study_name = vphen_userdatasettings[0].phenomenon_name;

    #ifdef __DEBUG_MODE__
    std::cout << "\n" << __FUNCTION__ << " [END] (" << __FILE__ << ":" << __LINE__ << ")"; std::cout.flush();
    #endif
}

}
