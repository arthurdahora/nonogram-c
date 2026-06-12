

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "util.h"

#define MAX_CLUES 4

#define MAX_HISTORICO 64

#define LARGURA_JANELA 800
#define ALTURA_JANELA 800
#define TITULO_JANELA "Nonogram"

#define LINHAS 8
#define COLUNAS 8
#define ALTURA_CELULA 50
#define LARGURA_CELULA 50
#define LARGURA_TABULEIRO LARGURA_CELULA *COLUNAS
#define ALTURA_TABULEIRO ALTURA_CELULA *LINHAS
#define MARGIN_HORIZONTAL (int)((LARGURA_JANELA / 2) - (LARGURA_TABULEIRO / 2))
#define MARGIN_VERTICAL (int)((ALTURA_JANELA / 2) - (ALTURA_TABULEIRO / 2))

typedef enum
{
    MENU,
    JOGANDO,
    GANHOU,
    PERDEU
} EstadoJogo;
typedef enum
{
    MODO_NORMAL,
    MODO_CLASSICO
} ModoJogo;

typedef struct
{
    int linha;
    int coluna;
    int estado_anterior;
} Acao;

void inicia_tabuleiro(Celula tabuleiro[][COLUNAS]);
void desenha_tabuleiro(Tela *t, Celula tabuleiro[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[]);
void verifica_clique(Tela *t, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], ModoJogo modo_jogo, int *vidas, EstadoJogo *estado_jogo, Acao historico[], int *topo_historico, int *celulas_pintadas);
void escreve_numeros(Tela *t, int *numeros, int qtd, Ponto inicio, bool horizontal);
void cicla_estado_celula(Celula *c);
void gera_solucao(bool solucao[][COLUNAS]);
void calcula_clues_linha(bool solucao[][COLUNAS], int linha, int clues[], int *qtd);
void calcula_clues_coluna(bool solucao[][COLUNAS], int coluna, int clues[], int *qtd);
void calcula_todos_clues(bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[]);
void gera_nonogram(bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[]);
bool verifica_vitoria(Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS]);
void desenha_menu(Tela *t, ModoJogo *modo_jogo, EstadoJogo *estado_jogo, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[]);
void limpa_tabuleiro(Celula tabuleiro[][COLUNAS]);
void desenha_botoes_jogo(Tela *t, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[], int *vidas, EstadoJogo *estado_jogo, ModoJogo modo_jogo, Acao historico[], int *topo_historico, int *celulas_pintadas, int *total_celulas_solucao);
void desenha_tela_ganhou(Tela *t, EstadoJogo *estado_jogo, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[], int *vidas);
void desenha_tela_perdeu(Tela *t, EstadoJogo *estado_jogo, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[], int *vidas);
void desenha_vidas(Tela *t, int vidas);
void empilha_acao(Acao historico[], int *topo, int i, int j, int estado_anterior);
void desfaz_acao(Celula tabuleiro[][COLUNAS], Acao historico[], int *topo, int *celulas_pintadas);
int conta_celulas_solucao(bool solucao[][COLUNAS]);
void desenha_contador(Tela *t, int celulas_pintadas, int total);
void desenha_botoes_fim_de_jogo(Tela *t, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[], int *vidas, EstadoJogo *estado_jogo, ModoJogo modo_jogo, Acao historico[], int *topo_historico, int *celulas_pintadas, int *total_celulas_solucao);

int main(int argc, char **argv)
{
    EstadoJogo estado_jogo = MENU;
    ModoJogo modo_jogo;
    bool solucao[LINHAS][COLUNAS];

    EstadoJogo estado_anterior = MENU;

    int vidas = 3;

    int clues_linhas[LINHAS][MAX_CLUES];
    int qtd_clues_linhas[LINHAS];

    int clues_colunas[COLUNAS][MAX_CLUES];
    int qtd_clues_colunas[COLUNAS];

    int tecla_pressionada;

    Acao historico[MAX_HISTORICO];
    int topo_historico = 0;
    int total_celulas_solucao = 0;
    int celulas_pintadas = 0;

    Tela t;

    Celula tabuleiro_nonogram[LINHAS][COLUNAS];

    inicia_tabuleiro(tabuleiro_nonogram);

    srand(time(NULL));

    inicia_tela(&t, LARGURA_JANELA, ALTURA_JANELA, TITULO_JANELA);

    while ((tecla_pressionada = codigo_tecla(&t)) != ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        limpa_tela(&t);

        if (estado_anterior == MENU && estado_jogo == JOGANDO)
        {
            celulas_pintadas = 0;
            topo_historico = 0;
            vidas = 3;
        }
        estado_anterior = estado_jogo;
        if (estado_jogo == MENU)
        {
            desenha_menu(&t, &modo_jogo, &estado_jogo, tabuleiro_nonogram, solucao, clues_linhas, qtd_clues_linhas, clues_colunas, qtd_clues_colunas);
        }
        else if (estado_jogo == JOGANDO)
        {
            total_celulas_solucao = conta_celulas_solucao(solucao);

            desenha_tabuleiro(&t, tabuleiro_nonogram, clues_linhas, qtd_clues_linhas, clues_colunas, qtd_clues_colunas);
            desenha_botoes_jogo(&t, tabuleiro_nonogram, solucao, clues_linhas, qtd_clues_linhas, clues_colunas, qtd_clues_colunas, &vidas, &estado_jogo, modo_jogo, historico, &topo_historico, &celulas_pintadas, &total_celulas_solucao);
            verifica_clique(&t, tabuleiro_nonogram, solucao, modo_jogo, &vidas, &estado_jogo, historico, &topo_historico, &celulas_pintadas);

            if (modo_jogo == MODO_NORMAL)
            {
                desenha_vidas(&t, vidas);
            }
            if (modo_jogo == MODO_CLASSICO)
            {
                desenha_contador(&t, celulas_pintadas, total_celulas_solucao);
            }
        }
        else if (estado_jogo == GANHOU)
        {
            desenha_tela_ganhou(&t, &estado_jogo, tabuleiro_nonogram, solucao, clues_linhas, qtd_clues_linhas, clues_colunas, qtd_clues_colunas, &vidas);
            desenha_botoes_fim_de_jogo(&t, tabuleiro_nonogram, solucao, clues_linhas, qtd_clues_linhas, clues_colunas, qtd_clues_colunas, &vidas, &estado_jogo, modo_jogo, historico, &topo_historico, &celulas_pintadas, &total_celulas_solucao);
        }
        else if (estado_jogo == PERDEU)
        {
            desenha_tela_perdeu(&t, &estado_jogo, tabuleiro_nonogram, solucao, clues_linhas, qtd_clues_linhas, clues_colunas, qtd_clues_colunas, &vidas);
            desenha_botoes_fim_de_jogo(&t, tabuleiro_nonogram, solucao, clues_linhas, qtd_clues_linhas, clues_colunas, qtd_clues_colunas, &vidas, &estado_jogo, modo_jogo, historico, &topo_historico, &celulas_pintadas, &total_celulas_solucao);
        }

        mostra_tela();
        espera(30);
    }

    finaliza_tela(&t);

    return 0;
}

void inicia_tabuleiro(Celula tabuleiro[][COLUNAS])
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            int x = MARGIN_HORIZONTAL + (LARGURA_CELULA * j);
            int y = MARGIN_VERTICAL + (ALTURA_CELULA * i);
            Celula c = {{x, y}, {ALTURA_CELULA, LARGURA_CELULA}, VAZIO};
            tabuleiro[i][j] = c;
        }
    }
}

void desenha_tabuleiro(Tela *t, Celula tabuleiro[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[])
{
    Cor preto = {0, 0, 0};
    Cor vermelho = {1, 0, 0};
    define_cor(t, preto);

    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            desenha_retangulo(tabuleiro[i][j], t, tabuleiro[i][j].estado == COLORIDO);
            if (tabuleiro[i][j].estado == ALERTA)
            {
                define_cor(t, vermelho);
                desenha_x_dentro_ret(tabuleiro[i][j], t);
                define_cor(t, preto);
            }
        }
    }

    for (int i = 0; i < LINHAS; i++)
    {
        int x = MARGIN_HORIZONTAL - 10;
        int y = MARGIN_VERTICAL + (i * ALTURA_CELULA) + 15;
        Ponto p = {x, y};

        escreve_numeros(t, clues_linhas[i], qtd_clues_linhas[i], p, true);
    }

    for (int j = 0; j < COLUNAS; j++)
    {
        int x = MARGIN_HORIZONTAL + (j * LARGURA_CELULA) + 15;
        int y = MARGIN_VERTICAL - 10;
        Ponto p = {x, y};

        escreve_numeros(t, clues_colunas[j], qtd_clues_colunas[j], p, false);
    }
}

void escreve_numeros(Tela *t, int *numeros, int qtd, Ponto inicio, bool horizontal)
{
    int x = inicio.x, y = inicio.y, offset = 20;
    for (int i = 0; i < qtd; i++)
    {
        char buffer[10];
        sprintf(buffer, "%d", numeros[i]);

        if (horizontal)
        {
            if (i > 0)
                strcat(buffer, ",");
            x -= offset;
        }
        else
            y -= offset;

        Ponto p = {x, y};

        escreve_texto(t, p, buffer);
    }
}

void verifica_clique(Tela *t, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], ModoJogo modo_jogo, int *vidas, EstadoJogo *estado_jogo, Acao historico[], int *topo_historico, int *celulas_pintadas)
{
    Ponto mouse = posicao_mouse(t);

    if (botao_clicado(t))
    {
        int y_clique_tabuleiro = (mouse.x - MARGIN_HORIZONTAL);
        int x_clique_tabuleiro = (mouse.y - MARGIN_VERTICAL);

        if (x_clique_tabuleiro >= 0 && y_clique_tabuleiro >= 0)
        {

            int i = (int)(x_clique_tabuleiro / ALTURA_CELULA);
            int j = (int)(y_clique_tabuleiro / LARGURA_CELULA);

            if (i >= 0 && i < LINHAS && j >= 0 && j < COLUNAS)
            {
                if (modo_jogo == MODO_NORMAL)
                {
                    if (tabuleiro[i][j].estado == VAZIO && solucao[i][j] == 0)
                    {
                        tabuleiro[i][j].estado = ALERTA;
                        (*vidas)--;
                    }
                    if (tabuleiro[i][j].estado == VAZIO && solucao[i][j] == 1)
                    {
                        tabuleiro[i][j].estado = COLORIDO;
                        (*celulas_pintadas)++;
                    }
                }
                if (modo_jogo == MODO_CLASSICO)
                {
                    int estado_ant = tabuleiro[i][j].estado; // IA:CLAUDE
                    cicla_estado_celula(&tabuleiro[i][j]);
                    empilha_acao(historico, topo_historico, i, j, estado_ant); // IA:CLAUDE

                    if (tabuleiro[i][j].estado == COLORIDO)
                        (*celulas_pintadas)++;
                    if (estado_ant == COLORIDO)
                        (*celulas_pintadas)--;
                }

                if (verifica_vitoria(tabuleiro, solucao))
                    *estado_jogo = GANHOU;
                if (*vidas <= 0)
                    *estado_jogo = PERDEU;
            }
        }
        t->_botao = false;
    }
}

void cicla_estado_celula(Celula *c)
{

    switch (c->estado)
    {

    case VAZIO:
        c->estado = COLORIDO;
        break;

    case COLORIDO:
        c->estado = ALERTA;
        break;

    case ALERTA:
        c->estado = VAZIO;
        break;
    }
}
void gera_solucao(bool solucao[][COLUNAS])
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            solucao[i][j] = rand() % 2;
        }
    }
}
void calcula_clues_linha(bool solucao[][COLUNAS], int linha, int clues[], int *qtd)
{
    int contagem = 0;
    for (int j = 0; j < COLUNAS; j++)
    {
        if (solucao[linha][j] == 1)
        {
            contagem++;
        }
        else if (solucao[linha][j] == 0 && contagem > 0)
        {
            clues[*qtd] = contagem;
            (*qtd)++;
            contagem = 0;
        }
    }
    if (contagem > 0)
    {
        clues[*qtd] = contagem;
        (*qtd)++;
    }
}
void calcula_clues_coluna(bool solucao[][COLUNAS], int coluna, int clues[], int *qtd)
{
    int contagem = 0;
    for (int i = 0; i < LINHAS; i++)
    {
        if (solucao[i][coluna] == 1)
        {
            contagem++;
        }
        else if (solucao[i][coluna] == 0 && contagem > 0)
        {
            clues[*qtd] = contagem;
            (*qtd)++;
            contagem = 0;
        }
    }
    if (contagem > 0)
    {
        clues[*qtd] = contagem;
        (*qtd)++;
    }
}
void calcula_todos_clues(bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[])
{
    for (int i = 0; i < LINHAS; i++)
    {
        qtd_clues_linhas[i] = 0;
        calcula_clues_linha(solucao, i, clues_linhas[i], &qtd_clues_linhas[i]);
    }
    for (int j = 0; j < COLUNAS; j++)
    {
        qtd_clues_colunas[j] = 0;
        calcula_clues_coluna(solucao, j, clues_colunas[j], &qtd_clues_colunas[j]);
    }
}
void gera_nonogram(bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[])
{
    bool valido = false;
    while (!valido)
    {
        gera_solucao(solucao);
        calcula_todos_clues(solucao, clues_linhas, qtd_clues_linhas, clues_colunas, qtd_clues_colunas);
        valido = true;
        for (int i = 0; i < LINHAS && valido; i++)
        {
            if (qtd_clues_linhas[i] > MAX_CLUES || qtd_clues_linhas[i] == 0)
                valido = false;
            for (int k = 0; k < qtd_clues_linhas[i] && valido; k++)
            {
                if (clues_linhas[i][k] > MAX_CLUES)
                    valido = false;
            }
        }
        for (int j = 0; j < COLUNAS && valido; j++)
        {
            if (qtd_clues_colunas[j] > MAX_CLUES || qtd_clues_colunas[j] == 0)
                valido = false;
            for (int k = 0; k < qtd_clues_colunas[j] && valido; k++)
            {
                if (clues_colunas[j][k] > MAX_CLUES)
                    valido = false;
            }
        }
    }
}

bool verifica_vitoria(Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS])
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (solucao[i][j] == 1 && tabuleiro[i][j].estado != COLORIDO)

            {

                return false;
            }
            if (solucao[i][j] == 0 && tabuleiro[i][j].estado == COLORIDO)
            {
                return false;
            }
        }
    }
    return true;
}
void desenha_menu(Tela *t, ModoJogo *modo_jogo, EstadoJogo *estado_jogo, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[])
{
    Ponto titulo = {370, 300};
    escreve_texto(t, titulo, "Nonogram");

    Botao button_normal = {{{300, 350}, {200, 50}, VAZIO}, NORMAL, "Normal"};
    desenha_retangulo(button_normal.ret, t, false);
    Ponto p = {button_normal.ret.pos.x + 10, button_normal.ret.pos.y + 15};
    escreve_texto(t, p, "Normal");

    Botao button_classico = {{{300, 430}, {200, 50}, VAZIO}, NORMAL, "Classico"};
    desenha_retangulo(button_classico.ret, t, false);
    Ponto a = {button_classico.ret.pos.x + 10, button_classico.ret.pos.y + 15};
    escreve_texto(t, a, "Classico");
    Ponto mouse = posicao_mouse(t);
    if (botao_clicado(t))
    {
        if (mouse.x >= button_normal.ret.pos.x && // IA:CLAUDE fez a condição dos if's
            mouse.x <= button_normal.ret.pos.x + button_normal.ret.tam.larg &&
            mouse.y >= button_normal.ret.pos.y &&
            mouse.y <= button_normal.ret.pos.y + button_normal.ret.tam.alt)
        {
            *modo_jogo = MODO_NORMAL;
            inicia_tabuleiro(tabuleiro);
            gera_nonogram(solucao, clues_linhas, qtd_clues_linhas, clues_colunas, qtd_clues_colunas);
            *estado_jogo = JOGANDO;
            t->_botao = false;
        }
        if (mouse.x >= button_classico.ret.pos.x &&
            mouse.x <= button_classico.ret.pos.x + button_classico.ret.tam.larg &&
            mouse.y >= button_classico.ret.pos.y &&
            mouse.y <= button_classico.ret.pos.y + button_classico.ret.tam.alt)
        {
            *modo_jogo = MODO_CLASSICO;
            inicia_tabuleiro(tabuleiro);
            gera_nonogram(solucao, clues_linhas, qtd_clues_linhas, clues_colunas, qtd_clues_colunas);

            *estado_jogo = JOGANDO;
            t->_botao = false;
        }
    }
}
void limpa_tabuleiro(Celula tabuleiro[][COLUNAS])
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            tabuleiro[i][j].estado = VAZIO;
        }
    }
}
void desenha_botoes_jogo(Tela *t, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[], int *vidas, EstadoJogo *estado_jogo, ModoJogo modo_jogo, Acao historico[], int *topo_historico, int *celulas_pintadas, int *total_celulas_solucao)
{
    Botao button_limpar = {{{100, 650}, {200, 50}, VAZIO}, NORMAL, "Limpar"};
    desenha_retangulo(button_limpar.ret, t, false);
    Ponto p = {button_limpar.ret.pos.x + 10, button_limpar.ret.pos.y + 15};
    escreve_texto(t, p, "Limpar");

    Botao button_novo_nonogram = {{{310, 650}, {200, 50}, VAZIO}, NORMAL, "Novo Nonogram"};
    desenha_retangulo(button_novo_nonogram.ret, t, false);
    Ponto a = {button_novo_nonogram.ret.pos.x + 10, button_novo_nonogram.ret.pos.y + 15};
    escreve_texto(t, a, "Novo Nonogram");

    Botao button_menu_nonogram = {{{520, 650}, {200, 50}, VAZIO}, NORMAL, "MENU"};
    desenha_retangulo(button_menu_nonogram.ret, t, false);
    Ponto b = {button_menu_nonogram.ret.pos.x + 10, button_menu_nonogram.ret.pos.y + 15};
    escreve_texto(t, b, "MENU");

    Botao button_desfazer = {{{100, 720}, {200, 50}, VAZIO}, NORMAL, "Desfazer"};

    Ponto mouse = posicao_mouse(t);

    if (modo_jogo == MODO_CLASSICO)
    {
        desenha_retangulo(button_desfazer.ret, t, false);
        Ponto d = {button_desfazer.ret.pos.x + 10, button_desfazer.ret.pos.y + 15};
        escreve_texto(t, d, "Desfazer");
    }

    if (botao_clicado(t))
    {
        if (mouse.x >= button_limpar.ret.pos.x &&
            mouse.x <= button_limpar.ret.pos.x + button_limpar.ret.tam.larg &&
            mouse.y >= button_limpar.ret.pos.y &&
            mouse.y <= button_limpar.ret.pos.y + button_limpar.ret.tam.alt)
        {

            limpa_tabuleiro(tabuleiro);
            *vidas = 3;
            *topo_historico = 0;
            *celulas_pintadas = 0;

            *estado_jogo = JOGANDO;
            t->_botao = false;
        }
        if (modo_jogo == MODO_CLASSICO &&
            mouse.x >= button_desfazer.ret.pos.x &&
            mouse.x <= button_desfazer.ret.pos.x + button_desfazer.ret.tam.larg &&
            mouse.y >= button_desfazer.ret.pos.y &&
            mouse.y <= button_desfazer.ret.pos.y + button_desfazer.ret.tam.alt)
        {
            desfaz_acao(tabuleiro, historico, topo_historico, celulas_pintadas);
            t->_botao = false;
        }

        if (mouse.x >= button_novo_nonogram.ret.pos.x &&
            mouse.x <= button_novo_nonogram.ret.pos.x + button_novo_nonogram.ret.tam.larg &&
            mouse.y >= button_novo_nonogram.ret.pos.y &&
            mouse.y <= button_novo_nonogram.ret.pos.y + button_novo_nonogram.ret.tam.alt)
        {

            limpa_tabuleiro(tabuleiro);
            gera_nonogram(solucao, clues_linhas, qtd_clues_linhas, clues_colunas, qtd_clues_colunas);

            *vidas = 3;
            *topo_historico = 0;
            *celulas_pintadas = 0;
            *total_celulas_solucao = conta_celulas_solucao(solucao);

            *estado_jogo = JOGANDO;
            t->_botao = false;
        }
        if (mouse.x >= button_menu_nonogram.ret.pos.x &&
            mouse.x <= button_menu_nonogram.ret.pos.x + button_menu_nonogram.ret.tam.larg &&
            mouse.y >= button_menu_nonogram.ret.pos.y &&
            mouse.y <= button_menu_nonogram.ret.pos.y + button_menu_nonogram.ret.tam.alt)
        {

            *estado_jogo = MENU;
            t->_botao = false;
        }
    }
}
void desenha_tela_ganhou(Tela *t, EstadoJogo *estado_jogo, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[], int *vidas)
{
    Ponto titulo = {370, 300};
    escreve_texto(t, titulo, "GANHOU");
}

void desenha_tela_perdeu(Tela *t, EstadoJogo *estado_jogo, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[], int *vidas)
{
    Ponto titulo = {370, 300};
    escreve_texto(t, titulo, "PERDEU");
}
void desenha_vidas(Tela *t, int vidas)
{
    char texto[15];
    Ponto lugar = {50, 50};
    sprintf(texto, "Vidas: %d ", vidas);
    escreve_texto(t, lugar, texto);
}
void empilha_acao(Acao historico[], int *topo, int i, int j, int estado_anterior) // IA: CLAUDE fez a funcao
{
    if (*topo < MAX_HISTORICO)
    {

        historico[*topo].linha = i;
        historico[*topo].coluna = j;
        historico[*topo].estado_anterior = estado_anterior;
        (*topo)++;
    }
}
void desfaz_acao(Celula tabuleiro[][COLUNAS], Acao historico[], int *topo, int *celulas_pintadas) // IA: CLAUDE fez a funcao
{
    if (*topo > 0)
    {
        (*topo)--;
        int linha = historico[*topo].linha;
        int coluna = historico[*topo].coluna;
        int estado_ANT = historico[*topo].estado_anterior;

        if (tabuleiro[linha][coluna].estado == COLORIDO)
        {
            (*celulas_pintadas)--;
        }
        tabuleiro[linha][coluna].estado = estado_ANT;
    }
}

int conta_celulas_solucao(bool solucao[][COLUNAS])
{
    int cont = 0;
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (solucao[i][j] == 1)
            {
                cont++;
            }
        }
    }
    return cont;
}
void desenha_contador(Tela *t, int celulas_pintadas, int total)
{
    char texto[30];
    Ponto lugar = {50, 50};
    sprintf(texto, "Restam: %d / %d", total - celulas_pintadas, total);
    escreve_texto(t, lugar, texto);
}
void desenha_botoes_fim_de_jogo(Tela *t, Celula tabuleiro[][COLUNAS], bool solucao[][COLUNAS], int clues_linhas[][MAX_CLUES], int qtd_clues_linhas[], int clues_colunas[][MAX_CLUES], int qtd_clues_colunas[], int *vidas, EstadoJogo *estado_jogo, ModoJogo modo_jogo, Acao historico[], int *topo_historico, int *celulas_pintadas, int *total_celulas_solucao)
{
    Botao button_tentar = {{{310, 650}, {200, 50}, VAZIO}, NORMAL, "TENTAR NOVAMENTE"};
    desenha_retangulo(button_tentar.ret, t, false);
    Ponto k = {button_tentar.ret.pos.x + 10, button_tentar.ret.pos.y + 15};
    escreve_texto(t, k, "TENTAR NOVAMENTE");
    Ponto mouse = posicao_mouse(t);
    if (botao_clicado(t))
    {
        if (mouse.x >= button_tentar.ret.pos.x &&
            mouse.x <= button_tentar.ret.pos.x + button_tentar.ret.tam.larg &&
            mouse.y >= button_tentar.ret.pos.y &&
            mouse.y <= button_tentar.ret.pos.y + button_tentar.ret.tam.alt)
        {
            *estado_jogo = MENU;
            t->_botao = false;
        }
    }
}