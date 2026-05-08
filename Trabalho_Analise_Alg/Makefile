CXX = g++
CXXFLAGS = -Wall -g
TARGET = teste

SRCS = $(shell find . -name "*.cpp")

all: 
	$(CXX) $(SRCS) -o $(TARGET)  
	clear
	./$(TARGET) 1> relatorio_testes.txt 2> erros_relatorio.txt

compileall:
	$(CXX) $(SRCS) -o $(TARGET)  


clean:
	rm -f $(TARGET)