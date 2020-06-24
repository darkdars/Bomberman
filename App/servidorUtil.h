#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <ncurses.h>

#define FIFO "sss"

int NMAXPLAY = 0;
int NOBJECT = 0;
int NENEMY = 0;

typedef struct Tamanho Tamanho,*Tam;
typedef struct Labirinto Labirinto;
typedef struct Clientes Clientes,*Cli;
typedef struct LocalizacaoI LocalizacaoI,*LocalI;
typedef struct Jogador Jogador, *Jog;
typedef struct Informacao Informacao, *InfoP;
typedef struct TInimigo TInimigo, *TIP;


struct Tamanho{
	int largura;
	int comprimento;
};

struct LocalizacaoI{
	int antx, x;
	int anty, y;
	int antp, p;
	bool vida, bot;
};


struct Jogador{
	int pid;
	int cara;
	int bombas, mBombas;
	int vidas;
	int pontos;
	int antx, x;
	int anty, y;
	int antp, p;
	pthread_t bombaPT;
	bool bombaLock;
};

struct Labirinto{
	int Tab[600];
	bool pronto, run, vitoria, final;
	int fim;
	Jogador jogadores[20];
	LocalizacaoI inimigos[20];
	int nBomba, pBomba, bPid;
};

struct Clientes{
	int pid;
	char nome[20];
	char password[20];
	char cmd[100];
	char msg1[100];
	int info;
};

struct Informacao{
	bool dentro, sair, fim, vitoria;
	int anty, antx, x, y, info, tipo, atualiza;
	int cara;
};