add_executable(lab5 task.cpp)

set_property(TARGET lab5 PROPERTY CXX_SCAN_FOR_MODULES ON)

target_sources(lab5
    PRIVATE
        FILE_SET modules
        TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_SOURCE_DIR}/modules
        FILES
            ${CMAKE_SOURCE_DIR}/modules/graphs.cppm
)
