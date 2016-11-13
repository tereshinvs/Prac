/**********************************************************************
Состояния: внутри числа, внутри оператора, скобки, начальное состояние.
Переходы из начального состояния очевидны.
Из числа при считывании не цифры переходим в соответствующее состояние,
в противном случае остаёмся в состоянии числа.
При смене состояния выводим текущую лексему.
Имеется также конечное состояние, не реализованное явно
**********************************************************************/

#include <iostream>
#include <string>
#include <cctype>

enum {
    NUM_STATE, OP_STATE, BR_STATE, ST_STATE
};

bool isoperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

bool isbracket(char c) {
    return c == '(' || c == ')';
}

void out_token(std::string &token) {
    std::cout << token << std::endl;
    token = "";
}

int main() {
    std::string cur_token = "";
    char c;
    int cur_state = ST_STATE;
    while (std::cin >> c) {
        if (c == '.') {
            out_token(cur_token);
            break;
        }
        switch (cur_state) {
            case ST_STATE:
                if (std::isdigit(c))
                    cur_state = NUM_STATE;
                else if (isoperator(c))
                    cur_state = OP_STATE;
                else if (isbracket(c))
                    cur_state = BR_STATE;
                cur_token += c;
                break;

            case NUM_STATE:
                if (std::isdigit(c))
                    cur_token += c;
                else {
                    out_token(cur_token);
                    if (isoperator(c))
                        cur_state = OP_STATE;
                    else if (isbracket(c))
                        cur_state = BR_STATE;
                    cur_token += c;
                }
                break;

            case OP_STATE:
            case BR_STATE:
                out_token(cur_token);
                if (std::isdigit(c))
                    cur_state = NUM_STATE;
                else if (isoperator(c))
                    cur_state = OP_STATE;
                else if (isbracket(c))
                    cur_state = BR_STATE;
                cur_token += c;
                break;

            default:
                break;
        }
    }
}
