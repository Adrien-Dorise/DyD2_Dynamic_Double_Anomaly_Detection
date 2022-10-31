#
# This is the MakeFile for Dynamic Double Anomaly Detection (DyD2) program.
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings

# The C compiler is gcc
CFLAGS  = -g -Wall


default : DyD2

DyD2 : main.o DyD2.o uCluster.o math_HCE.o utility.o inout.o
	gcc $(CFLAGS) main.o DyD2.o uCluster.o math_HCE.o utility.o inout.o -o DyD2


%.o : sources/%.c 
	gcc -c $<

main.o : sources/parameters.h sources/DyD2.h sources/inout.h
DyD2.o : sources/DyD2.h sources/parameters.h sources/uCluster.h sources/inout.h
uCluster.o : sources/uCluster.h sources/math_HCE.h
math_HCE.o : sources/math_HCE.h sources/utility.h
utility.o : sources/utility.h sources/parameters.h
inout.o : sources/inout.h sources/parameters.h sources/uCluster.h

# To start over from scratch, type 'make clean'.
clean: 
	rm *.o DyD2.exe