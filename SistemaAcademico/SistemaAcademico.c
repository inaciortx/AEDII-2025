#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <locale.h>

#define D 4
long int indiceAluno;
long int indiceDisciplina;

typedef struct Chave {

    long int index;
    char chave[100];

} Chave;

typedef struct Aluno { 

    char matricula[128];
    char nome_do_aluno[256];
    bool valido;
    
} Aluno;

typedef struct Disciplina {

    char codigo_disciplina[128];
    char nome_da_disciplina[256];
    bool valido;

} Disciplina;

typedef struct Matricula { 

    int id_matricula;
    char matricula_aluno[128];
    char codigo_disciplina[128];
    float media_final;
    bool valido;
     
} Matricula;

typedef struct BTPage {

    int totalChaves;
    bool folha;
    Chave chaves[D-1];
    struct BTPage *filho[D];

} BTPage;

//Funções de interface
void Menu ( BTPage **AlunoRaiz, BTPage **DisciplinaRaiz );
void PrintaDadosAluno( long int index );
void PrintaDadosDisciplina( long int index);

//Funções principais da Btree
BTPage* Insere( BTPage *raiz, Chave key );
BTPage* CriaNo( void ); 
int Busca( BTPage*no, char *chave );

//Funções aux da Btree
void DivideNoFilho( BTPage *pai, int indiceFilho );
void InsereNaoCheio( BTPage *no, Chave key );

//Funções de arquivo ( DADOS )
//void InsereDeArquivoAluno( BTPage **raiz );
//void InsereDeArquivoDisciplina( BTPage **raiz );
void AtualizaArquivoDeDadosAluno( Aluno aluno );
void AtualizaArquivoDeDadosDisciplina( Disciplina disciplina );

// Funções de arquivos ( INDICE )
void SalvaBT( BTPage *no, FILE *fp );
void CarregaBTAlunos( BTPage **raiz, FILE *fp );
void CarregaBTDisciplinas( BTPage ** raiz, FILE *fp );


int main ( void ) {

    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "pt_BR.UTF-8");

    BTPage *alunoRaiz;
    BTPage *disciplinaRaiz;

    alunoRaiz = CriaNo();
    disciplinaRaiz = CriaNo();

    FILE *index_alunos, *index_disciplinas;
    index_alunos = fopen( "indexAlunos.idx", "rb" );
    index_disciplinas = fopen( "indexDisciplinas.idx", "rb" );
    
    CarregaBTAlunos( &alunoRaiz, index_alunos );
    fclose( index_alunos );

    CarregaBTDisciplinas( &disciplinaRaiz, index_disciplinas );
    fclose( index_disciplinas );

    Menu( &alunoRaiz, &disciplinaRaiz );

    index_alunos = fopen( "indexAlunos.idx", "wb" );
    SalvaBT( alunoRaiz, index_alunos );
    fclose(index_alunos);

    index_disciplinas = fopen( "indexDisciplinas.idx", "wb" );
    SalvaBT( disciplinaRaiz, index_disciplinas );
    fclose(index_disciplinas);


}

void Menu ( BTPage **AlunoRaiz, BTPage **DisciplinaRaiz ) {

    int menu = -1;
    int subMenu = -1;
    Aluno aluno;
    Disciplina disciplina;
    Chave key;


    do {
    
    printf("\n======== SISTEMA ACADÊMICO ========\n");
    printf("( 1 ) Alunos\n( 2 ) Disciplinas\n( 3 ) Matrículas\n( 0 ) Sair\nOpção: ");
    scanf("%d", &menu );
    
    switch ( menu ) {
        case 1:
            do {
                printf("\n======== SISTEMA ACADÊMICO->Alunos ========\n");
                printf("( 1 ) Cadastrar Aluno\n( 2 ) Buscar Aluno\n( 3 ) Desligar Aluno\n( 4 ) Atualizar Dados\n( 0 ) Voltar\nOpção: ");
                scanf("%d", &subMenu );
            
            switch ( subMenu ) {

                case 1:
                    printf("Digite a matrícula do aluno:\n");
                    scanf("%s", aluno.matricula );
                    printf("Digite o nome do aluno:\n");
                    scanf(" %[^\n]", aluno.nome_do_aluno );

                    strcpy(key.chave, aluno.matricula);
                    key.index = indiceAluno++;
                    aluno.valido = true;

                    AtualizaArquivoDeDadosAluno( aluno );
                    *AlunoRaiz = Insere( *AlunoRaiz, key );
                    break;

                case 2:
                    printf("Digite a matrícula do aluno:\n");
                    scanf("%s", aluno.matricula );

                    long int index = Busca( *AlunoRaiz, aluno.matricula);

                    if ( index != -1 ) {
                        PrintaDadosAluno( index );
                    } else {
                        printf("Aluno não encontrado na base de dados\n");
                    }

                    break;
                
                case 3:
                    printf("Função nao implementada\n");
                    break;
                case 4: 
                    printf("Função nao implementada\n");
                    break;
                case 0:
                    printf("Voltando...\n");
                    break;
                default:
                    printf("Operação inválida\n");
                    break;
            }
            } while ( subMenu != 0 );
            break;
        case 2: 
            do {
                printf("\n======== SISTEMA ACADÊMICO->Disciplinas ========\n");
                printf("( 1 ) Cadastrar Disciplina\n( 2 ) Buscar Disciplina\n( 3 ) Deletar Disciplina\n( 4 ) Atualizar Dados\n( 0 ) Voltar\nOpção: ");
                scanf("%d", &subMenu );
             switch ( subMenu ) {

                case 1:
                    printf("Digite o código da disciplina:\n");
                    scanf("%s", disciplina.codigo_disciplina );
                    printf("Digite o nome da disciplina:\n");
                    scanf(" %[^\n]", disciplina.nome_da_disciplina );

                    strcpy(key.chave, disciplina.codigo_disciplina);
                    key.index = indiceDisciplina++;
                    disciplina.valido = true;

                    AtualizaArquivoDeDadosDisciplina( disciplina );
                    *DisciplinaRaiz = Insere( *DisciplinaRaiz, key );
                    break;
                case 2:
                    printf("Digite o código da disciplina:\n");
                    scanf("%s", disciplina.codigo_disciplina );

                    long int index = Busca( *DisciplinaRaiz, disciplina.codigo_disciplina);

                    if ( index != -1 ) {
                        PrintaDadosDisciplina( index );
                    } else {
                        printf("Disciplina não encontrada na base de dados\n");
                    }
                    break;
                case 3:
                    printf("Função nao implementada\n");
                    break;
                case 4: 
                    printf("Função nao implementada\n");
                    break;
                case 0:
                    printf("Voltando...\n");
                    break;
                default:
                    printf("Operação inválida\n");
                    break;
            }
            } while ( subMenu != 0 );
            break;
        case 3:
            do {
                printf("( 1 ) Matricular Aluno\n( 2 ) Buscar \n( 3 ) Desmatricular aluno( 4 ) Atualizar Nota\n");
            } while ( true );
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("OPÇÃO INVÁLIDA\n");
            break;
    }
    
    } while ( menu != 0);
}

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

//Divide um nó filho ( CHEIO ) de um nó pai ( NÃO CHEIO )

void DivideNoFilho( BTPage *pai, int indiceFilho ) {

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

void InsereNaoCheio( BTPage *no, Chave key ) {

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

/* void InsereDeArquivoAlunos( BTPage **raiz ) {

    FILE *fp;
    FILE *arq_dados;

    fp = fopen( "aluno.txt", "r" );
    if ( fp == NULL ) {
        printf(" erro ao abrir alunos.txt ");
        exit(1);
    }
    arq_dados = fopen( "alunos_dados.dat", "wb" );
    if ( arq_dados == NULL ) {
        printf(" erro na criação de alunos_dados.dat ");
        exit(1);
    }

    char line[128];
    Chave key; 
    Aluno aluno;
    indiceAluno = 0;
    
    while ( fgets( line, 128, fp) ) {
        
        char *token = strtok( line, "," );
        strcpy( key.chave, token );
        strcpy( aluno.matricula, token);

        token = strtok( NULL, "," );
        token[strcspn(token, "\n")] = 0;
        strcpy( aluno.nome_do_aluno, token );

        key.index = indiceAluno++;
        aluno.valido = true;
      
        fwrite( &aluno, sizeof(Aluno), 1, arq_dados );
        *raiz = Insere( *raiz, key );        
    }

    fclose(fp);
    fclose( arq_dados );

}
*/

/* void InsereDeArquivoDisciplina( BTPage **raiz ) {

    FILE *fp;
    FILE *arq_dados;

    fp = fopen( "disciplinas.txt", "r" );
    if ( fp == NULL ) {
        printf(" erro ao abrir disciplinas.txt ");
        exit(1);
    }
    arq_dados = fopen( "disciplinas_dados.dat", "wb" );
    if ( arq_dados == NULL ) {
        printf(" erro na criação de disciplinas_dados.dat ");
        exit(1);
    }

    char line[128];
    Chave key; 
    Disciplina disciplina;
    indiceDisciplina = 0;
    
    while ( fgets( line, 128, fp) ) {
        
        char *token = strtok( line, "," );
        strcpy( key.chave, token );
        strcpy( disciplina.codigo_disciplina, token);

        token = strtok( NULL, "," );
        token[strcspn(token, "\n")] = 0;
        strcpy( disciplina.nome_da_disciplina, token );

        key.index = indiceDisciplina++;
        disciplina.valido = true;
      
        fwrite( &disciplina, sizeof(Disciplina), 1, arq_dados );
        *raiz = Insere( *raiz, key );        
    }

    fclose(fp);
    fclose( arq_dados );
}*/

void AtualizaArquivoDeDadosAluno( Aluno aluno ) {

    FILE *fp;

    fp = fopen( "alunos_dados.dat", "ab" );
        if ( fp == NULL ) {
            printf(" erro ao abrir alunos_dados.dat ");
            exit(1);
        }

        fwrite( &aluno, sizeof(Aluno), 1, fp );

        fclose( fp );

}

void AtualizaArquivoDeDadosDisciplina( Disciplina disciplina ) {

    FILE *fp;

    fp = fopen( "disciplinas_dados.dat", "ab" );
        if ( fp == NULL ) {
            printf(" erro ao abrir disciplinas_dados.dat ");
            exit(1);
        }

        fwrite( &disciplina, sizeof(Disciplina), 1, fp );

        fclose( fp );

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

void PrintaDadosAluno( long int index ) {

      FILE *fp;
      Aluno aluno;

    fp = fopen( "alunos_dados.dat", "rb" );
        if ( fp == NULL ) {
            printf(" erro ao abrir alunos_dados.dat ");
            exit(1);
        }

    long int offset = index * sizeof(Aluno);

    fseek(fp, offset, SEEK_SET);
    fread(&aluno, sizeof(Aluno), 1, fp);

    if (aluno.valido) {
            printf("\n=== Dados do Aluno ===\n");
            printf("Matrícula: %s\n", aluno.matricula);
            printf("Nome: %s\n", aluno.nome_do_aluno);
            printf("=======================\n");
        } else {
            printf("Aluno com matrícula %s foi encontrado, mas está desligado.\n", aluno.matricula);
        }

    fclose(fp);
}

void PrintaDadosDisciplina( long int index ) {

      FILE *fp;
      Disciplina disciplina;

    fp = fopen( "disciplinas_dados.dat", "rb" );
        if ( fp == NULL ) {
            printf(" erro ao abrir disciplinas_dados.dat ");
            exit(1);
        }

    long int offset = index * sizeof(Disciplina);

    fseek(fp, offset, SEEK_SET);
    fread(&disciplina, sizeof(Disciplina), 1, fp);

    if (disciplina.valido) {
            printf("\n======= Dados da Disciplina ======\n");
            printf("Código da disciplina: %s\n", disciplina.codigo_disciplina);
            printf("Nome da disciplina: %s\n", disciplina.nome_da_disciplina);
            printf("====================================\n");
        } else {
            printf("A disciplina de código %s não está mais ativa\n", disciplina.codigo_disciplina);
        }

    fclose(fp);
}

void SalvaBT( BTPage *no, FILE *fp ) {

    if( no == NULL ) {
        return;
    }

    for ( int i = 0; i < no->totalChaves; i++ ) {
        fwrite( &no->chaves[i], sizeof(Chave), 1, fp );
    }

    if( !no->folha ) {
        for( int i = 0; i <= no->totalChaves; i++ ){
            SalvaBT( no->filho[i], fp );
        }
    }
}

void CarregaBTAlunos( BTPage **raiz, FILE *fp ) {

    Chave key;
    indiceAluno = 0;

    while( fread( &key, sizeof(Chave), 1, fp ) == 1 ) {
        
        *raiz = Insere( *raiz, key );

        if( key.index >= indiceAluno ) {
            indiceAluno = key.index+1;
        }
    }
}

void CarregaBTDisciplinas( BTPage ** raiz, FILE *fp ) {

    Chave key;
    indiceDisciplina = 0;

    while( fread( &key, sizeof(Chave), 1, fp ) == 1 ) {
        
        *raiz = Insere( *raiz, key );

        if( key.index >= indiceDisciplina ) {
            indiceDisciplina = key.index+1;
        }
    }
}