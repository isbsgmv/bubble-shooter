add_library(projectile
    src/projectile.cpp
    src/trajectory.cpp
    src/collision.cpp
    include/projectile.hpp
    include/trajectory.hpp
    include/collision.hpp
    include/geometry.hpp
)

target_include_directories(projectile PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/utils/)

target_link_libraries(projectile PUBLIC board renderer)

if(COMPILE_TESTS)
    add_subdirectory(test)
endif()
