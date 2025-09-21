#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

//Structs de auxilio

typedef struct Tabuleiro {

    char Ch;
    bool isWord;
    
} Board;

typedef struct Palavra {
    
    char palavra[25];
    int direction;
    int index[2];
    bool revelada;

} Word;

typedef struct Jogo {

    Board **board;
    Word *lista;
    int tamanhoLista;
    int tamanhoTabuleiro;
    int palavrasRestantes;
    
} Game;


// Funções 
Board** CriaTabuleiro( int size );
Game InserePalavras( Game jogo, int size);
Word* InsereNaLista( Word* list, char *word, int i, int j, int way, int listSize );
void PrintaTabuleiro(int size, Board **board);
int EncontraCaminho( Board **board, int size, int i, int j, int wLen);
Game VerificaPalavra ( Game jogo, Word *list, char *word );
Game AtualizaTabuleiro( Game jogo, Word *list, int listIndex );



// Main 

int main (void) {

    int matrixSize = 0;
    
    do {
        printf("Digite o tamanho do tabuleiro ( 2 a 20 ): ");
        scanf("%d", &matrixSize);
    } while ( matrixSize > 20 || matrixSize < 2);

   
    Game jogo;
    jogo.board = CriaTabuleiro( matrixSize );
    jogo.tamanhoTabuleiro = matrixSize;
    jogo.lista = NULL;

    jogo = InserePalavras( jogo, matrixSize );
    PrintaTabuleiro( jogo.tamanhoTabuleiro, jogo.board );

    char palavra[20];

    printf("\n| Caso queira desistir digite '0' |\n");
    while ( jogo.palavrasRestantes > 0 ) {

        printf("\n| Palavras Restantes: %d |\nInsira a palavra que voce achou: ", jogo.palavrasRestantes);
        scanf("%s", palavra );

        if ( strcmp( "0", palavra)  == 0 ) {
            printf("Quem sabe da proxima vez...");
            exit(0);
        }
    
        int len = strlen(palavra);
        for ( int i = 0; i < len; i++ ) {
            palavra[i] = toupper(palavra[i]);
        }
        jogo = VerificaPalavra( jogo, jogo.lista, palavra );
        
    }

    printf(" *** VOCE ENCONTROU TODAS AS PALAVRAS, PARABENS! *** ");

}

// Corpo das funções

Board** CriaTabuleiro( int size ) {

    Board **board;
    board = malloc( sizeof(Board*) * size );

    for( int i = 0; i < size; i++ ){
        board[i] = malloc( sizeof(Board) * size );
    }

    srand(time(NULL));
    int random = 0;
 
 
    for ( int i = 0; i < size; i++ ) {
        for( int j = 0; j < size; j++ ) {
            
            random = rand() % 26 + 'A';
            board[i][j].Ch = random;
            board[i][j].isWord = false;   
       
        }
   }

    return board;
}

void PrintaTabuleiro( int size, Board **board ) {

    printf("====");
    for(int i = 0; i < size; i++ ){
        printf("===");
    }
    printf("\n");

    for ( int i = 0; i < size; i++ ) {
        printf("| ");
        for ( int j = 0; j < size; j++ ) {
            printf(" %c ", board[i][j].Ch );
        }
        printf(" |");
        printf("\n");
    }
    printf("====");
    for(int i = 0; i < size; i++ ){
        printf("===");
    }
    printf("\n");

}

Game InserePalavras ( Game jogo, int size) {

    FILE *arq;
    arq = fopen("input.txt", "r");
    if ( arq == NULL ) {
        printf("ERRO");
        exit(0);
    }

    char word[50];
    jogo.tamanhoLista = 1;
    bool finded = true;

    while( fgets(word, 50, arq) != NULL ) {
        int attempts = 0;

        int len = strlen(word);
        if ( word[len - 1] == '\n' ) {
            word[--len] = '\0';
        }

        if ( size >= len ) {

            do {
                attempts++;
                finded = true;

                int randomI = rand() % size;
                int randomJ = 0;

                if ( len == size && ( randomI == 0 || randomI == size - 1 )) {
                    randomJ = rand() % size;
                } else if ( len == size ) {
                    randomJ = ( rand() % 2 ) * ( size - 1 );
                } else {
                    randomJ = rand() % size;
                }

                int path = EncontraCaminho( jogo.board, size, randomI, randomJ, len );
                int wIndex = 0;

                if ( path != -1 ) {
                    jogo.lista = InsereNaLista( jogo.lista, word, randomI, randomJ, path, jogo.tamanhoLista );
                    jogo.palavrasRestantes = jogo.tamanhoLista;
                    jogo.tamanhoLista++;
                }   

                switch ( path ) {

                    case 1: // BAIXO
                        for ( int i = randomI; i < randomI + len; i++ ) {
                            jogo.board[i][randomJ].Ch = word[wIndex];
                            jogo.board[i][randomJ].isWord = true;
                            wIndex++;
                        }
                        break;
                    case 2: // DIREITA
                        for ( int j = randomJ; j < randomJ + len; j++ ) {
                            jogo.board[randomI][j].Ch = word[wIndex];
                            jogo.board[randomI][j].isWord = true;
                            wIndex++;
                        }
                        break;
                    case 3: // CIMA
                        for ( int i = 0; i < len; i++ ) {
                            jogo.board[randomI-i][randomJ].Ch = word[wIndex];
                            jogo.board[randomI-i][randomJ].isWord = true;
                            wIndex++;
                        }
                        break;
                    case 4: // ESQUERDA
                        for ( int j = 0; j < len; j++ ) {
                            jogo.board[randomI][randomJ-j].Ch = word[wIndex];
                            jogo.board[randomI][randomJ-j].isWord = true;
                            wIndex++;
                        }
                        break;
                    case -1: // ERRO
                        finded = false;
                        break;
                }


            } while ( !finded && attempts < 100);
                if ( !finded ) {
                    printf("erro ao inserir palavra no tabuleiro: %s\n", word);
                }
            
        } else {
            printf("palavra nao cabe no tabuleiro: %s\n", word);
        }
            
    }
    return jogo;
}

int EncontraCaminho( Board **board, int size, int i, int j, int wLen) {

    
    int randomizer[4] = {0,0,0,0};
    int num = 1;

    while ( num != 5 ) {
        int rIndex = rand() % 4;
        if ( randomizer[rIndex] == 0 ) {
            randomizer[rIndex] = num;
            num++;
        }  
    }

    int c = 0;
    int k = 0;
    bool finded = true;
    
    while ( k < 4 ) {
        switch ( randomizer[k] ) {
    
        case 1: // BAIXO
            if ( i + wLen <= size ){ 
                for( c = i; c < i + wLen; c++ ) {
                    if ( board[c][j].isWord ) {
                        finded = false;
                        break;
                    }
                }
                if ( finded ) return 1;
            }
            break;
        case 2: //DIREITA
            if ( j + wLen <= size ) { 
                finded = true;
                for( c = j; c < j + wLen; c++ ) {
                    if ( board[i][c].isWord ) {
                        finded = false;
                        break;
                    }
                }
                if ( finded ) return 2;
            }
            break;
        case 3: //CIMA
            if ( i - wLen + 1 >= 0 ) { 
                finded = true;
                for( c = 0; c < wLen ; c++ ) {
                    if ( board[i-c][j].isWord ) {
                        finded = false;
                        break;
                    }
                }
                if ( finded ) return 3;
            }
            break;
        case 4: //ESQUERDA
            if ( j - wLen + 1 >= 0) { // ESQUERDA
                finded = true;
                for( c = 0; c < wLen; c++ ) {
                    if ( board[i][j-c].isWord ) {
                        finded = false;
                        break;
                    }
                }
                if ( finded ) return 4;
            }
            break;
        }
        k++;
    }

    return -1;

}

Word* InsereNaLista( Word *list, char *word, int i, int j, int way, int listSize ) {

    Word *temp = ( Word *)realloc( list , sizeof(Word) * listSize);
    if ( temp == NULL ) {
        printf(" realloc falhou");
        return list;
    }
    list = temp;    

    list[listSize-1].direction = way;
    list[listSize-1].index[0] = i;
    list[listSize-1].index[1] = j;
    list[listSize-1].revelada = false;
    strcpy( list[listSize-1].palavra, word );

    return list;
    
}

Game VerificaPalavra ( Game jogo, Word *list, char *word ) {
    

    for ( int i = 0; i < jogo.tamanhoLista; i++ ) {
        if ( strcmp( list[i].palavra, word ) == 0 ) {
            
            if ( list[i].revelada ) {
                printf("Palavra ja foi encontrada!\n");
                return jogo;
            }
            jogo.palavrasRestantes--;
            jogo = AtualizaTabuleiro( jogo, jogo.lista, i );
            PrintaTabuleiro( jogo.tamanhoTabuleiro, jogo.board );
            printf("Voce encontrou a palavra: %s\n", list[i].palavra); 
            return jogo;
        }
    }
    printf("\nPalavra nao esta no tabuleiro!\n");

    return jogo;
}

Game AtualizaTabuleiro( Game jogo, Word *list, int listIndex ) {

    int i = list[listIndex].index[0];
    int j = list[listIndex].index[1];
    int path = list[listIndex].direction;
    int len = strlen( list[listIndex].palavra );
    list[listIndex].revelada = true;

    switch ( path ) {

        case 1: // BAIXO
            for ( int k = i; k < i + len; k++ ) {
               jogo.board[k][j].Ch = '*';
            }
            break;
        case 2: // DIREITA
            for ( int k = j; k < j + len; k++ ) {
                jogo.board[i][k].Ch = '*';
            }
            break;
        case 3: // CIMA
            for ( int k = 0; k < len; k++ ) {
                jogo.board[i-k][j].Ch = '*';
            }
            break;
        case 4: // ESQUERDA
            for ( int k = 0; k < len; k++ ) {
                jogo.board[i][j-k].Ch = '*';
            }
            break;

    }

    jogo.lista = list;

    return jogo;

}

