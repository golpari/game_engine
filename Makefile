CXX = clang++
CXXFLAGS = -std=c++17 -O2
INCDIRS = -I./Third\ Party/glm-0.9.9.8 -I./Third\ Party

.PHONY: all clean

all: game_engine_linux

game_engine_linux:
	$(CXX) $(CXXFLAGS) $(INCDIRS) *.cpp -o $@

clean:
	rm -f game_engine_linux *.o
