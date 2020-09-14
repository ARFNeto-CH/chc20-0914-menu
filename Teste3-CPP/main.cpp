#include "menu.h"
using namespace std;

int main(void)
{
    // nornal: menu a partir do disco
    Menu teste;
    teste.carrega_do_arquivo("menu.txt");
    int asw = teste.select();
    cout << "\nRetornou " << asw << " \"" << teste.getopt(asw) << "\"" << endl;
    return 0;
};  // main()
// fim