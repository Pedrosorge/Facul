#ifndef DATA_VECTOR_GENERATOR
#define DATA_VECTOR_GENERATOR

#include <vector>
#include <random>

class DataVectorGenerator{

    private:
        std::random_device rd;
        std::mt19937 generator;

    public:
        DataVectorGenerator(); 
        std::vector<int> genVectorRandomlyUniformed(unsigned int size);
        std::vector<int> genVectorAlreadyAscendentSorted(unsigned int size);
        std::vector<int> genVectorAlreadyDecrescentSorted(unsigned int size);
            
};

#endif