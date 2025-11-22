#pragma once
class File {
    public:
        virtual size_t write (const void * ptr, size_t size, size_t count) = 0;
        virtual size_t read (void * ptr, size_t size, size_t count) = 0;
};

