#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cmath>

using namespace std;

template <typename T>
class CircularDynamicArray{
    private:
        int cap;
        int size;
        int front;
        T *arr;
        T *newArr;
        T *arr2;

    int wrapper(int n) {                //Math to convert user index to real index
        if (front + n >= cap) {
            //cout << "n + front is " << n + front << " turning into " << (front + n) % cap << endl;
        }
        if (n < 0) {
            return wrapper(n + cap);
        }
        else {
            return (front + n) % cap;
        }
    }

    void swap(T *a, T *b)
    {
        T temp = *a;
        *a = *b;
        *b = temp;
    }

    void swap(T a, T b) {
        T temp = a;
        a = b;
        b = temp;
    }

    int binarySearch(T arr[], int l, int r, int x) {
        if (r >= l) {
        int mid = l + (r - l) / 2;
        if (arr[mid] == x){
            return mid;
        }
        if (arr[mid] > x){
            return binarySearch(arr, l, mid - 1, x);
        }
        return binarySearch(arr, mid + 1, r, x);
        }
        return -1;
    }
 
    public:
    CircularDynamicArray() {            //Default constructor
        arr = new T[cap=2];
        size = 0;
        front = 0;
    }

    CircularDynamicArray(int s) {       //Overloaded constructor with int
        arr = new T[cap=s];
        size = 0;
        front = 0;
    }

    ~CircularDynamicArray(){            //Destructor
        delete arr;
    }

    void swapVals(T *a, T *b) {         //Swap two values within the CDA
        T temp = *a;
        *a = *b;
        *b = temp;
    }

    CircularDynamicArray(CircularDynamicArray &a) {              //Copy Constructor
        cap = a.cap;
        size = a.size;

        for(int i = 0; i < size; i++) {
            arr[i] = a[i];
        }
    }

    CircularDynamicArray& operator=(CircularDynamicArray& a) {
        cap = a.cap;
        size = a.size;
        T *arr = new T[cap];
        for(int i = 0; i < size; i++) {
            arr[i] = a[i];
        }
        return *this;
    }

    T &operator[](int x) {
        if(x > size) {
            cout << "attempting to access " << x << endl;
            cout << "Index out of bounds" <<endl; 
            // return first element
            return arr[wrapper(0)];
        }
        
        return arr[wrapper(x)];
    }

    void addEnd(T v) {                  //Add v to end of arr in O(1) amortized 
        //cout << "addEnd" << endl;
        if (size == 0) {                //if array is empty
            //cout << "if" << endl;
            arr[0] = v;
            size++;
        }
        else if (size < cap) {          //has space for add
            //cout << "else if 1" << endl;
            arr[wrapper(size)] = v;
            size++;
        }
        else {                          //if size = capacity
            arr2 = new T[cap*=2];
            //cout << "else if 2" << endl;
            for (int i = 0; i < cap/2; i++) {
                arr2[i] = arr[i];
            }
            arr = arr2;
            addEnd(v);
        }
    }

    void addFront(T v) {                //Add v to front of arr (circular) in O(1) amortized
        if (size == 0) {                //if array is empty
            arr[0] = v;
        }
        else if (size < cap) {          //has space for add
            arr[wrapper(-1)] = v;
            front--;
            if (front < 0) {
                front += cap;
            }
        }
        else {                          //if size = capacity
            arr2 = new T[cap*=2];
            for (int i = 0; i < cap/2; i++) {
                arr2[i] = arr[i];
            }
            arr = arr2;
            addFront(v);
        }
        size++;
    }

    void delEnd() {                     //Deletes end of arr in O(1) amortized
        if (size-1 >= (cap*.25)) {      //if resize not needed
            arr[wrapper(size)] = -1;
            size--;
        }
        else {                          //if less than 1/4 array is full
            cap = cap/2;
            arr2 = new T[cap];
            for (int i = 0; i < cap; i++) {
                arr2[i] = arr[i];
            }
            arr = arr2;
            size--;
        }
    }

    void delFront() {                   //Deletes front of arr in O(1) amortized
        if (size-1 >= (cap*.25)) {      //if resize not needed
            arr[wrapper(0)] = -1;
            size--;
            front++;
        }
        else {                          //if less than 1/4 array is full
            cap = cap/2;
            arr2 = new T[cap];
            for (int i = 0; i < cap; i++) {
                arr2[i] = arr[i];
            }
            arr = arr2;
            size--;
            front++;
        }
    }

    int length() {               //return size
        return size;
    }

    int capacity() {             //return capacity
        return cap;
    }

    void clear() {                      //clears arr, starts over size 0 capacity 2
        delete arr;
        arr = new T[cap=2];
        size = 0;
    }

    int linearSearch (T e) {
        for (int i = 0; i < size-1; i++) {
            if (arr[wrapper(i)] == e) {
                return i;
            }
        }
        return -1;
    }

    int binSearch(T e) {
        int val = binarySearch(arr, 0, size - 1, e) - front;
        while (val < 0) {
            val += cap;
        }
        return val;
    }
};