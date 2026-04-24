#include <bits/stdc++.h>
#include "./DataVectorGenerator/DataVectorGenerator.h"
#include "./Interfaces/Interfaces.h"
#include "./SortAlgoritms/InsertionSort.h"
#include "./SortAlgoritms/MergeSort.h"

using namespace std;

int main() {

    DataVectorGenerator vector_gen;
    InsertionSort insertion_sorter_rand;
    MergeSort merge_sorter_rand;

    cout << "Vetor random: ";
    for(int i=0;i<100;i++){
        insertion_sorter_rand.sort(vector_gen.genVectorRandomlyUniformed(1000));
    }
    cout << insertion_sorter_rand.generateRelatory();

    for(int i=0;i<100;i++){
        merge_sorter_rand.sort(vector_gen.genVectorRandomlyUniformed(1000));
    }
    cout << merge_sorter_rand.generateRelatory();

    return 0;

}