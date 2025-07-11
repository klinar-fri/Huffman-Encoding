#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "priorityQueue.h"

#ifndef printTree_H
#define printTree_H

void izpisiDrevoRek(huffDrevo* node, int nivo){
    if(node == NULL){
        return;
    }
    for(int i = 0; i < nivo - 1; i++){
        printf("|   ");
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
        char currBits[9];
        charToBin(koren->mnozicaZnakov[0], &currBits[0]);
        printf("%s", currBits);
        // printf("%d", koren->mnozicaZnakov[0]);
    }else{
        printf("0");
    }
    zapisHuffDrevesa(koren->levo);
    zapisHuffDrevesa(koren->desno);
}

#endif