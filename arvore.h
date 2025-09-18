#ifndef ARVORE_H
#define ARVORE_H

#include "ficha.h"

typedef struct ficha_no {
    Ficha ela;
    struct ficha_no *prox;
} FichaNo;

typedef struct no_arv {
    Pri pri;
    struct no_arv *esq;
    struct no_arv *dir;
    FichaNo *prim;
    FichaNo **ultimo;
} NoArv;

void pool_inicia(void);
NoArv * pool_obtem(Pri pri);
void pool_devolve(NoArv *ele);

NoArv ** arvore_busca(NoArv **raiz, Pri prioridade);

void imprimir_arvore(int nivel, NoArv *no, bool tudo);

#endif
