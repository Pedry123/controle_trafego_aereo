#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n = 0;

typedef struct aviao {
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

char* converteTempo(int n) {
    int minutos = n * 15;
    int horas = minutos / 60;
    minutos = minutos % 60;

    char* tempoConvertido = malloc(6 * sizeof(char));
    sprintf(tempoConvertido, "%02d:%02d", horas, minutos);
    return tempoConvertido;
}

char* condicao(aviao *x, int n) { //atrasado, adiantado ou normal
    if (x->tempo < n) {
        return "atrasado";
    } else if (x->tempo > n) {
        return "adiantado";
    } else {
        return "como previsto";
    }
}

// funcoes auxiliares

int verificarPistaLivre(int pista) {
    if (pista == 1) {
        return filaVazia(pista1);
    } else if (pista == 2) {
        return filaVazia(pista2);
    }
    return 0;
}

int trocarPista(int pista) {
    if (pista == 1 && verificarPistaLivre(2)) {
        return 2;
    } else if (pista == 2 && verificarPistaLivre(1)) {
        return 1;
    }
    return pista;
}

void solicitaPouso(fila *pista, int id, int n) {
    int pistaEscolhida = trocarPista(pista == pista1 ? 1 : 2);
    if (pistaEscolhida != 1 && pistaEscolhida != 2) {
        printf("Tempo %02d: Aviao %d (previsto para pousar %s) aguardando pista\n", n, id, converteTempo(n));
    } else {
        enfileira(pistaEscolhida == 1 ? pista1 : pista2, id, n + 1, 1 + rand() % 8);
        printf("Solicitacao de pouso do voo %d na pista %d\n", id, pistaEscolhida);
    }
}

void pousar(fila *pista, int tempoAtual) {
    if (!filaVazia(pista)) {
        aviao *x = pista->inicio->prox;
        if (x->tempo <= tempoAtual) {
            pista->inicio->prox = x->prox;
            if (x == pista->fim) {
                pista->fim = pista->inicio;
            }

            printf("Tempo %02d: Aviao %d (previsto para pousar %s) pousando na pista %d %s\n", tempoAtual, x->id, converteTempo(x->tempo), pista == pista1 ? 1 : 2, condicao(x, tempoAtual));
            free(x);
        }
    }
}

void solicitaDecolagem(fila *pista, int id, int n) {
    enfileira(pista, id, n + 3, 1 + rand() % 8);
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
            printf("Tempo %02d: Aviao %d (previsto para decolar %s) decolando na pista 3 %s\n", tempoAtual, x->id, converteTempo(x->tempo), condicao(x, tempoAtual));
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
    for (int i = 0; i < 96; i++) {
        printf("%s\n", converteTempo(tempoAtual));

        // Gerar solicitações de pouso e decolagem aleatoriamente
        for (int i = 0; i < rand() % 5; i++) {
            if (rand() % 2 == 0) {
                int id = 2 * (rand() % 250);
                solicitaPouso(pista1, id, tempoAtual);
            } else {
                int id = 2 * (rand() % 250);
                solicitaPouso(pista2, id, tempoAtual);
            }
        }
        for (int i = 0; i < rand() % 4; i++) {
            if (rand() % 2 == 0) {
                int id = 2 * (rand() % 250) + 1;
                solicitaDecolagem(pista3, id, tempoAtual);
            }
        }
        

        pousar(pista1, tempoAtual);
        pousar(pista2, tempoAtual);
        decolar(pista3, tempoAtual);

        tempoAtual++;
        printf("\n\n");
    }

    apagaFila(pista1);
    free(pista1);
    apagaFila(pista2);
    free(pista2);
    apagaFila(pista3);
    free(pista3);

    return 0;
}
