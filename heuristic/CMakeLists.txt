cmake_minimum_required(VERSION 3.5)
project(heuristic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(SOURCE_FILES heuristic.cpp ../commons/LKHParser.h ../commons/LKHParser.cpp ../commons/FileHelper.h ../commons/FileHelper.cpp)
add_executable(heuristic ${SOURCE_FILES})

if (GUROBI_INCLUDE_DIR)
# in cache already
set(GUROBI_FOUND TRUE)
set(GUROBI_INCLUDE_DIRS "${GUROBI_INCLUDE_DIR}" )
set(GUROBI_LIBRARIES "${GUROBI_LIBRARY};${GUROBI_CXX_LIBRARY}" )
else (GUROBI_INCLUDE_DIR)

find_path(GUROBI_INCLUDE_DIR
    NAMES gurobi_c++.h
    PATHS  "/home/jeansilva/gurobi652/linux64/include"
            "$ENV{GUROBI_HOME}/include"
    #CMAKE_ROOT_PATH "/home/jeansilva/gurobi652/linux64"
    )

find_library( GUROBI_LIBRARY
        NAMES gurobi
    gurobi45
    gurobi46
  gurobi50
  gurobi51
  gurobi52
  gurobi55
  gurobi56
  gurobi60
  gurobi65
  libgurobi_c++.a
  libgurobi60.so
        PATHS "$ENV{GUROBI_HOME}/lib"
              "/home/jeansilva/gurobi652/linux64/lib"
        )

find_library( GUROBI_CXX_LIBRARY
        NAMES gurobi_c++
        libgurobi65.so
        libgurobi_c++.a
        PATHS "$ENV{GUROBI_HOME}/lib"
              "/Library/gurobi604/mac64/lib"
              "C:\\libs\\gurobi604\\lib"

        )

set(GUROBI_INCLUDE_DIRS "${GUROBI_INCLUDE_DIR}" )
set(GUROBI_LIBRARIES "${GUROBI_LIBRARY};${GUROBI_CXX_LIBRARY}" )


endif(GUROBI_INCLUDE_DIR)

include_directories(${GUROBI_INCLUDE_DIR})
target_link_libraries(heuristic ${GUROBI_CXX_LIBRARY})
target_link_libraries(heuristic ${GUROBI_LIBRARY})