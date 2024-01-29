CXX := clang++
CXXFLAGS := -std=c++17 -O2
INCLUDES := -IThird\ Party -IThird\ Party/glm-0.9.9.8

SOURCES := Main.cpp
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
