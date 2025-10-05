#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <locale.h>

#define ALPHABET_SIZE 10

typedef struct TrieNode {

    struct TrieNode **childrens;
    bool isEndOfWord;
    char **wordsList;
    int listIndex;
 
} Node;

int EncontraIndice( char c );
Node* CriaNo();
Node* InsereNaTrie( Node *root, char *word);
bool BuscaPorCodigo( Node *root, char *code );
void BuscaPorPrefixo ( Node *root, char *prefix );
void LiberaTrie( Node *root );
void Menu( Node *root );
void removerAcentos( char *word );
Node* InsereDeArquivo();
void DFSWord( Node *node );
void PrintaNodo( char **list, int listSize );

int main ( void ) {

    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "pt_BR.UTF-8");

    Node *root = CriaNo();
    root = InsereDeArquivo( root );

    Menu( root );
    LiberaTrie ( root );

    system( "pause" );

}

Node* CriaNo() {
    
    Node *newNode = ( Node * )malloc( sizeof(Node));
    newNode->isEndOfWord = false;
    newNode->childrens = calloc( (ALPHABET_SIZE), sizeof(Node *));
    newNode->listIndex = 0;
    newNode->wordsList = NULL;

    return newNode;
}

int EncontraIndice( char c ) {
    
      switch ( c ){
        case 'A':
        case 'a':
        case 'b':
        case 'B':
        case 'C':
        case 'c':
            return 2;
            break;
        case 'd':
        case 'D':
        case 'e':
        case 'E':
        case 'F':
        case 'f':
            return 3;
            break;
        case 'g':
        case 'G':
        case 'h':
        case 'H':
        case 'i':
        case 'I':        
            return 4;
            break;
        case 'j':
        case 'J':
        case 'k':
        case 'K':
        case 'l':
        case 'L':        
            return 5;
            break;
        case 'M':
        case 'm':
        case 'o':
        case 'O':
        case 'n':
        case 'N':        
            return 6;
            break;
        case 'Q':
        case 'q':
        case 'R':
        case 'r':
        case 's':
        case 'S': 
        case 'p':
        case 'P':       
            return 7;
            break;
        case 't':
        case 'T':
        case 'u':
        case 'U':
        case 'v':
        case 'V':        
            return 8;
            break;
        case 'w':
        case 'W':
        case 'x':
        case 'X':
        case 'y':
        case 'Y': 
        case 'z':
        case 'Z':       
            return 9;
            break;
        default:
            return 0;
            break;
    }   
}

Node* InsereNaTrie( Node *root, char *word ) {

    if ( root == NULL ) {
        root = CriaNo();
    }

    char copia[256];
    strcpy( copia, word );
    removerAcentos( copia );

    Node *node = root;

    for ( int i = 0; copia[i] != '\0'; i++ ) {

        if ( node->childrens[EncontraIndice(copia[i])] == NULL ) {
            node->childrens[EncontraIndice(copia[i])] = CriaNo();
            node = node->childrens[EncontraIndice(copia[i])];
        } else {
            node = node->childrens[EncontraIndice(copia[i])];
        }
    }

    node->isEndOfWord = true;

    node->wordsList = realloc( node->wordsList, sizeof( char *) * (node->listIndex + 1) );
    node->wordsList[node->listIndex] = malloc( strlen(word) + 1 );

    strcpy(node->wordsList[node->listIndex], word);
    node->listIndex++;
 
    return root;
    
}

bool BuscaPorCodigo( Node *root, char *code ) {

    Node *node = root;
  
    for ( int i = 0; code[i] != '\0'; i++ ) {
        if ( node->childrens[code[i] - '0'] != NULL ) {
            node = node->childrens[code[i] - '0'];
        } else {
            printf("\nNenhuma palavra corresponde a esse codigo!\n");
            return false;
        }

    }
    if ( node->isEndOfWord ) {
        printf("\nPalavra que correpondem ao codigo: %s\n", code);
        PrintaNodo( node->wordsList, node->listIndex );
        return true;
    } 
    printf("\nNenhuma palavra corresponde a esse codigo!\n");
    return false;
}

void BuscaPorPrefixo ( Node *root, char *prefix ) {

    Node *node = root;

    for ( int i = 0; prefix[i] != '\0'; i++ ) {
        if ( node->childrens[prefix[i] - '0'] != NULL ) {
            node = node->childrens[prefix[i] - '0'];
        } else {
            printf("\nPrefixo nao esta na Trie!\n");
            return;
        }   
    }

    printf("\n===== PALAVRAS COM O PREFIXO '%s' =====\n", prefix );
  
    DFSWord( node );

    printf("============================\n");
}

void DFSWord( Node *node ) {

    if ( node->isEndOfWord ) {
       PrintaNodo( node->wordsList, node->listIndex );
    }
    for( int i = 0; i < ALPHABET_SIZE; i++ ) {

        if( node->childrens[i] != NULL ){

            DFSWord( node->childrens[i] );

        }
    }
}


void LiberaTrie(Node *node) {

    if (node == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->childrens[i] != NULL) {
            LiberaTrie(node->childrens[i]);
        }
    }
    free(node->childrens);
    free(node);

}

void Menu( Node *root ) {

    int menu = 2;
    char code[256];

    while ( menu != 0 ) {

        printf("\n===== ARVORE TRIE 9K =====\n");
        printf("(1) Busca por código\n(2) Busca pro prefixo\n(0) Sair\n   Digite sua escolha: ");
        scanf("%d", &menu );

        switch (menu){

            case 1:
                printf("Digite o codigo 9K ( EX: 2272 ): ");
                scanf("%s", code);
                BuscaPorCodigo( root, code );
                break;
            case 2: 
                printf("Digite o codigo 9K do prefixo ( EX: 2232 ): ");
                scanf("%s", code);
                BuscaPorPrefixo( root, code );
                break;
        }

    }   
}

void removerAcentos(char *str) { 
    
    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char c1 = (unsigned char)str[i];
        unsigned char c2 = (unsigned char)str[i+1];

        if (c1 == 0xC3) {
            switch (c2) {
                // A/a com acento
                case 0x81: case 0x80: case 0x82: case 0x83: case 0x84: 
                    str[i] = 'A'; break;
                case 0xA1: case 0xA0: case 0xA2: case 0xA3: case 0xA4: 
                    str[i] = 'a'; break;

                // E/e com acento
                case 0x89: case 0x88: case 0x8A: case 0x8B: 
                    str[i] = 'E'; break;
                case 0xA9: case 0xA8: case 0xAA: case 0xAB: 
                    str[i] = 'e'; break;

                // I/i com acento
                case 0x8D: case 0x8C: case 0x8E: case 0x8F: 
                    str[i] = 'I'; break;
                case 0xAD: case 0xAC: case 0xAE: case 0xAF: 
                    str[i] = 'i'; break;

                // O/o com acento
                case 0x93: case 0x92: case 0x94: case 0x95: case 0x96:  
                    str[i] = 'O'; break;
                case 0xB3: case 0xB2: case 0xB4: case 0xB5: case 0xB6:  
                    str[i] = 'o'; break;

                // U/u com acento
                case 0x9A: case 0x99: case 0x9B: case 0x9C: 
                    str[i] = 'U'; break;
                case 0xBA: case 0xB9: case 0xBB: case 0xBC: 
                    str[i] = 'u'; break;

                // �/�
                case 0x87: str[i] = 'C'; break; 
                case 0xA7: str[i] = 'c'; break; 
            }

            memmove(&str[i+1], &str[i+2], strlen(&str[i+2])+1);
        }
    }
}

Node* InsereDeArquivo( Node *root ) {
    FILE *fp = fopen("entrada9k.txt" , "r");
    if (!fp ) {
        printf("erro");
    }

    char word[256];
  
    while ( fgets( word, 256, fp) ) {

        word[strlen(word)-1]='\0';
        root = InsereNaTrie( root, word );
    }

    fclose(fp);

    return root;
}

void PrintaNodo( char **list, int listSize ) {
    for ( int i = 0; i < listSize; i++ ) {
            printf("- %s\n", list[i]);
        }
}
