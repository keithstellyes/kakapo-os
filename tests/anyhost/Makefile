CC := g++ -O2 -std=c++23 -Wall -Wextra

elftool: elftool.cpp ../../kernel/elf.hpp
	# have to cp to avoid headaches with the fact the kernel implemetning its own system headers
	# TODO maybe those should be in a protected dir
	cp ../../kernel/elf.hpp .
	$(CC) elftool.cpp -o elftool || rm elf.hpp
	rm elf.hpp
