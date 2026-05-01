#include "InsertionSort.h"
#include "../Timer/Timer.h"
#include <stdexcept>

InsertionSort::InsertionSort() { 
    runs = 0;
    num_swaps = 0;
    milisseconds = 0;
    comparations = 0;
}

std::vector<int> InsertionSort::sort(std::vector<int> vet) {

    if(vet.size()>=1000000) {
        throw std::runtime_error("Ordenar esse vetor pode levar segundos usando Insertion Sort!!");
    }

    Timer t;

    t.startTimer();
    for(int i=1;i<vet.size();i++){
        comparations++;
        for(int j=i;j>0 &&  vet[j-1] > vet[j];j--){
            std::swap(vet[j-1],vet[j]);
            comparations++;
            num_swaps++;
        }
    }

    runs++;
    milisseconds += t.endTimer();

    return vet;

};

std::string InsertionSort::generateRelatory() {

    if(runs == 0) return "\tEsse algoritmo não foi rodado nenhuma vez!!\n";
    std::string s;
    s+=  "\t-> Número de Testes: " + std::to_string(runs) + " testes\n";
    s+=  "\t-> Média de Swaps: " + std::to_string(num_swaps/runs) + " swaps\n";
    s+=  "\t-> Média de Comparações: " + std::to_string(comparations/runs) + " comparações\n";
    s+=  "\t-> Média de Tempo: " + std::to_string(milisseconds/runs) + " ms\n"; 
    return s; 
};

