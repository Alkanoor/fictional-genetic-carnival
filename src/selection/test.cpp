#include "elit_selection.hpp"
#include "rank_selection.hpp"
#include "multi_selection.hpp"
#include "quality_selection.hpp"


int main(int argc, char* argv[])
{
    if(argc<2)
    {
        if(argc>0)
            std::cout<<"[-] Usage "<<argv[0]<<" [log directory]"<<std::endl;
        else
            std::cout<<"[-] Usage [log directory]"<<std::endl;
        return 1;
    }

    std::string base(argv[1]);

    std::vector<std::string> tests = {"rank","quality","elit_rank","elit_quality","multi"};
    std::vector<std::shared_ptr<Selection<10, double> > > selections;

    selections.push_back(std::shared_ptr<Selection<10, double> >(new Rank_Selection<10, double>()));
    selections.push_back(std::shared_ptr<Selection<10, double> >(new Quality_Selection<10, double>()));
    selections.push_back(std::shared_ptr<Selection<10, double> >(new Elit_Selection<10, double>(1,selections[0])));
    selections.push_back(std::shared_ptr<Selection<10, double> >(new Rank_Selection<10, double>(2,selections[1])));
    selections.push_back(std::shared_ptr<Selection<10, double> >(
        new Multi_Selection<4, 10, double, float>(
            {selections[0],selections[1],selections[2],selections[3]},
            {0.1,0.2,0.3,0.4})));

    std::array<10,double> qualities = {0.5,0.9,0.75,0.55,3,100,0.01,2.5,2.7,0.88};
    std::array<10,int> index;


    return 0;
}
