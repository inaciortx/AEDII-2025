#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <locale.h>
#include "btree.h"
#include "arquivos.h"

long int indiceAluno = 0;
long int indiceDisciplina = 0;
long int indiceMatricula = 0; 

void Menu ( BTPage **AlunoRaiz, BTPage **DisciplinaRaiz, BTPage **MatriculaRaiz );

int main ( void ) {

    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "pt_BR.UTF-8");

    BTPage *alunoRaiz;
    BTPage *disciplinaRaiz;
    BTPage *matriculaRaiz;

    alunoRaiz = CriaNo();
    disciplinaRaiz = CriaNo();
    matriculaRaiz = CriaNo();

    FILE *index_alunos, *index_disciplinas, *index_matriculas;


    index_alunos = fopen( "indexAlunos.idx", "rb" );
    if ( index_alunos == NULL ) {
        printf(" Carregando dados iniciais de alunos...\n");
        InsereDeArquivoAluno( &alunoRaiz );
    } else {
        CarregaBTAlunos( &alunoRaiz, index_alunos );
        fclose( index_alunos );
    }
    index_disciplinas = fopen( "indexDisciplinas.idx", "rb" );
    if ( index_disciplinas == NULL ) {
        printf(" Carregando dados iniciais de disciplinas...\n");
        InsereDeArquivoDisciplina( &disciplinaRaiz );
    } else {
        CarregaBTDisciplinas( &disciplinaRaiz, index_disciplinas );
        fclose( index_disciplinas );
    }
    index_matriculas = fopen( "indexMatriculas.idx", "rb" );
        if ( index_matriculas == NULL ) {
            printf("Sem matrículas no sistema\n");
        } else {
            CarregaBTMatriculas( &matriculaRaiz, index_matriculas );
            fclose( index_matriculas );
        }
    
    Menu( &alunoRaiz, &disciplinaRaiz, &matriculaRaiz );

    index_alunos = fopen( "indexAlunos.idx", "wb" );
    SalvaBT( alunoRaiz, index_alunos );
    fclose( index_alunos );

    index_disciplinas = fopen( "indexDisciplinas.idx", "wb" );
    SalvaBT( disciplinaRaiz, index_disciplinas );
    fclose(index_disciplinas);

    index_matriculas = fopen("indexMatriculas.idx", "wb" );
    SalvaBT( matriculaRaiz, index_matriculas );
    fclose( index_matriculas );


}

void Menu ( BTPage **AlunoRaiz, BTPage **DisciplinaRaiz, BTPage **MatriculaRaiz ) {

    int menu = -1;
    int subMenu = -1;
    Aluno aluno;
    Disciplina disciplina;
    Matricula matricula;
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
                    printf("Digite a matrícula do aluno: ");
                    scanf("%s", aluno.matricula );

                    int teste = Busca( *AlunoRaiz, aluno.matricula );
                    if ( teste != -1 ) {
                        printf("\nJá existe um aluno com essa matrícula no sistema!");
                        PrintaDadosAluno( teste );
                        break;
                    }
                    
                    printf("Digite o nome do aluno: ");
                    scanf(" %[^\n]", aluno.nome_do_aluno );

                    strcpy(key.chave, aluno.matricula);
                    key.index = indiceAluno++;
                    aluno.valido = true;

                    AdicionaAlunoNoArquivo( aluno );
                    *AlunoRaiz = Insere( *AlunoRaiz, key );
                    break;

                case 2:
                    printf("Digite a matrícula do aluno: ");
                    scanf("%s", aluno.matricula );

                    long int index = Busca( *AlunoRaiz, aluno.matricula);

                    if ( index != -1 ) {
                        PrintaDadosAluno( index );
                    } else {
                        printf("Aluno não encontrado na base de dados\n");
                    }

                    break;
                
                case 3:
                    printf("Digite a matrícula do aluno que quer remover: ");
                    scanf("%s", aluno.matricula );

                    index = Busca( *AlunoRaiz, aluno.matricula );
                    if ( index == -1 ) {
                        printf("\nAluno não encontrado no sistema!\n");
                        break;
                    }
                    RemoverMatriculasPorAluno(aluno.matricula, MatriculaRaiz);
                    *AlunoRaiz = Remove(*AlunoRaiz, aluno.matricula);

                    FILE *fp = fopen("alunos_dados.dat", "r+b");
                    if (fp) {
                        Aluno aluno;
                        fseek(fp, index * sizeof(Aluno), SEEK_SET);
                        fread(&aluno, sizeof(Aluno), 1, fp);
                        aluno.valido = false; 
                        fseek(fp, index * sizeof(Aluno), SEEK_SET);
                        fwrite(&aluno, sizeof(Aluno), 1, fp);
                        fclose(fp);
                    }
        
                    printf("Aluno removido com sucesso!\n");
                    break;
                case 4: 
                    printf("Digite a matrícula que quer atualizar: ");
                    scanf("%s", aluno.matricula );
                    AtualizaAlunoDoArquivo( aluno.matricula, *AlunoRaiz );
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
                    printf("Digite o código da disciplina: ");
                    scanf("%s", disciplina.codigo_disciplina );

                    int teste = Busca( *DisciplinaRaiz, disciplina.codigo_disciplina );
                    if ( teste != -1 ) {
                        printf("\nJá existe uma disciplina com esse código no sistema!");
                        PrintaDadosDisciplina( teste );
                        break;
                    }

                    printf("Digite o nome do aluno: ");
                    scanf(" %[^\n]", aluno.nome_do_aluno );
                    printf("Digite o nome da disciplina: ");
                    scanf(" %[^\n]", disciplina.nome_da_disciplina );

                    strcpy(key.chave, disciplina.codigo_disciplina);
                    key.index = indiceDisciplina++;
                    disciplina.valido = true;

                    AdicionaDisciplinaNoArquivo( disciplina );
                    *DisciplinaRaiz = Insere( *DisciplinaRaiz, key );
                    break;
                case 2:
                    printf("Digite o código da disciplina: ");
                    scanf("%s", disciplina.codigo_disciplina );

                    int index = Busca( *DisciplinaRaiz, disciplina.codigo_disciplina);

                    if ( index != -1 ) {
                        PrintaDadosDisciplina( index );
                    } else {
                        printf("Disciplina não encontrada na base de dados\n");
                    }
                    break;
                case 3:
                    printf("Digite o código da disciplina que quer remover: ");
                    scanf("%s", disciplina.codigo_disciplina);

                    index = Busca(*DisciplinaRaiz, disciplina.codigo_disciplina);
                    
                    if (index == -1) {
                        printf("Disciplina não encontrada no sistema.\n");
                        break;
                    }
                        
                    RemoverMatriculasPorDisciplina(disciplina.codigo_disciplina, MatriculaRaiz);

                    *DisciplinaRaiz = Remove(*DisciplinaRaiz, disciplina.codigo_disciplina);
                    
                    FILE *fp = fopen("disciplinas_dados.dat", "r+b");
                    if (fp) {
                        Disciplina disciplina;
                        fseek(fp, index * sizeof(Disciplina), SEEK_SET);
                        fread(&disciplina, sizeof(Disciplina), 1, fp);
                        
                        disciplina.valido = false; 
                        
                        fseek(fp, index * sizeof(Disciplina), SEEK_SET);
                        fwrite(&disciplina, sizeof(Disciplina), 1, fp);
                        fclose(fp);
                    }
                        
                    printf("Disciplina removida com sucesso!\n");
                    break;
                case 4: 
                    printf("Digite o código da disciplina que quer atualizar: ");
                    scanf("%s", disciplina.codigo_disciplina );
                    AtualizaDisciplinaDoArquivo( disciplina.codigo_disciplina, *DisciplinaRaiz );
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
                printf("\n======== SISTEMA ACADÊMICO->Matriculas ========\n");
                printf("( 1 ) Matricular Aluno\n( 2 ) Buscar \n( 3 ) Desmatricular aluno\n( 4 ) Atualizar Nota\n( 0 ) Voltar\nOpção: ");
                scanf("%d", &subMenu );

                switch ( subMenu ) {

                    case 1:
                        printf("Digite o código da disciplina: ");
                        scanf("%s", matricula.codigo_disciplina );
                        if ( Busca( *DisciplinaRaiz, matricula.codigo_disciplina) == -1 ) {
                            printf("\n** Disciplina não encontrada no sistema! **\n");
                            break;
                        }
                        printf("Digite a matricula do aluno: ");
                        scanf(" %[^\n]", matricula.matricula_aluno);
                        if ( Busca( *AlunoRaiz, matricula.matricula_aluno ) == -1 ) {
                            printf("\n** Aluno não encontrado no sistema! **\n");
                            break;
                        }

                        if ( VerificaMatricula( matricula.matricula_aluno, matricula.codigo_disciplina ) ) {
                            printf("\n*O aluno ");
                            PrintaNomeAluno( matricula.matricula_aluno, *AlunoRaiz );
                            printf(" já esta matriculado na disciplina ");
                            PrintaNomeDisciplina( matricula.codigo_disciplina, *DisciplinaRaiz );
                            printf("*\n");
                            break;
                        }
                        matricula.id_matricula = indiceMatricula;
                        matricula.media_final = 0.0;
                        matricula.valido = true;

                        key.index = indiceMatricula;
                        sprintf(key.chave, "%ld", indiceMatricula );
                        indiceMatricula++;
                        
                        AdicionaMatriculaNoArquivo( matricula );
                        *MatriculaRaiz = Insere( *MatriculaRaiz, key );
                        printf("\nAluno matriculado com sucesso!\n");
                        break;
                    case 2:
                        printf("(1) Buscar disciplinas de aluno\n(2) Buscar alunos de disciplina\nOpção: ");
                        scanf("%d", &subMenu );
                        switch( subMenu ) {
                            case 1:
                                printf("Digite a matricula do aluno: ");
                                scanf("%s", matricula.matricula_aluno);
                                BuscaMatriculas( *AlunoRaiz, *DisciplinaRaiz, matricula.matricula_aluno, 0 );
                                break;

                            case 2: 
                                printf("Digite o código da disciplina: ");
                                scanf("%s", matricula.codigo_disciplina );
                                BuscaMatriculas(*AlunoRaiz, *DisciplinaRaiz, matricula.codigo_disciplina, 1 );
                                break;
                        }
                        break;
                    case 3: {
                        long int id_digitado;
                        printf("Digite o ID da matrícula a cancelar: ");
                        scanf("%ld", &id_digitado);

                        char chave_remocao[20];
                        sprintf(chave_remocao, "%ld", id_digitado);

                        long int index = Busca(*MatriculaRaiz, chave_remocao);

                        if (index == -1) {
                            printf("\nMatrícula não encontrada no sistema!\n");
                            break;
                        }

                        *MatriculaRaiz = Remove(*MatriculaRaiz, chave_remocao);

                        FILE *fp = fopen("matriculas_dados.dat", "r+b");
                        if (fp != NULL) {
                            Matricula matricula;

                            fseek(fp, index * sizeof(Matricula), SEEK_SET);
                            fread(&matricula, sizeof(Matricula), 1, fp);
                            
                            if (matricula.valido) {
                                matricula.valido = false; 
                                
                                fseek(fp, index * sizeof(Matricula), SEEK_SET);
                                fwrite(&matricula, sizeof(Matricula), 1, fp);
                                
                                printf("Matrícula ID %ld removida com sucesso!\n", id_digitado);
                            } else {
                                printf("Esta matrícula já estava removida.\n");
                            }
                            
                            fclose(fp);
                        }
                        break;
                    }
                    case 4:
                        printf("Digite o ID de matricula: ");
                        scanf("%s", key.chave );
                        AtualizaMatriculaDoArquivo( key.chave, *MatriculaRaiz );
                        break;
                    
                }
            } while ( subMenu != 0 );
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


