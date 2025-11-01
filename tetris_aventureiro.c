#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -----------------------------
// PEÇA
// -----------------------------
typedef struct {
    char tipo;
    int id;
} Peca;

// -----------------------------
// FILA CIRCULAR (peças futuras)
// -----------------------------
#define MAX 5

typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f)   { return f->total == MAX; }
int filaVazia(Fila *f)   { return f->total == 0; }

Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

void inserir(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia. Não é possível inserir.\n");
        return;
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX;
    f->total++;
}

void remover(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        printf("Fila vazia. Não é possível remover.\n");
        return;
    }
    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
}

void mostrarFila(Fila *f) {
    printf("Fila de peças\t");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
        printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
    }
    printf("\n");
}

// -----------------------------
// PILHA LINEAR (reserva)
// -----------------------------
#define RES_MAX 3

typedef struct {
    Peca itens[RES_MAX];
    int topo; // -1 indica pilha vazia
} Pilha;

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == RES_MAX - 1; }

void push(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("Pilha de reserva cheia. Não é possível reservar.\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = nova;
}

void pop(Pilha *p, Peca *removida) {
    if (pilhaVazia(p)) {
        printf("Pilha de reserva vazia. Nada para usar.\n");
        return;
    }
    *removida = p->itens[p->topo];
    p->topo--;
}

void mostrarPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
    }
    if (p->topo == -1) printf("(vazia)");
    printf("\n");
}

// -----------------------------
// EXIBIÇÃO DO ESTADO
// -----------------------------
void mostrarEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n");
    mostrarFila(f);
    mostrarPilha(p);
    printf("\n");
}

// -----------------------------
// PROGRAMA PRINCIPAL
// -----------------------------
int main() {
    Fila f;
    Pilha reserva;
    inicializarFila(&f);
    inicializarPilha(&reserva);
    srand((unsigned)time(NULL));

    int contadorID = 0;
    int opcao;

    // Inicializa a fila com MAX peças
    for (int i = 0; i < MAX; i++) {
        inserir(&f, gerarPeca(contadorID++));
    }

    do {
        mostrarEstado(&f, &reserva);

        printf("Opções de Ação:\n");
        printf("1 - Jogar peça (dequeue) \n");
        printf("2 - Reservar peça (da fila -> topo da pilha)\n");
        printf("3 - Usar peça reservada (pop da pilha)\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        if (scanf("%d", &opcao) != 1) {
            // Entrada inválida: limpa e continua
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            opcao = -1;
        }

        if (opcao == 1) {
            // Jogar peça: remove da fila
            if (!filaVazia(&f)) {
                Peca jogada;
                remover(&f, &jogada);
                printf("Jogou a peça: [%c %d]\n", jogada.tipo, jogada.id);
            } else {
                printf("Não há peça para jogar (fila vazia).\n");
            }

            // Repor para manter a fila cheia (se houver espaço)
            if (f.total < MAX) {
                inserir(&f, gerarPeca(contadorID++));
            }

        } else if (opcao == 2) {
            // Reservar peça: mover da fila para pilha (se houver espaço na pilha)
            if (pilhaCheia(&reserva)) {
                printf("Reserva cheia. Libere espaço antes de reservar outra peça.\n");
            } else if (filaVazia(&f)) {
                printf("Não há peça na fila para reservar.\n");
            } else {
                Peca frente;
                remover(&f, &frente);
                push(&reserva, frente);
                printf("Peça reservada: [%c %d]\n", frente.tipo, frente.id);

                // Repor para manter a fila cheia
                if (f.total < MAX) {
                    inserir(&f, gerarPeca(contadorID++));
                }
            }

        } else if (opcao == 3) {
            // Usar peça reservada: pop da pilha
            if (pilhaVazia(&reserva)) {
                printf("Não há peça reservada para usar.\n");
            } else {
                Peca usada;
                pop(&reserva, &usada);
                printf("Usou peça reservada: [%c %d]\n", usada.tipo, usada.id);
            }

            // Aqui a fila não perdeu elemento; só repõe se houver espaço (não deve haver).
            if (f.total < MAX) {
                inserir(&f, gerarPeca(contadorID++));
            }

        } else if (opcao == 0) {
            // sair
        } else {
            printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    printf("Encerrando o jogo...\n");
    return 0;
}
