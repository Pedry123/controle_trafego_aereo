#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n = 0;

typedef struct {
    int id;
    int tempo;
    int combustivel;
    struct aviao *prox;
} aviao;

typedef struct {
     aviao *inicio;
     aviao *fim;
} fila;

fila *pista1;
fila *pista2;
fila *pista3;

void criaFilaVazia(fila *f) {
    f->inicio = malloc(sizeof(aviao));
    f->fim = f->inicio;
    f->inicio->prox = NULL;
}

int filaVazia(fila *f) {
    return (f->inicio == f->fim);
}

void enfileira(fila *f, int id, int tempo, int combustivel) {
    f->fim->prox = malloc(sizeof(aviao));
    f->fim = f->fim->prox;
    f->fim->id = id;
    f->fim->tempo = tempo;
    f->fim->combustivel = combustivel;
    f->fim->prox = NULL;
}

aviao *desenfileira(fila *f) {
    aviao *plane;
    aviao *aux;
    if (filaVazia(f)) {
        printf("Fila vazia\n");
        return NULL;
    }
    aux = f->inicio;
    f->inicio = f->inicio->prox;
    plane = f->inicio;
    aux->prox = NULL;
    free(aux);
    return plane;
}

void apagaFila(fila *f) {
    while (!filaVazia(f)) {
        desenfileira(f);
    }
    free(f->inicio);
    f->inicio = NULL;
    f->fim = NULL;
}

void converteTempo(int n) {
    int hora_inteira;
    float horas, minutos;
    n *= 15;
    hora_inteira = n / 60;
    horas = n / 60.0;
    minutos = (horas - hora_inteira) * 60;
    if (minutos == 0) {
        printf("%.f:00\n", horas);
    } else {
        printf("%.f:%.f\n", horas, minutos);
    }
}

void solicitaPouso(fila *pista, int id, int n) {
    enfileira(pista, id, n + 1, 1 + rand() % 8);
    printf("Solicitacao de pouso do voo %d\n", id);
}

void pousar(fila *pista, int tempoAtual) {
    if (!filaVazia(pista)) {
        aviao *x = pista->inicio->prox;
        if (x->tempo <= tempoAtual) {
            pista->inicio->prox = x->prox;
            if (x == pista->fim) {
                pista->fim = pista->inicio;
            }
            printf("Tempo %02d: Aviao %d [%d] pousando na pista %d\n", tempoAtual, x->id, x->tempo, pista == pista1 ? 1 : 2);
            free(x);
        }
    }
}

void solicitaDecolagem(fila *pista, int id, int n) {
    enfileira(pista, id, n + 1, 1 + rand() % 8);
    printf("Solicitacao de decolagem do voo %d\n", id);
}

void decolar(fila *pista, int tempoAtual) {
    if (!filaVazia(pista)) {
        aviao *x = pista->inicio->prox;
        if (x->tempo <= tempoAtual) {
            pista->inicio->prox = x->prox;
            if (x == pista->fim) {
                pista->fim = pista->inicio;
            }
            printf("Tempo %02d: Aviao %d [%d] decolando na pista 3\n", tempoAtual, x->id, x->tempo);
            free(x);
        }
    }
}

int main() {
    srand(time(NULL));
    pista1 = malloc(sizeof(fila));
    pista2 = malloc(sizeof(fila));
    pista3 = malloc(sizeof(fila));
    criaFilaVazia(pista1);
    criaFilaVazia(pista2);
    criaFilaVazia(pista3);

    int tempoAtual = 0;
    for (int i = 0; i < 15; i++) {
        converteTempo(tempoAtual);

        // Gerar solicitações de pouso e decolagem aleatoriamente
        for (int i = 0; i < rand() % 4; i++) {
            if (rand() % 2 == 0) {
                int id = 2 * (rand() % 250);
                solicitaPouso(pista1, id, tempoAtual);
            } else {
                int id = 2 * (rand() % 250);
                solicitaPouso(pista2, id, tempoAtual);
            }

            if (rand() % 2 == 0) {
                int id = 2 * (rand() % 250) + 1;
                solicitaDecolagem(pista3, id, tempoAtual);
            }
        }

        pousar(pista1, tempoAtual);
        pousar(pista2, tempoAtual);
        decolar(pista3, tempoAtual);

        tempoAtual++;
    }

    apagaFila(pista1);
    free(pista1);
    apagaFila(pista2);
    free(pista2);
    apagaFila(pista3);
    free(pista3);

    return 0;
}
