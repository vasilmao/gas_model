# ------------------------------------Options-----------------------------------
LXXFLAGS = $(shell pkg-config --libs sdl2)
CXXFLAGS = -std=c++2a -O2 $(shell pkg-config --cflags sdl2) -Wall
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


$(BinDir)/a.out: $(IntDir)/main.o $(IntDir)/App.o $(IntDir)/Vector.o $(IntDir)/CoordSystem.o $(IntDir)/Renderer.o $(IntDir)/CollisionManager.o $(IntDir)/PhysShapes.o $(IntDir)/ChemicalReaction.o
	$(CXX) -o $(BinDir)/a.out $(IntDir)/main.o $(IntDir)/CollisionManager.o $(IntDir)/App.o $(IntDir)/Vector.o $(IntDir)/CoordSystem.o $(IntDir)/Renderer.o $(IntDir)/PhysShapes.o $(IntDir)/ChemicalReaction.o $(LXXFLAGS)

DBG_$(BinDir)/a.out: DBG_$(IntDir)/main.o DBG_$(IntDir)/App.o DBG_$(IntDir)/Vector.o DBG_$(IntDir)/CoordSystem.o DBG_$(IntDir)/Renderer.o DBG_$(IntDir)/CollisionManager.o DBG_$(IntDir)/PhysShapes.o DBG_$(IntDir)/ChemicalReaction.o
	$(CXX) -o $(BinDir)/a.out $(IntDir)/main.o $(IntDir)/App.o $(IntDir)/Vector.o $(IntDir)/CoordSystem.o $(IntDir)/Renderer.o $(IntDir)/Circle.o $(IntDir)/CollisionManager.o $(IntDir)/PhysShapes.o $(IntDir)/ChemicalReaction.o $(LXXFLAGS)

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
	rm -rf $(IntDir) $(BinDir)
# ----------------------------------Make rules----------------------------------