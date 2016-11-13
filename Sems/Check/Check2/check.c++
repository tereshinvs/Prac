#include <iostream>
#include <string>

enum {
    STATE_S, STATE_a, STATE_0, STATE_b, STATE_1, STATE_ERR
};

int main() {
    std::string s;
    while (std::cin >> s) {
        int state = s.size() > 0 ? STATE_S : STATE_ERR;
        for (auto it = s.begin(); state != STATE_ERR && it != s.end(); it++)
            switch (state) {
                case STATE_S:
                    if (*it == 'a')
                        state = STATE_a;
                    else
                        state = STATE_ERR;
                    break;

                case STATE_a:
                    if (*it == 'a')
                        state = STATE_a;
                    else if (*it == '0')
                        state = STATE_0;
                    else
                        state = STATE_ERR;
                    break;

                case STATE_0:
                    if (*it == '0')
                        state = STATE_0;
                    else if (*it == 'b')
                        state = STATE_b;
                    else
                        state = STATE_ERR;
                    break;

                case STATE_b:
                    if (*it == 'b')
                        state = STATE_b;
                    else if (*it == '1')
                        state = STATE_1;
                    else
                        state = STATE_ERR;
                    break;

                case STATE_1:
                    if (*it == '1')
                        state = STATE_1;
                    else
                        state = STATE_ERR;
                    break;

                default:
                    state = STATE_ERR;
                    break;
            }
        std::cout << (state == STATE_ERR ? 0 : 1) << std::endl;
    }
}