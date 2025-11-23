CC := g++ -O2 -std=c++23 -Wall -Wextra

sandbox: sandbox.cpp
	cp ../../../kernel/alloc.h .
	cp ../../../kernel/malloc.h .
	$(CC) sandbox.cpp -o sandbox || rm -f alloc.h malloc.h
	rm -f alloc.h malloc.h
