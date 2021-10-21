# -------------------------------------Files------------------------------------
SrcDir  = src

BinDir = bin
IntDir = obj
IncludeDir = src/include

Deps = $(wildcard $(SrcDir)/include/*.h)

CppSrc = $(notdir $(wildcard $(SrcDir)/*.cpp))

Objs = $(addprefix $(IntDir)/, $(CppSrc:.cpp=.o))

# ------------------------------------Options-----------------------------------
LXXFLAGS = $(shell pkg-config --libs sdl2) -lSDL2_ttf
CXXFLAGS = -I $(IncludeDir) -std=c++2a -O2 -Wall


# ----------------------------------Make rules----------------------------------
.PHONY: programm
programm: $(BinDir)/a.out


$(BinDir)/a.out: $(Objs)
	$(CXX) -o $(BinDir)/a.out $(Objs) $(LXXFLAGS)

vpath %.cpp $(SrcDir) $(TestDir)
$(IntDir)/%.o: %.cpp $(Deps) Makefile
	$(CXX) -c $< $(CXXFLAGS) -o $@

.PHONY: init
init:
	mkdir -p bin obj

.PHONY: clean
clean:
	rm -rf $(IntDir)/* $(BinDir)/*
# ----------------------------------Make rules----------------------------------