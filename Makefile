TARGET := BeSqlitEd

CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -pedantic \
	-Isrc/app \
	-Isrc/core \
	-Isrc/ui

LIBS := -lbe -lsqlite3

SOURCES := \
	src/app/BeSqlitEdApp.cpp \
	src/ui/MainWindow.cpp \
	src/core/Database.cpp \
	src/core/SchemaReader.cpp

OBJECTS := $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean
