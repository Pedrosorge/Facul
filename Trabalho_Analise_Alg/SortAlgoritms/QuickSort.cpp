#include "QuickSort.h"

#include <vector>
#include <string>
#include "../Timer/Timer.h"

QuickSort::QuickSort() {
    runs = 0;
    num_swaps = 0;
    milisseconds = 0;
};

std::vector<int> QuickSort::sort(std::vector<int> vet){

    Timer t; 

    t.startTimer();
    
    quick(0,vet.size(),vet);

    milisseconds += t.endTimer();
    runs++;

    return vet;

};

unsigned int QuickSort::position_pivot(unsigned int begin, unsigned int end, std::vector<int>& vet){
    int pivot_value = vet[end - 1];
    unsigned int i = begin;

    for (unsigned int j = begin; j < end - 1; j++) {
        if (vet[j] <= pivot_value) {
            std::swap(vet[i], vet[j]);
            num_swaps++;
            i++;
        }
    }
    std::swap(vet[i], vet[end - 1]);
    num_swaps++;
    return i;

};

void QuickSort::quick(unsigned int begin, unsigned int end, std::vector<int>& vet){
    
    if (begin < end && (end - begin) > 1) {
        unsigned int pivo = position_pivot(begin,end,vet);
        quick(begin,pivo,vet);
        quick(pivo+1,end,vet);
    }

};

std::string QuickSort::generateRelatory(){

    if(runs == 0) return "Esse algoritmo não foi rodado nenhuma vez!!\n";
    std::string s = "----------------- Quick Sort ------------------ \n";
    s+=  "Número de Testes: " + std::to_string(runs) + " testes\n";
    s+=  "Média de Swaps: " + std::to_string(num_swaps/runs) + " swaps\n";
    s+=  "Média de Tempo: " + std::to_string(milisseconds/runs) + " ms\n"; 
    return s; 

};