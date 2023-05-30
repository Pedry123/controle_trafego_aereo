#include<stdio.h>
#include<stdlib.h>
#include<time.h>

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

fila* criaFilaVazia() {
    fila *f = malloc(sizeof(fila));
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

int filaVazia(fila *f) {
    return (f->inicio == NULL);
}

void enfileira(fila *f, int id, int tempo, int combustivel) {
    aviao *novoAviao = malloc(sizeof(aviao));
    novoAviao->id = id;
    novoAviao->tempo = tempo;
    novoAviao->combustivel = combustivel;
    novoAviao->prox = NULL;

    if (filaVazia(f)) {
        f->inicio = novoAviao;
        f->fim = novoAviao;
    } else {
        f->fim->prox = novoAviao;
        f->fim = novoAviao;
    }
}

aviao* desenfileira(fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia\n");
        return NULL;
    }

    aviao *aviaoRemovido = f->inicio;
    f->inicio = f->inicio->prox;
    aviaoRemovido->prox = NULL;

    if (f->inicio == NULL) {
        f->fim = NULL;
    }

    return aviaoRemovido;
}

void apagaFila(fila *f) {
    while (!filaVazia(f)) {
        aviao *aviaoRemovido = desenfileira(f);
        free(aviaoRemovido);
    }

    free(f);
}

void converteTempo(int n) {
    int horas = n / 4;
    int minutos = (n % 4) * 15;
    printf("%02d:%02d\n", horas, minutos);
} 

void solicitaPouso(fila *pouso, int n) {
    int id = 2 * (rand() % 250);
    int combustivel = 1 + rand() % 8;
    enfileira(pouso, id, n, combustivel);
    printf("Tempo %02d: Aviao %d solicitou pouso\n", n, id);
}

void pousar(fila *pouso, fila *decolagem, int n) {
    if (!filaVazia(pouso)) {
        aviao *aviaoPouso = desenfileira(pouso);
        aviaoPouso->combustivel--;

        if (aviaoPouso->combustivel >= 0) {
            enfileira(decolagem, aviaoPouso->id, n + 1, aviaoPouso->combustivel);
            printf("Tempo %02d: Aviao %d pousando\n", n, aviaoPouso->id);
        } else {
            free(aviaoPouso);
        }
    }
}

void decolar(fila *decolagem, int n) {
    if (!filaVazia(decolagem)) {
        aviao *aviaoDecolagem = desenfileira(decolagem);
        printf("Tempo %02d: Aviao %d decolando\n", n, aviaoDecolagem->id);
        free(aviaoDecolagem);
    }
}

int main() {
    srand(time(NULL));
    fila *pouso = criaFilaVazia();
    fila *decolagem = criaFilaVazia();

    for (int i = 0; i < 5; i++) {
        converteTempo(i);

        solicitaPouso(pouso, i);
        pousar(pouso, decolagem, i + 1);
        
        decolar(decolagem, i + 2);
    }

    apagaFila(pouso);
    apagaFila(decolagem);

    return 0;
}
