#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define MAX_PECAS 1000  // Para garantir IDs únicos

// Definição da estrutura Peça
typedef struct {
    char nome;
    int id;
} Peca;

// Definição da estrutura Fila
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int frente;
    int tras;
    int tamanho;
    int contador_id;  // Para gerar IDs únicos
} Fila;

// Protótipos das funções
void inicializarFila(Fila *fila);
Peca gerarPeca(Fila *fila);
int filaVazia(Fila *fila);
int filaCheia(Fila *fila);
int enqueue(Fila *fila, Peca peca);
Peca dequeue(Fila *fila);
void exibirFila(Fila *fila);
void exibirMenu();

int main() {
    Fila fila;
    int opcao;
    
    // Inicializar random seed
    srand(time(NULL));
    
    // Inicializar a fila
    inicializarFila(&fila);
    
    printf("=== Tetris Stack - Fila de Pecas Futuras ===\n\n");
    
    do {
        exibirFila(&fila);
        exibirMenu();
        
        printf("\nDigite sua opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: // Jogar peça (dequeue)
                if (!filaVazia(&fila)) {
                    Peca pecaRemovida = dequeue(&fila);
                    printf("\n>>> Pecas jogada: [%c %d]\n", pecaRemovida.nome, pecaRemovida.id);
                } else {
                    printf("\n>>> Erro: Nao ha pecas na fila para jogar!\n");
                }
                break;
                
            case 2: // Inserir nova peça (enqueue)
                if (!filaCheia(&fila)) {
                    Peca novaPeca = gerarPeca(&fila);
                    if (enqueue(&fila, novaPeca)) {
                        printf("\n>>> Nova peca adicionada: [%c %d]\n", novaPeca.nome, novaPeca.id);
                    } else {
                        printf("\n>>> Erro: Nao foi possivel adicionar a peca!\n");
                    }
                } else {
                    printf("\n>>> Erro: A fila esta cheia!\n");
                }
                break;
                
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
    fila->contador_id = 0;
    
    // Preencher a fila inicial com 5 peças
    for(int i = 0; i < TAMANHO_FILA; i++) {
        Peca novaPeca = gerarPeca(fila);
        enqueue(fila, novaPeca);
    }
}

// Gera uma nova peça com tipo aleatório e ID único
Peca gerarPeca(Fila *fila) {
    Peca peca;
    
    // Tipos de peças disponíveis
    char tipos[] = {'I', 'O', 'T', 'L'};
    int numTipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Escolher tipo aleatório
    peca.nome = tipos[rand() % numTipos];
    
    // Atribuir ID único
    peca.id = fila->contador_id++;
    
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

// Exibe o estado atual da fila
void exibirFila(Fila *fila) {
    printf("=== Fila de Pecas Futuras ===\n");
    
    if (filaVazia(fila)) {
        printf("Fila vazia!\n");
        return;
    }
    
    printf("Pecas: ");
    
    // Percorrer a fila circular
    int indice = fila->frente;
    for(int i = 0; i < fila->tamanho; i++) {
        printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
        indice = (indice + 1) % TAMANHO_FILA;
    }
    printf("\n");
}

// Exibe o menu de opções
void exibirMenu() {
    printf("\n=== Opcoes ===\n");
    printf("1 - Jogar peca (dequeue)\n");
    printf("2 - Inserir nova peca (enqueue)\n");
    printf("0 - Sair\n");
}
