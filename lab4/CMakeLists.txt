add_executable(lab4 task.cpp)

set_property(TARGET lab4 PROPERTY CXX_SCAN_FOR_MODULES ON)

target_sources(lab4
    PRIVATE
        FILE_SET modules
        TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_SOURCE_DIR}/modules
        FILES
            ${CMAKE_SOURCE_DIR}/modules/graphs.cppm
)
