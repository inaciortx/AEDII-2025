#include "btree.h"

// Aux inserção
static void DivideNoFilho( BTPage *pai, int indiceFilho );
static void InsereNaoCheio( BTPage *no, Chave key );
// Aux remoção
static int EncontraIndice(BTPage *no, char *chave);
static void RemoveDaFolha(BTPage *no, int idx);
static Chave Antecessor(BTPage *no, int idx);
static Chave Sucessor(BTPage *no, int idx);
static void FundirPaginas(BTPage *no, int idx);
static void EmprestaAnterior(BTPage *no, int idx);
static void EmprstaSeguinte(BTPage *no, int idx);
static void Preencher(BTPage *no, int idx);
static void RemoveNo(BTPage *no, char *chave);

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

BTPage* Remove( BTPage *raiz, char *chave ) {

    if (!raiz) {
        printf("A árvore está vazia.\n");
        return NULL;
    }

    RemoveNo(raiz, chave);

    if (raiz->totalChaves == 0) {
        BTPage *tmp = raiz;
        if (raiz->folha) {
            raiz = NULL; 
        } else {
            raiz = raiz->filho[0];
        }
        free(tmp);
        if (raiz == NULL) {
            raiz = CriaNo();
        }
    }
    return raiz;

}

static int EncontraIndice(BTPage *no, char *chave) {

    int i = 0;
    while (i < no->totalChaves && strcmp(no->chaves[i].chave, chave) < 0) {
        i++;
    }
    return i;

}

static void RemoveDaFolha(BTPage *no, int idx) {

    for (int i = idx + 1; i < no->totalChaves; ++i) {
        no->chaves[i - 1] = no->chaves[i];
    }
    no->totalChaves--;
}

static Chave Antecessor(BTPage *no, int idx) {
    BTPage *atual = no->filho[idx];
    while (!atual->folha) {
        atual = atual->filho[atual->totalChaves];
    }
    return atual->chaves[atual->totalChaves - 1];
}

static Chave Sucessor(BTPage *no, int idx) {
    BTPage *atual = no->filho[idx + 1];
    while (!atual->folha) {
        atual = atual->filho[0];
    }
    return atual->chaves[0];
}

static void FundirPaginas(BTPage *no, int idx) {
    BTPage *filho = no->filho[idx];
    BTPage *irmao = no->filho[idx + 1];
    int t = D / 2; 

    filho->chaves[t - 1] = no->chaves[idx];

    for (int i = 0; i < irmao->totalChaves; ++i) {
        filho->chaves[i + t] = irmao->chaves[i];
    }

    if (!filho->folha) {
        for (int i = 0; i <= irmao->totalChaves; ++i) {
            filho->filho[i + t] = irmao->filho[i];
        }
    }

    filho->totalChaves += irmao->totalChaves + 1; 

    for (int i = idx + 1; i < no->totalChaves; ++i) {
        no->chaves[i - 1] = no->chaves[i];
    }
    for (int i = idx + 2; i <= no->totalChaves; ++i) {
        no->filho[i - 1] = no->filho[i];
    }

    no->totalChaves--;

    free(irmao);
}

static void EmprestaAnterior(BTPage *no, int idx) {
    BTPage *filho = no->filho[idx];
    BTPage *irmao = no->filho[idx - 1];

    for (int i = filho->totalChaves - 1; i >= 0; --i) {
        filho->chaves[i + 1] = filho->chaves[i];
    }
    if (!filho->folha) {
        for (int i = filho->totalChaves; i >= 0; --i) {
            filho->filho[i + 1] = filho->filho[i];
        }
    }

    filho->chaves[0] = no->chaves[idx - 1];
    if (!filho->folha) {
        filho->filho[0] = irmao->filho[irmao->totalChaves];
    }

    no->chaves[idx - 1] = irmao->chaves[irmao->totalChaves - 1];

    filho->totalChaves++;
    irmao->totalChaves--;
}

static void EmprstaSeguinte(BTPage *no, int idx) {
    BTPage *filho = no->filho[idx];
    BTPage *irmao = no->filho[idx + 1];

    filho->chaves[filho->totalChaves] = no->chaves[idx];
    if (!filho->folha) {
        filho->filho[filho->totalChaves + 1] = irmao->filho[0];
    }

    no->chaves[idx] = irmao->chaves[0];

    for (int i = 1; i < irmao->totalChaves; ++i) {
        irmao->chaves[i - 1] = irmao->chaves[i];
    }
    if (!irmao->folha) {
        for (int i = 1; i <= irmao->totalChaves; ++i) {
            irmao->filho[i - 1] = irmao->filho[i];
        }
    }

    filho->totalChaves++;
    irmao->totalChaves--;
}

static void Preencher(BTPage *no, int idx) {
    int t = D / 2; 


    if (idx != 0 && no->filho[idx - 1]->totalChaves >= t) {
        EmprestaAnterior(no, idx);
    }

    else if (idx != no->totalChaves && no->filho[idx + 1]->totalChaves >= t) {
        EmprstaSeguinte(no, idx);
    }

    else {
        if (idx != no->totalChaves) {
            FundirPaginas(no, idx); 
        } else {
            FundirPaginas(no, idx - 1); 
        }
    }
}

static void RemoveNo(BTPage *no, char *chave) {
    int idx = EncontraIndice(no, chave);
    int t = D / 2; 

    // A chave está neste nó
    if (idx < no->totalChaves && strcmp(no->chaves[idx].chave, chave) == 0) {
        
        if (no->folha) {
           
            RemoveDaFolha(no, idx);
        } else {
            
            BTPage *filho = no->filho[idx];
            BTPage *irmao = no->filho[idx + 1];

            if (filho->totalChaves >= t) {
                
                Chave antecessor = Antecessor(no, idx);
                no->chaves[idx] = antecessor;
                RemoveNo(filho, antecessor.chave);
            } 
            else if (irmao->totalChaves >= t) {
                
                Chave sucessor = Sucessor(no, idx);
                no->chaves[idx] = sucessor;
                RemoveNo(irmao, sucessor.chave);
            } 
            else {
                
                FundirPaginas(no, idx);
                RemoveNo(filho, chave);
            }
        }
    } else {
        
        if (no->folha) {
            return;
        }

        bool flag = ( (idx == no->totalChaves) ? true : false );

        if (no->filho[idx]->totalChaves < t) {
            Preencher(no, idx);
        }

        if (flag && idx > no->totalChaves) {
            RemoveNo(no->filho[idx - 1], chave);
        } else {
            RemoveNo(no->filho[idx], chave);
        }
    }
}