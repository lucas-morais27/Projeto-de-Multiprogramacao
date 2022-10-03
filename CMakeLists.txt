cmake_minimum_required(VERSION 3.5)

project(processos-threads)

include_directories( include )

add_executable( auxiliar
                src/Auxiliar.cpp)
add_executable( sequencial
                src/Sequencial.cpp)
add_executable( processos
                src/Processos.cpp)
add_executable( threads
                src/Threads.cpp)
                
target_link_libraries(threads pthread)