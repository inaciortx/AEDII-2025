#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ALPHABET_SIZE 52

typedef struct TrieNode {

    struct TrieNode **childrens;
    int numOfChildrens;
    bool isEndOfWord;

} Node;

int EncontraIndice( char c );
Node* CriaNo();
Node* InsereNaTrie( Node *root, char *word );
bool ProcuraNaTrie( Node *root, char *word );
void BuscaPorPrefixo ( Node *root, char *prefix );
void DFSWord( Node *node, char *word, int depth );
void PrintaTrie( Node *node );
Node* RemoverPalavra( Node *root, char *word );
Node* RemoveDaTrie( Node *node, char *word, int index );
void LiberaTrie( Node *root );
void Menu( Node *root );

int main ( void ) {

    Node *root = NULL;
    Menu( root );
    LiberaTrie( root );
  
}

Node* CriaNo() {
    
    Node *newNode = ( Node * )malloc( sizeof(Node));
    newNode->isEndOfWord = false;
    newNode->childrens = calloc( ALPHABET_SIZE, sizeof(Node *));
    newNode->numOfChildrens = 0;

    return newNode;
}


int EncontraIndice( char c ) {
    
    if ( c >= 'a' && c <= 'z' ) {
        return c - 'a';
    } else {
        return c - 'A'+26;
    }
    
}

Node* InsereNaTrie( Node *root, char *word ) {

    if ( root == NULL ) {
        root = CriaNo();
    }

    Node *node = root;

    for ( int i = 0; word[i] != '\0'; i++ ) {

        if ( node->childrens[EncontraIndice(word[i])] == NULL ) {
            node->childrens[EncontraIndice(word[i])] = CriaNo();
            node->numOfChildrens++;
            node = node->childrens[EncontraIndice(word[i])];
        } else {
            node = node->childrens[EncontraIndice(word[i])];
        }
    }
    if ( node->isEndOfWord ) {
        printf("\nPalavra ja esta na Trie: %s\n", word );
        return root;
    }
    node->isEndOfWord = true;
    printf("\nPalavra adicionada com suscesso: %s\n", word);

    return root;
    
}

bool ProcuraNaTrie( Node *root, char *word ) {

    Node *node = root;

    for ( int i = 0; word[i] != '\0'; i++ ) {
        if ( node->childrens[EncontraIndice(word[i])] != NULL ) {
            node = node->childrens[EncontraIndice(word[i])];
        } else {
            printf("\nPalavra nao esta na Trie: %s\n", word);
            return false;
        }
    }
    if ( node->isEndOfWord ) {
        printf("\nPalavra esta na Trie: %s \n", word);
        return true;
    } 
    printf("\nPalavra nao esta na Trie: %s\n", word);
    return false;
}

void BuscaPorPrefixo ( Node *root, char *prefix ) {

    Node *node = root;
    char word[256];
    word[0] = '\0';


    for ( int i = 0; prefix[i] != '\0'; i++ ) {
        if ( node->childrens[EncontraIndice(prefix[i])] != NULL ) {
            node = node->childrens[EncontraIndice(prefix[i])];
        } else {
            printf("\nPrefixo nao esta na Trie!\n");
            return;
        }   
    }

    printf("\n===== PALAVRAS COM O PREFIXO '%s' =====\n", prefix );
  
    strcpy(word, prefix);
    DFSWord( node, word, strlen(prefix) );

    printf("============================\n");
}

void DFSWord( Node *node, char *word, int depth ) {

    if ( node->isEndOfWord ) {
        word[depth] = '\0';
        printf("%s\n", word );
    }

    for( int i = 0; i < ALPHABET_SIZE; i++ ) {
        if( node->childrens[i] != NULL ){

            if ( i < 26 ) {
                word[depth] = 'a' + i;
            } else {
                word[depth] = 'A' + ( i - 26 );
            }
            DFSWord( node->childrens[i], word, depth + 1);
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

Node* RemoverPalavra( Node *root, char *word ) {
    
    return RemoveDaTrie( root, word, 0);
    
}

Node* RemoveDaTrie( Node *node, char *word, int index) {

    if (node == NULL) {
        return NULL;
    }

    if ( word[index] == '\0') {
        printf("\nPalavra removida com suscesso: %s\n", word);
        node->isEndOfWord = false;
    }
    

    if ( word[index] != '\0' ) {
        if ( node->childrens[EncontraIndice(word[index])] != NULL ) {
            Node* children = node->childrens[EncontraIndice(word[index])];
            node->childrens[EncontraIndice(word[index])] = RemoveDaTrie( node->childrens[EncontraIndice(word[index])], word, index + 1 );
            if ( children != NULL && node->childrens[EncontraIndice(word[index])] == NULL ) {
                node->numOfChildrens--;
            }
        }
        else {
            printf("\nPalavra nao esta na Trie: %s\n", word);
            return node;
        }
    }

    if ( node->numOfChildrens == 0 && !node->isEndOfWord) {
        free(node->childrens);
        free(node);
        node = NULL;
        return node;
    }

    return node;
 
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
    char word[256];

    while ( menu != 5 ) {

        printf("\n===== ARVORE TRIE =====\n");
        printf("(1) Inserir\n(2) Buscar\n(3) Remover\n(4) Imprimir\n(5) Sair\nDigite sua escolha: ");
        scanf("%d", &menu );

        switch( menu ) {
            
            case 1:
                printf("\nDigite a palavra para ser inserida: ");
                scanf("%s", word );
                root = InsereNaTrie( root, word );
                break;

            case 2:
                do {
                    
                printf("(1) Buscar Palavra\n(2) Buscar por Prefixo");
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
            
            case 3:
                printf("\nDigite a palavra que quer remover ");
                scanf("%s", word );
                root = RemoverPalavra( root, word );                        
                break;
            
            case 4:
                PrintaTrie( root );
                break;

        }
        
    }
}