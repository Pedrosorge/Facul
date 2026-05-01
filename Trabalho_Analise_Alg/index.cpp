#include <bits/stdc++.h>
#include "./DataVectorGenerator/DataVectorGenerator.h"
#include "./Interfaces/Interfaces.h"
#include "./SortAlgoritms/InsertionSort.h"
#include "./SortAlgoritms/MergeSort.h"
#include "./SortAlgoritms/QuickSort.h"
#include "./SortAlgoritms/RadixSort.h"
#include <stdexcept>

#define NUM_TEST 1

using namespace std;

int main() {
    
    DataVectorGenerator vector_gen;
    vector<int> VECTOR_SIZES = {1000, 10000, 100000, 1000000};

    vector<vector<ISortAlgorithm*>> rand = { 
        {new InsertionSort(), new InsertionSort(),new InsertionSort(),new InsertionSort()},
        {new MergeSort(), new MergeSort(),new MergeSort(),new MergeSort()},
        {new QuickSort(), new QuickSort(),new QuickSort(),new QuickSort()},
        {new RadixSort(), new RadixSort(),new RadixSort(),new RadixSort()},
    };
    vector<vector<ISortAlgorithm*>> asc_sorted = { 
        {new InsertionSort(), new InsertionSort(),new InsertionSort(),new InsertionSort()},
        {new MergeSort(), new MergeSort(),new MergeSort(),new MergeSort()},
        {new QuickSort(), new QuickSort(),new QuickSort(),new QuickSort()},
        {new RadixSort(), new RadixSort(),new RadixSort(),new RadixSort()},
    };
    vector<vector<ISortAlgorithm*>> desc_sorted = { 
        {new InsertionSort(), new InsertionSort(),new InsertionSort(),new InsertionSort()},
        {new MergeSort(), new MergeSort(),new MergeSort(),new MergeSort()},
        {new QuickSort(), new QuickSort(),new QuickSort(),new QuickSort()},
        {new RadixSort(), new RadixSort(),new RadixSort(),new RadixSort()},
    };
    vector<vector<ISortAlgorithm*>> rep = { 
        {new InsertionSort(), new InsertionSort(),new InsertionSort(),new InsertionSort()},
        {new MergeSort(), new MergeSort(),new MergeSort(),new MergeSort()},
        {new QuickSort(), new QuickSort(),new QuickSort(),new QuickSort()},
        {new RadixSort(), new RadixSort(),new RadixSort(),new RadixSort()},
    };  

    cout << "Iniciando os testes...\r";

    // VETORES COM VALORES ALEATÓRIOS
    for(int s=0;s<VECTOR_SIZES.size();s++){
        vector<int> vet_teste = vector_gen.genVectorRandomlyUniformed(VECTOR_SIZES[s]);
        for(int i=0;i<NUM_TEST;i++){
            for(auto r: rand){
                vector<int> cp_vet_teste(vet_teste.begin(),vet_teste.end());
                try{
                    r[s]->sort(cp_vet_teste);
                } catch (runtime_error& e){
                    cerr << "Erro ao tentar ordenar vetor: " << e.what() << endl;
                    continue;
                }
            }
        }
    }

    // VETORES COM VALORES CRESCENTES
    for(int s=0;s<VECTOR_SIZES.size();s++){
        vector<int> vet_teste = vector_gen.genVectorAlreadyAscendentSorted(VECTOR_SIZES[s]);
        for(int i=0;i<NUM_TEST;i++){
            for(auto r: asc_sorted){
                vector<int> cp_vet_teste(vet_teste.begin(),vet_teste.end());
                try{
                    r[s]->sort(cp_vet_teste);
                } catch (runtime_error& e){
                    cerr << "Erro ao tentar ordenar vetor: " << e.what() << endl;
                    continue;
                }
            }
        }
    }

    // VETORES COM VALORES DECRESCENTES
    for(int s=0;s<VECTOR_SIZES.size();s++){
        vector<int> vet_teste = vector_gen.genVectorAlreadyDecrescentSorted(VECTOR_SIZES[s]);
        for(int i=0;i<NUM_TEST;i++){
            for(auto r: desc_sorted){
                vector<int> cp_vet_teste(vet_teste.begin(),vet_teste.end());
                try{
                    r[s]->sort(cp_vet_teste);
                } catch (runtime_error& e){
                    cerr << "Erro ao tentar ordenar vetor: " << e.what() << endl;
                    continue;
                }
            }
        }
    }

    // VETORES COM VALORES REPETIDOS
    for(int s=0;s<VECTOR_SIZES.size();s++){
        vector<int> vet_teste = vector_gen.genVectorWithRepetitionOfElements(VECTOR_SIZES[s]);
        for(int i=0;i<NUM_TEST;i++){
            for(auto r: rep){
                vector<int> cp_vet_teste(vet_teste.begin(),vet_teste.end());
                try{
                    r[s]->sort(cp_vet_teste);
                } catch (runtime_error& e){
                    cerr << "Erro ao tentar ordenar vetor: " << e.what() << endl;
                    continue;
                }
            }
        }
    }


    // GERAÇÃO DO RELATÓRIO DOS ALGORITMOS
    cout << "\n\nRelatório dos Algoritmos\n\n";
    vector<string> nomes_algoritmos = {"Insertion Sort", "Merge Sort", "Quick Sort", "Radix Sort"};

    for (int i = 0; i < 4; i++) { 
        cout << "========================================================" << endl;
        cout << "               " << nomes_algoritmos[i] << "                 " << endl;
        cout << "========================================================" << endl;

        for (int s = 0; s < VECTOR_SIZES.size(); s++) {
            cout << "=> TAMANHO: " << VECTOR_SIZES[s] << endl;

            cout << " - Vetor com Valores Aleatórios:" << endl;
            cout << rand[i][s]->generateRelatory();

            cout << " - Vetor Ordenado Crescentemente:" << endl;
            cout << asc_sorted[i][s]->generateRelatory();

            cout << " - Vetor Ordenado Decrescentemente:" << endl;
            cout << desc_sorted[i][s]->generateRelatory();

            cout << " - Vetor com Valores Repetidos:" << endl;
            cout << rep[i][s]->generateRelatory();

            cout << "--------------------------------------------------------" << endl;
        }
        cout << endl;
    }


    return 0;

}