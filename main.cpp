#include <iostream>
#include "shunting-yard.h"
#include "structs.h"
#include "operators.h"

using namespace std;


char* alphabet;





int main()
{
    automata* bruh = orop(".", charfunc("b", 0, 0), charfunc("a", 0, 0));
    bruh = kleene(".", bruh, 0);
    bruh = concat(".", bruh, charfunc("a", 0, 0));
    bruh = concat(".", bruh, charfunc("b", 0, 0));
    bruh = concat(".", bruh, charfunc("b", 0, 0));

    cout <<"start\t"<< bruh->start
    <<"\taccept\t" << bruh->accept << endl;
    for (auto& trans:bruh->gamma)
        cout << trans.current << "\t==>\t" 
        << trans.character << "\t==>\t" 
        << trans.next << endl;
    return 0;
}



