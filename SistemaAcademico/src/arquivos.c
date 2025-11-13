#include "arquivos.h"

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

void PrintaDadosMatricula( long int index ) {

      FILE *fp;
      Matricula matricula;

    fp = fopen( "matriculas_dados.dat", "rb" );
        if ( fp == NULL ) {
            printf(" erro ao abrir matriculas_dados.dat ");
            exit(1);
        }

    long int offset = index * sizeof(Matricula);

    fseek(fp, offset, SEEK_SET);
    fread(&matricula, sizeof(Matricula), 1, fp);

    if (matricula.valido) {
            printf("\n=== Dados da Matricula ===\n");
            printf("Matrícula: %s\n", matricula.matricula_aluno);
            printf("Código da disciplina: %s\n", matricula.codigo_disciplina);
            printf("Nota: %.2f\n", matricula.media_final);
            printf("=======================\n");
        } else {
            printf("Matricula não está mais ativa");
        }

    fclose(fp);
}

void PrintaNomeAluno( char *chave, BTPage *raiz ) {

    int index = Busca( raiz, chave  );
    
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
            printf("%s", aluno.nome_do_aluno );
        }

    fclose(fp);
}

void PrintaNomeDisciplina( char *chave, BTPage *raiz ) {

    int index = Busca( raiz, chave  );
    
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
            printf("%s", disciplina.nome_da_disciplina );
        }

    fclose(fp);
}

void InsereDeArquivoAluno( BTPage **raiz ) {

    FILE *fp;
    FILE *arq_dados;

    fp = fopen( "alunos.txt", "r" );
    if ( fp == NULL ) {
        printf(" erro ao abrir alunos.txt ");
        exit(1);
    }
    arq_dados = fopen( "alunos_dados.dat", "wb" );
    if ( arq_dados == NULL ) {
        printf(" erro na criação de alunos_dados.dat ");
        fclose( fp );
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

void InsereDeArquivoDisciplina( BTPage **raiz ) {

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
        fclose( fp );
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
}

void AdicionaAlunoNoArquivo( Aluno aluno ) {

    FILE *fp;

    fp = fopen( "alunos_dados.dat", "ab" );
        if ( fp == NULL ) {
            printf(" erro ao abrir alunos_dados.dat ");
            exit(1);
        }

        fwrite( &aluno, sizeof(Aluno), 1, fp );

        fclose( fp );

}

void AdicionaDisciplinaNoArquivo( Disciplina disciplina ) {

    FILE *fp;

    fp = fopen( "disciplinas_dados.dat", "ab" );
        if ( fp == NULL ) {
            printf(" erro ao abrir disciplinas_dados.dat ");
            exit(1);
        }

        fwrite( &disciplina, sizeof(Disciplina), 1, fp );

        fclose( fp );

}

void AdicionaMatriculaNoArquivo( Matricula matricula ) {

    FILE *fp;

    fp = fopen( "matriculas_dados.dat", "ab" );
        if ( fp == NULL ) {
            printf(" erro ao abrir matriculas_dados.dat ");
            exit(1);
        }

        fwrite( &matricula, sizeof(Matricula), 1, fp );

        fclose( fp );
}

void AtualizaAlunoDoArquivo( char *chave, BTPage *raiz ){  
    
    long int index = Busca( raiz, chave );
    if ( index == -1 ) {
        printf("Aluno não encontrado no banco de dados\n");
        return;
    }

    printf("\n* DADOS ATUAIS DO ALUNO *");
    PrintaDadosAluno( index );

    long int offset = index * sizeof(Aluno);
    Aluno aluno;
    int menu = -1;

    FILE *fp;
    fp = fopen( "alunos_dados.dat", "r+b" ); 
    if ( fp == NULL ) {
        printf(" erro ao abrir alunos_dados.dat para atualizar");
        exit(1);
    }
    
    fseek(fp, offset, SEEK_SET);
    fread( &aluno, sizeof(Aluno), 1, fp );


    while ( menu != 0 ) {
        printf("\nATUALIZAR DADOS:\n(1) Nome\n(0) Confirmar mudanças\nOpção: ");
        scanf("%d", &menu );

        switch ( menu ) {

            case 1:
                printf("Novo nome: ");
                scanf(" %[^\n]", aluno.nome_do_aluno );
                break;
            case 0:
                printf("Registro atualizado com sucesso!\n");
                break;
        }
    }

    fseek(fp, offset, SEEK_SET);
    fwrite( &aluno, sizeof(Aluno), 1, fp );

    fclose( fp );

}

void AtualizaDisciplinaDoArquivo( char *chave, BTPage *raiz ) {
    
    long int index = Busca( raiz, chave );
    if ( index == -1 ) {
        printf("Disciplina não encontrada no banco de dados\n");
        return;
    }

    printf("\n* DADOS ATUAIS DA DISCIPLINA *");
    PrintaDadosDisciplina( index );

    long int offset = index * sizeof(Disciplina);
    Disciplina disciplina;
    int menu = -1;

    FILE *fp;
    fp = fopen( "disciplinas_dados.dat", "r+b" ); 
    if ( fp == NULL ) {
        printf(" erro ao abrir disciplinas_dados.dat para atualizar");
        exit(1);
    }
    
    fseek(fp, offset, SEEK_SET);
    fread( &disciplina, sizeof(Disciplina), 1, fp );


    while ( menu != 0 ) {
        printf("\nATUALIZAR DADOS:\n(1) Nome\n(0) Confirmar mudanças\nOpção: ");
        scanf("%d", &menu );

        switch ( menu ) {

            case 1:
                printf("Novo nome: ");
                scanf(" %[^\n]", disciplina.nome_da_disciplina );
                break;
            case 0:
                printf("Registro atualizado com sucesso!\n");
                break;
        }
    }

    fseek(fp, offset, SEEK_SET);
    fwrite( &disciplina, sizeof(Disciplina), 1, fp );

    fclose( fp );
}

void AtualizaMatriculaDoArquivo( char *chave, BTPage *raiz ) {

    long int index = Busca( raiz, chave );
    if ( index == -1 ) {
        printf("Matricula não encontrada no banco de dados\n");
        return;
    }

    printf("\n* DADOS ATUAIS DA MATRICULA *");
    PrintaDadosMatricula( index );

    long int offset = index * sizeof(Matricula);
    Matricula matricula;

    FILE *fp;
    fp = fopen( "matriculas_dados.dat", "r+b" ); 
    if ( fp == NULL ) {
        printf(" erro ao abrir matriculas_dados.dat para atualizar");
        exit(1);
    }
    
    fseek(fp, offset, SEEK_SET);
    fread( &matricula, sizeof(Matricula), 1, fp );

    printf("Média final da discplina: ");
    scanf("%f", &matricula.media_final);

    fseek(fp, offset, SEEK_SET);
    fwrite( &matricula, sizeof(Matricula), 1, fp );

    fclose( fp );
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

void BuscaMatriculas( BTPage *alunos, BTPage *disciplinas, char *chave, int search ){

    FILE *fp;
    Matricula matricula;
    int infos = 0;

    switch ( search ) {

        case 0:
            fp = fopen( "matriculas_dados.dat", "rb");
            if ( fp == NULL ) {
                printf("Erro ao abrir matricular_dat!");
                exit(1);
            }
        while ( fread( &matricula, sizeof(Matricula), 1, fp ) == 1 ) {
            if ( strcmp( matricula.matricula_aluno, chave) == 0 && matricula.valido ){
                if ( infos == 0 ) {
                    printf("\nMatriculas do Aluno: ");
                    PrintaNomeAluno( matricula.matricula_aluno, alunos );
                    printf("\n");
                } 
                printf("( ID: %ld ) - Disciplina: %s - ( Nota: %.2f )\n", matricula.id_matricula, matricula.codigo_disciplina, matricula.media_final );
                infos++;
            }
        }
        if ( infos == 0 ) {
            printf("Nenhuma matricula encontrada para este Aluno");
        }
        fclose(fp);
        break;
        
        case 1: 
        fp = fopen( "matriculas_dados.dat", "rb");
        if ( fp == NULL ) {
                printf("Erro ao abrir matricular_dat!");
                exit(1);
            }
        while ( fread( &matricula, sizeof(Matricula), 1, fp ) == 1 ) {
            if ( strcmp( matricula.codigo_disciplina, chave) == 0 && matricula.valido ){
                if ( infos == 0 ) {
                    printf("\nAlunos da Disciplina: ");
                    PrintaNomeDisciplina( matricula.codigo_disciplina, disciplinas);
                    printf("\n");
                } 
                printf("( ID: %ld ) - Matricula: %s - ( Nota: %.2f )\n", matricula.id_matricula, matricula.matricula_aluno, matricula.media_final );
                infos++;
            }
        }
        if ( infos == 0 ) {
            printf("Nenhuma matricula encontrada para esta disciplina");
        }
        fclose(fp);
        break;

    }

}

void CarregaBTMatriculas( BTPage **raiz, FILE *fp ) {

    Chave key;
    indiceMatricula = 0;

    while( fread( &key, sizeof(Chave), 1, fp ) == 1 ) {
        
        *raiz = Insere( *raiz, key );

        if( key.index >= indiceMatricula ) {
            indiceMatricula = key.index+1;
        }
    }

}

bool VerificaMatricula(char *aluno, char *disciplina ) {
    FILE *fp;
    Matricula matricula;

    fp = fopen("matriculas_dados.dat", "rb");
    if (fp == NULL) {
        return false; 
    }

    while (fread(&matricula, sizeof(Matricula), 1, fp) == 1) {
        if (matricula.valido && 
            strcmp(matricula.matricula_aluno, aluno) == 0 && 
            strcmp(matricula.codigo_disciplina, disciplina ) == 0) {
            
            fclose(fp);
            return true; 
        }
    }

    fclose(fp);
    return false; 
}

void RemoverMatriculasPorAluno(char *matricula_aluno, BTPage **MatriculaRaiz) {
    FILE *fp = fopen("matriculas_dados.dat", "r+b"); 
    if (fp == NULL) return; 

    Matricula matricula;
    char chave_remocao[20];
    long offset = 0;

    while (fread(&matricula, sizeof(Matricula), 1, fp) == 1) {
        
        if (matricula.valido && strcmp(matricula.matricula_aluno, matricula_aluno) == 0) {
            
            sprintf(chave_remocao, "%ld", matricula.id_matricula);
            *MatriculaRaiz = Remove(*MatriculaRaiz, chave_remocao);

            matricula.valido = false;
            
            fseek(fp, offset, SEEK_SET); 
            fwrite(&matricula, sizeof(Matricula), 1, fp);
            fseek(fp, offset + sizeof(Matricula), SEEK_SET);
            
            printf("-> Matrícula (ID: %ld, Disciplina: %s) removida em cascata.\n", matricula.id_matricula, matricula.codigo_disciplina);
        }
        
        offset += sizeof(Matricula);
    }

    fclose(fp);
}

void RemoverMatriculasPorDisciplina(char *codigo_disciplina, BTPage **MatriculaRaiz) {
    FILE *fp = fopen("matriculas_dados.dat", "r+b"); 
    if (fp == NULL) return; 

    Matricula matricula;
    char chave_remocao[20];
    long offset = 0;

    while (fread(&matricula, sizeof(Matricula), 1, fp) == 1) {
        
        if (matricula.valido && strcmp(matricula.codigo_disciplina, codigo_disciplina) == 0) {
            
            sprintf(chave_remocao, "%ld", matricula.id_matricula);
            *MatriculaRaiz = Remove(*MatriculaRaiz, chave_remocao);

            matricula.valido = false;
            
            fseek(fp, offset, SEEK_SET); 
            fwrite(&matricula, sizeof(Matricula), 1, fp);
            fseek(fp, offset + sizeof(Matricula), SEEK_SET); 
            
            printf("-> Matrícula (ID: %ld, Matricua do aluno: %s) removida em cascata.\n", matricula.id_matricula, matricula.matricula_aluno);
        }
        
        offset += sizeof(Matricula);
    }

    fclose(fp);
}