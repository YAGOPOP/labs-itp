add_executable(lab1_task1 task1.cpp)
add_executable(lab1_task2 task2.cpp)


add_custom_target(lab1)
add_dependencies(lab1 lab1_task1 lab1_task2)
