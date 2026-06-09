#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_DADOS "contas.bin"

typedef struct {
    int numeroConta;
    char nome[50];
    double saldo;
    int ativo; 
} Cliente;

void cadastrarCliente(FILE *arq);
void consultarCliente(FILE *arq);
void atualizarSaldo(FILE *arq);
void encerrarConta(FILE *arq);
void listarClientes(FILE *arq);
void restaurarERetomar(FILE *arq);

int main() {
    FILE *arq = fopen(ARQUIVO_DADOS, "rb+");
    if (arq == NULL) {
        arq = fopen(ARQUIVO_DADOS, "wb+");
        if (arq == NULL) {
            printf("Erro ao abrir ou criar o arquivo de dados.\n");
            return 1;
        }
    }

    int opcao = 0;
    do {
        printf("\n=== SISTEMA DE MANUTENCAO DE CONTAS ===\n");
        printf("1. Cadastrar cliente em posicao especifica\n");
        printf("2. Consultar cliente por numero da conta\n");
        printf("3. Atualizar saldo do cliente\n");
        printf("4. Encerrar conta (remover cliente)\n");
        printf("5. Listar todos os clientes\n");
        printf("6. Restaurar leitura do inicio (rewind) e listar\n");
        printf("7. Encerrar sistema\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); /

        switch(opcao) {
            case 1: cadastrarCliente(arq); break;
            case 2: consultarCliente(arq); break;
            case 3: atualizarSaldo(arq); break;
            case 4: encerrarConta(arq); break;
            case 5: listarClientes(arq); break;
            case 6: restaurarERetomar(arq); break;
            case 7: printf("Encerrando o sistema...\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while(opcao != 7);

    fclose(arq);
    return 0;
}


void cadastrarCliente(FILE *arq) {
    int posicao;
    Cliente c;

    printf("Digite a posicao do registro onde deseja cadastrar (0, 1, 2...): ");
    scanf("%d", &posicao);
    getchar();

    printf("Numero da conta: ");
    scanf("%d", &c.numeroConta);
    getchar();
    printf("Nome do cliente: ");
    fgets(c.nome, sizeof(c.nome), stdin);
    c.nome[strcspn(c.nome, "\n")] = '\0'; 
    printf("Saldo inicial: ");
    scanf("%lf", &c.saldo);
    c.ativo = 1; 

    
    fseek(arq, posicao * sizeof(Cliente), SEEK_SET);
    
    if (fwrite(&c, sizeof(Cliente), 1, arq) == 1) {
        printf("Cliente cadastrado com sucesso na posicao %d!\n", posicao);
    } else {
        printf("Erro ao gravar os dados do cliente.\n");
    }
}


void consultarCliente(FILE *arq) {
    int contaProcurada;
    Cliente c;
    int encontrado = 0;

    printf("Digite o numero da conta para consulta: ");
    scanf("%d", &contaProcurada);

    rewind(arq); 
    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo && c.numeroConta == contaProcurada) {
            printf("\n--- Cliente Encontrado ---\n");
            printf("Conta: %d\n", c.numeroConta);
            printf("Nome: %s\n", c.nome);
            printf("Saldo: R$ %.2f\n", c.saldo);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("Cliente com a conta %d nao foi encontrado ou esta inativo.\n", contaProcurada);
    }
}


void atualizarSaldo(FILE *arq) {
    int contaProcurada;
    Cliente c;
    int encontrado = 0;

    printf("Digite o numero da conta para atualizar o saldo: ");
    scanf("%d", &contaProcurada);

    rewind(arq);
    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo && c.numeroConta == contaProcurada) {
            printf("Saldo atual: R$ %.2f\n", c.saldo);
            printf("Digite o novo saldo: ");
            scanf("%lf", &c.saldo);

            
            fseek(arq, -1 * sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arq);
            
            printf("Saldo atualizado com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("Conta nao encontrada para atualizacao.\n");
    }
}


void encerrarConta(FILE *arq) {
    int contaProcurada;
    Cliente c;
    int encontrado = 0;

    printf("Digite o numero da conta que deseja encerrar: ");
    scanf("%d", &contaProcurada);

    rewind(arq);
    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo && c.numeroConta == contaProcurada) {
            c.ativo = 0;

            fseek(arq, -1 * sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arq);

            printf("Conta %d encerrada com sucesso do sistema.\n", contaProcurada);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("Conta nao encontrada.\n");
    }
}

void listarClientes(FILE *arq) {
    Cliente c;
    int total = 0;
    long posicaoAtual = ftell(arq);

    rewind(arq); 
    printf("\n--- LISTA DE CLIENTES ATIVOS ---\n");
    printf("%-10s %-30s %-15s\n", "CONTA", "NOME", "SALDO");
    
    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo) {
            printf("%-10d %-30s R$ %-13.2f\n", c.numeroConta, c.nome, c.saldo);
            total++;
        }
    }
    if (total == 0) {
        printf("Nenhum cliente cadastrado ou ativo.\n");
    }
    
   
    fseek(arq, posicaoAtual, SEEK_SET);
}

void restaurarERetomar(FILE *arq) {
    printf("\nExecutando comando rewind()...\n");
    rewind(arq);
    listarClientes(arq);
}