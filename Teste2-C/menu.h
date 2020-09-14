#define		_BLOCK_			(10)
#define		_CLS_			(1)
#define		_CRT_SECURE_NO_WARNINGS
#define		_LIMITE_ARGC_	(30)
#define		_LIMITE_LARG_	(78)
#define		NOT				!

#include <stdio.h>
#include <windows.h>

typedef struct
{
	int			argc; // opcoes + titulo
	char**      argv; // as tais opcoes do menu
	int			dflt; // opcao padrao
	unsigned	opt; // parametros
	int			timeout_ms;

	int			linha; // posicao
	int			coluna;

}	MenuConfig;


int			_m_apaga_bloco(MenuConfig*);
char**		_m_carrega_da_memoria(const int, const char* []);
int			_m_carrega_do_arquivo(const char*, MenuConfig*);
int			_m_cls();
void		_m_mensagem_YX(COORD, const char*, char);
int			_m_menu(MenuConfig*);
int			_m_menu_helper(MenuConfig*);
int			_m_show_config(MenuConfig* C);

// fim