#include "RadixSort.h"
#include <vector>
#include <string>
#include "../Timer/Timer.h"

RadixSort::RadixSort(){
    runs = 0;
    milisseconds = 0;
};

std::vector<int> RadixSort::sort(std::vector<int> vet) {

    Timer t;
    t.startTimer();

    radix(vet);

    milisseconds+=t.endTimer();
    runs++;
    return vet;

};

void RadixSort::radix(std::vector<int> &vet){
 
    int m = vet[0];
    for(int i=1;i<vet.size();i++){
        if(m<vet[i]) m = vet[i];
    }

    for(int exp=1; m/exp > 0; exp *= 10){
        countingSort(vet,exp);
    }

};

void RadixSort::countingSort(std::vector<int> &vet, unsigned long long exp) {

    std::vector<long long> dig_freq(10,0);

    for(int i=0;i<vet.size();i++){
        dig_freq[vet[i]/exp % 10]++;
    }

    for(int i=1;i<10;i++){
        dig_freq[i] += dig_freq[i-1];
    }

    std::vector<int> aux(vet.size());

    for(int i=vet.size()-1;i>=0;i--){
        aux[--dig_freq[vet[i]/exp % 10]] = vet[i];
    }

    for(int i=0;i<vet.size();i++){
        vet[i] = aux[i];
    }

};

std::string RadixSort::generateRelatory(){

    std::string s = "---------------- Radix Sort -------------------\n";
    s+= "Número de Testes: " + std::to_string(runs) + " testes\n";
    s+= "Média de Tempo " + std::to_string(milisseconds/runs) + " ms\n";

    return s;
}
