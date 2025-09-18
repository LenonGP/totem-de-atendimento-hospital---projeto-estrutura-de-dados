#ifndef FICHA_H //se nao estiver definida 
#define FICHA_H //defina

#include <stdbool.h>

typedef enum { //numera as prioridades de 1 a 6
    GESTANTE = 1,
    IDOSO,
    PNE, // Pessoas com necessidades especiais
    CRIANCA_COLO,
    DOENCA_CRONICA,
    DEMAIS_PACIENTES,
} Pri;

extern char *PriNomes[DEMAIS_PACIENTES];

typedef enum { 
    OFTALMOLOGISTA = 1,
    COBAIA_DE_EXPERIMENTO_MORTAL,
    DOACAO_DE_RIM,
    TRANSPLANTE_DE_CORACAO,
    DOACAO_DE_SANGUE,
    MASSAGEM,
    DEGUSTACAO_DE_TARJA_PRETA,
    RAIO_X,
    FRATURA,
    BEZETACIL,
    ANALISE_DE_CALVICIE,
    ENVENENAMENTO_RADIOATIVO,
    INJECAO_NA_TESTA
} Especialidade;

extern char *EspecialidadeNomes[INJECAO_NA_TESTA];

typedef struct ficha {
    int id;
    int idade;
    char nome[100];
    Especialidade especialidade;
} Ficha;

Ficha ficha_simples(int id, Pri prioridade);

#endif
