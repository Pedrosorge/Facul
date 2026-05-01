#include "QuickSort.h"

#include <vector>
#include <string>
#include "../Timer/Timer.h"

QuickSort::QuickSort() {
    runs = 0;
    num_swaps = 0;
    milisseconds = 0;
    comparations = 0;
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
    
   // Aplicando a técnica da Mediana de Três
    if (end - begin >= 3) {
        unsigned int mid = begin + (end - begin) / 2;
        int a = vet[begin];
        int b = vet[mid];
        int c = vet[end - 1];

        // Determina qual é a mediana e a posiciona no final do vetor (onde o pivô é esperado)
        if ((a >= b && a <= c) || (a <= b && a >= c)) {
            std::swap(vet[begin], vet[end - 1]);
        } else if ((b >= a && b <= c) || (b <= a && b >= c)) {
            std::swap(vet[mid], vet[end - 1]);
        }
        // Se c (vet[end-1]) é a mediana, ele já está na posição correta
    }

    int pivot_value = vet[end - 1];
    unsigned int i = begin;

    for (unsigned int j = begin; j < end - 1; j++) {
        comparations++;
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

    if(runs == 0) return "\tEsse algoritmo não foi rodado nenhuma vez!!\n";

    std::string s;
    s+=  "\t-> Número de Testes: " + std::to_string(runs) + " testes\n";
    s+=  "\t-> Média de Swaps: " + std::to_string(num_swaps/runs) + " swaps\n";
    s+=  "\t-> Média de Comparações: " + std::to_string(comparations/runs) + " comparações\n";
    s+=  "\t-> Média de Tempo: " + std::to_string(milisseconds/runs) + " ms\n"; 
    return s; 

};