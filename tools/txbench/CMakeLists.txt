set(TXBENCH_SOURCES
        main.cpp
        )

add_executable(txbench ${TXBENCH_SOURCES})
target_include_directories(txbench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(txbench txbench-base)
