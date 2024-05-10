//RULES
//Перед названием переменных необходимо употребить тип данных (только int, float, double, char)
//for(int start, int end, int step){...}
//while(<переменная> <знак> <переменная> <знак &&, ||> <...>){...}
//if (<переменная> <знак> <переменная> <знак &&, ||> <...>){...}
//Только после if: else(<переменная> <знак> <переменная> <знак &&, ||> <...>){...}
//Доступные операции (+,-,*,/,%)
//чтобы выполнить операцию над переменной, нужно сначала ее объявить, причем при объявлении после и перед равно нужны пробелы

#include <iostream>
#include <fstream>
#include "ReadCmd.h"
using namespace std;

int main(){
    string cmd;
    ifstream fin;
    size_t count = 0;
    fin.open("hello.txt"); // окрываем файл для чтения
    if (!fin.is_open()) {
        cout << "ERROR OPEN!";
    }

    else{
        while (getline(fin, cmd))
        {
           count++;
        }
    }
    fin.close();     // закрываем файл

    fin.open("hello.txt"); // окрываем файл для чтения
    string* cmds = new string[count];
    if (!fin.is_open()) {
        cout << "ERROR OPEN!";
    }
    else{
        int i = 0;
        while (getline(fin, cmd))
        {
            cmds[i++] = cmd;
        }
    }
    fin.close();     // закрываем файл
    ReadCmd rc(cmds, count);

}