# Makefile

# 定义编译器和编译选项
GCC := g++
#GCC := gcc
CFLAGS :=  -Wextra -std=c++11
#CFLAGS := -Wall -Wextra -std=c++11
#CFLAGS := -Wall -std=c++11
#CFLAGS := -std=c++11

export $(UCESBDIR)
RootLIBS := $(shell root-config --libs --cflags)
RootLIBS2 := $(shell root-config --libs) -lGui -lRHTTP -lrt

SOURCES = $(wildcard *.cpp)
SOURCES += $(wildcard *.h)



DESPEC: $(SOURCES) LinkDict.cpp
	$(GCC) $(CFLAGS)  $^ -o $@  $(RootLIBS) $(RootLIBS2)


LinkDict.cpp : Data.h Linkdef.h 
	    #root 6
		rootcling -f LinkDict.cpp  $^ 
	    # root 5
		#rootcint -f LinkDict.cpp -c $^

clean:
	rm -rf $(OBJS) DESPEC *Dict*
