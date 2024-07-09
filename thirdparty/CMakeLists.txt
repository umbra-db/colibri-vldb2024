add_library(picojson INTERFACE)
target_compile_definitions(picojson INTERFACE PICOJSON_USE_INT64)
target_include_directories(picojson INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(duckdb STATIC IMPORTED)
set_target_properties(duckdb PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/duckdb/libduckdb.a)
