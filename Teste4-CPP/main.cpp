#include "menu.h"
using namespace std;

int main()
{
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
	int asw = menu2.select();
	cout << "\nRetornou " << asw << " \"" << menu2.getopt(asw) << "\"" << endl;
	return 0;
};

// fim