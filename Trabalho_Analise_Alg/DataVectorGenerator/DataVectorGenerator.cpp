#include "DataVectorGenerator.h"
#include <algorithm>
#include <numeric>

DataVectorGenerator::DataVectorGenerator() : generator(rd()) {};

std::vector<int> DataVectorGenerator::genVectorRandomlyUniformed(unsigned int size){

    std::vector<int> vet(size);
    std::uniform_int_distribution<> dis(0,4*size);

    std::generate(vet.begin(),vet.end(), [&](){ return dis(generator); });

    return vet;
};

std::vector<int> DataVectorGenerator::genVectorAlreadyAscendentSorted(unsigned int size){

    std::uniform_int_distribution<> dis(0,15);
    unsigned int curr = dis(generator);
    std::vector<int> vet(size);
    vet[0] = curr;
    dis.param(std::uniform_int_distribution<int>::param_type(0,size));

    std::generate(vet.begin()+1,vet.end(), [&]() {
        curr+=dis(generator);
        return curr;
    });

    return vet;
};

std::vector<int> DataVectorGenerator::genVectorAlreadyDecrescentSorted(unsigned int size){    
    
    std::uniform_int_distribution<> dis(2*size,5*size);
    unsigned int curr = dis(generator);
    std::vector<int> vet(size);
    dis.param(std::uniform_int_distribution<int>::param_type(0,curr/size-1));

    std::generate(vet.begin(),vet.end(), [&]() {
        curr-=dis(generator);
        return curr;
    });

    return vet;

};

std::vector<int> DataVectorGenerator::genVectorWithRepetitionOfElements(unsigned int size){
    
    std::vector<int> vet(size);
    std::uniform_int_distribution<> dis(0,log10(size)+1);
    unsigned int deslocation = 1; 
    unsigned int aux = dis(generator);

    while(aux--){
        deslocation*=10;
    }

    dis.param(std::uniform_int_distribution<int>::param_type(deslocation,deslocation+size/2));

    std::generate(vet.begin(),vet.end(), [&](){ return dis(generator); });

    return vet;

};


