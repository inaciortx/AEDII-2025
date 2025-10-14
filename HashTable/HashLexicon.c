#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <locale.h>
#include <math.h>

#define HASH_SIZE 50000
int reHashFactor = 1;

typedef struct Informacao {

    char word[256];
    char class[256];
    int polarity;
    char classification;

} Info;

typedef struct Nodo {

    Info infos;
    int state; // -1) Não ocupada // 0) Ocupada // 1) Removida
            
} Node;

typedef struct HashTable {

    Node *node;
    int freeSlots;

} Hash;

Hash *InsereDeArquivo ( Hash *hash );

int PRH ( char *word ); //Polynomial rolling hash
int Hash2( char *word );
int Hash3( char *word, int t);

Hash *InicializaHash ( void );
void Menu( Hash *hash );
void PrintaInfos( Info infos );
bool BuscaNaHash( Hash *hash, char *word );
Hash* InserePalavra( Hash *hash, char *word, char *class, int polarity, char classification );
Hash* RemovePalavra( Hash *hash );
Hash* ReHashing( Hash *hash );
void removerAcentos(char *str);

int main (void) {

    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "pt_BR.UTF-8");

    Hash *hashTable = InicializaHash();
    hashTable = InsereDeArquivo( hashTable );

    Menu( hashTable );
    
}

Hash* InicializaHash( void ) {

    Hash *hashTable = ( Hash *)malloc(sizeof(Hash));
    hashTable->node = malloc( sizeof(Node) * ( HASH_SIZE * reHashFactor) );
    hashTable->freeSlots = HASH_SIZE * reHashFactor;

    for ( int i = 0; i < HASH_SIZE * reHashFactor; i++ ) {

        hashTable->node[i].state = -1;
        hashTable->node[i].infos.word[0] = '\0';
        hashTable->node[i].infos.class[0] = '\0';
        hashTable->node[i].infos.classification = '\0';
        hashTable->node[i].infos.polarity = 0;    
    }

    return hashTable;
}

Hash* InsereDeArquivo ( Hash *hash ) {

    FILE *fp = fopen("OpLexicon.txt" , "r");
    if (!fp ) {
        printf("erro");
        exit(1);
    }

    char line[256];
    char word[256];
    char class[256];
    int polarity = 0;   
    char classification;

    while ( fgets( line, 256, fp) ) {
        
        char *token = strtok( line, "," );
        strcpy( word, token );
        token = strtok( NULL, "," );
        strcpy( class, token );
        token = strtok( NULL, ",");
        polarity = atoi(token);
        token = strtok( NULL, ",");
        classification = token[0];
        
        int key = (PRH(word));

        if ( hash->node[key].state == -1 ) {

            strcpy( hash->node[key].infos.word, word );
            strcpy( hash->node[key].infos.class, class );
            hash->node[key].infos.polarity = polarity;
            hash->node[key].infos.classification = classification;
            hash->node[key].state = 0;

        } else if ( hash->node[Hash2(word)].state == -1 ) {

            key = Hash2(word);

            strcpy( hash->node[key].infos.word, word );
            strcpy( hash->node[key].infos.class, class );
            hash->node[key].infos.polarity = polarity;
            hash->node[key].infos.classification = classification;
            hash->node[key].state = 0;

        } else {

            int t = 1;
            int key = Hash3(word, t);

            while( hash->node[key].state != -1 ) {
                key = Hash3(word, t);
                t++;
            }

            strcpy( hash->node[key].infos.word, word );
            strcpy( hash->node[key].infos.class, class );
            hash->node[key].infos.polarity = polarity;
            hash->node[key].infos.classification = classification;
            hash->node[key].state = 0;

        } 
        hash->freeSlots--;
        if ( hash->freeSlots < (HASH_SIZE * reHashFactor * 0.3) ) {
            hash = ReHashing( hash );
        }
    }

    return hash;   
}

int PRH ( char *word ) { //Polynomial Rolling Hash

    char temp[256];
    strcpy(temp, word);
    removerAcentos(temp);

    long long key = 0;

    for ( int i = 0; temp[i] != '\0' ; i++ ) {

        key += (temp[i] * (long long)pow( 53, i ));

    }
    
    return abs(key % ( HASH_SIZE * reHashFactor ) );

}

int Hash2 ( char *word ) {

    char temp[256];
    strcpy(temp, word);
    removerAcentos(temp);

    long long key = 0;

    for ( int i = 0; temp[i] != '\0'; i++ ) {

        key += temp[i];

    }

    return abs( key % ( HASH_SIZE * reHashFactor ));

}

int Hash3 ( char *word, int t ) {
    return abs( ( PRH(word) + Hash2(word) * t ) % ( HASH_SIZE * reHashFactor ) );
}

void Menu( Hash *hash ) {

    int menu = -1;
    char word[256];
    char class[256];
    int polarity = -2;

    while ( menu != 0 ) {

        printf("\n========== MENU ==========\n");
        printf("(1) Buscar\n(2) Remover\n(3) Inserir\n(4) Verificar Posições \n(0) Sair\n");
        printf("Digite sua opção: ");
        scanf("%d", &menu);

        switch( menu ) {

            case 1:
                printf("Digite a palavra que quer buscar: ");
                scanf("%s", word );
                if (!BuscaNaHash( hash, word )) {
                    printf("\nPalavra não está na Hash!\n");
                };
                break;
            case 2:
                hash = RemovePalavra( hash );
                break;
            case 3:
                
                printf("\nDigite as informações que quer inserir\nPalavra: ");
                scanf("%s", word);

                if ( BuscaNaHash( hash, word ) ){
                    printf("\nPalavra já está na Hash!\n");
                    break;
                }

                printf("Classe gramatical: ");
                scanf("%s", class);
                while ( polarity > 1 || polarity < -1 ) {
                    printf("Polaridade ( -1 / 0 / 1 ): ");
                    scanf("%d", &polarity);
                }
                hash = InserePalavra( hash, word, class, polarity, 'M' );
                printf("\nPalavra adicionada com sucesso!\n");
                break;
            case 4:
                printf("\nAtualmente a Hash tem %d posições livres\n", hash->freeSlots );
                break;
            case 0:
                free(hash->node);
                free(hash);
                break;
        }
    }


}

void PrintaInfos( Info infos ) {
    printf("\nPalavra: %s\nCategoria: %s\nPolaridade: %d\nModificação: %c\n", infos.word, infos.class, infos.polarity, infos.classification);
}

bool BuscaNaHash( Hash *hash, char* word ) {

    int key = PRH(word);

    if ( strcmp( word, hash->node[key].infos.word ) == 0 && hash->node[key].state == 0) {

        PrintaInfos( hash->node[key].infos );

    } else if ( strcmp( word, hash->node[(Hash2(word))].infos.word ) == 0 && hash->node[(Hash2(word))].state == 0) {
        PrintaInfos( hash->node[(Hash2(word))].infos );

    } else {

        int t = 1;
        int key = Hash3( word, t );
        
        while( strcmp( hash->node[key].infos.word, word) != 0 && t < 1000 ) {            
            key = Hash3( word, t );
            t++;
        }
            if ( t == 1000 || hash->node[key].state == 1 ) {
                return false;
            }

        PrintaInfos( hash->node[key].infos );
    }
    return true;
}

Hash* InserePalavra ( Hash *hash, char *word, char *class, int polarity, char classification ) {

    int key = (PRH(word));

    if ( hash->node[key].state != 0 ) {

        strcpy( hash->node[key].infos.word, word );
        strcpy( hash->node[key].infos.class, class );
        hash->node[key].infos.polarity = polarity;
        hash->node[key].infos.classification = classification;
        hash->node[key].state = 0;

    } else if (hash->node[Hash2(word)].state != 0 ) {

        key = Hash2(word);

        strcpy( hash->node[key].infos.word, word );
        strcpy( hash->node[key].infos.class, class );
        hash->node[key].infos.polarity = polarity;
        hash->node[key].infos.classification = classification;
        hash->node[key].state = 0;

    } else {

        int t = 1;
        int key = Hash3(word, t);

        while( !(hash->node[key].state == 0) ) {
            key = Hash3(word, t);
            t++;
        }

        strcpy( hash->node[key].infos.word, word );
        strcpy( hash->node[key].infos.class, class );
        hash->node[key].infos.polarity = polarity;
        hash->node[key].infos.classification = classification;
        hash->node[key].state = 0;

    }
    hash->freeSlots--;

    if ( hash->freeSlots < (HASH_SIZE * reHashFactor * 0.30) ) {
        hash = ReHashing( hash );
    }
    return hash;
}

Hash* RemovePalavra( Hash *hash ) {

    char word[256];

    printf("Digite a palavra que quer remover: ");
    scanf("%s", word);

    int key = PRH(word);

    if ( strcmp( word, hash->node[key].infos.word ) == 0 && hash->node[key].state == 0) {

        hash->node[key].state = 1;
        hash->freeSlots++;
        printf("\nPalavra removida com suscesso!\n");
        return hash;

    } else if ( strcmp( word, hash->node[Hash2(word)].infos.word ) == 0 && hash->node[Hash2(word)].state == 0) {

        hash->node[Hash2(word)].state = 1;
        hash->freeSlots++;
        printf("\nPalavra removida com suscesso!\n");
        return hash;

    } else {

        int t = 1;
        int key = Hash3(word, t);

        while( strcmp( hash->node[key].infos.word, word) != 0 && t < 1000 ) {            
            key = Hash3(word, t);
            t++;
        }
            if ( t == 1000 || hash->node[key].state == 1 ) {
                return hash;
            } else {
                hash->node[key].state = 1;
                hash->freeSlots++;
                printf("\nPalavra removida com suscesso!\n");
            }
    }

    return hash;

}

Hash* ReHashing( Hash *hash ) {

    reHashFactor++;

    Hash *newHash = InicializaHash();

    printf("\nAumentando o tamanho da Hash...\n");

    for ( int i = 0; i < HASH_SIZE * ( reHashFactor - 1); i++ ){

        if ( hash->node[i].state == 0 ) {
            newHash = InserePalavra( newHash, hash->node[i].infos.word, hash->node[i].infos.class, hash->node[i].infos.polarity, hash->node[i].infos.classification );
        }
    }
    free(hash->node);
    free(hash);

    return newHash;
}

void removerAcentos(char *str) { // Tive que procurar como faz isso pra não dar problema na impressão
    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char c1 = (unsigned char)str[i];
        unsigned char c2 = (unsigned char)str[i+1];

        // Acentos estão em UTF-8, começam com 0xC3
        if (c1 == 0xC3) {
            switch (c2) {
                // A/a com acento
                case 0x81: case 0x80: case 0x82: case 0x83: case 0x84: // Á À Â Ã Ä
                    str[i] = 'A'; break;
                case 0xA1: case 0xA0: case 0xA2: case 0xA3: case 0xA4: // á à â ã ä
                    str[i] = 'a'; break;

                // E/e com acento
                case 0x89: case 0x88: case 0x8A: case 0x8B: // É È Ê Ë
                    str[i] = 'E'; break;
                case 0xA9: case 0xA8: case 0xAA: case 0xAB: // é è ê ë
                    str[i] = 'e'; break;

                // I/i com acento
                case 0x8D: case 0x8C: case 0x8E: case 0x8F: // Í Ì Î Ï
                    str[i] = 'I'; break;
                case 0xAD: case 0xAC: case 0xAE: case 0xAF: // í ì î ï
                    str[i] = 'i'; break;

                // O/o com acento
                case 0x93: case 0x92: case 0x94: case 0x95: case 0x96: // Ó Ò Ô Õ Ö
                    str[i] = 'O'; break;
                case 0xB3: case 0xB2: case 0xB4: case 0xB5: case 0xB6: // ó ò ô õ ö
                    str[i] = 'o'; break;

                // U/u com acento
                case 0x9A: case 0x99: case 0x9B: case 0x9C: // Ú Ù Û Ü
                    str[i] = 'U'; break;
                case 0xBA: case 0xB9: case 0xBB: case 0xBC: // ú ù û ü
                    str[i] = 'u'; break;

                // Ç/ç
                case 0x87: str[i] = 'C'; break; // Ç
                case 0xA7: str[i] = 'c'; break; // ç
            }

            // remove o segundo byte do caractere UTF-8
            memmove(&str[i+1], &str[i+2], strlen(&str[i+2])+1);
        }
    }
}