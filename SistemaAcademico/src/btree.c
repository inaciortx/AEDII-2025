#include "btree.h"

static void DivideNoFilho( BTPage *pai, int indiceFilho );
static void InsereNaoCheio( BTPage *no, Chave key );

BTPage* CriaNo( void ) {

    BTPage *novoNo = calloc( 1, sizeof(BTPage) );
    if ( novoNo == NULL ) {
        printf(" ERRO ");
        exit(1);
    }
    novoNo->totalChaves = 0;
    novoNo->folha = true;

    return novoNo;

}

BTPage* Insere( BTPage *raiz, Chave key ) {
    BTPage *r = raiz;

    if ( r->totalChaves == D -1 ) {
        
        BTPage *novaRaiz = CriaNo();
        novaRaiz->folha = false;
        novaRaiz->filho[0] = r;

        DivideNoFilho( novaRaiz, 0 );

        InsereNaoCheio( novaRaiz, key );
        return novaRaiz; 
    } else {

        InsereNaoCheio( r, key );
        return r;
    }

}

int Busca( BTPage *no, char *chave ) {

    int indexFilho = 0;

    while( indexFilho < no->totalChaves && strcmp( chave, no->chaves[indexFilho].chave) > 0 ){
        indexFilho++;
    }

    if ( indexFilho < no->totalChaves && strcmp( chave, no->chaves[indexFilho].chave) == 0 ) {
        return no->chaves[indexFilho].index;
    }
    
    if( no->folha ) {
        return -1;
    }

    return Busca( no->filho[indexFilho], chave );
    
}

static void DivideNoFilho( BTPage *pai, int indiceFilho ) {

    BTPage *filhoCheio = pai->filho[indiceFilho];

    BTPage *novoNo = CriaNo();
    novoNo->folha = filhoCheio->folha; 

    int t = D / 2; 

    for (int j = 0; j < t - 1; j++) { 
        novoNo->chaves[j] = filhoCheio->chaves[j + t];
    }
    novoNo->totalChaves = t - 1; 

    if (!filhoCheio->folha) {

        for (int j = 0; j < t; j++) { 
            novoNo->filho[j] = filhoCheio->filho[j + t];
        }
    }

    filhoCheio->totalChaves = t - 1; 

    for (int j = pai->totalChaves; j >= indiceFilho + 1; j--) {
        pai->filho[j + 1] = pai->filho[j];
    }

    pai->filho[indiceFilho + 1] = novoNo;

    for (int j = pai->totalChaves - 1; j >= indiceFilho; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
    }

    pai->chaves[indiceFilho] = filhoCheio->chaves[t - 1]; 


    pai->totalChaves++;
}

static void InsereNaoCheio( BTPage *no, Chave key ) {

    int indiceChave =  no->totalChaves - 1;

    if ( no->folha ) {
        while ( indiceChave >= 0 && strcmp(key.chave, no->chaves[indiceChave].chave) < 0 ) {
            no->chaves[indiceChave + 1] = no->chaves[indiceChave];
            indiceChave--;
        }
        no->chaves[indiceChave + 1] = key;
        no->totalChaves++;
    } else {
        while (indiceChave >= 0 && strcmp(key.chave, no->chaves[indiceChave].chave) < 0) {
            indiceChave--;
        }
        indiceChave++;
    
        if ( no->filho[indiceChave]->totalChaves == D - 1 ) {
            DivideNoFilho( no, indiceChave );

            if ( strcmp(key.chave, no->chaves[indiceChave].chave) > 0 ) {
                indiceChave++;
            }
        }
    
        InsereNaoCheio( no->filho[indiceChave], key );
    }
}