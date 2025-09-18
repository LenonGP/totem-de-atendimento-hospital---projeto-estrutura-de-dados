#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "tad_configs.h"

bool is_utf8; //questão de estetica no terminal 

static int menu(void) {
    printf("1. Aguardar\n2. Simular\n3. Terminar\n4. Ler\n"
           "5. Ficha simples\n6. Ficha personalizada\n"
           "7. Sempre imprimir arvore quando ela mudar\n"
           "8. Parar de imprimir arvore quando ela muda\n"
           "9. Imprimir lista de espera uma vez\n"
           "10. Relatorio de Especialidades\n"
           "0. Sair\n");
    printf(is_utf8 ? "Informe a opção desejada: " : "Informe a opcao desejada: ");
    int aux;
    if (scanf("%d", &aux) != 1) {
        scanf("%*[^\n]"); //limpar buffer de entrada
    }
    return aux;
}

static int ler_int(char const *mensagem, int min, int max)
{
    printf("%s: ", mensagem);
    int i;
    while (scanf("%d", &i) != 1 || (i < min || i > max)) //aceita apenas valores entre 1 e 6 (prioridades)
    {
        scanf("%*[^\n]");
        printf("Erro. %s novamente: ", mensagem);
    }
    return i;
}

static void print_prioridades(void) {
    puts("Prioridades");
    for (int i = 1; i <= DEMAIS_PACIENTES; i++) {
        printf("%d. %s\n", i, PriNomes[i-1]); //imprime o numero da prioridade e seu nome correspondente 
    }
}

int main(void) {
  is_utf8 = setlocale(LC_ALL, "pt_BR.UTF-8"); //localiza palavras em portugues (estetica apenas)
  Configs tad_configs_back;
  Configs *tad_configs = &tad_configs_back; //passando referencia para modificação de dados 

  int op;
  // criar TAD e abrir arquivo
  do {
      op = menu();
      switch(op) {
          case 1: {
              configs_atualizar(tad_configs, AGUARDAR);
              break;
          }
          case 2: {
              configs_atualizar(tad_configs, SIMULAR);
              break;
          }
          case 3: {
              configs_atualizar(tad_configs, TERMINAR);
              sleep(2);
              configs_atualizar(tad_configs, AGUARDAR);
              break;
          }
          case 4: {
              //carregar configurações
              configs_ler(tad_configs);
              //exibir configurações (carrega do arquivo existente se existir)
              configs_mostrar(tad_configs);
              break;
          }
          case 5: {
              //gera um nome, idade e local de atendimento aleatório
              print_prioridades();
              int pri = ler_int("Digite a prioridade", GESTANTE, DEMAIS_PACIENTES);
              configs_ficha(tad_configs, pri, false);
              break;
          }
          case 6: {
              //pede para digitar a prioridade, nome, idade e escolher um local para ser atendido
              print_prioridades();
              int pri = ler_int("Digite a prioridade", GESTANTE, DEMAIS_PACIENTES);
              
              printf("Digite o nome: ");
              while (scanf(" %99[^\n]", tad_configs_back.ficha.nome) != 1) { //while para permitir espaço entre nome e sobrenome
                  printf("Erro. Digite o nome novamente: ");
              }
              scanf("%*[^\n]");

              int idade = tad_configs_back.ficha.idade
                      = ler_int("Digite a idade", 0, 200);

              if (IDOSO == pri && idade < 60) {
                  printf(is_utf8 ? 
                    "Que falta de caráter! Tu não é idoso ainda!!! Atendimento cancelado\n"
                  : "Que falta de carater! Tu nao eh idoso ainda!!! Atendimento cancelado\n");
                  break;
              }

              puts("Especialidades:");
              for (int i = OFTALMOLOGISTA; i <= INJECAO_NA_TESTA; i++) {
                  printf("%2d. %s\n", i, EspecialidadeNomes[i-1]);
              }
              tad_configs_back.ficha.especialidade
                      = ler_int("Digite a especialidade", OFTALMOLOGISTA, INJECAO_NA_TESTA);

              configs_ficha(tad_configs, pri, true);
              break;
          }
          case 7: // Fall through.
          case 8: // Fall through.
          case 9: {
              configs_atualizar(tad_configs, op - 2); //mostra lista de pacientes 
              break;
          }
          case 10: {
              configs_atualizar(tad_configs, RELATORIO_ESPECIALIDADES);
              break;
          }
          case 0: {
              printf(is_utf8 ? "Até a próxima!\n" : "Ate a proxima!\n");
              break;
          }
          default: {
              printf(is_utf8 ? "Opção inválida!\n" : "Opcao invalida!\n");
          }
      }
  } while (op != 0);
  return 0;
}