#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estruturas para o jogo

// Estrutura de Sala para o Nível Novato
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura de Pista para o Nível Aventureiro
typedef struct Pista {
    char texto[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// Estrutura de Suspeito para o Nível Mestre
typedef struct Suspeito {
    char nome[50];
    struct Suspeito *next;
    char pistas[10][100]; // Limite de 10 pistas por suspeito
} Suspeito;

// Funções de Nível Novato

Sala* criarSala(char* nome) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

void explorarSalas(Sala* sala) {
    char escolha;
    Sala* salaAtual = sala;

    while (1) {
        printf("Você está na sala: %s\n", salaAtual->nome);
        printf("Escolha uma direção: (e) Esquerda, (d) Direita, (s) Sair: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } else if (escolha == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } else if (escolha == 's') {
            printf("Saindo da exploração...\n");
            break;
        } else {
            printf("Opção inválida ou caminho não disponível.\n");
        }
    }
}

// Funções de Nível Aventureiro

Pista* criarPista(char* texto) {
    Pista* novaPista = (Pista*) malloc(sizeof(Pista));
    strcpy(novaPista->texto, texto);
    novaPista->esquerda = NULL;
    novaPista->direita = NULL;
    return novaPista;
}

Pista* inserirBST(Pista* raiz, Pista* novaPista) {
    if (raiz == NULL) {
        return novaPista;
    }
    
    if (strcmp(novaPista->texto, raiz->texto) < 0) {
        raiz->esquerda = inserirBST(raiz->esquerda, novaPista);
    } else {
        raiz->direita = inserirBST(raiz->direita, novaPista);
    }
    return raiz;
}

void listarPistas(Pista* raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esquerda);
        printf("Pista: %s\n", raiz->texto);
        listarPistas(raiz->direita);
    }
}

// Funções de Nível Mestre

#define TAM_HASH 10

Suspeito* tabelaHash[TAM_HASH];

int hashCode(char* chave) {
    int soma = 0;
    for (int i = 0; i < strlen(chave); i++) {
        soma += chave[i];
    }
    return soma % TAM_HASH;
}

void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

void inserirHash(char* pista, char* suspeito) {
    int indice = hashCode(suspeito);

    Suspeito* novoSuspeito = (Suspeito*) malloc(sizeof(Suspeito));
    strcpy(novoSuspeito->nome, suspeito);
    strcpy(novoSuspeito->pistas[0], pista); // Apenas a primeira pista por enquanto
    novoSuspeito->next = tabelaHash[indice];
    tabelaHash[indice] = novoSuspeito;
}

void listarAssociacoes() {
    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito* atual = tabelaHash[i];
        if (atual != NULL) {
            printf("Suspeitos no índice %d:\n", i);
            while (atual != NULL) {
                printf("Nome: %s, Pistas: %s\n", atual->nome, atual->pistas[0]);
                atual = atual->next;
            }
        }
    }
}

// Função Principal
int main() {
    // 🌱 Nível Novato
    Sala* sala1 = criarSala("Hall de Entrada");
    Sala* sala2 = criarSala("Biblioteca");
    Sala* sala3 = criarSala("Cozinha");
    sala1->esquerda = sala2;
    sala1->direita = sala3;

    explorarSalas(sala1);

    // 🔍 Nível Aventureiro
    Pista* pista1 = criarPista("Pista de Sangue na Biblioteca");
    Pista* pista2 = criarPista("Pegadas de Lama na Cozinha");

    Pista* pistas = NULL;
    pistas = inserirBST(pistas, pista1);
    pistas = inserirBST(pistas, pista2);

    listarPistas(pistas);

    // 🧠 Nível Mestre
    inicializarHash();
    inserirHash("Pista de Sangue na Biblioteca", "Suspeito 1");
    inserirHash("Pegadas de Lama na Cozinha", "Suspeito 2");

    listarAssociacoes();

    return 0;
}
