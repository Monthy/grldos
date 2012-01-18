# Makefile para GRlDOs
#
# Pedro A. Garcia Rosado, aka Monthy
# 
# http://www.gr-lida.org/
# ermonthy@gmail.com
# Marzo 07 2011
#

CC = gpp
CFLAGS  = -Wall -O2 -c
LDFLAGS = -Wall -O2 -ffast-math
LIBS    = -lalfont -ladime -lalgif -ljpgal -lldpng -lpng -lz -lalleg -lReadINI
INCS    =

SRCS = main.cpp func.cpp grldos.cpp xml/xmlparser.cpp
OBJS = $(SRCS:.cpp=.o)
EXE  = grldos.exe

all: $(SRCS) $(EXE)
	
$(EXE): $(OBJS) 
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	-rm -f $(OBJS) $(EXE)
