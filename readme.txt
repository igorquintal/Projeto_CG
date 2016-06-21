 _______ _________ _______           _______  _       
(       )\__   __/(  ____ \|\     /|(  ____ \( \      
| () () |   ) (   | (    \/| )   ( || (    \/| (      
| || || |   | |   | |      | |   | || (__    | |      
| ||_|| |   | |   | | ____ | |   | ||  __)   | |      
| |   | |   | |   | | \_  )| |   | || (      | |      
| )   ( |___) (___| (___) || (___) || (____/\| (____/\
|/     \|\_______/(_______)(_______)(_______/(_______/
                                                      
 _______  _______ _________ _______  _______  _       _________
(  ____ )(  ____ \\__   __/(  ____ )(  ___  )( (    /|\__   __/
| (    )|| (    \/   ) (   | (    )|| (   ) ||  \  ( |   ) (   
| (____)|| (__       | |   | (____)|| |   | ||   \ | |   | |   
|  _____)|  __)      | |   |     __)| |   | || (\ \) |   | |   
| (      | (         | |   | (\ (   | |   | || | \   |   | |   
| )      | (____/\   | |   | ) \ \__| (___) || )  \  |___) (___
|/       (_______/   )_(   |/   \__/(_______)|/    )_)\_______/
                                                               
 _______ _________ _______           _        _______ _________ _______  _______ 
(  ____ \\__   __/(       )|\     /|( \      (  ___  )\__   __/(  ___  )(  ____ )
| (    \/   ) (   | () () || )   ( || (      | (   ) |   ) (   | (   ) || (    )|
| (_____    | |   | || || || |   | || |      | (___) |   | |   | |   | || (____)|
(_____  )   | |   | ||_|| || |   | || |      |  ___  |   | |   | |   | ||     __)
      ) |   | |   | |   | || |   | || |      | (   ) |   | |   | |   | || (\ (   
/\____) |___) (___| )   ( || (___) || (____/\| )   ( |   | |   | (___) || ) \ \__
\_______)\_______/|/     \|(_______)(_______/|/     \|   )_(   (_______)|/   \__/


=============================================================================================================
=============================================================================================================

Trabalho de Computação Gráfica utilizando OpenGL e Glut.

Feito pelos alunos Cassiano Zaghi (7987400), Igor Quintal (8622353) e Lucas Tomazela (8124271), do curso de Engenharia de Computação - USP.

=============================================================================================================
=============================================================================================================

Comandos para jogar:
-->	'Setas' (esquerda e direita) controlam o ônibus.
-->	Tecla 'Espaço' troca de câmera (primeira ou terceira pessoa).
--> Em primeira pessoa, 'P' ativa e desativa o painel.
--> 'S' inicia o jogo.
--> 'R' reinicia  o jogo, após perder.

Ao colidir com um muro, o jogador perde uma vida (do total de 3). Ao colodir com um boost (cubo em chamas), a velocidade do ônibus aumenta.
Após perder o jogo, aperte 'r' para reiniciá-lo.

============================================================================================================
============================================================================================================

Desenvolvimento do jogo:

O jogo foi inteiramente desenvolvido em Linux, mais especificamente, em distribuições Ubuntu (15.04 e 16.04).
A linguagem de programação usada foi unicamente a linguagem C, e foi usada a biblioteca OpenGL Utility Toolkit (GLUT) para o desenvolvimento em OpenGL.
Quaisquer outras bibliotecas externas não foram utilizadas. Caso o usuário não possua as bibliotecas GLUT, ele pode instalá-las através dos comandos:

--> OpenGL Utility Toolkit (GLUT)
# apt-get install freeglut3-dev

--> Miscellaneous Mesa GL utilities
# sudo apt-get install mesa-utils

--> X11 miscellaneous utility library, X11 Input extension library
# sudo apt-get install libxmu-dev libxi-dev

Evidentemente que o compilador GCC também é necessário.

===========================================================================================================
===========================================================================================================

Texturas:

Todas as texturas necessárias para o funcionamento do programa se encontram em anexo, juntamente com o código.
ATENÇÃO: AS TEXTURAS DEVEM ESTAR EXATAMENTE NA MESMA PASTA QUE O ARQUIVO DE CÓDIGO E EXECUTÁVEL. CASO CONTRÁRIO, UM ERRO DE EXECUÇÃO SERÁ APRESENTADO.

==========================================================================================================
==========================================================================================================
Compilação e Execução:

Um MakeFile se encontra em anexo, juntamente com o código. Para compilação e execução, são necessários os seguitnes comandos:

-->	Compilar:
# make

--> Executar:
# make run

--> Limpar arquivo executável:
# make clean

Caso o usuário não queira usar o MakeFile, basta inserir os seguintes comandos:

-->	Compilar:
# gcc trab1.c -o trab -lglut -lGLU -lGL -lm -Wall

--> Executar:
# ./trab

Onde 'trab.c' é o arqivo em código C.

========================================================================================================
========================================================================================================

GitHub e link para vídeo:

--> https://github.com/igorquintal/Projeto_CG.git
--> https://www.youtube.com/watch?v=qxnibcGTMhk

========================================================================================================
========================================================================================================

Quaisquer dúvidas, entrar em contato com:

--> Cassiano Zaghi de Oliveira: czoliv@gmail.com
--> Igor Quintal Mendes: igorquintalmendes@gmail.com
--> Lucas Tomazela: tomazela.lucas@gmail.com

========================================================================================================
========================================================================================================