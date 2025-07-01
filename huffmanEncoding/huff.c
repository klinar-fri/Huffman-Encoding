#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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

void izpisiDrevoRek(huffDrevo* node, int nivo){
    if(node == NULL){
        return;
    }
    for(int i = 0; i < nivo - 1; i++){
        printf("|  ");
    }
    if(nivo != 0){
        printf("+-- %s/%d\n", node->mnozicaZnakov, node->teza);
    }else{
        printf("%s/%d\n", node->mnozicaZnakov, node->teza);
    }


    izpisiDrevoRek(node->levo, nivo + 1);
    izpisiDrevoRek(node->desno, nivo + 1);
}


void izpisiHuffNode(huffDrevo* curr){
    printf("%s/%d\n", curr->mnozicaZnakov, curr->teza);
}

// vrne 0, če je prva lepša in 1 če je druga lepša
int lepsaMnozicaZnakov(huffDrevo* prvi, huffDrevo* drugi){
    if(prvi->mnozicaZnakov[0] < drugi->mnozicaZnakov[0]){
        return 0;
    }else{
        return 1;
    }
}


void izpisiDrevo(huffDrevo** tabDreves, int idx){
    bool prvi = true;
    for(int i = 0; i < idx; i++){
        if(prvi){
            printf("%s/%d", tabDreves[i]->mnozicaZnakov, tabDreves[i]->teza);
            prvi = false;
        }else{
            printf(" %s/%d", tabDreves[i]->mnozicaZnakov, tabDreves[i]->teza);
        }
    }
    printf("\n");
}

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

void swap(huffDrevo* a, huffDrevo* b){
    huffDrevo tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapifyUp(PriorityQueue* pq, int idx){
    if(idx == 0){
        return; // koren : stop
    }else{
        int parentIdx = (idx - 1) / 2;
        if(pq->tabela[idx]->teza < pq->tabela[parentIdx]->teza){
            swap(pq->tabela[idx], pq->tabela[parentIdx]);
            heapifyUp(pq, parentIdx);
        }else if(pq->tabela[idx]->teza == pq->tabela[parentIdx]->teza && lepsaMnozicaZnakov(pq->tabela[idx], pq->tabela[parentIdx]) == 0){
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
        if(pq->tabela[levi]->teza < pq->tabela[najmanjsi]->teza ||
           (pq->tabela[levi]->teza == pq->tabela[najmanjsi]->teza &&
            lepsaMnozicaZnakov(pq->tabela[levi], pq->tabela[najmanjsi]) == 0)){
            najmanjsi = levi;
        }
    }

    if(desni < pq->size){
        if(pq->tabela[desni]->teza < pq->tabela[najmanjsi]->teza ||
           (pq->tabela[desni]->teza == pq->tabela[najmanjsi]->teza &&
            lepsaMnozicaZnakov(pq->tabela[desni], pq->tabela[najmanjsi]) == 0)){
            najmanjsi = desni;
        }
    }

    if(najmanjsi != idx){
        swap(pq->tabela[idx], pq->tabela[najmanjsi]);
        heapifyDown(pq, najmanjsi);
    }
}


// adds element curr to the queue according to the priority in heapifyUp
void addToQueue(PriorityQueue* pq, huffDrevo* curr){
    pq->tabela[pq->size++] = curr;
    heapifyUp(pq, pq->size - 1);
}

// removes the first element from the Queue and restores the heap priority of the queue
huffDrevo* removeFromQueue(PriorityQueue* pq){
    huffDrevo* min = pq->tabela[0];
    pq->tabela[0] = pq->tabela[--pq->size];
    heapifyDown(pq, 0);
    return min;
}

void kodiraj(char* imeVhodne, char* imeIzhodne){
    FILE* input = fopen(imeVhodne, "r");
    int hash[128] = {0};
    while(1){
        char c = fgetc(input);
        if(c == EOF){
            break;
        }else{
            hash[c - 'A']++;
        }
    }

    PriorityQueue* pq = malloc(sizeof(PriorityQueue));
    pq->tabela = calloc(128, sizeof(huffDrevo*));
    pq->size = 0;

    int idx = 0;
    for(int i = 0; i < 128; i++){
        if(hash[i] != 0){
            // printf("%c/%d ", i + 'A', hash[i]);
            huffDrevo* novoDrevo = malloc(sizeof(huffDrevo));
            char* currMnozica = calloc(128, sizeof(char));
            currMnozica[0] = i + 'A';
            currMnozica[1] = '\0';
            novoDrevo->mnozicaZnakov = currMnozica;
            novoDrevo->teza = hash[i];
            novoDrevo->levo = NULL;
            novoDrevo->desno = NULL;
            addToQueue(pq, novoDrevo);
            idx++;
        }
    }
    
    int i = 0;
    while (pq->size > 1){
        huffDrevo* l = removeFromQueue(pq);
        huffDrevo* d = removeFromQueue(pq);
        huffDrevo* novoDrevo = malloc(sizeof(huffDrevo));
        novoDrevo->levo = l;
        novoDrevo->desno = d;
        novoDrevo->teza = l->teza + d->teza;
        char* novaMnozica = calloc(128, sizeof(char));
        novaMnozica[0] = '\0';
        strcat(novaMnozica, l->mnozicaZnakov);
        strcat(novaMnozica, d->mnozicaZnakov);

        // Sort the combined character set
        int len = strlen(novaMnozica);
        for (int i = 0; i < len - 1; i++) {
            for (int j = 0; j < len - 1 - i; j++) {
                if (novaMnozica[j] > novaMnozica[j + 1]) {
                    char tmp = novaMnozica[j];
                    novaMnozica[j] = novaMnozica[j + 1];
                    novaMnozica[j + 1] = tmp;
                }
            }
        }



        novoDrevo->mnozicaZnakov = novaMnozica;
        addToQueue(pq, novoDrevo);
    }

    // izpisiDrevo(pq->tabela, pq->size);
    izpisiDrevoRek(pq->tabela[0], 0);
    fclose(input);
}


int main(){
    int ukaz;
    char* imeVhodne = calloc(51, sizeof(char));
    char* imeIzhodne = calloc(51, sizeof(char));
    scanf("%d %s %s", &ukaz, imeVhodne, imeIzhodne);
    if(ukaz == 1){
        kodiraj(imeVhodne, imeIzhodne);
    }else{
        // dekodiraj();
    }
    free(imeVhodne);
    free(imeIzhodne);
    return 0;
}