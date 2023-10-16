LIBS = -lm
CFLAGS = -Wall -O1
CC = g++

EXE = target/main


SRC_FILES = main.cpp \
	fat32.cpp \
	boot_sector.cpp \
	utils.cpp \
	device.cpp \
	file_system.cpp \
	file.cpp \
	device_formater.cpp \
	shell.cpp \

COMPILED_FILES = $(foreach item,$(SRC_FILES:.cpp=.o) ,target/build/src/$(item))

target/build/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<

build: $(COMPILED_FILES)
	$(CC) $^ -o $(EXE) $(LIBS)

clean:
	rm -r target

run: build
	./$(EXE)

all: clean build run