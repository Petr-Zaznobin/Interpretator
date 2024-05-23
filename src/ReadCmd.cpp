#include "ReadCmd.h"
#include <regex>
using namespace std;

ReadCmd::ReadCmd(string* cmds_, size_t count) {
    size = count;
    cmds = new string[size];
    copy(cmds_, cmds_ + count, cmds);
    dataTypes = {{"int",1},
                              {"double", 1},
                              {"float",  1},
                              {"char",   1}};
    funcs = {{"for",   1},
                              {"while", 1},
                              {"if",    1},
                              {"main",  1},
                              {"}",     1}};
    ops = {{'+', 1},
                          {'-', 1},
                          {'*', 1},
                          {'/', 1},
                          {'%', 1}};
    should_read_cmd.push(true);
}

ReadCmd::~ReadCmd() {
    cmd = "";
    cmds = {};
    size = 0;
    cur = 0;
    if (!brackets.empty()){
        throw logic_error("brackets isn't empty in the end");
    }
    if (!last_func.empty()){
        throw logic_error("last_func isn't empty in the end");
    }

    delete[]cmds;
    ops.clear();
    funcs.clear();
    dataTypes.clear();
    if (vals.size()>0){
        vals.clear();
    }
    while(!should_read_cmd.empty()){
        should_read_cmd.pop();
    }
}

void ReadCmd::getCmds() {
    cur = 0;
    for (cur; cur < size; cur++){
        Parsing(cmds[cur]);
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

void ReadCmd::Parsing(string cmd_) {
    bool repeat_circle = false;
    cmd = cmd_;
    if(!should_read_cmd.top()){
        if (cmd_[0] == 'i' && cmd[1] == 'f' && cmd[2] == ' '){
            count_circles_in_steps++;
        }
        else if(cmd_[0] == 'f' && cmd[1] == 'o' && cmd[2] == 'r' && cmd_[3] == ' '){
            count_circles_in_steps++;;
        }
        else if(cmd_[0] == 'w' && cmd[1] == 'h' && cmd[2] == 'i' && cmd_[3] == 'l' && cmd_[4] == 'e' && cmd[5] == ' '){
            count_circles_in_steps++;
        }
    }
    if(should_read_cmd.top() || cmd_ == "}"){
        if (cmd_ == "}" && ((count_circles_in_steps-1) >= 0)) {
            count_circles_in_steps--;
        }
        else{
            int i = 0;
            string word = "";
            while (cmd[i]!= ' ' && cmd[i]!='(') {
                word.push_back(cmd[i]);
                if (cmd[i] == '}'){
                    break;
                }
                i ++;
            }
            if (word[0] == '*'){
                repeat_circle = true;
                word.erase(0,1);
            }
            if (funcs.count(word) == 1) {
                switch (word[0]) {
                    case 'i':
                        procIf();
                        break;
                    case 'f':
                        if (repeat_circle){word.insert(0, "*");}
                        procFor();
                        break;
                    case 'w':
                        if (repeat_circle){word.insert(0, "*");}
                        procWhile();
                        break;
                    case 'm':
                        procMain();
                        break;
                    case '}':
                        procBrack();
                        break;
                }
            } else if (dataTypes.count(word) == 1){
                procValue(word, cmd_);
            }
            else{changeValue();}
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
                for (i = type.size() + 1; cmd_[i] != ' '; i++) {
                    name.push_back(cmd_[i]);
                }
                while (cmd_[i] != '=') {
                    i++;
                    if (i > cmd_.size()) {
                        throw length_error("No symbol = in assignment");
                    }
                }
                i++;
                for (i; cmd_[i] != ';' && i < cmd_.size(); i++) {
                    if(cmd_[i] == ' '){continue;}
                    data.push_back(cmd_[i]);
                    if (i > cmd_.size()) {
                        throw length_error("No symbol ; in assignment");
                    }
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

void ReadCmd::changeValue() {
    string main_var;
    string sign;
    string first_var;
    string second_var;
    string data_type;
    //regex firstTypeRegex("(.*?)\\s([=|+=|-=|/=|*=])\\s(.*?);"); //var += 5;
    regex firstTypeRegex(R"(\b(\w+)\s*(\+=|-=|\*=|/=|%=)\s*(\w+);\b)");
    regex secondTypeRegex("(.*?)\\s=\\s(.*?)\\s([+|-|*|/])\\s(.*?);"); //var = 3+2
    smatch match;
    if (regex_search(cmd, match, firstTypeRegex)){
        main_var = match[1];
        sign = match[2];
        first_var = match[3];
        if (vals.count(main_var.insert(0, "i"))==1){
            data_type = "i";
        }
        else if (main_var.erase(0,1) == match[1] && vals.count(main_var.insert(0, "d"))==1){
            data_type = "d";
        }
        else if (main_var.erase(0,1) == match[1] && vals.count(main_var.insert(0, "f"))==1){
            data_type = "f";
        }
        else if (main_var.erase(0,1) == match[1] && vals.count(main_var.insert(0, "c"))==1){
            data_type = "c";
        }
        else{
            throw invalid_argument("No such main var in change_val");
        }
        if (int(first_var[0]) < 48 || int(first_var[0] > 57)){ //встретили переменную
            if(vals.count(first_var.insert(0, data_type)) == 0){
                throw invalid_argument("No such first var in change_val");
            }
            else{
                if (sign == "="){
                    vals[main_var] = vals[first_var];
                }
                else if (sign == "+="){
                    vals[main_var]+=vals[first_var];
                }
                else if (sign == "-="){
                    vals[main_var]-=vals[first_var];
                }
                else if (sign == "*="){
                    vals[main_var]*=vals[first_var];
                }
                else if (sign == "/="){
                    vals[main_var]/=vals[first_var];
                }
            }
        }
        else{ //встретили чиселко
            if (data_type == "i") {
                int argument = stringToInt(first_var);
                if (sign == "="){
                    vals[main_var] = argument;
                }
                else if (sign == "+="){
                    vals[main_var]+=argument;
                }
                else if (sign == "-="){
                    vals[main_var]-=argument;
                }
                else if (sign == "*="){
                    vals[main_var]*=argument;
                }
                else if (sign == "/="){
                    vals[main_var]/=argument;
                }
            }
            else if (data_type == "d"){
                double argument  = stringToDouble(first_var);
                if (sign == "="){
                    vals[main_var] = argument;
                }
                else if (sign == "+="){
                    vals[main_var]+=argument;
                }
                else if (sign == "-="){
                    vals[main_var]-=argument;
                }
                else if (sign == "*="){
                    vals[main_var]*=argument;
                }
                else if (sign == "/="){
                    vals[main_var]/=argument;
                }
            }
            else if(data_type == "f"){
                float argument  = stringToFloat(first_var);
                if (sign == "="){
                    vals[main_var] = argument;
                }
                else if (sign == "+="){
                    vals[main_var]+=argument;
                }
                else if (sign == "-="){
                    vals[main_var]-=argument;
                }
                else if (sign == "*="){
                    vals[main_var]*=argument;
                }
                else if (sign == "/="){
                    vals[main_var]/=argument;
                }
            }
            else if(data_type == "c"){
                char argument  = stringToChar(first_var);
                if (sign == "="){
                    vals[main_var] = argument;
                }
                else if (sign == "+="){
                    vals[main_var]+=argument;
                }
                else if (sign == "-="){
                    vals[main_var]-=argument;
                }
                else if (sign == "*="){
                    vals[main_var]*=argument;
                }
                else if (sign == "/="){
                    vals[main_var]/=argument;
                }
            }
        }
    }
    else if (regex_search(cmd, match, secondTypeRegex)){
        main_var = match[1];
        sign = match[3];
        first_var = match[2];
        second_var = match[4];
        bool first_is_var = false;
        bool second_is_var = false;
        if (vals.count(main_var.insert(0, "i"))==1){
            data_type = "i";
        }
        else if (main_var.erase(0,1) == match[1] && vals.count(main_var.insert(0, "d"))==1){
            data_type = "d";
        }
        else if (main_var.erase(0,1) == match[1] && vals.count(main_var.insert(0, "f"))==1){
            data_type = "f";
        }
        else if (main_var.erase(0,1) == match[1] && vals.count(main_var.insert(0, "c"))==1){
            data_type = "c";
        }
        else{
            throw invalid_argument("No such main var in change_val");
        }

        if (int(first_var[0]) < 48 || int(first_var[0] > 57)){
            if(vals.count(first_var.insert(0, data_type)) == 0){
                throw invalid_argument("No such first var in change_val");
            }
            else{first_is_var = true;}
        }
        if (int(second_var[0]) < 48 || int(second_var[0] > 57)){
            if(vals.count(second_var.insert(0, data_type)) == 0){
                throw invalid_argument("No such second var in change_val");
            }
            else{second_is_var = true;}
        }

        if (sign == "+"){
            if (first_is_var && second_is_var){
                vals[main_var] = vals[first_var] + vals[second_var];
            }
            else if (first_is_var && !second_is_var){
                if(data_type == "i"){vals[main_var] = vals[first_var] + stringToInt(second_var);}
                else if(data_type == "d"){vals[main_var] = vals[first_var] + stringToDouble(second_var);}
                else if(data_type == "f"){vals[main_var] = vals[first_var] + stringToFloat(second_var);}
                else if(data_type == "c"){vals[main_var] = vals[first_var] + stringToChar(second_var);}
            }
            else if(!first_is_var && second_is_var){
                if (data_type == "i"){vals[main_var] = stringToInt(first_var) + vals[second_var];}
                else if (data_type == "d"){vals[main_var] = stringToDouble(first_var) + vals[second_var];}
                else if (data_type == "f"){vals[main_var] = stringToFloat(first_var) + vals[second_var];}
                else if (data_type == "c"){vals[main_var] = stringToChar(first_var) + vals[second_var];}
            }
            else if (!first_is_var && !second_is_var){
                if (data_type == "i"){vals[main_var] = stringToInt(first_var) + stringToInt(second_var);}
                else if (data_type == "d"){vals[main_var] = stringToDouble(first_var) + stringToDouble(second_var);}
                else if (data_type == "f"){vals[main_var] = stringToFloat(first_var) + stringToFloat(second_var);}
                else if (data_type == "c"){vals[main_var] = stringToChar(first_var) + stringToChar(second_var);}
            }
        }
        else if (sign == "-"){
            if (first_is_var && second_is_var){
                vals[main_var] = vals[first_var] - vals[second_var];
            }
            else if (first_is_var && !second_is_var){
                if(data_type == "i"){vals[main_var] = vals[first_var] - stringToInt(second_var);}
                else if(data_type == "d"){vals[main_var] = vals[first_var] - stringToDouble(second_var);}
                else if(data_type == "f"){vals[main_var] = vals[first_var] - stringToFloat(second_var);}
                else if(data_type == "c"){vals[main_var] = vals[first_var] - stringToChar(second_var);}
            }
            else if(!first_is_var && second_is_var){
                if (data_type == "i"){vals[main_var] = stringToInt(first_var) - vals[second_var];}
                else if (data_type == "d"){vals[main_var] = stringToDouble(first_var) - vals[second_var];}
                else if (data_type == "f"){vals[main_var] = stringToFloat(first_var) - vals[second_var];}
                else if (data_type == "c"){vals[main_var] = stringToChar(first_var) - vals[second_var];}
            }
            else if (!first_is_var && !second_is_var){
                if (data_type == "i"){vals[main_var] = stringToInt(first_var) - stringToInt(second_var);}
                else if (data_type == "d"){vals[main_var] = stringToDouble(first_var) - stringToDouble(second_var);}
                else if (data_type == "f"){vals[main_var] = stringToFloat(first_var) - stringToFloat(second_var);}
                else if (data_type == "c"){vals[main_var] = stringToChar(first_var) - stringToChar(second_var);}
            }
        }
        else if (sign == "*"){
            if (first_is_var && second_is_var){
                vals[main_var] = vals[first_var] * vals[second_var];
            }
            else if (first_is_var && !second_is_var){
                if(data_type == "i"){vals[main_var] = vals[first_var] * stringToInt(second_var);}
                else if(data_type == "d"){vals[main_var] = vals[first_var] * stringToDouble(second_var);}
                else if(data_type == "f"){vals[main_var] = vals[first_var] * stringToFloat(second_var);}
                else if(data_type == "c"){vals[main_var] = vals[first_var] * stringToChar(second_var);}
            }
            else if(!first_is_var && second_is_var){
                if (data_type == "i"){vals[main_var] = stringToInt(first_var) * vals[second_var];}
                else if (data_type == "d"){vals[main_var] = stringToDouble(first_var) * vals[second_var];}
                else if (data_type == "f"){vals[main_var] = stringToFloat(first_var) * vals[second_var];}
                else if (data_type == "c"){vals[main_var] = stringToChar(first_var) * vals[second_var];}
            }
            else if (!first_is_var && !second_is_var){
                if (data_type == "i"){vals[main_var] = stringToInt(first_var) * stringToInt(second_var);}
                else if (data_type == "d"){vals[main_var] = stringToDouble(first_var) * stringToDouble(second_var);}
                else if (data_type == "f"){vals[main_var] = stringToFloat(first_var) * stringToFloat(second_var);}
                else if (data_type == "c"){vals[main_var] = stringToChar(first_var) * stringToChar(second_var);}
            }
        }
        else if (sign == "/"){
            if (first_is_var && second_is_var){
                vals[main_var] = vals[first_var] / vals[second_var];
            }
            else if (first_is_var && !second_is_var){
                if(data_type == "i"){vals[main_var] = vals[first_var] / stringToInt(second_var);}
                else if(data_type == "d"){vals[main_var] = vals[first_var] / stringToDouble(second_var);}
                else if(data_type == "f"){vals[main_var] = vals[first_var] / stringToFloat(second_var);}
                else if(data_type == "c"){vals[main_var] = vals[first_var] / stringToChar(second_var);}
            }
            else if(!first_is_var && second_is_var){
                if (data_type == "i"){vals[main_var] = stringToInt(first_var) / vals[second_var];}
                else if (data_type == "d"){vals[main_var] = stringToDouble(first_var) / vals[second_var];}
                else if (data_type == "f"){vals[main_var] = stringToFloat(first_var) / vals[second_var];}
                else if (data_type == "c"){vals[main_var] = stringToChar(first_var) / vals[second_var];}
            }
            else if (!first_is_var && !second_is_var){
                if (data_type == "i"){vals[main_var] = stringToInt(first_var) / stringToInt(second_var);}
                else if (data_type == "d"){vals[main_var] = stringToDouble(first_var) / stringToDouble(second_var);}
                else if (data_type == "f"){vals[main_var] = stringToFloat(first_var) / stringToFloat(second_var);}
                else if (data_type == "c"){vals[main_var] = stringToChar(first_var) / stringToChar(second_var);}
            }
        }
        else if (sign == "%"){
            if (first_is_var && second_is_var){
                if (data_type == "i"){
                    vals[main_var] = int(vals[first_var]) % int(vals[second_var]);
                }

            }
            else if (first_is_var && !second_is_var){
                if(data_type == "i"){vals[main_var] = int(vals[first_var]) % int(stringToInt(second_var));}
            }
            else if(!first_is_var && second_is_var){
                if (data_type == "i"){vals[main_var] = int(stringToInt(first_var)) - int(vals[second_var]);}
            }
            else if (!first_is_var && !second_is_var){
                if (data_type == "i"){vals[main_var] = int(stringToInt(first_var)) - int(stringToInt(second_var));}
            }
        }
    }
    else {throw invalid_argument("error in change value");}
}

void ReadCmd::procIf() {
    int i = 0;
    string left = "";
    string right = "";
    string symb = "";
    while(cmd[i]!='('){i++;}
    i++;
    while (cmd[i]!=' '){
        left.push_back(cmd[i]);
        i++;
    }
    i++;
    while (cmd[i]!= ' ') {
        symb.push_back(cmd[i]);
        i++;
    }
    i++;
    while (cmd[i]!=')'){
        right.push_back(cmd[i]);
        i++;
    }
    string left_full = left;
    string right_full = right;
    if (vals.count(left_full.insert(0, "i")) > 0 && vals.count(right_full.insert(0, "i")) > 0){
        left.insert(0,"i");
        right.insert(0,"i");
    }
    else if (left_full.erase(0,1) == left && right_full.erase(0,1) == right &&
    vals.count(left_full.insert(0, "f")) > 0 && vals.count(right_full.insert(0, "f")) > 0){
        left.insert(0,"f");
        right.insert(0,"f");
    }
    else if (left_full.erase(0,1) == left && right_full.erase(0,1) == right &&
             vals.count(left_full.insert(0, "d")) > 0 && vals.count(right_full.insert(0, "d")) > 0){
        left.insert(0,"d");
        right.insert(0,"d");
    }
    else if (left_full.erase(0,1) == left && right_full.erase(0,1) == right &&
             vals.count(left_full.insert(0, "c")) > 0 && vals.count(right_full.insert(0, "c")) > 0){
        left.insert(0,"c");
        right.insert(0,"c");
    }
    else{throw logic_error("No such val or neodinakovie types");}
    switch (case_symb(symb)) {
        case '1':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (vals[left] == vals[right]){
                    brackets.push("{");
                    last_func.push("i");
                    should_read_cmd.push(true);
                }
                else{
                    should_read_cmd.push(false);
                    count_circles_in_steps = 0;
                    brackets.push("{");
                    last_func.push("i");
                }
            }
            break;
        }
        case '2':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (vals[left] != vals[right]){
                    brackets.push("{");
                    last_func.push("i");
                    should_read_cmd.push(true);
                }
                else{
                    should_read_cmd.push(false);
                    count_circles_in_steps = 0;
                    brackets.push("{");
                    last_func.push("i");
                }
            }
            break;
        }
        case '3':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (vals[left] > vals[right]){
                    brackets.push("{");
                    last_func.push("i");
                    should_read_cmd.push(true);
                }
                else{
                    should_read_cmd.push(false);
                    count_circles_in_steps = 0;
                    brackets.push("{");
                    last_func.push("i");
                }
            }
            break;
        }
        case '4':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (vals[left] >= vals[right]){
                    brackets.push("{");
                    last_func.push("i");
                    should_read_cmd.push(true);
                }
                else{
                    should_read_cmd.push(false);
                    count_circles_in_steps = 0;
                    brackets.push("{");
                    last_func.push("i");
                }
            }
            break;
        }
        case '5':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (vals[left] < vals[right]){
                    brackets.push("{");
                    last_func.push("i");
                    should_read_cmd.push(true);
                }
                else{
                    should_read_cmd.push(false);
                    count_circles_in_steps = 0;
                    brackets.push("{");
                    last_func.push("i");
                }
            }
            break;
        }
        case '6':
        {
            if (vals.count(left)>0 && vals.count(right)>0){
                if (vals[left] <= vals[right]){
                    brackets.push("{");
                    last_func.push("i");
                    should_read_cmd.push(true);
                }
                else{
                    should_read_cmd.push(false);
                    count_circles_in_steps = 0;
                    brackets.push("{");
                    last_func.push("i");
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
    //regex whileRegex("while\\((.*?)\\)\\{");
    if (cmd[0] != '*'){
        brackets.push("{");
        last_func.push("w");
    }
    else{
        cmd.erase(0,1);
        cmds[cur].erase(0,1);
    }
    regex whileRegex(R"(while\s*\((.*?)\)\s*\{)");
    smatch match;
    if (regex_search(cmd, match, whileRegex)) {
        string condition = match[1].str();
        regex conditionRegex(R"((\w+)\s*(==|!=|<=|>=|<|>)\s*(\w+)\s*)"); //processing condition
        smatch match;
        if (regex_search(condition, match, conditionRegex)) {
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
            if (first_name[0] != second_name[0]){
                throw logic_error("neodinakovie types");
            }
            switch (case_symb(symbol)) {
            case '1':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (vals[first_name] == vals[second_name]) {
                        last_func.push(intToString(cur));
                        should_read_cmd.push(true);
                    }
                    else {
                        should_read_cmd.push(false);
                        count_circles_in_steps = 0;
                    }
                    break;
                }
            }
            case '2':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (vals[first_name] != vals[second_name]) {
                        last_func.push(intToString(cur));
                        should_read_cmd.push(true);
                    }
                    else {
                        should_read_cmd.push(false);
                        count_circles_in_steps = 0;
                    }
                }
                break;
            }
            case '3':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (vals[first_name] > vals[second_name]) {
                        last_func.push(intToString(cur));
                        should_read_cmd.push(true);
                    }
                    else {
                        should_read_cmd.push(false);
                        count_circles_in_steps = 0;
                    }
                }
                break;
            }
            case '4':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (vals[first_name] >= vals[second_name]) {
                        last_func.push(intToString(cur));
                        should_read_cmd.push(true);
                    }
                    else {
                        should_read_cmd.push(false);
                        count_circles_in_steps = 0;
                    }
                }
                break;
            }
            case '5':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (vals[first_name] < vals[second_name]) {
                        last_func.push(intToString(cur));
                        should_read_cmd.push(true);
                    }
                    else {
                        should_read_cmd.push(false);
                        count_circles_in_steps = 0;
                    }
                }
                break;
            }
            case '6':
            {
                if (vals.count(first_name) > 0 && vals.count(second_name) > 0) {
                    if (vals[first_name] <= vals[second_name]) {
                        last_func.push(intToString(cur));
                        should_read_cmd.push(true);
                    }
                    else {
                        should_read_cmd.push(false);
                        count_circles_in_steps = 0;
                    }
                }
                break;
            }

            }
        }
    }
}

void ReadCmd::procFor() {
    if (cmd[0] != '*'){
        brackets.push("{");
        last_func.push("f");
        regex forRegex(R"(for\s*\(\s*((.*?)\s.*?)\s*;\s*(.*?)\s*;\s*(.*?)\s*\)\s*\{)");
        smatch match;
        int i = 0;
        if (regex_search(cmd, match, forRegex)){
            string type = match[2].str();
            string initialization = match[1].str();
            string condition = match[3].str();
            string increment = match[4].str();
            procValue(type, initialization); //processing of initialization
            regex conditionRegex (R"((\w+)\s*(==|!=|<=|>=|<|>)\s*(\w+))");//processing condition
            //regex conditionRegex(R"((.*?)\s([<|>|==|!=|>=|<=])\s(.*?))"); //processing condition
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
                    first_name.insert(0, "d");
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
                    second_name.insert(0, "d");
                }
                else if (tmp.erase(0,1) == first_name && vals.count(tmp.insert(0, "f"))==1){
                    second_name.insert(0, "f");
                }
                else if (tmp.erase(0,1) == first_name && vals.count(tmp.insert(0, "c"))==1){
                    second_name.insert(0, "c");
                }
                else{throw invalid_argument ("second variable not found in for loop");}
                if (first_name[0] != second_name[0]){
                    throw logic_error("neodinakovie types");
                }

//                if (increment[1] == '+' && increment[2] == '+'){ //processing inc/decrement
//                    tmp = "+";
//                }
//                else if (increment[1] == '-' && increment[2] == '-'){
//                    tmp = "-";
//                }
//                else{throw invalid_argument("increment or decrement is invalid");}

                switch (case_symb(symbol)) {
                    case '1':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] == vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
                            }
                            break;
                        }
                    }
                    case '2':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] != vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
                            }
                        }
                        break;
                    }
                    case '3':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] > vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
                            }
                        }
                        break;
                    }
                    case '4':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] >= vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
                            }
                        }
                        break;
                    }
                    case '5':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] < vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
                            }
                        }
                        break;
                    }
                    case '6':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] <= vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
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
    else{
        cmd = cmd.erase(0,1);
        cmds[cur].erase(0,1);
        regex forRegex(R"(for\s*\(\s*(.*?)\s(.*?)\s*;\s*(.*?)\s*;\s*(.*?)\s*\)\s*\{)");
        smatch match;
        int i = 0;
        if (regex_search(cmd, match, forRegex)){
            string type = match[1].str();
            string val = match[2].str();
            string condition = match[3].str();
            string increment = match[4].str();
            string name = "i";
            string tmp = "";
            //procValue(type, initialization); //processing of initialization
            while(val[i]!=' '){
                name.push_back(val[i]);
                i++;
            }
            if (increment[1] == '+' && increment[2] == '+'){ //processing inc/decrement
                if (vals.count(name)>0){
                    vals[name]++;
                }
            }
            else if (increment[1] == '-' && increment[2] == '-'){
                if (vals.count(name)>0){
                    vals[name]++;
                }
            }
            else{throw invalid_argument("increment or decrement is invalid");}
            regex conditionRegex (R"((\w+)\s*(==|!=|<=|>=|<|>)\s*(\w+))");//processing condition
            //regex conditionRegex(R"((.*?)\s([<|>|==|!=|>=|<=])\s(.*?))"); //processing condition
            smatch match;
            if(regex_search(condition, match, conditionRegex)){
                string first_name = match[1].str();
                string symbol = match[2].str();
                string second_name = match[3].str();
                tmp = first_name;
                if (vals.count(tmp.insert(0, "i"))==1){
                    first_name.insert(0, "i");
                }
                else if (tmp.erase(0,1) == first_name && vals.count(tmp.insert(0, "d"))==1){
                    first_name.insert(0, "d");
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
                    second_name.insert(0, "d");
                }
                else if (tmp.erase(0,1) == first_name && vals.count(tmp.insert(0, "f"))==1){
                    second_name.insert(0, "f");
                }
                else if (tmp.erase(0,1) == first_name && vals.count(tmp.insert(0, "c"))==1){
                    second_name.insert(0, "c");
                }
                else{throw invalid_argument ("second variable not found in for loop");}
                if (first_name[0] != second_name[0]){
                    throw logic_error("neodinakovie types");
                }

                switch (case_symb(symbol)) {
                    case '1':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] == vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
                            }
                            break;
                        }
                    }
                    case '2':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] != vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
                            }
                        }
                        break;
                    }
                    case '3':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] > vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
                            }
                        }
                        break;
                    }
                    case '4':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] >= vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
                            }
                        }
                        break;
                    }
                    case '5':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] < vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
                            }
                        }
                        break;
                    }
                    case '6':
                    {
                        if (vals.count(first_name)>0 && vals.count(second_name)>0){
                            if (vals[first_name] <= vals[second_name]){
                                last_func.push(intToString(cur));
                                should_read_cmd.push(true);
                            }
                            else{
                                should_read_cmd.push(false);
                                count_circles_in_steps = 0;
                            }
                        }
                        break;
                    }

                }

            }
            else{throw invalid_argument("incorrect initialization of the For loop");}
        }
    }

}

void ReadCmd::procBrack() {
    string last_function = last_func.top();
    last_func.pop();
    switch (last_function[0]) {
        case 'i': {
            string val = brackets.top();
            pair<string, double> tmp;
            while(val != "{"){
                if(vals.count(val))
                {
                    string name = val;
                    name.erase(0, 1);
                    tmp = make_pair(name, vals[val]);
                    resVals.push_back(tmp);
                    vals.erase(val);
                }
                brackets.pop();
                vals.erase(val);
                val = brackets.top();
            }
            brackets.pop();
            should_read_cmd.pop();
            break;
        }
        default:
        {
            string circle_begin = last_function;
//            last_function = brackets.top();
//            brackets.pop();
            if (last_function[0] >= 48 && last_function[0] <= 57){
                if (!should_read_cmd.top()){
                    string val = brackets.top();
                    while(val != "{"){
                        brackets.pop();
                        val = brackets.top();
                    }
                    brackets.pop();
                    should_read_cmd.pop();
                }
                else{
                    int num_of_circle = stringToInt(circle_begin);
                    if (cmds[num_of_circle][0] == '*'){
                        cmds[num_of_circle].erase(0,1);
                    }
                    cmds[num_of_circle].insert(0, "*");
                    cur = num_of_circle-1;
                    should_read_cmd.pop();
                }
            }
            else if (last_function[0] == 'w' || last_function[0] == 'f'){
                if (!should_read_cmd.top()){
                    string val = brackets.top();
                    while(val != "{"){
                        brackets.pop();
                        vals.erase(val);
                        val = brackets.top();
                    }
                    brackets.pop();
                    should_read_cmd.pop();
                }
            }
            else if(last_function[0] == 'm'){
                string val = brackets.top();
                pair<string, double> tmp;
                while(val != "{"){
                    if (vals.count(val))
                    {
                        string name = val;
                        name.erase(0, 1);
                        tmp = make_pair(name, vals[val]);
                        resVals.push_back(tmp);
                        vals.erase(val);
                    }
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
double ReadCmd::findResVal(string name)
{
    for (pair < string, double> elem: resVals)
    {
        if (elem.first == name)
            return elem.second;
    }
    throw invalid_argument("There is no val with such name");
}
void ReadCmd::printResVal()
{
    for (pair < string, double> elem : resVals)
    {
        cout << elem.first << " = " << elem.second << endl;
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
    while (tmp>0){
        res.push_back(tmp%10 + 48);
        tmp/=10;
        i++;
    }
    reverse(res.begin(), res.end());
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

