# Jogo de Exploração - Projeto de Programação avançada

Uma aplicação simples em C desenvolvida para terminal, onde o jogador navega por um mapa 2D, desvia de obstáculos e coleta itens.

## Breve Descrição:

O projeto consiste em um jogo de labirinto em modo texto no qual o jogador controla um personagem representado pelo símbolo @. O objetivo é explorar o mapa, desviar das paredes (#) e coletar itens ($) para aumentar a pontuação.

## Elementos do Mapa:

@: Jogador

#: Parede/Obstáculo (bloqueia a movimentação)

$: Item coletável

.: Espaço livre/caminho percorrível

## Estrutura dos modulos:

O código atual está contido no arquivo main.c e estruturado da seguinte forma:

Configurações Globais:

Define a dimensão da matriz/mapa (LINHAS e COLUNAS).

mapa[][]: Matriz que representa o cenário.

jogadorX e jogadorY: Posição atual do jogador.

itensColetados: Contador de pontuação.

Função mostrarMapa():

Módulo responsável por renderizar o estado atual do mapa e exibir o personagem @ na tela, além da pontuação atual.

Função main():

Loop Principal (while): Gerencia o fluxo contínuo do jogo.

Leitura de Comandos: Entrada via teclado (controles WASD para movimentação e Q para sair).

Validação de Movimento: Verifica limites do mapa e colisões com obstáculos (#).

Coleta de Itens: Atualiza o contador e remove o item ($) do mapa ao passar por ele.

Instruções de Compilação e Execução

## Pré-requisitos

Um compilador C instalado (como gcc ou clang).

## Compilação:

Para compilar o arquivo joguin.c, abra o terminal na pasta do arquivo e execute:

gcc main.c -o Jogo


## Execução:

Windows (Command Prompt / PowerShell):

Jogo.exe


## Controles:

W: Mover para cima

A: Mover para a esquerda

S: Mover para baixo

D: Mover para a direita

Q: Sair do jogo

## Integrantes: 

**Daniel Matos Oitaven - 27805**