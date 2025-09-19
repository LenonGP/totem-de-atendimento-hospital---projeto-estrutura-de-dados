
# Sistema de espera de um hospital

Este projeto, desenvolvido para a disciplina de Estrutura de Dados, simula um sistema de gerenciamento de fila de espera de um hospital. Ele é composto por dois programas principais que se comunicam: o Totem, onde os pacientes se registram, e a TV, que exibe o status da fila e os chamados para atendimento.


## Conceito 

1. **Totem:** Funciona como a interface de entrada para os pacientes. Através de um menu, o usuário pode registrar a chegada de um novo paciente, seja com dados aleatórios ("Ficha Simples", utilizada para testes no qual deseja verificar se a fila de prioridade esta funcionando de acordo) ou inserindo informações detalhadas como nome, idade e a especialidade desejada ("Ficha Personalizada", que simula uma situação real).

2. **TV:** Simula a tela da sala de espera. Este programa lê as informações enviadas pelo totem, organiza os pacientes em filas de acordo com sua prioridade e simula o fluxo de atendimento, chamando os pacientes, mostrando quem está sendo atendido e anunciando os próximos. 

A comunicação entre os dois programas é feita de forma assíncrona através de um arquivo binário (**configuracoes.dat**), que serve como um canal de troca de informações e comandos. 
## Estrutura de Dados

A organização dos pacientes é o núcleo do projeto e utiliza duas estruturas de dados distintas para gerenciar a fila:

- **Árvore Binária de Busca**: Pacientes com prioridade (Gestantes, Idosos, PNE, etc.) são inseridos em uma árvore binária. A prioridade do paciente é a chave do nó. Isso garante que, ao chamar o próximo paciente, o sistema sempre busque de forma eficiente o de maior prioridade disponível (o nó mais à esquerda da árvore).

- **Fila(FIFO)**: Os "Demais Pacientes", que não possuem prioridade, são alocados em uma fila simples (First-In, First-Out). Isso garante um atendimento justo por ordem de chegada para essa categoria.
## Funcionalidades 

- **Dois Programas Independentes**: Separação clara entre a entrada de dados (totem) e o processamento/visualização (tv).

- **Sistema de Prioridades**: Gerenciamento de pacientes prioritários utilizando uma árvore para garantir o atendimento correto.

- **Atendimento Personalizado e Aleatório**: O totem permite tanto a criação de fichas com dados inseridos pelo usuário quanto a geração de fichas com dados aleatórios para simulações rápidas.

- **Simulação de Atendimento**: A TV exibe o paciente em atendimento, o tempo simulado do procedimento e um "diagnóstico" bem-humorado ao final.

- **Relatórios**: Capacidade de gerar um relatório que exibe a quantidade de atendimentos realizados para cada especialidade médica.

- **Especialidades Variadas**: O sistema conta com uma lista de especialidades que vai do comum ao cômico, como "Oftalmologista", "Análise de Calvície" e "Cobaia de Experimento Mortal".
# Para executar 

## Compilação 

O projeto utiliza um Makefile para facilitar a compilação. Para compilar, basta executar o comando no seu terminal:

 ```bash
  make
```
isso ira gerar os executaveis tv e totem. 

## Execução

Para que o sistema funcione corretamente, você deve executar cada programa em um terminal separado:

 ```bash
  ./tv
```
a tv começara no modo "aguardando"

No segundo terminal execute:  
 ```bash
  ./totem
```
no totem aparecera uma lista de opções, para selecionar basta digitar o numero e precionar enter, as atualizações apareceram automaticamente no terminal TV . 