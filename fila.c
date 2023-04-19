#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>

#include "estruturas.h"
#include "menus.h"
#include "medicos.h"
#include "utentes.h"
#include "base_dados.h"
#include "fila.h"

void adicionarUtenteNaFilaDeEspera(Medico* medico, char* nomeUtente) {
    Utente utente;
    int codigoMedico = medico->codigo;

    // Abre o arquivo de utentes
    FILE* arquivo = fopen("utentes.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de utentes.\n");
        return;
    }

    // Procura o registro do utente com o nome fornecido
    int encontrou = 0;
    while (fscanf(arquivo, "%[^,],%d,%d\n", utente.nome, &utente.codigo, &utente.codigo_medico) == 3) {
        if (strcmp(nomeUtente, utente.nome) == 0 && utente.codigo_medico == codigoMedico) {
            encontrou = 1;
            break;
        }
    }

    // Fecha o arquivo de utentes
    fclose(arquivo);

    // Se n�o encontrou o utente, exibe uma mensagem de erro e retorna
    if (!encontrou) {
        printf("Utente nao encontrado.\n");
        return;
    }

    // Adiciona o utente � fila de espera do m�dico
    Utente* ultimo = medico->fila_espera;
    while (ultimo != NULL && ultimo->proximo != NULL) {
        ultimo = ultimo->proximo;
    }

    Utente* novoUtente = (Utente*) malloc(sizeof(Utente));
    if (novoUtente == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    *novoUtente = utente;
    novoUtente->proximo = NULL;

    if (ultimo == NULL) {
        medico->fila_espera = novoUtente;
    } else {
        ultimo->proximo = novoUtente;
    }

    printf("Utente %s adicionado na fila de espera do medico %s.\n", utente.nome, medico->nome);
}


// Fun��o para remover o primeiro utente da fila de espera de um m�dico
void removerUtente(Medico* medico) {
    // Verifica se a fila est� vazia
    if (medico->fila_espera == NULL) {
        printf("A fila de espera est� vazia.\n");
        return;
    }

    // Armazena o primeiro n� em uma vari�vel tempor�ria
    Utente* primeiroUtente = medico->fila_espera;

    // Define o segundo n� como o primeiro da fila
    medico->fila_espera = medico->fila_espera->proximo;

    // Libera a mem�ria alocada para o n� removido
    free(primeiroUtente);
}

// Fun��o para imprimir a fila de espera de um m�dico
void imprimirFilaEspera(Medico* medico) {
    printf("Fila de espera do m�dico %s (C�digo: %d):\n", medico->nome, medico->codigo);

    // Percorre a fila de espera e imprime os dados de cada n�
    Utente* utenteAtual = medico->fila_espera;
    while (utenteAtual != NULL) {
        printf("Nome: %s    C�digo: %d\n", utenteAtual->nome, utenteAtual->codigo);
	    utenteAtual = utenteAtual->proximo;
	}
}

void Menu_Fila(Medico* medico){
	system("CLS");
	ASCII_Print(2);
	char string[255];
	Utente utente;
	printf("Nome do utente para entrar na fila de espera: ");
	fgets(string, sizeof(string), stdin);
	string[strcspn(string, "\n")] = '\0'; // remove o caractere '\n' do final da string
	if (VerificarNomeUtente(string) == 1) {
		char linha[100];
		int i = 0;
		FILE* arquivo = fopen("utentes.txt", "r");
		FILE* arquivo2 = fopen("medicos.txt", "r");
		while (fscanf(arquivo, "%[^,],%d,%d\n", utente.nome, &utente.codigo, &utente.codigo_medico) == 3) {
			if (strcmp(string, utente.nome) == 0) {
				while (fscanf(arquivo2, "%[^,],%d\n", linha, &i) == 2) {
					if (i == utente.codigo_medico) {
						medico->codigo = i;
						strcpy(medico->nome, linha);
						adicionarUtenteNaFilaDeEspera(medico, string);
						fclose(arquivo);
						fclose(arquivo2);
					}
				}
			}
		}
		fclose(arquivo);
		fclose(arquivo2);
		if (medico != NULL) {
			imprimirFilaEspera(medico);
		}
		printf("\n\nPressione ENTER para continuar...");
    	getchar(); // aguarda a tecla ENTER ser pressionada
		Menu_Atendimento();
	} else {
		printf("Utente nao encontrado!");
		printf("\n\nPressione ENTER para continuar...");
    	getchar(); // aguarda a tecla ENTER ser pressionada
		Menu_Atendimento();
	}
}

void Menu_Lista_Medico(Medico* medico_ptr) {
    system("CLS");
    int num, i, opcao = 0, tecla;
    char** nomes_medicos = Nomes_Medicos("medicos.txt", &num);

    if (num == 0) {
        printf("N�o h� m�dicos registrados! Registre um m�dico primeiro.");
        printf("\n\nPressione ENTER para continuar...");
    	getchar(); // aguarda a tecla ENTER ser pressionada
        Menu_Utentes();
    }

    while (1) {
        system("CLS");
        printf("Lista de M�dicos:\n\n");
        for (i = 0; i < num; i++) {
            printf("%s %s\n", opcao == i ? ">" : " ", nomes_medicos[i]);
        }

        tecla = getch();

        if (tecla == 224) {
            tecla = getch(); 

            switch (tecla) {
                case 72: // seta para cima
                    opcao = (opcao + num - 1) % num;
                    break;
                case 80: // seta para baixo
                    opcao = (opcao + 1) % num;
                    break;
            }
        } else if (tecla == 13) { // enter
            strcpy(medico_ptr->nome, nomes_medicos[opcao]);
            medico_ptr->codigo = Codigo_Medico(nomes_medicos[opcao]);
            imprimirFilaEspera(medico_ptr);
            break;
        }
    }

    for (i = 0; i < num; i++) {
        free(nomes_medicos[i]);
    }
    free(nomes_medicos);

    printf("\n\nPressione ENTER para continuar...");
    getchar(); // aguarda a tecla ENTER ser pressionada
    Menu_Utentes();
}


