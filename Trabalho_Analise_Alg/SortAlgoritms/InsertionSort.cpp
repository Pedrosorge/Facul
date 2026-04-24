#include "InsertionSort.h"
#include "../Timer/Timer.h"

InsertionSort::InsertionSort() { 
    runs = 0;
    num_swaps = 0;
    milisseconds = 0;
}

std::vector<int> InsertionSort::sort(std::vector<int> vet) {

    Timer *t = new Timer;
    int swap_count = 0;

    t->startTimer();
    for(int i=1;i<vet.size();i++){
        for(int j=i;j>0 &&  vet[j-1] > vet[j];j--){
            std::swap(vet[j-1],vet[j]);
            swap_count++;
        }
    }

    runs++;
    num_swaps += swap_count;
    milisseconds += t->endTimer();
    delete t;

    return vet;

};

std::string InsertionSort::generateRelatory() {

    if(runs == 0) return "Esse algoritmo não foi rodado nenhuma vez!!\n";
    std::string s = "----------------- Insertion Sort ------------------ \n";
    s+=  "Número de Testes: " + std::to_string(runs) + " testes\n";
    s+=  "Média de Swaps: " + std::to_string(num_swaps/runs) + " swaps\n";
    s+=  "Média de Tempo: " + std::to_string(milisseconds/runs) + " ms\n"; 
    return s; 
};

