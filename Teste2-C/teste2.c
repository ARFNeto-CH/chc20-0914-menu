#include "menu.h"

int main(void)
{
	// menu vem do disco
	const char* arquivo = "outro-menu.txt";
	MenuConfig menu;
	_m_carrega_do_arquivo( arquivo, &menu);

	int asw = _m_menu(&menu);
	
	if (asw > 0)
		printf("\n\nOpcao: %d '%s'\n", asw, menu.argv[asw]);
	else
		printf("\n\nCancelado\n");
	 
	return 0;
};	// main();

// fim