#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ficha.h"

char *PriNomes[DEMAIS_PACIENTES] = {
  "Gestante",
  "Idoso",
  "Pessoa com necessidades especiais (PNE)",
  "Crianca de colo",
  "Doenca cronica",
  "Demais pacientes"
};

char *EspecialidadeNomes[INJECAO_NA_TESTA] = {
 "Oftalmologista",
 "Cobaia de experimento mortal",
 "Doacao de rim",
 "Transplante de coracao",
 "Doacao de sangue",
 "Fisioterapia",
 "Degustacao de tarja preta",
 "Raio X",
 "Fratura",
 "Bezetacil",
 "Analise de calvicie",
 "Envenenamento radioativo",
 "Injecao na testa"
};

Ficha ficha_simples(int id, Pri prioridade) {
    Ficha ficha;
    ficha.id = id;

    if (IDOSO == prioridade) {
        ficha.idade = (rand() % 40) + 60;
    }
    else {
        ficha.idade = rand() % 100;
    }
    ficha.especialidade = (rand() % INJECAO_NA_TESTA) + 1;

    static char *nomes[] = {
        "Fernando", "Iracema", "Carlos", "Paulo", "Maria", "Antonio",
        "Arnaldo", "Luiz", "Bernardo", "Monica", "Roberto", "Edvaldo",
        "Mario", "Walter"
    };
    static char *sobrenomes[] = {
        " Baruci", " Oridez", " Cezare", " Albuquerque", " Tirone",
        " Gonzaga", " Belluzzo", " Contursi", " Facchina", " Nunes",
        " Della", " Nobre", " Frizzo", " Frasson", " Gianinni",
        " Munhoz", " Vander"
    };

    enum {
        NOME_QNT = sizeof(nomes) / sizeof(*nomes),
        SOBRENOME_QNT = sizeof(sobrenomes) / sizeof(*sobrenomes)
    };
    char *nome_escolhido; //ponteiro para guardar o nome selecionado

    if (prioridade == GESTANTE) {
        do {
            nome_escolhido = nomes[rand() % NOME_QNT];
        } while (nome_escolhido[strlen(nome_escolhido) - 1] != 'a'); //loop de verificação da ultima letra
    } else {
        nome_escolhido = nomes[rand() % NOME_QNT];
    }

    strcpy(ficha.nome, nome_escolhido);

    strcat(ficha.nome, sobrenomes[rand() % SOBRENOME_QNT]);
    if (rand() % 2) {
        strcat(ficha.nome, sobrenomes[rand() % SOBRENOME_QNT]);
    }

    return ficha;
}
