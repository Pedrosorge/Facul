#ifndef QUICK_SORT
#define QUICK_SORT

#include <vector>
#include <string>
#include "../Interfaces/Interfaces.h"

class QuickSort : public ISortAlgorithm{

    private:
        unsigned long long runs;
        unsigned long long num_swaps;
        long long milisseconds; 
    
    public:
        QuickSort();
        std::vector<int> sort(std::vector<int> vet) override;
        std::string generateRelatory() override;
        void quick(unsigned int begin, unsigned int end,std::vector<int>& vet);
        unsigned int position_pivot(unsigned int beguin, unsigned int end, std::vector<int>& vet);

};


#endif