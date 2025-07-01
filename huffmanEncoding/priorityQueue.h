#ifndef priorityQueue_H
#define priorityQueue_H

typedef struct huffDrevo huffDrevo;
struct huffDrevo{
    char* mnozicaZnakov;
    int teza;
    huffDrevo* levo;
    huffDrevo* desno;
};

typedef struct PriorityQueue PriorityQueue;
struct PriorityQueue{
    huffDrevo** tabela;
    int size;
};

void swap(huffDrevo* a, huffDrevo* b){
    huffDrevo tmp = *a;
    *a = *b;
    *b = tmp;
}

// vrne 0, če je prva lepša in 1 če je druga lepša
int lepsaMnozicaZnakov(huffDrevo* prvi, huffDrevo* drugi){
    if(prvi->mnozicaZnakov[0] < drugi->mnozicaZnakov[0]){
        return 0;
    }else{
        return 1;
    }
}

void heapifyUp(PriorityQueue* pq, int idx){
    if(idx == 0){
        return; // koren : stop
    }else{
        int parentIdx = (idx - 1) / 2;
        if(pq->tabela[idx]->teza < pq->tabela[parentIdx]->teza){
            swap(pq->tabela[idx], pq->tabela[parentIdx]);
            heapifyUp(pq, parentIdx);
        }else if((pq->tabela[idx]->teza == pq->tabela[parentIdx]->teza) && (lepsaMnozicaZnakov(pq->tabela[idx], pq->tabela[parentIdx]) == 0)){
            swap(pq->tabela[idx], pq->tabela[parentIdx]);
            heapifyUp(pq, parentIdx);
        }
    }
}

void heapifyDown(PriorityQueue* pq, int idx){
    int najmanjsi = idx;
    int levi = 2 * idx + 1;
    int desni = 2 * idx + 2;

    if(levi < pq->size){
        if(pq->tabela[levi]->teza < pq->tabela[najmanjsi]->teza){
            najmanjsi = levi;
        }else if(pq->tabela[levi]->teza == pq->tabela[najmanjsi]->teza && lepsaMnozicaZnakov(pq->tabela[levi], pq->tabela[najmanjsi]) == 0){
            najmanjsi = levi;
        }
    }

    if(desni < pq->size){
        if(pq->tabela[desni]->teza < pq->tabela[najmanjsi]->teza){
            najmanjsi = desni;
        }else if(pq->tabela[desni]->teza == pq->tabela[najmanjsi]->teza && lepsaMnozicaZnakov(pq->tabela[desni], pq->tabela[najmanjsi]) == 0){
            najmanjsi = desni;
        }
    }

    if(najmanjsi != idx){
        swap(pq->tabela[idx], pq->tabela[najmanjsi]);
        heapifyDown(pq, najmanjsi);
    }
}


// doda element curr glede na prioriteto v vrsti(Queue)
void addToQueue(PriorityQueue* pq, huffDrevo* curr){
    pq->tabela[pq->size++] = curr;
    heapifyUp(pq, pq->size - 1);
}

// odstrani prvi element iz vrste in ohrani heap-priority vrste
huffDrevo* removeFromQueue(PriorityQueue* pq){
    huffDrevo* min = pq->tabela[0];
    pq->tabela[0] = pq->tabela[--pq->size];
    heapifyDown(pq, 0);
    return min;
}


#endif





/*

// Prepočasi + ne uporabljamo prioritiy queue

void sortTabDreves(huffDrevo** tabDreves, int idx){
    // bubble sort
    for(int i = 0; i < idx; i++){
        for(int j = 0; j < idx - 1 - i; j++){
             if(tabDreves[j]->teza > tabDreves[j + 1]->teza){
                huffDrevo* tmp = tabDreves[j];
                tabDreves[j] = tabDreves[j + 1];
                tabDreves[j + 1] = tmp;
            }else if(tabDreves[j]->teza == tabDreves[j + 1]->teza){
                if(lepsaMnozicaZnakov(tabDreves[j], tabDreves[j + 1]) == 1){
                    huffDrevo* tmp = tabDreves[j];
                    tabDreves[j] = tabDreves[j + 1];
                    tabDreves[j + 1] = tmp;
                }
            }
        }
    }
}
*/