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
	_m_show_config(&cfg);
	printf("\nTecle ENTER para selecionar usando esse MENU\n");
	int asw = fgetc(stdin);

	// TESTE 1: menu aqui
	asw = _m_menu(&cfg);
	if (asw > 0)
		printf("\n\nSelecionada opcao: %d '%s'\n", asw, cfg.argv[asw]);
	else
		printf("\n\nCancelado\n");
	printf("\nTecle ENTER para ler novo MENU do disco\n");
	asw = fgetc(stdin);

	// TESTE 2: menu vem do disco
	MenuConfig disco;
	_m_carrega_do_arquivo("menu.txt", &disco);
	printf("\nLido...\n");
	_m_show_config(&disco);
	printf("\nTecle ENTER para testar esse MENU\n");
	asw = fgetc(stdin);

	asw = _m_menu(&disco);
	if (asw > 0)
		printf("\n\nOpcao: %d '%s'\n", asw, disco.argv[asw]);
	else
		printf("\n\nCancelado\n");

	// agora testa com um que vem do disco
	printf("\nTecle ENTER para ler outro MENU do disco\n");
	asw = fgetc(stdin);

	_m_carrega_do_arquivo("outro-menu.txt", &disco);
	printf("\nLido...\n");
	_m_show_config(&disco);
	printf("\nTecle ENTER para selecionar usando esse MENU\n");
	asw = fgetc(stdin);

	asw = _m_menu(&disco);
	if (asw > 0)
		printf("\n\nOpcao: %d '%s'\n", asw, disco.argv[asw]);
	else
		printf("\n\nCancelado\n");
	return 0;
};	// main();

// fim