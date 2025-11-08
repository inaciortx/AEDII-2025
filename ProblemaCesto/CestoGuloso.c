#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include <stdlib.h>


int counter = 0;

typedef struct {
    const char *nome;
    double peso;
    int valor;
    double razao;
} Item;

int compare(const void *a, const void *b) {
    Item *item1 = (Item *)a;
    Item *item2 = (Item *)b;

    // Ordena do MAIOR para o MENOR razão
    if (item1->razao > item2->razao)
        return 1;
    else if (item1->razao < item2->razao)
        return -1;
    else
        return 0;
}

int main(void) {

    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "pt_BR.UTF-8");

    Item itens[] = {
        {"Casca de Carapanaúba", 1.8, 16, 0},
        {"Yãkoana", 4.0, 34, 0},
        {"Tucumã", 2.5, 15, 0},
        {"Jenipapo", 4.5, 28, 0},
        {"Fibra", 1.0, 10, 0}
    };

    for ( int i = 0; i < 5; i++ ) {
        counter++;
        itens[i].razao = itens[i].peso / itens[i].valor;
    }

    qsort( itens, 5, sizeof(Item), compare );

    double capacidade = 7.0;
    double melhor_peso = 0;
    int melhor_valor = 0;

    Item escolhidos[5];
    int indexEscolhidos = 0;

    for ( int i = 0; i < 5; i++ ) {
        counter++;
        if( itens[i].peso + melhor_peso < capacidade ) {
            melhor_peso += itens[i].peso;
            melhor_valor += itens[i].valor;
            escolhidos[indexEscolhidos].nome = itens[i].nome;
            escolhidos[indexEscolhidos].peso = itens[i].peso;
            escolhidos[indexEscolhidos].valor = itens[i].valor;
            indexEscolhidos++;
        }
    }
 
    printf("=== Solução Gulosa ===\n");

    printf("Itens escolhidos:\n");
    for (int i = 0; i < indexEscolhidos; ++i) {
            printf(" - %s (peso %.1f, valor %d)\n", escolhidos[i].nome, escolhidos[i].peso, escolhidos[i].valor);
    }

    printf("Peso total: %.1f kg\n", melhor_peso);
    printf("Valor total: %d\n", melhor_valor);
    printf("%d", counter);


    return 0;
}