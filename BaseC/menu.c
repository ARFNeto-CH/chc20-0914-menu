#include "menu.h"

int			_m_apaga_bloco(MenuConfig* C)
{
	if (C == NULL) return 0;
	if (C->argv == NULL) return 0;
	for (int i = 0; i < C->argc; i += 1) free(C->argv[i]);
	free(C->argv);
	return 0;
};	// _m_apaga_bloco()

char**		_m_carrega_da_memoria(const int n_opt, const char* opcao[])
{
	int argc = 0;
	char** argv = (char**)malloc(sizeof(char*) * n_opt);
	// loads all strings into **argv
	for (int n = 0; n < n_opt; n += 1)
	{
		argv[argc] = (char*)malloc(1 + strlen(opcao[n]) + 2);
		sprintf(argv[argc], " %s ", opcao[n]);
		argc += 1;
	};	// while
	return argv;
};	// _m_carrega_da_memoria()

int			_m_carrega_do_arquivo(const char* arquivo, MenuConfig* cfg)
{
	if (cfg == NULL) return -1;
	// as opcoes podem ter sido alocadas como const char*
	// no programa e nao dinamicamente. Nesse caso basta
	// reatribuir os ponteiros. O bit 1 marca 1 se o valor
	// era estatico
	if (cfg->argc > 0) _m_apaga_bloco(cfg);
	FILE* in = fopen(arquivo, "r");
	if (in == NULL) return -2;
	// tem arquivo e tem menuconfig
	struct
	{
		int     argc;
		char** argv;
	}   ex;
	int N = _BLOCK_; // 1st block
	ex.argc = 0;
	ex.argv = (char**)malloc(sizeof(char*) * _BLOCK_);
	// loads all strings into **argv
	char buffer[_LIMITE_LARG_]; // para ler as opcoes

	// le as opcoes do disco e monta o bloco
	while (NOT feof(in))
	{
		if (ex.argc >= N)
		{   // block is full
			N = N + _BLOCK_;
			char* new_block = realloc(ex.argv, (N * sizeof(char*)));
			// aumentou o bloco de arqumentos
			ex.argv = (char**)new_block;
		};
		fgets(buffer, _LIMITE_LARG_, in);
		if (buffer[0] == 10) break; // linha em branco termina
		if (buffer[0] == '#') continue; // comentario
		buffer[strlen(buffer) - 1] = 0;
		ex.argv[ex.argc] = (char*)malloc(1 + strlen(buffer) + 2);
		sprintf(ex.argv[ex.argc], " %s ", buffer);
		ex.argc += 1;
	};	// while

	// libera o que nao usou da memoria
	if (N != ex.argc)
	{
		char* new_size = realloc(ex.argv, (ex.argc * sizeof(char*)));
		ex.argv = (char**)new_size;
	};

	// monta o menuconfig
	cfg->argc = ex.argc;
	cfg->argv = ex.argv;
	N = fscanf(in, "%d,%d,%d,%d,%d",
		&cfg->linha, &cfg->coluna, &cfg->dflt,
		&cfg->timeout_ms, &cfg->opt);

	// testa se vieram todas as opcoes
	if (N != 5)
		switch (N)
		{
		case 0:
			cfg->linha = 2;
		case 1:
			cfg->coluna = 8;
		case 2:
			cfg->dflt = 1;
		case 3:
			cfg->timeout_ms = 100;
		case 4:
			cfg->opt = 1;
			break;
		default:
			cfg->linha = 2;
			cfg->coluna = 8;
			cfg->dflt = 1;
			cfg->timeout_ms = 100;
			cfg->opt = 1;
			break;
		};
	fclose(in);
	return 0;
};	// _m_carrega_do_arquivo()

int			_m_cls()
{	// limpa a tela no windows, do jeito oficial
	CONSOLE_SCREEN_BUFFER_INFO		info;
	HANDLE		H = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD		origem = { 0,0 };
	DWORD			total;
	GetConsoleScreenBufferInfo(H, &info);
	FillConsoleOutputCharacter(H, (TCHAR)' ',
		info.dwSize.X * info.dwSize.Y,
		origem, &total);
	FillConsoleOutputAttribute(
		H, info.wAttributes,
		info.dwSize.X * info.dwSize.Y,
		origem, &total);
	SetConsoleCursorPosition(H, origem);
	return 0;
};	// end cls()

void		_m_mensagem_YX(COORD Pos, const char* msg, char opt)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	DWORD total = 0;
	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(Console, &info);
	WORD Reverso = (info.wAttributes & 0xF0) | (info.wAttributes & 0xF) << 4;
	SetConsoleCursorPosition(Console, Pos);

	if (opt == 0)
		SetConsoleTextAttribute(Console, info.wAttributes);
	else
		SetConsoleTextAttribute(Console, Reverso);

	WriteConsoleA(Console, (void*)msg, strlen(msg), &total, NULL);

	SetConsoleTextAttribute(Console, info.wAttributes); // reeset
	return;
}	// _m_mensagem_em_video_reverso()

int			_m_menu(MenuConfig* C)
{
	// limpa a tela
	// mostra o menu inicial
	// usa menu_helper para navegar
	// retorna a opcao para quem chamou
	if (C->argc == 0) return -1;
	if (C->argv == NULL) return -2;
	char clear = C->opt & _CLS_;
	if (clear) _m_cls();
	COORD Pos = { .X = (SHORT)C->coluna, .Y = (SHORT)C->linha };
	int res = 0;

	_m_mensagem_YX(Pos, C->argv[0], 1); // O titulo
	Pos.Y += 2; // vai mostrar as opcoes
	for (int i = 1; i < C->argc; i += 1, Pos.Y += 1)
		_m_mensagem_YX(Pos, C->argv[i], (i == C->dflt));
	// navega
	int opt = C->dflt; // a opcao selecionada
	Pos.Y = (SHORT)(C->linha + C->dflt + 1);
	while ((res = _m_menu_helper(C)) != 'R')
	{
		if (res == 'E')
		{
			if (clear) _m_cls();
			return -1;
		};
		// vai mudar a opcao
		_m_mensagem_YX(Pos, C->argv[opt], 0);
		if (res == '+')	opt = (opt >= C->argc - 1) ? 1 : opt + 1;
		else
			if (res == '-')	opt = (opt < 2) ? C->argc - 1 : opt - 1;
			else
				if (res == '0')	opt = C->dflt;
		Pos.Y = (SHORT)(C->linha + opt + 1);
		_m_mensagem_YX(Pos, C->argv[opt], 1);
	};	// while()
	if (clear) _m_cls();
	return opt;
};	// _m_menu()

int			_m_menu_helper(MenuConfig* cfg)
{	// valores de retorno
#define		_DEFAULT_  '0'
#define		_DESCE_    '-'
#define		_ENTER_    'R'
#define		_ESCAPE_   'E'
#define		_SOBE_     '+'
	// desce e sobe no indice das opcoes
	typedef struct _INPUT_RECORD Input;
	if (cfg == NULL) return 0;
	Input	buffer[32];
	char	c = 0;
	HANDLE	Console = GetStdHandle(STD_INPUT_HANDLE);
	DWORD	tempo = cfg->timeout_ms;
	DWORD	tem_tecla = WAIT_OBJECT_0;
	int		total = 0;
	int		sem_escolha = 1;
	WORD	v = 0; // a tecla
	do
	{
		FlushConsoleInputBuffer(Console);
		if (WaitForSingleObject(Console, tempo) != tem_tecla) continue;
		PeekConsoleInput(Console, buffer, 32, (LPDWORD)&total);
		if (total == 0) break;
		for (int i = 0; i < total; i += 1)
		{
			if (buffer[i].Event.KeyEvent.bKeyDown) continue;
			v = buffer[i].Event.KeyEvent.wVirtualKeyCode;
			c = buffer[i].Event.KeyEvent.uChar.AsciiChar;
			// todos os testes sao definitivos entao so retorna
			if (v == VK_ADD) return _SOBE_;
			if (v == VK_SUBTRACT) return _DESCE_;
			if (v == VK_DOWN) return _SOBE_;
			if (v == VK_UP) return _DESCE_;
			if (v == VK_OEM_PLUS) return _SOBE_;
			if (v == '0') return _DEFAULT_;
			if (v == '+') return _SOBE_;
			if (v == VK_ESCAPE) return _ESCAPE_;
			if (v == VK_RETURN) return _ENTER_;
			if (c == '+') return _SOBE_;
			if (c == '-') return _DESCE_;
			if (c == '0') return _DEFAULT_;
		}
	} while (sem_escolha);
	return 0;
};	// _m_menu_helper()

int			_m_show_config(MenuConfig* C)
{
	printf("\n\tConfig: %d argumentos\n\n", C->argc);
	for (int i = 0; i < C->argc; i += 1)
		printf("%d  '%s' [%zd]\n", i, C->argv[i], strlen(C->argv[i]));
	printf(
		"\n\tDefault = %d '%s'\n\tTimeout = %d ms\n",
		C->dflt, C->argv[C->dflt], C->timeout_ms);
	printf("\topcoes = %d\n", C->opt);
	printf("\tOrigem: (%d, %d)\n", C->linha, C->coluna);
	printf("\n\nTecle ENTER ->  "); fgetc(stdin);

	return 0;
};

// fim