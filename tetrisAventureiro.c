#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3
#define MAX_PECAS 1000

// Definição da estrutura Peça
typedef struct {
    char nome;
    int id;
} Peca;

// Definição da estrutura Fila (circular)
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

// Definição da estrutura Pilha
typedef struct {
    Peca pecas[TAMANHO_PILHA];
    int topo;
    int tamanho;
} Pilha;

// Protótipos das funções
void inicializarFila(Fila *fila);
void inicializarPilha(Pilha *pilha);
Peca gerarPeca(int *contador_id);
int filaVazia(Fila *fila);
int filaCheia(Fila *fila);
int pilhaVazia(Pilha *pilha);
int pilhaCheia(Pilha *pilha);
int enqueue(Fila *fila, Peca peca);
Peca dequeue(Fila *fila);
int push(Pilha *pilha, Peca peca);
Peca pop(Pilha *pilha);
void exibirEstado(Fila *fila, Pilha *pilha);
void exibirMenu();

int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contador_id = 0;
    
    // Inicializar random seed
    srand(time(NULL));
    
    // Inicializar estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    // Preencher fila inicial com peças
    for(int i = 0; i < TAMANHO_FILA; i++) {
        Peca novaPeca = gerarPeca(&contador_id);
        enqueue(&fila, novaPeca);
    }
    
    printf("=== Tetris Stack - Gerenciador de Pecas ===\n\n");
    
    do {
        exibirEstado(&fila, &pilha);
        exibirMenu();
        
        printf("\nDigite sua opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: { // Jogar peça (remover da frente da fila)
                if (!filaVazia(&fila)) {
                    Peca pecaRemovida = dequeue(&fila);
                    printf("\n>>> Pecas jogada: [%c %d]\n", pecaRemovida.nome, pecaRemovida.id);
                    
                    // Adicionar nova peça ao final da fila para mantê-la cheia
                    Peca novaPeca = gerarPeca(&contador_id);
                    if (enqueue(&fila, novaPeca)) {
                        printf(">>> Nova peca adicionada a fila: [%c %d]\n", novaPeca.nome, novaPeca.id);
                    }
                } else {
                    printf("\n>>> Erro: Nao ha pecas na fila para jogar!\n");
                }
                break;
            }
                
            case 2: { // Reservar peça (mover da fila para a pilha)
                if (filaVazia(&fila)) {
                    printf("\n>>> Erro: Nao ha pecas na fila para reservar!\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("\n>>> Erro: A pilha de reserva esta cheia!\n");
                } else {
                    Peca pecaReservada = dequeue(&fila);
                    if (push(&pilha, pecaReservada)) {
                        printf("\n>>> Pecas reservada: [%c %d]\n", pecaReservada.nome, pecaReservada.id);
                        
                        // Adicionar nova peça ao final da fila para mantê-la cheia
                        Peca novaPeca = gerarPeca(&contador_id);
                        if (enqueue(&fila, novaPeca)) {
                            printf(">>> Nova peca adicionada a fila: [%c %d]\n", novaPeca.nome, novaPeca.id);
                        }
                    }
                }
                break;
            }
                
            case 3: { // Usar peça reservada (remover do topo da pilha)
                if (pilhaVazia(&pilha)) {
                    printf("\n>>> Erro: Nao ha pecas reservadas para usar!\n");
                } else {
                    Peca pecaUsada = pop(&pilha);
                    printf("\n>>> Pecas reservada usada: [%c %d]\n", pecaUsada.nome, pecaUsada.id);
                }
                break;
            }
                
            case 0: // Sair
                printf("\n>>> Obrigado por jogar Tetris Stack!\n");
                break;
                
            default:
                printf("\n>>> Opcao invalida! Tente novamente.\n");
        }
        
        printf("\n");
        
    } while(opcao != 0);
    
    return 0;
}

// Inicializa a fila com valores padrão
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

// Inicializa a pilha com valores padrão
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
    pilha->tamanho = 0;
}

// Gera uma nova peça com tipo aleatório e ID único
Peca gerarPeca(int *contador_id) {
    Peca peca;
    
    // Tipos de peças disponíveis
    char tipos[] = {'I', 'O', 'T', 'L'};
    int numTipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Escolher tipo aleatório
    peca.nome = tipos[rand() % numTipos];
    
    // Atribuir ID único
    peca.id = (*contador_id)++;
    
    return peca;
}

// Verifica se a fila está vazia
int filaVazia(Fila *fila) {
    return fila->tamanho == 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *fila) {
    return fila->tamanho == TAMANHO_FILA;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *pilha) {
    return pilha->tamanho == 0;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *pilha) {
    return pilha->tamanho == TAMANHO_PILHA;
}

// Adiciona uma peça ao final da fila (enqueue)
int enqueue(Fila *fila, Peca peca) {
    if (filaCheia(fila)) {
        return 0; // Falha
    }
    
    // Usando fila circular
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->pecas[fila->tras] = peca;
    fila->tamanho++;
    
    return 1; // Sucesso
}

// Remove uma peça do início da fila (dequeue)
Peca dequeue(Fila *fila) {
    Peca pecaVazia = {' ', -1}; // Peça vazia para retorno em caso de erro
    
    if (filaVazia(fila)) {
        return pecaVazia;
    }
    
    Peca pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    
    return pecaRemovida;
}

// Adiciona uma peça ao topo da pilha (push)
int push(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        return 0; // Falha
    }
    
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    pilha->tamanho++;
    
    return 1; // Sucesso
}

// Remove uma peça do topo da pilha (pop)
Peca pop(Pilha *pilha) {
    Peca pecaVazia = {' ', -1}; // Peça vazia para retorno em caso de erro
    
    if (pilhaVazia(pilha)) {
        return pecaVazia;
    }
    
    Peca pecaRemovida = pilha->pecas[pilha->topo];
    pilha->topo--;
    pilha->tamanho--;
    
    return pecaRemovida;
}

// Exibe o estado atual da fila e da pilha
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("=== ESTADO ATUAL ===\n");
    
    // Exibir fila de peças
    printf("Fila de pecas\t");
    if (filaVazia(fila)) {
        printf("Fila vazia!");
    } else {
        int indice = fila->frente;
        for(int i = 0; i < fila->tamanho; i++) {
            printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
            indice = (indice + 1) % TAMANHO_FILA;
        }
    }
    printf("\n");
    
    // Exibir pilha de reserva
    printf("Pilha de reserva\t");
    if (pilhaVazia(pilha)) {
        printf("Pilha vazia!");
    } else {
        printf("(Topo -> Base): ");
        for(int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
    }
    printf("\n");
}

// Exibe o menu de opções
void exibirMenu() {
    printf("\n=== OPCOES DE ACAO ===\n");
    printf("1 - Jogar peca\n");
    printf("2 - Reservar peca\n");
    printf("3 - Usar peca reservada\n");
    printf("0 - Sair\n");
}
