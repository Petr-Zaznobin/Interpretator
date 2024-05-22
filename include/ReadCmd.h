#include <iostream>
#include <string>
#include <map>
#include<stack>
#include <vector>
#pragma once
 using namespace std;
 //Данный класс содержит в себе строку до символа ";".
 //Данный класс разибрает строку на команды и выполняет их.
class ReadCmd{
private:
    string* cmds; //массив команд
    size_t size; //количество команд (строк массива)
    size_t cur; //строчка нахождения в данный момент
    stack<string> brackets; //здесь лежат переменные из блока и открывающиеся скобки
    stack<string> last_func; //здесь лежат названия последних функций и номера строк начал функций для for и while
    string cmd; //считываемая команда (строчка)
    map<string, double> vals; //мапа переменных, первая букква имени - тип данных
    map<string, int> dataTypes;//мапа типов данных 
    vector<pair <string, double> > resVals; //вектор отработанных переменных имя+значение в double
    map<string, int> funcs; //мапа доступных функций
    map<char, int> ops; //мапа доступных операций
    char case_symb(string symb); //функция, возращающая спец. код знака сравнения
    stack<bool> should_read_cmd;
    int count_circles_in_steps = 0; //считает сколько открытых скобок было, когда не должен читать, чтобы попасть в свою скобку
public:
    ReadCmd(string* cmds, size_t count); //конструктор
    ~ReadCmd(); //деструктор
    void getCmds(); //вызывает parcer на каждую строчку из массива cmds
    void Parsing(string cmd_); //разбор строки на составляющие (функции, переменные, операции)
    void procValue(string type, string cmd_); //обработка объявления переменной
    void changeValue(); //обработка изменения переменной
    int stringToInt(string data); //преобразование строки в int
    double stringToDouble(string data); //преобразование строки в double
    float stringToFloat(string data); //преобразование строки в float
    string intToString(int data); //преобразование int в string
    void doubleCheck(string data);
    bool isDigit(char first_symbol);
    void procIf(); //обработка if
    void procWhile(); //обработка while
    void procFor(); //обработка for
    void procMain(); //обработка main
    void procBrack(); //обработка закрывающейся фигурной скобки
    char stringToChar(string data); //преобразование строки в char
    double findResVal(string name);//поиск финального значения отработанной переменной
    void printResVal(); //вывод всех переменных
};
