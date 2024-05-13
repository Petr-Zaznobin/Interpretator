#include "ReadCmd.h"
#include <regex>
using namespace std;

ReadCmd::ReadCmd(string* cmds_, size_t count) {
    size = count;
    memcpy(cmds, cmds_, size);
    map<string, int> dataTypes = {{"int",1},
                              {"double", 1},
                              {"float",  1},
                              {"char",   1}};
    map<string, int> funcs = {{"for",   1},
                              {"while", 1},
                              {"if",    1},
                              {"main",  1},
                              {"input", 1},
                              {"print", 1},
                              {"}",     1}};
    map<char, int> ops = {{'+', 1},
                          {'-', 1},
                          {'*', 1},
                          {'/', 1},
                          {'%', 1}};
}

ReadCmd::~ReadCmd() {
    cmd = {};
    vals.clear();
    funcs.clear();
}

void ReadCmd::getCmds() {
    int cur = 0;
    for (size_t i = 0; i< size; i++){
        Parsing(cmds[cur++]);
    }
}

char ReadCmd::case_symb(string symb) {
    if (symb=="=="){
        return '1';
    }
    else if(symb == "!="){
        return '2';
    }
    else if(symb == ">"){
        return '3';
    }
    else if(symb == ">="){
        return '4';
    }
    else if(symb == "<"){
        return '5';
    }
    else if(symb == "<="){
        return '6';
    }
    else {throw invalid_argument("ERROR IN CONDITION");}
}

//ДОБАВИТЬ СЛУЧАЙ ЗАКРЫТИЯ СКОБКИ И CЛУЧАЙ main()!!!!!
void ReadCmd::Parsing(string cmd_) {
    cmd = cmd_;
    if(should_read_cmd == true){
        int i = 0;
        string word = "";
        while (cmd[i]!= ' ' || cmd[i]!='(') {
            word.push_back(cmd[i]);
            i ++;
        }
        if (funcs.count(word) == 1) {//ДОБАВИТЬ СЛУЧАЙ ЗАКРЫТИЯ СКОБКИ!!!!!
            switch (word[i]) {
                case 'i':
                    procIf();
                    break;
                case 'f':
                    procFor();
                    break;
                case 'w':
                    procWhile();
                    break;
                case 'm':
                    procMain();
                    break;
                case '}':
                    procBrack();
                    break;
            }
        } else {
            procValue(word, cmd);
        }
    }
}

//первая буква названия переменной - тип переменной
void ReadCmd::procValue(string type, string cmd_) {
    int i = 0;
    string name= "";
    string data= "";
    if (dataTypes.count(type) == 1){
        name.push_back(type[0]);
        switch (type[0]) {
            case 'i': {
                for (int i = type.size() + 1; cmd_[i] != ' '; i++) {
                    name.push_back(cmd_[i]);
                }
                while (cmd_[i] != '=') {
                    i++;
                    if (i > cmd_.size()) {
                        throw length_error("No symbol = in assignment");
                    }
                }
                i++;
                for (i; cmd_[i] != ';'; i++) {
                    data.push_back(cmd_[i]);
                }
                int intData = stringToInt(data);
                if (vals.count(name)>0)
                    throw invalid_argument("Such value exist");
                vals.insert(pair<string, int>(name, intData));
                brackets.push(name);
                break;
            }

            case 'd': {
                for (int i = type.size() + 1; cmd_[i] != ' '; i++) {
                    name.push_back(cmd_[i]);
                }
                while (cmd_[i] != '=') {
                    i++;
                    if (i > cmd_.size()) {
                        throw length_error("No symbol = in assignment");
                    }
                }
                i++;
                for (i; cmd_[i] != ';'; i++) {
                    data.push_back(cmd_[i]);
                }
                int doubleData = stringToDouble(data);
                if (vals.count(name) > 0)
                    throw invalid_argument("Such value exist");
                vals.insert(pair<string, int>(name, doubleData));
                brackets.push(name);
                break;
            }

            case 'f': {
                for (int i = type.size() + 1; cmd_[i] != ' '; i++) {
                    name.push_back(cmd_[i]);
                }
                while (cmd_[i] != '=') {
                    i++;
                    if (i > cmd_.size()) {
                        throw length_error("No symbol = in assignment");
                    }
                }
                i++;
                for (i; cmd_[i] != ';'; i++) {
                    data.push_back(cmd_[i]);
                }
                int floatData = stringToFloat(data);
                if (vals.count(name) > 0)
                    throw invalid_argument("Such value exist");
                vals.insert(pair<string, int>(name, floatData));
                brackets.push(name);
                break;
            }
            case 'c': {
                for (int i = type.size() + 1; cmd_[i] != ' '; i++) {
                    name.push_back(cmd_[i]);
                }
                while (cmd_[i] != '=') {
                    i++;
                    if (i > cmd_.size()) {
                        throw length_error("No symbol = in assignment");
                    }
                }
                i++;
                for (i; cmd_[i] != ';'; i++) {
                    data.push_back(cmd_[i]);
                }
                int charData = stringToChar(data);
                if (vals.count(name) > 0)
                    throw invalid_argument("Such value exist");
                vals.insert(pair<string, int>(name, charData));
                brackets.push(name);
                break;
            }

            default: //значит это операция над переменной
            {
                string val1 = "";
                string val2 = "";
                char op;
                for (int i; cmd_[i]!= ' '; i++){
                    name.push_back(cmd_[i]);
                }
                while(cmd_[i]!= '=') {
                    i++;
                    if (i > cmd_.size()) {
                        throw length_error("No symbol = in operation");
                    }
                }
                i++;
                auto found = vals.find(val1);
                for(i; found==vals.cend(); i++) {
                    if (i > cmd_.size()) {
                        throw length_error("Value error in operation");
                    }
                    val1.push_back(cmd_[i]);
                    found = vals.find(val1);
                }
                i+=2;
                if (ops.count(cmd_[i]) == 1) {op = cmd_[i];}
                else {throw logic_error ("Syntaxis operation error");}
                i++;
                found = vals.find(val2);
                for(i; found==vals.cend(); i++) {
                    if (i > cmd_.size()) {
                        throw length_error("Value error in operation");
                    }
                    val2.push_back(cmd_[i]);
                    found = vals.find(val1);
                }
                found = vals.find(name);
                switch (op) {

                    case '+':
                        if (found == vals.cend()) {
                            vals.insert(pair<string, int>(name, vals.find(val1)->second + vals.find(val2)->second));
                        } else {
                            vals.find(name)->second = vals.find(val1)->second + vals.find(val2)->second;
                        }
                        break;
                    case '-':
                        if (found == vals.cend()) {
                            vals.insert(pair<string, int>(name, vals.find(val1)->second - vals.find(val2)->second));
                        } else {
                            vals.find(name)->second = vals.find(val1)->second - vals.find(val2)->second;
                        }
                        break;

                    case '*':
                        if (found == vals.cend()) {
                            vals.insert(pair<string, int>(name, vals.find(val1)->second * vals.find(val2)->second));
                        } else {
                            vals.find(name)->second = vals.find(val1)->second * vals.find(val2)->second;
                        }
                        break;

                    case '/':
                        if (found == vals.cend()) {
                            if (vals.find(val2)->second != 0) {
                                vals.insert(pair<string, int>(name, vals.find(val1)->second / vals.find(val2)->second));
                            } else { throw logic_error("Div 0"); }
                        } else {
                            if (vals.find(val2)->second != 0) {
                                vals.find(name)->second = vals.find(val1)->second / vals.find(val2)->second;
                            } else { throw logic_error("Div 0"); }
                        }
                        break;

                    case '%':
                        if (found == vals.cend()) {
                            if (vals.find(val2)->second != 0) {
                            vals.insert(pair<string, int>(name,
                                                          int(vals.find(val1)->second) % int(vals.find(val2)->second)));
                            } else { throw logic_error("Div 0"); }
                        } else {
                            if (vals.find(val2)->second != 0) {
                            vals.find(name)->second = int(vals.find(val1)->second) % int(vals.find(val2)->second);
                            } else { throw logic_error("Div 0"); }
                        }
                        break;
                }
            }
        }
    }
}

void ReadCmd::procIf() {
    int i = 0;
    string left = "";
    string right = "";
    string symb = "";
    while(cmd[i]!='('){i++;}
    while (cmd[i]!=' '){
        left.push_back(cmd[i]);
        i++;
    }
    i++;
    while (cmd[i]!= ' ') {
        symb.push_back(cmd[i]);
    }
    i++;
    while (cmd[i]!=')'){
        right.push_back(cmd[i]);
    }
    switch (case_symb(symb)) {
        case '1':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (left == right){
                    brackets.push("{");
                    last_func.push("i");
                }
                else{
                    should_read_cmd = false;
                }
                break;
            }
        }
        case '2':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (left != right){
                    brackets.push("{");
                    last_func.push("i");
                }
                else{
                    should_read_cmd = false;
                }
            }
            break;
        }
        case '3':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (left > right){
                    brackets.push("{");
                    last_func.push("i");
                }
                else{
                    should_read_cmd = false;
                }
            }
            break;
        }
        case '4':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (left >= right){
                    brackets.push("{");
                    last_func.push("i");
                }
                else{
                    should_read_cmd = false;
                }
            }
            break;
        }
        case '5':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (left < right){
                    brackets.push("{");
                    last_func.push("i");
                }
                else{
                    should_read_cmd = false;
                }
            }
            break;
        }
        case '6':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (left <= right){
                    brackets.push("{");
                    last_func.push("i");
                }
                else{
                    should_read_cmd = false;
                }
            }
            break;
        }

    }

}

void ReadCmd::procMain() {
    brackets.push("{");
    last_func.push("m");
}

void ReadCmd::procWhile() {
    regex whileRegex("while\\((.*?)\\)\\{");
    smatch match;
    if (regex_search(cmd, match, whileRegex)) {
        string condition = match[1].str();
        regex conditionRegex("(.*)(.*)(.*)"); //processing condition
        smatch match;
        if (regex_search(condition, match, conditionRegex)) {
            brackets.push("{");
            string tmp = "";
            string first_name = match[1].str();
            string symbol = match[2].str();
            string second_name = match[3].str();
            tmp = first_name;
            if (vals.count(tmp.insert(0, "i")) == 1) {
                first_name.insert(0, "i");
            }
            else if (tmp.erase(0, 1) == first_name && vals.count(tmp.insert(0, "d")) == 1) {
                first_name.insert(0, "d");
            }
            else if (tmp.erase(0, 1) == first_name && vals.count(tmp.insert(0, "f")) == 1) {
                first_name.insert(0, "f");
            }
            else if (tmp.erase(0, 1) == first_name && vals.count(tmp.insert(0, "c")) == 1) {
                first_name.insert(0, "c");
            }
            else {throw invalid_argument("first variable is not found");}
            
            tmp = second_name;
            if (vals.count(tmp.insert(0, "i")) == 1) {
                second_name.insert(0, "i");
            }
            else if (tmp.erase(0, 1) == second_name && vals.count(tmp.insert(0, "d")) == 1) {
                second_name.insert(0, "d");
            }
            else if (tmp.erase(0, 1) == second_name && vals.count(tmp.insert(0, "f")) == 1) {
                second_name.insert(0, "f");
            }
            else if (tmp.erase(0, 1) == second_name && vals.count(tmp.insert(0, "c")) == 1) {
                second_name.insert(0, "c");
            }
            else { throw invalid_argument("second variable is not found"); }

            switch (case_symb(symbol)) {
            case '1':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (first_name == second_name) {
                        brackets.push("{");
                        last_func.push("w");
                        last_func.push(intToString(cur));
                    }
                    else {
                        should_read_cmd = false;
                    }
                    break;
                }
            }
            case '2':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (first_name != second_name) {
                        brackets.push("{");
                        last_func.push("w");
                        last_func.push(intToString(cur));
                    }
                    else {
                        should_read_cmd = false;
                    }
                }
                break;
            }
            case '3':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (first_name > second_name) {
                        brackets.push("{");
                        last_func.push("w");
                        last_func.push(intToString(cur));
                    }
                    else {
                        should_read_cmd = false;
                    }
                }
                break;
            }
            case '4':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (first_name >= second_name) {
                        brackets.push("{");
                        last_func.push("w");
                        last_func.push(intToString(cur));
                    }
                    else {
                        should_read_cmd = false;
                    }
                }
                break;
            }
            case '5':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (first_name < second_name) {
                        brackets.push("{");
                        last_func.push("w");
                        last_func.push(intToString(cur));
                    }
                    else {
                        should_read_cmd = false;
                    }
                }
                break;
            }
            case '6':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (first_name <= second_name) {
                        brackets.push("{");
                        last_func.push("w");
                        last_func.push(intToString(cur));
                    }
                    else {
                        should_read_cmd = false;
                    }
                }
                break;
            }

            }
        }
    }
}

void ReadCmd::procFor() {
    regex forRegex("for\\(((.*?)\\s(.*?));(.*?);(.*?)\\)\\{");
    smatch match;
    int i = 0;
    if (regex_search(cmd, match, forRegex)){
        string type = match[2].str();
        string initialization = match[1].str();
        string condition = match[4].str();
        string increment = match[5].str();
        brackets.push("{");
        procValue(type, initialization); //processing of initialization
        regex conditionRegex("(.*)([<|>|==|!=|>=|<=])(.*)"); //processing condition
        smatch match;
        if(regex_search(condition, match, conditionRegex)){
            string tmp = "";
            string first_name = match[1].str();
            string symbol = match[2].str();
            string second_name = match[3].str();
            tmp = first_name;
            if (vals.count(tmp.insert(0, "i"))==1){
                first_name.insert(0, "i");
            }
            else if (tmp.erase(0,1) == first_name && vals.count(tmp.insert(0, "d"))==1){
                first_name.insert(0, "d");//////////стояло i, скорее всего ошибся, исправила
            }
            else if (tmp.erase(0,1) == first_name && vals.count(tmp.insert(0, "f"))==1){
                first_name.insert(0, "f");
            }
            else if (tmp.erase(0,1) == first_name && vals.count(tmp.insert(0, "c"))==1){
                first_name.insert(0, "c");
            }
            else{throw invalid_argument ("first variable not found in for loop");}

            tmp = second_name;
            if (vals.count(tmp.insert(0, "i"))==1){
                second_name.insert(0, "i");
            }
            else if (tmp.erase(0,1) == first_name && vals.count(tmp.insert(0, "d"))==1){
                second_name.insert(0, "d");//////////стояло i, скорее всего ошибся, исправила
            }
            else if (tmp.erase(0,1) == first_name && vals.count(tmp.insert(0, "f"))==1){
                second_name.insert(0, "f");
            }
            else if (tmp.erase(0,1) == first_name && vals.count(tmp.insert(0, "c"))==1){
                second_name.insert(0, "c");
            }
            else{throw invalid_argument ("second variable not found in for loop");}

            if (increment[1] == '+' && increment[2] == '+'){ //processing inc/decrement
                tmp = "+";
            }
            else if (increment[1] == '-' && increment[2] == '-'){
                tmp = "-";
            }
            else{throw invalid_argument("increment or decrement is invalid");}

            switch (case_symb(symbol)) {
                case '1':
                {
                    if (vals.count(first_name)>0 && vals.count(second_name)>0){
                        if (first_name == second_name){
                            brackets.push("{");
                            last_func.push("f");
                            last_func.push(intToString(cur));
                        }
                        else{
                            should_read_cmd = false;
                        }
                        break;
                    }
                }
                case '2':
                {
                    if (vals.count(first_name)>0 && vals.count(second_name)>0){
                        if (first_name != second_name){
                            brackets.push("{");
                            last_func.push("f");
                            last_func.push(intToString(cur));
                        }
                        else{
                            should_read_cmd = false;
                        }
                    }
                    break;
                }
                case '3':
                {
                    if (vals.count(first_name)>0 && vals.count(second_name)>0){
                        if (first_name > second_name){
                            brackets.push("{");
                            last_func.push("f");
                            last_func.push(intToString(cur));
                        }
                        else{
                            should_read_cmd = false;
                        }
                    }
                    break;
                }
                case '4':
                {
                    if (vals.count(first_name)>0 && vals.count(second_name)>0){
                        if (first_name >= second_name){
                            brackets.push("{");
                            last_func.push("f");
                            last_func.push(intToString(cur));
                        }
                        else{
                            should_read_cmd = false;
                        }
                    }
                    break;
                }
                case '5':
                {
                    if (vals.count(first_name)>0 && vals.count(second_name)>0){
                        if (first_name < second_name){
                            brackets.push("{");
                            last_func.push("f");
                            last_func.push(intToString(cur));
                        }
                        else{
                            should_read_cmd = false;
                        }
                    }
                    break;
                }
                case '6':
                {
                    if (vals.count(first_name)>0 && vals.count(second_name)>0){
                        if (first_name <= second_name){
                            brackets.push("{");
                            last_func.push("i");
                            last_func.push(intToString(cur));
                        }
                        else{
                            should_read_cmd = false;
                        }
                    }
                    break;
                }

            }


        }
        else{throw invalid_argument("incorrect initialization of the For loop");}
    }
    else{throw invalid_argument("incorrect initialization of the For loop");}
}

void ReadCmd::procBrack() {
    string last_function = last_func.top();
    last_func.pop();
    switch (last_function[0]) {
        case 'i': {
            string val = brackets.top();
            while(val != "{"){
                brackets.pop();
                val = brackets.top();
            }
            brackets.pop();
            break;
        }
        default:
        {
            string circle_begin = last_function;
            last_function = brackets.top();
            brackets.pop();
            if (last_function[0] == 'w' || last_function[0] == 'f'){
                if (!should_read_cmd){
                    string val = brackets.top();
                    while(val != "{"){
                        brackets.pop();
                        val = brackets.top();
                    }
                    brackets.pop();
                }
                else{cur = stringToInt(circle_begin); brackets.pop();}
            }
            else if(last_function[0] == 'm'){
                string val = brackets.top();
                while(val != "{"){
                    brackets.pop();
                    val = brackets.top();
                }
                brackets.pop();
            }
            else{throw invalid_argument("What is it? In stack of brackets");}
            break;
        }
    }
}

int ReadCmd::stringToInt(string data)
{
	int res = 0;
	for (char i: data)
	{
		if (i < '0' || i > '9')
			throw invalid_argument("Expected int");
		res = res * 10 + i-'0';
	}
	return res;
}

char ReadCmd::stringToChar(string data)
{
	if (data.size() > 1)
		throw invalid_argument("Expected one symbol");
	return data[0];
}

string ReadCmd::intToString(int data) {
    int tmp = data;
    int i = 0;
    string res = "";
    while (tmp!=0){
        res[i] = (tmp%10 + 48);
        res[i]/=10;
    }
    return res;
}

/*bool ReadCmd::isDigit(const char& first_symbol)
{
	if ((first_symbol <= '9' && first_symbol >= '0') || first_symbol == '.')
		return true;
	return false;
}*/

bool ReadCmd::isDigit(char first_symbol)
{
    if ((first_symbol <= '9' && first_symbol >= '0') || first_symbol == '-' || first_symbol == '.')
        return true;
    return false;
}

void ReadCmd::doubleCheck(string data)
{
	int e = 0;
	int dot = 0;
	
	for (char i : data)
	{
		if (i == 'e') e++;
		if (i == '.') dot++;
	}
	if (e > 1 || dot > 1 || data.find('e') < data.find('.'))
	{
		throw invalid_argument("The mistake in a digit");
	}
	if (data[data.length() - 1] == 'e')
	{
		throw invalid_argument("The mistake in a digit");
	}
	for (size_t j = 0; j < data.length(); j++)
		if (!(isDigit(data[j]) || data[j] == 'e' || data[j] == '+' || data[j] == '-'))
		{
			throw invalid_argument("The mistake in a digit");
		}
	
}

double ReadCmd::stringToDouble(string data)
{
	doubleCheck(data);
	double res = 0.0, sign = 1.0;

if (data.find('.') != -1 && data.find('e') == -1) {//there is dot

	double beforepoint = 0, afterpoint = 0;
	string beforepoint_s, afterpoint_s;
	size_t p = data.find('.');//position of the dot

	for (size_t i = 0; i < p; i++) { beforepoint_s += data[i]; }
	for (size_t i = p + 1; i < data.size(); i++) { afterpoint_s += data[i]; }

	beforepoint = stringToDouble(beforepoint_s);
	afterpoint = stringToDouble(afterpoint_s);

	afterpoint /= pow(10, afterpoint_s.size());

	res = beforepoint + afterpoint;

}

else if (data.find('e') != -1) {//there is e

	double beforeE = 0.0, afterE = 0.0;
	string beforeE_s, afterE_s;

	size_t p = data.find('e');

	for (size_t i = 0; i < p; i++) { beforeE_s += data[i]; }
	for (size_t i = p + 1; i < data.size(); i++) { afterE_s += data[i]; }

	beforeE = stringToDouble(beforeE_s);
	afterE = stringToDouble(afterE_s);

	if (afterE > 0) res = beforeE * pow(10.0, afterE);
	else {
		res = beforeE;
		for (size_t i = 0; i < (-1.0 * afterE); i++) { res /= 10; }
	}
}

else { //no dot, no e

	if (data[0] == '~' || data[0] == '-') {

		for (size_t i = 1; i < data.size(); i++) {
			res = res * 10.0 + static_cast<double>(data[i] - '0');
		}
		res *= -1.0;
	}

	else {

		for (size_t i = 0; i < data.size(); i++) {
			res = res * 10.0 + static_cast<double>(data[i] - '0');
		}
	}
}

return res; 
}

float ReadCmd::stringToFloat(string data)
{
	doubleCheck(data);
	float res = 0.0, sign = 1.0;

	if (data.find('.') != -1 && data.find('e') == -1) {//there is dot

		double beforepoint = 0, afterpoint = 0;
		string beforepoint_s, afterpoint_s;
		size_t p = data.find('.');//position of the dot

		for (size_t i = 0; i < p; i++) { beforepoint_s += data[i]; }
		for (size_t i = p + 1; i < data.size(); i++) { afterpoint_s += data[i]; }

		beforepoint = stringToDouble(beforepoint_s);
		afterpoint = stringToDouble(afterpoint_s);

		afterpoint /= pow(10, afterpoint_s.size());

		res = beforepoint + afterpoint;

	}

	else if (data.find('e') != -1) {//there is e

		double beforeE = 0.0, afterE = 0.0;
		string beforeE_s, afterE_s;

		size_t p = data.find('e');

		for (size_t i = 0; i < p; i++) { beforeE_s += data[i]; }
		for (size_t i = p + 1; i < data.size(); i++) { afterE_s += data[i]; }

		beforeE = stringToDouble(beforeE_s);
		afterE = stringToDouble(afterE_s);

		if (afterE > 0) res = beforeE * pow(10.0, afterE);
		else {
			res = beforeE;
			for (size_t i = 0; i < (-1.0 * afterE); i++) { res /= 10; }
		}
	}

	else { //no dot, no e

		if (data[0] == '~' || data[0] == '-') {

			for (size_t i = 1; i < data.size(); i++) {
				res = res * 10.0 + static_cast<double>(data[i] - '0');
			}
			res *= -1.0;
		}

		else {

			for (size_t i = 0; i < data.size(); i++) {
				res = res * 10.0 + static_cast<double>(data[i] - '0');
			}
		}
	}

	return res;
}
