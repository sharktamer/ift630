#ifndef _OPENCL_PROGRAM_H_
#define _OPENCL_PROGRAM_H_

#include "contexte.h"
#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <string>


const int ENTREE = CL_MEM_READ_ONLY;
const int SORTIE = CL_MEM_READ_WRITE;
const int ES     = CL_MEM_READ_WRITE;

const char* oclErrorString(cl_int error)
{
    static const char* errorString[] = {
        "CL_SUCCESS",
        "CL_DEVICE_NOT_FOUND",
        "CL_DEVICE_NOT_AVAILABLE",
        "CL_COMPILER_NOT_AVAILABLE",
        "CL_MEM_OBJECT_ALLOCATION_FAILURE",
        "CL_OUT_OF_RESOURCES",
        "CL_OUT_OF_HOST_MEMORY",
        "CL_PROFILING_INFO_NOT_AVAILABLE",
        "CL_MEM_COPY_OVERLAP",
        "CL_IMAGE_FORMAT_MISMATCH",
        "CL_IMAGE_FORMAT_NOT_SUPPORTED",
        "CL_BUILD_PROGRAM_FAILURE",
        "CL_MAP_FAILURE",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "CL_INVALID_VALUE",
        "CL_INVALID_DEVICE_TYPE",
        "CL_INVALID_PLATFORM",
        "CL_INVALID_DEVICE",
        "CL_INVALID_CONTEXT",
        "CL_INVALID_QUEUE_PROPERTIES",
        "CL_INVALID_COMMAND_QUEUE",
        "CL_INVALID_HOST_PTR",
        "CL_INVALID_MEM_OBJECT",
        "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
        "CL_INVALID_IMAGE_SIZE",
        "CL_INVALID_SAMPLER",
        "CL_INVALID_BINARY",
        "CL_INVALID_BUILD_OPTIONS",
        "CL_INVALID_PROGRAM",
        "CL_INVALID_PROGRAM_EXECUTABLE",
        "CL_INVALID_KERNEL_NAME",
        "CL_INVALID_KERNEL_DEFINITION",
        "CL_INVALID_KERNEL",
        "CL_INVALID_ARG_INDEX",
        "CL_INVALID_ARG_VALUE",
        "CL_INVALID_ARG_SIZE",
        "CL_INVALID_KERNEL_ARGS",
        "CL_INVALID_WORK_DIMENSION",
        "CL_INVALID_WORK_GROUP_SIZE",
        "CL_INVALID_WORK_ITEM_SIZE",
        "CL_INVALID_GLOBAL_OFFSET",
        "CL_INVALID_EVENT_WAIT_LIST",
        "CL_INVALID_EVENT",
        "CL_INVALID_OPERATION",
        "CL_INVALID_GL_OBJECT",
        "CL_INVALID_BUFFER_SIZE",
        "CL_INVALID_MIP_LEVEL",
        "CL_INVALID_GLOBAL_WORK_SIZE",
    };   
    const int errorCount = sizeof(errorString) / sizeof(errorString[0]);

    const int index = -error;

    return (index >= 0 && index < errorCount) ? errorString[index] : "";

}


class Program
{
    std::ifstream fichierSource;
	cl_int i;
    cl::Kernel noyau;
    cl::Buffer parametres[10];
    cl::Buffer resultat;
    int tailleResultat;
    int nbPara=0;
    cl::Context contexte;
    cl::CommandQueue listeCmd;
    cl::Device gpu; 
    cl_int err;	
public :
    Program(Contexte, std::string, std::string );
    void init(Contexte, std::string, std::string );
    template<typename T>
    void initPara( T para, int taille, int sens);
    template<typename T, typename... Args>
    void initPara(T para, int taille, int sens, Args... args);
    template<typename T>
    void initPara(T para);
    template<typename T, typename... Args>
    void initPara(T para, Args... args);
    template<typename T> 
    void execute(int dim, int nbPcs, int nbGroupe, T resul);
    //~Program();

};

Program::Program(Contexte enviro, std::string nomFichier, std::string  nomKernel)
{
    init(enviro, nomFichier, nomKernel);
}

void Program::init(Contexte enviro, std::string nomFichier, std::string nomKernel)
{
    contexte = enviro.obtenirContexte();
    listeCmd = enviro.obtenirCmd();
    gpu = enviro.obtenirDevice();

    fichierSource.open(nomFichier);
    /* Load kernel source code */
    std::string code((std::istreambuf_iterator<char>(fichierSource)),
                     std::istreambuf_iterator<char>());
	cl::Program::Sources source(1, std::make_pair(code.c_str(), code.length()+1));
    // Make program of the source code in the context
    cl::Program program = cl::Program(contexte, source);
 
    // Build program for these specific devices
    //program.build(contexte.getInfo<CL_CONTEXT_DEVICES>());
    err =program.build(contexte.getInfo<CL_CONTEXT_DEVICES>());
    std::cout << "program.build: " <<  oclErrorString(err) << std::endl;
    if (err != CL_SUCCESS){
        std::cout << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(gpu) << std::endl;
        std::cout << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(gpu) << std::endl;
       std::cout << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu) << std::endl;
    }
    noyau = cl::Kernel(program, nomKernel.c_str(), &err); std::cout << " kernel " << err << std::endl;
}

template<typename T>
void Program::initPara(T para, int taille, int sens)
{
    parametres[nbPara] = cl::Buffer(contexte, sens , taille * sizeof(*para), NULL, &err);
    std::cout << "buffer = " << err << " , taille = " << sizeof(*para)<< std::endl;
    if (sens == ENTREE)
    {
        std::cout << "Ecriture du tampon de taille " << taille << std::endl;
        err = listeCmd.enqueueWriteBuffer(parametres[nbPara], CL_TRUE, 0, taille*sizeof(*para), para);
        std::cout << "Ecriture du tampon de taille " << taille << std::endl;
   }
    else if  (sens == SORTIE)
    {
        resultat = parametres[nbPara];
        tailleResultat = taille*sizeof(*para);
    }
    else if  (sens == ES)
    {
        err = listeCmd.enqueueWriteBuffer(parametres[nbPara], CL_TRUE, 0, taille*sizeof(*para), para);
    }
    std::cout << " noyau.setArg(" << nbPara << ", " << ")" << std::endl;
    err = noyau.setArg(nbPara, parametres[nbPara]); std::cout << "setarg = " << err << std::endl;
    std::cout << " No P = " << nbPara << " , Taille = " << sizeof(*para)<< " :: "<< tailleResultat << std::endl;
    nbPara++;
}

template<typename T, typename... Args>
void Program::initPara(T para, int taille, int sens, Args... args)
{
   
    initPara(para, taille, sens);

    initPara(args...);
 
}

template<typename T>
void Program::initPara(T para)
{
    std::cout << " noyau.setArg(" << nbPara << ", " << para << ")" << std::endl;
    err = noyau.setArg(nbPara, para);
    nbPara++;
}

template<typename T, typename... Args>
void Program::initPara(T para, Args... args)
{
    initPara(para);

    initPara(args...);
}

template<typename T>
void Program::execute(int dim, int nbPcs, int nbGroupe, T resul)
{
   if (dim == 1)
   { 
      err = listeCmd.enqueueNDRangeKernel(noyau, cl::NullRange, cl::NDRange(nbPcs), cl::NDRange(nbGroupe)); std::cout<< "Execution : " << err<< std::endl;
      listeCmd.finish(); std::cout << "terminé........" << std::endl;
      err = listeCmd.enqueueReadBuffer(resultat, CL_TRUE, 0, tailleResultat, resul); std::cout<< err<< std::endl;
   }
   else if   (dim == 2)
   { 
      err = listeCmd.enqueueNDRangeKernel(noyau, cl::NullRange, cl::NDRange(nbPcs, nbPcs), cl::NDRange(nbGroupe,nbGroupe)); 
std::cout<< "Execution : " << err<< std::endl;
      listeCmd.finish();std::cout << "terminé........" << std::endl;
      err = listeCmd.enqueueReadBuffer(resultat, CL_TRUE, 0, tailleResultat, resul); std::cout<< err<< std::endl;
   }
   else if   (dim == 3)
   { 
      std::cout << "On a trois dimension......" << std::endl;
      err = listeCmd.enqueueNDRangeKernel(noyau, cl::NullRange, cl::NDRange(nbPcs, nbPcs, nbPcs), cl::NDRange(nbGroupe, nbGroupe, nbGroupe)); std::cout<< "Execution : " <<  err<< std::endl;
std::cout << "resul = " << resul << std::endl;
     std::cout << "Taille = " << tailleResultat << std::endl;
      listeCmd.finish();std::cout << "terminé........" << std::endl;
     err = listeCmd.enqueueReadBuffer(resultat, CL_TRUE, 0, tailleResultat, resul); std::cout<< err<< std::endl;
   }
}

#endif //_OPENCL_PROGRAM_H_
