CC=g++
CFLAGS=-std=c++11 -c -O3
LDFLAGS=
SOURCES=teste.cpp FileHelper.cpp FileHelper.h LKHParser.cpp LKHParser.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=teste

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -f $(EXECUTABLE) *.o
