#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para peça
typedef struct {
    char tipo;
    int id;
} Peca;

#define MAX 5

// Estrutura para fila
typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->total == MAX;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->total == 0;
}

// Gera automaticamente uma nova peça
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Inserção (enqueue)
void inserir(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia. Não é possível inserir.\n");
        return;
    }

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX;
    f->total++;
}

// Remoção (dequeue)
void remover(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        printf("Fila vazia. Não é possível remover.\n");
        return;
    }

    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
}

// Exibe a fila de peças
void mostrarFila(Fila *f) {
    printf("Fila de Peças: ");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
        printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
    }
    printf("\n");
}

// Função principal
int main() {
    Fila f;
    inicializarFila(&f);
    srand(time(NULL));

    int contadorID = 0;
    int opcao;

    // Inicializa com 5 peças
    for (int i = 0; i < MAX; i++) {
        inserir(&f, gerarPeca(contadorID++));
    }

    do {
        mostrarFila(&f);
        printf("\n1 - Jogar Peça (dequeue)\n");
        printf("2 - Inserir Nova Peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca removida;
            remover(&f, &removida);
        } 
        else if (opcao == 2) {            
            if (!filaCheia(&f)) {
                Peca nova = gerarPeca(contadorID++);
                inserir(&f, nova);
                printf("Nova peça inserida: [%c %d]\n\n", nova.tipo, nova.id);
            } else {
                printf("Fila cheia. Não é possível inserir.\n\n");
            }
        }

    } while (opcao != 0);

    printf("Encerrando o jogo...\n");
    return 0;
}
