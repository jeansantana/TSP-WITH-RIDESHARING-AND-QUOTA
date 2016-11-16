cmake_minimum_required(VERSION 3.5)
project(transgenetic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp macros.h mersennetwister.h endossimbionte.cpp endossimbionte.h vetor_transgenetico.cpp vetor_transgenetico.h plasmideo.cpp plasmideo.h virus.cpp virus.h PCVPQ.cpp PCVPQ.h mersennetwister.cpp mersennetwister.h solucao.cpp solucao.h plasmideo_recombinado.cpp plasmideo_recombinado.h transposon.cpp transposon.h transgenetic.cpp transgenetic.h)
add_executable(transgenetic ${SOURCE_FILES})