add_executable(lab2 task.cpp)
add_executable(debugging debugging.cpp)

add_library(liblab2 liblab2.cpp)

target_link_libraries(lab2 PRIVATE liblab2)
target_link_libraries(debugging PRIVATE liblab2)
