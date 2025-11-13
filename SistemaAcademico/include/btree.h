#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define D 4 

typedef struct Chave {
    long int index;
    char chave[100];
} Chave;

typedef struct BTPage {
    int totalChaves;
    bool folha;
    Chave chaves[D-1];
    struct BTPage *filho[D];
} BTPage;

BTPage* CriaNo( void ); 
BTPage* Insere( BTPage *raiz, Chave key );
int Busca( BTPage *no, char *chave );
BTPage* Remove( BTPage *raiz, char *chave );

#endif 