#include "menu.h"

int main(void)
{
	const char* opcoes[] =
	{
		" Menu - Um exemplo de opcoes ",
		" 1 - See also ",
		" 2 - Console Functions ",
		" 3 - FlushConsoleInputBuffer ",
		" 4 - Low - Level Console Input Functions ",
		" 5 - PeekConsoleInput ",
		" 6 - ReadConsole ",
		" 7 - ReadConsoleInput ",
		" 8 - ReadFile ",
		" 9 - MenuConfig um_teste "
	};	// um teste 9 opcoes
	int		n_opt = sizeof(opcoes) / sizeof(char*);

	MenuConfig  cfg =
	{
	  .argc = n_opt,
	  .argv = _m_carrega_da_memoria(n_opt, opcoes),
	  .dflt = 4,
	  .opt = 0,
	  .timeout_ms = 100,
	  .linha = 10,
	  .coluna = 10
	};

	int asw = _m_menu(&cfg);
	
	if (asw > 0)
		printf("\n\nSelecionada opcao: %d '%s'\n", asw, cfg.argv[asw]);
	else
		printf("\n\nCancelado\n");

	return 0;
};	// main();

// fim