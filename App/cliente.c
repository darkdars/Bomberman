#include "servidorUtil.h"

int x, y, pontos;

void leCliente(Clientes *pCli)
{
	Clientes tempCli;
	int fdCliente, nob=0;
	char np[20];
	sprintf(np, "np%d", getpid());
	fdCliente = open(np, O_RDONLY);
	read(fdCliente, &tempCli, sizeof(tempCli));
	close(fdCliente);
	*pCli=tempCli;
}
//Recebe e imprime o tabuleiro inicial
void leCliente2()
{
	int tempLab[600];
	int fdCliente;
	char np[20];
	sprintf(np, "np%d", getpid());
	fdCliente = open(np, O_RDONLY);
	read(fdCliente, tempLab, sizeof(int)*600);
	close(fdCliente);
	clear();
	y = (LINES-4)/2;
	x = (COLS-20)/2;
	int l = 2, c = 2;
	for (int i = 0; i < 600; i++)
	{
		if (tempLab[i]==0)//casa vazia
		{
			color_set(1, NULL);
			mvaddstr (l, c, " ");
  		}else if (tempLab[i]==1)// parede
		{
		  	color_set(3, NULL);
		  	mvaddstr (l, c, " ");
		}else if (tempLab[i]==2)//destrutivel
		{
		  	color_set(4, NULL);
		  	mvaddstr (l, c, " ");
		}else if (tempLab[i]==3)//Objecto
		{
		  	color_set(5, NULL);
		  	mvaddstr (l, c, " ");
		}else if (tempLab[i]==4)//inimigo
		{
			color_set(6 ,NULL);
			mvaddstr(l, c, "0");
		}else if(tempLab[i]==5)//bomba
		{
			color_set(7 ,NULL);
			mvaddstr(l, c, " ");
		}else if(tempLab[i]==6)//Mega Bomba
		{
			color_set(7 ,NULL);
			mvaddstr(l, c, "-");
		}else if(tempLab[i]==7)//Explosao
		{
			color_set(7 ,NULL);
			mvaddstr(l, c, " ");
		}else if(tempLab[i]>=10)//jogador
		{
			color_set(2 ,NULL);
			mvprintw(l, c, "%d", tempLab[i]/10);
		}
		c++;

		if ((i+1)%30 == 0 && i!=0)
		{
			l++;
			c = 2;
		}
	}
	color_set(1, NULL);
	mvaddstr(5, 35, "Jogador  :");
	mvprintw(5, 46, "%d", tempLab[284]/10);
	mvaddstr(6, 35, "Vida     : 3");
	mvaddstr(8, 35, "Pontuacao: 0");
	mvaddstr(9, 35, "Objectos : 0/4");
	mvaddstr(11,35, "Bombas   : 3");
	mvaddstr(12,35, "MegaBomba: 2");

	refresh();
}
//Envia Informação ao Servidor
void escServidor(Clientes *pCli)
{
	Clientes tempCli = *pCli;
	int fdServidor;
	fdServidor = open(FIFO, O_WRONLY);
	write(fdServidor, &tempCli, sizeof(tempCli));
	close(fdServidor);
}
//Trata do Login do Cliente
void loginFunc(Cli pCli)
{
  	int INPUT=0;


	do{
		y = (LINES-4)/2;
  		x = (COLS-20)/2;
		color_set(1, NULL);
	    mvaddstr (y-2, x, "login:");
	    mvaddstr (y+0, x, "  Utilizador:");
	    mvaddstr (y+1, x, "  Palavra-passe:");
	    mvgetstr (y+0, x, pCli->nome );
	    mvprintw (y+0, x + 15, pCli->nome);
	    mvgetstr (y+1, x, pCli->password );
	    mvprintw (y+1, x + 18, pCli->password); 
		escServidor(pCli);
		leCliente(pCli);
		if (strcmp(pCli->cmd, "entrou") != 0)
		{
			mvprintw (y+3, x+2, "%s", pCli->msg1);
	    	color_set(2, NULL);
	    	mvaddstr (y+5, x, "Sair [PRESSIONE ESC]");
	    	color_set(1,NULL);
	    	INPUT = getch();
	    	if (INPUT==27)
	    	{
	    		strcpy(pCli->cmd, "quit");
	    	}

		}

		clear();
	    refresh();
	    fflush(stdin);
	}while(strcmp(pCli->cmd, "entrou") != 0 && strcmp(pCli->cmd, "quit") != 0);
}
//Controla a informação recebida pelo servidor durante a fase de Jogo
void *recebe_msg(void *dados)
{
	Cli pCli = (Clientes*)dados;
	Informacao info;
	
	int fdCliente;
	char np[20];
	sprintf(np, "np%d",pCli->pid);

	fdCliente = open(np, O_RDONLY);
	do{
		refresh();
	    read(fdCliente, &info, sizeof(info));
	    if (info.info == 0)
	    {
	    	if (info.tipo == 0)//Atualiza a Posicao dos Jogadores
	    	{
	    		color_set(2, NULL);
		    	mvaddstr (info.anty + 2, info.antx + 2, " ");
		    	mvprintw (info.y + 2, info.x + 2,"%d",info.cara);
	    	}else if (info.tipo == 1)//Atualiza posicao dos inimigos
	    	{
	    		color_set(6, NULL);
		    	mvaddstr (info.anty+2, info.antx+2, " ");
		    	mvaddstr (info.y+2, info.x+2, "0");
	    	}else if (info.tipo == 2)//Tira Jogadore, Objectos e Destrutiveis do mapa
	    	{
	    		color_set(1, NULL);
		    	mvaddstr (info.y+2, info.x+2, " ");
	    	}else if (info.tipo == 3)//Mete bomba no mapa
	    	{
	    		color_set(7, NULL);
	    		mvaddstr (info.y+2, info.x+2, " ");
	    	}else if (info.tipo == 4)//Mete bomba no mapa
	    	{
	    		color_set(7, NULL);
	    		mvaddstr (info.y+2, info.x+2, "-");
	    	}else if (info.tipo == 5)//Mete Jogador em cima de explosao
	    	{
	    		color_set(8, NULL);
	    		mvaddstr (info.anty+2, info.antx+2, " ");
	    		mvprintw (info.y+2, info.x+2, "%d", info.cara);
	    	}else if (info.tipo == 6)//Mete Jogador em cima de explosao
	    	{
	    		color_set(2, NULL);
	    		mvaddstr (info.anty+2, info.antx+2, " ");
	    		color_set(8, NULL);
	    		mvprintw (info.y+2, info.x+2, "%d", info.cara);
	    	}else if (info.tipo == 7)//Mete a saida
	    	{
	    		color_set(2, NULL);
	    		mvaddstr (info.anty+2, info.antx+2, " ");
	    		color_set(8, NULL);
	    		mvprintw (info.y+2, info.x+2, "%d", info.cara);
	    	}else if (info.tipo == 8)//BombaDrop
	    	{
	    		color_set(9, NULL);
	    		mvaddstr (info.y+2, info.x+2, " ");
	    	}else if (info.tipo == 9)//MBombaDrop
	    	{
	    		color_set(9, NULL);
	    		mvaddstr (info.y+2, info.x+2, "-");
	    	}else if (info.tipo == 11)//VidaDrop
	    	{
	    		color_set(9, NULL);
	    		mvaddstr (info.y+2, info.x+2, "V");
	    	}else if (info.tipo == 12)//PontosDrop
	    	{
	    		color_set(9, NULL);
	    		mvaddstr (info.y+2, info.x+2, "P");
	    	}
	    }else if (info.info == 1)//Atualiza vida
	    {
	    	color_set(1, NULL);
	    	mvprintw (6, 46, "%d", info.atualiza);	
	    }else if (info.info == 2)//Atualiza Pontuacao
	    {
	    	color_set(1, NULL);
	    	mvprintw (8, 46, "%d", info.atualiza);
	    	pontos = info.atualiza;
	    }else if (info.info == 3)//Atualiza Objectos apanhados 
	    {
	    	color_set(1, NULL);
	    	mvprintw (9, 46, "%d", info.atualiza);
	    }else if (info.info == 4)//Atualiza Bombas
	    {
	    	color_set(1, NULL);
	    	mvprintw (11, 46, "%d", info.atualiza);
	    }else if (info.info == 5)//Atualiza MegaBombas
	    {
	    	color_set(1, NULL);
	    	mvprintw (12, 46, "%d", info.atualiza);
	    }else if (info.info == 6)//Mostra Saida
	    {
	    	color_set(10, NULL);
	    	mvaddstr (8+2, 28+2, " ");
	    	mvaddstr (9+2, 28+2, " ");
	    	mvaddstr (10+2, 28+2, " ");
	    	mvaddstr (11+2, 28+2, " ");
	    }
		refresh();
	}while(info.fim != TRUE && info.sair != TRUE && info.vitoria != TRUE);
	if (info.sair == TRUE)
	{
		strcpy(pCli->cmd, "leave");
	}else if (info.fim == TRUE)
	{
		strcpy(pCli->cmd, "derrota");
	}else if (info.vitoria == TRUE)
	{
		strcpy(pCli->cmd, "vitoria");
	}
	close(fdCliente);
	refresh();
}
//Controla o envio de informação para o servidor durante a fase de Jogo
void gameControls(Cli pCli)
{
	int INPUT=0;
	pthread_t tarefa;

	bool lock = TRUE;
	char jogada;

	clear();
	escServidor(pCli);
	leCliente2();
	pthread_create(&tarefa, NULL, &recebe_msg, (void*) pCli);

	strcpy(pCli->cmd, "info");

	do{
		if (lock==TRUE)
		{
			INPUT = getch();
			jogada = INPUT;
		}
		switch(jogada){
			case 'n': pCli->info = 1; break;
			case 'm': pCli->info = 2; break;
		}
	    if (INPUT == 27 && lock == TRUE){
    		pCli->info = 27;
    		escServidor(pCli);
    		lock = FALSE;
	    }
	    else if (INPUT == KEY_DOWN)
	    	pCli->info = 258;
	    else if (INPUT == KEY_UP)
	    	pCli->info = 259;
	    else if (INPUT == KEY_LEFT)
	    	pCli->info = 260;
	    else if (INPUT == KEY_RIGHT)
	    	pCli->info = 261;

	    if (lock == TRUE && strcmp(pCli->cmd, "info")==0)
	    {
	    	escServidor(pCli);
	    	pCli->info = 0;
	    }
		
	}while(strcmp(pCli->cmd, "leave")!=0 && strcmp(pCli->cmd, "derrota")!=0 && strcmp(pCli->cmd, "vitoria")!=0);
	pthread_join(tarefa, NULL);
	
	clear();
}
// Creditos do Jogo
void creditos_func()
{
	int INPUT;

	do{

		y = (LINES-4)/2;
  		x = (COLS-20)/2;
		clear();
		refresh();
    	color_set(2, NULL);
      	mvaddstr (y+0, x-3, "   Trabalho realizado por:");
    	color_set(1, NULL);
    	mvaddstr (y+2, x-3, ">      Carlos Santana");
      	mvaddstr (y+3, x-3, "         Nº21240449");
      	mvaddstr (y+4, x-3, ">        Hugo Silva");
      	mvaddstr (y+5, x-3, "         Nº21240009");
      	INPUT = getch();

  	}while(INPUT != '\n' && INPUT != ' ' && INPUT != 27 );

	color_set(1,NULL);
	refresh();
	clear();
}
void derrota()
{
	int INPUT;

	do{

		y = (LINES-4)/2;
  		x = (COLS-20)/2;
		clear();
		refresh();
    	color_set(2, NULL);
      	mvaddstr (y+0, x-3, "   Foi Derrotado");
    	color_set(1, NULL);
    	mvaddstr (y+2, x-3, ">   Sua pontuacao - ");
    	color_set(2, NULL);
      	mvprintw (y+2, x+17, "%d", pontos);
      	INPUT = getch();

  	}while(INPUT != '\n' && INPUT != ' ' && INPUT != 27 );

	color_set(1,NULL);
	refresh();
	clear();	
}
void vitoria(Cli pCli)
{
	int INPUT;

	do{

		y = (LINES-4)/2;
  		x = (COLS-20)/2;
		clear();
		refresh();
    	color_set(2, NULL);
      	mvaddstr (y+0, x-3, "   Parabens Passou o nivel 1");
    	color_set(1, NULL);
    	mvaddstr (y+2, x-3, ">   Sua pontuacao - ");
    	color_set(2, NULL);
      	mvprintw (y+2, x+17, "%d", pontos);
      	INPUT = getch();

  	}while(INPUT != '\n' && INPUT != ' ' && INPUT != 27 );

	color_set(1,NULL);
	refresh();
	clear();
}
//Controla o menu do Jogo
void menuFunc(Cli pCli)
{
  	int INPUT=0;
  	int SELECTOR = 1;

	do{
		y = (LINES-4)/2;
  		x = (COLS-10)/2;
		color_set(1, NULL);  
	    mvaddstr (y+0, x, "Entrar Jogo");
	    mvaddstr (y+1, x, " Creditos");
	    mvaddstr (y+2, x, "   Sair");
	    color_set (2, NULL);

	    switch(SELECTOR) {
	      case 1: mvaddstr (y+0, x-1, "> Entrar Jogo"); break;
	      case 2: mvaddstr (y+1, x-1, ">  Creditos"); break;
	      case 3: mvaddstr (y+2, x-1, ">    Sair"); break;
	    }

	    INPUT = getch();
	    clear();

	    switch(INPUT) {
	      case KEY_UP: if(SELECTOR != 1) SELECTOR--; break;
	      case KEY_DOWN: if(SELECTOR != 3) SELECTOR++; break;
	    }

	    if(INPUT == '\n' || INPUT == ' ') {
	    	if(SELECTOR == 1){
		      	strcpy(pCli->cmd, "iniJogo");
				escServidor(pCli);
				leCliente(pCli);
				if (strcmp(pCli->cmd,"iniPronto")==0)
				{
					do{
						gameControls(pCli);
						if (strcmp(pCli->cmd, "vitoria") == 0)
						{
							vitoria(pCli);
						}else if (strcmp(pCli->cmd, "derrota") == 0)
						{
							derrota();
						}
					}while(strcmp(pCli->cmd, "nextL") == 0);
					
				}else{
					mvprintw(y+4, x-15, pCli->msg1);
				}
				strcpy(pCli->cmd, "vazio");
			}
		    if(SELECTOR == 2){
		    	creditos_func();
	        }
	        if(SELECTOR == 3){
	      		strcpy(pCli->cmd, "quit");
	        }
	    }
	    refresh();
	}while(strcmp(pCli->cmd, "quit")!=0);
}
//Trata dos sinais SIGUSR1 e SIGINT
void shutdown(int signal)
{
	char np[20];
	sprintf(np, "np%d", getpid());
  	if (signal == SIGUSR1)
  	{
	  	unlink(np);
  		printf("\nO servidor fez Shutdown\n");
  	}else if (signal == SIGINT)
  	{
  		Clientes cli;
  		cli.pid = getpid();
		strcpy(cli.cmd, "quit");
  		escServidor(&cli);
  		unlink(np);
  		printf("\nVolte quando puder!!!\n");
  	}

  	endwin();
	exit(0);
}
void main(int argc, char **argv)
{
	
	int fdServidor, fdCliente;
	char np[10];
	
	Clientes cli, *pCli = &cli;

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
	//Verifica se o servidor encontra-se ligado
	if(access(FIFO,F_OK)!=0)
	{
		printf("O servidor não está a correr\n");
		exit(0);
	}
	
	cli.pid = getpid();
	
	sprintf(np,"np%d", getpid());
	mkfifo(np, 0600);
	strcpy(cli.cmd, "verifica");
	escServidor(pCli);
	leCliente(pCli);

	initscr();
	start_color();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(FALSE);
	set_escdelay(25);

	init_pair (1, COLOR_WHITE, COLOR_BLACK);
	init_pair (2, COLOR_GREEN, COLOR_BLACK);
	init_pair (3, COLOR_BLACK, COLOR_WHITE);
	init_pair (4, COLOR_BLACK, COLOR_BLUE);
	init_pair (5, COLOR_BLACK, COLOR_MAGENTA);
	init_pair (6, COLOR_RED, COLOR_BLACK);
	init_pair (7, COLOR_WHITE, COLOR_RED);
	init_pair (8, COLOR_GREEN, COLOR_RED);
	init_pair (9, COLOR_WHITE, COLOR_YELLOW);
	init_pair (10, COLOR_WHITE, COLOR_GREEN);


	if(strcmp(cli.cmd, "login")==0){
		loginFunc(pCli);
	}
	if(strcmp(cli.cmd, "entrou")==0){
		strcpy(pCli->cmd, "vazio");
		menuFunc(pCli);
	}
	strcpy(cli.cmd, "quit");
	escServidor(pCli);
	unlink(np);


  	endwin();
	exit(0);
}