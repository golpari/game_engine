CXX := clang++
CXXFLAGS := -std=c++17 -O2
INCLUDES := -I./ThirdParty -I./ThirdParty/glm-0.9.9.8 -I./ThirdParty/rapidjson -I./include/SDL2 -I./include/SDL2_image -I./include/SDL2_mixer -I./include/SDL2_ttf -I./include

SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
TARGET := game_engine_linux

# names of libraries
LIBS := -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image

# Specify the path to the SDL2 libraries
LDFLAGS := -L./lib $(LIBS)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
