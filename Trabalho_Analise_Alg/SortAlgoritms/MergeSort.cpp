#include "MergeSort.h"
#include "../Timer/Timer.h"

#include <vector>
#include <string>

MergeSort::MergeSort() {
    runs = 0;
    milisseconds = 0;
    comparations = 0;
}

std::vector<int> MergeSort::sort(std::vector<int> vet) {

    Timer t;
    t.startTimer();

    aux = std::vector<int>(vet.size());
    merge(0,vet.size(),vet);
    
    milisseconds += t.endTimer();
    runs++;

    return vet;
}

void MergeSort::merge(unsigned int start,unsigned int end, std::vector<int>& vet) {

    if(start<end-1){
        unsigned int mid = (start+end)/2;
        merge(start,mid, vet);
        merge(mid, end, vet);
        intercalate(start, mid, end, vet);
    }

}

void MergeSort::intercalate(unsigned int s, unsigned int m, unsigned int e, std::vector<int>&vet){
    
    int x=0;
    
    int i=s,j=m;
    while(i<m && j<e){
        comparations++;
        if(vet[i]<vet[j]) aux[x++] = vet[i++]; 
        else aux[x++] = vet[j++];
    }
    while(i<m) aux[x++] = vet[i++];
    while(j<e) aux[x++] = vet[j++];

    for(int t=0;t<x;t++) vet[s+t] = aux[t]; 

}

std::string MergeSort::generateRelatory(){

    if(runs == 0) return "\tEsse algoritmo não foi rodado nenhuma vez!!!";

    std::string s;
    s+= "\t-> Número de Testes: " + std::to_string(runs) + " testes\n";
    s+= "\t-> Média de Tempo: " + std::to_string(milisseconds/runs) + " ms\n";
    s+= "\t-> Média de Comparatções: " + std::to_string(comparations/runs) + " comparações\n";

    return s;
}