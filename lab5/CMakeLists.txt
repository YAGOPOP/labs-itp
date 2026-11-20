include(FetchContent)

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)

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
        BASE_DIRS
            ${CMAKE_SOURCE_DIR}/modules
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            ${CMAKE_SOURCE_DIR}/modules/graphs.cppm
            ${CMAKE_CURRENT_SOURCE_DIR}/maze_visualization.cppm
)

target_link_libraries(lab5 PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

add_executable(lab5-test test.cpp)

set_property(TARGET lab5-test PROPERTY CXX_SCAN_FOR_MODULES ON)

target_sources(lab5-test
    PRIVATE
        FILE_SET modules
        TYPE CXX_MODULES
        BASE_DIRS
            ${CMAKE_SOURCE_DIR}/modules
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            ${CMAKE_SOURCE_DIR}/modules/graphs.cppm
            ${CMAKE_CURRENT_SOURCE_DIR}/maze_visualization.cppm
)

target_link_libraries(lab5-test PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
