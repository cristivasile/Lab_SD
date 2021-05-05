#include <iostream>
#include <fstream>
#include <list>

using namespace std;

ifstream f("mergeheap.in");
ofstream g("mergeheap.out");

struct Nod {
    int val, grad;
    Nod* copil, * frate, * parinte;
};

Nod* newNod(int x) {    //se creeaza un nod nou
    Nod* newNod = new Nod;
    newNod-> grad = 0;
    newNod-> val = x;
    newNod-> copil = NULL;
    newNod-> frate = NULL;
    newNod-> parinte = NULL;
    return newNod;
}

class BinomialHeap {
    list <Nod*> heap;
public:

    void deleteRadacina(Nod* min) {     //se elimina radacina unui tree primit ca parametru si se "distribuie" copii in heap

        if (min->grad == 0) {           //daca tree-ul are grad 0 se sterge direct
            delete min;
            return;
        }

        Nod* del = min;                //se pastreaza adresa initiala pentru stergerea ulterioara

        min = min->copil;               //se merge in left-most child-ul tree-ului
        do {
            min->parinte = NULL;        //pentru fiecare copil, se seteaza parintele ca NULL
            heap.push_front(min);       //fiecare copil este push-at in heap
            min = min->frate;           //se avanseaza la urmatorul copil al radacinii (fratele copilului curent)
        } while (min != NULL);

        delete del;
    }

    Nod* mergeTrees(Nod* t1, Nod* t2) {     //se merge-uiesc 2 arbori binomiali

        if (t1->val < t2->val) {            //daca val radacinii lui tree-ului t1 este mai mare ca val radacinii lui t2 se interschimba
            Nod* aux;
            aux = t1;
            t1 = t2;
            t2 = aux;
        }

        t2->frate = t1->copil;              
        t1->copil = t2;
        t2->parinte = t1;
        t1->grad++;

        return t1;
    }

    void reHeap() {     //se repara proprietatea de binomial heap, adica se unesc toti arborii care au acelasi grad
        list <Nod*> ::iterator pred, curr, next, del;

        if (heap.size() <= 1)
            return;

        pred = heap.begin();
        curr = pred;
        curr++;
        next = curr;
        
        if(next!=heap.end())
        next++;

        while (curr != heap.end()) {

            if ((*pred)->grad < (*curr)->grad) { //daca arborii au grad diferit => nu se pot uni
                pred++;
                curr++;
                if(next!=heap.end())
                next++;
            }
            else if ((*pred)->grad == (*curr)->grad) {     //daca arborii curenti au acelasi grad

                if (next != heap.end() && ((*pred)->grad == (*next)->grad)) {   //daca si arborele urmator are acelasi grad vrem sa avansam, pentru a le uni pe ultimele 2 de gradul curent
                    pred++;
                    curr++;
                    next++;
                }

                else {                                                          //altfel, se unesc arborii curenti
                    *pred = mergeTrees(*pred, *curr);
                    del = curr;
                    curr++;
                    heap.erase(del);                                            //se sterge arborele 2 dupa ce se avanseaza pozitia
                    if (next != heap.end())
                        next++;
                }
            }
        }
    }

    void reHeapForPush() {     //acelasi algoritm ca mai sus, dar care se opreste dupa ce nu mai poate merge-ui primul heap.
        list <Nod*> ::iterator pred, curr, next, del;

        if (heap.size() <= 1)
            return;

        pred = heap.begin();
        curr = pred;
        curr++;
        next = curr;

        if (next != heap.end())
            next++;

        while (curr != heap.end()) {

            if ((*pred)->grad < (*curr)->grad) {    //la insertie suntem siguri ca elementul inserat va ramane mereu in primul arbore. Daca gradele nu corespund, putem iesi
                break;
            }
            else{    

                /* Acest caz nu mai este posibil
                if (next != heap.end() && ((*pred)->grad == (*next)->grad)) { 
                    pred++;
                    curr++;
                    next++;
                }
                */

                *pred = mergeTrees(*pred, *curr);
                del = curr;
                curr++;
                heap.erase(del);
                if (next != heap.end())
                   next++;
            }
        }
    }

    void meld(BinomialHeap& h2) {   //se meld-uieste un binomial heap la binomial heap-ul curent

        list <Nod*> ::iterator heap1 = heap.begin();
        list <Nod*> ::iterator heap2 = h2.heap.begin();
        
        list <Nod*> newHeap;

        //algoritm de interclasare, in functie de gradurile arborilor din fiecare heap
        while (heap1 != heap.end() && heap2 != h2.heap.end()) {
            if ( (*heap1)->grad <= (*heap2) -> grad) {
                newHeap.push_back(*heap1);
                heap1++;
            }
            else {
                newHeap.push_back(*heap2);
                heap2++;
            }
        }

        while (heap1 != heap.end()) {
            newHeap.push_back(*heap1);
            heap1++;
        }

        while (heap2 != h2.heap.end()) {
            newHeap.push_back(*heap2);
            heap2++;
        }

        heap = newHeap;     //se pune heap-ul rezultat in binomial heap-ul #1
        reHeap(); // se ruleaza algoritmul care uneste arborii de acelasi grad pe heap-ul rezultat

        h2.heap.clear(); //se sterge binomial heap-ul #2
    }


    Nod* getRadacina() {        //se returneaza nodul care contine minimul
        Nod* min = newNod(-999999999);

        for (auto it = heap.begin(); it != heap.end(); it++) {
            if ((*it)->val > min->val) {
                min = *it;
            }
        }

        return min;
    }

    int top() {
        return getRadacina()->val;
    }

    void push(int x) {
        Nod* newTree = newNod(x);
        heap.push_front(newTree);
        reHeapForPush();
    }

    void pop() {
        Nod* radacina = getRadacina();

        BinomialHeap newHeap;   //se creeaza un heap nou

        newHeap.deleteRadacina(radacina);  //se pun copii tree-ului care contine minimul in heap-ul nou

        heap.remove(radacina);   //se sterge tree-ul care contine minimul din heap

        meld(newHeap);  //se insereaza copii tree-ului in heap
    }
};

int N, M;
BinomialHeap Heap[101];
int main()
{
    f >> N >> M;

    int opt, heap, x, heap1, heap2;
    for (int i = 1; i <= M; ++i) {
        f >> opt;

        if (opt == 1) {                        //insert 

            f >> heap >> x;
            Heap[heap].push(x);

        }
        if (opt == 2) {                        //delete min + afis

            f >> heap;
            g << Heap[heap].top() << '\n'; 
            Heap[heap].pop();                     
        }
        if (opt == 3) {                       //merge 

            f >> heap1 >> heap2;
            Heap[heap1].meld(Heap[heap2]);
        }
    }
    return 0;
}
