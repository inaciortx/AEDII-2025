#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct TrieNode {

    struct TrieNode **childrens;
    int numOfChildrens;
    bool isEndOfWord;
    int polarity;
    char class[256];
    
} Node;

int EncontraIndice( char c );
Node* CriaNo();
Node* InsereNaTrie( Node *root, char *word, char* class, int polarity );
bool ProcuraNaTrie( Node *root, char *word );
void BuscaPorPrefixo ( Node *root, char *prefix );
void DFSWord( Node *node, char *word, int depth );
void PrintaTrie( Node *node );
void LiberaTrie( Node *root );
void Menu( Node *root );
void removerAcentos( char *word );
Node* InsereDeArquivo();
void DFSWordCompleto( Node *node, char *word, int depth );
Node* EditaPolaridade( Node *root, char *word, int polarity);

int main ( void ) {

    Node *root = CriaNo();
    root = InsereDeArquivo( root );

    Menu( root );
}

Node* CriaNo() {
    
    Node *newNode = ( Node * )malloc( sizeof(Node));
    newNode->isEndOfWord = false;
    newNode->childrens = calloc( 27, sizeof(Node *));
    newNode->numOfChildrens = 0;
    newNode->polarity = 0;

    return newNode;
}


int EncontraIndice( char c ) {
    if ( c == 32 ) return 26;
    return tolower(c) - 'a';
}

Node* InsereNaTrie( Node *root, char *word, char *class, int polarity ) {

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
            node->numOfChildrens++;
            node = node->childrens[EncontraIndice(copia[i])];
        } else {
            node = node->childrens[EncontraIndice(copia[i])];
        }
    }

    node->isEndOfWord = true;
    node->polarity = polarity;
    strcpy( node->class, class);

    return root;
    
}



bool ProcuraNaTrie( Node *root, char *word ) {

    Node *node = root;

    char copia[256];
    strcpy( copia, word );

    for ( int i = 0; copia[i] != '\0'; i++ ) {
        if ( node->childrens[EncontraIndice(copia[i])] != NULL ) {
            node = node->childrens[EncontraIndice(copia[i])];
        } else {
            printf("\nPalavra nao esta na Trie!\n");
            return false;
        }
    }
    if ( node->isEndOfWord ) {
        printf("\n======================\n");
        printf("Palavra: %s\nClasse: %s\nPolaridade: %d\n", word, node->class, node->polarity);
        printf("======================\n");
        return true;
    } 
    printf("\nPalavra nao esta na Trie!\n");
    return false;
}

void BuscaPorPrefixo ( Node *root, char *prefix ) {

    if( root == NULL ) return;

    char copia[256];
    strcpy( copia, prefix );

    Node *node = root;
    char word[256];
    word[0] = '\0';


    for ( int i = 0; copia[i] != '\0'; i++ ) {
        if ( node->childrens[EncontraIndice(copia[i])] != NULL ) {
            node = node->childrens[EncontraIndice(copia[i])];
        } else {
            printf("\nPrefixo nao esta na Trie!\n");
            return;
        }   
    }

    printf("\n===== PALAVRAS COM O PREFIXO '%s' =====\n", copia );
  
    strcpy(word, copia);
    DFSWordCompleto( node, word, strlen(copia) );

    printf("============================\n");
}

void DFSWord( Node *node, char *word, int depth ) {

    if ( node->isEndOfWord ) {
        word[depth] = '\0';
        printf("%s\n", word );
    }

    for( int i = 0; i < 27; i++ ) {
        if( node->childrens[i] != NULL ){

            if ( i == 26 ) {
                word[depth] = ' ';
            } else {
            word[depth] = 'a' + i;
            }
            DFSWord( node->childrens[i], word, depth + 1);
        }
    }
}

void DFSWordCompleto( Node *node, char *word, int depth ) {

    if ( node->isEndOfWord ) {
        word[depth] = '\0';
        printf("\n======================\n");
        printf("Palavra: %s\nClasse: %s\nPolaridade: %d\n", word, node->class, node->polarity);
        printf("======================\n");
    }

    for( int i = 0; i < 27; i++ ) {
        if( node->childrens[i] != NULL ){

            if ( i == 26 ) {
                word[depth] = ' ';
            } else {
            word[depth] = 'a' + i;
            }
            DFSWordCompleto( node->childrens[i], word, depth + 1);
        }
    }
}

void PrintaTrie( Node *node ) {
    
    if ( node == NULL || node->numOfChildrens == 0 ) {
        printf("\nTrie esta vazia\n");
        return;
    }

    char word[256];
    word[0] = '\0';
    printf("\n=== TRIE COMPLETA === \n");
    DFSWord( node, word, 0 );
    printf("=========================");
}

void LiberaTrie(Node *node) {

    if (node == NULL) {
        return;
    }
    for (int i = 0; i < 26; i++) {
        if (node->childrens[i] != NULL) {
            LiberaTrie(node->childrens[i]);
        }
    }
    free(node->childrens);
    free(node);

}

void Menu( Node *root ) {

    int menu = 0;
    int polaridade = 0;
    char word[256];

    while ( menu != 4 ) {

        printf("\n===== ARVORE TRIE SENTIMENTO =====\n");
        printf("(1) Buscar\n(2) Editar polaridade\n(3) Imprimir\n(4) Sair\n   Digite sua escolha: ");
        scanf("%d", &menu );

        switch( menu ) {
            
            case 1:
                do {
                    
                printf("(1) Buscar Palavra\n(2) Buscar por Prefixo\n   Digite sua escolha:  ");
                scanf("%d", &menu );

                if( menu > 2 && menu < 1 ) {
                    printf("Digite um numero valido!\n");
                }

                } while ( menu > 2 && menu < 1 );

                switch( menu ) {

                    case 1: 
                        printf("\nDigite a palavra que quer buscar: ");
                        scanf("%s", word );
                        ProcuraNaTrie( root, word );
                        break;
                    case 2:
                        printf("\nDigite o prefixo que quer buscar: ");
                        scanf("%s", word );
                        BuscaPorPrefixo( root, word );
                        break;
                }
                break;
            
            case 2:
                printf("\nDigite a palavra que quer editar: ");
                scanf("%s", word );
                do {
                    printf("\nDigite a polaridade para ser alterada ( 1, 0, -1 ) : ");
                    scanf("%d", &polaridade);
                }while ( polaridade > 1 || polaridade < -1 );
                root = EditaPolaridade( root, word, polaridade );                        
                break;
            
            case 3:
                PrintaTrie( root );
                break;

        }
        
    }
}

void removerAcentos(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char c = str[i];

        if (c == 0xC3 && (unsigned char)str[i+1] >= 0xA0 && (unsigned char)str[i+1] <= 0xA5) {
            str[i] = 'a';
            memmove(&str[i+1], &str[i+2], strlen(&str[i+2])+1);
        }

        else if (c == 0xC3 && (unsigned char)str[i+1] >= 0xA8 && (unsigned char)str[i+1] <= 0xAB) {
            str[i] = 'e';
            memmove(&str[i+1], &str[i+2], strlen(&str[i+2])+1);
        }

        else if (c == 0xC3 && (unsigned char)str[i+1] >= 0xAC && (unsigned char)str[i+1] <= 0xAF) {
            str[i] = 'i';
            memmove(&str[i+1], &str[i+2], strlen(&str[i+2])+1);
        }

        else if (c == 0xC3 && (unsigned char)str[i+1] >= 0xB2 && (unsigned char)str[i+1] <= 0xB6) {
            str[i] = 'o';
            memmove(&str[i+1], &str[i+2], strlen(&str[i+2])+1);
        }

        else if (c == 0xC3 && (unsigned char)str[i+1] >= 0xBA && (unsigned char)str[i+1] <= 0xBF) {
            str[i] = 'u';
            memmove(&str[i+1], &str[i+2], strlen(&str[i+2])+1);
        }

        else if (c == 0xC3 && (unsigned char)str[i+1] == 0xA7) {
            str[i] = 'c';
            memmove(&str[i+1], &str[i+2], strlen(&str[i+2])+1);
        }
    }
}




Node* InsereDeArquivo( Node *root ) {
    FILE *fp = fopen("entrada.txt" , "r");
    if (!fp ) {
        printf("erro");
    }

    char line[256];
    char word[256];
    char class[256];
    int polarity = 0;   

    while ( fgets( line, 256, fp) ) {
        
        char *token = strtok( line, "," );
        strcpy( word, token );
        token = strtok( NULL, "," );
        strcpy( class, token );
        token = strtok( NULL, ",");
        polarity = atoi(token);

        root = InsereNaTrie( root, word, class, polarity);
    }

    fclose(fp);

    return root;
}

Node* EditaPolaridade( Node *root, char *word, int polarity) {

    Node *node = root;

    char copia[256];
    strcpy( copia, word );

    for ( int i = 0; copia[i] != '\0'; i++ ) {
        if ( node->childrens[EncontraIndice(copia[i])] != NULL ) {
            node = node->childrens[EncontraIndice(copia[i])];
        } else {
            printf("\nPalavra nao esta na Trie!\n");
            return root;
        }
    }
    if ( node->isEndOfWord ) {
        printf("\nPolaridade da palavra '%s' alterada!\n", word );
        printf("\nAnterior: %d / Nova: %d\n", node->polarity, polarity);
        node->polarity = polarity;
        return root;
    } 
    printf("\nPalavra nao esta na Trie!\n");
    return root;
}