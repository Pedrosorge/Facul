#ifndef RADIX_SORT
#define RADIX_SORT

#include "../Interfaces/Interfaces.h"

#include <vector>
#include <string>

class RadixSort : public ISortAlgorithm {

    private:
        unsigned long long runs;
        long long milisseconds;

    public:
        RadixSort();
        std::vector<int> sort(std::vector<int> vet);
        std::string generateRelatory();
        void radix(std::vector<int> &vet);
        void countingSort(std::vector<int> &vet, unsigned long long exp);

};

#endif