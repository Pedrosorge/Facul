#include "MergeSort.h"
#include "../Timer/Timer.h"

#include <vector>
#include <string>

MergeSort::MergeSort() {
    runs = 0;
    milisseconds = 0;
}

std::vector<int> MergeSort::sort(std::vector<int> vet) {

    Timer *t = new Timer;
    t->startTimer();

    aux = std::vector<int>(vet.size());
    merge(0,vet.size(),vet);
    
    milisseconds += t->endTimer();
    delete t;
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
        if(vet[i]<vet[j]) aux[x++] = vet[i++]; 
        else aux[x++] = vet[j++];
    }
    while(i<m) aux[x++] = vet[i++];
    while(j<e) aux[x++] = vet[j++];

    for(int t=0;t<x;t++) vet[s+t] = aux[t]; 

}

std::string MergeSort::generateRelatory(){

    std::string s = "---------------- Merge Sort -------------------\n";
    s+= "Número de Testes: " + std::to_string(runs) + " testes\n";
    s+= "Média de Tempo " + std::to_string(milisseconds/runs) + " ms\n";

    return s;
}