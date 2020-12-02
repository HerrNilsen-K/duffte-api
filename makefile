cc = clang++
appName = program.exe

sources = $(wildcard */*/*/*/*.cpp) $(wildcard */*/*/*.cpp) $(wildcard */*/*.cpp) $(wildcard */*.cpp) $(wildcard *.cpp)
#sources = $(shell find . -regex '.*\.\(cpp\|h\)')
objects = $(patsubst %.cpp, %.o, $(sources))

flags = -g -Wall -std=c++17
libPath = -L"C:\\Users\\Karl\\Desktop\\c++\\libraries\\GLFW\\lib-vc2019" \
		-L"C:\\Users\\Karl\\Desktop\\c++\\libraries\\GLEW\\lib\\Release\\x64" 
libs = -lglew32 -lglfw3 -lopengl32 -lglu32 -lShell32 -lmsvcrt -lgdi32 -lUser32 -lkernel32
VPATH := $(sort $(dir .))

#Compile, link and execute the program
all: program.exe run

program.exe: $(objects)
	$(cc) -o $(appName) $^ $(libPath) $(libs)
  
%.o : %.cpp
	$(cc) $(flags) -c -o $@ $<
  
#Delete all object files
#WARNING! The whole project needs to be recompiled after this
clean:
	del /s *.o

.PHONY: all
  
.PHONY run:
	./$(appName)