include(FetchContent)

# Отключаем ненужные компоненты SFML
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)

# Загружаем SFML 2.6.2
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SFML)

add_executable(lab5 task.cpp)

set_property(TARGET lab5 PROPERTY CXX_SCAN_FOR_MODULES ON)

target_sources(lab5
    PRIVATE
        FILE_SET modules
        TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_SOURCE_DIR}/modules
        FILES ${CMAKE_SOURCE_DIR}/modules/graphs.cppm
)

# Подключаем графику, окна и системные функции SFML
target_link_libraries(lab5 PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
