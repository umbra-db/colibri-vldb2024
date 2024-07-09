set(TXBENCH_DATAGEN_SOURCES
        generator/HattrickGenerator.cpp
        generator/HyBenchGenerator.cpp
        generator/TatpGenerator.cpp
        generator/TpccGenerator.cpp
        generator/TpcchGenerator.cpp
        main.cpp
        )

add_executable(txbench-datagen ${TXBENCH_DATAGEN_SOURCES})
target_include_directories(txbench-datagen PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(txbench-datagen txbench-base)
