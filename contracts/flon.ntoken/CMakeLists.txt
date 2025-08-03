add_contract(flon.ntoken flon.ntoken ${CMAKE_CURRENT_SOURCE_DIR}/src/flon.ntoken.cpp)

target_include_directories(flon.ntoken
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(flon.ntoken
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/flon.ntoken.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/flon.ntoken.contracts.md @ONLY )

target_compile_options( flon.ntoken PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
