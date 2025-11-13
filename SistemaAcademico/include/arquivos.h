#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdio.h>
#include <stdbool.h>
#include "btree.h" 

extern long int indiceAluno;
extern long int indiceDisciplina;
extern long int indiceMatricula; 

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
    long int id_matricula;
    char matricula_aluno[128];
    char codigo_disciplina[128];
    float media_final;
    bool valido;
} Matricula;

// Funções de Interface
void PrintaDadosAluno( long int index );
void PrintaDadosDisciplina( long int index );
void PrintaDadosMatricula( long int index );
void PrintaNomeDisciplina( char *chave, BTPage *raiz );
void PrintaNomeAluno( char *chave, BTPage *raiz );

// Funções de arquivo de entrada
void InsereDeArquivoAluno( BTPage **raiz ); 
void InsereDeArquivoDisciplina( BTPage **raiz );
 
// Funções de arquivo de dados
void AdicionaAlunoNoArquivo( Aluno aluno ); 
void AdicionaDisciplinaNoArquivo( Disciplina disciplina ); 
void AdicionaMatriculaNoArquivo( Matricula matricula );
void RemoverMatriculasPorAluno(char *matricula_aluno, BTPage **MatriculaRaiz);
void RemoverMatriculasPorDisciplina(char *codigo_disciplina, BTPage **MatriculaRaiz);

void AtualizaAlunoDoArquivo( char *chave, BTPage *raiz ); 
void AtualizaDisciplinaDoArquivo( char *chave, BTPage *raiz ); 
void AtualizaMatriculaDoArquivo( char *chave, BTPage *raiz );
void BuscaMatriculas( BTPage *alunos, BTPage *disciplinas, char *chave, int search );
bool VerificaMatricula(char *aluno, char *disciplina);

// Funções de arquivos de índice
void SalvaBT( BTPage *no, FILE *fp ); 
void CarregaBTAlunos( BTPage **raiz, FILE *fp );
void CarregaBTDisciplinas( BTPage **raiz, FILE *fp );
void CarregaBTMatriculas( BTPage **raiz, FILE *fp ); 




#endif 