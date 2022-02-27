#include <stdlib.h>

void sift_down(int* vector, size_t cantidad, size_t posicion);
void sift_up(int* vector, size_t cantidad, size_t posicion);

//convicerte en heap un vector comun y corriene
void heapify(int* vector, size_t cantidad){
    size_t posicion = (cantidad-1)/2;
    while ((posicion>0)){
        sift_down(vector, cantidad, posicion);
        posicion--;
    }    
}

//ordena de menor a mayor el heap 
void heap_sort(int* vector, size_t cantidad){
    heapify(vector, cantidad);
    while(cantidad >0){
        swap(vector, 0, cantidad-1);
        cantidad--;
        sift_down(vector, cantidad, 0); 
    }
}