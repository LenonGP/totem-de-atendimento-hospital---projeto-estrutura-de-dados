#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "arvore.h"

static NoArv backing[DEMAIS_PACIENTES]; // memoria 
static NoArv *disponivel[DEMAIS_PACIENTES]; //gerente da memoria 
static int n_disp = DEMAIS_PACIENTES; // contador para ver nos disponiveis

void pool_inicia(void) {
    for (int i = 0; i < DEMAIS_PACIENTES; i++) { //aponta para os nos 
        disponivel[i] = backing + i;
    }
}

NoArv * pool_obtem(Pri pri) {
    assert(n_disp > 0); // para se estiver NULL
    NoArv *ele = disponivel[--n_disp]; // decrementa o contador
    assert(ele);
    static NoArv padrao = {0};
    *ele = padrao;
    ele->pri = pri; // define a prioridade do nó solicitado
    return ele;
}

void pool_devolve(NoArv *ele) { //devolve nos que n estao sendo usados
    assert(n_disp < DEMAIS_PACIENTES); 
    assert(ele);
    disponivel[n_disp++] = ele; // devolve o no la lista de disponiveis
}

NoArv ** arvore_busca(NoArv **raiz, Pri prioridade) {
    if (NULL == *raiz || prioridade == (*raiz)->pri) { // duplo * para função inserir modifique diretamente a estrutura da arvore
        return raiz;
    } else if (prioridade < (*raiz)->pri) {
        return arvore_busca(&(*raiz)->esq, prioridade);
    } else {
        assert(prioridade > (*raiz)->pri);
        return arvore_busca(&(*raiz)->dir, prioridade);
    }
}

void imprimir_arvore(int nivel, NoArv *no, bool tudo) {
    extern bool is_utf8;

    if (0 == nivel) {
        puts("");
        if (NULL == no) {
            puts(is_utf8 ? "Ninguém esperando" : "Ninguem esperando");
            return;
        }
    }

    int cont = 0;
    for (FichaNo *atual = no->prim; atual; atual = atual->prox) {
        cont++;
    }
    assert(cont > 0);

    printf(
        "%*sRI %d: %s. %d na fila\n",
        nivel+1, "P", no->pri, PriNomes[no->pri-1], cont
    );

    if (tudo) {
        for (FichaNo *atual = no->prim; atual; atual = atual->prox) {
            printf("%*s%d - %s, %d. %s\n", nivel + 1, " ",
                    atual->ela.id, atual->ela.nome, atual->ela.idade,
                    EspecialidadeNomes[atual->ela.especialidade-1]);
        }
    }

    if (no->esq) {
        imprimir_arvore(nivel+1, no->esq, tudo);
    }
    if (no->dir) {
        imprimir_arvore(nivel+1, no->dir, tudo);
    }
}
