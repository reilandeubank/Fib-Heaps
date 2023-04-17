#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cmath>
//#include "CircularDynamicArray.cpp"

using namespace std;

template <typename keyType>
class Heap{
    private: 
        CircularDynamicArray<keyType> arr;
        keyType dummy;
        int size;

        int parent(int i) {
            return floor(i/2);
        }

        int left(int i) {
            return 2*i;
        }

        int right(int i) {
            return (2*i)+1;
        }

        void buildHeap() {               //builds the whole minheap (bottom up)
            for(int i=floor((size)/2); i >= 1; i--) {
                heapify(i);
            }
        }

        void heapify(int i) {          //builds heap for each subtree
            int l = left(i);
            int r = right(i);
            int smallest;
            if (l <= size && arr[l] < arr[i]) {
                smallest = l;
            }
            else {
                smallest = i;
            }
            if (r <= size && arr[r] < arr[smallest]) {
                smallest = r;
            }
            if (smallest != i) {
                arr.swapVals(&arr[i], &arr[smallest]);
                heapify(smallest);
            }
        }

    public:
        Heap() {
            size = 0;
            arr.addFront(dummy);
        }

        Heap(keyType k[], int s) {                                 //constructor sets arr = k[], then bottom up head builds O(n)
            size = 0;
            arr.addFront(dummy);
            for (int i = 0; i < s; i++) {
                arr.addEnd(k[i]);
            }
            size = s;
            buildHeap();
        }

        Heap (Heap &a) {
            dummy = a.dummy;
            size = a.size;
            for (int i = 0; i < size; i++) {
                arr.addEnd(a.arr[i]);
            }
        }

        ~Heap() {
            for (int i = 0; i < size; i++) {
                arr.delEnd();
            }
        }

        Heap& operator=(Heap& a) {
            dummy = a.dummy;
            size = a.size;
            for (int i = 0; i < size; i++) {
                arr.addEnd(a.arr[i]);
            }
        }

        void insert(keyType k) {                                    //Inserts at the end, then bubbles up to the top, O(lg n)
            //cout << "Inserting " << k << endl;
            arr.addEnd(k);                                          //insert element k at the end
            size++;
            int i = size;

            while (i > 1 && arr[parent(i)] > arr[i]) {             //fixing minheap property
                arr.swapVals(&arr[parent(i)], &arr[i]);
                i = parent(i);
            }
        }

        keyType extractMin() {                                      //Return min val in heap and restructures, O(lg n)
            keyType min = arr[1];
            if (size <= 1) {
                return arr[0];
            }
            if (size == 1) {
                size--;
                arr.delEnd();
                return min;
            }
            arr[1] = arr[size-1];
            arr.delEnd();
            size--;
            heapify(1);
            return min;
        }

        keyType peekKey() {                                         //Return minimum val in heap without extracting, O(1)
            return arr[1];
        }

        void printKey() {                                           //Prints all values in the heap in array order
            for(int i = 1; i <= size; i++) {
                cout << arr[i] << " ";
            }
            cout << endl;
        }
};