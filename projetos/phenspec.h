#ifndef STCPHENSPEC_H
#define STCPHENSPEC_H


#include "levenum.h"


namespace lev
{
//.............................................................................
// Implementado por: Nancy Mieko Abe, *NMA*
// Data: 22/05/2006
//............................................................................
/**
 * Projeto: SDK-LEVSOFT
 *
 * O objetivo desta classe é armazenar informações do fenômeno que podem ser especificados pelo projetista, tais como:
 * o metodo de resolusão da equações diferenciais parciais, o metodo de resolução do sistema de equações, o tipo de elemento, a ordem do elemento, etc..
 *
 * Usos previstos:	Aplicações para modelagem (ex:Modelador LEVSOFT ,especificações de projeto)
 *					Aplicações para análise de resultados (ex: Explorador LEVSOFT)
 *					Aplicações para resolusão (via classes de projeto) (ex: Solveres LEVSOFT)
 */
class CPhenSpecBase
{
protected:
        std::string sPhenomenonName;            ///< String que armazena o nome do fen�meno
        std::string sPDESolverMethod;           ///< String que armazena o nome do metodo para resolu��o das equa��es diferenciais parciais (FEM, EFG, FVM, etc...)
        std::string sSystemOfEquationsSolver;   ///< String que armazena o nome do metodo num�rico para resolu��o do sistema de equa��es (ICCG, ILUCG, ARNOLDI, ...)
        std::string sElementType;               ///< String que armazena o tipo de elemento
//        std::string sElementOrder;              ///< String que armazena a ordem do elemento
		int			iElementOrder;				///< ordem do elemento
        std::string sMeshType;                  ///< String que armazena o tipo de malha
        std::string sTimeUnit;                  ///< String que armazena a unidade de tempo
        std::string sTemperatureUnit;           ///< String que armazena a unidade de temperatura
        std::string sMassUnit;                  ///< String que armazena a unidade de massa
        std::string sLengthUnit;                ///< String que armazena a unidade de comprimento
        std::string sAngleUnit;                 ///< String que armazena a unidade de angulo
//		std::string sDegreesOfFreedom;			///< String que armazena o n�mero de graus de liberdade do fen�meno
		int			iDegreesOfFreedom;			///< o n�mero de graus de liberdade do fen�meno
		double		dSolvConvPrec;				///< Double que armazena a precis�o desejada para o solver
		int         nSolvConvSaveIteStep;		///< Int que armazena a cada quantos passos de itera��o do solver os dados ser�o gravados (se valor igual 0, dados n�o s�o gravados)

public:
		 CPhenSpecBase() {dSolvConvPrec=1e-7;iDegreesOfFreedom=1;iElementOrder=1; }		///< Construtor
		~CPhenSpecBase() {nSolvConvSaveIteStep=0;}     ///< Destrutor

		inline void PDESolverMethod(std::string name){sPDESolverMethod = name;}  ///< Opera��o que atribui o metodo de resolu��o das equa��es diferenciais parciais
		inline std::string PDESolverMethod(void){return sPDESolverMethod;}  ///< Opera��o que retorna o metodo de resolu��o das equa��es diferenciais parciais

        inline void SystemOfEquationsSolver(std::string str){sSystemOfEquationsSolver = str;}  ///< Opera��o que atribui o metodo de resolu��o do sistema de equa��es
        inline std::string SystemOfEquationsSolver(void){return sSystemOfEquationsSolver;}  ///< Opera��o que retorna o metodo de resolu��o do sistema de equa��es

        inline void AngleUnit(std::string str){sAngleUnit = str;}         ///< Opera��o que atribui a unidade de angulo
        inline std::string AngleUnit(void){return sAngleUnit;}         ///< Opera��o que retorna a unidade de angulo

        inline void ElementOrder(int o) { iElementOrder = o; }  ///< Opera��o que atribui a ordem do elemento
        inline int ElementOrder(void) {return iElementOrder;}  ///< Opera��o que retorna a ordem do elemento

        inline void ElementType(std::string str) {sElementType = str;}  ///< Opera��o que atribui tipo do elemento
        inline std::string ElementType(void){return sElementType;}  ///< Opera��o que retorna tipo do elemento

        inline void LengthUnit(std::string str){sLengthUnit = str;}  ///< Opera��o que atribui unidade de comprimento
        inline std::string LengthUnit(void) {return sLengthUnit;}  ///< Opera��o que retorna unidade de comprimento

        inline void MassUnit(std::string str) {sMassUnit = str;}  ///< Opera��o que atribui unidade de massa
        inline std::string MassUnit(void){return sMassUnit;}  ///< Opera��o que retorna unidade de massa

        inline void MeshType(std::string str){sMeshType = str;}  ///< Opera��o que atribui tipo de malha
        inline std::string MeshType(void){return sMeshType;}  ///< Opera��o que retorna tipo de malha
        inline MESHTYPE MeshTypeEnum(void)
		{
			MESHTYPE eMeshType = GEOMETRIC;

			if(sMeshType == "arithmetic")
				eMeshType = ARITHMETIC;
			else if(sMeshType == "geometric")
				eMeshType = GEOMETRIC;
			else if(sMeshType == "harmonic")
				eMeshType = HARMONIC;
			else if(sMeshType == "squared_harmonic")
				eMeshType = SQUARED_HARMONIC;
			else if(sMeshType == "self_adaptive")
				eMeshType = SELF_ADAPTIVE;
            else if(sMeshType == "fronteira")
            	eMeshType = FRONTEIRA;
            else if(sMeshType == "media_vizinhos")
            	eMeshType = MED_VIZ;
			return eMeshType;
		}  ///< Operacao que retorna tipo de malha

        inline PHENOMENONTYPE PhenomenonTypeEnum(void)
        {
        	PHENOMENONTYPE eType = UNKNOWNPHENOMENON;

			if(sPhenomenonName == "electrostatic")					eType = ELECTROSTATIC;
			else if(sPhenomenonName == "electrokinetic")			eType = ELECTROKINETIC;
			else if(sPhenomenonName == "magnetostatic")				eType = MAGNETOSTATIC;
			else if(sPhenomenonName == "thermmalstationary")		eType = THERMALSTATIONARY;
			else if(sPhenomenonName == "scalaropticalfiber")		eType = SCALAROPTICALFIBER;
        	else if(sPhenomenonName == "scalaropticalwave")     	eType = SCALAROPTICALWAVE;
        	else if(sPhenomenonName == "electrooptic")          	eType = ELETROOPTIC;
        	else if(sPhenomenonName == "magnetooptic")          	eType = MAGNETOOPTIC;
        	else if(sPhenomenonName == "thermooptic")          		eType = THERMOOPTIC;
        	else if(sPhenomenonName == "acustooptic")          		eType = ACUSTOOPTIC;
        	else if(sPhenomenonName == "vectorialwavepropagation")  eType = VECTORIALWAVEPROPAGATION;
        	else if(sPhenomenonName == "transientheatconduction")   eType = TRANSIENTHEATCONDUCTION;
        	else if(sPhenomenonName == "transportphenomena")   		eType = TRANSPORTPHENOMENA;

			return eType;
        }  ///< Operacao que retorna tipo de malha

        inline void PhenomenonName(std::string str) {sPhenomenonName = str;}///* std::cout << "\nem void PhenomenonName(std::string str): str=" << str; std::cout.flush();*/}  ///< Operacao que atribui nome do fenï¿½meno
        inline std::string PhenomenonName(void) {return sPhenomenonName;}  ///< Operacao que retorna nome do fenï¿½meno

        inline void TemperatureUnit(std::string str){sTemperatureUnit=str;}  ///< Operacao que atribui unidade de temperatura especificada.
        inline std::string TemperatureUnit(void){return sTemperatureUnit;}  ///< Operacao que retorna unidade de temperatura especificada.

        inline void TimeUnit(std::string str){sTimeUnit = str;}  ///< Operacao que atribui unidade de temperatura especificada para o projeto.
        inline std::string TimeUnit(void){return sTimeUnit;}  ///< Operacao que retorna unidade de temperatura especificada para o projeto.

		inline double GetSolverPrecision(void) {return dSolvConvPrec;}///< Operacao que atribui valor de precisao para o solver.
		inline double GetSolverSaveIteStep(void) {return nSolvConvSaveIteStep ;}///< Operacao que atribui a cada quantos passos de iteracao, os dados de convergencia do solver serï¿½o gravados (se valor igual 0, dados nï¿½o sï¿½o gravados).
		inline void SetSolverPrecision(double prec) {dSolvConvPrec = prec;}///< Operacao que atribui valor de precisao para o solver.
		inline void SetSolverSaveIteStep(int st) {nSolvConvSaveIteStep = st;}///< Operacao que atribui a cada quantos passos de iteracao, os dados de convergencia do solver serï¿½o gravados (se valor igual 0, dados nï¿½o sï¿½o gravados).

		//..............................................................................
		// Get_UnitFactorLength
		//*NMA*: 10/07/2007
		//implementar c�lculo de fatores para outras unidades de medida para o SI tamb�m
		//..............................................................................
		inline int Get_UnitFactorLength(void)///< Retorna o fator para transforma��o de metros na unidade escolhida pelo usu�rio
		{
			int unitFactor=0;
			if(sLengthUnit == "meter")
					unitFactor	= 0;
			else if(sLengthUnit == "centimeter")
					unitFactor	= -2;
			else if(sLengthUnit == "milimeter")
					unitFactor	= -3;
			else if(sLengthUnit == "micrometer")
					unitFactor	= -6;
			else if(sLengthUnit == "nanometer")
					unitFactor	= -9;
			else if(sLengthUnit == "angstrom")
					unitFactor	= -10;

			return unitFactor;
		}//end of Get_LengthUnit

        inline void DegreesOfFreedom(int dof) {iDegreesOfFreedom = dof;}  ///< Opera��o que atribui o numero de graus de liberdade
        inline int  DegreesOfFreedom(void){return iDegreesOfFreedom;}  ///< Opera��o que retorna o numero de graus de liberdade
};

}
#endif //STCPHENSPEC_H
