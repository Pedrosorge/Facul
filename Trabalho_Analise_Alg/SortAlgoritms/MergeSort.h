#ifndef MERGE_SORT
#define MERGE_SORT

#include <vector>
#include <string>
#include "../Interfaces/Interfaces.h"

class MergeSort : public ISortAlgorithm {

    private:
        unsigned long long runs;
        long long milisseconds; 
        std::vector<int> aux;

    public: 
        MergeSort();
        std::vector<int> sort(std::vector<int> vet) override;
        std::string generateRelatory() override;
        void merge(unsigned int start, unsigned int end, std::vector<int>&vet);
        void intercalate(unsigned int s, unsigned int m, unsigned int e, std::vector<int>&vet);
};

#endif