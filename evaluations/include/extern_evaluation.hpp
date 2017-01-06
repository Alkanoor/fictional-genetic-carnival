#ifndef EXTERN_EVALUATION_HPP
#define EXTERN_EVALUATION_HPP


#define EXTERN_EVALUATION_LOG

#include "logger/include/log_in_file.hpp"

#include <sys/wait.h>
#include <unistd.h>


///***************************************************************************************
/// Class that provides ability to evaluate individuals through an extern program
/// eval method can be used as evaluation in simple_selection_on_evaluation_one_block
///***************************************************************************************

template <typename T, size_t Population_size>
class Extern_Evaluation
{
    public:
        Extern_Evaluation(const std::string& program_name, const std::string& output_path, char gene_separator = ',', char individual_separator = ';'
                          #ifdef EXTERN_EVALUATION_LOG
                                , std::shared_ptr<Info_Warning_Error_Logger> logger = Easy_Log_In_File::getInfoLog()
                          #endif
                          , std::shared_ptr<Info_Warning_Error_Logger> error_logger = Easy_Log_In_File::getErrorLog());

        const std::array<T, Population_size>& eval(const std::array<std::vector<char>, Population_size>& population, Genotype& genes);
        T eval_atomic(const std::vector<char>& population, Genotype& genes);

    private:
        std::string program;
        std::string output_path;
        char gene_separator, individual_separator;

        std::shared_ptr<Info_Warning_Error_Logger> error_logger;
        #ifdef EXTERN_EVALUATION_LOG
            std::shared_ptr<Info_Warning_Error_Logger> info_logger;
        #endif

        std::array<T, Population_size> temporary;

        int fork_and_exec(const std::string& args);
};


template <typename T, size_t Population_size>
Extern_Evaluation<T, Population_size>::Extern_Evaluation(const std::string& program_name, const std::string& output_path, char gene_separator, char individual_separator
                                        #ifdef EXTERN_EVALUATION_LOG
                                              , std::shared_ptr<Info_Warning_Error_Logger> logger
                                        #endif
                                        , std::shared_ptr<Info_Warning_Error_Logger> error_logger) :
    program(program_name),
    output_path(output_path),
    gene_separator(gene_separator),
    individual_separator(individual_separator),
    error_logger(error_logger),
    info_logger(logger)
{}

template <typename T, size_t Population_size>
const std::array<T, Population_size>& Extern_Evaluation<T, Population_size>::eval(const std::array<std::vector<char>, Population_size>& population, Genotype& genes)
{
    std::ostringstream args;
    bool add_first = false;
    for(const std::vector<char>& individual : population)
    {
        if(!add_first)
            add_first = true;
        else
            args<<individual_separator;
        bool add_first2 = false;

        genes.interprete(individual);

        int a = genes.get_number_integer_genes();
        for(int i=0; i<a; i++)
        {
            if(!add_first2)
                add_first2 = true;
            else if(gene_separator)
                args<<gene_separator;
            args<<genes.get_gene_int(i).get_current_interpretation();
        }

        a = genes.get_number_float_genes();
        for(int i=0; i<a; i++)
        {
            if(!add_first2)
                add_first2 = true;
            else if(gene_separator)
                args<<gene_separator;
            args<<genes.get_gene_float(i).get_current_interpretation();
        }
    }

    fork_and_exec(args.str());

    std::ifstream ifs(output_path, std::ios::binary|std::ios::in);
    for(int i=0; i<(int)Population_size; i++)
        ifs>>temporary[i];

    return temporary;
}

template <typename T, size_t Population_size>
T Extern_Evaluation<T, Population_size>::eval_atomic(const std::vector<char>& individual, Genotype& genes)
{
    std::ostringstream args;

    bool add_first = false;
    genes.interprete(individual);

    int a = genes.get_number_integer_genes();
    for(int i=0; i<a; i++)
    {
        if(!add_first)
            add_first = true;
        else if(gene_separator)
            args<<gene_separator;
        args<<genes.get_gene_int(i).get_current_interpretation();
    }

    a = genes.get_number_float_genes();
    for(int i=0; i<a; i++)
    {
        if(!add_first)
            add_first = true;
        else if(gene_separator)
            args<<gene_separator;
        args<<genes.get_gene_float(i).get_current_interpretation();
    }

    fork_and_exec(args.str());

    T ret;
    std::ifstream ifs(output_path, std::ios::binary|std::ios::in);
    ifs>>ret;

    return ret;
}

template <typename T, size_t Population_size>
int Extern_Evaluation<T, Population_size>::fork_and_exec(const std::string& args)
{
    int pid = fork();
    int status = -1;
    if(pid<0)
    {
        error_logger->error("Error forking");
        exit(-1);
    }
    else if(!pid)
    {
        char* argv[3] = {(char*)program.c_str(), (char*)args.c_str(), NULL};
        if(execv(program.c_str(), argv)<0)
        {
            error_logger->error("Error during execv of ", program);
            exit(-1);
        }
    }
    else
    {
        #ifdef EXTERN_EVALUATION_LOG
            info_logger->write("Launching evaluation program ", program, " with child pid ", pid);
        #endif
        wait(&status);
        #ifdef EXTERN_EVALUATION_LOG
            info_logger->write("Program return status : ", status);
        #endif
    }

    return status;
}


#endif
