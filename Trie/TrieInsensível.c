#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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
char* ConverteEmMinuscula( char *word );
void LiberaTrie( Node *root );

int main ( void ) {

    Node *root = CriaNo();
    InsereNaTrie( root, "Gato" );
    InsereNaTrie( root, "gato" );
    InsereNaTrie( root, "Gata");

    PrintaTrie( root );
    RemoverPalavra( root, "gata" );
    PrintaTrie( root );

    LiberaTrie(root);
}

Node* CriaNo() {
    
    Node *newNode = ( Node * )malloc( sizeof(Node));
    newNode->isEndOfWord = false;
    newNode->childrens = calloc( 26, sizeof(Node *));
    newNode->numOfChildrens = 0;

    return newNode;
}


int EncontraIndice( char c ) {
    return tolower(c) - 'a';
}

Node* InsereNaTrie( Node *root, char *word ) {

    if ( root == NULL ) {
        root = CriaNo();
    }

    char copia[256];
    strcpy( copia, word );
    ConverteEmMinuscula( copia );

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

    return root;
    
}

bool ProcuraNaTrie( Node *root, char *word ) {

    Node *node = root;

    char copia[256];
    strcpy( copia, word );
    ConverteEmMinuscula( copia );

    for ( int i = 0; copia[i] != '\0'; i++ ) {
        if ( node->childrens[EncontraIndice(copia[i])] != NULL ) {
            node = node->childrens[EncontraIndice(copia[i])];
        } else {
            printf("\nPalavra nao esta na Trie!\n");
            return false;
        }
    }
    if ( node->isEndOfWord ) {
        printf("\nPalavra esta na Trie!\n");
        return true;
    } 
    printf("\nPalavra nao esta na Trie!\n");
    return false;
}

void BuscaPorPrefixo ( Node *root, char *prefix ) {

    if( root == NULL ) return;

    char copia[256];
    strcpy( copia, prefix );
    ConverteEmMinuscula( copia );

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
    DFSWord( node, word, strlen(copia) );

    printf("============================\n");
}

void DFSWord( Node *node, char *word, int depth ) {

    if ( node->isEndOfWord ) {
        word[depth] = '\0';
        printf("%s\n", word );
    }

    for( int i = 0; i < 26; i++ ) {
        if( node->childrens[i] != NULL ){

            word[depth] = 'a' + i;
            DFSWord( node->childrens[i], word, depth + 1);
        }
    }
}

void PrintaTrie( Node *node ) {
    
    if ( node == NULL || node->numOfChildrens == 0 ) {
        printf("Trie esta vazia\n");
        return;
    }

    char word[256];
    word[0] = '\0';
    printf("\n=== TRIE COMPLETA === \n");
    DFSWord( node, word, 0 );
    printf("=========================");
}

Node* RemoverPalavra( Node *root, char *word ) {
    
    char copia[256];
    strcpy( copia, word);
    ConverteEmMinuscula( copia );

    return RemoveDaTrie( root, copia, 0);
    
}

Node* RemoveDaTrie( Node *node, char *word, int index) {

    if ( word[index] == '\0') {
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
            printf("\nPalavra nao esta na Trie!\n");
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

char* ConverteEmMinuscula( char *word ) {

    for ( int i = 0; word[i] != '\0'; i++ ) {
        word[i] = tolower(word[i]);
    }

    return word;
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