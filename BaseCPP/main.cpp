#include "menu.h"
using namespace std;

int main()
{
	// nornal: menu a partir do disco
    Menu teste;
    teste.carrega_do_arquivo("menu.txt");
    int asw = teste.select();
    cout << "\nRetornou " << asw << " \""<< teste.getopt(asw) << "\"" <<  endl;
	
	cout << 
		"\nTecle ENTER para o proximo teste: " <<
		" menu definido no programa\n";
	cin.ignore(20, '\n');
	teste.cls();

	// um teste sem arquivo: as opcoes estao no programa
	vector<string> outro
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
	};
	Menu menu2(outro.size(), outro, 12, 12, 2, 1, 80);
	asw = menu2.select();
	cout << "\nRetornou " << asw << " \"" << menu2.getopt(asw) << "\"" << endl;

	// terceiro teste: le novas opcoes para o mesmo menu
	cout <<
		"\nTecle ENTER para o proximo teste: " <<
		" vai ler novas opcoes do disco\n";
	cin.ignore(20, '\n');
	menu2.carrega_do_arquivo("outro-menu.txt");
	cout << "\nTecle ENTER para ver as novas opcoes\n";
	cin.ignore(20, '\n');
	cout << menu2;
	cout << "\nTecle ENTER para testar com esse menu \n";
	cin.ignore(20, '\n');
	asw = menu2.select();
	cout << "\nRetornou " << asw << " \"" << menu2.getopt(asw) << "\"" << endl;

	return 0;
};

// fim