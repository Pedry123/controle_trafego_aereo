#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int id;
} Aviao;

typedef struct node {
    Aviao aviao;
    struct node* prox;
} Node;

typedef struct {
    Node* inicio;
    Node* fim;
} Fila;

void inicializarFila(Fila* fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
}

int estaVazia(Fila* fila) {
    return (fila->inicio == NULL);
}

void enfileirar(Fila* fila, Aviao aviao) {
    Node* novoNode = (Node*)malloc(sizeof(Node));
    novoNode->aviao = aviao;
    novoNode->prox = NULL;

    if (estaVazia(fila)) {
        fila->inicio = novoNode;
        fila->fim = novoNode;
    } else {
        fila->fim->prox = novoNode;
        fila->fim = novoNode;
    }
}

Aviao desenfileirar(Fila* fila) {
    if (estaVazia(fila)) {
        Aviao aviao_vazio;
        aviao_vazio.id = -1;
        return aviao_vazio;
    }

    Node* node = fila->inicio;
    Aviao aviao = node->aviao;
    fila->inicio = node->prox;
    free(node);

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    return aviao;
}

int gerarIdAviao() {
    static int id = 0;
    return ++id;
}

void simular_ATC(int n, int alpha, int tempo_simulacao) {
    Fila fila_de_pouso, fila_de_decolagem, fila_solicitacoes_pouso, fila_solicitacoes_decolagem;
    inicializarFila(&fila_de_pouso);
    inicializarFila(&fila_de_decolagem);
    inicializarFila(&fila_solicitacoes_pouso);
    inicializarFila(&fila_solicitacoes_decolagem);

    int quantidade_pista1 = 0;
    int quantidade_pista2 = 0;
    int quantidade_pista3 = 0;
    int tempo_atual = 0;

    while (tempo_atual < tempo_simulacao) {
        // Gerar solicitações de pouso e decolagem aleatoriamente
        if (rand() % 2 == 0) {
            Aviao aviao;
            aviao.id = gerarIdAviao();

            if (rand() % 2 == 0) {
                enfileirar(&fila_solicitacoes_pouso, aviao);
            } else {
                enfileirar(&fila_solicitacoes_decolagem, aviao);
            }
        }

        // Processar solicitações de pouso
        if (!estaVazia(&fila_solicitacoes_pouso)) {
            Aviao aviao = desenfileirar(&fila_solicitacoes_pouso);
            printf("Tempo %d: Aviao %d solicitou pouso\n", tempo_atual, aviao.id);
            enfileirar(&fila_de_pouso, aviao);
        }

        // Processar solicitações de decolagem
        if (!estaVazia(&fila_solicitacoes_decolagem)) {
            Aviao aviao = desenfileirar(&fila_solicitacoes_decolagem);
            printf("Tempo %d: Aviao %d solicitou decolagem\n", tempo_atual, aviao.id);
            enfileirar(&fila_de_decolagem, aviao);
        }

        // Processar pousos
        if (quantidade_pista1 < alpha && !estaVazia(&fila_de_pouso)) {
            Aviao aviao = desenfileirar(&fila_de_pouso);
            quantidade_pista1++;
            printf("Tempo %d: Aviao %d pousando na pista 1\n", tempo_atual, aviao.id);
        } else if (quantidade_pista2 < alpha && !estaVazia(&fila_de_pouso)) {
            Aviao aviao = desenfileirar(&fila_de_pouso);
            quantidade_pista2++;
            printf("Tempo %d: Aviao %d pousando na pista 2\n", tempo_atual, aviao.id);
        } else if (quantidade_pista3 < alpha && !estaVazia(&fila_de_pouso)) {
            Aviao aviao = desenfileirar(&fila_de_pouso);
            quantidade_pista3++;
            printf("Tempo %d: Aviao %d pousando na pista 3\n", tempo_atual, aviao.id);
        }

        // Processar decolagens
        if (quantidade_pista1 < alpha && !estaVazia(&fila_de_decolagem)) {
            Aviao aviao = desenfileirar(&fila_de_decolagem);
            quantidade_pista1++;
            printf("Tempo %d: Aviao %d decolando da pista 1\n", tempo_atual, aviao.id);
        } else if (quantidade_pista2 < alpha && !estaVazia(&fila_de_decolagem)) {
            Aviao aviao = desenfileirar(&fila_de_decolagem);
            quantidade_pista2++;
            printf("Tempo %d: Aviao %d decolando da pista 2\n", tempo_atual, aviao.id);
        } else if (quantidade_pista3 < alpha && !estaVazia(&fila_de_decolagem)) {
            Aviao aviao = desenfileirar(&fila_de_decolagem);
            quantidade_pista3++;
            printf("Tempo %d: Aviao %d decolando da pista 3\n", tempo_atual, aviao.id);
        }

        // Atualizar o tempo das pistas
        if (quantidade_pista1 > 0) {
            quantidade_pista1--;
        }
        if (quantidade_pista2 > 0) {
            quantidade_pista2--;
        }
        if (quantidade_pista3 > 0) {
            quantidade_pista3--;
        }

        tempo_atual++;
    }
}

int main() {
    srand(time(NULL));
    int n, alpha, tempo_simulacao;
    printf("Informe o número máximo de aviões: ");
    scanf("%d", &n);
    printf("Informe o número máximo de aviões em uma pista: ");
    scanf("%d", &alpha);
    printf("Informe o tempo de simulação: ");
    scanf("%d", &tempo_simulacao);
    simular_ATC(n, alpha, tempo_simulacao);
    return 0;
}
