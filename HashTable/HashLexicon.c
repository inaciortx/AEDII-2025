#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <locale.h>
#include <math.h>

#define HASH_SIZE 60539

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

    Node node[HASH_SIZE];
    int loadFactor;

} Hash;

Hash *InsereDeArquivo ( Hash *hash );
int PRH ( char *word ); //Polynomial rolling hash
int Hash2( char *word );
Hash *InicializaHash ( void );
void Menu( Hash *hash );
void PrintaInfos( Info infos );
bool BuscaNaHash( Hash *hash, char *word );
Hash* InserePalavra( Hash *hash );
Hash* RemovePalavra( Hash *hash );

int main (void) {

    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "pt_BR.UTF-8");

    Hash *hashTable = InicializaHash();
    hashTable = InsereDeArquivo( hashTable );

    Menu( hashTable );
    
}

Hash* InicializaHash( void ) {

    Hash *hashTable = ( Hash *)malloc(sizeof(Hash));

    for ( int i = 0; i < HASH_SIZE; i++ ) {

        hashTable->node[i].state = -1;
        hashTable->node[i].infos.word[0] = '\0';
        hashTable->node[i].infos.class[0] = '\0';
        hashTable->node[i].infos.classification = '\0';
        hashTable->node[i].infos.polarity = 0;    
        hashTable->loadFactor = HASH_SIZE; 
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
            int key = abs( ( PRH(word) + Hash2(word) * t ) % HASH_SIZE );

            while( hash->node[key].state != -1 ) {
                key = abs( ( PRH(word) + Hash2(word) * t ) % HASH_SIZE );
                t++;
            }

            strcpy( hash->node[key].infos.word, word );
            strcpy( hash->node[key].infos.class, class );
            hash->node[key].infos.polarity = polarity;
            hash->node[key].infos.classification = classification;
            hash->node[key].state = 0;
        } 
        hash->loadFactor--;
    }

    return hash;   
}

int PRH ( char *word ) { //Polynomial Rolling Hash

    long long key = 0;

    for ( int i = 0; word[i] != '\0' ; i++ ) {

        key += (word[i] * (long long)pow( 53, i ));

    }
    
    return abs(key % HASH_SIZE);

}

int Hash2 ( char *word ) {

    long long key = 0;

    for ( int i = 0; word[i] != '\0'; i++ ) {

        key += word[i];

    }

    return abs( key % HASH_SIZE );

}

void Menu( Hash *hash ) {

    int menu = -1;
    char word[256];

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
                hash = InserePalavra( hash );
                break;
            case 4:
                printf("\nAtualmente a Hash tem %d posições livres\n", hash->loadFactor );
                break;
            case 0:
                break;
        }
    }


}

void PrintaInfos( Info infos ) {
    printf("\nPalavra: %s\nCategoria: %s\nPolaridade: %d\nModificação: %c\n", infos.word, infos.class, infos.polarity, infos.classification);
}

bool BuscaNaHash( Hash *hash, char* word ) {

    if ( strcmp( word, hash->node[abs(PRH(word))].infos.word ) == 0 && hash->node[abs(PRH(word))].state == 0) {

        PrintaInfos( hash->node[abs(PRH(word))].infos );

    } else if ( strcmp( word, hash->node[abs(Hash2(word))].infos.word ) == 0 && hash->node[abs(Hash2(word))].state == 0) {

        PrintaInfos( hash->node[abs(Hash2(word))].infos );

    } else {

        int t = 1;
        int key = abs( ( PRH(word) + Hash2(word) * t ) % HASH_SIZE );

        while( strcmp( hash->node[key].infos.word, word) != 0 && t < 1000 ) {            
            key = abs( ( PRH(word) + Hash2(word) * t ) % HASH_SIZE );
            t++;
        }
            if ( t == 1000 || hash->node[key].state == 1 ) {
                return false;
            } 
    }
    return true;
}

Hash* InserePalavra( Hash *hash ) {

    char word[256];
    char class[256];
    int polarity = -2;

    printf("\nDigite as informações que quer inseirir\nPalavra: ");
    scanf("%s", word);

    if ( BuscaNaHash( hash, word ) ){
        printf("\nPalavra já está na Hash!\n");
        return hash;
    }

    printf("Classe gramatical: ");
    scanf("%s", class);
    while ( polarity > 1 || polarity < -1 ) {
        printf("Polaridade ( -1 / 0 / 1 ): ");
        scanf("%d", &polarity);
    }
    
    int key = (PRH(word));

    if ( hash->node[key].state == -1 || hash->node[key].state == 1 ) {

        strcpy( hash->node[key].infos.word, word );
        strcpy( hash->node[key].infos.class, class );
        hash->node[key].infos.polarity = polarity;
        hash->node[key].infos.classification = 'M';
        hash->node[key].state = 0;

    } else if (hash->node[Hash2(word)].state == -1 || hash->node[Hash2(word)].state == 1 ) {

        key = Hash2(word);

        strcpy( hash->node[key].infos.word, word );
        strcpy( hash->node[key].infos.class, class );
        hash->node[key].infos.polarity = polarity;
        hash->node[key].infos.classification = 'M';
        hash->node[key].state = 0;

    } else {

        int t = 1;
        int key = abs( ( PRH(word) + Hash2(word) * t ) % HASH_SIZE );

        while( hash->node[key].state != -1 || hash->node[key].state != 1 ) {
            key = abs( ( PRH(word) + Hash2(word) * t ) % HASH_SIZE );
            t++;
        }

        strcpy( hash->node[key].infos.word, word );
        strcpy( hash->node[key].infos.class, class );
        hash->node[key].infos.polarity = polarity;
        hash->node[key].infos.classification = 'M';
        hash->node[key].state = 0;

    } 
    printf("\nPalavra adicionada com suscesso!\n");
    hash->loadFactor--;
    return hash;
}

Hash* RemovePalavra( Hash *hash ) {

    char word[256];

    printf("Digite a palavra que quer remover: ");
    scanf("%s", word);

    if ( strcmp( word, hash->node[abs(PRH(word))].infos.word ) == 0 && hash->node[abs(PRH(word))].state == 0) {

        hash->node[abs(PRH(word))].state = 1;
        hash->loadFactor++;
        printf("\nPalavra removida com suscesso!\n");
        return hash;

    } else if ( strcmp( word, hash->node[abs(Hash2(word))].infos.word ) == 0 && hash->node[abs(Hash2(word))].state == 0) {

        hash->node[abs(Hash2(word))].state = 1;
        hash->loadFactor++;
        printf("\nPalavra removida com suscesso!\n");
        return hash;

    } else {

        int t = 1;
        int key = abs( ( PRH(word) + Hash2(word) * t ) % HASH_SIZE );

        while( strcmp( hash->node[key].infos.word, word) != 0 && t < 1000 ) {            
            key = abs( ( PRH(word) + Hash2(word) * t ) % HASH_SIZE );
            t++;
        }
            if ( t == 1000 || hash->node[key].state == 1 ) {
                return hash;
            } else {
                hash->node[key].state = 1;
                hash->loadFactor++;
                printf("\nPalavra removida com suscesso!\n");
            }
    }

    return hash;

}