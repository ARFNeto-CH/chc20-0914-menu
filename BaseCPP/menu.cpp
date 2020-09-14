#include <fstream>
#include <string>
#include <vector>

#include "menu.h"

Menu::Menu()
{	// padrao
	argc = 0;
	argv = {};
	completo = false;
	dflt = 1;
	opt = 1;
	timeout_ms = 80;

	linha = 10;
	coluna = 10;
};

Menu::Menu(int argc, vector<string>& argv,
	int linha, int coluna, int dflt, int opt, int timeout_ms) :
	linha(linha), coluna(coluna), dflt(dflt),
	opt(opt), timeout_ms(timeout_ms),
	argc(argc), argv(argv)
{
	completo = argv.size() > 0;
};

int		Menu::carrega_do_arquivo(string arquivo)
{
	// antes de tudo apaga as opcoes de antes
	argv.erase( argv.begin(), argv.end() ); // apaga tudo
	ifstream entrada{ arquivo };
	string uma;
	getline(entrada, uma);
	while (!entrada.eof())
	{
		if (uma.length() == 0)break;
		if (uma[0] != '#')
			argv.push_back(" " + uma + " ");
		getline(entrada, uma);
	};  // while()
	// le os outros parametros
	getline(entrada, uma, ',');	linha = stoi(uma);
	getline(entrada, uma, ',');	coluna = stoi(uma);
	getline(entrada, uma, ',');	dflt = stoi(uma);
	getline(entrada, uma, ',');	timeout_ms = stoi(uma);
	getline(entrada, uma);		opt = stoi(uma);
	entrada.close();
	if (argv.size() == 0) return -1;
	completo = true;
	argc = argv.size();
	return 0;
}

int Menu::carrega_opcoes(const vector<string>& V)
{
	if (V.size() < 3) return -1;
	argv = V;
	argc = V.size();
	completo = true;
	return 0;
};	// carrega_do_arquivo()

int		Menu::cls()
{
	CONSOLE_SCREEN_BUFFER_INFO		info;
	HANDLE		H = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD		origem = { 0,0 };
	DWORD			total;
	GetConsoleScreenBufferInfo(H, &info);
	int r = FillConsoleOutputCharacter(H, (TCHAR)' ',
		info.dwSize.X * info.dwSize.Y,
		origem, &total);
	int s = FillConsoleOutputAttribute(
		H, info.wAttributes,
		info.dwSize.X * info.dwSize.Y,
		origem, &total);
	SetConsoleCursorPosition(H, origem);
	return 0;
}

string	Menu::getopt(int opt)
{
	return argv[opt];
}

void	Menu::mensagem_YX(COORD Pos, const string& msg, char opt)
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

	int res = WriteConsoleA(Console, msg.c_str(), msg.length(), &total, NULL);

	SetConsoleTextAttribute(Console, info.wAttributes); // reset
	return;

}

int		Menu::select()
{
#define		_CLS_ 0x01

	// limpa a tela
	// mostra o menu inicial
	// usa menu_helper para navegar
	// retorna a opcao para quem chamou
	if (argc == 0) return -1;
	if (argv.size() == 0) return -2;
	char clear = opt & _CLS_;
	if ( clear ) cls();
	COORD Pos{ (SHORT)coluna, (SHORT)linha };
	int res = 0;

	mensagem_YX(Pos, argv[0], 1); // O titulo
	Pos.Y += 2; // vai mostrar as opcoes
	for (int i = 1; i < argc; i += 1, Pos.Y += 1)
		mensagem_YX(Pos, argv[i], (i == dflt));
	// navega
	int opt = dflt; // a opcao selecionada
	Pos.Y = linha + dflt + 1;
	while ((res = helper()) != 'R')
	{
		if (res == 'E')
		{
			if ( clear ) cls();
			return -1;
		};
		// vai mudar a opcao
		mensagem_YX(Pos, argv[opt], 0);
		if (res == '+')	opt = (opt >= argc - 1) ? 1 : opt + 1;
		else
			if (res == '-')	opt = (opt < 2) ? argc - 1 : opt - 1;
			else
				if (res == '0')	opt = dflt;
		Pos.Y = linha + opt + 1;
		mensagem_YX(Pos, argv[opt], 1);
	};	// while()
	if ( clear ) cls();
	return opt;
}

int		Menu::helper()
{
// valores de retorno
#define		_DEFAULT_  '0'
#define		_DESCE_    '-'
#define		_ENTER_    'R'
#define		_ESCAPE_   'E'
#define		_SOBE_     '+'
	// desce e sobe no indice das opcoes
		typedef struct _INPUT_RECORD Input;
		Input	buffer[32];
		char	c = 0;
		HANDLE	Console = GetStdHandle(STD_INPUT_HANDLE);
		DWORD	tempo = timeout_ms;
		DWORD	tem_tecla = WAIT_OBJECT_0;
		DWORD	total = 0;
		int		sem_escolha = 1;
		WORD	v = 0; // a tecla
		do
		{
			FlushConsoleInputBuffer(Console);
			if ( WaitForSingleObject(Console, tempo) != tem_tecla) continue;
			int n = PeekConsoleInput(Console, buffer, 32, (LPDWORD)&total);
			if (total == 0) break;
			for (DWORD i = 0; i < total; i += 1)
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

	return 0;
}

ostream& operator<<(ostream& saida, const Menu& M)
{
	saida << "\n\n" << M.argc << " argumentos\n\n";
	for (int i = 0; i < M.argc; i += 1)
	{
		saida << setw(2) << i << "  " << M.argv[i] << "\n";
	};
	saida <<
		"\n\tDefault = " << M.dflt <<
		"\n\tTimeout = " << M.timeout_ms << "ms" <<
		"\n\tOpcoes  = " << M.opt << 
		"\n\tOrigem: (" << M.linha << ", " << M.coluna << ")" <<
		"\n\tCompleto: " << M.completo << "\n";
	return saida;
};	// operator<<
