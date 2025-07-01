#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "priorityQueue.h"

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

void charToBin(int num, char* bits){
    for(int i = 7; i >= 0; i--){
        bits[i] = num % 2 + '0';
        num = num / 2;
    }
}

void zapisHuffDrevesa(huffDrevo* koren){
    if(koren == NULL){
        return;
    }
    if(koren->levo == NULL && koren->desno == NULL){
        printf("1");
        char* currBits = calloc(9, sizeof(char));
        charToBin(koren->mnozicaZnakov[0], currBits);
        printf("%s", currBits);
        free(currBits);
        // printf("%d", koren->mnozicaZnakov[0]);
    }else{
        printf("0");
    }
    zapisHuffDrevesa(koren->levo);
    zapisHuffDrevesa(koren->desno);
}

// void kodiraniNiz(huffDrevo* koren, char* prvotniNiz){

// }

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

        // tukaj bil bug, to sem pozabil preveriti, ker zgleda v
        // zg funkcijah nekaj ni čisto ok
        if(lepsaMnozicaZnakov(l, d) == 0){
            novoDrevo->levo = l;
            novoDrevo->desno = d;
        }else{
            novoDrevo->levo = d;
            novoDrevo->desno = l;
        }
        
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
    //izpisiDrevoRek(pq->tabela[0], 0);
    zapisHuffDrevesa(pq->tabela[0]);
    // kodiraniNiz(pq->tabela[0]);
    printf("\n");
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