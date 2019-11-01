#include "getTileNonetool.h"
#include <iostream>
using namespace std;
int main(int argc, char *argv[]){
    if (argc > 2){
        cout << "we expect only path to file, fucking idiot" << endl;
        return 0;
    }
    string fileName = argv[1];
    string title = getTitleNoneTool(fileName);
    cout << "title: " << title << " title length:" << title.length() << endl;

}