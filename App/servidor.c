#include "servidorUtil.h"

int cliShutdown[20];
pthread_mutex_t trinco;

//Forma base de comunicao
void escCliente(Clientes *pCli)
{
	Clientes tempCli = * pCli;
	int fdCliente;
	char np[20];
	sprintf(np, "np%d", tempCli.pid);
	fdCliente = open(np, O_WRONLY);
	write(fdCliente, &tempCli, sizeof(tempCli));
	close(fdCliente);
}
//Envia mapa ao jogador que acabou de entrar no jogo
void escCliente2(int *pLab, int pid)
{
	int tempLab[600];
	for (int i = 0; i < 600; i++){tempLab[i]=pLab[i];}
	int fdCliente;
	char np[20];
	sprintf(np, "np%d", pid);
	fdCliente = open(np, O_WRONLY);
	write(fdCliente, tempLab, sizeof(int)*600);
	close(fdCliente);
}
//Envia informacao ao jogador que mandou pedido durante o jogo
void escCliente3(Informacao *pInfo, int pid)
{
	Informacao tempInfo = *pInfo;
	int fdCliente;
	char np[20];
	sprintf(np, "np%d", pid);
	fdCliente = open(np, O_WRONLY);
	write(fdCliente, &tempInfo, sizeof(tempInfo));
	close(fdCliente);
}
//Enivia informacao a todos os jogadores
void escCliente4(Labirinto *game, Informacao *pInfo)
{
	Informacao tempInfo = *pInfo;
	int fdCliente;
	char np[20];
	for (int i = 0; i < 20; i++)
	{
		if (game->jogadores[i].pid != 0)
		{
			sprintf(np, "np%d", game->jogadores[i].pid);
			fdCliente = open(np, O_WRONLY);
			write(fdCliente, &tempInfo, sizeof(tempInfo));
			close(fdCliente);
		}
	}
}
//Funcao para separar as palavras como stork
void Seperar(char *string, char *aux, int *p)
{

	int i = 0, j = 0;

	for (i = *p; i<strlen(string); i++) {
		if (*(string + i) == ' ') {
			*p = i + 1;
			return;
		}

		*(aux + j) = *(string + i);
		j++;

	}
}
//Por aux limpo
void limpa(char *aux)
{
	for(int i = 0;i<100;i++){
		*(aux + i) = '\0';
	}
}
Cli LerFicheiro_Clientes(char *nomeFich, int *tam) 
{

	FILE *f;
	Cli aux = NULL;
	char nome[20] = { "" }, password[20] = { "" };
	int i = 0;

	if ((f = fopen(nomeFich, "rt")) == NULL) {
		printf("Erro a Abrir ficheiro %s para leitura!\n", nomeFich);
		return NULL;
	}

	while (fscanf(f, "%s %s", nome, password) == 2) {
		(*tam) = (*tam) + 1;
		aux = realloc(aux, (*tam) * sizeof(Clientes));
		if (aux == NULL) {
			printf("Erro a alocar memoria!\n");
			fclose(f);
		}


		strcpy((aux + i)->nome, nome);
		strcpy((aux + i)->password, password);

		i++;

	}
	fclose(f);
	return aux;
}
void GravaFicheiro_Clientes(char *nomeFich, int tam, Cli x, int new) 
{
	//implementar
	FILE *f;
	char Nome_Fich[100] = "Clientes_aux.txt";


	if ((f = fopen(Nome_Fich, "wt")) == NULL) {
		printf("Erro a abrir o ficheiro %s para escrita!\n", Nome_Fich);
		return;
	}

	for (int i = 0; i<tam; i++) {
		fprintf(f, "%s %s\n", (x + i)->nome, (x + i)->password);
	}

	fclose(f);

	if (new == 0) {
		if (remove(nomeFich) == -1) {
			printf("ERRO A REMOVER O FICHEIRO %s!\n", nomeFich);
			return;
		}
	}

	if (rename(Nome_Fich, nomeFich) == -1) {
		printf("ERRO A RENOMEAR de %s para %s!\n", Nome_Fich, nomeFich);
		return;
	}
}
int posicaoPid(int pid, Cli lCli) 
{
	for(int i = 0; i<20; i++)
	{
		if(lCli[i].pid == pid)
			return i;
	}
	return 20;
}
int posicaoLogin(char *nome, Cli lCli) 
{
	for(int i = 0; i<20; i++)
	{
		if(strcmp(lCli[i].nome, nome) ==0)
			return i;
	}
	return 20;
}

int getJogador(Labirinto game, int pid)
{
	for (int i = 0; i < 20; ++i)
	{
		if (game.jogadores[i].pid == pid)
		{
			return i;
		}
	}
}

bool verificaRegrasDirecao(int pos, int direcao)
{
	if ((pos + direcao) >= 0 && (pos + direcao) < 600)
	{
		if (direcao == -30 || direcao == 30)
		{
			return TRUE;
		}else if ((pos + direcao)/30 == pos/30 && (direcao == -1 || direcao == 1))
		{
			return TRUE;
		}else
			return FALSE;
	}else
		return FALSE;	
}

bool verificaRegraPosicao(Labirinto *game, int pos)
{
	if (game->Tab[pos] == 0 || game->Tab[pos] == 3 || game->Tab[pos] == 7 || game->Tab[pos] == 8 || game->Tab[pos] == 9 || game->Tab[pos] == 11 || game->Tab[pos] == 12)
	{
		return TRUE;
	}else
		return FALSE;
}
bool verificaRegraPosicaoB(Labirinto *game, int pos)
{
	if (game->Tab[pos] == 0 || game->Tab[pos] == 7)
	{
		return TRUE;
	}else
		return FALSE;
}

void preparaJogador(Labirinto *game, int nivel, int lugar)
{
	if (nivel == 0)//Prepara Todos os jogadores
	{
		for (int i = 0; i < 20; ++i)
		{
			game->jogadores[i].pid = 0;
			game->jogadores[i].cara = i + 1;
			game->jogadores[i].pid = 0;
			game->jogadores[i].bombas = 3;
			game->jogadores[i].mBombas = 2;
			game->jogadores[i].vidas = 3;
			game->jogadores[i].pontos = 0;
			game->inimigos[i].vida = FALSE;
			game->inimigos[i].bot = FALSE;
		}
		game->fim = 0;
		game->pronto = FALSE;
		game->run = FALSE;
		game->vitoria = FALSE;
		game->final = FALSE;
	}
	else if (nivel == 1)//Inicia o jogo
	{
		game->run = TRUE;
		preparaJogador(game, 2, lugar);
	}
	else if (nivel == 2)//Prepara outros jogadores
	{
		game->jogadores[lugar].antx = 14;
		game->jogadores[lugar].anty = 9;
		game->jogadores[lugar].antp = 284;
		game->jogadores[lugar].x = 14;
		game->jogadores[lugar].y = 9;
		game->jogadores[lugar].p = 284;
		game->Tab[game->jogadores[lugar].p] = (game->jogadores[lugar].cara) * 10;
		game->jogadores[lugar].bombas = 3;
		game->jogadores[lugar].mBombas = 2;
		game->jogadores[lugar].vidas = 3;
		game->jogadores[lugar].pontos = 0;
		game->jogadores[lugar].bombaLock = FALSE;
	}
	else if (nivel == 3)//Tira jogador
	{
		game->jogadores[lugar].pid = 0;
		game->Tab[game->jogadores[lugar].p] = 0;
	}
	else if (nivel == 4)//Tira jogador caso saia e esteja no jogo
	{
		for (int i = 0; i < 20; ++i)
		{
			if (game->jogadores[i].pid == lugar)
			{
				preparaJogador(game, 3, i);
				break;
			}
		}
	}
}
void estadoJogo(Labirinto *game, int nivel)
{
	int aJogar = 0;
	if (nivel == 0)
	{
		if (game->vitoria ==  TRUE)
		{
			Informacao tempInfo;
			tempInfo.vitoria = TRUE;
			game->pronto = FALSE;
			game->run = FALSE;
			escCliente4(game, &tempInfo);
			for (int i = 0; i < NENEMY; i++)
			{
				game->inimigos[i].bot = FALSE; 
			}
			preparaJogador(&game, 0, 0);

		}
	}else if (nivel == 1)
	{
		for (int i = 0; i < 20; i++)
		{
			if (game->jogadores[i].pid != 0)
			{
				aJogar++;
			}
		}
		if (aJogar == 0)
		{
			game->pronto = FALSE;
			game->run = FALSE;
			for (int i = 0; i < NENEMY; i++)
			{
				game->inimigos[i]. bot = FALSE; 
			}

		}
	}else if (nivel == 2)
	{
		if (game->fim == NOBJECT)
		{
			Informacao tempInfo;
			tempInfo.info = 6;
			escCliente4(game, &tempInfo);
			game->final = TRUE;
		}
	}
}
void drop(Labirinto *game, int pos)
{
	int item, item2;
	item = rand()%4;
	item2 = rand()%4;

	switch(item){
		case 0:
			game->Tab[pos] = 8;
		case 1:
			if (item2 > 2)
			{
				game->Tab[pos] = 9;
			}
			else
				game->Tab[pos] = 8;
		case 2:
			if (item2 < 1)
			{
				game->Tab[pos] = 11;
			}
			else
				game->Tab[pos] = 8;
		case 3:
			if (item2 > 1)
			{
				game->Tab[pos] = 12;
			}
			else
				game->Tab[pos] = 8;
	}
}

void *controlaBombas(void *dados)
{
	Labirinto *game = (Labirinto *) dados;
	int nivel = game->nBomba;
	int pos = game->pBomba;
	int pid = game->bPid;
	Informacao info;
	int lugar = getJogador(*game, pid);
	int x, y;
	int n,tipo, posicoes[17], vPosicoes[17], xPosicoes[17], yPosicoes[17], posiContador = 1;
	bool upLock = FALSE, downLock = FALSE, leftLock = FALSE, rightLock = FALSE;

	posicoes[0] = pos;
	vPosicoes[0] = game->Tab[posicoes[0]];
	xPosicoes[0] = x = game->jogadores[lugar].x;
	yPosicoes[0] = y = game->jogadores[lugar].y;
	

	usleep(2000000);
	if (nivel == 1)
	{
		n = 3;
		tipo = 3;
	}else {
		n = 5;
		tipo = 4;
	}
	
	for (int i = 1; i < n; i++)
	{
		if (game->Tab[pos-(30*i)] != 1 && game->Tab[pos-(30*i)] != 3 && upLock !=TRUE)
		{
			posicoes[posiContador] = pos-(30*i);
			vPosicoes[posiContador] = game->Tab[posicoes[posiContador]];
			game->Tab[pos-(30*i)] = 7;
			xPosicoes[posiContador] = info.x = x + 0;
			yPosicoes[posiContador] = info.y = y - 1 * i;
			info.info = 0; info.tipo = tipo;
			posiContador++;
			escCliente4(game, &info);
		}else
			upLock = TRUE;
		if (game->Tab[pos+(30*i)] != 1 && game->Tab[pos+(30*i)] != 3 && downLock != TRUE)
		{
			posicoes[posiContador] = pos+(30*i);
			vPosicoes[posiContador] = game->Tab[posicoes[posiContador]];
			game->Tab[pos+(30*i)] = 7;
			xPosicoes[posiContador] = info.x = x + 0;
			yPosicoes[posiContador] = info.y = y + 1 * i;
			info.info = 0; info.tipo = tipo;
			posiContador++;
			escCliente4(game, &info);
		}else
				downLock = TRUE;
		if (game->Tab[pos-(1*i)] != 1 && game->Tab[pos-(1*i)] != 3 && leftLock != TRUE)
		{
			posicoes[posiContador] = pos-(1*i);
			vPosicoes[posiContador] = game->Tab[posicoes[posiContador]];
			game->Tab[pos-(1*i)] = 7;
			xPosicoes[posiContador] = info.x = x - 1 * i;
			yPosicoes[posiContador] = info.y = y + 0;
			info.info = 0; info.tipo = tipo;
			posiContador++;
			escCliente4(game, &info);
		}else
			leftLock = TRUE;
		if (game->Tab[pos+(1*i)] != 1 && game->Tab[pos+(1*i)] != 3 && rightLock!= TRUE)
		{
			posicoes[posiContador] = pos+(1*i);
			vPosicoes[posiContador] = game->Tab[posicoes[posiContador]];
			game->Tab[pos+(1*i)] = 7;
			xPosicoes[posiContador] = info.x = x + 1 * i;
			yPosicoes[posiContador] = info.y = y + 0;
			info.info = 0; info.tipo = tipo;
			posiContador++;
			escCliente4(game, &info);
		}else 
			rightLock= TRUE;
	}

	for (int i = 0; i < posiContador; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (posicoes[i] == game->jogadores[j].p)
			{
				info.atualiza = --game->jogadores[j].vidas;
				info.info = 1;
				if (game->jogadores[j].vidas < 0)
				{
					int l2 = getJogador(*game, game->jogadores[j].pid);
					info.fim = TRUE;
					escCliente3(&info, game->jogadores[j].pid);
					info.fim = FALSE;
					preparaJogador(game, 3, l2);
					estadoJogo(game, 1);
				}
				else
					escCliente3(&info, game->jogadores[j].pid);
				info.cara = game->jogadores[j].cara;
				info.info = 0; info.tipo = 5;
				info.antx = game->jogadores[j].x; info.x = game->jogadores[j].x; info.anty = game->jogadores[j].y; info.y = game->jogadores[j].y;
				escCliente4(game, &info);
			}
		}
	}
	usleep(2000000);
	game->Tab[pos] = 0;
	info.info = 0; info.tipo = 2;
	info.x = x; info.y = y;
	escCliente4(game, &info); 

	upLock = FALSE; downLock = FALSE; leftLock = FALSE; rightLock = FALSE;
 
	for (int i = 1; i < n; i++)
	{
		if (game->Tab[pos-(30*i)] !=1 && game->Tab[pos-(30*i)] != 8 && game->Tab[pos-(30*i)] !=9 && game->Tab[pos-(30*i)] != 11 && game->Tab[pos-(30*i)] != 12 && upLock != TRUE)
		{
			game->Tab[pos-(30*i)] = 0;
			info.x = x + 0;
			info.y = y - 1 * i;
			info.info = 0; info.tipo = 2;
			escCliente4(game, &info);
		}else
			upLock = TRUE;
		if (game->Tab[pos+(30*i)] !=1 && game->Tab[pos+(30*i)] !=8 && game->Tab[pos+(30*i)] !=9 && game->Tab[pos+(30*i)] !=11 && game->Tab[pos+(30*i)] !=12 && downLock != TRUE)
		{
			game->Tab[pos+(30*i)] = 0;
			info.x = x + 0;
			info.y = y + 1 * i;
			info.info = 0; info.tipo = 2;
			escCliente4(game, &info);
		}else
			downLock = TRUE;
		if (game->Tab[pos-(1*i)] !=1 && game->Tab[pos-(1*i)] !=8 && game->Tab[pos-(1*i)] !=9 && game->Tab[pos-(1*i)] !=11 && game->Tab[pos-(1*i)] !=12 && leftLock != TRUE)
		{
			game->Tab[pos-(1*i)] = 0;
			info.x = x - 1 * i;
			info.y = y + 0 * i;
			info.info = 0; info.tipo = 2;
			escCliente4(game, &info);
		}else
			leftLock = TRUE;
		if (game->Tab[pos+(1*i)] !=1 && game->Tab[pos+(1*i)] !=8 && game->Tab[pos+(1*i)] !=9 && game->Tab[pos+(1*i)] !=11 && game->Tab[pos+(1*i)] !=12 &&rightLock != TRUE)
		{
			game->Tab[pos+(1*i)] = 0;
			info.x = x + 1 * i;
			info.y = y + 0 * i;
			info.info = 0; info.tipo = 2;
			escCliente4(game, &info);
		}else
			rightLock = TRUE;
	}
	
	for (int i = 0; i < posiContador; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (posicoes[i] == game->jogadores[j].p)
			{
				game->Tab[posicoes[i]] = game->jogadores[j].cara * 10;
				info.cara = game->jogadores[j].cara;
				info.info = 0; info.tipo = 0;
				info.antx = game->jogadores[j].antx; info.x = game->jogadores[j].x; info.anty = game->jogadores[j].anty; info.y = game->jogadores[j].y;
				escCliente4(game, &info);
			}
		}
		if (vPosicoes[i] == 2 && i != 0)
		{
			int prob = rand()%10;
			if (prob >3)
			{
				drop(game, posicoes[i]);
				info.info = 0; info.tipo = game->Tab[posicoes[i]];
				info.x = xPosicoes[i];
				info.y = yPosicoes[i];
				escCliente4(game, &info);
			}
			
		}
	}

	game->jogadores[lugar].bombaLock = FALSE;
}

void *contralaBot(void *dados)
{
	Labirinto *game = (Labirinto *) dados;
	Informacao info;
	int ch;
	int lugar;
	bool moveLock = FALSE;
	pthread_mutex_lock(&trinco);
	for (int i = 0; i < 20; ++i)
	{
		if (game->inimigos[i].vida =TRUE && game->inimigos[i].bot == FALSE)
		{
			lugar = i;
			game->inimigos[lugar].bot = TRUE;

			break;
		}
	}
	
	printf("O inimigo %d iniciou com sucesso\n", lugar);

	pthread_mutex_unlock(&trinco);
	do{
		usleep(500000);
		game->inimigos[lugar].antp = game->inimigos[lugar].p;
		info.antx = game->inimigos[lugar].antx = game->inimigos[lugar].x;
		info.anty = game->inimigos[lugar].anty = game->inimigos[lugar].y;
		if (game->Tab[game->inimigos[lugar].p] == 7)
		{
			game->inimigos[lugar].bot = FALSE;
			info.info = 0; info.tipo = 2;
		}else{
			ch = rand()%4;
			switch(ch){
				case 0:
					if (verificaRegrasDirecao(game->inimigos[lugar].p, 30) == TRUE)
					{
						if (verificaRegraPosicaoB(game, game->inimigos[lugar].p + 30) == TRUE)
						{
							game->inimigos[lugar].p = game->inimigos[lugar].p + 30;
							game->inimigos[lugar].y = game->inimigos[lugar].p / 30;
							game->Tab[game->inimigos[lugar].antp] = 0;
							game->Tab[game->inimigos[lugar].p] = 4;
						}
					}
					break;
				case 1:
					if (verificaRegrasDirecao(game->inimigos[lugar].p, -30) == TRUE)
					{
						if (verificaRegraPosicaoB(game, game->inimigos[lugar].p - 30) == TRUE)
						{
							game->inimigos[lugar].p = game->inimigos[lugar].p - 30;
							game->inimigos[lugar].y = game->inimigos[lugar].p / 30;
							game->Tab[game->inimigos[lugar].antp] = 0;
							game->Tab[game->inimigos[lugar].p] = 4;
						}
					}
					break;
				case 2:
					if (verificaRegrasDirecao(game->inimigos[lugar].p, -1) == TRUE)
					{
						if (verificaRegraPosicaoB(game, game->inimigos[lugar].p -1) == TRUE)
						{
							game->inimigos[lugar].p = game->inimigos[lugar].p - 1;
							game->inimigos[lugar].x--;
							game->Tab[game->inimigos[lugar].antp] = 0;
							game->Tab[game->inimigos[lugar].p] = 4;
						}
					}
					break;
				case 3:
					if (verificaRegrasDirecao(game->inimigos[lugar].p, 1) == TRUE)
					{
						if (verificaRegraPosicaoB(game, game->inimigos[lugar].p + 1) == TRUE)
						{
							game->inimigos[lugar].p = game->inimigos[lugar].p + 1;
							game->inimigos[lugar].x++;
							game->Tab[game->inimigos[lugar].antp] = 0;
							game->Tab[game->inimigos[lugar].p] = 4;

						}
					}
					break;
			}
			info.info = 0; info.tipo = 1;
		}
		
		info.x = game->inimigos[lugar].x;
		info.y = game->inimigos[lugar].y;
		printf("Sou %d ", lugar);
		printf("%d ", game->inimigos[lugar].antp);
		printf("%d\n", game->inimigos[lugar].p);
		escCliente4(game, &info);

	}while(game->inimigos[lugar].bot == TRUE);

	drop(game, game->inimigos[lugar].p);
	info.info = 0; info.tipo = game->Tab[game->inimigos[lugar].p];
	escCliente4(game, &info);

	printf("O inimigo %d desligou com sucesso\n", lugar);
}

void shutdown(int signal)
{
  	if (signal == SIGUSR1 || signal == SIGINT)
  	{
  		for (int i = 0; i < 20; ++i)
		{
			if(cliShutdown[i] != 0)
			{
				kill(cliShutdown[i], SIGUSR1);
			}
		}
		unlink(FIFO);
  		printf("\nO servidor fez Shutdown com sucesso\n");
  	}

	exit(0);
}

void main(int argc, char **argv)
{
	int fdServidor, nob;
	char comando[100], aux[100], ficheiro_cliente[100] = "Clientes.txt";
	int d = 0, tam = 0, confirma = 0;
	char sair_loop[10];
	int new_file = 0;
	
	//var select
	fd_set read_fd;
	struct timeval tv;
	int ret;
	
	Clientes cli, lCli[20];

	Cli array = NULL;
	Labirinto game;


	pthread_t inimigoPT[20];

	preparaJogador(&game, 0, 0);

	setbuf(stdout, NULL);
	//Inicia o SIGUSR1 e verifica
	if(signal(SIGUSR1,shutdown) == SIG_ERR)
	{
		printf("Erro a tratar do SIGUSR1\n");
		exit(0);
	}
	//Inicia o SIGINT e verifica
	if(signal(SIGINT, shutdown) == SIG_ERR)
	{
		printf("Erro a tratar do SIGINT\n");
		exit(0);
	}
	//Verifica se ja existe um servidor
	if(access(FIFO, F_OK) == 0){
		printf("Ja existe um servidor\n");
		exit(0);
	}

	//Pede nome do ficheiro com osa utilizadores
	do{
		new_file = 0;
		printf("Bem Vindo! Diga o nome do ficheiro de utilizadores: ");
		scanf(" %99[^\n]", ficheiro_cliente);
		if (access(ficheiro_cliente, F_OK) == -1) {
			printf("Nao reconhecido o nome do ficheiro.\nDeseja avancar?[sim/nao]\n");
			scanf(" %9[^\n]", sair_loop);
			new_file = 1;
			
		}else {

			array = LerFicheiro_Clientes(ficheiro_cliente, &tam);

			if (array == NULL) {
				printf("Erro a Ler ficheiro\n");
			}
			break;

		}
	}while (strcmp("sim", sair_loop) != 0);
	
	//Prepare o nome e pid do pCli
	for(int i = 0; i <20; i++){
		strcpy(lCli[i].nome, "vazio");
		lCli[i].pid = 0;
		cliShutdown[i]=0;
	}
	
	//Iniciar servidor
	mkfifo(FIFO, 0600);
	fdServidor = open(FIFO, O_RDWR);


	do {
		
		//Preparar Select
		FD_ZERO(&read_fd);
		FD_SET(0, &read_fd);
		FD_SET(fdServidor, &read_fd);
		tv.tv_sec = 5;
		tv.tv_usec = 5;
		
		ret = select(fdServidor+1, &read_fd, NULL, NULL, &tv);
		
		limpa(aux);

		if(ret == 0){ 
			fflush(stdout);
		}
		else if(ret > 0)
		{
			if(FD_ISSET(0, &read_fd))
			{
				scanf(" %99[^\n]", comando);

				if (strcmp(comando, "users") == 0) {
					printf("Users Ligados ao Servidor:\n");
					for (int i = 0; i < 20; ++i)
					{
						if (lCli[i].pid != 0 && strcmp(lCli[i].nome, "vazio") !=0)
						{
							printf("- %s\n", lCli[i].nome);
						}else if (lCli[i].pid != 0 && strcmp(lCli[i].nome, "vazio") ==0)
						{
							printf("- %s, ainda nao efectuou login\n", lCli[i].nome);
						}
					}

				}
				else if (strcmp(comando, "game") == 0) {
					//falta implementar
					printf("game\n");

				}
				else if (strcmp(comando, "shutdown") == 0) {
					for (int i = 0; i < 20; ++i)
					{
						if(lCli[i].pid != 0)
						{
							kill(lCli[i].pid, SIGUSR1);
						}
					}
				}
				else {
					limpa(aux);
					Seperar(comando, aux, &d);
					if (strcmp(aux, "add") == 0) {
						char password[20] = { "" }, username[20] = { "" };

						Seperar(comando, username, &d);
						
						if(strcmp("add",username) == 0){
							confirma = 1;
						}else{
							printf("username: %s\n", username);
						}

						for (int i = 0; i<tam; i++) {
							if (strcmp(array[i].nome, username) == 0) {
								printf("Já existe esse utilizador %s!\n", username);
								confirma = 1;
							}
						}


						//ALOCAR MEMORIA
						if (confirma != 1) {
							Seperar(comando, password, &d);
							printf("password: %s\n", password);

							tam++;
							array = realloc(array, tam * sizeof(Clientes));
							if (array == NULL) {
								printf("Erro a alocar memoria!\n");
								tam--;
							}


							strcpy((array + (tam - 1))->nome, username);
							strcpy((array + (tam - 1))->password, password);
						}

						d = 0;
						confirma = 0;
					}
					else if (strcmp(aux, "map") == 0) {
						char numero[20] = { "" }, np[20];
						Seperar(comando, numero, &d);
						if (strcmp(numero, "1")==0)
						{
							int tempx = 0;
							int tabTemp[600] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
												1,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,1,
												1,0,1,2,1,2,1,1,2,1,2,1,2,0,0,0,2,1,2,1,2,1,1,2,1,2,1,0,0,1,
												1,2,2,3,2,2,4,0,2,0,2,0,2,1,1,1,2,0,2,0,2,0,4,2,2,3,2,2,1,1,
												1,2,1,2,1,2,0,0,2,1,2,1,2,1,1,1,2,1,2,1,2,0,0,2,1,2,1,2,0,1,
												1,2,2,2,2,2,1,1,2,0,2,0,2,0,2,0,2,0,2,0,2,1,1,2,2,2,2,2,1,1,
												1,2,1,0,1,2,1,1,2,1,2,1,2,0,2,0,2,1,2,1,2,1,1,2,1,0,1,2,0,1,
												1,2,0,0,0,2,2,0,0,0,2,0,2,0,0,0,2,0,2,0,0,0,2,2,0,0,0,2,1,1,
												1,2,1,0,1,0,0,2,2,1,2,1,1,1,0,1,1,1,2,1,2,2,0,0,1,0,1,2,2,1,
												1,2,0,2,0,0,1,1,2,2,2,2,1,0,0,0,1,2,2,2,2,1,1,0,0,2,0,2,2,1,
												1,2,0,2,0,0,1,1,2,2,2,2,1,0,0,0,1,2,2,2,2,1,1,0,0,2,0,2,2,1,
												1,2,1,0,1,0,0,2,2,1,2,1,1,1,1,1,1,1,2,1,2,2,0,0,1,0,1,2,2,1,
												1,2,0,0,0,2,2,0,0,0,2,0,2,0,2,0,2,0,2,0,0,0,2,2,0,0,0,2,1,1,
												1,2,1,0,1,2,1,1,2,1,2,1,2,2,2,2,2,1,2,1,2,1,1,2,1,0,1,2,0,1,
												1,2,2,2,2,2,1,1,2,0,2,0,2,0,2,0,2,0,2,0,2,1,1,2,2,2,2,2,1,1,
												1,2,1,2,1,2,0,0,2,1,2,1,2,1,1,1,2,1,2,1,2,0,0,2,1,2,1,2,0,1,
												1,2,2,3,2,2,4,0,2,0,2,0,2,1,1,1,2,0,2,0,2,0,4,2,2,3,2,2,1,1,
												1,0,1,2,1,2,1,1,2,1,2,1,2,0,0,0,2,1,2,1,2,1,1,2,1,2,1,0,0,1,
												1,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,1,
												1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
										};

							NOBJECT = 0;
							NENEMY = 0;
							for (int i = 0; i < 600; i++)
							{
								game.Tab[i] = tabTemp[i];
								if (game.Tab[i] == 4)
								{
									game.inimigos[NENEMY].antx = game.inimigos[NENEMY].x = tempx;
									game.inimigos[NENEMY].anty = game.inimigos[NENEMY].y = i/30;
									game.inimigos[NENEMY].antp = game.inimigos[NENEMY].p = i;
									game.inimigos[NENEMY].vida = TRUE;
									NENEMY++;
								}
								if(game.Tab[i] == 3)
								{
									NOBJECT++;
								}
								tempx++;
								if (tempx >29)
								{
									tempx = 0;
								}
								/*printf("%d", tabTemp[i]);
								if ((i+1)%30 == 0 && i!=0)
								{
									printf("\n");
								}
								*/
							}
							game.pronto = TRUE;
							printf("\nMapa 1 carregado com sucesso\n");
						}
					}
					else if (strcmp(aux, "kick") == 0) {
						//fazer kick
						char user[20] = { "" }, np[20];
						Seperar(comando, user, &d);

						if(strcmp("kick",user) != 0){
							int login = posicaoLogin(user, lCli);
							if (login != 20)
							{
								strcpy(lCli[login].nome, "vazio");
								strcpy(lCli[login].password, "vazio");
								NMAXPLAY--;
								kill(lCli[login].pid, SIGUSR1);
								lCli[login].pid = 0;
								cliShutdown[login] = 0;
								printf("Foi retirado o Cliente %s.\n",user);
								d = 0;
							}
							else{
								printf("O Cliente %s nao se encontra ligado ao Servidor\n", user);
							}
							
							d = 0;
						}
			   		}else {
						printf("Comando Incorreto\n");
						printf("Comandos existentes:\n");
						printf("\t- users\n");
						printf("\t- game\n");
						printf("\t- add <username> <password>\n");
						printf("\t- kick <username>\n");
						printf("\t- map <nome-ficheiro>\n");
						printf("\t- shutdown\n");
					}
				}
				d = 0;		
			}
			else if(FD_ISSET(fdServidor, &read_fd))
			{
				nob = read(fdServidor, &cli, sizeof(cli));
				if(nob==sizeof(cli))
				{
					if(strcmp(cli.cmd, "verifica")==0)
					{
						if(NMAXPLAY > 20)
						{
							strcpy(cli.msg1,"O servidor encontra-se cheio, tente mais tarde.");
						}
						else{
							strcpy(cli.cmd,"login");
							for (int i = 0; i < 20; ++i)
							{
								if(lCli[i].pid == 0)
								{
									lCli[i].pid = cli.pid;
									cliShutdown[i] = cli.pid;
									NMAXPLAY ++;
									printf("\nRecebi o pid %d.\n", lCli[i].pid);
									break;
								}
							}
						}
						escCliente(&cli);
					}
					else if(strcmp(cli.cmd, "login") == 0)
					{
						if (array!=NULL)
						{
							for(int x = 0; x<tam; x++)
							{
								if(strcmp(array[x].nome, cli.nome)==0)
								{
									int login = posicaoLogin(cli.nome, lCli);
									if(login != 20)
									{
										strcpy(cli.msg1,"O utilizador ja se encontra com o login efectuado");
									}else if (strcmp(array[x].password, cli.password) == 0)
									{
											int insere = posicaoPid(cli.pid, lCli);
											strcpy(cli.cmd, "entrou");
											strcpy(lCli[insere].nome, cli.nome);
											strcpy(lCli[insere].password, cli.password);
											strcpy(cli.msg1,"Bem Vindo !!!");
											printf("\nEntrou o Cliente %s com o pid %d\n", cli.nome, cli.pid);
									}else{
										strcpy(cli.msg1,"A password esta errada.");
									}
									break;
								}
								else if(x == tam-1)
								{
									strcpy(cli.msg1,"O nome de utilizador nao se encontra na lista de utilizadores.");
								}
							}
						}else{
							strcpy(cli.msg1,"O nome de utilizador nao se encontra na lista de utilizadores.");
						}
						escCliente(&cli);							
					}
					else if (strcmp(cli.cmd,"iniJogo")==0)
					{
						if (game.pronto == TRUE)
						{
							strcpy(cli.cmd, "iniPronto");
						}else{
							strcpy(cli.msg1, "O servidor ainda nao carregou nenhum mapa");
						}
						escCliente(&cli);
					}
					else if(strcmp(cli.cmd,"iniPronto")==0)
					{
						Informacao tempInfo;

						int i = 0;
						for (i = 0; i < 20; i++)
						{
							if (game.jogadores[i].pid == 0)
							{
								game.jogadores[i].pid = cli.pid;
								break;
							}
						}
						
						if (game.run == FALSE)
						{

							pthread_mutex_init(&trinco, NULL);
							for (int j = 0; j < 20; j++)
							{
								if (game.inimigos[j].vida == TRUE)
								{
									pthread_create(&inimigoPT[j], NULL, &contralaBot, (void *) &game);
								
								}
							}
							preparaJogador(&game, 1, i);
														
						}else{
							preparaJogador(&game, 2, i);
						}
						escCliente2(game.Tab, cli.pid);
						tempInfo.info = 0; tempInfo.tipo = 0;
						tempInfo.cara = game.jogadores[i].cara;
						tempInfo.x = game.jogadores[i].x;
						tempInfo.y = game.jogadores[i].y;
						tempInfo.antx = game.jogadores[i].antx;
						tempInfo.anty = game.jogadores[i].anty;
						escCliente4(&game, &tempInfo);
					}
					else if (strcmp(cli.cmd,"info")==0)
					{
						printf("1\n");
						int lugar = getJogador(game, cli.pid);
						printf("2\n");

						Informacao info;
						info.info = 0;
						info.cara = game.jogadores[lugar].cara;
						
						if (cli.info == 27)//Sair do jogo
						{
							info.info = 0; info.tipo = 2;
							info.sair = TRUE;
							info.x = game.jogadores[lugar].x;
							info.y = game.jogadores[lugar].y;
							preparaJogador(&game, 3, lugar);
							escCliente3(&info, cli.pid);
							info.sair = FALSE;
							estadoJogo(&game, 1);
						}
						else if (cli.info == 258 ||cli.info == 259 ||cli.info == 260 ||cli.info == 261)//Movimento
						{
							bool moveLock = FALSE, stdLock = TRUE;
							game.jogadores[lugar].antp = game.jogadores[lugar].p;
							info.antx = game.jogadores[lugar].antx = game.jogadores[lugar].x;
							info.anty = game.jogadores[lugar].anty = game.jogadores[lugar].y;
							if(cli.info == 258 && verificaRegrasDirecao(game.jogadores[lugar].p, 30) == TRUE)//Movimento para cima
							{
								if (verificaRegraPosicao(&game, game.jogadores[lugar].p + 30) == TRUE)
								{									
									game.jogadores[lugar].p = game.jogadores[lugar].p + 30;
									game.jogadores[lugar].y = game.jogadores[lugar].p / 30;
									moveLock =TRUE;
								}
							}else if(cli.info == 259 && verificaRegrasDirecao(game.jogadores[lugar].p, -30) == TRUE)//Movimento para baixo
							{
								if (verificaRegraPosicao(&game, game.jogadores[lugar].p - 30) == TRUE)
								{
									game.jogadores[lugar].p = game.jogadores[lugar].p - 30;
									game.jogadores[lugar].y = game.jogadores[lugar].p / 30;
									moveLock =TRUE;
								}
							}else if(cli.info == 260 && verificaRegrasDirecao(game.jogadores[lugar].p, -1) == TRUE)//Movimento para esquerda
							{
								if (verificaRegraPosicao(&game, game.jogadores[lugar].p - 1) == TRUE)
								{
									game.jogadores[lugar].p = game.jogadores[lugar].p -1;
									game.jogadores[lugar].x--;
									moveLock =TRUE;
								}
							}else if(cli.info == 261 && verificaRegrasDirecao(game.jogadores[lugar].p, 1) == TRUE)//Movimento para direita
							{
								if (verificaRegraPosicao(&game, game.jogadores[lugar].p + 1) == TRUE)
								{
									game.jogadores[lugar].p = game.jogadores[lugar].p +1;
									game.jogadores[lugar].x++;
									moveLock =TRUE;
								}
							}
							if (game.Tab[game.jogadores[lugar].antp] == 5 || game.Tab[game.jogadores[lugar].antp] == 6)//Deixa as Bombas
							{
								info.antx = game.jogadores[lugar].antx = game.jogadores[lugar].x;
								info.anty = game.jogadores[lugar].anty = game.jogadores[lugar].y;
							}
							if (game.Tab[game.jogadores[lugar].p] == 3)
							{
								info.atualiza = game.jogadores[lugar].pontos = game.jogadores[lugar].pontos + 100;
								info.info = 2;
								game.fim++;
								escCliente3(&info, cli.pid);
								info.atualiza = game.fim;
								info.info = 3;
								escCliente4(&game, &info);
								estadoJogo(&game, 2);
							}else if (game.Tab[game.jogadores[lugar].p] == 8)
							{
								info.atualiza = ++game.jogadores[lugar].bombas;
								info.info = 4;
								escCliente3(&info, cli.pid);
							}else if (game.Tab[game.jogadores[lugar].p] == 9)
							{
								info.atualiza = ++game.jogadores[lugar].mBombas;
								info.info = 5;
								escCliente3(&info, cli.pid);
							}else if (game.Tab[game.jogadores[lugar].p] == 11)
							{
								info.atualiza = ++game.jogadores[lugar].vidas;
								info.info = 1;
								escCliente3(&info, cli.pid);
							}else if (game.Tab[game.jogadores[lugar].p] == 12)
							{
								info.atualiza = game.jogadores[lugar].pontos = game.jogadores[lugar].pontos + 10;
								info.info = 2;
								escCliente3(&info, cli.pid);
							}

							if (moveLock == TRUE)
							{
								if (game.Tab[game.jogadores[lugar].antp] >= 10)
								{
									game.Tab[game.jogadores[lugar].antp] = 0;
								}
								if (game.Tab[game.jogadores[lugar].p] == 7)
								{
									info.atualiza = --game.jogadores[lugar].vidas;
									info.info = 1;
									if (game.jogadores[lugar].vidas < 0)
									{
										preparaJogador(&game, 3, lugar);
										info.fim = TRUE;
										escCliente3(&info, cli.pid);
										info.fim = FALSE;
										estadoJogo(&game, 1);
									}else 
										escCliente3(&info, cli.pid);
									if (game.Tab[game.jogadores[lugar].antp] == 7 || game.Tab[game.jogadores[lugar].antp] == 5 ||game.Tab[game.jogadores[lugar].antp] == 6)
									{
										info.info = 0; info.tipo = 5;
									}else 
										info.info = 0; info.tipo = 6;
									
									stdLock = FALSE;
								}
								else {
									game.Tab[game.jogadores[lugar].p] = game.jogadores[lugar].cara * 10;
								}
								
							}
							if (stdLock == TRUE)
							{
								info.info = 0; info.tipo = 0;
							}							
							info.x = game.jogadores[lugar].x;
							info.y = game.jogadores[lugar].y;

							if (game.final == TRUE)
							{
								if (game.jogadores[lugar].p == 358 || game.jogadores[lugar].p == 328 || game.jogadores[lugar].p == 298 || game.jogadores[lugar].p == 268)
								{
									game.vitoria = TRUE;
									estadoJogo(&game, 0);
								}
							}

						}
						else if (cli.info == 1)//usar bomba
						{
							if (game.jogadores[lugar].bombas > 0 && game.jogadores[lugar].bombaLock == FALSE)
							{
								game.Tab[game.jogadores[lugar].p] = 5;
								info.atualiza = --game.jogadores[lugar].bombas;
								info.x = game.jogadores[lugar].x;
								info.y = game.jogadores[lugar].y;
								game.nBomba = 1;
								game.pBomba = game.jogadores[lugar].p;
								game.bPid = cli.pid;
								game.jogadores[lugar].bombaLock = TRUE;
								pthread_create(&(game.jogadores[lugar].bombaPT), NULL, &controlaBombas, (void *) &game);
								info.info = 4;
								escCliente3(&info, cli.pid);
								info.info = 0; info.tipo = 3;
							}
						}
						else if (cli.info == 2)//usar MegaBomba
						{
							if (game.jogadores[lugar].mBombas > 0 && game.jogadores[lugar].bombaLock == FALSE)
							{
								game.Tab[game.jogadores[lugar].p] = 6;
								info.atualiza = --game.jogadores[lugar].mBombas;
								info.x = game.jogadores[lugar].x;
								info.y = game.jogadores[lugar].y;
								game.nBomba = 2;
								game.pBomba = game.jogadores[lugar].p;
								game.bPid = cli.pid;
								game.jogadores[lugar].bombaLock = TRUE;
								pthread_create(&(game.jogadores[lugar].bombaPT), NULL, &controlaBombas, (void *) &game);
								info.info = 5;
								escCliente3(&info, cli.pid);
								info.info = 0; info.tipo = 4;
							}
						}
						printf("3\n");
						escCliente4(&game, &info);
						printf("4\n");

						
					}
					else if(strcmp(cli.cmd,"quit")==0)
					{
						preparaJogador(&game, 4, cli.pid);
						int login = posicaoPid(cli.pid,lCli);
						strcpy(cli.nome, lCli[login].nome);
						strcpy(lCli[login].nome, "vazio");
						strcpy(lCli[login].password, "vazio");
						lCli[login].pid = 0;
						cliShutdown[login] = 0;
						NMAXPLAY--;
						printf("\nSaiu o Cliente %s com o pid %d\n", cli.nome, cli.pid);				
					}
				}
			}
		}
		d = 0;

	} while (strcmp(comando, "shutdown") != 0);

	GravaFicheiro_Clientes(ficheiro_cliente, tam, array, new_file);
	
	close(fdServidor);
	unlink(FIFO);
	
	printf("Os clientes e o servidor foram terminados com sucesso\n");
	exit(0);
}