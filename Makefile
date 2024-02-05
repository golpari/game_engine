CXX := clang++
CXXFLAGS := -std=c++17 -O2
INCLUDES := -I./ThirdParty -I./ThirdParty/glm-0.9.9.8 -I./ThirdParty/rapidjson

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
TARGET := game_engine_linux

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
