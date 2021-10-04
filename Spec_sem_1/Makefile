SHELL = /bin/sh

WINDOWS_FLAGS = -g -O3 -W -Wall -Wunused -Wcast-align -Werror -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations
FAST_FLAGS = -O3
LINUX_FLAGS = -O3 -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
PROF_FLAGS = -O3 -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format -pg

#CFLAGS = -O3 -W -Wall -Wcast-align -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations
CFLAGS = $(WINDOWS_FLAGS)

current: a01.out a02.out a03.out a04.out a05.out a06.out a07.out a08.out
	rm -f *.o
others: 

main%.o: main%.cpp
local_array.o: local_array.cpp
tasks_array.o: tasks_array.cpp
tasks_sequences.o: tasks_sequences.cpp

a%.out: main%.o local_array.o tasks_array.o tasks_sequences.o
	g++ $(CFLAGS) $^ -o $@
%.o: %.cpp
	g++ -c $(CFLAGS) $< -o $@

clean: 
	rm -f *.out
	rm -f *.o
	rm -f *.exe
