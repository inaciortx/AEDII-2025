#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 95

typedef struct TrieNode {

    struct TrieNode **childrens;
    int numOfChildrens;
    bool isEndOfWord;
    int polarity;
    char class[256];
    char modified;
    
} Node;

int EncontraIndice( char c );
Node* CriaNo();
Node* InsereNaTrie( Node *root, char *word, char* class, int polarity, char modified );
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
bool SalvaEmArquivo(Node *root, char *filename);
void DFSSalvar(Node *node, FILE *fp, char *word, int depth);

int main ( void ) {

    Node *root = CriaNo();
    root = InsereDeArquivo( root );

    Menu( root );
}

Node* CriaNo() {
    
    Node *newNode = ( Node * )malloc( sizeof(Node));
    newNode->isEndOfWord = false;
    newNode->childrens = calloc( (ALPHABET_SIZE), sizeof(Node *));
    newNode->numOfChildrens = 0;
    newNode->polarity = 0;

    return newNode;
}

int EncontraIndice( char c ) {
    return c - 32;
}

Node* InsereNaTrie( Node *root, char *word, char *class, int polarity, char modified ) {

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
    node->modified = modified;
    strcpy( node->class, class);

    return root;
    
}

bool ProcuraNaTrie( Node *root, char *word ) {

    Node *node = root;

    char copia[256];
    strcpy( copia, word );
    removerAcentos(copia);

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
        printf("Palavra: %s\nClasse: %s\nPolaridade: %d\nModificacao: %c\n", word, node->class, node->polarity, node->modified);
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
    removerAcentos(copia);

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

    for( int i = 0; i < ALPHABET_SIZE; i++ ) {
        if( node->childrens[i] != NULL ){

            word[depth] = i + 32;
            
            DFSWord( node->childrens[i], word, depth + 1);
        }
    }
}

void DFSWordCompleto( Node *node, char *word, int depth ) {

    if ( node->isEndOfWord ) {
        word[depth] = '\0';
        printf("\n======================\n");
        printf("Palavra: %s\nClasse: %s\nPolaridade: %d\nModificacao: %c\n", word, node->class, node->polarity, node->modified);
        printf("======================\n");
    }

    for( int i = 0; i < ALPHABET_SIZE; i++ ) {
        if( node->childrens[i] != NULL ){
    
            word[depth] = i + 32;
            
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
    for (int i = 0; i < ALPHABET_SIZE; i++) {
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

    while ( menu != 5 ) {

        printf("\n===== ARVORE TRIE SENTIMENTO =====\n");
        printf("(1) Buscar\n(2) Editar polaridade\n(3) Imprimir\n(4) Atualizar Arquivo\n(5) Sair\n   Digite sua escolha: ");
        scanf("%d", &menu );

        switch( menu ) {
            
            case 1:
                do {
                    
                printf("(1) Buscar Palavra\n(2) Buscar por Prefixo\n   Digite sua escolha:  ");
                scanf("%d", &menu );

                if( menu > 2 || menu < 1 ) {
                    printf("Digite um numero valido!\n");
                }

                } while ( menu > 2 || menu < 1 );

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

            case 4:
                SalvaEmArquivo( root , "saida.txt" );
                break;

        }
        
    }
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

Node* InsereDeArquivo( Node *root ) {
    FILE *fp = fopen("entrada.txt" , "r");
    if (!fp ) {
        printf("erro");
    }

    char line[256];
    char word[256];
    char class[256];
    int polarity = 0;   
    char modified;

    while ( fgets( line, 256, fp) ) {
        
        char *token = strtok( line, "," );
        strcpy( word, token );
        token = strtok( NULL, "," );
        strcpy( class, token );
        token = strtok( NULL, ",");
        polarity = atoi(token);
        token = strtok( NULL, ",");
        modified = token[0];

        root = InsereNaTrie( root, word, class, polarity, modified);
    }

    fclose(fp);

    return root;
}

Node* EditaPolaridade( Node *root, char *word, int polarity) {

    Node *node = root;

    char copia[256];
    strcpy( copia, word );
    removerAcentos(copia);

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
        node->modified = 'M';
        return root;
    } 
    printf("\nPalavra nao esta na Trie!\n");
    return root;
}

bool SalvaEmArquivo(Node *root, char *filename) {

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("erro");
        return false;
    }

    printf("\nSalvando dados no arquivo '%s'...\n", filename);

    char word[256]; 

    DFSSalvar(root, fp, word, 0);

    fclose(fp);
    printf("Dados salvos com sucesso!\n");
    return true;
}

void DFSSalvar(Node *node, FILE *fp, char *word, int depth) {
    if (node == NULL) {
        return;
    }

    if (node->isEndOfWord) {
        word[depth] = '\0'; 
        
        fprintf(fp, "%s,%s,%d,%c\n", word, node->class, node->polarity, node->modified);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->childrens[i] != NULL) {
            word[depth] = i + 32; 
            DFSSalvar(node->childrens[i], fp, word, depth + 1);
        }
    }
}