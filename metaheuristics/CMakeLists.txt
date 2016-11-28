cmake_minimum_required(VERSION 3.5)
project(METAHEURISTIC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp ../../commons/FileHelper.cpp ../../commons/FileHelper.h ../../commons/LKHParser.cpp ../../commons/LKHParser.h genetic/Chromosome.cpp genetic/Chromosome.h common/macros.h model/Solution.cpp model/Solution.h common/Shared.cpp common/Shared.h common/MersenneTwister.cpp common/MersenneTwister.h genetic/GeneticAlgorithm.cpp genetic/GeneticAlgorithm.h)
add_executable(METAHEURISTIC ${SOURCE_FILES})