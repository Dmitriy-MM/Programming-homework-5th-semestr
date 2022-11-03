WINDOWS_FLAGS = -g -O0 -W -Wall -Wunused -Wcast-align -Werror -pedantic -pedantic-errors  -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wno-long-long -Wmissing-declarations


ERRORS_PRE_COMPILE = -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
ERRORS_POST_COMPILE = -fsanitize=address -fstack-protector-all -g
ERRORS = $(ERRORS_PRE_COMPILE) $(ERRORS_POST_COMPILE)


FAST_FLAGS = -Os -ffast-math -mfpmath=sse
OPTIMIZE = $(FAST_FLAGS)
ARCH_FAST = -mfpmath=sse -fno-exceptions -mtune=generic -march=native


LIBNAME = -DLIBNAME=\"non_intrin_aux_accurate.so\"
DO_FORCE_NON_INTRIN = -DDO_FORCE_NON_INTRIN=0
DD = $(DO_FORCE_NON_INTRIN) $(LIBNAME)


LINUX_FLAGS = $(ERRORS_PRE_COMPILE) -g

PROF_FLAGS = -pg

INTRIN = -mfpmath=sse -fno-exceptions -mtune=generic -march=corei7-avx -mfma -mavx
DYN_LIB = -ldl
CFLAGS = $(ARCH_FAST) $(LINUX_FLAGS)

current: a.out


main.o: main.cpp
matrix.o: matrix.cpp
GJ_solve.o: GJ_solve.cpp

accurate_solve_GJ.o: accurate_solve_GJ.cpp non_intrin_aux_accurate.so
	g++ -c -O2 $(ARCH_FAST) $(DD) -fno-fast-math  $< -o $@
non_intrin_aux_accurate.so: non_intrin_aux.cpp
	g++ -shared -O2 $(ARCH_FAST) -fno-fast-math $< -o $@

aux_matrix_funcs.o: aux_matrix_funcs.cpp

non_intrin_aux.o: non_intrin_aux.cpp
	g++ -c $(OPTIMIZE) $(ARCH_FAST) $< -o $@
intrin_aux.o: intrin_aux.cpp
	g++ -c $(OPTIMIZE) $(INTRIN) $(ARCH_FAST) $< -o $@

a.out: main.o matrix.o aux_matrix_funcs.o intrin_aux.o non_intrin_aux.o accurate_solve_GJ.o GJ_solve.o
	g++ $(DD) $(CFLAGS) $(DYN_LIB) $^ -o $@
	rm -f *.o

%.o: %.cpp
	g++ -c $(DD) $(CFLAGS) $< -o $@

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.exe
	rm -f *.so

test_degenerated: a.out
	./a.out 2 7 7 0 a.txt
	./a.out 2 7 7 0 ae40.txt
	./a.out 2 7 7 0 ae-40.txt
	./a.out 2 7 7 0 dgnrtd.txt

test: a.out
	./a.out 12 7 7 4
	./a.out 20 7 7 1
	./a.out 8000 100 7 1


test_big: a.out
	date > summary.txt
	echo -------------------16000-------------------- >> summary.txt
	./a.out 16000 50 0 1 >> summary.txt
	./a.out 16000 100 0 1 >> summary.txt
	./a.out 16000 150 0 1 >> summary.txt
	echo --------f2------
	./a.out 16000 50 0 2 >> summary.txt
	./a.out 16000 100 0 2 >> summary.txt
	./a.out 16000 150 0 2 >> summary.txt
	echo --------f3------
	./a.out 16000 50 0 3 >> summary.txt
	./a.out 16000 100 0 3 >> summary.txt
	./a.out 16000 150 0 3 >> summary.txt
	date >> summary.txt 
	cp summary.txt summary_heavy.txt

	
	
