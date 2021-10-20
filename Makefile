# ------------------------------------Options-----------------------------------
LXXFLAGS = $(shell pkg-config --libs sdl2) -lSDL2_ttf
CXXFLAGS = -std=c++2a -O2 $(shell pkg-config --cflags sdl2) -lSDL2_ttf -Wall
DEBUG_CXXFLAGS = -g -std=c++2a -O0 $(shell pkg-config --cflags sdl2) -Wall -D DEBUG
# ------------------------------------Options-----------------------------------

# -------------------------------------Files------------------------------------
SrcDir  = src

BinDir = bin
IntDir = obj

Deps = $(wildcard $(SrcDir)/*.h)\
       $(wildcard $(TestDir)/*.h)

CppSrc = $(notdir $(wildcard $(SrcDir)/*.cpp))\

Objs = $(addprefix $(IntDir)/, $(CppSrc:.cpp=.o))
# -------------------------------------Files------------------------------------

# ----------------------------------Make rules----------------------------------
.PHONY: programm
programm: $(BinDir)/a.out

.PHONY: debug
debug: DBG_$(BinDir)/a.out


$(BinDir)/a.out: $(IntDir)/main.o $(IntDir)/App.o $(IntDir)/Vector.o $(IntDir)/CoordSystem.o $(IntDir)/GraphicLib.o $(IntDir)/CollisionManager.o $(IntDir)/PhysShapes.o $(IntDir)/CollisionReaction.o $(IntDir)/MoleculeBox.o $(IntDir)/PlotMoleculeCounter.o $(IntDir)/Button.o
	$(CXX) -o $(BinDir)/a.out $(IntDir)/main.o $(IntDir)/CollisionManager.o $(IntDir)/App.o $(IntDir)/Vector.o $(IntDir)/CoordSystem.o $(IntDir)/GraphicLib.o $(IntDir)/PhysShapes.o $(IntDir)/CollisionReaction.o $(IntDir)/MoleculeBox.o $(IntDir)/PlotMoleculeCounter.o $(IntDir)/Button.o $(LXXFLAGS)

# DBG_$(BinDir)/a.out: DBG_$(IntDir)/main.o DBG_$(IntDir)/App.o DBG_$(IntDir)/Vector.o DBG_$(IntDir)/CoordSystem.o DBG_$(IntDir)/GraphicLib.o DBG_$(IntDir)/CollisionManager.o DBG_$(IntDir)/PhysShapes.o DBG_$(IntDir)/CollisionReaction.o
#     $(CXX) -o $(BinDir)/a.out $(IntDir)/main.o $(IntDir)/App.o $(IntDir)/Vector.o $(IntDir)/CoordSystem.o $(IntDir)/GraphicLib.o $(IntDir)/Circle.o $(IntDir)/CollisionManager.o $(IntDir)/PhysShapes.o $(IntDir)/CollisionReaction.o $(LXXFLAGS)

vpath %.cpp $(SrcDir) $(TestDir)
$(IntDir)/%.o: %.cpp $(Deps) Makefile
	$(CXX) -c $< $(CXXFLAGS) -o $@

DBG_$(IntDir)/%.o: %.cpp $(Deps) Makefile
	$(CXX) -c $< $(DEBUG_CXXFLAGS) -o $@

.PHONY: init
init:
	mkdir -p bin obj

.PHONY: clean
clean:
	rm -rf $(IntDir)/* $(BinDir)/*
# ----------------------------------Make rules----------------------------------