#ifndef INSERTION_SORT
#define INSERTION_SORT

#include <vector>
#include <string>
#include "../Interfaces/Interfaces.h"

class InsertionSort : public ISortAlgorithm{

    private:
        unsigned long long runs;
        unsigned long long num_swaps;
        long long milisseconds; 

    public:
        InsertionSort();
        std::vector<int> sort(std::vector<int> vet) override;
        std::string generateRelatory() override;
        
};

#endif