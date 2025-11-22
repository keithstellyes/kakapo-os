#pragma once

#include "File.hpp"
#include "string.h"
#include "stdio.h"
class Terminal : public File {
    public:
        virtual void writeString(const char *s)
        {
            write(s, strlen(s), 1);
        }
        virtual void updateCursorRow(unsigned int row) = 0;
        virtual void updateCursorColumn(unsigned int col) = 0;
        virtual unsigned int getCursorRow();
        virtual unsigned int getCursorColumn();
        virtual void clearScreen() = 0;
        // TODO: do decrement for row and incr/decr for col
        virtual void incrementCursorRow()
        {
            updateCursorRow(getCursorRow()+1);
        }
        virtual void writeStringLine(const char *s)
        {
            writeString(s);
            writeString("\n");
        }
};
