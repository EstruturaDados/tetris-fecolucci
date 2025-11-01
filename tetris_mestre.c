//Desafio final do 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5
#define RES_MAX 3

typedef struct {
    char tipo;
    int id;
} Peca;

typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

typedef struct {
    Peca itens[RES_MAX];
    int topo;
} Pilha;

// -------- Fila --------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX;
}

int filaVazia(Fila *f) {
    return f->total == 0;
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

// -------- Pilha --------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == RES_MAX - 1;
}

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

// -------- Trocas --------
void trocarUma(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("Não há peça na fila para trocar.\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("Não há peça na pilha para trocar.\n");
        return;
    }
    int idxFrente = f->inicio;
    int idxTopo = p->topo;

    Peca tmp = f->itens[idxFrente];
    f->itens[idxFrente] = p->itens[idxTopo];
    p->itens[idxTopo] = tmp;

    printf("Troca realizada: frente da fila <-> topo da pilha.\n");
}

void trocarTres(Fila *f, Pilha *p) {
    if (f->total < 3) {
        printf("A fila não possui 3 peças para a troca múltipla.\n");
        return;
    }
    if (p->topo < 2) {
        printf("A pilha não possui 3 peças para a troca múltipla.\n");
        return;
    }

    int q0 = f->inicio;
    int q1 = (f->inicio + 1) % MAX;
    int q2 = (f->inicio + 2) % MAX;

    int s0 = p->topo;
    int s1 = p->topo - 1;
    int s2 = p->topo - 2;

    Peca fila0 = f->itens[q0];
    Peca fila1 = f->itens[q1];
    Peca fila2 = f->itens[q2];

    Peca pilha0 = p->itens[s0];
    Peca pilha1 = p->itens[s1];
    Peca pilha2 = p->itens[s2];

    f->itens[q0] = pilha0;
    f->itens[q1] = pilha1;
    f->itens[q2] = pilha2;

    p->itens[s0] = fila2;
    p->itens[s1] = fila1;
    p->itens[s2] = fila0;

    printf("Troca múltipla realizada entre as 3 primeiras da fila e as 3 da pilha.\n");
}

// -------- Utilitários --------
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

void mostrarEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n");
    mostrarFila(f);
    mostrarPilha(p);
    printf("\n");
}

// -------- Main --------
int main() {
    Fila f;
    Pilha reserva;
    inicializarFila(&f);
    inicializarPilha(&reserva);
    srand((unsigned)time(NULL));

    int contadorID = 0;
    int opcao;

    for (int i = 0; i < MAX; i++) {
        inserir(&f, gerarPeca(contadorID++));
    }

    do {
        mostrarEstado(&f, &reserva);

        printf("Opções de Ação:\n");
        printf("1 - Jogar peça (dequeue da fila)\n");
        printf("2 - Reservar peça (da fila -> topo da pilha)\n");
        printf("3 - Usar peça reservada (pop da pilha)\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Opção: ");

        if (scanf("%d", &opcao) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            opcao = -1;
        }

        if (opcao == 1) {
            if (!filaVazia(&f)) {
                Peca jogada;
                remover(&f, &jogada);
                printf("Jogou a peça: [%c %d]\n", jogada.tipo, jogada.id);
            } else {
                printf("Não há peça para jogar (fila vazia).\n");
            }
            if (f.total < MAX) {
                inserir(&f, gerarPeca(contadorID++));
            }

        } else if (opcao == 2) {
            if (pilhaCheia(&reserva)) {
                printf("Reserva cheia. Libere espaço antes de reservar outra peça.\n");
            } else if (filaVazia(&f)) {
                printf("Não há peça na fila para reservar.\n");
            } else {
                Peca frente;
                remover(&f, &frente);
                push(&reserva, frente);
                printf("Peça reservada: [%c %d]\n", frente.tipo, frente.id);
                if (f.total < MAX) {
                    inserir(&f, gerarPeca(contadorID++));
                }
            }

        } else if (opcao == 3) {
            if (pilhaVazia(&reserva)) {
                printf("Não há peça reservada para usar.\n");
            } else {
                Peca usada;
                pop(&reserva, &usada);
                printf("Usou peça reservada: [%c %d]\n", usada.tipo, usada.id);
            }

        } else if (opcao == 4) {
            trocarUma(&f, &reserva);

        } else if (opcao == 5) {
            trocarTres(&f, &reserva);

        } else if (opcao == 0) {
            /* sair */
        } else {
            printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    printf("Encerrando o jogo...\n");
    return 0;
}
