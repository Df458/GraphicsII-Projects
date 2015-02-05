CXX=i686-w64-mingw32-g++
SRC=$(wildcard *.cpp) $(wildcard 3DClasses/*.cpp)
OBJ=$(patsubst %.cpp,obj/%.o,$(SRC))
DEP=$(patsubst %.cpp,obj/%.depend,$(SRC))
CXXFLAGS=-g -std=c++11 -pedantic -Wall -Wno-comment -static-libgcc
CXXLIBS=-static -ldinput8 -ldxguid -ld3d9 -ld3dx9 -lgdi32 -luser32
obj/%.depend: $(SRCPATH)%.cpp
	@echo -e "Building dependecies for \e[1;35m$<\e[0m..."
	@set -e; rm -f $@; \
	$(CXX) -M $(CXXFLAGS) $(OSFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

obj/%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXLIBS)

all: $(DEP) $(OBJ)
	$(CXX) -o game.exe $(CXXFLAGS) $(OBJ) $(CXXLIBS) 

-include $(patsubst %.cpp,obj/%.depend,$(SRC))

clean:
	rm -f $(OBJ) $(DEP)
	rm -f game.exe

$(shell mkdir -p obj)

