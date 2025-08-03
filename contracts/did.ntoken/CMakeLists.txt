add_contract(did.ntoken did.ntoken ${CMAKE_CURRENT_SOURCE_DIR}/src/did.ntoken.cpp)

target_include_directories(did.ntoken
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(did.ntoken
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/did.ntoken.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/did.ntoken.contracts.md @ONLY )

target_compile_options( did.ntoken PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
