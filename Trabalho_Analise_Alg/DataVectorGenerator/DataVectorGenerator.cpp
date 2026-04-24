#include "DataVectorGenerator.h"
#include <algorithm>
#include <numeric>


DataVectorGenerator::DataVectorGenerator() : generator(rd()) {};

std::vector<int> DataVectorGenerator::genVectorRandomlyUniformed(unsigned int size){

    std::vector<int> vet(size);
    std::uniform_int_distribution<> dis(-2*size,2*size);

    std::generate(vet.begin(),vet.end(), [&](){ return dis(generator); });

    return vet;
};

std::vector<int> DataVectorGenerator::genVectorAlreadyAscendentSorted(unsigned int size){

    unsigned int curr = 1;
    std::vector<int> vet(size);

    std::iota(vet.begin(),vet.end(), curr);

    return vet;
};

std::vector<int> DataVectorGenerator::genVectorAlreadyDecrescentSorted(unsigned int size){
    
    std::vector<int> vet(size);
    unsigned int curr = size;
    
    std::generate(vet.begin(),vet.end(), [&]() { return size--; });
    return vet;

};

