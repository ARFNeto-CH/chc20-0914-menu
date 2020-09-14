#pragma once
#define		NOT !

#include <iomanip>
#include <iostream>
#include <vector>
#include <Windows.h>

using namespace std;

class Menu
{
private:
	int				argc; // opcoes + titulo
	vector<string>	argv;
	int		dflt; // opcao padrao
	int		opt; // parametros
	int		timeout_ms;

	int		linha; // posicao
	int		coluna;

	bool	completo; // para poder montar em fases

public:
	 Menu();
	 Menu(int, vector<string>&, int , int , int , int , int );
	 ~Menu() {};

public:
	int			carrega_do_arquivo(string);
	int			carrega_opcoes(const vector<string>&);
	int			cls();
	string		getopt(int);
	void		mensagem_YX(COORD, const string&, char);
	int			select(void);
	int			helper(void);

	friend  ostream& operator<<(ostream&, const Menu&);

};

