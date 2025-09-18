#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ficha.h"
#include "tad_configs.h"

#define CONFIGS_FILE "configuracoes.dat"

static FILE *configs_abrir(void) {
    FILE *arquivo;
    if(!(arquivo = fopen(CONFIGS_FILE, "rb+"))) { // arquivo já existe (le e escreve)
        arquivo = fopen(CONFIGS_FILE, "wb+"); // cria arquivo novo (escreve)
        Configs tad = {.status = AGUARDAR};
        if (arquivo) {
          fwrite(&tad, sizeof(Configs), 1, arquivo);
        }
    }
    return arquivo; //retorna o ponteiro para o aquivo
}

static void configs_fechar(FILE *arquivo) {
    fclose(arquivo);
}

static void configs_salvar(Configs *tad) {
    FILE *arquivo = configs_abrir();
    if (tad && arquivo) { //verificação de ponteiro valido 
      fwrite(tad, sizeof(Configs), 1, arquivo);
    }
    configs_fechar(arquivo);
}

extern bool is_utf8;

static void reclama(void) //reclama caso ocorrer erro de comunicação na leitura
{
    puts(is_utf8 ? "\nErro de comunicação" : "\nErro de comunicacao");
}

bool configs_ler(Configs *tad) {
  FILE *arquivo = configs_abrir();
  if (tad && arquivo) {
    fread(tad, sizeof(Configs), 1, arquivo);
  }
  configs_fechar(arquivo);

  #define RECLAMA() reclama(); return false //macro para facilitar identificação de erro 

  //inicio das validações
  if (tad->status < AGUARDAR) {
      RECLAMA();
  }
  if (tad->status > RELATORIO_ESPECIALIDADES) {
      RECLAMA();
  }

  if (tad->status >= AGUARDAR && tad->status <= TERMINAR) { //status deve estar dentro do intervalo 
      return true;
  }
  if (IMPRIMIR_ALTERACAO == tad->status
          || IMPRIMIR_ALTERACAO_NAO == tad->status) {
      return true;
  }

  int id = tad->id;
  static int id_ultimo = -1;

  if (id <= 0) { //id n pode ser 0 nem negativo
      RECLAMA();
  }
  else if (id_ultimo == id) { //id n pode ser repetido
      return false;
  }
  else if (id > id_ultimo + 20) { // não deve dar tanto erro
      RECLAMA();
  }
  else if (id_ultimo != -1 && id > id_ultimo + 1) { //caso pule algum id reclama 
      printf(is_utf8 ? "\nOcorreram %d erros de comunicação\n"
                     : "\nOcorreram %d erros de comunicacao\n", id - id_ultimo - 1);
  }
  
  //atualiza o ultimo id valido 
  id_ultimo = id;

  if (FICHA == tad->status || FICHA_COMPLETA == tad->status) { //garante que a prioridade exista (numero entre 1 e 6)
      if (tad->prioridade < GESTANTE) {
          RECLAMA();
      }
      if (tad->prioridade > DEMAIS_PACIENTES) {
          RECLAMA();
      }
  }

  if (FICHA_COMPLETA == tad->status)  //garante que idade e especialidade sejam existentes 
  {
      if (tad->ficha.idade < 0 || tad->ficha.idade > 200) {
          RECLAMA();
      }
      if (tad->ficha.especialidade < OFTALMOLOGISTA) {
          RECLAMA();
      }
      if (tad->ficha.especialidade > INJECAO_NA_TESTA) {
          RECLAMA();
      }
      tad->ficha.nome[99] = '\0'; 
      if (!tad->ficha.nome[0])
      {
          tad->ficha.nome[0] = 'D'; tad->ficha.nome[1] = 'E'; // caso nome esteja vazio escreve dee
          tad->ficha.nome[2] = 'E'; tad->ficha.nome[3] = '\0';// mas n funcionou ;-; 
      }
  }

  return true; // se validou tudo retorna true
}
void configs_mostrar(Configs *tad) {
  if (tad) {
      // exibe configurações (carrega do arquivo existente se existir)
      printf(is_utf8 ? "\nConfigurações: " : "\nConfiguracoes: ");
      char *status; //ponteiro da string
      switch (tad->status)
      {
          case AGUARDAR:       status = "Aguardar"; break;
          case SIMULAR:        status = "Simular";  break;
          case TERMINAR:       status = "Terminar"; break;
          case FICHA:          status = "Inserir ficha simples";  break;
          case FICHA_COMPLETA: status = "Inserir ficha completa"; break;
          case IMPRIMIR_ALTERACAO:
              status = is_utf8 ? "Mostrar árvore sempre que mudar"
                               : "Mostrar arvore sempre que mudar";
          break;
          case IMPRIMIR_ALTERACAO_NAO:
              status = is_utf8 ? "Nao mostrar árvore quando mudar"
                               : "Nao mostrar arvore quando mudar";
          break;
          case IMPRIMIR_ARVORE:
              status = is_utf8 ? "Imprimir árvore" : "Imprimir arvore";
          break;
          case RELATORIO_ESPECIALIDADES:
              status = "Gerar relatorio de especialidades";
              break;
          default:
              status = is_utf8 ? "Inválido" : "Invalido";
          break;
      }
      printf(" - Status: %s\n", status);
  }
}

static int id = 1;

void configs_atualizar(Configs *tad, statusProcessamento status) {
  if (tad) {
      tad->status = status;
      if (IMPRIMIR_ARVORE == status || status == RELATORIO_ESPECIALIDADES) {
          tad->id = id++;
      }
      configs_salvar(tad);
  }
}

void configs_ficha(Configs *tad, int prioridade, bool eh_completo)
{
    assert(tad);

    tad->status = eh_completo ? FICHA_COMPLETA : FICHA;
    tad->prioridade = prioridade;
    tad->id = id++;

    configs_salvar(tad);
}