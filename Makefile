#Yasmine Brooks
#Dr.Pounds
#Project 2
#November 10, 2018

CC = gcc
C++ = g++
LIBDIRS = -L/usr/lib64
INCDIRS = -I/usr/include
LDLIBS = -lglut -lGL -lGLU -lX11 -lm

testFile:
	$(C++) -o testFile testFile.o $(INCDIRS) $(LIBDIRS) $(LDLIBS)


