# ALGORITHMS FOR THE TRAVELING SALESMAN PROBLEM WITH RIDESHARING AND QUOTA

## Algorithms List

### Instructions

This project uses gurobi version 6.5, so the cmake files uses this version. If you want to use another version you must to change each `CMakeLists.txt` file on the folders, for instance: `exact/CMakeLists.txt`.

To run the code, you have to run the command `cmake .` on the folder that contains the 
`CMakeLists.txt` file.

### 1. Exact

That algorithm uses gurobi solver. The math model was implemented here.
    

### 2. Heuristic
	
This algorithm uses LKH 2.0.7 by Helsgaun, a Lin-Kernighan Traveling Salesman Heuristic implementation, to obtain a route R. Then the gurobi solver is used to get the best embark configuration for R.

### 3. Metaheuristics

#### 3.1 Genetic

#### 3.2 Memetic

#### 3.3 Transgenetic

### 4. How to use

The genetic and memectic algorithms are in the folder metaheuristics. There we have a `main.cpp` file. To compile we have to go to the `build` folder and run `cmake ..` and then `make`. After tha we have an executable file called `METAHEURISTIC` and we can run it. This executable have some parameters:

1. Entry file
2. Params to run:
3. Population Size
4. Number of avaliations
5. Crossover rate
6. Global mutation rate
7. Algorithm type:
	
	7.1 - 1 to use Genetic Algorithm
	
	7.2 - 2 to use Memetic Algorithm

8. Genetic operator for crossover/recombination:

	8.1 - 1 to use split and recombiantion operator

	8.2 - 2 to use the SCX operator

	8.3 - 3 to user the SCX based operator
	
Example: `./executable instances/big.in 100 200 0.5 0.6 20 1 1`