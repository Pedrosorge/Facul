#include <bits/stdc++.h>
#include "./DataVectorGenerator/DataVectorGenerator.h"
#include "./Interfaces/Interfaces.h"
#include "./SortAlgoritms/InsertionSort.h"
#include "./SortAlgoritms/MergeSort.h"
#include "./SortAlgoritms/QuickSort.h"
#include "./SortAlgoritms/RadixSort.h"
#include <stdexcept>

#define NUM_TEST 10

using namespace std;

int main() {
    
    DataVectorGenerator vector_gen;
    vector<int> VECTOR_SIZES = {1000, 10000, 100000, 1000000};

    // VETORES COM VALORES ALEATÓRIOS
    cout << "========================================================================" << endl;
    cout << "===================== VETOR COM VALORES ALEATÒRIOS =====================" << endl;
    cout << "========================================================================" << endl;
    for(auto size: VECTOR_SIZES){
        cout << "=> TAMANHO: "<< size << endl;
        vector<ISortAlgorithm*> rand = 
        {
            new InsertionSort(),
            new MergeSort(),
            new QuickSort(),
            new RadixSort()
        };
        
        vector<int> vet_teste = vector_gen.genVectorRandomlyUniformed(size);
        for(int i=0;i<NUM_TEST;i++){
            for(auto r: rand){
                vector<int> cp_vet_teste(vet_teste.begin(),vet_teste.end());
                try{
                    r->sort(cp_vet_teste);
                } catch (runtime_error& e){
                    cerr << "Erro ao tentar ordenar vetor: " << e.what() << endl;
                    continue;
                }
            }
        }
        
        for(auto r:rand){
            cout << r->generateRelatory() << endl;
            delete r;
        }
    }

    // VETORES COM VALORES JÁ ORDENADOS
    cout << "========================================================================" << endl;
    cout << "=================== VETOR ORDENADOS CRESCENTEMENTE =====================" << endl;
    cout << "========================================================================" << endl; 
    for(auto size: VECTOR_SIZES){
        cout << "=> TAMANHO: "<< size << endl;
        vector<ISortAlgorithm*> rand = 
        {
            new InsertionSort(),
            new MergeSort(),
            new QuickSort(),
            new RadixSort()
        };
        
        vector<int> vet_teste = vector_gen.genVectorAlreadyAscendentSorted(size);
        for(int i=0;i<NUM_TEST;i++){
            for(auto r: rand){
                vector<int> cp_vet_teste(vet_teste.begin(),vet_teste.end());
                try{
                    r->sort(cp_vet_teste);
                } catch (runtime_error& e){
                    continue;
                }
            }
        }
        
        for(auto r:rand){
            cout << r->generateRelatory() << endl;
            delete r;
        }
    }

    // VETORES COM VALORES ORDENADOS DE FORMA DECRESCENTE
    cout << "========================================================================" << endl;
    cout << "================== VETOR ORDENADO DECRESCENTEMENTE =====================" << endl;
    cout << "========================================================================" << endl;
    for(auto size: VECTOR_SIZES){
        cout << "=> TAMANHO: "<< size << endl;
        vector<ISortAlgorithm*> rand = 
        {
            new InsertionSort(),
            new MergeSort(),
            new QuickSort(),
            new RadixSort()
        };
        
        vector<int> vet_teste = vector_gen.genVectorAlreadyDecrescentSorted(size);
        for(int i=0;i<NUM_TEST;i++){
            for(auto r: rand){
                vector<int> cp_vet_teste(vet_teste.begin(),vet_teste.end());
                try{
                    r->sort(cp_vet_teste);
                } catch (runtime_error& e){
                    continue;
                }
            }
        }
        
        for(auto r:rand){
            cout << r->generateRelatory() << endl;
            delete r;
        }
    }

    // VETORES COM VARIOS VALORES REPETIDOS
    cout << "========================================================================" << endl;
    cout << "===================== VETOR COM VALORES REPETIDOS ======================" << endl;
    cout << "========================================================================" << endl;
    for(auto size: VECTOR_SIZES){
        cout << "=> TAMANHO: "<< size << endl;
        vector<ISortAlgorithm*> rand = 
        {
            new InsertionSort(),
            new MergeSort(),
            new QuickSort(),
            new RadixSort()
        };
        
        vector<int> vet_teste = vector_gen.genVectorWithRepetitionOfElements(size);
        for(int i=0;i<NUM_TEST;i++){
            for(auto r: rand){
                vector<int> cp_vet_teste(vet_teste.begin(),vet_teste.end());
                try{
                    r->sort(cp_vet_teste);
                } catch (runtime_error& e){
                    continue;
                }
            }
        }
        
        for(auto r:rand){
            cout << r->generateRelatory() << endl;
            delete r;
        }
    }



    return 0;

}