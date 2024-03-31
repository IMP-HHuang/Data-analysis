# Makefile

# 定义编译器和编译选项
GCC := g++
CFLAGS :=  -Wextra -std=c++11
#CFLAGS := -Wall -Wextra -std=c++11

OBJ = DESPEC
MainFile = $(OBJ).cpp
LinkFile = Linkdef.h
UserDict = LinkDict.cpp


SourceFile := $(wildcard $(shell pwd)/src/*.cpp)
IncludeFile := $(wildcard $(shell pwd)/include/*.h)
DIR_INC := -I $(shell pwd)/include

export $(UCESBDIR)
RootLIBS := $(shell root-config --libs --cflags)
RootLIBS2 := $(shell root-config --libs) -lGui -lRHTTP -lrt

all:$(OBJ)
$(OBJ): $(MainFile) $(SourceFile) $(IncludeFile)  $(UserDict)
	$(GCC) $(CFLAGS)  $^ -o $@  $(RootLIBS) $(RootLIBS2) $(DIR_INC) 


$(UserDict): $(IncludeFile) $(LinkFile)
	#root 6
	rootcint -f $@  $^
	# root 5
	#rootcint -f $@ -c $^

clean:
	rm -rf $(OBJ) *Dict*
