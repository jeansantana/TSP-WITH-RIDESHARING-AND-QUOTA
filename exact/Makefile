# g++ -std=c++11 -O3 $1.cpp -o $1 
# -I/home/jeansilva/gurobi652/linux64/include/ -L/home/jeansilva/gurobi652/linux64/lib/ -lgurobi_c++ -lgurobi65

CC = g++
CFLAGS = -std=c++11 -O3
SOURCES = TSPRQ_EXACT.cpp
GRBFLAGS = -I/home/jeansilva/gurobi652/linux64/include/ -L/home/jeansilva/gurobi652/linux64/lib/ -lgurobi_c++ -lgurobi65
EXECUTABLE = TSPRQ_EXACT

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE):
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE).run $(GRBFLAGS)

clean:
	$(RM) $(EXECUTABLE)
