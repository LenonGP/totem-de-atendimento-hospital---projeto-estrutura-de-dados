#include <assert.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "arvore.h"
#include "tad_configs.h"

bool is_utf8;

static NoArv *raiz = NULL;

static FichaNo *filaDemaisPacientes = NULL;
static FichaNo **filaDemaisPacientesUltimo = &filaDemaisPacientes;

static bool is_print = false;

static int placar_atendimentos[INJECAO_NA_TESTA] = {0};// guarda os pacientes que ja foram atendidos 


static void gerar_relatorio_especialidades(void) {
    printf(is_utf8 ? "\n Relatório de Atendimentos Realizados\n" : "\n--- Relatorio de Atendimentos Realizados ---\n");

    bool houve_atendimentos = false;
    for (int i = 0; i < INJECAO_NA_TESTA; i++) {
        if (placar_atendimentos[i] > 0) { // caso tenha tido pacientes mostra quantos
            printf("%s %d pacientes\n", EspecialidadeNomes[i], placar_atendimentos[i]);
            houve_atendimentos = true;
        }
    }

    if (!houve_atendimentos) {
        printf(is_utf8 ? "Nenhum atendimento foi concluído no período.\n" : "Nenhum atendimento foi concluido no periodo.\n");
    }
    printf("-------------------------------------------\n");
}


static void inserir(Configs *configs) {
    FichaNo * novaFicha = malloc(sizeof (FichaNo));
    if (!novaFicha) {
        printf(is_utf8 ?
               "\nPaciente %s com id %d não pode ser atendido"
", pois acabou a memória, ou seja, esse hospital tá lotado!!!!" :
"\nPaciente %s com id %d nao pode ser atendido, pois acabou a"
" memoria, ou seja, esse hospital ta lotado!!!!!!!",
configs->ficha.nome, configs->id);
        return;
    }
    novaFicha->ela = configs->ficha;
    novaFicha->prox = NULL;

    if (configs->prioridade == DEMAIS_PACIENTES) { //decide se o paciente vai para a fifo ou arvore

        *filaDemaisPacientesUltimo = novaFicha;
        filaDemaisPacientesUltimo = &novaFicha->prox;

    } else {
        NoArv **local = arvore_busca(&raiz, configs->prioridade);
        NoArv *normal = *local;
        bool novo = false;
        if (!*local) {
            *local = pool_obtem(configs->prioridade);
            normal = *local;
            normal->prim = NULL;
            normal->ultimo = &normal->prim;
            novo = true;
        }
        *normal->ultimo = novaFicha;
        normal->ultimo = &novaFicha->prox;

        if (novo && is_print) {
            imprimir_arvore(0, raiz, false);
        }
    }
    //mesma mensagem para todos os pacientes(fila e arvore)
    printf("\nNa recepcao %s, %d (%s) ID %d, prioridade %s\n", configs->ficha.nome
           , configs->ficha.idade
           , EspecialidadeNomes[configs->ficha.especialidade-1], configs->id
           , PriNomes[configs->prioridade-1]);
}


static Ficha atender(Pri *prioridade) { //garante que a arvore seja esvaziada primeiro
    if (raiz != NULL) {
        NoArv **importante = &raiz;
        while ((*importante)->esq) {
            importante = &(*importante)->esq;
        }

        *prioridade = (*importante)->pri;
        FichaNo *paciente = (*importante)->prim;
        Ficha patient = paciente->ela;

        (*importante)->prim = paciente->prox;
        if (!paciente->prox) {
            NoArv *direita = (*importante)->dir;
            pool_devolve(*importante);
            *importante = direita;
            if (is_print) {
                imprimir_arvore(0, raiz, false);
            }
        }

        free(paciente);
        return patient;

    } else if (filaDemaisPacientes != NULL) { // se a arvore estiver vazia, vai pra lista

        *prioridade = DEMAIS_PACIENTES;

        FichaNo *paciente = filaDemaisPacientes;
        Ficha patient = paciente->ela;

        filaDemaisPacientes = paciente->prox;

        if (filaDemaisPacientes == NULL) {
            filaDemaisPacientesUltimo = &filaDemaisPacientes; // reset do ponteiro ultimo
        }

        free(paciente);
        return patient;
    }

    // tentou atender com tudo vazio
    assert(!"Proximo.... Opa nao tem ninguem na sala de espera!!");
    Ficha ficha_vazia = {0}; //retorno de segurança
    return ficha_vazia;
}

static void anunciar_proximo(void) {
    if (raiz != NULL) { //mostra proximo paciente com prioridade 
        NoArv *prox_prioridade = raiz;
        while (prox_prioridade->esq) {
            prox_prioridade = prox_prioridade->esq;
        }
        Ficha *f = &prox_prioridade->prim->ela;
        printf(is_utf8 ?
            "\nProx. Paciente PR %s ID %d" :
            "\nProx. Paciente PR %s ID %d",
            f->nome, f->id);
    } else {
        printf(is_utf8 ?
            "\nNenhum paciente(PR) na recapção." :
            "\nNenhum paciente(PR) na recepcao.");
    }

    if (filaDemaisPacientes != NULL) { //mostra proximo paciente sem prioridade 
        Ficha *f = &filaDemaisPacientes->ela;
        printf(is_utf8 ?
            "\nProx. Paciente SP %s ID %d" :
            "\nProx. Paciente SP %s ID %d",
            f->nome, f->id);
    } else {
        printf(is_utf8 ?
            "\nNenhum paciente(SP) na recepção." :
            "\nNenhum paciente(SP) na recepcao.");
    }
}



int main(void) {
    pool_inicia();
    srand(time(NULL));
    is_utf8 = setlocale(LC_ALL, "pt_BR.UTF-8");
    Configs tad_configs_back;
    Configs *tad_configs = &tad_configs_back;

    statusProcessamento estado = AGUARDAR;
    bool uma_vez = true;

    int timeout = 0;
    Ficha atendido;

    while(tad_configs->status != TERMINAR) {
        sleep(1);
        if(SIMULAR == estado) {
            if (0 == timeout) {
                if (raiz != NULL || filaDemaisPacientes != NULL) { //verifica se a arvore ou a fila tem pacientes
                    Pri prioridade;
                    atendido = atender(&prioridade);
                    
                    placar_atendimentos[atendido.especialidade - 1]++; // assim que o paciente é atendido sua especialidade é incrementada

                    timeout = (rand() % 5) + 10;
                    printf(is_utf8 ? "\nATENÇÃO ID %d (%s), vá ao consultório!"
                                   : "\nATENCAO ID %d (%s), va ao consultorio!"
                                   , atendido.id, PriNomes[prioridade-1]);
                    anunciar_proximo(); // anuncia os proximos pacientes
                    puts("");
                    uma_vez = true;
                }
                else { //só entra aqui se ambas estiverem vazias
                    if (uma_vez) {
                        printf("\nSala de espera vazia.");
                    }
                    uma_vez = false;
                    putchar('.');
                    fflush(stdout);
                }
            }
            if (timeout > 0) {
                if (uma_vez) {
                    printf("\nAtendendo %s, %d (%s).", atendido.nome, atendido.idade
                           , EspecialidadeNomes[atendido.especialidade - 1]);
                }
                else {
                    putchar('.');
                    fflush(stdout);
                }
                uma_vez = false;
                assert(timeout > 0);
                timeout--;

                if (0 == timeout) {
                    printf(is_utf8 ? "\nDiagnóstico: " : "\nDiagnostico: ");
                    switch (rand() % 5) {
                        case 0: puts("morreu"); break;
                        case 1: puts("entrou em coma"); break;
                        case 2: puts("mais um cliente satisfeito!");
                                puts(is_utf8 ? "Próximo!!!" : "Proximo!!!"); break;
                        case 3: puts("cliente insatisfeito!"); break;
                        case 4: puts("reclamou do atendimento"); break;
                    }
                    uma_vez = true;
                }
            }
        } else {
            printf(uma_vez ? "\nAguardando." : ".");
            fflush(stdout);
            uma_vez = false;
        }
        if (!configs_ler(tad_configs)) {
            continue;
        }

        switch (tad_configs->status) {
            case AGUARDAR: // Fall through.
            case SIMULAR: // Fall through.
            case TERMINAR:
                if (estado != tad_configs->status) {
                    uma_vez = true;
                }
                estado = tad_configs->status;
            break;
            case FICHA: // Fall through.
                tad_configs->ficha = ficha_simples(tad_configs->id
                                                   , tad_configs->prioridade);
            case FICHA_COMPLETA:
                inserir(tad_configs);
                uma_vez = true;
            break;
            case IMPRIMIR_ALTERACAO:
                is_print = true;
            break;
            case IMPRIMIR_ALTERACAO_NAO:
                is_print = false;
            break;
            case IMPRIMIR_ARVORE:
                imprimir_arvore(0, raiz, true);
                uma_vez = true;
            break;
            case RELATORIO_ESPECIALIDADES:
                gerar_relatorio_especialidades();
                configs_atualizar(tad_configs, estado);
                uma_vez = true;
            break;
            default: assert(!"Invalido"); break;
        }
    }

    return 0;
}