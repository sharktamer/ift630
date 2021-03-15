#ifndef _OCL_CONTEXTE_H_
#define _OCL_CONTEXTE_H_

#include <CL/cl.hpp>
#include <iostream>
#include <utility>
#include <fstream>
#include <string>

class Contexte
{
    std::vector<cl::Platform> mes_fabricants;
    std::vector<cl::Device> mes_gpu;
    cl::Platform fabricants_par_defaut ;
    cl::Device gpu_par_defaut; 
    cl::Context contexte;
    cl::CommandQueue listeCmd;
    cl_int err;

public:
    Contexte();
    int obtenirInfo();
    int init();
    cl::Context obtenirContexte(){return contexte;}
    cl::CommandQueue obtenirCmd(){return listeCmd;}
    cl::Device obtenirDevice(){return gpu_par_defaut;}
    
};

Contexte::Contexte()
{
    int err ;
    err = init();
    std::cout << "Résultat de l'initialisation = " << err << std::endl;

}
int Contexte::obtenirInfo()
{
    int cpt;
    cl::Platform::get(&mes_fabricants);

    if (mes_fabricants.size() == 0) 
    {
        std::cerr << "Error: Failed to find a platform!" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "---------------------" << std::endl << "fabricants detectee " << std::endl;
    cpt = 0;
    for (cl::Platform fabricants : mes_fabricants) 
    {
        std::cout << " ["<<cpt<<"] ---> "<<fabricants.getInfo<CL_PLATFORM_NAME>()<< std::endl;
        cpt++;
    }
    std::cout << "---------------------" << std::endl;

    fabricants_par_defaut = mes_fabricants[0];
    
    std::cout << "fabricants par defaut "<<fabricants_par_defaut.getInfo<CL_PLATFORM_NAME>()<< std::endl;  
   //get default device of the default platform
   
    fabricants_par_defaut.getDevices(CL_DEVICE_TYPE_ALL, &mes_gpu);
    if(mes_gpu.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        return EXIT_FAILURE;
    }
    std::cout << "---------------------" << std::endl << "GPU detectee " << std::endl;
    cpt = 0;
    for (cl::Device gpu : mes_gpu) 
    {
        std::cout << " ["<<cpt<<"] ---> " << gpu.getInfo<CL_DEVICE_NAME>()<< std::endl;
        cpt++;
    }
    std::cout << "---------------------" << std::endl;

    gpu_par_defaut=mes_gpu[0];
    std::cout<< "Utilise par defaut gpu : "<<gpu_par_defaut.getInfo<CL_DEVICE_NAME>()<<std::endl ;
    return 0;
} 


//-----------------------------------------------------------------------
// Creation d'un contexte pour executer un programme OpenCL sur NVIDIA GPU
//----------------------------------------------------------------------
int Contexte::init()
{
    cl::Platform::get(&mes_fabricants);
    if (mes_fabricants.size() == 0) 
    {
        std::cerr << "Error: Failed to find a platform!" << std::endl;
        return EXIT_FAILURE;
    }
    fabricants_par_defaut = mes_fabricants[0];
    std::cout << "fabricants par defaut "<<fabricants_par_defaut.getInfo<CL_PLATFORM_NAME>()<< std::endl;  

    cl_context_properties properties[] =
    { CL_CONTEXT_PLATFORM, (cl_context_properties)(fabricants_par_defaut)(), 0};
    contexte = cl::Context(CL_DEVICE_TYPE_GPU, properties);
    mes_gpu = contexte.getInfo<CL_CONTEXT_DEVICES>();
    std::cout << "Nombre de cartes GPU " << mes_gpu.size()<< std::endl;
    gpu_par_defaut=mes_gpu[0];
    std::cout<< "Utilise par defaut gpu : "<<gpu_par_defaut.getInfo<CL_DEVICE_NAME>()<<std::endl ;
    listeCmd = cl::CommandQueue(contexte, gpu_par_defaut, 0, &err);

    return 0;
}

#endif //_OCL_CONTEXTE_H_
