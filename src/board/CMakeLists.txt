add_library(board
    src/board.cpp
    include/board.hpp
)

target_include_directories(board PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(board PUBLIC utils)

if(COMPILE_TESTS)
    add_subdirectory(test)
endif()
