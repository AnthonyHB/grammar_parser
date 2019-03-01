#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "parser.h"

using namespace std;


int main (int argc, char* argv[])
{
    if (argc < 2) {
        cout << "Error: missing argument\n";
        return 1;
    }

    int task = atoi(argv[1]);
    Parser p;
    Grammar g = p.get_g();

    switch (task) {
        case 1: g.task1(); break;
        case 2: g.task2(); break;
        case 3: g.task3(); break;
        case 4: g.task4(); break;
        case 5: g.task5(); break;
        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}
