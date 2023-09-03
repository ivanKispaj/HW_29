CPP = g++ # компилятор
FLAGS = -std=c++2a -stdlib=libc++ -fdiagnostics-color=always  # флаги компиляции
TARGETSRC = ./*.cpp 
TARGET = queue.exe # цель -> исполняемый файл

all: 	$(TARGETSRC)
		$(CPP) $(FLAGS) -g $(TARGETSRC) -o  ./$(BIN_DIR)$(TARGET)

		