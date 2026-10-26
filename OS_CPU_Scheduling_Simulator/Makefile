CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = cpu_scheduler

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
