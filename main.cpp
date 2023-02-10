#include <iostream>
#include "shunting-yard.h"

using namespace std;

int main()
{

    automata a = djkstra();
    cout << a.alphabet << endl;
    return 0;
}