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

    if(vet.size()>=100000) {
        throw std::runtime_error("Ordenar esse vetor pode levar segundos usando Insertion Sort!!");
    }

    Timer t;
    int swap_count = 0;

    t.startTimer();
    for(int i=1;i<vet.size();i++){
        comparations++;
        for(int j=i;j>0 &&  vet[j-1] > vet[j];j--){
            std::swap(vet[j-1],vet[j]);
            comparations++;
            swap_count++;
        }
    }

    runs++;
    num_swaps += swap_count;
    milisseconds += t.endTimer();

    return vet;

};

std::string InsertionSort::generateRelatory() {

    std::string s =  "----------------- Insertion Sort ------------------\n";
    if(runs == 0) return "Esse algoritmo não foi rodado nenhuma vez!!\n";

    s+=  "Número de Testes: " + std::to_string(runs) + " testes\n";
    s+=  "Média de Swaps: " + std::to_string(num_swaps/runs) + " swaps\n";
    s+=  "Média de Comparações: " + std::to_string(comparations/runs) + " swaps\n";
    s+=  "Média de Tempo: " + std::to_string(milisseconds/runs) + " ms\n"; 
    return s; 
};

