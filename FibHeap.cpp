#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cmath>
//#include "CircularDynamicArray.cpp"

using namespace std;

template <typename T>
class FibHeapNode{
    public:
    FibHeapNode<T> *left;
    FibHeapNode<T> *right;
    FibHeapNode<T> *p;
    FibHeapNode<T> *child;
    int degree;
    bool mark;
    bool isLeftChild;
    T key;

    FibHeapNode() {
        left = right = this;
        degree = 0;
        mark = isLeftChild = false;
    }

    FibHeapNode(T k) {
        degree = 0;
        mark = isLeftChild = false;
        key = k;
    }

};

template <typename keyType>
class FibHeap{
    private: 
    FibHeapNode<keyType> *min;
    FibHeapNode<keyType> *head;
    FibHeapNode<keyType> *tail;
    int n;

    void link(FibHeapNode<keyType> *y, FibHeapNode<keyType> *x) {
        FibHeapNode<keyType> *leftChild = new FibHeapNode<keyType>();
        FibHeapNode<keyType> *tailChild = new FibHeapNode<keyType>();
        //cout << "linking " << y->key << " to be child of " << x->key << endl;

        if (y == head) {
            head = y->right;
        }
        else if (y == tail) {
            tail = y->left;
        }
        y->left->right = y->right;
        y->right->left = y->left;
        y->left = y->right = nullptr;

        if (x->child == nullptr) {
            x->child = y;
            y->right = y->left = y;
            y->isLeftChild = true;
            y->p = x;
        }
        else {
            leftChild = x->child;
            tailChild = leftChild->left;
            y->right = leftChild;
            y->left = tailChild;
            leftChild->left = y;
            tailChild->right = y;
            x->child->isLeftChild = false;
            y->isLeftChild = true;
            x->child = y;
            y->p = x;
        }
        x->degree++;
        y->mark = false;
    }

    void consolidate() {
        int deg = ceil(log(n)/log(1.62));
        FibHeapNode<keyType>* arr[deg];
        for (int i = 0; i < deg; i++) {
            arr[i] = nullptr;
        }
        FibHeapNode<keyType> *w = new FibHeapNode<keyType>();
        FibHeapNode<keyType> *u = new FibHeapNode<keyType>();
        FibHeapNode<keyType> *x = new FibHeapNode<keyType>();
        FibHeapNode<keyType> *y = new FibHeapNode<keyType>();
        int d;
        w = head;
        while (true) {
            x = w;
            u = w->right;
            d = x->degree;
            while (arr[d] != nullptr) {
                y = arr[d];
                //cout << "***while, x is " << x->key << " and u is " << u->key << " and y is " << y->key << endl;
                if (y == x) {
                    break;
                }
                if (x->key > y->key) {
                    //cout << "swapping" << endl;
                    FibHeapNode<keyType> *temp = new FibHeapNode<keyType>();
                    temp = x;
                    x = y;
                    y = temp;
                }
                link(y, x);
                if (y == u) {
                    //cout << "y == u" << endl;
                    u = w->right;
                }
                arr[d] = nullptr;
                d++;
            }
            //cout << "-- out of while loop --" << endl;
            arr[d] = x;
            w = u;
            if (w == head || w == w->right) {
                break;
            }
        }
        min = nullptr;
        for (int i = 0; i < deg; i++) {
            if (arr[i] != nullptr) {
                if (min == nullptr) {
                    head = tail = min = arr[i];
                    head->left = head->right = tail;
                    tail->right = tail->left = head;
                    arr[i]->left = arr[i]->right = arr[i];
                }
                else {
                    arr[i]->left = head->left;
                    arr[i]->right = head;
                    head->left->right = arr[i];
                    head->left = arr[i];
                    if (arr[i]->key < min->key) {
                        min = arr[i];
                    }
                }
            }
        }
        tail = head->left;
    }

    void cut(FibHeapNode<keyType> *x, FibHeapNode<keyType> *y) {
        x->left->right = x->right;
        x->right->left = x->left;
        if (x->isLeftChild) {
            if (x != x->right) {
                y->child = x->right;
                x->right->isLeftChild = true;
            }
            else {
                y->child = nullptr;
            }
            x->isLeftChild = false;
        }

        y->degree--;
        x->left = x->right = nullptr;

        //adding x to rootlist
        x->left = tail;
        x->right = head;
        tail->right = x;
        tail = x;
        head->left = x;

        x->p = nullptr;
        x->mark = false;
    }

    void cascadingCut(FibHeapNode<keyType> *y) {
        FibHeapNode<keyType> *z = new FibHeapNode<keyType>();
        z = y->p;
        if (z != nullptr) {
            if (!(y->mark)) {
                y->mark = true;
            }
            else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }

    void printHelper(FibHeapNode<keyType> *x) {
        cout << x->key << " ";
        if (x->child != nullptr) {
                printHelper(x->child);
            //}
        }
        if (!(x->right->isLeftChild)) {
            if (x->right->p != nullptr) {
                printHelper(x->right);
            }
        }
    }

    void destructHelper(FibHeapNode<keyType> *x) {
        if (x->child != nullptr) {
            destructHelper(x->child);
        }
        if (!(x->right->isLeftChild)) {
            destructHelper(x->right);
        }
        delete x;
    }

    void insertNode(FibHeapNode<keyType> *x) {
        if (min == nullptr) {
            head = tail = min = x;
            x->left = x->right = x;
        }
        else {
            x->left = tail;
            x->right = head;
            tail->right = x;
            tail = x;
            head->left = x;
            if (x->key < min->key) {
                min = x;
            }
        }
        n++;
    }

    public:
    FibHeap() {
        min = new FibHeapNode<keyType>();
        head = new FibHeapNode<keyType>();
        tail = new FibHeapNode<keyType>();
        min = head = tail = nullptr;
        n = 0;
    }

    FibHeap(keyType k[], int s, CircularDynamicArray<FibHeapNode<keyType>*> &handle) {          // inserts all elements into rootlist then consolidates, O(n)
        min = new FibHeapNode<keyType>();
        head = new FibHeapNode<keyType>();
        tail = new FibHeapNode<keyType>();
        min = head = tail = nullptr;
        n = s;
        for (int i = 0; i < s; i++) {
            handle.addEnd(insert(k[i]));
        }
        consolidate();
    }

    ~FibHeap() {
        // FibHeapNode<keyType> *curr = new FibHeapNode<keyType>();
        // curr = head;
        // while (curr->right != head || (curr->right == head && curr == head)) {
        //     destructHelper(curr);
        // }
    }

    keyType peekKey() {                                                                         // returns smallest key, no extraction
        return min->key;
    }

    keyType extractMin() {                                                                      //returns smallest key, all consolidation done here, O(lg n) amortized
        //cout << "extractMin" << endl;
        FibHeapNode<keyType> *z = new FibHeapNode<keyType>();
        FibHeapNode<keyType> *curr = new FibHeapNode<keyType>();
        FibHeapNode<keyType> *next = new FibHeapNode<keyType>();
        z = min;
        if (z != nullptr) {
            if (z->degree > 0) {
                curr = z->child;
                while (true) {
                    next = curr->right;
                    curr->left = tail;
                    curr->right = head;
                    head->left->right = curr;
                    head->left = curr;
                    tail = curr;
                    curr->p = nullptr;
                    curr = next;
                    if (curr->isLeftChild) {
                        curr->isLeftChild = false;
                        break;
                    }
                }
            }
            if (z == head) {
                head = z->right;
            }
            if (z == tail) {
                tail = z->right;
            }
            z->left->right = z->right;
            z->right->left = z->left;
            if (z == z->right) {
                min = nullptr;
            }
            else {
                min = z->right;
                consolidate();
            }
            n--;
        }
        //cout << z->key << endl;
        return z->key;
    }

    FibHeapNode<keyType>* insert(keyType k) {                                                   // inserts into root list, O(1) amortized
        FibHeapNode<keyType> *x = new FibHeapNode<keyType>();
        x->degree = 0;
        x->p = x->child = nullptr;
        x->mark = false;
        x->key = k;
        insertNode(x);
        return x;
    }

    bool decreaseKey(FibHeapNode<keyType> *h, keyType k) {                                      // changes key of node h and moves it to root list
        FibHeapNode<keyType> *y = new FibHeapNode<keyType>();
        if (k > h->key) {
            return false;
        }
        h->key = k;
        y = h->p;
        if (y != nullptr && h->key < y->key) {
            cut(h, y);
            cascadingCut(y);
        }
        if (k < min->key) {
            min = h;
        }
        return true;
    }

    void merge(FibHeap<keyType> &H2) {                                                          // merges rootlist of H2 into current FibHeap O(1)
        this->tail->right = H2.head;
        H2.head->left = this->tail;
        this->head->left = H2.tail;
        H2.tail->right = this->head;
        this->tail = H2.tail;
        if ((this->min == nullptr) || (H2.min != nullptr && H2.min->key < this->min->key)) {
            this->min = H2.min;
        }
        n += H2.n;
    }

    void printKey() {                                                                           // Modified preorder recursion to print all keys in heap
        if (n == 0) {
            return;
        }
        FibHeapNode<keyType> *curr = new FibHeapNode<keyType>();
        curr = head;
        while (true) {
            cout << "Rank " << curr->degree << endl;
            cout << curr->key << " ";
            if (curr->degree > 0 && curr->child != nullptr) {
                printHelper(curr->child);
            }
            cout << "\n";
            if (curr == tail) {
                break;
            }
            curr = curr->right;
            if (curr == head) {
                break;
            }
        }
    }
};