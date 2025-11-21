#pragma once

#include "keyboard.hpp"
char SHIFT_TABLE[128];

void init_shift_table()
{
    for(int i = 0; i < 128; i++) {
        SHIFT_TABLE[i] = i;
    }
    const char keys[] = "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./";
    const char vals[] = "~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
    static_assert(sizeof(keys) == sizeof(vals), "keys and vals should be same length.");
    for(unsigned int i = 0; i < sizeof(keys) - 1; i++) {
        SHIFT_TABLE[(int)keys[i]] = vals[i];
    }
}

const char GREETING[] =
" _   __      _                       _____ _          _ _\n"
"| | / /     | |                     /  ___| |        | | |\n"
"| |/ /  __ _| | ____ _ _ __   ___   \\ `--.| |__   ___| | |\n"
"|    \\ / _` | |/ / _` | '_ \\ / _ \\   `--. \\ '_ \\ / _ \\ | |\n"
"| |\\  \\ (_| |   < (_| | |_) | (_) | /\\__/ / | | |  __/ | |\n"
"\\_| \\_/\\__,_|_|\\_\\__,_| .__/ \\___/  \\____/|_| |_|\\___|_|_|\n"
"                      | |\n"
"                      |_|\n"
"\n";
class KakapoShell {
    private:
        Terminal *terminal;
        char commandBuffer[256];
        size_t commandBufferIndex = 0;
        bool shiftDown = false;
    public:
        constexpr KakapoShell(Terminal *terminal)
        {
            this->terminal = terminal;
            terminal->updateCursorRow(0);
            terminal->updateCursorColumn(0);
            terminal->clearScreen();
            terminal->writeString(GREETING);
            prompt();
        }
        void prompt()
        {
            terminal->updateCursorColumn(0);
            terminal->updateCursorRow(terminal->getCursorRow() + 1);
            terminal->writeString("k) ");
            terminal->updateCursorColumn(sizeof("k) ") - 1);
        }
        void onInput(keyboard_input input)
        {
            if(input.is_ascii) {
                if(input.pressed == PRESSED) {
                    onAsciiPressed(shiftDown ? SHIFT_TABLE[input.ascii] : input.ascii);
                    return;
                }
            } else {
                if(input.isShift()) {
                    shiftDown = !input.pressed;
                }
            }
        }
        void printLine(const char *s) {
            terminal->updateCursorColumn(0);
            terminal->updateCursorRow(terminal->getCursorRow() + 1);
            terminal->writeString(s);
            prompt();
        }
        size_t parseCommandWord(const char *s) {
            size_t len = 0;
            while(*s && (*s != ' ')) {
                s++;
                len++;
            }
            return len;
        }
        void doCommand(const char *s) {
            size_t commandWordLen = parseCommandWord(s);
            if(commandWordLen == 4) {
                // assume echo, for now
                s += 4;
                while(*s && (*s == ' ')) s++;
                printLine(s);
            }
        }
        void onAsciiPressed(char c) {
            char printCharBuff[2];
            switch(c) {
                case '\b':
                    if(commandBufferIndex > 0) {
                        commandBufferIndex--;
                    }
                    printCharBuff[0] = ' ';
                    printCharBuff[1] = '\0';

                    terminal->updateCursorColumn(terminal->getCursorColumn() - 1);
                    terminal->writeString(printCharBuff);
                    terminal->updateCursorColumn(terminal->getCursorColumn() - 1);
                    break;
                case '\n':
                    commandBuffer[commandBufferIndex] = '\0';
                    doCommand(commandBuffer);
                    commandBufferIndex = 0;
                    break;
                default:
                    commandBuffer[commandBufferIndex++] = c;
                    printCharBuff[0] = c;
                    printCharBuff[1] = '\0';
                    terminal->writeString(printCharBuff);
                    terminal->updateCursorColumn(terminal->getCursorColumn());
                    break;
            }
        }
};
