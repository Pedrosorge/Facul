CXX = g++
CXXFLAGS = -Wall 
TARGET = teste

SRCS = $(shell find . -name "*.cpp")

all: 
	$(CXX) $(SRCS) -o $(TARGET)  
	clear
	./$(TARGET)

clean:
	rm -f $(TARGET)