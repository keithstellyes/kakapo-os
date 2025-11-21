#pragma once

class Terminal {
    public:
        virtual void writeString(const char *s) = 0;
        virtual void updateCursorRow(unsigned int row) = 0;
        virtual void updateCursorColumn(unsigned int col) = 0;
        virtual unsigned int getCursorRow();
        virtual unsigned int getCursorColumn();
        virtual void clearScreen() = 0;
};
