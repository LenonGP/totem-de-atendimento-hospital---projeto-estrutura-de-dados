#ifndef TAD_CONFIGS_H
#define TAD_CONFIGS_H

#include <stdbool.h>
#include <stdio.h>

#include "ficha.h"

typedef enum {
    AGUARDAR,
    SIMULAR,
    TERMINAR,
    FICHA,
    FICHA_COMPLETA, // se o paciente quiser preencher os dados
    IMPRIMIR_ALTERACAO, // se ativado e tiver alteração na arvore (mostra)
    IMPRIMIR_ALTERACAO_NAO, // quando ativa esse n mostra mais 
    IMPRIMIR_ARVORE,
    RELATORIO_ESPECIALIDADES 
} statusProcessamento;

typedef struct conf {
    statusProcessamento status;
    union {
        int id;
        struct {
            Ficha ficha;
            Pri prioridade;
        };
    };
} Configs;

bool configs_ler(Configs *tad); // devolve true se for válido ou se importa
void configs_mostrar(Configs *tad);
void configs_atualizar(Configs *tad, statusProcessamento status);
void configs_ficha(Configs *tad, int prioridade, bool eh_completo);

#endif