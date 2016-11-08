#include <cstdlib>
#include <fstream>
#include <iostream>

#include "elit_selection.hpp"
#include "rank_selection.hpp"
#include "multi_selection.hpp"
#include "quality_selection.hpp"


int main(int argc, char* argv[])
{
    if(argc<2)
    {
        if(argc>0)
            std::cout<<"[-] Usage "<<argv[0]<<" [log file] (number of tests)"<<std::endl;
        else
            std::cout<<"[-] Usage [log file] (number of tests)"<<std::endl;
        return 1;
    }

    std::string log_file(argv[1]);
    int n_tests = 5;
    if(argc>2)
        n_tests = atoi(argv[2]);

    std::vector<std::string> tests = {"rank","quality","elit_rank","elit_quality","multi"};
    std::vector<std::shared_ptr<Selection<10, double> > > selections;

    selections.push_back(std::shared_ptr<Selection<10, double> >(new Rank_Selection<10, double, float>()));
    selections.push_back(std::shared_ptr<Selection<10, double> >(new Quality_Selection<10, double>()));
    selections.push_back(std::shared_ptr<Selection<10, double> >(new Elit_Selection<10, double>(1, selections[0])));
    selections.push_back(std::shared_ptr<Selection<10, double> >(new Elit_Selection<10, double>(2, selections[1])));
    selections.push_back(std::shared_ptr<Selection<10, double> >(
        new Multi_Selection<4, 10, double, float>(
            {selections[0],selections[1],selections[2],selections[3]},
            {0.1,0.2,0.3,0.4})));

    std::cout<<"[+] Initialization finished"<<std::endl;

    std::array<double,10> qualities = {0.5,0.9,0.75,0.55,3,100,0.01,2.5,2.7,0.88};
    //std::array<int,10> index;

    int max_size = -1;
    for(auto s : tests)
        if((int)s.size()>max_size || max_size<0)
            max_size = (int)s.size();

    std::ofstream ofs(log_file, std::ios::trunc|std::ios::out);
    for(int i=0; i<n_tests; i++)
    {
        std::cout<<"[+] "<<i<<"th test"<<std::endl;
        ofs<<"========================"<<std::endl;
        ofs<<"Test "<<i<<std::endl;
        ofs<<"========================"<<std::endl;
        for(int j=0; j<(int)tests.size(); j++)
        {
            ofs<<tests[j]<<" :";
            for(int k=0; k<max_size+1; k++)
                ofs<<" ";
            auto temp_index = selections[j]->apply(qualities, 0);
            ofs<<temp_index<<std::endl;
        }
    }
    return 0;
}
