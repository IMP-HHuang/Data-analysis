OBJ = GUI

CXX=g++        
CXXFLAGS:=-g -Wall -lrt  -I. 
#-std=c++11 
ROOTLIBS = $(shell root-config --libs) -lGui -lSpectrum #-lRHTTP
ROOTINC = -I$(shell root-config --incdir) 
CXXFLAGS+=$(ROOTINC)
LIBS:=

#INCLUDES:= $(wildcard $(shell pwd)/*.h)
INCLUDES:= gui.h
#SRCS:= $(wildcard $(shell pwd)/*.cpp)
#OBJECTS:=$(SRCS:.cpp=.o) 
#OBJECTS:= main.o gui.o MainDirct.o
SRCS:= $(wildcard $(shell pwd)/*.cpp)
SRCS+= MainDirct.cpp
OBJECTS:=$(SRCS:.cpp=.o) 

all: $(OBJ)


$(OBJ): $(OBJECTS)
	$(CXX) -o  $@  $(OBJECTS) $(CXXFLAGS) $(LIBS) $(ROOTLIBS)

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) -c $< 

MainDirct.cpp: $(INCLUDES) LinkDef.h
	rootcint -f $@ -c $^


#enviroment:
#	./enviroment.sh

clean: 
	rm -f *.o *~ $(OBJ) *Dirct*

very-clean:
	rm -f $(OBJ) *.o *~

.PHONY: clean very-clean
#.SILENT:
